#include "driver/bus_spi_8.h"
#include "driver/bus_spi_8_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#       define BUS_SPI_PORT_DEFAULT 0
#endif

#define BUS_SPI_8_FRAME_SIZE    1

void _bus_spi_8_device_free(struct light_object *obj);
static struct lobj_type _spi_8_ltype = (struct lobj_type) {
        .release = _bus_spi_8_device_free
};
static device_t *_spi_8_device_alloc();
static uint8_t _spi_8_init(device_t *device);
static uint8_t _spi_8_add(device_t *device);
static uint8_t _spi_8_msg(device_t *device, message_handle_t *handle);
static const struct device_event _spi_8_event = {
        .init = _spi_8_init,
        .add = _spi_8_add,
        .message = _spi_8_msg
};
Shale_Static_Driver_Define(bus_spi_8, DRIVER_ID_BUS_SPI_8, class_iobus, &_spi_8_ltype, _spi_8_device_alloc, _spi_8_event);
//driver_t *driver_bus_spi_8;

driver_t *shale_driver_bus_spi_8()
{
        return &_driver_bus_spi_8;
}
void _bus_spi_8_device_free(struct light_object *obj)
{
        shale_free(lobject_to_bus_spi_8_device(obj));
}

static device_t *_spi_8_device_alloc()
{
        struct bus_spi_8_device *device = shale_malloc(sizeof(struct bus_spi_8_device));
        return &device->header.header;
}
static uint8_t _spi_8_init(device_t *device_header)
{
        struct bus_spi_8_device *device = device_to_bus_spi_8_device(device_header);

        device->header.bus_protocol = IOBUS_PROTOCOL_SPI;
        device->header.frame_size = BUS_SPI_8_FRAME_SIZE;
        return LIGHT_OK;
}
static uint8_t _spi_8_add(device_t *device_header)
{
        struct bus_spi_8_device *device = device_to_bus_spi_8_device(device_header);

        return LIGHT_OK;
}
static uint8_t _spi_8_msg(device_t *device_header, message_handle_t *handle)
{
        struct bus_spi_8_device *device = device_to_bus_spi_8_device(device_header);
        switch(handle->msg.msg_id) {
                case IOBUS_MSG_RX:
                return MX_DELEGATE;
        }
        // message not handled, pass to parent
        return MX_DELEGATE;

}
