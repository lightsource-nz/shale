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

typedef struct class_table {
    class_t *p_class;
    queue_t *queue;
    device_init_t init;
    msg_handler_t handler;
} class_table_t;
typedef struct driver_table {
    driver_t *driver;
    queue_t *queue;
    device_init_t init;
    msg_handler_t handler;
} driver_table_t;

class_table_t *_class_table_lookup(class_t *p_class);
driver_table_t *_driver_table_lookup(driver_t *driver);

uint32_t _handle_id_new();

#endif