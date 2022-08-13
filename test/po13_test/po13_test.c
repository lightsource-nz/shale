#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>

#include <stdlib.h>

void main()
{
    shale_init();
    shale_class_display_init();
    shale_driver_sh1107_init();

    device_t *display_main = malloc(sizeof(device_t));
    display_main->class_data = malloc(sizeof(display_data_t));

}
