#define __SHALE_SOURCE__

#include "class/iobus.h"
#include "class/display_internal.h"

//static class_t *class_display;

Shale_Static_Class_Define(class_display, CLASS_ID_DISPLAY, shale_class_display_handle_message);
/*
uint8_t shale_class_display_init()
{
    uint8_t retval;
    if(retval = shale_class_init(class_display, CLASS_ID_DISPLAY,
                                 &shale_class_display_handle_message)) {
        // log error
        return retval;
    }
    return LIGHT_OK;
}
*/
uint8_t shale_class_display_device_init(display_device_t *device, driver_t *driver, const uint8_t *id)
{
    // TODO verify validity of display driver
    return shale_device_init(&device->header, driver, id);
}
// TODO add return codes to allow message handlers to either consume messages,
//  or delegate handling to the driver level
Message_Handler(shale_class_display_handle_message)
{
    assert_device_class(device, CLASS_ID_DISPLAY);

    display_device_t *disp = to_display_device(device);
    switch (handle->msg.msg_id)
    {
    case DISPLAY_GET_DIMENSION: ;
        dimension_t *reply = shale_malloc(sizeof(dimension_t));
        *reply = disp->dimension;
        handle->reply = reply;
        return MX_DONE;
    
    case DISPLAY_SET_DIMENSION: ;
        disp->dimension = *(dimension_t *)handle->msg.param[0];
        handle->dest = device;
        return MX_FORWARD;
    default:
        break;
    }
}
class_t *shale_class_display()
{
    return &_class_display;
}

void shale_class_display_dispatch(device_t *device)
{

}
