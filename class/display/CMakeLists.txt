
add_library(class_display INTERFACE)

target_include_directories(
    class_display INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    class_display INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/display.c
)

target_link_libraries(
    class_display INTERFACE
    shale_core
)

