#ifndef __USER_TIM3LAMP_H__
#define __USER_TIM3LAMP_H__
#include "stm32f10x.h"



#define BLINK_SLOW      1
#define BLINK_FAST      2
#define ALWAYS_LIGHT    3

void tim3lamp_init(void);



void led_lamp_blink(void);



#endif



