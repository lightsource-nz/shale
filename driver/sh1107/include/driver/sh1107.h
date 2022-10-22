#ifndef _DRIVER_SH1107_H
#define _DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"

#include "oled.h"

#define DRIVER_ID_SH1107        "shale:driver:sh1107"

typedef struct sh1107_data {
    ioport_t *port;
} sh1107_data_t;

typedef struct sh1107_device {
    uint8_t name[NAME_LENGTH];
    class_t *dev_class;
    driver_t *driver;
    display_data_t *class_data;
    sh1107_data_t *driver_data;
} sh1107_device_t;

void shale_driver_sh1107_init();
driver_t *shale_driver_sh1107();
void shale_driver_sh1107_init_device(device_t *device);

#endif