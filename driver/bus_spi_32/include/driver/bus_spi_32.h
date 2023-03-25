#ifndef _DRIVER_BUS_SPI_32_H
#define _DRIVER_BUS_SPI_32_H

// driver bus_spi_32
// SPI interface driver with 32-bit transfer frame size

#include "shale.h"
#include "class/iobus.h"

#define DRIVER_ID_BUS_SPI_32        "shale:driver:bus_spi_32"

Shale_Static_Driver(bus_spi_32);

struct bus_spi_32_interface {
    struct iobus_interface header;
    bool is_master;
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_cs;
};
#define to_bus_spi_32_interface(ptr) container_of(ptr, struct bus_spi_32_interface, header)
#define device_to_bus_spi_32_interface(ptr) to_bus_spi_32_interface(to_iobus_interface(ptr))
#define lobject_to_bus_spi_32_interface(ptr) device_to_bus_spi_32_interface(to_device_interface(ptr))

struct device_interface *shale_interface_bus_spi_32();

#endif