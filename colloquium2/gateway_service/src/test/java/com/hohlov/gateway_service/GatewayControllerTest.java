package com.hohlov.gateway_service;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.springframework.security.test.web.servlet.setup.SecurityMockMvcConfigurers.springSecurity;

@SpringBootTest
class GatewayControllerTest {

    @Autowired
    private WebApplicationContext webApplicationContext;

    private MockMvc mockMvc;

    @BeforeEach
    void setUp() {
        this.mockMvc = MockMvcBuilders.webAppContextSetup(webApplicationContext).apply(springSecurity())
                .build();
    }

    //Rate limit test
    @Test
    @WithMockUser(username = "admin")
    void shouldEnforceRateLimit() throws Exception {
        boolean limit = false;
        for (int i = 0; i < 12; ++i) {
            if (mockMvc.perform(get("/tasks")).andReturn().getResponse().getStatus() == 429) {
                limit = true;
                break;
            }
        }
        if (!limit) {
            throw new AssertionError("Rate limit was not reached after 12 requests");
        }
    }

    // Proxy POST test
    @Test
    @WithMockUser(username = "admin")
    void shouldPost() throws Exception {
        String taskJson = """
            {
             "title": "Buy milk",
             "description": "3.2% fat is required",
             "status": "todo"
            }
            """;
        mockMvc.perform(post("/tasks").contentType(MediaType.APPLICATION_JSON)
                        .content(taskJson))
                .andExpect(status().isCreated());
    }

    // Proxy GET test
    @Test
    @WithMockUser(username = "admin")
    void shouldGet() throws Exception {
        mockMvc.perform(get("/tasks")).andExpect(status().isOk());
    }
}
