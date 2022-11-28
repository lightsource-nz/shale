#ifndef _SHALE_H
#define _SHALE_H

// NOTE for simplicity, object referencing by unique string ID has
//      been replaced by direct pointers. references may be reinstated
//      later, or removed permanently.
//      ID string fields in framework objects are currently used only
//      as labels to aid in debugging.
#include <pico/platform.h>
#include <pico/util/queue.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

#define NAME_LENGTH                     32
#define SHALE_CLASS_MAX_DRIVERS         8

#define SHALE_MAX_CLASSES               8
#define SHALE_MAX_DRIVERS               16
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

#define Shale_Static_Class(name) class_t* __section(".shaledata.classes") _##name = &name
#define Shale_Static_Driver(name) driver_t* __section(".shaledata.drivers") _##name = &name
#define Shale_Static_Device(name) device_t* __section(".shaledata.devices") _##name = &name

typedef struct device_manager device_manager_t;
typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;
typedef struct message message_t;

#include "shale/message.h"
#include "shale/thread.h"
#include "shale/driver.h"

typedef struct device {
    device_manager_t *context;
    uint8_t id[NAME_LENGTH];
    driver_t *driver;
    queue_t *queue;
    uint8_t state;
    uint8_t *class_data;
    uint8_t *driver_data;
} device_t;

typedef struct device_manager {
    uint8_t id[NAME_LENGTH];
    uint8_t scheduler_strategy;
    int mq_lock;
    uint8_t device_count;
    device_t *device_table[SHALE_MANAGER_MAX_DEVICES];
} device_manager_t;

#define Message_Handler(name) uint8_t name(device_t *device, message_handle_t *handle)

#define LOG_TRACE           "TRACE"
#define LOG_DEBUG           "DEBUG"
#define LOG_INFO            "INFO"
#define LOG_WARN            "WARN"
#define LOG_ERROR           "ERROR"

#define log_trace(format, ...) shale_logv(LOG_TRACE, (format), __VA_ARGS__)
#define shale_log(level, format, ...) shale_logv(level, (format), __VA_ARGS__)
#define shale_logv(level, format, args) vprintf("[##level##]: " format, args)  

void shale_init();

device_manager_t *shale_device_manager_new(uint8_t *id);
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver);
device_t *shale_device_manager_device_new(device_manager_t *context, uint8_t *id, driver_t *dev_driver);
bool shale_device_message_pending(device_t *device);
message_handle_t *shale_device_message_get_next(device_t *device);

void shale_service_message_queues();
void shale_service_message_queues_n(uint16_t count);
void shale_device_manager_service_message_queues(device_manager_t *context);
void shale_device_manager_service_message_queues_n(device_manager_t *context, uint16_t count);

#endif
