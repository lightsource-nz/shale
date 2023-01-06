
add_library(shale_core INTERFACE)

target_include_directories(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/shale_core.c
    ${CMAKE_CURRENT_LIST_DIR}/src/message.c
    ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
    ${CMAKE_CURRENT_LIST_DIR}/src/thread.c
)

pico_set_linker_script(shale_core ${CMAKE_CURRENT_LIST_DIR}/memmap_shale.ld)

target_link_libraries(
    shale_core
    INTERFACE
    light_object
    hardware_exception
    pico_util
)

