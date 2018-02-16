#ifndef __USER_CH438Q_H__
#define __USER_CH438Q_H__
#include "stm32f10x.h"





#define CH438Q_RBR_OFFSET   0
#define CH438Q_THR_OFFSET   0
#define CH438Q_DLL_OFFSET   0
#define CH438Q_IER_OFFSET   1
#define CH438Q_DLM_OFFSET   1
#define CH438Q_IIR_OFFSET   2
#define CH438Q_FCR_OFFSET   2
#define CH438Q_LCR_OFFSET   3
#define CH438Q_MCR_OFFSET   4
#define CH438Q_LSR_OFFSET   5
//#define CH438Q_MSR_OFFSET   6
//#define CH438Q_SCR_OFFSET   7



#define CH438Q_GetITStatus(PORT)   (ch438_reg_read(CH438_BASE_ADDR[PORT] + CH438Q_IIR_OFFSET) & 0x0F)

#define CH438Q_IT_RXNE  0x04
#define CH438Q_IT_IDLE  0x0C


#define CH438Q_D_GET()      ((u8)(GPIO_ReadInputData(GPIOD)>>8))
#define CH438Q_D_SET(X)     (GPIO_Write(GPIOD, (((u16)X<<8)|(GPIO_ReadOutputData(GPIOD)&0x00ff))))

#define CH438Q_WR_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_12, X))
#define CH438Q_ALE_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_13, X))
#define CH438Q_RD_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_14, X))
#define CH438Q_CS_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_15, X))



extern u8 ch438q_buffer[256];
extern u8 ch438q_flag;
extern u8 ch438q_length;

u8 ch438_ssr_get_port(void);

void EXTI9_5_IRQHandler(void);

void ch438_init(void);

u8 ch438_reg_read(uint8_t reg);

void ch438_reg_write(uint8_t reg, uint8_t value);

void ch438_put_char(u8 port, u8 value);

#endif
