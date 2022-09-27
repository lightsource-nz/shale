#define __SHALE_SOURCE__
#include "driver/sh1107.h"
#include "shale_internal.h"

#include <stdlib.h>

#ifndef SH1107_TRANSPORT_DEFAULT
#   define SH1107_TRANSPORT_DEFAULT OLED_TRANSPORT_SPI
#endif

static display_api_t api_sh1107 = {
    
};

static driver_t *driver_sh1107;

void shale_driver_sh1107_init()
{
    class_t *driver_class = shale_class_display();
    assert_class(driver_class);
    driver_sh1107 = shale_driver_new(DRIVER_ID_SH1107, driver_class, &api_sh1107, sizeof(sh1107_data_t), &shale_driver_sh1107_init_device);
}
void shale_driver_sh1107_init_device(device_t *generic_device)
{
    sh1107_device_t *device = (sh1107_device_t *)generic_device;
    device->driver_data->transport_type = SH1107_TRANSPORT_DEFAULT;
}
driver_t *shale_driver_sh1107()
{
    return driver_sh1107;
}
