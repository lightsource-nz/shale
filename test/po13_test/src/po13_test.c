#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_DISPLAY_MAIN "device:display.main"

Shale_Static_Device_Define(display_main, ID_DISPLAY_MAIN, driver_sh1107)

void main()
{
    shale_init();
    
    __breakpoint();
}
