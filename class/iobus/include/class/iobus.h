#ifndef _CLASS_IOBUS_H
#define _CLASS_IOBUS_H

#include "shale.h"

#define CLASS_ID_IOBUS      "shale.class.iobus"

#define IOBUS_MSG_TX        0
#define IOBUS_MSG_RX        1
#define IOBUS_MSG_TXRX      2

#define IOBUS_PROP_FRAME_SIZE       0
#define IOBUS_PROP_BUS_PROTOCOL     1

Shale_Static_Class(class_iobus);

struct iobus_device {
    struct device header;
    const uint8_t frame_size;
    const uint8_t bus_protocol;
};

struct iobus_consumer {
    struct iobus *target;
};

#define to_iobus_device(ptr) container_of(ptr, struct iobus_device, header)

class_t *shale_class_iobus();

#endif