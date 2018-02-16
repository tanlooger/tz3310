#ifndef __USER_TIM4MOTOR_H__
#define __USER_TIM4MOTOR_H__
#include "stm32f10x.h"

#define MOTOR_PULSE(X, Y)	(GPIO_WriteBit(GPIOB, X, Y))
#define RUNNING     1
#define STOPPING    0

#define MOTOR1_TIMOUT_STEPS     12000
#define MOTOR2_TIMOUT_STEPS     12000
#define MOTOR3_TIMOUT_STEPS     4000
#define MOTOR_ADD_STEPS         800


#define MOTOR3_FOUND_ACTION     1
#define MOTOR3_FOUND_TURN_ACTION     2
#define MOTOR3_TARGET_TO_ACTION     3
#define MOTOR3_TARGET_BEGIN_ACTION     4
#define MOTOR3_TARGET_END_ACTION     5
#define MOTOR3_TARGET_TURN_ACTION     6


void motor_run(u8 motor_id, u8 direction, u32 steps);

void motor_stop(u8 motor_id);

void motor_current_position(u8 status);

void motor_ctrl(void);

void motor_sensor(void);

void initial_vars(void);

#endif

