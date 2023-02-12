#ifndef _DRIVER_BUS_SPI_H
#define _DRIVER_BUS_SPI_H

#include "shale.h"
#include "class/iobus.h"

#define DRIVER_ID_BUS_SPI        "shale:driver:bus_spi"

Shale_Static_Driver(bus_spi);

struct bus_spi_device {
    struct iobus_device header;
    bool is_master;
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_cs;
};
#define to_bus_spi_device(ptr) container_of(ptr, struct bus_spi_device, header)
#define device_to_bus_spi_device(ptr) to_bus_spi_device(to_iobus_device(ptr))
#define lobject_to_bus_spi_device(ptr) device_to_bus_spi_device(to_device_instance(ptr))

uint8_t shale_driver_bus_spi_init();
driver_t *shale_driver_bus_spi();
uint8_t shale_driver_bus_spi_device_init(struct device *device, const uint8_t *id);

#endif