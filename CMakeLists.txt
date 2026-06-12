cmake_minimum_required(VERSION 3.16)
project(opengl LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
)

FetchContent_MakeAvailable(glm)

# Glfw3
FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_MakeAvailable(glfw3)

# OpenGL
find_package(OpenGL REQUIRED)

# Build & Pack
add_executable(opengl
        src/main.cpp
        src/shader.cpp
        src/shader.h
        include/glad/glad.c
)

target_include_directories(opengl
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/include/glad
        PRIVATE ${CMAKE_SOURCE_DIR}/include/KHR
)
target_link_libraries(opengl PRIVATE glfw OpenGL::GL glm::glm)

add_custom_target(run
        COMMAND $<TARGET_FILE:opengl>
        DEPENDS opengl
)
