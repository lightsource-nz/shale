
add_library(light_message INTERFACE)

target_include_directories(
        light_message INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
        light_message INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/message.c
)

if(NOT PICO_NO_HARDWARE)
        target_link_libraries(
                light_message
                INTERFACE
        )
endif()

target_link_libraries(
        light_message
        INTERFACE
        light_util
)

