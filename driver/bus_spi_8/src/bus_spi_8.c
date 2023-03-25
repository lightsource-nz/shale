#include "driver/bus_spi_8.h"
#include "driver/bus_spi_8_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#       define BUS_SPI_PORT_DEFAULT 0
#endif

#define BUS_SPI_8_FRAME_SIZE    1

void _bus_spi_8_interface_release(struct light_object *obj);
static struct lobj_type _spi_8_ltype = (struct lobj_type) {
        .release = _bus_spi_8_interface_release
};
void _spi_8_interface_free(struct device *device);
static struct device_interface *_spi_8_interface_alloc();
static uint8_t _spi_8_init(struct device_interface *device);
static uint8_t _spi_8_add(struct device_interface *device);
static uint8_t _spi_8_msg(struct device_interface *device, message_handle_t *handle);
static const struct interface_event _spi_8_event = {
        .alloc = _spi_8_interface_alloc,
        .free = _spi_8_interface_free,
        .init = _spi_8_init,
        .add = _spi_8_add,
        .message = _spi_8_msg
};
Shale_Static_Driver_Define(bus_spi_8, DRIVER_ID_BUS_SPI_8, class_iobus, &_spi_8_ltype, _spi_8_event);
//driver_t *driver_bus_spi_8;

driver_t *shale_driver_bus_spi_8()
{
        return &_driver_bus_spi_8;
}
void _spi_8_interface_free(struct device_interface *iface)
{
        shale_free(interface_to_bus_spi_8_interface(iface));
}
void _bus_spi_8_interface_release(struct light_object *obj)
{
        _spi_8_interface_free(to_device_interface(obj));
}

static struct device_interface *_spi_8_interface_alloc()
{
        struct bus_spi_8_interface *iface = shale_malloc(sizeof(struct bus_spi_8_interface));
        return &iface->header.header;
}
static uint8_t _spi_8_init(struct device_interface *iface_header)
{
        struct bus_spi_8_interface *iface = interface_to_bus_spi_8_interface(iface_header);

        iface->header.bus_protocol = IOBUS_PROTOCOL_SPI;
        iface->header.frame_size = BUS_SPI_8_FRAME_SIZE;
        return LIGHT_OK;
}
static uint8_t _spi_8_add(struct device_interface *iface_header)
{
        struct bus_spi_8_interface *iface = interface_to_bus_spi_8_interface(iface_header);

        return LIGHT_OK;
}
static uint8_t _spi_8_msg(struct device_interface *iface_header, message_handle_t *handle)
{
        struct bus_spi_8_interface *iface = interface_to_bus_spi_8_interface(iface_header);
        switch(handle->msg.msg_id) {
                case IOBUS_MSG_RX:
                return MX_DELEGATE;
        }
        // message not handled, pass to parent
        return MX_DELEGATE;

}
