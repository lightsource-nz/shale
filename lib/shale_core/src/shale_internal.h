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

int16_t _list_indexof(void *list[], uint8_t count, void *item);
void _list_delete_at_index(void *list[], uint8_t *count, uint8_t index);
void _list_delete_item(void *list[], uint8_t *count, void *item);

uint8_t _class_register(class_t *_class, const uint8_t *id);
uint8_t _driver_register(driver_t *driver, const uint8_t *id);
uint8_t _device_register(device_manager_t *context, device_t *device, const uint8_t *id);
uint8_t _thread_register(shale_thread_t *thread);

class_t *_class_table_lookup(uint8_t *id);
driver_t *_driver_table_lookup(uint8_t *id);
device_t *_device_table_lookup(uint8_t *id);

void _service_message_queues();
void _dispatch_message_for_device(device_t *device);

#endif