#ifndef __USER_CH438Q_H__
#define __USER_CH438Q_H__
#include <stdint.h>

#define CH438_Pin_ALE GPIO_Pin_13   //B
#define CH438_Pin_WR GPIO_Pin_12    //B
#define CH438_Pin_RD GPIO_Pin_14    //B
#define CH438_Pin_CS GPIO_Pin_15    //B
#define CH438_Pin_INT GPIO_Pin_6   //C

#define CH438_Pin_D0 GPIO_Pin_8     //D
#define CH438_Pin_D1 GPIO_Pin_9			//D
#define CH438_Pin_D2 GPIO_Pin_10			//D
#define CH438_Pin_D3 GPIO_Pin_11			//D
#define CH438_Pin_D4 GPIO_Pin_12			//D
#define CH438_Pin_D5 GPIO_Pin_13			//D
#define CH438_Pin_D6 GPIO_Pin_14		//D
#define CH438_Pin_D7 GPIO_Pin_15	//D


#define CH438_REG_RBR_THR_DLL	0x00
#define CH438_REG_IER_DLM			0x01
#define CH438_REG_IIR_FCR			0x02
#define CH438_REG_LCR					0x03
#define CH438_REG_MCR					0x04
#define CH438_REG_LSR					0x05
#define CH438_REG_MSR					0x06
#define CH438_REG_SCR					0x07



uint8_t ch438_ssr_get_port(void);

void EXTI9_5_IRQHandler(void);

void ch438_init(void);

uint8_t ch438_reg_read(uint8_t reg);
void ch438_reg_write(uint8_t reg, uint8_t value);
extern char ch438_bool;

#endif
