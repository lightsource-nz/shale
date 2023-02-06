#define __SHALE_SOURCE__

#include "class/iobus.h"
#include "class/iobus_internal.h"

//static class_t *class_display;

static Message_Handler(shale_class_iobus_handle_message);

Shale_Static_Class_Define(iobus, CLASS_ID_IOBUS, shale_class_iobus_handle_message);

uint8_t shale_class_iobus_device_init(struct iobus_device *device, driver_t *driver, struct lobj_type *type, const uint8_t *id)
{
    // TODO verify validity of display driver
    return shale_device_init(&device->header, driver, type, id);
}
// TODO add return codes to allow message handlers to either consume messages,
//  or delegate handling to the driver level
static Message_Handler(shale_class_iobus_handle_message)
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
