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

static class_table_t *class_table[SHALE_MAX_CLASSES];
static driver_table_t *driver_table[SHALE_MAX_DRIVERS];
static device_t *device_table[SHALE_MAX_DEVICES];

static uint8_t thread_state = SHALE_THREAD_ACTIVE;
static thread_condition_t thread_cond = { NULL, MXS_QUEUED };

static uint8_t class_count, driver_count, device_count = 0;
static uint32_t next_msgid = 0;
static int mq_lock;

class_table_t *_class_table_lookup(class_t *p_class)
{
    for(int i = 0; i < SHALE_MAX_CLASSES; i++) {
        if(class_table[i]->p_class == p_class)
            return class_table[i];
    }
    return NULL;
}
driver_table_t *_driver_table_lookup(driver_t *driver)
{
    for(int i = 0; i < SHALE_MAX_DRIVERS; i++) {
        if(driver_table[i]->driver == driver)
            return driver_table[i];
    }
    return NULL;
}

// TODO this function will need to be thread-safe if we run on both cores
uint32_t _handle_id_new()
{
    return next_msgid++;
}

void shale_init()
{
    if(!mq_lock)
        mq_lock = spin_lock_claim_unused(true);
    // TODO implement initializer arrays in linker script
    // shale_process_static_classes();
    // shale_process_static_drivers();
}

msg_handle_t *shale_message_send(message_t message)
{
    msg_handle_t *handle = shale_malloc(sizeof(msg_handle_t));
    handle->handle_id = _handle_id_new();
    handle->msg = message;
    handle->status = MXS_QUEUED;
    handle->reply = NULL;
    class_table_t *target_class = _class_table_lookup(message.target->driver->driver_class);
    queue_add_blocking(target_class->queue, handle);
    return handle;
}
void shale_message_await(msg_handle_t *handle, uint8_t status)
{
    thread_cond = (thread_condition_t) { handle, status };
    thread_state = SHALE_THREAD_BLOCKED;
    // TODO implement yield to message scheduler
}

uint8_t s_class_register(class_table_t *dev_class);
class_t *shale_class_new(uint8_t *id, size_t data_length, device_init_t init, msg_handler_t handler)
{
    class_table_t *table_obj = shale_malloc(sizeof(class_table_t));
    table_obj->queue = shale_malloc(sizeof(queue_t));
    queue_init_with_spinlock(table_obj->queue, sizeof(msg_handle_t), SHALE_QUEUE_DEPTH, mq_lock);
    class_t *class_obj = shale_malloc(sizeof (class_t));
    strcpy(class_obj->id, id);
    class_obj->data_length = data_length;
    table_obj->p_class = class_obj;
    table_obj->init = init;
    table_obj->handler = handler;
    s_class_register(table_obj);
    return class_obj;
}
uint8_t s_class_register(class_table_t *table)
{
    if(class_count >= SHALE_MAX_CLASSES)
        return ERROR_MAX_ENTITIES;
    uint8_t id = class_count++;
    class_table[id] = table;
    return SHALE_SUCCESS;
}
uint8_t s_driver_register(driver_table_t *driver);
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, size_t data_length,
    device_init_t init, msg_handler_t handler)
{
    assert_class(drv_class);
    driver_table_t *table_obj = shale_malloc(sizeof(driver_table_t));
    table_obj->queue = shale_malloc(sizeof(queue_t));
    queue_init_with_spinlock(table_obj->queue,sizeof(message_t), SHALE_QUEUE_DEPTH, mq_lock);
    driver_t *driver_obj = shale_malloc(sizeof(driver_t));
    strcpy(driver_obj->id, id);
    driver_obj->driver_class = drv_class;
    driver_obj->data_length = data_length;
    table_obj->driver = driver_obj;
    table_obj->init = init;
    table_obj->handler = handler;
    s_driver_register(table_obj);
    return driver_obj;
}
uint8_t s_driver_register(driver_table_t *table)
{
    if(driver_count >= SHALE_MAX_DRIVERS)
        return ERROR_MAX_ENTITIES;
    uint8_t id = driver_count++;
    driver_table[id] = table;
    if(table->driver->driver_class->driver_count >= SHALE_CLASS_MAX_DRIVERS)  {
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
    _class_table_lookup(dev_driver->driver_class)->init(device_obj);
    _driver_table_lookup(dev_driver)->init(device_obj);
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
