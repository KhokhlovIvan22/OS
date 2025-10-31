cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(core STATIC sources/array.cpp sources/marker.cpp)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(Main main.cpp)

target_link_libraries(Main PRIVATE core)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    tests/array_tests.cpp
    tests/marker_tests.cpp
    tests/end2end.cpp
)
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests PRIVATE core GTest::gtest_main)
target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
include(GoogleTest) 
gtest_discover_tests(Tests)
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS Tests
)
