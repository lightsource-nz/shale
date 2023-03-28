#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>
#include <class/iobus.h>
#include <driver/bus_spi_8.h>
#include <driver/bus_spi_32.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_BUS_SPI_0    "device:bus.spi.0"
#define ID_BUS_SPI_1    "device:bus.spi.1"
#define ID_DISPLAY_MAIN "display.main"

Shale_Static_Interface_Define(if_bus_spi_0, "interface:" ID_BUS_SPI_0, bus_spi_32)
Shale_Static_Interface_Define(if_bus_spi_1, "interface:" ID_BUS_SPI_0, bus_spi_8)
Shale_Static_Interface_Define(if_display_main, "interface:" ID_DISPLAY_MAIN, sh1107)

// we use the variant of the SPI driver with a 32-bit frame size
Shale_Static_Device_Define(bus_spi_0, ID_BUS_SPI_0, if_bus_spi_0)
Shale_Static_Device_Define(bus_spi_1, ID_BUS_SPI_1, if_bus_spi_1)
Shale_Static_Device_Define(display_main, ID_DISPLAY_MAIN, if_display_main)

void main()
{
    shale_init();
    
    __breakpoint();
}
