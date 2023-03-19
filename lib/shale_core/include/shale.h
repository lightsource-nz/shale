#ifndef _SHALE_H
#define _SHALE_H

// proposed name:
// MOOSE: Message Oriented Operating System for Embedded platforms

// NOTE for simplicity, object referencing by unique string ID has
//      been replaced by direct pointers. references may be reinstated
//      later, or removed permanently.
//      ID string fields in framework objects are currently used only
//      as labels to aid in debugging.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <pico/platform.h>

#ifdef PICO_RP2040
#include <pico/util/queue.h>
#endif

#include <light_message.h>
#include <light_object.h>

#define shale_malloc malloc
#define shale_free free

#ifdef SHALE_DEBUG
#   define assert_class(pclass) \
        do { \
            if((pclass) == NULL) \
                panic("Assertion failed: null pointer at address 0x%8x", (pclass)); \
            if(!shale_class_registered((pclass))) \
                panic("Assertion failed: class object at 0x%8x (id=%s) not registered", (pclass), (pclass)->id); \
        } while (0)
#   define assert_driver(pdriver) \
        do { \
            if((pdriver) == NULL) \
                panic("Assertion failed: null pointer at address 0x%8x", (pdriver)); \
            if(!shale_driver_registered((pdriver))) \
                panic("Assertion failed: driver object at 0x%8x (id=%s) not registered", (pdriver), (pdriver)->id); \
        } while (0)
#   define assert_device(pdevice) \
        do { \
            if((pdevice) == NULL) \
                panic("Assertion failed: null pointer at address 0x%8x", (pdevice)); \
            if(!shale_device_registered((pdevice))) \
                panic("Assertion failed: device object at 0x%8x (id=%s) not registered", (pdevice), (pdevice)->id); \
        } while (0)
#   define assert_class_id(pclass, id) \
        do { \
            assert_class((pclass)); \
            if(!strcmp((pclass)->id, id)) \
                panic("Assertion failed: expected class [%s] but found [%s]", id, (pclass)->id);
        } while (0)
#   define assert_driver_id(pdriver, id) \
        do { \
            assert_driver((pdriver)); \
            if(!strcmp((pdriver)->id, id)) \
                panic("Assertion failed: expected driver [%s] but found [%s]", id, (pdriver)->id);
        } while (0)
#   define assert_device_class(pdevice, class_id) \
        do { \
            assert_device((pdevice)); \
            assert_class((pdevice)->driver->class, (class_id)); \
        } while (0)
#   define assert_device_driver(pdevice, driver_id) \\
        do { \
            assert_device((pdevice)); \
            assert_driver((pdevice)->driver, (driver_id)); \
        } while (0)
#else
#   define assert_class(pclass)
#   define assert_driver(pdriver)
#   define assert_device(pdevice)
#   define assert_class_id(pclass, id)
#   define assert_driver_id(pdriver, id)
#   define assert_device_class(pdevice, class_id)
#   define assert_device_driver(pdevice, driver_id)
#endif

// rp2040 can run 2 threads, one per core
// TODO move this to platform-specific config files
#define SHALE_CPU_HARD_THREAD_COUNT     2

// TODO implement version fields properly
#define SHALE_VERSION_STR               "0.1.0"

#define SHALE_MAX_STATIC_CLASSES        8
#define SHALE_MAX_STATIC_DRIVERS        8
#define SHALE_MAX_STATIC_DEVICES        8

#define SHALE_MAX_CLASSES               8
#define SHALE_CLASS_MAX_DRIVERS         8
#define SHALE_MAX_DEVICES               24
#define SHALE_MAX_THREADS               2
#define SHALE_QUEUE_DEPTH               8

#define SHALE_MANAGER_MAX_DEVICES       16
#define SHALE_MANAGER_DEFAULT_NAME      "devmgr:default"

// function return codes
#define SHALE_SUCCESS                   0
#define ERROR_MAX_ENTITIES              UINT8_MAX
#define ERROR_ENTITY_INVALID            (UINT8_MAX-1)

#define SHALE_MANAGER_STRATEGY_RR       0

#define SHALE_DEVICE_STATE_INIT         0
#define SHALE_DEVICE_STATE_ACTIVE       1
#define SHALE_DEVICE_STATE_SUSPEND      2
#define SHALE_DEVICE_STATE_ERROR        UINT8_MAX

typedef struct device_manager device_manager_t;
typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;
typedef struct message message_t;

#include "shale/message.h"
#include "shale/thread.h"
#include "shale/driver.h"

#define to_device_instance(object) container_of(object, device_t, header)

// TODO provide a platform agnostic message queueing interface which also
// works in host mode
typedef struct device {
#ifdef PICO_RP2040
    queue_t queue;
#endif
    struct light_object header;
    driver_t *driver;
    uint8_t state;
} device_t;
typedef struct device_descriptor {
    device_t *object;
    const uint8_t *id;
    const driver_descriptor_t *driver;
} device_descriptor_t;

struct event {
    uint16_t id;
    struct device *source;
};

#ifdef PICO_RP2040
#define __static_device __section(".shaledata.devices")
#define Light_Device_Load(name)
#else
#define __static_device
#define Light_Device_Load(name) \
        extern uint8_t static_device_count; \
        extern const device_descriptor_t *static_devices[]; \
        void __attribute__((constructor)) _load_##name() { static_devices[static_device_count++] = name##_desc; }
#endif

#define Shale_Static_Device(name) \
        extern const device_descriptor_t _##name##_desc

#define Shale_Static_Device_Define(name, _id, _driver) \
        static struct _driver##_device _##name; \
        const device_descriptor_t __in_flash(".descriptors") _##name##_desc = { \
                .object = &_##name.header.header, \
                .id = _id, \
                .driver = &_driver_##_driver##_desc \
        }; \
        const device_descriptor_t* __static_device name##_desc = &_##name##_desc; \
        Light_Device_Load(name)

extern struct lobj_type ltype_device_manager;

#define LTYPE_DEVICE_MANAGER_NAME "device_manager"
#define to_device_manager(object) container_of(object, device_manager_t, header)

typedef struct device_manager {
    struct light_object header;
    uint8_t scheduler_strategy;
#ifdef PICO_RP2040
    int mq_lock;
#endif
    uint8_t device_count;
    device_t *device_table[SHALE_MANAGER_MAX_DEVICES];
} device_manager_t;

#define Message_Handler(name) uint8_t name(device_t *device, message_handle_t *handle)

uint8_t shale_init();

extern uint8_t *shale_class_describe(struct device_class *_class);
extern uint8_t *shale_driver_describe(struct device_driver *driver);
extern uint8_t *shale_device_describe(struct device *device);

extern uint8_t shale_device_static_add(const device_descriptor_t *desc);
uint8_t shale_device_manager_init(device_manager_t *devmgr, const uint8_t *id);
device_t *shale_device_new(driver_t *driver, const uint8_t *id_format, ...);
device_t *shale_device_new_va(driver_t *driver, const uint8_t *id_format, va_list vargs);
device_t *shale_device_new_ctx(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, ...);
device_t *shale_device_new_ctx_va(device_manager_t *ctx, driver_t *driver, const uint8_t *id_format, va_list vargs);
device_t *shale_device_find(const uint8_t *id);
device_t *shale_device_find_ctx(device_manager_t *ctx, const uint8_t *id);
static inline device_t *shale_device_get(device_t *device)
{
        return to_device_instance(light_object_get(&device->header));
}
static inline void shale_device_put(device_t *device)
{
        light_object_put(&device->header);
}

static inline const uint8_t *shale_device_name(device_t *device)
{
        return light_object_get_name(&device->header);
}
bool shale_device_message_pending(device_t *device);
message_handle_t *shale_device_message_get_next(device_t *device);

struct device_manager *shale_device_manager_default();
void shale_service_message_queues();
void shale_service_message_queues_n(uint16_t count);
void shale_device_manager_service_message_queues(device_manager_t *context);
void shale_device_manager_service_message_queues_n(device_manager_t *context, uint16_t count);

#endif
