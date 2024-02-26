#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_gpio.h"

struct ftdi_context *
ft_gpio_init (unsigned char bitmask) {

    struct ftdi_context *ftdi;
    int f;

    ftdi = ftdi_new();
    if (!ftdi) {
        fprintf(stderr, "ftdi_new failed\n");
        return NULL;
    }

    f = ftdi_usb_open(ftdi, 0x0403, 0x6014);

    if (f < 0 && f != -5) {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        return NULL;
    }

    ftdi_set_bitmode(ftdi, bitmask, BITMODE_BITBANG);

    fprintf(stderr, "ftdi open succeeded: %d\n",f);

    return ftdi;
}


void
ft_gpio_close (struct ftdi_context *ftdi) {
    ftdi_disable_bitbang(ftdi);

    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
}


void
ft_gpio_set (struct ftdi_context *ftdi, unsigned char byte) {
    ftdi_write_data(ftdi, &byte, 1);
}


unsigned char
ft_gpio_read (struct ftdi_context *ftdi) {
    unsigned char byte;

    ftdi_read_pins(ftdi, &byte);

    return byte;
}


