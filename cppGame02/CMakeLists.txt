cmake_minimum_required(VERSION 3.11)
project(cppGame02 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")

if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/logo.rc")
    add_executable(${PROJECT_NAME} ${SOURCES} logo.rc)
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(${PROJECT_NAME} PRIVATE -static)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    include/entities
    include/example_data
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})