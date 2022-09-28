#include "shale.h"

#include <stdlib.h>

#define ERROR_MAX_ENTITIES          UINT8_MAX;
#define ERROR_ENTITY_INVALID        UINT8_MAX-1;

extern void const *__shaledata_classes_start;
extern void const *__shaledata_classes_end;
extern void const *__shaledata_drivers_start;
extern void const *__shaledata_drivers_end;
extern void const *__shaledata_devices_start;
extern void const *__shaledata_devices_end;

static class_t *class_table[SHALE_MAX_CLASSES];
static driver_t *driver_table[SHALE_MAX_DRIVERS];
static device_t *device_table[SHALE_MAX_DEVICES];

static uint8_t class_count, driver_count, device_count = 0;

void shale_init()
{

}

class_t *shale_device_class(uint8_t id)
{
    if(id < class_count) {
        return class_table[id];
    }
    return NULL;
}
driver_t *shale_device_driver(uint8_t id)
{
    if(id < driver_count) {
        return driver_table[id];
    }
    return NULL;
}
device_t *shale_device(uint8_t id)
{
    if(id < device_count) {
        return device_table[id];
    }
    return NULL;
}

uint8_t shale_class_create(class_t *dev_class)
{
    if(class_count >= SHALE_MAX_CLASSES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = class_count++;
    class_table[id] = dev_class;
    return id;
}
uint8_t shale_driver_create(driver_t *driver)
{
    if(driver_count >= SHALE_MAX_DRIVERS)
        return ERROR_MAX_ENTITIES;
    uint8_t id = driver_count++;
    driver_table[id] = driver;
    return id;
}
uint8_t shale_device_create(device_t *device)
{
    if(device_count >= SHALE_MAX_DEVICES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = device_count++;
    device_table[id] = device;
    return id;
}
