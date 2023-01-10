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

static void _device_instance_release(struct light_object *obj);

struct lobj_type ltype_device_instance = {
        .release = &_device_instance_release
};

static void _device_manager_release(struct light_object *obj);

struct lobj_type ltype_device_manager = {
        .release = &_device_manager_release
};

uint8_t shale_log_buffer[SHALE_LOG_BUFFER_SIZE];

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

void shale_init()
{
    shale_thread_init();
    manager_default = shale_device_manager_new(SHALE_MANAGER_DEFAULT_NAME);
    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
}
#define SERVICE_COUNT_INF UINT16_MAX
void shale_service_message_queues()
{
    shale_service_message_queues_n(SERVICE_COUNT_INF);
}
void shale_service_message_queues_n(uint16_t count)
{
    shale_device_manager_service_message_queues_n(manager_default, count);
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
    if(queue_try_peek(device->queue, &handle)) {
        uint8_t status = device->driver->driver_class->events.message(handle->msg.target, handle);
        switch(status) {
            case MX_DONE:
            break;
            case MX_FORWARD:
            queue_t *next_q = handle->dest->queue;
            if(queue_try_add(next_q, handle)) {
                if(!queue_try_remove(device->queue, &handle)) {
                    shale_fatal("queueing error condition 000 on device %s, message ID %x", light_object_get_name(&device->header), handle->msg.msg_id);
                }
            }
            break;
            case MX_DELEGATE:
            // TODO need a sensible way to allow class handlers to chain to driver specific handler
            break;
        }
    }

}
device_manager_t *shale_device_manager_new(uint8_t *id)
{
    device_manager_t *obj = shale_malloc(sizeof(device_manager_t));
    light_object_init(&obj->header, &ltype_device_manager);
    obj->mq_lock = spin_lock_claim_unused(true);
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
    light_object_init(&device_obj->header, &ltype_device_instance);
    device_obj->queue = shale_malloc(sizeof(queue_t));
    queue_init_with_spinlock(device_obj->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, context->mq_lock);
    device_obj->class_data = shale_malloc(dev_driver->driver_class->data_length);
    device_obj->driver_data = shale_malloc(dev_driver->data_length);
    device_obj->driver = to_device_driver(light_object_get(&dev_driver->header));
    device_obj->state = SHALE_DEVICE_STATE_INIT;
    _device_register(context, device_obj, id);
    dev_driver->driver_class->events.init(device_obj);
    dev_driver->events.init(device_obj);
    return device_obj;
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
    device->context = context;
    return SHALE_SUCCESS;
}

bool shale_device_message_pending(device_t *device)
{
    message_handle_t *handle;
    return queue_try_peek(device->queue, &handle);
}
message_handle_t *shale_device_message_get_next(device_t *device)
{

}
