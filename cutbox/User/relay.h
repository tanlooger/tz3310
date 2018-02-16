#ifndef __USER_RELAY_H__
#define __USER_RELAY_H__
#include "stm32f10x.h"





void relay_init(void);





void set_relay(uint8_t value);


uint8_t get_relay_status(void);

#endif
