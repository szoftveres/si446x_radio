#ifndef __FT_SPI__
#define __FT_SPI__

#include "ft_gpio.h"

#define PIN_SS    (3)
#define PIN_RST   (5)
#define PIN_SCK   (0)
#define PIN_MISO  (2)
#define PIN_MOSI  (1)

#define USLEEP_LEN  (10)



void
ft_spi_init (void);

void
ft_spi_close (void);

void
ft_spi_device_reset (void);

void
ft_spi_transmit (unsigned char *byte, int len);

#endif
