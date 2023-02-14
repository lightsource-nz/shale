
add_library(light_spi INTERFACE)

target_include_directories(
    light_spi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    light_spi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/spi.c
)

if(NOT PICO_NO_HARDWARE)
    target_link_libraries(
        light_spi
        INTERFACE
        hardware_spi
    )
endif()

target_link_libraries(
    light_spi
    INTERFACE
)

