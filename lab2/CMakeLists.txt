cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Lab2Threads)

add_library(core STATIC Core/array_functions.cpp)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/Core)

add_executable(Main main.cpp min_max.cpp average.cpp)

target_link_libraries(Main PRIVATE core)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set(CMAKE_CXX_STANDARD 98)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(UnitTests UnitTests.cpp min_max.cpp average.cpp)
target_link_libraries(UnitTests PRIVATE core gtest_main)
add_test(NAME UnitTests COMMAND UnitTests)
include(GoogleTest)
gtest_discover_tests(UnitTests)
