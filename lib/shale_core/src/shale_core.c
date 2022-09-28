#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <string.h>

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
    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
}

uint8_t s_class_register(class_t *dev_class);
class_t *shale_class_new(uint8_t *id, size_t data_length, void (*init_device)(device_t *, driver_t *))
{
    class_t *class_obj = shale_malloc(sizeof (class_t));
    strcpy(class_obj->id, id);
    class_obj->data_length = data_length;
    class_obj->init_device = init_device;
    s_class_register(class_obj);
    return class_obj;
}
uint8_t s_class_register(class_t *dev_class)
{
    if(class_count >= SHALE_MAX_CLASSES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = class_count++;
    class_table[id] = dev_class;
    return SHALE_SUCCESS;
}
uint8_t s_driver_register(driver_t *driver);
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, void *drv_api, size_t data_length, void (*init_device)(device_t *))
{
    assert_class(drv_class);
    driver_t *driver_obj = shale_malloc(sizeof(driver_t));
    strcpy(driver_obj->id, id);
    driver_obj->driver_class = drv_class;
    driver_obj->driver_api = drv_api;
    driver_obj->data_length = data_length;
    driver_obj->init_device = init_device;
    s_driver_register(driver_obj);
    return driver_obj;
}
uint8_t s_driver_register(driver_t *driver)
{
    if(driver_count >= SHALE_MAX_DRIVERS)
        return ERROR_MAX_ENTITIES;
    uint8_t id = driver_count++;
    driver_table[id] = driver;
    if(driver->driver_class->driver_count >= SHALE_CLASS_MAX_DRIVERS)  {
        return ERROR_MAX_ENTITIES;
    }
    return SHALE_SUCCESS;
}
uint8_t s_device_register(device_t *device);
// allocate device memory, and call class/driver init handlers
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver)
{
    assert_class(dev_driver->driver_class);
    assert_driver(dev_driver);
    device_t *device_obj = shale_malloc(sizeof(device_t));
    device_obj->class_data = shale_malloc(dev_driver->driver_class->data_length);
    device_obj->driver_data = shale_malloc(dev_driver->data_length);
    strcpy(id, device_obj->name);
    device_obj->driver = dev_driver;
    s_device_register(device_obj);
    dev_driver->driver_class->init_device(device_obj, dev_driver);
    dev_driver->init_device(device_obj);
    return device_obj;
}
uint8_t s_device_register(device_t *device)
{
    if(device_count >= SHALE_MAX_DEVICES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = device_count++;
    device_table[id] = device;
    return SHALE_SUCCESS;
}
