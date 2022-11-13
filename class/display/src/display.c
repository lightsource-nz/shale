#define __SHALE_SOURCE__

#include "class/display.h"
#include "class/display_internal.h"

static class_t *class_display;

void shale_class_display_init()
{
    class_display = shale_class_new(CLASS_ID_DISPLAY, sizeof(display_data_t),
    &shale_class_display_init_device,
    &shale_class_display_handle_message);
}
void shale_class_display_init_device(device_t *device)
{

}
// TODO add return codes to allow message handlers to either consume messages,
//  or delegate handling to the driver level
Message_Handler(shale_class_display_handle_message)
{
    assert_device_class(message.target, CLASS_ID_DISPLAY);

    display_device_t *disp = (display_device_t *)device;
    switch (handle->msg.msg_id)
    {
    case DISPLAY_GET_DIMENSION:
        dimension_t *reply = shale_malloc(sizeof(dimension_t));
        *reply = disp->class_data->dimension;
        handle->reply = reply;
        return MX_DONE;
    
    case DISPLAY_SET_DIMENSION:
        display_data_t *data = (display_data_t *)device->class_data;
        data->dimension = *(dimension_t *)handle->msg.param[0];
        handle->dest = device;
        return MX_FORWARD;
    default:
        break;
    }
}
class_t *shale_class_display()
{
    return class_display;
}

void shale_class_display_dispatch(device_t *device)
{

}
