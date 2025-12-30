package com.hohlov.todo_api;

import io.swagger.v3.oas.annotations.Operation;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.*;

@RestController
@RequestMapping("/tasks")
public class TaskController {
    private final TaskRepository repository;

    private static final Logger logger = LoggerFactory.getLogger(TaskController.class);

    public TaskController(TaskRepository repository) {
        this.repository = repository;
        logger.info("TaskController is ready to go");
    }

    @Operation(summary="Get all tasks")
    @GetMapping
    public List<Task> getAll() {
        logger.info("GET: fetching all tasks");
        List<Task> tasks = repository.findAll();
        if (tasks.isEmpty()) {
            logger.info("The database is empty");
        } else {
            logger.info("Found {} task(s)", tasks.size());
        }
        return tasks;
    }

    @Operation(summary = "Create a new task")
    @PostMapping
    public ResponseEntity<Task> create(@RequestBody Task task) {
        logger.info("POST: creating new task '{}'", task.getTitle());
        Task saved = repository.save(task);
        logger.info("Task successfully saved with ID {}", saved.getId());
        return new ResponseEntity<>(saved, HttpStatus.CREATED);
    }

    @Operation(summary = "Find task by ID")
    @Cacheable(value = "tasks", key = "#id")
    @GetMapping("/{id}")
    public ResponseEntity<Task> get(@PathVariable Long id) {
        logger.info("GET: searching for task with ID {}", id);
        Task found = repository.findById(id).orElse(null);
        if (found == null) {
            logger.warn("Task with ID {} not found", id);
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        logger.info("Task with ID {} retrieved successfully", id);
        return new ResponseEntity<>(found, HttpStatus.OK);
    }

    @Operation(summary = "Replace entire task")
    @CacheEvict(value = "tasks", key = "#id")
    @PutMapping("/{id}")
    public ResponseEntity<Task> update(@PathVariable Long id, @RequestBody Task task) {
        logger.info("PUT: received update request for task with ID {}", id);
        Task found = repository.findById(id).orElse(null);
        if (found == null) {
            logger.warn("Task with ID {} not found", id);
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        task.setId(id);
        Task updated = repository.save(task);
        logger.info("Task with ID {} updated successfully", id);
        return new ResponseEntity<>(updated, HttpStatus.OK);
    }

    @Operation(summary = "Update task partially",
            description = "Send only the fields you want to update. You can remove other fields from the JSON body. " +
            "If a field is not provided, its value in the database will remain unchanged")
    @CacheEvict(value = "tasks", key = "#id")
    @PatchMapping("/{id}")
    public ResponseEntity<Task> patch(@PathVariable Long id, @RequestBody Task task) {
        logger.info("PATCH: check for changes in task with ID {}", id);
        Task found = repository.findById(id).orElse(null);
        if (found == null) {
            logger.warn("Task with ID {} not found", id);
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        boolean isChanged = false;
        if (task.getTitle() != null && !task.getTitle().equals(found.getTitle())) {
            logger.info("ID {} -> title changed", id);
            found.setTitle(task.getTitle());
            isChanged = true;
        }
        if (task.getStatus() != null && !task.getStatus().equals(found.getStatus())) {
            logger.info("ID {} -> status changed", id);
            found.setStatus(task.getStatus());
            isChanged = true;
        }
        if (task.getDescription() != null && !task.getDescription().equals(found.getDescription())) {
            logger.info("ID {} -> description updated", id);
            found.setDescription(task.getDescription());
            isChanged = true;
        }
        if (isChanged) {
            repository.save(found);
            logger.info("Task with ID {} changes saved successfully", id);
        } else {
            logger.info("Task with ID {} - nothing changed, skipping save", id);
        }
        return new ResponseEntity<>(found, HttpStatus.OK);
    }

    @Operation(summary = "Delete task")
    @CacheEvict(value = "tasks", key = "#id")
    @DeleteMapping("/{id}")
    public ResponseEntity<Void> delete(@PathVariable Long id) {
        logger.info("DELETE: request to remove task with ID {}", id);
        if (!repository.existsById(id)) {
            logger.warn("Task with ID {} not found, nothing to delete", id);
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        repository.deleteById(id);
        logger.info("Task with ID {} successfully deleted", id);
        return new ResponseEntity<>(HttpStatus.OK);
    }
}