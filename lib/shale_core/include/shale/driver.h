#ifndef _SHALE_DRIVER_H
#define _SHALE_DRIVER_H

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
typedef struct class_descriptor {
    class_t *object;
    const uint8_t *id;
    message_handler_t handler;
} class_descriptor_t;
typedef struct driver_descriptor {
    driver_t *object;
    const uint8_t *id;
    const class_descriptor_t *parent;
    message_handler_t handler;
} driver_descriptor_t;

extern struct lobj_type ltype_device_class;
extern struct lobj_type ltype_device_driver;
#define LTYPE_DEVICE_CLASS_NAME "device_class"
#define LTYPE_DEVICE_DRIVER_NAME "device_driver"

#define to_device_class(object) container_of(object, class_t, header)
#define to_device_driver(object) container_of(object, driver_t, header)

#ifdef PICO_RP2040
#define __static_class __section(".shaledata.classes")
#define __static_driver __section(".shaledata.drivers")
#define Light_Class_Load(name)
#define Light_Driver_Load(name)
#else
#define __static_class
#define __static_driver
#define Light_Class_Load(name) \
        void __attribute__((constructor)) _load_##name() { shale_class_static_add(name##_desc); }
#define Light_Driver_Load(name) \
        void __attribute__((constructor)) _load_##name() { shale_driver_static_add(name##_desc); }
#endif

#define Shale_Static_Class(name) \
        extern const class_descriptor_t _##name##_desc; \
        const class_descriptor_t* __static_class name##_desc = &_##name##_desc; \
        Light_Class_Load(name)
#define Shale_Static_Driver(name) \
        extern const driver_descriptor_t _##name##_desc; \
        const driver_descriptor_t* __static_driver name##_desc = &_##name##_desc; \
        Light_Driver_Load(name)
//#define Shale_Static_Device(name) device_t* __section(".shaledata.devices") _##name = &name

#define Shale_Static_Class_Define(name, _id, _handler) \
        static class_t _##name; \
        const class_descriptor_t __in_flash(".descriptors") _##name##_desc = { .object = &_##name, .id = _id, .handler = _handler }
#define Shale_Static_Driver_Define(name, _class, _id, _handler) \
        static driver_t _##name; \
        const driver_descriptor_t __in_flash(".descriptors") _##name##_desc = { .object = &_##name, .parent = _class, .id = _id, .handler = _handler }

uint8_t shale_class_static_add(const class_descriptor_t *desc);
uint8_t shale_driver_static_add(const driver_descriptor_t *desc);
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
