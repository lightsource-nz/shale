#ifndef _SHALE_DRIVER_SH1107_H
#define _SHALE_DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"

#include "oled.h"

typedef struct sh1107_data {
    uint8_t transport_type;
    void *transport;
} sh1107_data_t;

typedef struct sh1107_device {
    uint8_t name[NAME_LENGTH];
    class_t *dev_class;
    driver_t *driver;
    display_data_t *class_data;
    sh1107_data_t *driver_data;
} sh1107_device_t;

void shale_driver_sh1107_init();

#endif