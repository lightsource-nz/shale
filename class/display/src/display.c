#include "class/display.h"

static class_t class_display = {
    .id = CLASS_ID_DISPLAY
};

void shale_class_display_init()
{
    shale_class_create(&class_display);
}

void shale_class_display_dispatch()
