#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_DISPLAY_MAIN "device:display.main"

void main()
{
    shale_init();
    shale_class_display_init();
    shale_driver_sh1107_init();

    class_t *class_display = shale_class_display();
    driver_t *driver_sh1107 = shale_driver_sh1107();

    sh1107_device_t *display_main = (sh1107_device_t *)shale_device_new(ID_DISPLAY_MAIN, driver_sh1107);
    __breakpoint();
}
