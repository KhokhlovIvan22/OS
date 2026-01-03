### Colloquim 2 - To-Do List REST API (Microservices Edition)

**A Spring Boot 4.0.1 application providing a RESTful API for task management. It implements CRUD operations using a SQLite database, includes basic caching, monitoring, logging tools and a secure Gateway layer for traffic control.**

### Features & Functionality
* **CRUD Logic**: Implementation of Create, Read, Update and Delete operations for task resources.
* **Database Integration**: Persistent storage using **SQLite** via **Spring Data JPA**.
* **Testing Interface**: Support for interactive endpoint testing via a web-based UI.
* **Performance**: **Spring Cache** usage for data retrieval optimization.
* **Monitoring**: Real-time system health and metrics via **Spring Boot Actuator**.
* **Logging**: Comprehensive event logging for request processing and database transactions using **SLF4J/Logback**.
* **Data Format**: Standardized **JSON** exchange with correct HTTP status code mapping.
* **Gateway & Security**: Integrated **API Gateway** with **Basic Authentication** and **Rate Limiting** (10 requests per minute) powered by **Bucket4j**.

### Technology Stack
* **Language**: Java 21
* **Framework**: **Spring Boot 4.0.1 (Web MVC)**
* **Security**: Spring Security & Spring Security Test
* **Traffic control**: Bucket4j Core
* **Database**: SQLite
* **Build system**: Maven 3.x
* **Libraries**: Lombok, Hibernate, SLF4J/Logback, RestTemplate

### Key Project Files
**Core service (`/todo_api` — Port: 8081):**
* **pom.xml** — Project dependencies and build configuration.
* **TodoApiApplication.java** — Application entry point; starts the embedded server.
* **Task.java** — Task entity class (ID, title, description, status).
* **TaskController.java** — REST controller for handling HTTP requests.
* **TaskRepository.java** — Repository interface for SQLite database operations.
* **application.properties** — Database and application settings.
* **TaskControllerTest.java** — Comprehensive integration test for the full task lifecycle (CRUD operations).
* **TodoApiApplicationTests.java** — Basic smoke test to verify application context loading.

**Gateway Service (`/gateway_service` — Port: 8080):**
* **pom.xml** — Configuration for Gateway-specific dependencies (Bucket4j, Security).
* **GatewayController.java** — Proxy controller with **Rate Limiting** and request routing to port 8081.
* **SecurityConfig.java** — Security layer configuration.
* **application.properties** — Server settings and proxy target configuration.
* **GatewayControllerTest.java** — Specialized tests for **Rate Limiting** (429 error) and Proxy logic.
* **GatewayServiceTests.java** — Context loading and security filter chain verification.

### API Endpoints (Access via Gateway :8080)
* **GET /tasks** — get all tasks.
* **POST /tasks** — create a new task.
* **GET /tasks/{id}** — get a task by ID.
* **PUT /tasks/{id}** — update a task by ID.
* **PATCH /tasks/{id}** — partially update task status.
* **DELETE /tasks/{id}** — delete a task by ID.

### Monitoring & Testing URLs
* **Swagger UI (Interactive API)**: [http://localhost:8080/swagger-ui/index.html](http://localhost:8080/swagger-ui/index.html)
* **Actuator Health (Backend)**: [http://localhost:8081/actuator/health](http://localhost:8081/actuator/health)
* **Actuator Metrics (Backend)**: [http://localhost:8081/actuator/metrics](http://localhost:8081/actuator/metrics)
