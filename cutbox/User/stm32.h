#ifndef __USER_STM32_H__
#define __USER_STM32_H__
#include "stm32f10x.h"


#define MOTOR1_SENSOR_HEAD  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
#define MOTOR1_SENSOR_TAIL  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15))
#define MOTOR2_SENSOR_HEAD  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14))
#define MOTOR2_SENSOR_TAIL  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13))
#define MOTOR3_SENSOR_O  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12))
#define MOTOR3_SENSOR_A  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))
#define MOTOR3_SENSOR_B  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))
#define MOTOR3_SENSOR_C  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))


#define MOTOR3_O    4
#define MOTOR3_A    5
#define MOTOR3_B    6
#define MOTOR3_C    7

#define MOTOR1_EN(X)    (GPIO_WriteBit(GPIOA, GPIO_Pin_12, X))
#define MOTOR2_EN(X)    (GPIO_WriteBit(GPIOA, GPIO_Pin_10, X))
#define MOTOR3_EN(X)    (GPIO_WriteBit(GPIOA, GPIO_Pin_8, X))

#define MOTOR1_DIR_OUT(X)	(GPIO_WriteBit(GPIOA, GPIO_Pin_11, X))
#define MOTOR1_DIR_IN()	    (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11))
#define MOTOR2_DIR_OUT(X)	(GPIO_WriteBit(GPIOA, GPIO_Pin_9, X))
#define MOTOR2_DIR_IN()	    (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_9))
#define MOTOR3_DIR_OUT(X)	(GPIO_WriteBit(GPIOC, GPIO_Pin_9, X))
#define MOTOR3_DIR_IN()	    (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9))

#define MOTOR_HEAD  0
#define MOTOR_TAIL  1
#define MOTOR_ENABLE	0
#define MOTOR_DISABLE	1

#define RELAY_READ_0    (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_13))
#define RELAY_READ_1    (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14))
#define RELAY_READ_2    (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15))
#define RELAY_READ_3    (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12))
#define RELAY_READ_4    (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2))
#define RELAY_READ_5    (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1))
//#define RELAY_READ_6    (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8))


#define RELAY_WRITE_0(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_13, X))
#define RELAY_WRITE_1(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_14, X))
#define RELAY_WRITE_2(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_15, X))
#define RELAY_WRITE_3(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_12, X))
#define RELAY_WRITE_4(X)    (GPIO_WriteBit(GPIOA, GPIO_Pin_2, X))
#define RELAY_WRITE_5(X)    (GPIO_WriteBit(GPIOA, GPIO_Pin_1, X))
#define BOARD_GROUND(X)    (GPIO_WriteBit(GPIOC, GPIO_Pin_8, X))


#define LED(X)  (GPIO_WriteBit(GPIOD, GPIO_Pin_7, X))
#define LAMP(X)  (GPIO_WriteBit(GPIOD, GPIO_Pin_8, X))


//#define DISCHARGE   (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))

#define CUTBOX_ID_1 (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
#define CUTBOX_ID_2 (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))


#define MOTOR_INIT_COMMAND  0x1A
#define RELAY_INIT_COMMAND  0x00


void stm32_init(void);

void gpio_init(void);

void tim4_init(void);

void usart3_init(void);


void delay_ms(u16 time);


u8 get_relay_status(void);


u8 get_motor_status(void);


u8 checksum(u8 data[], u16 length);

#endif



