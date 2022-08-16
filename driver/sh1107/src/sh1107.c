#include "driver/sh1107.h"

#include <stdlib.h>

static display_api_t api_sh1107 = {
    
};

static driver_t driver_sh1107 = {
    .id = DRIVER_ID_SH1107,
    .class_id = CLASS_ID_DISPLAY,
    .driver_api = &api_sh1107
};

void shale_driver_sh1107_init()
{
    shale_driver_create(&driver_sh1107);
}
