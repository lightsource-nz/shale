#ifndef _DRIVER_SH1107_H
#define _DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"
#include "class/iobus.h"

#define DRIVER_ID_SH1107        "shale:driver:sh1107"

Shale_Static_Driver(sh1107);

struct sh1107_interface {
    struct display_interface header;
    struct iobus_consumer port;
};
#define to_sh1107_interface(ptr) container_of(ptr, struct sh1107_interface, header)
#define interface_to_sh1107_interface(ptr) to_sh1107_interface(to_display_interface(ptr))
#define lobject_to_sh1107_interface(ptr) interface_to_sh1107_interface(to_device_interface(ptr))

uint8_t shale_driver_sh1107_init();
driver_t *shale_driver_sh1107();
uint8_t shale_driver_sh1107_interface_init(struct device_interface *iface, const uint8_t *id);

#endif