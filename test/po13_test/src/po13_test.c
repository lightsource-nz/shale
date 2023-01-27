#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_DISPLAY_MAIN "device:display.main"

void main()
{
    shale_init();
//    shale_class_display_init();
//    shale_driver_sh1107_init();

    sh1107_device_t display_main;
    uint8_t status = shale_driver_sh1107_device_init(&display_main, ID_DISPLAY_MAIN);
    __breakpoint();
}
