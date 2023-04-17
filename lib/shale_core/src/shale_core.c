#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <pico/platform.h>
#ifdef PICO_RP2040
#include <pico/sync.h>
#endif
#include <string.h>

device_manager_t manager_default;

#define SHALE_ID_LTYPE_DEVICE_MANAGER "device_manager"
static void _device_manager_release(struct light_object *obj);
struct lobj_type ltype_device_manager = {
        .id = SHALE_ID_LTYPE_DEVICE_MANAGER,
        .release = &_device_manager_release
};
#define SHALE_ID_LTYPE_DEVICE "device_instance"
static void _device_release(struct light_object *obj);
struct lobj_type ltype_device = {
        .id = SHALE_ID_LTYPE_DEVICE,
        .release = &_device_release
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
uint8_t static_class_count, static_driver_count, static_interface_count, static_device_count = 0;
const class_descriptor_t *static_classes[SHALE_MAX_STATIC_CLASSES];
const driver_descriptor_t *static_drivers[SHALE_MAX_STATIC_DRIVERS];
const struct interface_descriptor *static_interfaces[SHALE_MAX_STATIC_INTERFACES];
const device_descriptor_t *static_devices[SHALE_MAX_STATIC_DEVICES];
static void shale_load_static_classes()
{
        for(uint8_t i = 0; i < static_class_count; i++) {
                shale_class_static_add(static_classes[i]);
        }
        light_debug("preloaded %d device classes", static_class_count);
}
static void shale_load_static_drivers()
{
        for(uint8_t i = 0; i < static_driver_count; i++) {
                shale_driver_static_add(static_drivers[i]);
        }
        light_debug("preloaded %d device drivers", static_driver_count);
}
static void shale_load_static_interfaces()
{
        for(uint8_t i = 0; i < static_interface_count; i++) {
                shale_interface_static_add(static_interfaces[i]);
        }
        light_debug("preloaded %d interfaces", static_interface_count);
}
static void shale_load_static_devices()
{
        for(uint8_t i = 0; i < static_device_count; i++) {
                shale_device_static_add(static_devices[i]);
        }
        light_debug("preloaded %d devices", static_device_count);
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
device_t *_device_lookup(device_manager_t *context, const uint8_t *id)
{
    for(int i = 0; i < context->device_count; i++) {
        if(strcmp(light_object_get_name(&context->device_table[i]->header), id))
            return context->device_table[i];
    }
    return NULL;
}
struct device_interface *_interface_lookup(struct device_manager *context, const uint8_t *id)
{
    for(int i = 0; i < context->interface_count; i++) {
        if(strcmp(light_object_get_name(&context->interface_table[i]->header), id))
            return context->interface_table[i];
    }
    return NULL;
}
static void _device_release(struct light_object *obj)
{
        struct device *device = to_device_instance(obj);
        for(uint8_t i = 0; i < device->if_count; i++) {
                shale_interface_put(device->interface[i]);
        }

        shale_free(device);
}
static void _device_manager_release(struct light_object *obj)
{
        struct device_manager *devman = to_device_manager(obj);
#ifdef PICO_RP2040
        spin_lock_unclaim(devman->mq_lock);
#endif
        shale_free(to_device_manager(obj));
}
#define SHALE_DESCRIBE_BUFFER_LENGTH 128
static uint8_t _describe_buffer[SHALE_DESCRIBE_BUFFER_LENGTH];
uint8_t *shale_class_describe(struct device_class *_class)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "CLASS: id=%s", shale_class_name(_class));
        return _describe_buffer;
}
uint8_t *shale_driver_describe(struct device_driver *driver)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "DRIVER: id=%s, class=%s", shale_driver_name(driver),
                                        shale_class_name(driver->driver_class));
        return _describe_buffer;
}
uint8_t *shale_interface_describe(struct device_interface *iface)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "INTERFACE: id=%s, class=%s, driver=%s", shale_interface_name(iface),
                shale_class_name(iface->driver->driver_class), shale_driver_name(iface->driver));
        return _describe_buffer;
}
uint8_t *shale_device_describe(struct device *device)
{
        snprintf(_describe_buffer, SHALE_DESCRIBE_BUFFER_LENGTH,
                "DEVICE: id=%s, interfaces: %d", shale_device_name(device), device->if_count);
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
        shale_load_static_interfaces();
        shale_load_static_devices();

        struct class_table *ctable = shale_class_table_main_get();
        for(uint8_t i = 0; i < ctable->count; i++) {
                struct device_class *_class = ctable->classes[i];
                light_debug("[%s]", shale_class_describe(_class));
                for(uint8_t j = 0; j < _class->driver_count; j++) {
                        light_debug("+--[%s]", shale_driver_describe(_class->drivers[j]));
                }
        }
        
        struct device_manager *devman = shale_device_manager_default();
        for(uint8_t i = 0; i < devman->device_count; i++) {
                light_debug("[%s]",shale_device_describe(devman->device_table[i]));
                for(uint8_t j = 0; j < devman->device_table[i]->if_count; j++) {
                        light_debug("+--[%s]",shale_interface_describe(devman->device_table[i]->interface[j]));
                }
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
void _dispatch_message_for_device(device_t *device)
{
        light_debug("device '%s' totally just handled a message", shale_device_name(device));
}
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

uint8_t shale_interface_static_add(const struct interface_descriptor *desc)
{
        struct device_interface *ifx = desc->object;
        struct interface_ref refs[SHALE_INTERFACE_MAX_REFS];
        for(uint8_t i = 0; i < desc->ref_count; i++) {
                refs[i] = (struct interface_ref) { .ref_id = desc->refs[i].ref_id, .target = desc->refs[i].ref->object };
        }
        uint8_t retval;
        if(retval = shale_interface_init(ifx, desc->if_driver->object, "%s", desc->id)) {
                light_debug("shale_interface_init failed with code %s", light_error_to_string(retval));
                return retval;
        }

        ifx->header.is_static = true;

        return LIGHT_OK;
}
uint8_t shale_interface_init(struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_interface_init_va(ifx, driver, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_interface_init_va(struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, va_list vargs)
{
        return shale_interface_init_ctx_va(&manager_default, ifx, driver, id_format, vargs);
}
uint8_t shale_interface_init_ctx(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_interface_init_ctx_va(ctx, ifx, driver, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_interface_init_ctx_va(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, va_list vargs)
{
        light_object_init(&ifx->header, driver->interface_ltype);
#ifdef PICO_RP2040
        queue_init_with_spinlock(&interface->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, ctx->mq_lock);
#endif
        ifx->driver = shale_driver_get(driver);
        ifx->state = SHALE_INTERFACE_STATE_INIT;
        uint8_t retval;
        if(retval = _device_manager_add_interface(ctx, ifx, id_format, vargs)) {
                light_warn("failed to create new interface '%s' at context '%s", id_format, ctx->header.id);
#ifdef PICO_RP2040
                queue_free(&interface->queue);
#endif
                shale_driver_put(driver);
                return retval;
        }
        driver->driver_class->events.init(ifx);
        driver->events.init(ifx);
        ifx->state = SHALE_INTERFACE_STATE_ACTIVE;
        return LIGHT_OK;
}
struct device_interface *shale_interface_new(driver_t *driver, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_interface_new_va(driver, id_format, vargs);
        va_end(vargs);
}
struct device_interface *shale_interface_new_va(driver_t *driver, const uint8_t *id_format, va_list vargs)
{
        return shale_interface_new_ctx_va(&manager_default, driver, id_format, vargs);
}
struct device_interface *shale_interface_new_ctx(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_interface_new_ctx_va(ctx, driver, id_format, vargs);
        va_end(vargs);
}
struct device_interface *shale_interface_new_ctx_va(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, va_list vargs)
{
        return shale_interface_new_ref_ctx_va(ctx, driver, 0, NULL, id_format, vargs);
}
uint8_t shale_interface_init_ref(struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_interface_init_ref_va(ifx, driver, ref_count, refs, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_interface_init_ref_va(struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs)
{
        return shale_interface_init_ref_ctx_va(&manager_default, ifx, driver, ref_count, refs, id_format, vargs);
}
uint8_t shale_interface_init_ref_ctx(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_interface_init_ref_ctx_va(ctx, ifx, driver, ref_count, refs, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_interface_init_ref_ctx_va(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs)
{
        light_object_init(&ifx->header, driver->interface_ltype);
#ifdef PICO_RP2040
        queue_init_with_spinlock(&interface->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, ctx->mq_lock);
#endif
        ifx->driver = shale_driver_get(driver);
        for(uint8_t i = 0; i < ref_count; i++) {
                ifx->refs[i] = *refs[i];
        }
        ifx->state = SHALE_INTERFACE_STATE_INIT;
        uint8_t retval;
        if(retval = _device_manager_add_interface(ctx, ifx, id_format, vargs)) {
                light_warn("failed to create new interface '%s' at context '%s", id_format, ctx->header.id);
#ifdef PICO_RP2040
                queue_free(&interface->queue);
#endif
                shale_driver_put(driver);
                return retval;
        }
        driver->driver_class->events.init(ifx);
        driver->events.init(ifx);
        ifx->state = SHALE_INTERFACE_STATE_ACTIVE;
        return LIGHT_OK;

}
struct device_interface *shale_interface_new_ref(driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_interface_new_ref_va(driver, ref_count, refs, id_format, vargs);
        va_end(vargs);
}
struct device_interface *shale_interface_new_ref_va(driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs)
{
        return shale_interface_new_ref_ctx_va(&manager_default, driver, ref_count, refs, id_format, vargs);
}
struct device_interface *shale_interface_new_ref_ctx(device_manager_t *ctx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_interface_new_ref_ctx_va(ctx, driver, ref_count, refs, id_format, vargs);
        va_end(vargs);
}
struct device_interface *shale_interface_new_ref_ctx_va(device_manager_t *ctx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs)
{
        struct device_interface *ifx;
        if(!(ifx = driver->events.alloc())) {
                light_warn("failed to allocate memory for interface with id pattern '%s'", id_format);
                return NULL;
        }

        uint8_t retval;
        if(retval = shale_interface_init_ref_ctx_va(ctx, ifx, driver, ref_count, refs, id_format, vargs)) {
                light_warn("failed to initialize new interface with id pattern '%s': %s", id_format, light_error_to_string(retval));
                driver->events.free(ifx);
                return NULL;
        }
        return ifx;
}
struct device_interface *shale_interface_find(const uint8_t *id)
{
        return shale_interface_find_ctx(&manager_default, id);
}
struct device_interface *shale_interface_find_ctx(device_manager_t *ctx, const uint8_t *id)
{
        return _interface_lookup(ctx, id);
}

uint8_t shale_device_static_add(const device_descriptor_t *desc)
{
        struct device *device = desc->object;
        struct device_interface *if_list[SHALE_DEVICE_MAX_INTERFACES];
        uint8_t i;
        for(i = 0; i < desc->if_count; i++) {
                if_list[i] = desc->interface[i]->object;
        }

        uint8_t retval;
        if(retval = shale_device_init_composite(device, desc->if_count, if_list, "%s", desc->id)) {
                return retval;
        }

        device->header.is_static = 1;
        return LIGHT_OK;
}
device_t *shale_device_new(struct device_interface *if_main, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_device_new_va(if_main, id_format, vargs);
        va_end(vargs);
}
device_t *shale_device_new_va(struct device_interface *if_main, const uint8_t *id_format, va_list vargs)
{
        return shale_device_new_ctx_va(&manager_default, if_main, id_format, vargs);
}
device_t *shale_device_new_ctx(device_manager_t *ctx, struct device_interface *if_main, const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_device_new_ctx_va(ctx, if_main, id_format, vargs);
        va_end(vargs);
}
device_t *shale_device_new_ctx_va(device_manager_t *ctx, struct device_interface *if_main, const uint8_t *id_format, va_list vargs)
{
        struct device_interface *if_list[] = { if_main };
        return shale_device_new_composite_ctx_va(ctx, 1, if_list, id_format, vargs);
}
device_t *shale_device_new_composite(uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_device_new_composite_va(if_count, interface, id_format, vargs);
        va_end(vargs);
}
device_t *shale_device_new_composite_va(uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, va_list vargs)
{
        return shale_device_new_composite_ctx_va(&manager_default, if_count, interface, id_format, vargs);
}
device_t *shale_device_new_composite_ctx(device_manager_t *ctx, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, ...)
{
        va_list vargs;
        va_start(vargs, id_format);
        return shale_device_new_composite_ctx_va(ctx, if_count, interface, id_format, vargs);
        va_end(vargs);
}
device_t *shale_device_new_composite_ctx_va(device_manager_t *ctx, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, va_list vargs)
{
        struct device *device;
        if(!(device = shale_malloc(sizeof(struct device)))) {
                light_warn("failed to allocate device memory for id pattern '%s'", id_format);
                return NULL;
        }
        uint8_t retval;
        if(retval = shale_device_init_composite_ctx_va(ctx, device, if_count, interface, id_format, vargs)) {
                shale_free(device);
                return NULL;
        }

        return device;
}

uint8_t shale_device_init(struct device *device, struct device_interface *if_main, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_device_init_va(device, if_main, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_device_init_va(struct device *device, struct device_interface *if_main, const uint8_t *id_format, va_list vargs)
{
        return shale_device_init_ctx_va(&manager_default, device, if_main, id_format, vargs);
}
uint8_t shale_device_init_ctx(device_manager_t *ctx, struct device *device, struct device_interface *if_main, const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_device_init_ctx_va(ctx, device, if_main, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_device_init_ctx_va(device_manager_t *ctx, struct device *device, struct device_interface *if_main, const uint8_t *id_format, va_list vargs)
{
        struct device_interface *if_list[] = { if_main };
        return shale_device_init_composite_ctx_va(ctx, device, 1, if_list, id_format, vargs);
}
uint8_t shale_device_init_composite(struct device *device, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_device_init_composite_va(device, if_count, interface, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_device_init_composite_va(struct device *device, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, va_list vargs)
{
        return shale_device_init_composite_ctx_va(&manager_default, device, if_count, interface, id_format, vargs);
}
uint8_t shale_device_init_composite_ctx(device_manager_t *ctx, struct device *device, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, ...)
{
        va_list vargs;

        va_start(vargs, id_format);
        return shale_device_init_composite_ctx_va(ctx, device, if_count, interface, id_format, vargs);
        va_end(vargs);
}
uint8_t shale_device_init_composite_ctx_va(device_manager_t *ctx, struct device *device, uint8_t if_count, struct device_interface *interface[], const uint8_t *id_format, va_list vargs)
{
        light_object_init(&device->header, &ltype_device);
        
        device->state = SHALE_DEVICE_STATE_INIT;
        for(uint8_t i = 0; i < if_count; i++) {
                // TODO clean up these counted references in release method
                device->interface[i] = shale_interface_get(interface[i]);
                device->if_count++;
        }

        device->if_main = device->interface[0];
        uint8_t retval;
        if(retval = _device_manager_add_device(ctx, device, id_format, vargs)) {
                light_warn("failed to create new device '%s' at context '%s", id_format, ctx->header.id);
                for(uint8_t i = 0; i < device->if_count; i++) {
                        shale_interface_put(device->interface[i]);
                }
                shale_free(device);
                return retval;
        }
        for(uint8_t i = 0; i < device->if_count; i++) {
                device->interface[i]->state = SHALE_INTERFACE_STATE_ACTIVE;
                device->interface[i]->driver->events.add(device->interface[i]);
                device->interface[i]->driver->driver_class->events.add(device->interface[i]);
        }
        device->state = SHALE_DEVICE_STATE_ACTIVE;
        return LIGHT_OK;
}
device_t *shale_device_find(const uint8_t *id)
{
        return shale_device_find_ctx(&manager_default, id);
}
device_t *shale_device_find_ctx(device_manager_t *ctx, const uint8_t *id)
{
        return _device_lookup(ctx, id);
}
uint8_t _device_manager_add_device(device_manager_t *context, device_t *device, const uint8_t *id_format, va_list vargs)
{
        if(context->device_count >= SHALE_MANAGER_MAX_DEVICES)
                return ERROR_MAX_ENTITIES;
        // TODO verify that device id is unique for this DM
        int retval;
        if(retval = light_object_add_va(&device->header, &context->header, id_format, vargs)) {
                light_debug("failed to add device with id pattern '%s' to device manager '%s' (%s)",
                                        id_format, context->header.id, light_error_to_string(retval));
                return retval;
        }
        context->device_table[context->device_count++] = device;
        return SHALE_SUCCESS;
}
uint8_t _device_manager_add_interface(device_manager_t *context, struct device_interface *interface, const uint8_t *id_format, va_list vargs)
{
        if(context->interface_count >= SHALE_MANAGER_MAX_DEVICES)
                return ERROR_MAX_ENTITIES;
        // TODO verify that interface id is unique for this DM
        int retval;
        if(retval = light_object_add_va(&interface->header, &context->header, id_format, vargs)) {
                light_debug("failed to add interface with id pattern '%s' to device manager '%s' (%s)",
                                        id_format, context->header.id, light_error_to_string(retval));
                return retval;
        }
        context->interface_table[context->interface_count++] = interface;
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
