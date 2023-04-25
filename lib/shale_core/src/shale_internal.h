#ifndef _SHALE_INTERNAL_H
#define _SHALE_INTERNAL_H

#ifndef __SHALE_SOURCE__
#   error __file__ " should not be included outside of the SHALE codebase"
#endif

#ifndef _SHALE_H
#   error __file__ " must be included after shale.h"
#endif

#include <stdlib.h>

#define SHALE_THREAD_ACTIVE         0x00
#define SHALE_THREAD_READY          0x01
#define SHALE_THREAD_BLOCKED        0x02

uint8_t _class_register(class_t *_class, const uint8_t *id);
uint8_t _driver_register(driver_t *driver, const uint8_t *id);
uint8_t _thread_register(shale_thread_t *thread);

void _service_message_queues();
void _dispatch_message_for_device(device_t *device);

#endif