#ifndef _SHALE_DRIVER_H
#define _SHALE_DRIVER_H

struct device_event {
        uint8_t (*init)(struct device *);
        uint8_t (*add)(struct device *);
        uint8_t (*message)(struct device *, struct message_handle *);
};

typedef struct device_class {
    struct light_object header;
    struct device_event events;
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;
typedef struct device_driver {
    struct light_object header;
    class_t *driver_class;
    const struct lobj_type *device_type;
    struct device *(*device_alloc)();
    struct device_event events;
} driver_t;
typedef struct class_descriptor {
    class_t *object;
    const uint8_t *id;
    struct device_event events;
} class_descriptor_t;
typedef struct driver_descriptor {
    driver_t *object;
    const uint8_t *id;
    const class_descriptor_t *parent;
    const struct lobj_type *device_type;
    struct device *(*device_alloc)();
    const struct device_event events;
} driver_descriptor_t;

struct class_table {
        struct light_object header;
        uint8_t count;
        class_t *classes[SHALE_MAX_CLASSES];
};
#define to_class_table(ptr) container_of(ptr, struct class_table, header)

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
        extern uint8_t static_class_count; \
        extern const class_descriptor_t *static_classes[]; \
        void __attribute__((constructor)) _load_##name() { static_classes[static_class_count++] = class_##name##_desc; }
#define Light_Driver_Load(name) \
        extern uint8_t static_driver_count; \
        extern const driver_descriptor_t *static_drivers[]; \
        void __attribute__((constructor)) _load_##name() { static_drivers[static_driver_count++] = driver_##name##_desc; }
#endif

#define Shale_Static_Class(name) \
        extern const class_descriptor_t _class_##name##_desc
#define Shale_Static_Driver(name) \
        extern const driver_descriptor_t _driver_##name##_desc
//#define Shale_Static_Device(name) device_t* __section(".shaledata.devices") _##name = &name

#define Shale_Static_Class_Define(name, _id, _events) \
        static class_t _class_##name; \
        const class_descriptor_t __in_flash(".descriptors") _class_##name##_desc = { .object = &_class_##name, .id = _id, .events = _events }; \
        const class_descriptor_t* __static_class class_##name##_desc = &_class_##name##_desc; \
        Light_Class_Load(name)
#define Shale_Static_Driver_Define(name, _id, _class, _type, _alloc, _events) \
        static driver_t _driver_##name; \
        const driver_descriptor_t __in_flash(".descriptors") _driver_##name##_desc = { .object = &_driver_##name, .parent = &_##_class##_desc, .id = _id, .device_type = _type, .device_alloc = _alloc, .events = _events }; \
        const driver_descriptor_t* __static_driver driver_##name##_desc = &_driver_##name##_desc; \
        Light_Driver_Load(name)

extern void shale_class_setup();
extern struct class_table *shale_class_table_main_get();
extern uint8_t shale_class_static_add(const class_descriptor_t *desc);
extern uint8_t shale_driver_static_add(const driver_descriptor_t *desc);
extern uint8_t shale_class_init(class_t *_class, const uint8_t *id, struct device_event events);
extern uint8_t shale_driver_init(driver_t *driver, class_t *drv_class, const uint8_t *id,
                                    const struct lobj_type *dev_type,
                                    struct device *(*device_alloc)(), struct device_event events);
extern class_t *shale_class_find(uint8_t *id);
extern class_t *shale_class_find_ctx(struct class_table *ctx, uint8_t *id);
extern driver_t *shale_driver_find(class_t *_class, uint8_t *id);
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
static inline const uint8_t *shale_class_name(class_t *_class)
{
        return light_object_get_name(&_class->header);
}
static inline const uint8_t *shale_driver_name(driver_t *driver)
{
        return light_object_get_name(&driver->header);
}
static inline class_t *shale_driver_class(driver_t *driver)
{
        return driver->driver_class;
}
extern void shale_class_deliver_message(class_t *target, device_t *device, message_handle_t *msg);
extern void shale_driver_deliver_message(driver_t *target, device_t *device, message_handle_t *msg);
#endif
