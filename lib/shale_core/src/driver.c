#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

static void _device_class_release(struct light_object *obj);
static void _device_driver_release(struct light_object *obj);

struct lobj_type ltype_device_class = {
        .release = &_device_class_release
};
struct lobj_type ltype_device_driver = {
        .release = &_device_driver_release
};

static struct class_table {
        struct light_object header;
        uint8_t count;
        class_t *classes[SHALE_MAX_CLASSES];
} class_table_global;

static void _device_class_release(struct light_object *obj)
{
    free(to_device_class(obj));
}
static void _device_driver_release(struct light_object *obj)
{
    free(to_device_driver(obj));
}

class_t *shale_class_new(uint8_t *id, size_t data_length, device_init_t init, message_handler_t message)
{
    class_t *class_obj = shale_malloc(sizeof (class_t));
    light_object_init(&class_obj->header, &ltype_device_class);
    //light_object_set_name(&class_obj->header, "class:%s", id);
    class_obj->data_length = data_length;
    class_obj->events.init = init;
    class_obj->events.message = message;
    uint8_t status = _class_register(class_obj, id);
    if(status) {
        //TODO log error
        return NULL;
    }
    return class_obj;
}
uint8_t _class_register(class_t *_class, const uint8_t *id)
{
    if(class_table_global.count >= SHALE_MAX_CLASSES)
        return ERROR_MAX_ENTITIES;
    light_object_add(&_class->header, &class_table_global.header, "%s", id);
    class_table_global.classes[class_table_global.count++] = _class;
    return SHALE_SUCCESS;
}
uint8_t _class_add_driver(class_t *_class, driver_t *driver, const uint8_t *id)
{
    if(_class->driver_count >= SHALE_CLASS_MAX_DRIVERS)
        return ERROR_MAX_ENTITIES;
    uint8_t status;
    if(status = light_object_add(&_class->header, &driver->header, "%s", id)) {
        // TODO log object system error
        return status;
    }
    _class->drivers[_class->driver_count++] = driver;
    return SHALE_SUCCESS;
}
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, size_t data_length,
    device_init_t init, message_handler_t message)
{
    assert_class(drv_class);
    driver_t *driver_obj = shale_malloc(sizeof(driver_t));
    driver_obj->driver_class = drv_class;
    driver_obj->data_length = data_length;
    driver_obj->events.init = init;
    driver_obj->events.message = message;
    uint8_t status = _driver_register(driver_obj, id);
    if(status) {
        shale_free(driver_obj);
        //TODO log error
        return NULL;
    }
    return driver_obj;
}
// register driver in class driver table
uint8_t _driver_register(driver_t *driver, const uint8_t *id)
{
    return _class_add_driver(driver->driver_class, driver, id);
}

void shale_class_deliver_message(class_t *target, device_t *device, message_handle_t *msg)
{
    target->events.message(device, msg);
}
void shale_driver_deliver_message(driver_t *target, device_t *device, message_handle_t *msg);

class_t *_class_lookup(uint8_t *id)
{
    for(int i = 0; i < class_table_global.count; i++) {
        class_t *class_i = class_table_global.classes[i];
        if(strcmp(light_object_get_name(&class_i->header), id))
            return class_i;
    }
    return NULL;
}
driver_t *_driver_lookup(uint8_t *id)
{
    for(int i = 0; i < class_table_global.count; i++) {
        class_t *class_i = class_table_global.classes[i];
        for(uint8_t j = 0; j < class_i->driver_count; j++){
            driver_t *driver_j = class_i->drivers[j];
            if(strcmp(light_object_get_name(&driver_j->header), id))
                return driver_j;
        }
    }
    return NULL;
}
