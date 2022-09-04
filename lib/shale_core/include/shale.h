#ifndef _SHALE_H
#define _SHALE_H

// NOTE for simplicity, object referencing by unique string ID has
//      been replaced by direct pointers. references may be reinstated
//      later, or removed permanently.
//      ID string fields in framework objects are currently used only
//      as labels to aid in debugging.

#include <stdint.h>
#include <stdbool.h>

//#define NAME_LENGTH             32
#define CLASS_MAX_DRIVERS       8

#define SHALE_MAX_CLASSES       8
#define SHALE_MAX_DRIVERS       16
#define SHALE_MAX_DEVICES       24

typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;

typedef struct device {
    uint8_t name[NAME_LENGTH];
    class_t *dev_class;
    driver_t *driver;
    void *class_data;
    void *driver_data;
} device_t;

typedef struct device_class {
    const uint8_t id[NAME_LENGTH];
    const uint8_t data_length;
    uint8_t driver_count;
    driver_t *drivers[CLASS_MAX_DRIVERS];
} class_t;

typedef struct device_driver {
    const uint8_t id[NAME_LENGTH];
//    const uint8_t class_id[NAME_LENGTH];
    class_t *driver_class;
    const void *driver_api;
    const uint8_t data_length;
} driver_t;

void shale_init();

//class_t *shale_class_get(uint8_t id);
//driver_t *shale_driver_get(uint8_t id);
//device_t *shale_device_get(uint8_t id);

//class_t *shale_class_find(uint8_t *name);
//driver_t *shale_driver_find(uint8_t *name);
//device_t *shale_device_find(uint8_t *name);

void shale_class_register(class_t *dev_class);
void shale_driver_register(driver_t *driver);
void shale_device_register(device_t *device);

#endif