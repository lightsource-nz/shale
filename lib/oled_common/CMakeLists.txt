
add_library(oled_common INTERFACE)

target_include_directories(
    oled_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    oled_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/oled.c
)

if(NOT PICO_NO_HARDWARE)
    target_link_libraries(
        shale_core
        INTERFACE
        hardware_spi
        hardware_i2c
    )
endif()

target_link_libraries(
    oled_common
    INTERFACE
)

