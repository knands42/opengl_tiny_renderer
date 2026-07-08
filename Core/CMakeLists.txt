set(SOURCES
        Vendor/stb_image.cpp

        Source/GLError.h

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

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES})

target_link_libraries(Core glfw)
target_link_libraries(Core glad)
target_link_libraries(Core glm)

target_include_directories(Core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
)
