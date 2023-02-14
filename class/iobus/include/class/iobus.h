#ifndef _CLASS_IOBUS_H
#define _CLASS_IOBUS_H

#include "shale.h"

#define CLASS_ID_IOBUS      "shale.class.iobus"

enum iobus_msg {
        IOBUS_MSG_ATTACH = 0,
        IOBUS_MSG_DETACH,
        IOBUS_MSG_TX,
        IOBUS_MSG_RX,
        IOBUS_MSG_TXRX
};
#define IOBUS_PROP_FRAME_SIZE       0
#define IOBUS_PROP_BUS_PROTOCOL     1

#define IOBUS_PROTOCOL_I2C          0
#define IOBUS_PROTOCOL_SPI          1

Shale_Static_Class(iobus);

struct iobus_device {
        struct device header;
        uint8_t frame_size;
        uint8_t bus_protocol;
        struct iobus_consumer *consumer;
};

struct iobus_consumer {
        struct iobus_device *target;
        void (*consume)(struct event *);
};

#define to_iobus_device(ptr) container_of(ptr, struct iobus_device, header)

uint8_t shale_class_iobus_device_init(struct iobus_device *device, driver_t *driver, struct lobj_type *type, const uint8_t *id);
class_t *shale_class_iobus();

#endif