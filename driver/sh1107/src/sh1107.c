#include "driver/sh1107.h"

#include <stdlib.h>

static driver_t driver_sh1107 = {

};

void shale_driver_sh1107_init()
{
    shale_driver_create(&driver_sh1107);
}
