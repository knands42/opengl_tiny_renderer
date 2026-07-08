file(GLOB SOURCES
    Vendor/stb_image.h
    Vendor/stb_image.cpp
    Source/*.h
    Source/*.cpp
    Source/Events/*.h
    Source/Events/*.cpp
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core PUBLIC glfw)
target_link_libraries(Core PUBLIC glad)
target_link_libraries(Core PUBLIC glm)


target_include_directories(Core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
)
