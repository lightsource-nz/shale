#include "driver/sh1107.h"

#include <stdlib.h>

void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);

static display_api_t api_sh1107 = {
    .get_dimension  = &shale_display_dimension_get,
    .get_channels   = &shale_display_channels_get,
    .get_encoding   = &shale_display_encoding_get,
    .get_bpp        = &shale_display_bpp_get,
    .get_px_addr    = &sh1107_get_px_addr,
    .device_write   = &sh1107_device_write
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

// write a single value of up to 4 bytes to the given address. smaller values are read from
// the low bytes of [write_data]
void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);