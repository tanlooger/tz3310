#include "stm32f10x.h"
#ifndef __USER_STM32_H__
#define __USER_STM32_H__


extern uint8_t usart1_flag;


//extern uint8_t ch438q_buffer[2048];
extern uint8_t ch438q_buffer2[2048];

//extern uint16_t ch438q_length;
extern uint16_t ch438q_length2;
//extern uint16_t ch438q_counter;

extern uint8_t ch438q_timeout;
extern uint8_t ch438q_port;
extern uint8_t ch438q_flag;

extern uint8_t usart1_buffer[256];
extern uint8_t usart1_buffer_bak[256];
extern uint8_t usart1_flag;
extern uint8_t usart1_length;
extern uint8_t usart1_length_bak;
extern uint8_t usart1_counter;


extern uint8_t usart2_buffer[256];
extern uint8_t usart2_flag;
extern uint8_t usart2_length;
extern uint8_t usart2_counter;

extern uint8_t Power_State;
extern char LEDCHANCE;

void led_show(void);
void time_led(void);

void stm32_init(void);

int delay_438(int tim_in);

void CLS_all_power(void);

void Power_judge(void);

void delay(int delay_time);

void HardFault_Handler(void);


void delay_ms(u16 time);


void delay_us(u16 time);


#endif
