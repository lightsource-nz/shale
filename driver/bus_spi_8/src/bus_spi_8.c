#include "driver/bus_spi_8.h"
#include "driver/bus_spi_8_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#   define BUS_SPI_PORT_DEFAULT 0
#endif

void _bus_spi_8_device_free(struct light_object *obj);
static struct lobj_type lobj_type_bus_spi_8 = (struct lobj_type) {
        .release = _bus_spi_8_device_free
};

Shale_Static_Driver_Define(bus_spi_8, DRIVER_ID_BUS_SPI_8, class_iobus, shale_driver_bus_spi_8_device_init, shale_driver_bus_spi_8_handle_message);
//driver_t *driver_bus_spi_8;

uint8_t shale_driver_bus_spi_8_device_init(struct device *device, const uint8_t *id)
{
    struct bus_spi_8_device *dev_bus_spi_8 = device_to_bus_spi_8_device(device);
    uint8_t retval;
    if(retval = shale_class_iobus_device_init(&dev_bus_spi_8->header, &_driver_bus_spi_8, &lobj_type_bus_spi_8, id)) {
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
Message_Handler(shale_driver_bus_spi_8_handle_message)
{
    struct bus_spi_8_device *sh_device = device_to_bus_spi_8_device(device);
    switch(handle->msg.msg_id) {
        case IOBUS_MSG_RX:
        /* TODO validate dimensions against hw */
        return MX_DELEGATE;
    }
    // message not handled, pass to parent
    return MX_DELEGATE;
}
driver_t *shale_driver_bus_spi_8()
{
    return &_driver_bus_spi_8;
}
void _bus_spi_8_device_free(struct light_object *obj)
{
    shale_free(lobject_to_bus_spi_8_device(obj));
}
