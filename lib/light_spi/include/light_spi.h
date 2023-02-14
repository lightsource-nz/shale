#ifndef _LIGHT_SPI_H
#define _LIGHT_SPI_H

#include <stdint.h>

#ifdef PICO_RP2040
#   include <hardware/spi.h>
#endif

#define LIGHT_SPI_PIN_NC                 UINT8_MAX

#define LIGHT_SPI_SPI_FLAG_PHASE         0
#define LIGHT_SPI_SPI_FLAG_POLARITY      1
#define LIGHT_SPI_SPI_FLAG_ORDER         2

#define light_spi_flag(flag)         (1 << (flag))
#define light_spi_flag_enabled(flag, value) \
                                    ((value) & light_spi_flag(flag))

struct spi_port {
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_cs;
    uint8_t pin_dc;
    uint8_t flags;
};

void light_spi_port_init(struct spi_port *port);
void light_spi_port_bind(struct spi_port *port, uint8_t flags,
                            uint8_t pin_sck, uint8_t pin_miso, uint8_t pin_mosi,
                            uint8_t pin_cs, uint8_t pin_dc);

#endif