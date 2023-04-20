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

struct iobus_interface {
        struct device_interface header;
        uint8_t frame_size;
        uint8_t bus_protocol;
        struct iobus_consumer *consumer;
};

struct iobus_consumer {
        struct iobus_interface *target;
        void (*consume)(struct device_interface, struct shale_message *);
};

#define to_iobus_interface(ptr) container_of(ptr, struct iobus_interface, header)
#define lobject_to_iobus_interface(ptr) to_iobus_interface(to_device_interface(ptr))

uint8_t shale_class_iobus_interface_init(struct iobus_interface *interface, struct device_driver *driver, struct lobj_type *type, const uint8_t *id);
class_t *shale_class_iobus();

#endif