#ifndef _CLASS_IOBUS_H
#define _CLASS_IOBUS_H

#include "shale.h"

#define CLASS_ID_IOBUS      "shale.class.iobus"

#define IOBUS_MSG_TX        0
#define IOBUS_MSG_RX        1
#define IOBUS_MSG_TXRX      2

#define IOBUS_PROP_FRAME_SIZE       0
#define IOBUS_PROP_BUS_PROTOCOL     1

#define IOBUS_PROTOCOL_I2C          0
#define IOBUS_PROTOCOL_SPI          1

Shale_Static_Class(iobus);

struct iobus_device {
    struct device header;
    uint8_t frame_size;
    uint8_t bus_protocol;
};

struct iobus_consumer {
    struct iobus *target;
};

#define to_iobus_device(ptr) container_of(ptr, struct iobus_device, header)

uint8_t shale_class_iobus_device_init(struct iobus_device *device, driver_t *driver, struct lobj_type *type, const uint8_t *id);
class_t *shale_class_iobus();

#endif