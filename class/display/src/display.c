#include "class/display.h"

static class_t *class_display;

void shale_class_display_init()
{
    class_display = shale_class_new(CLASS_ID_DISPLAY, sizeof(display_data_t), &shale_class_display_init_device);
}
void shale_class_display_init_device(device_t *device, driver_t *driver)
{

}
class_t *shale_class_display()
{
    return class_display;
}

void shale_class_display_dispatch(device_t *device)
{

}
