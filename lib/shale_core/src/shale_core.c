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

struct lobj_type ltype_device_instance = {
        .release = &_device_instance_release
};
struct lobj_type ltype_device_manager = {
        .release = &_device_manager_release
};

device_manager_t manager_default;

static void _device_instance_release(struct light_object *obj);

static void _device_manager_release(struct light_object *obj);

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
        if(strcmp(light_object_get_name(&context->device_table[i]->header), id))
            return context->device_table[i];
    }
    return NULL;
}
static void _device_instance_release(struct light_object *obj)
{
    free(to_device_instance(obj));
}
static void _device_manager_release(struct light_object *obj)
{
    free(to_device_manager(obj));
}

uint8_t shale_init()
{
    uint8_t retval;
    light_object_setup();
    shale_thread_init();
    if(retval = shale_device_manager_init(&manager_default, SHALE_MANAGER_DEFAULT_NAME)) {
        // TODO log error
        return retval;
    }

    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
    return LIGHT_OK;
}
#define SERVICE_COUNT_INF UINT16_MAX
void shale_service_message_queues()
{
    shale_service_message_queues_n(SERVICE_COUNT_INF);
}
void shale_service_message_queues_n(uint16_t count)
{
    shale_device_manager_service_message_queues_n(&manager_default, count);
}
void _service_queues_rr(device_manager_t *context, uint16_t count);
void shale_device_manager_service_message_queues(device_manager_t *context)
{
    shale_device_manager_service_message_queues_n(context, SERVICE_COUNT_INF);
}
void shale_device_manager_service_message_queues_n(device_manager_t *context, uint16_t count)
{
    switch(context->scheduler_strategy) {
    case SHALE_MANAGER_STRATEGY_RR:
        _service_queues_rr(context, count);
    }
}

void _service_queues_rr(device_manager_t *context, uint16_t count)
{
    uint16_t dispatch_count = 0;
    device_t *device;
    message_t *message;
    for(int i = 0; i < context->device_count; i++) {
        device = context->device_table[i];
        if(shale_device_message_pending(device)) {
            _dispatch_message_for_device(device);
            if(count != SERVICE_COUNT_INF && ++dispatch_count == count)
                break;
        }
    }
}
void _service_message_queues()
{

}

void _dispatch_message_for_device(device_t *device)
{
    message_handle_t *handle;
    if(queue_try_peek(&device->queue, &handle)) {
        uint8_t status = device->driver->driver_class->events.message(handle->msg.target, handle);
        switch(status) {
            case MX_DONE:
            break;
            case MX_FORWARD: ;
            queue_t *next_q = &handle->dest->queue;
            if(queue_try_add(next_q, handle)) {
                if(!queue_try_remove(&device->queue, &handle)) {
                    light_fatal("queueing error condition 000 on device %s, message ID %x", light_object_get_name(&device->header), handle->msg.msg_id);
                }
            }
            break;
            case MX_DELEGATE:
            // TODO need a sensible way to allow class handlers to chain to driver specific handler
            break;
        }
    }

}
uint8_t shale_device_manager_init(device_manager_t *devmgr, const uint8_t *id)
{
    light_object_init(&devmgr->header, &ltype_device_manager);
    devmgr->mq_lock = spin_lock_claim_unused(true);
    devmgr->scheduler_strategy = SHALE_MANAGER_STRATEGY_RR;
    devmgr->device_count = 0;

    uint8_t retval;
    if(retval = light_object_add(&devmgr->header, NULL, "%s", id)) {
        // log error
        return retval;
    }

    return LIGHT_OK;
}
// allocate device memory, and call class/driver init handlers
device_t *shale_device_init(device_t *dev, uint8_t *id, driver_t *dev_driver)
{
    shale_device_init_ctx(&manager_default, dev, id, dev_driver);
}

// TODO extract hardware-specific queueing code
uint8_t shale_device_init_ctx(device_manager_t *context, device_t *dev, uint8_t *id, driver_t *dev_driver)
{
    assert_class(dev_driver->driver_class);
    assert_driver(dev_driver);
    // TODO add assertion to verify manager
    // light_object_init(&dev->header, &ltype_device_instance);
    queue_init_with_spinlock(&dev->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, context->mq_lock);
    dev->driver = to_device_driver(light_object_get(&dev_driver->header));
    dev->state = SHALE_DEVICE_STATE_INIT;
    _device_register(context, dev, id);
    dev_driver->driver_class->events.init(dev);
    dev_driver->events.init(dev);
    return LIGHT_OK;
}
uint8_t _device_register(device_manager_t *context, device_t *device, const uint8_t *id)
{
    if(context->device_count >= SHALE_MANAGER_MAX_DEVICES)
        return ERROR_MAX_ENTITIES;
    // TODO verify that device id is unique for this DM
    int retval;
    if(!(retval = light_object_add(&device->header, &context->header, "%s", id)))
        return retval;
    context->device_table[context->device_count++] = device;
    return SHALE_SUCCESS;
}

bool shale_device_message_pending(device_t *device)
{
    message_handle_t *handle;
    return queue_try_peek(&device->queue, &handle);
}
message_handle_t *shale_device_message_get_next(device_t *device)
{

}
