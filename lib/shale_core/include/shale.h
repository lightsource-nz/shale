#ifndef _SHALE_H
#define _SHALE_H

// NOTE for simplicity, object referencing by unique string ID has
//      been replaced by direct pointers. references may be reinstated
//      later, or removed permanently.
//      ID string fields in framework objects are currently used only
//      as labels to aid in debugging.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define NAME_LENGTH                     32
#define SHALE_CLASS_MAX_DRIVERS         8

#define SHALE_MAX_CLASSES               8
#define SHALE_MAX_DRIVERS               16
#define SHALE_MAX_DEVICES               24

#define SHALE_SUCCESS                   0
#define ERROR_MAX_ENTITIES              UINT8_MAX
#define ERROR_ENTITY_INVALID            (UINT8_MAX-1)

#define SHALE_DEVICE_STATE_INIT         0
#define SHALE_DEVICE_STATE_ACTIVE       1
#define SHALE_DEVICE_STATE_SUSPEND      2
#define SHALE_DEVICE_STATE_ERROR        UINT8_MAX

typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;

typedef struct device_class {
    uint8_t id[NAME_LENGTH];
    size_t data_length;
    void (*init_device)(device_t *, driver_t *);
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;

typedef struct device_driver {
    uint8_t id[NAME_LENGTH];
//    const uint8_t class_id[NAME_LENGTH];
    class_t *driver_class;
    void (*init_device)(device_t *);
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

void shale_init();

class_t *shale_class_new(uint8_t *id, size_t data_length, void (*init_device)(device_t *, driver_t *));
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, void *drv_api, size_t data_length, void (*init_device)(device_t *));
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver);

#endif