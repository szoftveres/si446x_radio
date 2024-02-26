#ifndef __FT_GPIO__
#define __FT_GPIO__

#include <ftdi.h>


struct ftdi_context *
ft_gpio_init (unsigned char bitmask);

void
ft_gpio_close (struct ftdi_context *ftdi);

void
ft_gpio_set (struct ftdi_context *ftdi, unsigned char byte);

unsigned char
ft_gpio_read (struct ftdi_context *ftdi);


#endif
