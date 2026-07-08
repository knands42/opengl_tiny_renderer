set(SOURCES
        Vendor/stb_image.cpp

        Source/Main.cpp
        Source/GLError.h

        Source/Application.h
        Source/Application.cpp

        Source/Camera.h
        Source/Camera.cpp

        Source/IndexBuffer.h
        Source/IndexBuffer.cpp

        Source/Renderer.h
        Source/Renderer.cpp

        Source/Shader.h
        Source/Shader.cpp

        Source/Texture.h
        Source/Texture.cpp

        Source/VertexArray.h
        Source/VertexArray.cpp

        Source/VertexBuffer.h
        Source/VertexBuffer.cpp

        Source/VertexBufferLayout.h
        Source/VertexBufferLayout.cpp
)

add_executable(App)
target_sources(App PRIVATE ${SOURCES})

target_link_libraries(App glfw)
target_link_libraries(App glad)
target_link_libraries(App glm)

target_include_directories(App
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Textures
)

# Set working directory for CLion
set_target_properties(App PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)
