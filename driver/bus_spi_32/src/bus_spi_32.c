#include "driver/bus_spi_32.h"
#include "driver/bus_spi_32_internal.h"

#include <stdlib.h>

#ifndef BUS_SPI_PORT_DEFAULT
#   define BUS_SPI_PORT_DEFAULT 0
#endif

#define BUS_SPI_32_FRAME_SIZE   4

void _bus_spi_32_device_release(struct light_object *obj);
static struct lobj_type _spi_32_ltype = (struct lobj_type) {
        .release = _bus_spi_32_device_release
};

void _bus_spi_32_device_free(struct device *device);
static device_t *_spi_32_device_alloc();
static uint8_t _spi_32_init(device_t *device);
static uint8_t _spi_32_add(device_t *device);
static uint8_t _spi_32_msg(device_t *device, message_handle_t *handle);
static const struct device_event _spi_32_event = {
        .alloc = _spi_32_device_alloc,
        .free = _bus_spi_32_device_free,
        .init = _spi_32_init,
        .add = _spi_32_add,
        .message = _spi_32_msg
};

Shale_Static_Driver_Define(bus_spi_32, DRIVER_ID_BUS_SPI_32, class_iobus, &_spi_32_ltype, _spi_32_event);

driver_t *shale_driver_bus_spi_32()
{
    return &_driver_bus_spi_32;
}
void _bus_spi_32_device_free(struct device *device)
{
        shale_free(device_to_bus_spi_32_device(device));
}
void _bus_spi_32_device_release(struct light_object *obj)
{
        _bus_spi_32_device_free(to_device_instance(obj));
}

static device_t *_spi_32_device_alloc()
{
        struct bus_spi_32_device *device = shale_malloc(sizeof(struct bus_spi_32_device));
        return &device->header.header;
}
static uint8_t _spi_32_init(device_t *device_header)
{
        struct bus_spi_32_device *device = device_to_bus_spi_32_device(device_header);

        device->header.bus_protocol = IOBUS_PROTOCOL_SPI;
        device->header.frame_size = BUS_SPI_32_FRAME_SIZE;
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
