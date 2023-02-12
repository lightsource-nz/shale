#include <shale.h>
#include <class/display.h>
#include <driver/sh1107.h>
#include <class/iobus.h>
#include <driver/bus_spi.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_BUS_SPI_0    "device:bus.spi.0"
#define ID_DISPLAY_MAIN "device:display.main"

Shale_Static_Device_Define(bus_spi_0, ID_BUS_SPI_0, bus_spi)
Shale_Static_Device_Define(display_main, ID_DISPLAY_MAIN, sh1107)

void main()
{
    shale_init();
    
    __breakpoint();
}
