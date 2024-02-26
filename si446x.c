#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include "si446x.h"

static spibus_t spi;

#define DEBUG_SPI_RESP_UNIX(buf, len) \
{ int i; \
  printf("%s [", __FUNCTION__); \
  for (i = 0; i != len; i++) { \
    printf("0x%X ", buf[i]); \
  } \
  printf("]\n"); \
}


#define DEBUG_SPI_RESP(buf, len)

void
si446x_spi_setup (void (*transmit) (unsigned char *byte, int len)) {
    spi.transmit = transmit;
}


void
si446x_nop (void) {
    unsigned char bb[] = {0x00, 0x00};
    spi.transmit(bb, 2);
    DEBUG_SPI_RESP(bb, 2)
}


int
si446x_check_cts (void) {
    unsigned char bb[] = {0x44, 0x00};
    spi.transmit(bb, 2);
    DEBUG_SPI_RESP(bb, 2)
    return (bb[1] == 0xFF);
}


void
si446x_wait_cts (void) {
    int i;

    for (i = 0; i != 100; i++)
    {
        if (si446x_check_cts()) {
            return;
        }
        usleep(500UL);
    }

    fprintf(stderr, "Si446x CTS timeout\n");
    exit(-1);
}

void
si446x_cmd (unsigned char* dp, int len) {
    unsigned char dat[18];

    memcpy(dat, dp, len);
    si446x_wait_cts();

    spi.transmit(dat, len);
    DEBUG_SPI_RESP(dat, len);
}

void
si446x_resp (unsigned char* dp, int len) {
    unsigned char dat[18];

    memset(dat, 0x00, len + 2);
    dat[0] = 0x44;

    si446x_wait_cts();

    spi.transmit(dat, len + 2);
    DEBUG_SPI_RESP(dat, len + 2);
    memcpy(dp, &(dat[2]), len);
}


void
si446x_part_info (void)
{
    unsigned char cmd[] = {0x01};
    unsigned char resp[8];
    si446x_cmd(cmd, (sizeof(cmd) / sizeof(cmd[0])));
    si446x_resp(resp, 8);
    printf("Si%x%x rev:%x \n", resp[1], resp[2], resp[0]);
}


void
si446x_int_status (void) {
    unsigned char cmd[] = {0x20, 0x00, 0x00, 0x00};
    unsigned char resp[8];
    si446x_cmd(cmd, (sizeof(cmd) / sizeof(cmd[0])));
    si446x_resp(resp, 8);
    DEBUG_SPI_RESP_UNIX(resp, 8);
}


void
si446x_start_tx (unsigned char channel) {
    unsigned char cmd[] = {0x31, 0x00, 0x00, 0x00, 0x00};
    cmd[1] = channel;
    si446x_cmd(cmd, (sizeof(cmd) / sizeof(cmd[0])));
}


void
si446x_change_state (unsigned char state) {
    unsigned char cmd[] = {0x34, 0x00};
    cmd[1] = state;
    si446x_cmd(cmd, (sizeof(cmd) / sizeof(cmd[0])));
}


void
si446x_init (void)
{
    int i = 0;
    unsigned char data[] = RADIO_CONFIGURATION_DATA_ARRAY;
    unsigned char *dp = data;
    /*
    https://community.silabs.com/s/article/si4x6x-c2a-si4x55-c2a-startup-sequence?language=en_US
    */
    si446x_nop();

    while (*dp) {
        int len = (int)*dp;
        dp += 1;

        si446x_cmd(dp, len);
        dp += len;
        i++;
    }
    printf("%i cfg lines done\n", i);
    si446x_wait_cts();
    si446x_part_info();
}



