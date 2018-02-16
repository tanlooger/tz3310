#ifndef __USER_TIM3MOTOR_H__
#define __USER_TIM3MOTOR_H__
#include "stm32f10x.h"

void tim3motor_init(void);

void TIM4LED_init(void);

extern uint8_t time_counter;

extern uint8_t cutbox_timeout;

extern uint8_t cutbox_all_timeout;

#endif
