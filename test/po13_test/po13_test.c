#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>

#include <pico/platform.h>
#include <stdlib.h>

void main()
{
    shale_init();
    shale_class_display_init();
    shale_driver_sh1107_init();

    sh1107_device_t *display_main = malloc(sizeof(device_t));
    display_main->class_data = malloc(sizeof(display_data_t));
    display_main->driver_data = malloc(sizeof(sh1107_data_t));

    shale_device_create((device_t *)display_main);
    __breakpoint();
}
