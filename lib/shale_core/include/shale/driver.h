#ifndef _SHALE_DRIVER_H
#define _SHALE_DRIVER_H

#include <light_object.h>

typedef uint8_t (*message_handler_t)(device_t *, message_handle_t *);

typedef struct handler_block {
    message_handler_t message;
} handler_block_t;

typedef struct device_class {
    struct light_object header;
    handler_block_t events;
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;
typedef struct device_driver {
    struct light_object header;
    class_t *driver_class;
    handler_block_t events;
} driver_t;

extern struct lobj_type ltype_device_class;
extern struct lobj_type ltype_device_driver;
#define LTYPE_DEVICE_CLASS_NAME "device_class"
#define LTYPE_DEVICE_DRIVER_NAME "device_driver"

#define to_device_class(object) container_of(object, class_t, header)
#define to_device_driver(object) container_of(object, driver_t, header)

uint8_t shale_class_init(class_t *_class, const uint8_t *id, message_handler_t handler);
uint8_t shale_driver_init(driver_t *driver, class_t *drv_class, const uint8_t *id, message_handler_t handler);

static inline class_t *shale_class_get(class_t *_class)
{
    return to_device_class(light_object_get(&_class->header));
}
static inline void shale_class_put(class_t *_class)
{
    light_object_put(&_class->header);
}
static inline driver_t *shale_driver_get(driver_t *driver)
{
    return to_device_driver(light_object_get(&driver->header));
}
static inline void shale_driver_put(driver_t *driver)
{
    light_object_put(&driver->header);
}

void shale_class_deliver_message(class_t *target, device_t *device, message_handle_t *msg);
void shale_driver_deliver_message(driver_t *target, device_t *device, message_handle_t *msg);
#endif
