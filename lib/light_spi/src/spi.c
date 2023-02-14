#include <light_spi.h>

void light_spi_port_init(struct spi_port *port)
{
#ifdef PICO_RP2040
        if(port)
#endif
}

void light_spi_port_bind(struct spi_port *port, uint8_t flags,
                            uint8_t pin_sck, uint8_t pin_miso, uint8_t pin_mosi, uint8_t pin_cs, uint8_t pin_dc)
{
        *port = (struct spi_port) {
                .pin_sck = pin_sck,
                .pin_miso = pin_miso,
                .pin_mosi = pin_mosi,
                .pin_cs = pin_cs,
                .pin_dc = pin_dc,
                .flags = flags
        };
}
