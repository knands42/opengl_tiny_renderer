# Setup base
cmake_minimum_required(VERSION 3.16)
project(opengl LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(Dependencies.cmake)

# Build & Pack
add_executable(opengl
        src/Main.cpp
        src/Shader.cpp
        src/VertexBuffer.cpp
        src/Application.cpp
        src/IndexBuffer.cpp
        src/Renderer.cpp
        src/VertexArray.cpp
        src/Texture.cpp
        vendor/stb_image.cpp
)

target_include_directories(opengl
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/shaders
        PRIVATE ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(opengl PRIVATE glfw OpenGL::GL glm::glm glad)

add_custom_target(run
        COMMAND $<TARGET_FILE:opengl>
        DEPENDS opengl
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
