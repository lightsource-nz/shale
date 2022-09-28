#define __SHALE_SOURCE__
#include "driver/sh1107.h"
#include "shale_internal.h"

#include <stdlib.h>

void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);

#ifndef SH1107_TRANSPORT_DEFAULT
#   define SH1107_TRANSPORT_DEFAULT OLED_TRANSPORT_SPI
#endif

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

// write a single value of up to 4 bytes to the given address. smaller values are read from
// the low bytes of [write_data]
void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);