
add_executable(po13_test po13_test.c)

target_link_libraries(
    po13_test PRIVATE
    pico_stdlib
    pico_platform
    shale_core
    class_display
    driver_sh1107
)

pico_add_extra_outputs(po13_test)
