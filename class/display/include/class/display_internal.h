#ifndef _DISPLAY_INTERNAL_H
#define _DISPLAY_INTERNAL_H

#ifndef _CLASS_DISPLAY_H
#   error __file__ "must be included after class/display.h"
#endif

void shale_class_display_init_device(device_t *device);
Message_Handler(shale_class_display_handle_message);

#endif
