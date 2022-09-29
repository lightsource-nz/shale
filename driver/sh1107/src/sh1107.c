#define __SHALE_SOURCE__
#include "driver/sh1107.h"
#include "shale_internal.h"

#include <stdlib.h>

void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);

#ifndef SH1107_PORT_TYPE_DEFAULT
#   define SH1107_PORT_TYPE_DEFAULT OLED_PORT_SPI
#endif

#ifndef SH1107_SPI_PORT_DEFAULT
#   define SH1107_SPI_PORT_DEFAULT 0
#endif
#ifndef SH1107_I2C_PORT_DEFAULT
#   define SH1107_I2C_PORT_DEFAULT 0
#endif

static display_api_t api_sh1107 = {
    .get_dimension  = &shale_display_dimension_get,
    .get_channels   = &shale_display_channels_get,
    .get_encoding   = &shale_display_encoding_get,
    .get_bpp        = &shale_display_bpp_get,
    .get_px_addr    = &sh1107_get_px_addr,
    .device_write   = &sh1107_device_write
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
    device->driver_data->port->port_type = SH1107_PORT_TYPE_DEFAULT;
    switch(device->driver_data->port->port_type) {
        case OLED_PORT_SPI:
            device->driver_data->port->port_number = SH1107_SPI_PORT_DEFAULT;
            device->driver_data->port->bind.spi.pin_dc = OLED_PIN_NC;
            device->driver_data->port->bind.spi.pin_miso = OLED_PIN_NC;
            device->driver_data->port->bind.spi.pin_mosi = OLED_PIN_NC;
            device->driver_data->port->bind.spi.pin_sck = OLED_PIN_NC;
            break;
        case OLED_PORT_I2C:
            device->driver_data->port->port_number = SH1107_I2C_PORT_DEFAULT;
            device->driver_data->port->bind.i2c.pin_scl = OLED_PIN_NC;
            device->driver_data->port->bind.i2c.pin_sda = OLED_PIN_NC;
            break;
    }
}
driver_t *shale_driver_sh1107()
{
    return driver_sh1107;
}

// write a single value of up to 4 bytes to the given address. smaller values are read from
// the low bytes of [write_data]
void sh1107_device_write(sh1107_device_t *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel);