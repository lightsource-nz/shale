#define __SHALE_SOURCE__

#include "class/iobus.h"
#include "class/iobus_internal.h"

//static class_t *class_display;

static uint8_t _iobus_init(struct device *device);
static uint8_t _iobus_add(struct device *device);
static uint8_t _iobus_message(struct device *device, struct message_handle *handle);
static struct device_event _iobus_event = {
        .init = _iobus_init,
        .add = _iobus_add,
        .message = _iobus_message
};

Shale_Static_Class_Define(iobus, CLASS_ID_IOBUS, &_iobus_event);

uint8_t shale_class_iobus_device_init(struct iobus_device *device, driver_t *driver, struct lobj_type *type, const uint8_t *id)
{
    // TODO verify validity of display driver
    return shale_device_init(&device->header, driver, type, id);
}
static uint8_t _iobus_init(struct device *device)
{
    return LIGHT_OK;
}
static uint8_t _iobus_message(struct device *device, struct message_handle *handle)
{
    assert_device_class(device, CLASS_ID_IOBUS);

    struct iobus_device *bus = to_iobus_device(device);
    switch (handle->msg.msg_id)
    {
    case IOBUS_MSG_RX: ;
        return MX_DELEGATE;
    
    case IOBUS_MSG_TX: ;
        return MX_DELEGATE;
    default:
        return MX_DELEGATE;
    }
}
class_t *shale_class_iobus()
{
    return &_class_iobus;
}

void shale_class_iobus_dispatch(device_t *device)
{

}
