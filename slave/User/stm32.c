#include "stm32.h"
















void stm32_init(void)
{
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );
	
	
}


void delay_ms(u16 time)
{
   u16 i=0;
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}



void delay_us(u16 time)
{
   u16 i=0;
   while(time--)
   {
      i=12;  //自己定义
      while(i--) ;    
   }
}