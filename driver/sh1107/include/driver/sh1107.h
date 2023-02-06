#ifndef _DRIVER_SH1107_H
#define _DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"
#include "class/iobus.h"

#define DRIVER_ID_SH1107        "shale:driver:sh1107"

Shale_Static_Driver(sh1107);

struct sh1107_device {
    display_device_t header;
    struct iobus_consumer port;
};
#define to_sh1107_device(ptr) container_of(ptr, struct sh1107_device, header)
#define device_to_sh1107_device(ptr) to_sh1107_device(to_display_device(ptr))
#define lobject_to_sh1107_device(ptr) device_to_sh1107_device(to_device_instance(ptr))

uint8_t shale_driver_sh1107_init();
driver_t *shale_driver_sh1107();
uint8_t shale_driver_sh1107_device_init(struct sh1107_device *device, const uint8_t *id);

#endif