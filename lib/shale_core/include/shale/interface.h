/*
 *      shale/interface.h
 *      
 *      defines the fundamental system object type representing a concrete messaging
 *      endpoint in a running application. every system message is either addressed to,
 *      or emanates from, some specific interface. interfaces in a running system are
 *      uniquely identified by an ID string, the content of which may be either defined
 *      at compile time or generated programmatically.
 * 
 *      authored by Alex Fulton, 2023
 */
#ifndef _SHALE_INTERFACE_H
#define _SHALE_INTERFACE_H

#ifdef PICO_RP2040
#include <pico/util/queue.h>
#endif

#define SHALE_INTERFACE_STATE_INIT         0
#define SHALE_INTERFACE_STATE_ACTIVE       1
#define SHALE_INTERFACE_STATE_SUSPEND      2
#define SHALE_INTERFACE_STATE_ERROR        UINT8_MAX

// codes for interface reference state machine
#define REF_INIT                0
#define REF_READY               1
#define REF_NEGOTIATE           2
#define REF_ATTACH              3
#define REF_ERROR               UINT8_MAX

struct interface_ref {
        const struct device_class *ref_type;
        const uint8_t *ref_id;
        uint8_t state;
        struct device_interface *target;
};
// TODO provide a platform agnostic message queueing interface which also
// works in host mode
typedef struct device_interface {
#ifdef PICO_RP2040
    queue_t queue;
#endif
    struct device *parent;
    struct light_object header;
    struct device_driver *driver;
    uint8_t state;
    uint8_t ref_count;
    struct interface_ref refs[SHALE_INTERFACE_MAX_REFS];
} interface_t;

struct sh_event {
    struct light_message header;
    uint16_t id;
    struct interface *source;
};

extern struct lobj_type ltype_interface;

#define to_device_interface(object) container_of(object, interface_t, header)

extern uint8_t shale_interface_init(struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, ...);
extern uint8_t shale_interface_init_va(struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, va_list vargs);
extern uint8_t shale_interface_init_ctx(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, ...);
extern uint8_t shale_interface_init_ctx_va(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, const uint8_t *id_format, va_list vargs);
extern struct device_interface *shale_interface_new(driver_t *driver, const uint8_t *id_format, ...);
extern struct device_interface *shale_interface_new_va(driver_t *driver, const uint8_t *id_format, va_list vargs);
extern struct device_interface *shale_interface_new_ctx(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, ...);
extern struct device_interface *shale_interface_new_ctx_va(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, va_list vargs);
extern uint8_t shale_interface_init_ref(struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...);
extern uint8_t shale_interface_init_ref_va(struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs);
extern uint8_t shale_interface_init_ref_ctx(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...);
extern uint8_t shale_interface_init_ref_ctx_va(device_manager_t *ctx, struct device_interface *ifx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs);
extern struct device_interface *shale_interface_new_ref(driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...);
extern struct device_interface *shale_interface_new_ref_va(driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs);
extern struct device_interface *shale_interface_new_ref_ctx(device_manager_t *ctx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, ...);
extern struct device_interface *shale_interface_new_ref_ctx_va(device_manager_t *ctx, driver_t *driver, uint8_t ref_count, struct interface_ref *refs[], const uint8_t *id_format, va_list vargs);

static inline struct device_interface *shale_interface_get(struct device_interface *iface)
{
        return to_device_interface(light_object_get(&iface->header));
}
static inline void shale_interface_put(struct device_interface *iface)
{
        light_object_put(&iface->header);
}
static inline const uint8_t *shale_interface_name(struct device_interface *iface)
{
        return light_object_get_name(&iface->header);
}
#endif
