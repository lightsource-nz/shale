
add_executable(po13_test)

target_sources(po13_test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/po13_test.c)

target_link_libraries(
    po13_test PUBLIC
    pico_platform
    shale_core
    class_iobus
    class_display
    driver_sh1107
    driver_bus_spi_8
    driver_bus_spi_32
)
shale_link_static_objects(po13_test)
if(NOT PICO_NO_HARDWARE)
        pico_enable_stdio_semihosting(po13_test 1)
        target_link_libraries(po13_test PUBLIC pico_stdlib)
endif()
pico_add_extra_outputs(po13_test)
