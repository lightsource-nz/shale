#ifndef _SHALE_INTERNAL_H
#define _SHALE_INTERNAL_H

#ifndef __SHALE_SOURCE__
#   error __file__ " should not be included outside of the SHALE codebase"
#endif

#ifndef _SHALE_H
#   error __file__ " must be included after shale.h"
#endif

#include <pico/util/queue.h>
#include <stdlib.h>

#define SHALE_THREAD_ACTIVE         0x00
#define SHALE_THREAD_READY          0x01
#define SHALE_THREAD_BLOCKED        0x02

typedef struct thread_condition {
    msg_handle_t *handle;
    uint8_t status_awaited;
} thread_condition_t;

uint8_t _class_register(class_t *_class);
uint8_t _driver_register(driver_t *driver);
uint8_t _device_register(device_t *device);

class_t *_class_table_lookup(uint8_t *id);
driver_t *_driver_table_lookup(uint8_t *id);
device_t *_device_table_lookup(uint8_t *id);

void _dispatch_message_for_device(device_t *device);

#endif