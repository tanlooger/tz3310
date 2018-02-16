#ifndef __USER_MOTORCTRL_H__
#define __USER_MOTORCTRL_H__
#include "stm32f10x.h"



void motor_init(void);


void motor_error(void);


void delay(int delay_time);


uint8_t get_motor_status(void);

#endif

