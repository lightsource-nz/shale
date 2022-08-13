
add_library(driver_sh1107 INTERFACE)

target_include_directories(
    driver_sh1107 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    driver_sh1107 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/sh1107.c
)

target_link_libraries(
    driver_sh1107 INTERFACE
    shale_core
    class_display
    oled_common
)

