# CMakeList.txt: проект CMake для Lab1cmake; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Lab1cmake)

# Добавьте источник в исполняемый файл этого проекта.
add_library(core STATIC Core/employee.cpp Core/helpers.cpp)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/Core)
target_sources(core PRIVATE Core/employee.h Core/helpers.h)

add_executable(Creator Creator.cpp)
add_executable(Reporter Reporter.cpp)
add_executable(Main Main.cpp)

target_link_libraries(Creator PRIVATE core)
target_link_libraries(Reporter PRIVATE core)
target_link_libraries(Main PRIVATE core)

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(UnitTest UnitTest.cpp)

target_link_libraries(UnitTest PRIVATE core gtest_main)
add_test(NAME UnitTest COMMAND Test)
include(GoogleTest)
gtest_discover_tests(UnitTest)