cmake_minimum_required(VERSION 3.10)

project(VectorC VERSION 1.0.0 LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(vector src/vector.c)
target_include_directories(vector PUBLIC include)

add_executable(vector_example examples/main.c)
target_link_libraries(vector_example PRIVATE vector)

add_custom_target(run
    COMMAND vector_example
    DEPENDS vector_example
)

enable_testing()

file(GLOB TEST_SOURCES "tests/*.c")

add_executable(vector_test ${TEST_SOURCES})

target_link_libraries(vector_test PRIVATE vector)

add_test(NAME VectorTestSuite COMMAND vector_test)
