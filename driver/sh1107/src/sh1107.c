#include "driver/sh1107.h"

#include <stdlib.h>

static display_api_t api_sh1107 = {
    
};

driver_t shale_driver_sh1107 = {
    .id = DRIVER_ID_SH1107,
    .class_id = CLASS_ID_DISPLAY,
    .driver_api = &api_sh1107
};

void shale_driver_sh1107_init()
{
    shale_driver_register(&shale_driver_sh1107);
}

void shale_driver_sh1107_device_create(sh1107_device_t *dev)
{

    dev->dev_class = &shale_class_display;
    dev->driver = &shale_driver_sh1107;

}
