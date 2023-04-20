#include "driver/bus_spi_32.h"
#include "driver/bus_spi_32_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#   define BUS_SPI_PORT_DEFAULT 0
#endif

#define BUS_SPI_32_FRAME_SIZE   4

void _bus_spi_32_interface_release(struct light_object *obj);
static struct lobj_type _ltype_spi_32_interface = (struct lobj_type) {
        .release = _bus_spi_32_interface_release
};

void _spi_32_interface_free(struct device_interface *ifx_header);
static struct device_interface *_spi_32_interface_alloc();
static uint8_t _spi_32_init(struct device_interface *ifx_header);
static uint8_t _spi_32_add(struct device_interface *ifx_header);
static uint8_t _spi_32_msg(struct device_interface *ifx_header, message_handle_t *handle);
static const struct interface_event _spi_32_event = {
        .init = _spi_32_init,
        .add = _spi_32_add,
        .message = _spi_32_msg
};

Shale_Static_Driver_Define(bus_spi_32, DRIVER_ID_BUS_SPI_32, class_iobus, &_ltype_spi_32_interface, _spi_32_interface_alloc, _spi_32_interface_free, _spi_32_event);

struct device_driver *shale_driver_bus_spi_32()
{
    return &_driver_bus_spi_32;
}
void _spi_32_interface_free(struct device_interface *ifx_header)
{
        shale_free(interface_to_bus_spi_32_interface(ifx_header));
}
void _bus_spi_32_interface_release(struct light_object *obj)
{
        _spi_32_interface_free(to_device_interface(obj));
}

static struct device_interface *_spi_32_interface_alloc()
{
        struct bus_spi_32_interface *ifx = shale_malloc(sizeof(struct bus_spi_32_interface));
        return &ifx->header.header;
}
static uint8_t _spi_32_init(struct device_interface *ifx_header)
{
        struct bus_spi_32_interface *ifx = interface_to_bus_spi_32_interface(ifx_header);

        ifx->header.bus_protocol = IOBUS_PROTOCOL_SPI;
        ifx->header.frame_size = BUS_SPI_32_FRAME_SIZE;
        return LIGHT_OK;
}
static uint8_t _spi_32_add(struct device_interface *ifx_header)
{
        struct bus_spi_32_interface *ifx = interface_to_bus_spi_32_interface(ifx_header);

        return LIGHT_OK;
}
static uint8_t _spi_32_msg(struct device_interface *ifx_header, message_handle_t *handle)
{
        struct bus_spi_32_interface *ifx = interface_to_bus_spi_32_interface(ifx_header);

        return LIGHT_OK;
}
