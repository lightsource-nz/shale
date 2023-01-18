#include <oled.h>

void shale_oled_ioport_init(ioport_t *port)
{
    #ifdef PICO_RP2040
        if(port)
    #endif
}

void shale_oled_ioport_bind_spi(ioport_t *port, uint8_t pin_sck, uint8_t pin_miso, uint8_t pin_mosi, uint8_t pin_cs, uint8_t pin_dc)
{
    port->port_type = OLED_PORT_SPI;
    port->bind.spi = (spi_port_t) {
        .pin_sck = pin_sck,
        .pin_miso = pin_miso,
        .pin_mosi = pin_mosi,
        .pin_cs = pin_cs,
        .pin_dc = pin_dc
    };
}
