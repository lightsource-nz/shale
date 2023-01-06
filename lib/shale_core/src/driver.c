#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

struct lobj_type ltype_device_class = {
        .release = &_device_class_release
};
struct lobj_type ltype_device_driver = {
        .release = &_device_driver_release
};

static uint8_t class_count, driver_count = 0;
static class_t *class_table[SHALE_MAX_CLASSES];
static driver_t *driver_table[SHALE_MAX_DRIVERS];

class_t *shale_class_new(uint8_t *id, size_t data_length, device_init_t init, message_handler_t message)
{
    class_t *class_obj = shale_malloc(sizeof (class_t));
    light_object_init(&class_obj->header, &ltype_device_class);
    light_object_set_name(&class_obj->header, "class:%s", id);
    class_obj->data_length = data_length;
    class_obj->events.init = init;
    class_obj->events.message = message;
    uint8_t status = _class_register(class_obj);
    if(status) {
        //TODO log error
        return NULL;
    }
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
    device_init_t init, message_handler_t message)
{
    assert_class(drv_class);
    driver_t *driver_obj = shale_malloc(sizeof(driver_t));
    strcpy(driver_obj->id, id);
    driver_obj->driver_class = drv_class;
    driver_obj->data_length = data_length;
    driver_obj->events.init = init;
    driver_obj->events.message = message;
    uint8_t status = _driver_register(driver_obj);
    if(status) {
        //TODO log error
        return NULL;
    }
    return driver_obj;
}
// register driver in global and class driver tables
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

void shale_class_deliver_message(class_t *target, device_t *device, message_handle_t *msg)
{
    target->events.message(device, msg);
}
void shale_driver_deliver_message(driver_t *target, device_t *device, message_handle_t *msg);

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