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

void _sh1107_device_release(struct light_object *obj);
static struct lobj_type _sh1107_ltype = (struct lobj_type) {
        .release = _sh1107_device_release
};

void _sh1107_device_free(struct device *device);
static device_t *_sh1107_device_alloc();
static uint8_t _sh1107_init(device_t *device);
static uint8_t _sh1107_add(device_t *device);
static uint8_t _sh1107_msg(device_t *device, message_handle_t *handle);
static const struct interface_event _sh1107_event = {
        .alloc = _sh1107_device_alloc,
        .free = _sh1107_device_free,
        .init = _sh1107_init,
        .add = _sh1107_add,
        .message = _sh1107_msg
};

Shale_Static_Driver_Define(sh1107, DRIVER_ID_SH1107, class_display, &_sh1107_ltype, _sh1107_event);
//driver_t *driver_sh1107;

driver_t *shale_driver_sh1107()
{
    return &_driver_sh1107;
}
void _sh1107_device_release(struct light_object *obj)
{
        _sh1107_device_free(to_device_interface(obj));
}
void _sh1107_device_free(struct device *device)
{
        shale_free(device_to_sh1107_device(device));
}

// write a single value of up to 4 bytes to the given address. smaller values are read from
// the low bytes of [write_data]
void sh1107_device_write(struct sh1107_device *device, uint16_t write_addr, size_t write_len, uint32_t write_data)
{

}
uint16_t sh1107_get_px_addr(device_t *device, point2_t pixel)
{

}

static device_t *_sh1107_device_alloc()
{
        struct sh1107_device *device = shale_malloc(sizeof(struct sh1107_device));
        return &device->header.header;
}
static uint8_t _sh1107_init(device_t *device_header)
{
        struct sh1107_device *device = device_to_sh1107_device(device_header);

        return LIGHT_OK;
}
static uint8_t _sh1107_add(device_t *device_header)
{
        struct sh1107_device *device = device_to_sh1107_device(device_header);

        return LIGHT_OK;
}
static uint8_t _sh1107_msg(device_t *device_header, message_handle_t *handle)
{
        struct sh1107_device *device = device_to_sh1107_device(device_header);

        return LIGHT_OK;
}
