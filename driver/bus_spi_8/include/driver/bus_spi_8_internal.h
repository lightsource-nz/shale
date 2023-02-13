#ifndef _BUS_SPI_8_INTERNAL_H
#define _BUS_SPI_8_INTERNAL_H

#ifndef _DRIVER_BUS_SPI_8_H
#   error __file__ "must be included after driver/bus_spi_8.h"
#endif

void shale_driver_bus_spi_8_init_device(device_t *device);
Message_Handler(shale_driver_bus_spi_8_handle_message);

#endif
