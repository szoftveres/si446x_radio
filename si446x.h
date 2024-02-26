#ifndef __SI446X_H__
#define __DEFINE_H__

#include "radio_config_Si4461_b1_cw.h"


typedef struct spibus_s {

    void (*transmit) (unsigned char *byte, int len);

} spibus_t;


void
si446x_spi_setup (void (*transmit) (unsigned char *byte, int len));

void
si446x_int_status (void);

void
si446x_start_tx (unsigned char channel);

void
si446x_change_state (unsigned char state);

void
si446x_init (void);

#endif

