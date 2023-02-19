#define __SHALE_SOURCE__

#include "class/iobus.h"
#include "class/iobus_internal.h"

static uint8_t _iobus_init(struct device *device);
static uint8_t _iobus_add(struct device *device);
static uint8_t _iobus_message(struct device *device, struct message_handle *handle);
static const struct device_event _iobus_event = {
        .init = _iobus_init,
        .add = _iobus_add,
        .message = _iobus_message
};

Shale_Static_Class_Define(iobus, CLASS_ID_IOBUS, _iobus_event);

class_t *shale_class_iobus()
{
    return &_class_iobus;
}

static uint8_t _iobus_init(struct device *device_header)
{
        struct iobus_device *device = to_iobus_device(device_header);

        return LIGHT_OK;
}
static uint8_t _iobus_add(struct device *device_header)
{
        struct iobus_device *device = to_iobus_device(device_header);

        return LIGHT_OK;
}
static uint8_t _iobus_message(struct device *device_header, struct message_handle *handle)
{
        struct iobus_device *device = to_iobus_device(device_header);
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
