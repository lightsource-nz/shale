
add_library(shale_core INTERFACE)

target_include_directories(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    shale_core INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/shale_core.c
    ${CMAKE_CURRENT_LIST_DIR}/src/message.c
    ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
    ${CMAKE_CURRENT_LIST_DIR}/src/thread.c
)

set(SHALE_CORE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)
function(shale_link_static_objects TARGET)
    if(NOT PICO_NO_HARDWARE)
        pico_set_linker_script(${TARGET} ${SHALE_CORE_SOURCE_DIR}/memmap_shale.ld)
    endif()
endfunction()

if(NOT PICO_NO_HARDWARE)
    target_link_libraries(
        shale_core
        INTERFACE
        pico_util
        hardware_exception
    )
endif()

target_link_libraries(
    shale_core
    INTERFACE
    light_object
    light_message
)

