
add_library(oled_common INTERFACE)

target_include_directories(
    oled_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    oled_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/shale_core.c
)

target_link_libraries(
    oled_common
    INTERFACE

)

