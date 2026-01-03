Colloquim 2 - To-Do List REST API
A Spring Boot application providing a RESTful API for task management. It implements CRUD operations using a SQLite database and includes basic caching, monitoring and logging tools.

Features & Functionality
CRUD Logic: Implementation of Create, Read, Update and Delete operations for task resources.
Database Integration: Persistent storage using SQLite via Spring Data JPA.
Testing Interface: Support for interactive endpoint testing via a web-based UI.
Performance: Spring Cache usage for data retrieval optimization.
Monitoring: System health and metrics via Spring Boot Actuator.
Logging: Comprehensive event logging for request processing and database transactions using SLF4J/Logback.
Data Format: Standardized JSON exchange with correct HTTP status code mapping.
Technology Stack
Language: Java 21
Framework: Spring Boot 4.0.1 (Web MVC)
Database: SQLite
Build System: Maven 3.x
Libraries: Lombok, Hibernate, SLF4J/Logback
Key Project Files
pom.xml — Project dependencies and build configuration.
ToDoApi.java — Application entry point; starts the embedded server.
Task.java — Task entity class (ID, title, description, status).
TaskController.java — REST controller for handling HTTP requests.
TaskRepository.java — Repository interface for SQLite database operations.
application.properties — Database and application settings.
TaskControllerTest.java — Comprehensive integration test for the full task lifecycle (CRUD operations).
TodoApiApplicationTests.java — Basic smoke test to verify application context loading.
API Endpoints
GET /tasks — get all tasks.
POST /tasks — create a new task.
GET /tasks/{id} — get a task by ID.
PUT /tasks/{id} — update a task by ID.
PATCH /tasks/{id} — partially update task status.
DELETE /tasks/{id} — delete a task by ID.
Testing Interface URL: http://localhost:8080/swagger-ui/index.html