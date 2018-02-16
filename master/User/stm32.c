#include "stm32.h"
















void stm32_init(void)
{
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}



void gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    
    //Current check  low voltage means have current
    gpio.GPIO_Pin = GPIO_Pin_4;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &gpio ); 
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

u8 checksum(u8 data[], u16 length)
{
	int sum=0;
	int i=0;
	for(i=0;i<length;i++){
		sum+=(int)data[i];
	}
	return sum%256; //return the last 8 bits;
}


