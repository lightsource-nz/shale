
add_library(driver_sh1106 INTERFACE)

target_include_directories(
    driver_sh1106 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    driver_sh1106 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/display.c
)

target_link_libraries(
    driver_sh1106 INTERFACE
    shale_core
    class_display
)

