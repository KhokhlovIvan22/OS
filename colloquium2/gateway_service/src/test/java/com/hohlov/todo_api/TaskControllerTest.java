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
class TaskControllerTest {

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

        //Negative GET test
        mockMvc.perform(get("/tasks/2")).andExpect(status().isNotFound());
    }
}