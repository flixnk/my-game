cmake_minimum_required(VERSION 3.10)
project(my-game)

find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    include/entities
    include/example_data
)

target_link_libraries(${PROJECT_NAME} raylib)
