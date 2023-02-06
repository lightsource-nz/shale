#ifndef _IOBUS_INTERNAL_H
#define _IOBUS_INTERNAL_H

#ifndef _CLASS_IOBUS_H
#   error __file__ "must be included after class/display.h"
#endif

void shale_class_display_init_device(device_t *device);
Message_Handler(shale_class_display_handle_message);

#endif
