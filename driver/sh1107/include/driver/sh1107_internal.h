#ifndef _SH1107_INTERNAL_H
#define _SH1107_INTERNAL_H

#ifndef _DRIVER_SH1107_H
#   error __file__ "must be included after class/display.h"
#endif

void shale_driver_sh1107_init_device(device_t *device);
Message_Handler(shale_driver_sh1107_handle_message);

#endif
