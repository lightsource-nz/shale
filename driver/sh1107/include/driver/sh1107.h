#ifndef _DRIVER_SH1107_H
#define _DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"

#include "oled.h"

#define DRIVER_ID_SH1107        "shale:driver:sh1107"

Shale_Static_Driver(driver_sh1107);

typedef struct sh1107_device {
    display_device_t header;
    struct ioport_consumer port;
} sh1107_device_t;
#define to_sh1107_device(ptr) container_of(ptr, sh1107_device_t, header)
#define lobject_to_sh1107_device(ptr) to_sh1107_device(to_display_device(to_device_instance(ptr)))

uint8_t shale_driver_sh1107_init();
driver_t *shale_driver_sh1107();
uint8_t shale_driver_sh1107_device_init(sh1107_device_t *device, const uint8_t *id);

#endif