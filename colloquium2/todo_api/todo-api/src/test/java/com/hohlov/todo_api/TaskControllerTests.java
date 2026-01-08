package com.hohlov.todo_api;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.hamcrest.Matchers.hasSize;

@SpringBootTest
class TaskControllerTests {

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Autowired
    private TaskRepository taskRepository;

    private MockMvc mockMvc;

    @BeforeEach
    void setUp() {
        taskRepository.deleteAll();
        this.mockMvc = MockMvcBuilders.webAppContextSetup(webApplicationContext).build();
    }

    @Test
    void testPatchPreserve() throws Exception {
        //PATCH preservation test: check if fields not included in request are kept in DB
        String postJson = """
                {
                 "title": "Купить молоко",
                 "description": "Обязательно 3.2% жирности",
                 "status": "todo"
                }
                """;
        String response = mockMvc.perform(post("/tasks").contentType(MediaType.APPLICATION_JSON)
                        .content(postJson))
                .andReturn().getResponse().getContentAsString();
        String patchJson = "{\"status\": \"done\"}";
        mockMvc.perform(patch("/tasks/" + 1L)
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(patchJson))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.status").value("done"))
                .andExpect(jsonPath("$.description").value("Обязательно 3.2% жирности"))
                .andExpect(jsonPath("$.title").value("Купить молоко"));
    }

    @Test
    void testPutIgnoreId() throws Exception {
        //PUT ID protection test: check if ID from URL overrides ID from JSON body
        String postJson = """
                {
                 "title": "Запустить API",
                 "description": "Сделать тесты",
                 "status": "todo"
                }
                """;
        mockMvc.perform(post("/tasks")
                .contentType(MediaType.APPLICATION_JSON)
                .content(postJson));
        String putJson = """
                {
                 "id": 999,
                 "title": "Запустить API",
                 "description": "Тесты сделаны",
                 "status": "in_progress"
                }
                """;
        mockMvc.perform(put("/tasks/" + 1L)
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(putJson))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(1L))
                .andExpect(jsonPath("$.status").value("in_progress"));
    }

    @Test
    void testNotFoundResponse() throws Exception {
        //Negative GET test: attempt to get a task by a deliberately non-existent number
        long id = 9999L;
        mockMvc.perform(get("/tasks/" + id))
                .andExpect(status().isNotFound())
                .andExpect(jsonPath("$.timestamp").exists())
                .andExpect(jsonPath("$.status").value(404))
                .andExpect(jsonPath("$.error").value("Not Found"))
                .andExpect(jsonPath("$.message").value("Task with ID " + id + " not found"));
    }

    @Test
    void testFullTaskLifecycle() throws Exception {
        //POST test
        String postJson1 = """
                {
                 "title": "Купить молоко",
                 "description": "Обязательно 3.2% жирности",
                 "status": "todo"
                }
                """;
        mockMvc.perform(post("/tasks")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(postJson1))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.title").value("Купить молоко"));
        String postJson2 = """
                {
                 "title": "Запустить API",
                 "description": "Сделать тесты",
                 "status": "todo"
                }
                """;
        mockMvc.perform(post("/tasks")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(postJson2))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.title").value("Запустить API"));

        //GET all tasks test
        mockMvc.perform(get("/tasks"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$", hasSize(2)))
                .andExpect(jsonPath("$[0].title").value("Купить молоко"))
                .andExpect(jsonPath("$[1].title").value("Запустить API"));

        //GET task by ID test
        mockMvc.perform(get("/tasks/2"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.title").value("Запустить API"))
                .andExpect(jsonPath("$.description").value("Сделать тесты"));

        //PUT test
        String putJson = """
                {
                 "title": "Запустить API",
                 "description": "...",
                 "status": "in_progress"
                }
                """;
        mockMvc.perform(put("/tasks/2")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(putJson))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.title").value("Запустить API"))
                .andExpect(jsonPath("$.status").value("in_progress"));

        //PATCH test
        String patchJson = "{\"status\": \"done\"}";
        mockMvc.perform(patch("/tasks/2")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(patchJson))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.status").value("done"))
                .andExpect(jsonPath("$.title").value("Запустить API"));

        //DELETE test
        mockMvc.perform(delete("/tasks/1")).andExpect(status().isOk());
        mockMvc.perform(delete("/tasks/2")).andExpect(status().isOk());
    }
}