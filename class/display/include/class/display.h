#ifndef _CLASS_DISPLAY_H
#define _CLASS_DISPLAY_H

#include "shale.h"

#define CLASS_ID_DISPLAY "shale.class.display"

#define DISPLAY_BUFFER_SINGLE           0
#define DISPLAY_BUFFER_DOUBLE           1
#define DISPLAY_BUFFER_TRIPLE           2

#define DISPLAY_CHANNELS_BW             0
#define DISPLAY_CHANNELS_RGB            1

#define DISPLAY_ENC_MONO                0
#define DISPLAY_ENC_GRAY_256            1
#define DISPLAY_ENC_RGB_646             2
#define DISPLAY_ENC_RGB_666             3
#define DISPLAY_ENC_RGB_888             4

#define DISPLAY_GET_DIMENSION           0
#define DISPLAY_GET_ENCODING            1
#define DISPLAY_GET_BPP                 2
#define DISPLAY_GET_FRAMERATE           3

#define DISPLAY_SET_DIMENSION           8
#define DISPLAY_SET_FRAMERATE           9

//#define DISPLAY_CLASS_DATA_SIZE         (sizeof(display_data_t))

// geometric primitives
typedef struct vector2b {
    uint8_t x;
    uint8_t y;
} vector2b_t;

// represents a point in 16-bit screen coordinate space
typedef vector2b_t point2_t;
// represents a screen resolution of up to 255x255 pixels
typedef vector2b_t dimension_t;

struct display_interface;

// these types are made redundant by message-based procedure calling
/*
typedef struct display_api {
    dimension_t (*get_dimension)(struct display_interface*);
    uint8_t (*get_channels)(struct display_interface*);
    uint8_t (*get_encoding)(struct display_interface*);
    uint8_t (*get_bpp)(struct display_interface*);
    uint16_t (*get_px_addr)(struct display_interface*, point2_t);
    void (*device_write)(struct display_interface*, uint16_t, size_t, uint32_t);
} display_api_t;

typedef struct display_driver {
    driver_t header;
    display_api_t *api;
} display_driver_t;
#define to_display_driver(ptr) container_of(ptr, display_driver_t, header)
*/

Shale_Static_Class(display);

struct display_interface {
    struct device_interface header;
    dimension_t dimension;
    uint8_t channels;
    uint8_t encoding;
    uint8_t bpp;
    uint8_t framerate_hz;
    uint8_t *buffer_start;
};
#define to_display_interface(ptr) container_of(ptr, struct display_interface, header)

class_t *shale_class_display();
uint8_t shale_class_display_init();
uint8_t shale_class_display_interface_init(struct display_interface *device, driver_t *driver, struct lobj_type *type, const uint8_t *id);

dimension_t shale_display_dimension_get(struct display_interface *dev);
uint8_t shale_display_channels_get(struct display_interface *dev);
uint8_t shale_display_encoding_get(struct display_interface *dev);
uint8_t shale_display_bpp_get(struct display_interface *dev);
uint8_t shale_display_framerate_get(struct display_interface *dev);
void shale_display_framerate_set(struct display_interface *dev, uint8_t rate_hz);

#endif