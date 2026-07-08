set(SOURCES
        Source/Test.cpp
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES})

target_link_libraries(Core glfw)
target_link_libraries(Core glad)
target_link_libraries(Core glm)

target_include_directories(Core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendors
)
