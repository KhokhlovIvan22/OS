cmake_minimum_required(VERSION 3.20)
project(lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")

add_library(filequeue sources/filequeue.cpp)
target_include_directories(filequeue PUBLIC include)

add_executable(sender sources/sender.cpp)
target_link_libraries(sender PRIVATE filequeue)
add_executable(receiver sources/receiver.cpp)
target_link_libraries(receiver PRIVATE filequeue)
add_dependencies(receiver sender)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.11.0.zip)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests  tests/filequeue_test.cpp  tests/sender_test.cpp  tests/receiver_test.cpp tests/end2end.cpp)

target_link_libraries(tests PRIVATE gtest_main filequeue)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include ${googletest_SOURCE_DIR}/googletest/include)

include(GoogleTest)
gtest_discover_tests(tests)