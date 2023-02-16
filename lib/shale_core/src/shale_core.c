#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <pico/platform.h>
#ifdef PICO_RP2040
#include <pico/sync.h>
#endif
#include <string.h>

device_manager_t manager_default;

static void _device_instance_release(struct light_object *obj);
struct lobj_type ltype_device_instance = {
        .release = &_device_instance_release
};
#define SHALE_ID_DEVICE_MANAGER "device_manager"
static void _device_manager_release(struct light_object *obj);
struct lobj_type ltype_device_manager = {
        .id = SHALE_ID_DEVICE_MANAGER,
        .release = &_device_manager_release
};

#ifdef PICO_RP2040
extern int __shaledata_classes_start, __shaledata_classes_end;
extern int __shaledata_drivers_start, __shaledata_drivers_end;
extern int __shaledata_devices_start, __shaledata_devices_end;

static void shale_load_static_classes()
{
        uint8_t load_count = 0;
        const class_descriptor_t *next_class = (const class_descriptor_t *) &__shaledata_classes_start;
        while (next_class < (const class_descriptor_t *) &__shaledata_classes_end)
        {
                shale_class_static_add(next_class);
                load_count++;
                next_class++;
        }
        light_debug("preloaded %d device classes", load_count);
}
static void shale_load_static_drivers()
{
        uint8_t load_count = 0;
        const driver_descriptor_t *next_driver = (const driver_descriptor_t *) &__shaledata_drivers_start;
        while (next_driver < (const driver_descriptor_t *) &__shaledata_drivers_end)
        {
                shale_driver_static_add(next_driver);
                load_count++;
                next_driver++;
        }
        light_debug("preloaded %d device drivers", load_count);
}
static void shale_load_static_devices()
{
        uint8_t load_count = 0;
        const device_descriptor_t *next_device = (const device_descriptor_t *) &__shaledata_devices_start;
        while (next_device < (const device_descriptor_t *) &__shaledata_devices_end)
        {
                shale_device_static_add(next_device);
                load_count++;
                next_device++;
        }
        light_debug("preloaded %d devices", load_count);
}
#else
uint8_t static_class_count, static_driver_count, static_device_count = 0;
const class_descriptor_t *static_classes[SHALE_MAX_STATIC_CLASSES];
const driver_descriptor_t *static_drivers[SHALE_MAX_STATIC_DRIVERS];
const device_descriptor_t *static_devices[SHALE_MAX_STATIC_DEVICES];
static void shale_load_static_classes()
{
        for(uint8_t i = 0; i < static_class_count; i++) {
                shale_class_static_add(static_classes[i]);
        }
        light_debug("preloaded %d device classes",static_class_count);
}
static void shale_load_static_drivers()
{
        for(uint8_t i = 0; i < static_driver_count; i++) {
                shale_driver_static_add(static_drivers[i]);
        }
        light_debug("preloaded %d device drivers",static_driver_count);
}
static void shale_load_static_devices()
{
        for(uint8_t i = 0; i < static_device_count; i++) {
                shale_device_static_add(static_devices[i]);
        }
        light_debug("preloaded %d devices",static_device_count);
}
#endif

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
#define SHALE_DESCRIBE_BUFFER_LENGTH 128
static uint8_t _describe_buffer[SHALE_DESCRIBE_BUFFER_LENGTH];
uint8_t *shale_class_describe(const struct device_class *_class)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "device class, id=%s", _class->header.id);
        return _describe_buffer;
}
uint8_t *shale_driver_describe(const struct device_driver *driver)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "device driver, id=%s, class=%s", driver->header.id, driver->driver_class->header.id);
        return _describe_buffer;
}
uint8_t *shale_device_describe(const struct device *device)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "device, id=%s, class=%s, driver=%s", device->header.id,
                device->driver->header.id, device->driver->driver_class->header.id);
        return _describe_buffer;
}

uint8_t shale_init()
{
        light_info("Loading SHALE v%s...",SHALE_VERSION_STR);
        uint8_t retval;
        light_object_setup();
        shale_thread_init();
        shale_class_setup();

        if(retval = shale_device_manager_init(&manager_default, SHALE_MANAGER_DEFAULT_NAME)) {
                // TODO log error
                return retval;
        }
        shale_load_static_classes();
        shale_load_static_drivers();
        shale_load_static_devices();

        struct class_table *ctable = shale_class_table_main_get();
        for(uint8_t i = 0; i < ctable->count; i++) {
                struct device_class *_class = ctable->classes[i];
                light_debug("[%s]", shale_class_describe(_class));
                for(uint8_t j = 0; j < _class->driver_count; j++) {
                        light_debug("[%s]", shale_driver_describe(_class->drivers[j]));
                }
        }
        
        struct device_manager *devman = shale_device_manager_default();
        for(uint8_t i = 0; i < devman->device_count; i++) {
                light_debug("[%s]",shale_device_describe(devman->device_table[i]));
        }
    return LIGHT_OK;
}

struct device_manager *shale_device_manager_default()
{
    return &manager_default;
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
// TODO replace with platform independent message dispatch function
#ifdef PICO_RP2040
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
#else
void _dispatch_message_for_device(device_t *device) {}
#endif
uint8_t shale_device_manager_init(device_manager_t *devmgr, const uint8_t *id)
{
    light_object_init(&devmgr->header, &ltype_device_manager);
#ifdef PICO_RP2040
    devmgr->mq_lock = spin_lock_claim_unused(true);
#endif
    devmgr->scheduler_strategy = SHALE_MANAGER_STRATEGY_RR;
    devmgr->device_count = 0;

    uint8_t retval;
    if(retval = light_object_add(&devmgr->header, NULL, "%s", id)) {
        // log error
        return retval;
    }

    return LIGHT_OK;
}
// desc.init_device() takes (struct device *) and upcasts to full device type internally
uint8_t shale_device_static_add(const device_descriptor_t *desc)
{
        desc->driver->init_device(desc->object, desc->id);
        desc->object->header.is_static = 1;
}
uint8_t shale_device_init(device_t *dev, driver_t *dev_driver, struct lobj_type *type, const uint8_t *id)
{
    return shale_device_init_ctx(&manager_default, dev, dev_driver, type, id);
}
// TODO extract hardware-specific queueing code
uint8_t shale_device_init_ctx(device_manager_t *context, device_t *dev, driver_t *dev_driver, struct lobj_type *type, const uint8_t *id)
{
    assert_class(dev_driver->driver_class);
    assert_driver(dev_driver);
    // TODO add assertion to verify manager
    light_object_init(&dev->header, type);
#ifdef PICO_RP2040
    queue_init_with_spinlock(&dev->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, context->mq_lock);
#endif
    dev->driver = shale_driver_get(dev_driver);
    dev->state = SHALE_DEVICE_STATE_INIT;
    _device_register(context, dev, id);
    //dev_driver->driver_class->events.init(dev);
    //dev_driver->events.init(dev);
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
#ifdef PICO_RP2040
    return queue_try_peek(&device->queue, &handle);
#else
    return false;
#endif
}
message_handle_t *shale_device_message_get_next(device_t *device)
{

}
