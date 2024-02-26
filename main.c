#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ftdi.h>

#include "ft_spi.h"
#include "si446x.h"



int
main (int argc, char **argv) {
    int i;
    ft_spi_init();

    si446x_spi_setup(&ft_spi_transmit);

    ft_spi_device_reset();
    usleep(13100UL);
    si446x_init();


    printf("success \n");

    si446x_int_status();
    si446x_int_status();

    for (i = 0; i != 10; i++) {

        si446x_start_tx((unsigned char)i);
        sleep(5);
        si446x_change_state(0x03);
    }

    ft_spi_close();



    return 0;
}


