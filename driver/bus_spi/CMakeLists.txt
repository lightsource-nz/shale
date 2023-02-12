
add_library(driver_bus_spi INTERFACE)

target_include_directories(
    driver_bus_spi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    driver_bus_spi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/bus_spi.c
)

target_link_libraries(
    driver_bus_spi INTERFACE
    shale_core
    class_iobus
)
