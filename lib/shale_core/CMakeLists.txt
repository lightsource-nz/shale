
add_library(shale_core INTERFACE)

target_include_directories(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/shale_core.c
)

target_link_libraries(
    shale_core
    INTERFACE

)

