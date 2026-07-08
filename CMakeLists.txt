# Setup base
cmake_minimum_required(VERSION 3.16)
project(opengl_tiny_renderer LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

# Dependencies
include(Dependencies.cmake)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Submodules
add_subdirectory(App)
add_subdirectory(Core)

add_custom_target(run
        COMMAND App
        DEPENDS App
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/App
        COMMENT "Running App..."
)