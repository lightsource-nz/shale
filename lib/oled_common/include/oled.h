#ifndef _OLED_H
#define _OLED_H

#include <stdint.h>

#define OLED_TRANSPORT_I2C          0
#define OLED_TRANSPORT_SPI          1
#define OLED_TRANSPORT_PARALLEL     2

#define OLED_PIN_NC                 UINT8_MAX

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

void shale_oled_spi_device_setup(spi_port_t *port);

#endif