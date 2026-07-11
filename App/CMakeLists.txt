set(SOURCES
        Source/Main.cpp

        Source/Application.h
        Source/Application.cpp
)

add_executable(App)

target_sources(App PRIVATE ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(App Core imgui)

target_include_directories(App
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Textures
)

# Set working directory for CLion
set_target_properties(App PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)
