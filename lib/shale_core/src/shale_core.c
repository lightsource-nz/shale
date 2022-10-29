#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

#include <pico/sync.h>
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
static int mq_lock;

class_t *_class_lookup(uint8_t *id)
{
    for(int i = 0; i < SHALE_MAX_CLASSES; i++) {
        if(strcmp(class_table[i]->id, id))
            return class_table[i];
    }
    return NULL;
}
driver_t *_driver_lookup(uint8_t *id)
{
    for(int i = 0; i < SHALE_MAX_DRIVERS; i++) {
        if(strcmp(driver_table[i]->id, id))
            return driver_table[i];
    }
    return NULL;
}
device_t *_device_lookup(uint8_t *id)
{
    for(int i = 0; i < SHALE_MAX_DEVICES; i++) {
        if(strcmp(device_table[i]->id, id))
            return device_table[i];
    }
    return NULL;
}

void shale_init()
{
    if(!mq_lock)
        mq_lock = spin_lock_claim_unused(true);
    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
}

void _dispatch_message_for_device(device_t *device)
{
    msg_handle_t *handle;
    while(queue_try_peek(device->queue, &handle)) {
        uint8_t status = device->driver->driver_class->events.message(handle->msg.target, handle);
        switch(status) {
            case MX_DONE:
            break;
            case MX_FORWARD:
            queue_t *next_q = handle->dest->queue;
            if(queue_try_add(next_q, handle)) {
                if(!queue_try_remove(device->queue, &handle)) {
                    // TODO log queueing error

                }
            }
            else
            {
                // TODO log overflow condition
            }
            break;
        }
    }

}
class_t *shale_class_new(uint8_t *id, size_t data_length, device_init_t init, msg_handler_t message)
{
    class_t *class_obj = shale_malloc(sizeof (class_t));
    strcpy(class_obj->id, id);
    class_obj->data_length = data_length;
    class_obj->events.init = init;
    class_obj->events.message = message;
    _class_register(class_obj);
    return class_obj;
}
uint8_t _class_register(class_t *_class)
{
    if(class_count >= SHALE_MAX_CLASSES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = class_count++;
    class_table[id] = _class;
    return SHALE_SUCCESS;
}
uint8_t _class_add_driver(class_t *_class, driver_t *driver)
{

}
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, size_t data_length,
    device_init_t init, msg_handler_t message)
{
    assert_class(drv_class);
    driver_t *driver_obj = shale_malloc(sizeof(driver_t));
    strcpy(driver_obj->id, id);
    driver_obj->driver_class = drv_class;
    driver_obj->data_length = data_length;
    driver_obj->events.init = init;
    driver_obj->events.message = message;
    _driver_register(driver_obj);
    return driver_obj;
}
// register driver in global and class-local driver tables
uint8_t _driver_register(driver_t *driver)
{
    if(driver_count >= SHALE_MAX_DRIVERS)
        return ERROR_MAX_ENTITIES;
    uint8_t status = _class_add_driver(driver->driver_class, driver);
    if(status != SHALE_SUCCESS) {
        return status;
    }
    driver_table[driver_count++] = driver;
    return SHALE_SUCCESS;
}
// allocate device memory, and call class/driver init handlers
device_t *shale_device_new(uint8_t *id, driver_t *dev_driver)
{
    assert_class(dev_driver->driver_class);
    assert_driver(dev_driver);
    device_t *device_obj = shale_malloc(sizeof(device_t));
    device_obj->queue = shale_malloc(sizeof(queue_t));
    queue_init_with_spinlock(device_obj->queue,sizeof(msg_handle_t), SHALE_QUEUE_DEPTH, mq_lock);
    device_obj->class_data = shale_malloc(dev_driver->driver_class->data_length);
    device_obj->driver_data = shale_malloc(dev_driver->data_length);
    strcpy(id, device_obj->id);
    device_obj->driver = dev_driver;
    _device_register(device_obj);
    dev_driver->driver_class->events.init(device_obj);
    dev_driver->events.init(device_obj);
    return device_obj;
}
uint8_t _device_register(device_t *device)
{
    if(device_count >= SHALE_MAX_DEVICES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = device_count++;
    device_table[id] = device;
    return SHALE_SUCCESS;
}
