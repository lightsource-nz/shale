
add_library(class_iobus INTERFACE)

target_include_directories(
    class_iobus INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    class_iobus INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/iobus.c
)

target_link_libraries(
    class_iobus INTERFACE
    shale_core
)

