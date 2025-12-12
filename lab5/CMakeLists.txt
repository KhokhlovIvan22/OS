cmake_minimum_required(VERSION 3.20)
project(lab5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")

add_library(core sources/employee.cpp sources/message.cpp sources/pipes.cpp sources/ClientHandler.cpp)
target_include_directories(core PUBLIC include)

add_executable(server sources/server.cpp)
target_link_libraries(server PRIVATE core)

add_executable(client sources/client.cpp)
target_link_libraries(client PRIVATE core)

add_dependencies(server client)

include(FetchContent)
FetchContent_Declare( googletest URL https://github.com/google/googletest/archive/release-1.11.0.zip)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/unitTests.cpp tests/scenarioTests.cpp)

target_link_libraries(tests PRIVATE core gtest_main)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(tests)