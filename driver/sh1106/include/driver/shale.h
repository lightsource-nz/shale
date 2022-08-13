#ifndef _SHALE_H
#define _SHALE_H

#include <stdint.h>
#include <stdbool.h>

#define NAME_LENGTH             16
#define CLASS_MAX_DRIVERS       8

#define SHALE_MAX_CLASSES       8
#define SHALE_MAX_DRIVERS       16
#define SHALE_MAX_DEVICES       24

typedef struct device_class class_t;
typedef struct device_driver driver_t;
typedef struct device device_t;

typedef struct device {
    const uint8_t id;
    const uint8_t name[NAME_LENGTH];
    const class_t *dev_class;
    const driver_t *driver;
    void *class_data;
    void *driver_data;
} device_t;

typedef struct device_class {
    const uint8_t id;
    const uint8_t name[NAME_LENGTH];
    const uint8_t data_length;
    uint8_t driver_count;
    driver_t *drivers[CLASS_MAX_DRIVERS];
} class_t;

typedef struct device_driver {
    const uint8_t id;
    const uint8_t name[NAME_LENGTH];
    const uint8_t data_length;
} driver_t;

void shale_init();

class_t *shale_device_class(uint8_t id);
driver_t *shale_device_driver(uint8_t id);
device_t *shale_device(uint8_t id);

uint8_t shale_device_class_create(class_t *dev_class);
uint8_t shale_device_driver_create(driver_t *driver);
uint8_t shale_device_create(device_t *device);

#endif