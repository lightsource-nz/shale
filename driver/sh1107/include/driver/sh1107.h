#ifndef _SHALE_DRIVER_SH1107_H
#define _SHALE_DRIVER_SH1107_H

#include "shale.h"
#include "class/display.h"

#include "oled.h"

typedef struct sh1107_data {
    uint8_t transport_type;
    void *transport;
} sh1107_data_t;

void shale_driver_sh1107_init();

#endif