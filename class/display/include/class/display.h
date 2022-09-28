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

#define DISPLAY_CLASS_DATA_SIZE         (sizeof(display_data_t))

// geometric primitives
typedef struct vector2b {
    uint8_t x;
    uint8_t y;
} vector2b_t;

// represents a point in 16-bit screen coordinate space
typedef vector2b_t point2_t;
// represents a screen resolution of up to 255x255 pixels
typedef vector2b_t dimension_t;

typedef struct display_api {
    dimension_t (*get_dimension)(device_t*);
    uint8_t (*get_channels)(device_t*);
    uint8_t (*get_encoding)(device_t*);
    uint8_t (*get_bpp)(device_t*);
    uint16_t (*get_px_addr)(device_t*, point2_t);
    void (*device_write)(device_t*, uint16_t, size_t, uint32_t);
} display_api_t;

typedef struct display_driver {
    const uint8_t id[NAME_LENGTH];
    const uint8_t class_id[NAME_LENGTH];
    const display_api_t *driver_api;
    const uint8_t data_length;
} display_driver_t;

typedef struct display_data {
    dimension_t dimension;
    uint8_t channels;
    uint8_t encoding;
    uint8_t bpp;
    uint8_t framerate_hz;
    uint8_t *buffer_start;
} display_data_t;

typedef struct display_device {
    uint8_t name[NAME_LENGTH];
    class_t *dev_class;             // should always be class_display
    display_driver_t *driver;
    display_data_t *class_data;
    void *driver_api;
    void *driver_data;
} display_device_t;

class_t *shale_class_display();
void shale_class_display_init();
void shale_class_display_init_device(device_t *device, driver_t *driver);

dimension_t shale_display_dimension_get(display_device_t *dev);
uint8_t shale_display_channels_get(display_device_t *dev);
uint8_t shale_display_encoding_get(display_device_t *dev);
uint8_t shale_display_bpp_get(display_device_t *dev);
uint8_t shale_display_framerate_get(display_device_t *dev);
void shale_display_framerate_set(display_device_t *dev, uint8_t rate_hz);

#endif