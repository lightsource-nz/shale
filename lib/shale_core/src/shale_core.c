#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <pico/sync.h>
#include <string.h>

extern void const *__shaledata_classes_start;
extern void const *__shaledata_classes_end;
extern void const *__shaledata_drivers_start;
extern void const *__shaledata_drivers_end;
extern void const *__shaledata_devices_start;
extern void const *__shaledata_devices_end;

device_manager_t *manager_default;

int16_t _list_indexof(void *list[], uint8_t count, void *item)
{
    for(uint8_t i = 0; i < count; i++) {
        if(list[i] == item)
            return i;
    }
    return -1;
}
void _list_delete_at_index(void *list[], uint8_t *count, uint8_t index)
{
    for(uint8_t i = index; i < *count; i++) {
        list[i] = list[i + 1];
    }
    *count--;
}
void _list_delete_item(void *list[], uint8_t *count, void *item)
{
    int16_t index;
    if((index = _list_indexof(list, *count, item)) >= 0)
        _list_delete_at_index(list, count, index);
}
device_t *_device_lookup(device_manager_t *context, uint8_t *id)
{
    for(int i = 0; i < SHALE_MAX_DEVICES; i++) {
        if(strcmp(context->device_table[i]->id, id))
            return context->device_table[i];
    }
    return NULL;
}

void shale_init()
{
    shale_thread_init();
    manager_default = shale_device_manager_new(SHALE_MANAGER_DEFAULT_NAME);
    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
}

void shale_thread_yield()
{
    shale_thread_current()->thread_state = SHALE_THREAD_READY;
    _service_message_queues();
}
void shale_service_message_queues()
{
    shale_device_manager_service_message_queues(manager_default);
}
void shale_device_manager_service_message_queues(device_manager_t *context)
{
    
}

void _service_message_queues()
{

}

void _dispatch_message_for_device(device_t *device)
{
    msg_handle_t *handle;
    while(queue_try_peek(device->queue, &handle)) {
        uint8_t status = device->driver->driver_class->events.message(handle->msg.target, handle);
        switch(status) {
            case MX_DONE:
            break;
            case MX_FORWARD:
            queue_t *next_q = handle->dest->queue;
            if(queue_try_add(next_q, handle)) {
                if(!queue_try_remove(device->queue, &handle)) {
                    // TODO log queueing error

                }
            }
            else
            {
                // TODO log overflow condition
            }
            break;
        }
    }

}
device_manager_t *shale_device_manager_new(uint8_t *id)
{
    device_manager_t *obj = shale_malloc(sizeof(device_manager_t));
    obj->mq_lock = spin_lock_claim_unused(true);
    strcpy(id, obj->id);
    obj->scheduler_strategy = SHALE_MANAGER_STRATEGY_RR;
    obj->device_count = 0;

    return obj;
}
// allocate device memory, and call class/driver init handlers
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver)
{
    shale_device_manager_device_new(manager_default, id, dev_driver);
}

// TODO extract hardware-specific queueing code
device_t *shale_device_manager_device_new(device_manager_t *context, uint8_t *id, driver_t *dev_driver)
{
    assert_class(dev_driver->driver_class);
    assert_driver(dev_driver);
    // TODO add assertion to verify manager
    device_t *device_obj = shale_malloc(sizeof(device_t));
    device_obj->queue = shale_malloc(sizeof(queue_t));
    queue_init_with_spinlock(device_obj->queue,sizeof(msg_handle_t), SHALE_QUEUE_DEPTH, context->mq_lock);
    device_obj->class_data = shale_malloc(dev_driver->driver_class->data_length);
    device_obj->driver_data = shale_malloc(dev_driver->data_length);
    strcpy(id, device_obj->id);
    device_obj->driver = dev_driver;
    device_obj->state = SHALE_DEVICE_STATE_INIT;
    _device_register(context, device_obj);
    dev_driver->driver_class->events.init(device_obj);
    dev_driver->events.init(device_obj);
    return device_obj;
}
uint8_t _device_register(device_manager_t *context, device_t *device)
{
    if(context->device_count >= SHALE_MANAGER_MAX_DEVICES)
        return ERROR_MAX_ENTITIES;
    context->device_table[context->device_count++] = device;
    device->context = context;
    return SHALE_SUCCESS;
}
