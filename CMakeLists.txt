# Setup base
cmake_minimum_required(VERSION 3.16)
project(opengl_tiny_renderer LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(Dependencies.cmake)

# Build & Pack
# TODO: set sources
add_executable(opengl_tiny_renderer
        src/Main.cpp
        src/Shader.cpp
        src/VertexBuffer.cpp
        src/Application.cpp
        src/IndexBuffer.cpp
        src/Renderer.cpp
        src/VertexArray.cpp
        src/Texture.cpp
        src/Camera.cpp
        vendor/stb_image.cpp
)

target_include_directories(opengl_tiny_renderer
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/shaders
        PRIVATE ${CMAKE_SOURCE_DIR}/vendor
        PRIVATE ${CMAKE_SOURCE_DIR}/textures
)
target_link_libraries(opengl_tiny_renderer PRIVATE glfw OpenGL::GL glm::glm glad)

add_custom_target(run
        COMMAND $<TARGET_FILE:opengl_tiny_renderer>
        DEPENDS opengl_tiny_renderer
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
