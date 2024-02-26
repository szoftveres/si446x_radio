#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "ft_spi.h"

static unsigned char out;
static struct ftdi_context *ftdi;


void
ft_spi_init (void) {
    unsigned char bitmask;

    bitmask = (1 << PIN_SS) | (1 << PIN_RST) | (1 << PIN_SCK) | (1 << PIN_MOSI);

    // Init gpio
    ftdi = ft_gpio_init(bitmask);

    // Set initial state
    out = (1 << PIN_SS) | (1 << PIN_RST);
    ft_gpio_set(ftdi, out);
    usleep(USLEEP_LEN);
}


void
ft_spi_close (void) {
    ft_gpio_close(ftdi);
}


void
ft_spi_device_reset (void) {
    out |= (1 << PIN_RST);
    ft_gpio_set(ftdi, out);
    usleep(2000UL);
    out &= ~(1 << PIN_RST);
    ft_gpio_set(ftdi, out);
}


void
ft_spi_transmit (unsigned char *byte, int len) {

    //usleep(USLEEP_LEN * 2);
    out &= ~(1 << PIN_SS);
    ft_gpio_set(ftdi, out);
    //usleep(USLEEP_LEN);

    while (len--) {
        int cycle = 8;
        unsigned char in = 0;
        unsigned char txb = *byte;

        while (cycle--) {
            in = (in << 1);

            // set data
            if (txb & 0x80) {
                out |= (1 << PIN_MOSI);
            } else {
                out &= ~(1 << PIN_MOSI);
            }
            ft_gpio_set(ftdi, out);
            //usleep(USLEEP_LEN);

            // SCK high
            out |= (1 << PIN_SCK);
            ft_gpio_set(ftdi, out);
            //usleep(USLEEP_LEN);

            // read data
            if (ft_gpio_read(ftdi) & (1 << PIN_MISO)) {
                in |= 0x01;
            } else {
                in &= 0xFE;
            }

            // SCK low
            out &= ~(1 << PIN_SCK);
            ft_gpio_set(ftdi, out);

            txb = (txb << 1);
        }
        *byte = in;
        byte += 1;
    }

    //usleep(USLEEP_LEN);
    out |= (1 << PIN_SS);
    ft_gpio_set(ftdi, out);
    //usleep(USLEEP_LEN * 2);
}


