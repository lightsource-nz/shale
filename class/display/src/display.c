#define __SHALE_SOURCE__

#include "class/display.h"
#include "class/display_internal.h"
#include "class/iobus.h"

static uint8_t _display_init(struct device_interface *device);
static uint8_t _display_add(struct device_interface *device);
static uint8_t _display_message(struct device_interface *device, struct message_handle *handle);
static const struct interface_event _display_event = {
        .init = _display_init,
        .add = _display_add,
        .message = _display_message
};

Shale_Static_Class_Define_Ref(display, CLASS_ID_DISPLAY, _display_event, 1, Static_Class_Ref(ref_bus, iobus));

class_t *shale_class_display()
{
        return &_class_display;
}


static uint8_t _display_init(struct device_interface *device)
{
        return LIGHT_OK;
}
static uint8_t _display_add(struct device_interface *device)
{
        return LIGHT_OK;
}
static uint8_t _display_message(struct device_interface *device, struct message_handle *handle)
{
        return MX_DELEGATE;
}
