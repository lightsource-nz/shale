#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <pico/platform.h>
#ifdef PICO_RP2040
#include <pico/sync.h>
#endif
#include <string.h>

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
        return shale_interface_init_ctx_va(shale_device_manager_default(), ifx, driver, id_format, vargs);
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
        light_object_init(&ifx->header, driver->ifx_ltype);
#ifdef PICO_RP2040
        queue_init_with_spinlock(&interface->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, ctx->mq_lock);
#endif
        ifx->driver = shale_driver_get(driver);
        ifx->state = SHALE_INTERFACE_STATE_INIT;
        uint8_t retval;
        if(retval = shale_device_manager_add_interface(ctx, ifx, id_format, vargs)) {
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
        return shale_interface_new_ctx_va(shale_device_manager_default(), driver, id_format, vargs);
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
        return shale_interface_init_ref_ctx_va(shale_device_manager_default(), ifx, driver, ref_count, refs, id_format, vargs);
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
        light_object_init(&ifx->header, driver->ifx_ltype);
#ifdef PICO_RP2040
        queue_init_with_spinlock(&interface->queue,sizeof(message_handle_t), SHALE_QUEUE_DEPTH, ctx->mq_lock);
#endif
        ifx->driver = shale_driver_get(driver);
        for(uint8_t i = 0; i < ref_count; i++) {
                ifx->refs[i] = *refs[i];
        }
        ifx->state = SHALE_INTERFACE_STATE_INIT;
        uint8_t retval;
        if(retval = shale_device_manager_add_interface(ctx, ifx, id_format, vargs)) {
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
        return shale_interface_new_ref_ctx_va(shale_device_manager_default(), driver, ref_count, refs, id_format, vargs);
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
        if(!(ifx = driver->ifx_alloc())) {
                light_warn("failed to allocate memory for interface with id pattern '%s'", id_format);
                return NULL;
        }

        uint8_t retval;
        if(retval = shale_interface_init_ref_ctx_va(ctx, ifx, driver, ref_count, refs, id_format, vargs)) {
                light_warn("failed to initialize new interface with id pattern '%s': %s", id_format, light_error_to_string(retval));
                driver->ifx_free(ifx);
                return NULL;
        }
        return ifx;
}
