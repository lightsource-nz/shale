#include "driver/sh1107.h"
#include "driver/sh1107_internal.h"

#include <stdlib.h>

void sh1107_device_write(struct sh1107_device *device, uint16_t write_addr, size_t write_len, uint32_t write_data);
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

void _sh1107_device_free(struct light_object *obj);
static struct lobj_type lobj_type_sh1107 = (struct lobj_type) {
        .release = _sh1107_device_free
};

Shale_Static_Driver_Define(sh1107, DRIVER_ID_SH1107, display, shale_driver_sh1107_handle_message);
//driver_t *driver_sh1107;

uint8_t shale_driver_sh1107_init()
{
    class_t *driver_class = shale_class_display();
    assert_class(driver_class);
    uint8_t retval;
    if(retval = shale_driver_init(&_driver_sh1107, driver_class, DRIVER_ID_SH1107,
                                  &shale_driver_sh1107_handle_message)) {
        // log error
        return retval;
    }
    return LIGHT_OK;
}
uint8_t shale_driver_sh1107_device_init(struct sh1107_device *device, const uint8_t *id)
{
    uint8_t retval;
    if(retval = shale_class_display_device_init(&device->header, &_driver_sh1107, &lobj_type_sh1107, id)) {
        // log error
        return retval;
    }
    /*
    device->port.port_type = SH1107_PORT_TYPE_DEFAULT;
    switch(device->port.port_type) {
        case OLED_PORT_SPI:
            device->port.port_number = SH1107_SPI_PORT_DEFAULT;
            device->port.bind.spi.pin_dc = OLED_PIN_NC;
            device->port.bind.spi.pin_miso = OLED_PIN_NC;
            device->port.bind.spi.pin_mosi = OLED_PIN_NC;
            device->port.bind.spi.pin_sck = OLED_PIN_NC;
            break;
        case OLED_PORT_I2C:
            device->port.port_number = SH1107_I2C_PORT_DEFAULT;
            device->port.bind.i2c.pin_scl = OLED_PIN_NC;
            device->port.bind.i2c.pin_sda = OLED_PIN_NC;
            break;
    }
    */
}
Message_Handler(shale_driver_sh1107_handle_message)
{
    struct sh1107_device *sh_device = device_to_sh1107_device(device);
    switch(handle->msg.msg_id) {
        case DISPLAY_SET_DIMENSION:
        /* TODO validate dimensions against hw */
        return MX_DELEGATE;
    }
    // message not handled, pass to parent
    return MX_DELEGATE;
}
driver_t *shale_driver_sh1107()
{
    return &_driver_sh1107;
}
void _sh1107_device_free(struct light_object *obj)
{
    shale_free(lobject_to_sh1107_device(obj));
}

// write a single value of up to 4 bytes to the given address. smaller values are read from
// the low bytes of [write_data]
void sh1107_device_write(struct sh1107_device *device, uint16_t write_addr, size_t write_len, uint32_t write_data)
{

}
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel)
{

}