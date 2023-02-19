#ifndef _DRIVER_BUS_SPI_32_H
#define _DRIVER_BUS_SPI_32_H

// driver bus_spi_32
// SPI interface driver with 32-bit transfer frame size

#include "shale.h"
#include "class/iobus.h"

#define DRIVER_ID_BUS_SPI_32        "shale:driver:bus_spi_32"

Shale_Static_Driver(bus_spi_32);

struct bus_spi_32_device {
    struct iobus_device header;
    bool is_master;
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_cs;
};
#define to_bus_spi_32_device(ptr) container_of(ptr, struct bus_spi_32_device, header)
#define device_to_bus_spi_32_device(ptr) to_bus_spi_32_device(to_iobus_device(ptr))
#define lobject_to_bus_spi_32_device(ptr) device_to_bus_spi_32_device(to_device_instance(ptr))

driver_t *shale_driver_bus_spi_32();

#endif