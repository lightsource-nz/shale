#ifndef _DRIVER_BUS_SPI_8_H
#define _DRIVER_BUS_SPI_8_H

#include "shale.h"
#include "class/iobus.h"

#define DRIVER_ID_BUS_SPI_8        "shale:driver:bus_spi_8"

Shale_Static_Driver(bus_spi_8);

struct bus_spi_8_device {
    struct iobus_device header;
    bool is_master;
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_cs;
};
#define to_bus_spi_8_device(ptr) container_of(ptr, struct bus_spi_8_device, header)
#define device_to_bus_spi_8_device(ptr) to_bus_spi_8_device(to_iobus_device(ptr))
#define lobject_to_bus_spi_8_device(ptr) device_to_bus_spi_8_device(to_device_instance(ptr))

driver_t *shale_driver_bus_spi_8();

#endif