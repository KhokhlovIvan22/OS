# Colloquium 2 - To-Do List REST API

**A Spring Boot application providing a RESTful API for task management. It implements CRUD operations using a SQLite database, includes basic caching, monitoring, logging tools, a secure Gateway layer for traffic control and full Docker support.**

## Features & Functionality
* **CRUD logic**: implementation of Create, Read, Update and Delete operations for task resources.
* **Database integration**: persistent storage using **SQLite** via **Spring Data JPA**.
* **Testing interface**: support for interactive endpoint testing via a web-based UI (Swagger UI).
* **Performance**: **Spring Cache** usage for data retrieval optimization.
* **Monitoring**: real-time system health and metrics via **Spring Boot Actuator**.
* **Logging**: comprehensive event logging for request processing and database transactions using **SLF4J/Logback**.
* **Data format**: standardized **JSON** exchange with correct HTTP status code mapping.
* **Error handling**: Centralized exception management via **@ControllerAdvice**.
* **Gateway & security**: integrated **API Gateway** with **basic authentication** and **rate limiting** (Bucket4j).
* **Containerization**: fully automated deployment using **Docker Compose**.

## Technology Stack
* **Language**: Java 21
* **Framework**: **Spring Boot 3.4.1 (Web MVC, Data JPA, Cache, Actuator)**
* **Security**: Spring Security (Basic Auth)
* **Traffic control**: Bucket4j Core (Rate limiting)
* **Documentation**: SpringDoc OpenAPI (Swagger UI)
* **Database**: SQLite
* **Build system**: Maven 3.x
* **Containerization**: Docker, Docker Compose

## Deployment (Docker)
The project can be launched on any machine with **Docker Desktop** installed. Run from the root directory:
docker-compose up --build

*Docker handles all dependencies (Java, Maven) automatically.*

## Key Project Files

**Root Directory:**
* **docker-compose.yml** — Orchestrates Gateway and API containers, sets up networking and persistent volumes.

**Core service (/todo_api — Port: 8081):**
* **pom.xml** — Project dependencies and build configuration.
* **Dockerfile** — Multi-stage build (Maven build + JRE 21 runtime).
* **TodoApiApplication.java** — application entry point; starts the embedded server.
* **Task.java** — task entity class (ID, title, description, status).
* **TaskController.java** — REST controller for handling HTTP requests.
* **TaskRepository.java** — repository interface for SQLite database operations.
* **GlobalExceptionHandler.java** — component for handling application-wide exceptions.
* **application.properties** — database and application settings.
* **TaskControllerTests.java** — comprehensive integration test for the full task lifecycle (CRUD operations); check if fields not included in request are kept in database during partial updates; check if ID from URL overrides any ID provided in the JSON body for security; verify the structured JSON error response when searching for a non-existent task ID.
* **TodoApiApplicationTests.java** — basic smoke test to verify application context loading.

**Gateway Service (/gateway_service — Port: 8080):**
* **pom.xml** — configuration for Gateway-specific dependencies (Bucket4j, Security).
* **Dockerfile** — containerization for the Gateway layer.
* **GatewayController.java** — proxy controller with **rate limiting** and request routing to port 8081.
* **SecurityConfig.java** — security layer configuration (Basic Auth).
* **application.properties** — server settings and proxy target configuration.
* **GatewayControllerTests.java** — Specialized tests for rate limiting (429 error) and proxy logic.
* **GatewayServiceTests.java** — context loading and security filter chain verification.

## API Endpoints (Access via Gateway :8080)
* **GET /api/tasks** — get all tasks.
* **POST /api/tasks** — create a new task.
* **GET /api/tasks/{id}** — get a task by ID.
* **PUT /api/tasks/{id}** — update a task by ID.
* **PATCH /api/tasks/{id}** — partially update task status.
* **DELETE /api/tasks/{id}** — delete a task by ID.

## Monitoring & Testing URLs
* **Swagger UI (Interactive API)**: [http://localhost:8080/swagger-ui/index.html](http://localhost:8080/swagger-ui/index.html)
* **Actuator Health (Backend)**: [http://localhost:8081/actuator/health](http://localhost:8081/actuator/health)
* **Actuator Metrics (Backend)**: [http://localhost:8081/actuator/metrics](http://localhost:8081/actuator/metrics)
