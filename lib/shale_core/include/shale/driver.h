#ifndef _SHALE_DRIVER_H
#define _SHALE_DRIVER_H

typedef void (*device_init_t)(device_t *);
typedef uint8_t (*msg_handler_t)(device_t *, msg_handle_t *);

typedef struct handler_block {
    device_init_t init;
    msg_handler_t message;
} handler_block_t;

typedef struct device_class {
    uint8_t id[NAME_LENGTH];
    handler_block_t events;
    size_t data_length;
    uint8_t driver_count;
    driver_t *drivers[SHALE_CLASS_MAX_DRIVERS];
} class_t;

typedef struct device_driver {
    uint8_t id[NAME_LENGTH];
//    const uint8_t class_id[NAME_LENGTH];
    class_t *driver_class;
    handler_block_t events;
    size_t data_length;
} driver_t;

class_t *shale_class_new(uint8_t *id, size_t data_length,
    device_init_t init, msg_handler_t handler);
driver_t *shale_driver_new(uint8_t *id, class_t *drv_class, size_t data_length,
    device_init_t init, msg_handler_t handler);

void shale_class_deliver_message(class_t *target, device_t *device, msg_handle_t *msg);
void shale_driver_deliver_message(driver_t *target, device_t *device, msg_handle_t *msg);
#endif
