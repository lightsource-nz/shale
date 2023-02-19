#include "driver/bus_spi_32.h"
#include "driver/bus_spi_32_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#   define BUS_SPI_PORT_DEFAULT 0
#endif

#define BUS_SPI_32_FRAME_SIZE   4

void _bus_spi_32_device_free(struct light_object *obj);
static struct lobj_type lobj_type_bus_spi_32 = (struct lobj_type) {
        .release = _bus_spi_32_device_free
};

static device_t *_spi_32_device_alloc();
static uint8_t _spi_32_init(device_t *device);
static uint8_t _spi_32_add(device_t *device);
static uint8_t _spi_32_msg(device_t *device, message_handle_t *handle);
static const struct device_event _spi_32_event = {
        .init = _spi_32_init,
        .add = _spi_32_add,
        .message = _spi_32_msg
};

Shale_Static_Driver_Define(bus_spi_32, DRIVER_ID_BUS_SPI_32, class_iobus, _spi_32_device_alloc, _spi_32_event);

uint8_t shale_driver_bus_spi_32_device_init(struct device *device_header, const uint8_t *id)
{
    struct bus_spi_32_device *device = device_to_bus_spi_32_device(device_header);
    uint8_t retval;
    if(retval = shale_class_iobus_device_init(&device->header, &_driver_bus_spi_32, &lobj_type_bus_spi_32, id)) {
        light_warn("failed to init device, error code %s", light_error_to_string(retval));
        return retval;
    }
    device->header.bus_protocol = IOBUS_PROTOCOL_SPI;
    device->header.frame_size = BUS_SPI_32_FRAME_SIZE;
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
Message_Handler(shale_driver_bus_spi_32_handle_message)
{
    struct bus_spi_32_device *sh_device = device_to_bus_spi_32_device(device);
    switch(handle->msg.msg_id) {
        case IOBUS_MSG_RX:
        /* TODO validate dimensions against hw */
        return MX_DELEGATE;
    }
    // message not handled, pass to parent
    return MX_DELEGATE;
}
driver_t *shale_driver_bus_spi_32()
{
    return &_driver_bus_spi_32;
}
void _bus_spi_32_device_free(struct light_object *obj)
{
    shale_free(lobject_to_bus_spi_32_device(obj));
}

static device_t *_spi_32_device_alloc()
{
        struct bus_spi_32_device *device = shale_malloc(sizeof(struct bus_spi_32_device));
        return &device->header.header;
}
static uint8_t _spi_32_init(device_t *device_header)
{
        struct bus_spi_32_device *device = device_to_bus_spi_32_device(device_header);

        return LIGHT_OK;
}
static uint8_t _spi_32_add(device_t *device_header)
{
        struct bus_spi_32_device *device = device_to_bus_spi_32_device(device_header);

        return LIGHT_OK;
}
static uint8_t _spi_32_msg(device_t *device_header, message_handle_t *handle)
{
        struct bus_spi_32_device *device = device_to_bus_spi_32_device(device_header);

        return LIGHT_OK;
}
