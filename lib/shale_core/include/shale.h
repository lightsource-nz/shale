#ifndef _SHALE_H
#define _SHALE_H

// NOTE for simplicity, object referencing by unique string ID has
//      been replaced by direct pointers. references may be reinstated
//      later, or removed permanently.
//      ID string fields in framework objects are currently used only
//      as labels to aid in debugging.
#include <pico/platform.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define shale_malloc malloc

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

#define NAME_LENGTH                     32
#define SHALE_CLASS_MAX_DRIVERS         8

#define SHALE_MAX_CLASSES               8
#define SHALE_MAX_DRIVERS               16
#define SHALE_MAX_DEVICES               24
#define SHALE_QUEUE_DEPTH               8
#define SHALE_MQ_SPINLOCK               0

#define SHALE_SUCCESS                   0
#define ERROR_MAX_ENTITIES              UINT8_MAX
#define ERROR_ENTITY_INVALID            (UINT8_MAX-1)

#define SHALE_DEVICE_STATE_INIT         0
#define SHALE_DEVICE_STATE_ACTIVE       1
#define SHALE_DEVICE_STATE_SUSPEND      2
#define SHALE_DEVICE_STATE_ERROR        UINT8_MAX

#define Shale_Static_Class(name) class_t* __section(".shaledata.classes") _##name = &name
#define Shale_Static_Driver(name) driver_t* __section(".shaledata.drivers") _##name = &name
#define Shale_Static_Device(name) device_t* __section(".shaledata.devices") _##name = &name

typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;
typedef struct message message_t;
typedef struct msg_handle msg_handle_t;

typedef struct device_class {
    uint8_t id[NAME_LENGTH];
    size_t data_length;
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;

typedef struct device_driver {
    uint8_t id[NAME_LENGTH];
//    const uint8_t class_id[NAME_LENGTH];
    class_t *driver_class;
    void *driver_api;
    size_t data_length;
} driver_t;

typedef struct device {
    uint8_t name[NAME_LENGTH];
    driver_t *driver;
    uint8_t state;
    uint8_t *class_data;
    uint8_t *driver_data;
} device_t;

typedef struct message {
    device_t *target;
    uint8_t msg_id;
    void *param[];
} message_t;

typedef struct msg_handle {
    uint32_t handle_id;
    uint8_t status;
    message_t msg;
    void *reply;
} msg_handle_t;

typedef void (*device_init_t)(device_t *);
typedef uint8_t (*msg_handler_t)(device_t *, msg_handle_t *);

#define MX_DONE             0x00
#define MX_RESPOND          0x01
#define MX_DELEGATE         0x02
#define MX_ERROR            0xFF

#define MXS_QUEUED          0x00
#define MXS_SENT            0x01
#define MXS_RECEIVED        0x02
#define MXS_RESPONDED       0x03
#define MXS_ERROR           0xFF

#define mx(target, id) mx_p(target, id, NULL)
#define mx_p(target, id, param) \
    shale_message_send({target, id, param})

void shale_init();
msg_handle_t *shale_message_send(message_t message);
void shale_message_await(msg_handle_t *handle, uint8_t status);

class_t *shale_class_new(uint8_t *id, size_t data_length,
    device_init_t init, msg_handler_t handler);
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class,
    size_t data_length, device_init_t init, msg_handler_t handler);
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver);

#endif