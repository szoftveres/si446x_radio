#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

#define RF_POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80

#define RADIO_CONFIGURATION_DATA_ARRAY { \
        0x07, RF_POWER_UP, \
        0x00 \
}

#endif

