cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(shale C CXX ASM)

pico_sdk_init()

# libraries
add_subdirectory(lib/shale_core)
add_subdirectory(lib/oled_common)
add_subdirectory(lib/light_object)
add_subdirectory(lib/light_util)

# classes
add_subdirectory(class/display)

# drivers
add_subdirectory(driver/sh1107)

# tests
add_subdirectory(test/po13_test)
