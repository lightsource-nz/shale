/*
 *      shale/static.h
 *      types, macros and functions to support global system objects which
 *      are defined in static code, and initialized at platform load-time
 *      using compiler and/or linker magic
 * 
 *      authored by Alex Fulton, 2023
 */
#ifndef _SHALE_STATIC_H
#define _SHALE_STATIC_H

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
        const struct lobj_type *ifx_ltype;
        struct device_interface *(*ifx_alloc)();
        void (*ifx_free)(struct device_interface *);
        const struct interface_event events;
} driver_descriptor_t;

struct interface_ref_descriptor {
        uint8_t *ref_id;
        const struct interface_descriptor *ref;
};

struct interface_descriptor {
    interface_t *object;
    const uint8_t *id;
    const struct driver_descriptor *if_driver;
    const uint8_t ref_count;
    const struct interface_ref_descriptor refs[SHALE_INTERFACE_MAX_REFS];
};

typedef struct device_descriptor {
        struct device *object;
        const uint8_t *id;
        const struct interface_descriptor *if_main;
        const uint8_t if_count;
        const struct interface_descriptor *interface[SHALE_DEVICE_MAX_INTERFACES];
} device_descriptor_t;

#ifdef PICO_RP2040
#define __static_class __section(".shaledata.classes")
#define __static_driver __section(".shaledata.drivers")
#define __static_interface __section(".shaledata.interfaces")
#define __static_device __section(".shaledata.devices")
#define Light_Class_Load(name)
#define Light_Driver_Load(name)
#define Light_Interface_Load(name)
#define Light_Device_Load(name)
#else
#define __static_class
#define __static_driver
#define __static_interface
#define __static_device
#define Light_Class_Load(name) \
        extern uint8_t static_class_count; \
        extern const class_descriptor_t *static_classes[]; \
        void __attribute__((constructor)) _load_##name() { static_classes[static_class_count++] = class_##name##_desc; }
#define Light_Driver_Load(name) \
        extern uint8_t static_driver_count; \
        extern const driver_descriptor_t *static_drivers[]; \
        void __attribute__((constructor)) _load_##name() { static_drivers[static_driver_count++] = driver_##name##_desc; }
#define Light_Interface_Load(name) \
        extern uint8_t static_interface_count; \
        extern const struct interface_descriptor *static_interfaces[]; \
        void __attribute__((constructor)) _load_##name() { static_interfaces[static_interface_count++] = if_## name ##_desc; }
#define Light_Device_Load(name) \
        extern uint8_t static_device_count; \
        extern const struct device_descriptor *static_devices[]; \
        void __attribute__((constructor)) _load_##name() { static_devices[static_device_count++] = name##_desc; }
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
#define Shale_Static_Driver_Define(name, _id, _class, _type, _alloc, _free, _events) \
        static driver_t _driver_##name; \
        const driver_descriptor_t __in_flash(".descriptors") _driver_##name##_desc = { \
                .object = &_driver_##name, \
                .parent = &_##_class##_desc, \
                .id = _id, \
                .ifx_ltype = _type, \
                .ifx_alloc = _alloc, \
                .ifx_free = _free, \
                .events = _events \
        }; \
        const driver_descriptor_t* __static_driver driver_##name##_desc = &_driver_##name##_desc; \
        Light_Driver_Load(name)

#define Shale_Static_Interface(name) \
        extern const struct interface_descriptor _if_##name##_desc; \
        static const struct interface_descriptor *if_##name##_desc = &_if_##name##_desc

#define Static_Interface_Ref(refid, target) { .ref_id = #refid, .ref = &_if_## target ##_desc }

#define Shale_Static_Interface_Define(name, _id, _driver) \
        Shale_Static_Interface_Define_Ref(name, _id, _driver, 0,)

#define Shale_Static_Interface_Define_Ref(name, _id, _driver, _ref_count, _refs) \
        struct _driver##_interface _##name; \
        const struct interface_descriptor __in_flash(".descriptors") _if_##name##_desc = { \
                .object  = &_##name.header.header, \
                .id = _id, \
                .if_driver = &_driver_##_driver##_desc, \
                .ref_count = _ref_count, \
                .refs = { _refs } \
        }; \
        const struct interface_descriptor* __static_interface if_##name##_desc = &_if_##name##_desc; \
        Light_Interface_Load(name)

#define Shale_Static_Device(name) \
        extern const struct device_descriptor _##name##_desc

#define Shale_Static_Device_Define(name, _id, _iface) \
        static struct device _##name; \
        static const struct interface_descriptor * _## name ##_iface_list[] = { &_if_##_iface##_desc }; \
        Shale_Static_Device_Define_Composite(name, _id, _iface, 1, ( &_if_##_iface##_desc ))

#define Shale_Static_Device_Define_Composite(name, _id, _iface, _if_count, _iface_list) \
        const struct device_descriptor __in_flash(".descriptors") _##name##_desc = { \
                .object = &_## name, \
                .id = _id, \
                .if_main = &_if_##_iface##_desc, \
                .if_count = _if_count, \
                .interface = { _iface_list } \
        }; \
        const device_descriptor_t* __static_device name##_desc = &_##name##_desc; \
        Light_Device_Load(name)

extern uint8_t shale_class_static_add(const class_descriptor_t *desc);
extern uint8_t shale_driver_static_add(const driver_descriptor_t *desc);
extern uint8_t shale_interface_static_add(const struct interface_descriptor *desc);
extern uint8_t shale_device_static_add(const device_descriptor_t *desc);

#endif