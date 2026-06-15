#
# Core
#

set(SOURCES
        Source/Core/Application.h
        Source/Core/Application.cpp
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
        GLFW_INCLUDE_NONE
)

target_link_libraries(Core glfw)
target_link_libraries(Core glad)
target_link_libraries(Core glm)

target_include_directories(Core PUBLIC "Source" "vendor/stb")