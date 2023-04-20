cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(shale C CXX ASM)

pico_sdk_init()

# libraries
add_subdirectory(lib/shale_core)
add_subdirectory(lib/oled_common)
add_subdirectory(lib/light_object)
add_subdirectory(lib/light_message)
add_subdirectory(lib/light_util)
add_subdirectory(lib/light_spi)

# classes
add_subdirectory(class/display)
add_subdirectory(class/iobus)

# drivers
add_subdirectory(driver/sh1107)
add_subdirectory(driver/bus_spi_8)
add_subdirectory(driver/bus_spi_32)

# tests
add_subdirectory(test/po13_test)
