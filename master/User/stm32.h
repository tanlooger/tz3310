#ifndef __USER_STM32_H__
#define __USER_STM32_H__
#include "stm32f10x.h"

#define BOARD_LED_SET(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_8, X))

#define DEVICE_RUNNING  systatue[2]  //0x00 No device running; 0x04 Million ohm running; 0xFF Motor running
#define CUTBOX_HIGH_ONLINE  systatue[5]
#define CUTBOX_MID_ONLINE  systatue[4]
#define CUTBOX_LOW_ONLINE  systatue[3]
#define SLAVE_ONLINE  systatue[0]
#define CH438PORT_OPENING   (DEVICE_RUNNING-3)

#define DC_RESIST_CURRENT   (!(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)))

#define GREEN_LED_SET(X)   (GPIO_WriteBit(GPIOC, GPIO_Pin_5, X))
#define RED_LED_SET(X)     (GPIO_WriteBit(GPIOB, GPIO_Pin_0, X))
#define MASTER_RESET_BUTTON_GET()   (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7))
#define MOTOR_DISABLE()  (GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_SET))
#define MOTOR_ENABLE()  (GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_RESET))
#define MOTOR_PULSE_SET(X)  (GPIO_WriteBit(GPIOD, GPIO_Pin_2, X))
#define MOTOR_PULSE_GET()  (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2))
#define MOTOR_MILLION_OHM_DIRECTION()   (GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET))
#define IS_MOTOR_MILLION_OHM_DIRECTION()   (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_3))
#define MOTOR_DIELECTRIC_LOSS_DIRECTION()   (GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_RESET))
#define IS_MOTOR_DIELECTRIC_LOSS_DIRECTION()  !(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_3))
#define MILLION_OHM_MOTOR_SENSOR_GET()    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
#define DIELECTRIC_LOSS_MOTOR_SENSOR_GET()  (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))



#define MILLION_OHM_ON()    (GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET))
#define MILLION_OHM_OFF()    GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET)
#define DIELECTRIC_LOSS_ON()  (GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET))
#define DIELECTRIC_LOSS_OFF()  (GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET))
#define DC_RESIST_ON()  ((GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET)))
#define DC_RESIST_OFF()  ((GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET)))
#define TAP_SWITCH_ON() ((GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET)))
#define TAP_SWITCH_OFF() ((GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET)))

#define DC_RESIST1_ON()  ((GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET)))
#define DC_RESIST1_OFF()  ((GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET)))
#define DC_RESIST2_ON()  ((GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET)))
#define DC_RESIST2_OFF()  ((GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET)))
#define TAP_SWITCH1_ON() ((GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET)))
#define TAP_SWITCH1_OFF() ((GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET)))
#define TAP_SWITCH2_ON() ((GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET)))
#define TAP_SWITCH2_OFF() ((GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET)))




extern unsigned char systatue[6];

void delay_usrt1(int tim_in);

unsigned char slavstate(unsigned char st_x);// ²éÑ¯´Ô»ú×´Ì¬

void time_led(void);

void stm32_init(void);

int delay_438(int tim_in);

void CLS_all_power(void);

void PE7_judge(void);

void delay(int delay_time);

void HardFault_Handler(void);


void delay_ms(u16 time);


void gpio_init(void);

u8 checksum(u8 data[], u16 length);

#endif
