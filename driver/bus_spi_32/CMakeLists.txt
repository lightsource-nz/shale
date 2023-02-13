
add_library(driver_bus_spi_32 INTERFACE)

target_include_directories(
    driver_bus_spi_32 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    driver_bus_spi_32 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/bus_spi_32.c
)

target_link_libraries(
    driver_bus_spi_32 INTERFACE
    shale_core
    class_iobus
)
