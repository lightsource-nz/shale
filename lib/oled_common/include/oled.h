#ifndef _OLED_H
#define _OLED_H

#include <stdint.h>

#ifdef PICO_RP2040
#   include <hardware/spi.h>
#   include <hardware/i2c.h>
#endif

#define OLED_PORT_I2C          0
#define OLED_PORT_SPI          1
#define OLED_PORT_PARALLEL     2

#define OLED_PIN_NC                 UINT8_MAX

#define OLED_SPI_FLAG_PHASE         0
#define OLED_SPI_FLAG_POLARITY      1
#define OLED_SPI_FLAG_ORDER         2

#define oled_spi_flag(flag)         (1 << (flag))
#define oled_spi_flag_enabled(flag, value) \
                                    ((value) & oled_spi_flag(flag))

typedef struct i2c_port {
    uint8_t pin_scl;
    uint8_t pin_sda;
} i2c_port_t;

typedef struct spi_port {
    uint8_t pin_sck;
    uint8_t pin_miso;
    uint8_t pin_mosi;
    uint8_t pin_dc;
} spi_port_t;

typedef union ioport_bind {
    i2c_port_t i2c;
    spi_port_t spi;
} ioport_bind_t;

// a (hopefully) portable structure representing the hardware
// of an IO-port peripheral
typedef struct ioport {
    uint8_t port_type;
    uint8_t port_number;
    uint8_t flags;
    ioport_bind_t bind;
} ioport_t;

void shale_oled_ioport_init(ioport_t *port);

#endif