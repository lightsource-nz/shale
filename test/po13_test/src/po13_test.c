#include <shale.h>

#include <class/display.h>
#include <class/iobus.h>

#include <driver/sh1107.h>
#include <driver/bus_spi_8.h>
#include <driver/bus_spi_32.h>

#include <pico/platform.h>
#include <stdlib.h>

#define ID_BUS_SPI_0    "bus.spi.0"
#define ID_BUS_SPI_1    "bus.spi.1"
#define ID_DISPLAY_MAIN "display.main"

Shale_Static_Interface_Define(if_bus_spi_0, "interface:" ID_BUS_SPI_0, bus_spi_32)
Shale_Static_Interface_Define(if_bus_spi_1, "interface:" ID_BUS_SPI_1, bus_spi_8)
Shale_Static_Interface_Define_Ref(if_display_main, "interface:" ID_DISPLAY_MAIN, sh1107, 1, Static_Interface_Ref(ref_bus, if_bus_spi_1))

// we use the variant of the SPI driver with a 32-bit frame size
Shale_Static_Device_Define(bus_spi_0, "device:" ID_BUS_SPI_0, if_bus_spi_0)
Shale_Static_Device_Define(bus_spi_1, "device:" ID_BUS_SPI_1, if_bus_spi_1)
Shale_Static_Device_Define(display_main, "device:" ID_DISPLAY_MAIN, if_display_main)

void main()
{
    shale_init();
    
    __breakpoint();
}
