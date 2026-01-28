cmake_minimum_required(VERSION 3.10)
project(cppGame02 VERSION 1.0 LANGUAGES CXX)

set(RAYLIB_SRC_PATH "$ENV{HOME}/Apps/raylib/src")

find_library(RAYLIB_LIB 
    NAMES raylib libraylib.a
    PATHS ${RAYLIB_SRC_PATH}
    NO_DEFAULT_PATH
)

find_path(RAYLIB_INCLUDE_DIR 
    NAMES raylib.h 
    PATHS ${RAYLIB_SRC_PATH}
    NO_DEFAULT_PATH
)

message(STATUS "Raylib Lib: ${RAYLIB_LIB}")
message(STATUS "Raylib Include: ${RAYLIB_INCLUDE_DIR}")

if(NOT RAYLIB_LIB OR NOT RAYLIB_INCLUDE_DIR)
    message(FATAL_ERROR "raylib-Dateien fehlen immer noch in ${RAYLIB_SRC_PATH}!")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${RAYLIB_INCLUDE_DIR}
    include
    include/entities
    include/example_data
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${RAYLIB_LIB}
    m pthread dl
)