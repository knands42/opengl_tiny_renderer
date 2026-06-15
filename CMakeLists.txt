# Setup base
cmake_minimum_required(VERSION 3.16)
project(opengl LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(Dependencies.cmake)

# Projects
add_subdirectory(App)
add_subdirectory(Core)

# Build & Pack
add_executable(opengl
        include/glad/glad.c
        src/main.cpp
        src/Shader.cpp
        src/VertexBuffer.cpp
        src/Application.cpp
)

target_include_directories(opengl
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/shaders
        PRIVATE ${CMAKE_SOURCE_DIR}/include/glad
        PRIVATE ${CMAKE_SOURCE_DIR}/include/KHR
)
target_link_libraries(opengl PRIVATE glfw OpenGL::GL glm::glm)

add_custom_target(run
        COMMAND $<TARGET_FILE:opengl>
        DEPENDS opengl
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
