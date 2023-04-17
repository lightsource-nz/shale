#ifndef _SHALE_DRIVER_H
#define _SHALE_DRIVER_H
 
DeclareCommand(Interface_Query);
DeclareCommand(Interface_AttachConsumer);
DeclareCommand(Interface_SetConsumeTarget); 

DeclareEvent(Interface_Describe);
DeclareEvent(Interface_ConsumerAttached);
 
struct interface_event {
        struct device_interface *(*alloc)();
        void (*free)(struct device_interface *);
        uint8_t (*init)(struct device_interface *);
        uint8_t (*add)(struct device_interface *);
        uint8_t (*message)(struct device_interface *, struct message_handle *);
};

struct class_ref {
        const uint8_t *id;
        const struct device_class *ref;
};
typedef struct device_class {
    struct light_object header;
    struct interface_event events;
    uint8_t ref_count;
    const struct class_ref refs[SHALE_INTERFACE_MAX_REFS];
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;
typedef struct device_driver {
    struct light_object header;
    class_t *driver_class;
    const struct lobj_type *interface_ltype;
    struct interface_event events;
} driver_t;

struct class_ref_descriptor {
        const uint8_t *id;
        const struct class_descriptor *ref;
};
typedef struct class_descriptor {
    class_t *object;
    const uint8_t *id;
    const struct interface_event events;
    const uint8_t ref_count;
    const struct class_ref_descriptor refs[SHALE_INTERFACE_MAX_REFS];
} class_descriptor_t;
typedef struct driver_descriptor {
    driver_t *object;
    const uint8_t *id;
    const struct class_descriptor *parent;
    const struct lobj_type *device_type;
    const struct interface_event events;
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

#define Static_Class_Ref(refid, target) { .id = #refid, .ref = &_class_## target ##_desc }

#define Shale_Static_Class_Define(name, _id, _events) \
        Shale_Static_Class_Define_Ref(name, _id, _events, 0,)

#define Shale_Static_Class_Define_Ref(name, _id, _events, _ref_count, _refs) \
        static class_t _class_##name; \
        const class_descriptor_t __in_flash(".descriptors") _class_##name##_desc = { \
                .object = &_class_##name, \
                .id = _id, \
                .events = _events, \
                .ref_count = _ref_count, \
                .refs = { _refs } \
        }; \
        const class_descriptor_t* __static_class class_##name##_desc = &_class_##name##_desc; \
        Light_Class_Load(name)
#define Shale_Static_Driver_Define(name, _id, _class, _type, _events) \
        static driver_t _driver_##name; \
        const driver_descriptor_t __in_flash(".descriptors") _driver_##name##_desc = { .object = &_driver_##name, .parent = &_##_class##_desc, .id = _id, .device_type = _type, .events = _events }; \
        const driver_descriptor_t* __static_driver driver_##name##_desc = &_driver_##name##_desc; \
        Light_Driver_Load(name)

extern void shale_class_setup();
extern struct class_table *shale_class_table_main_get();
extern uint8_t shale_class_static_add(const class_descriptor_t *desc);
extern uint8_t shale_driver_static_add(const driver_descriptor_t *desc);
extern uint8_t shale_class_init(class_t *_class, const uint8_t *id, struct interface_event events, uint8_t ref_count, struct class_ref *refs[]);
extern uint8_t shale_driver_init(driver_t *driver, class_t *drv_class, const uint8_t *id,
                                    const struct lobj_type *ifx_type, struct interface_event events);
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
extern void shale_class_deliver_message(class_t *target, struct device_interface *iface, message_handle_t *msg);
extern void shale_driver_deliver_message(driver_t *target, struct device_interface *iface, message_handle_t *msg);
#endif
