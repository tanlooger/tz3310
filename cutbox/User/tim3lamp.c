#include "tim3lamp.h"
#include "stm32.h"

extern u8 motor_command;
extern u8 relay_command;
extern u8 motor_status;
extern u8 relay_status;

u16 led_counter;
u8 led_onoff=0;

void tim3lamp_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????
	
	//???TIM3???
	TIM_TimeBaseStructure.TIM_Period = 999; //???????????????????????????	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //?????? ?? :????=72/(?????+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??????????TIMx???????
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //?????TIM3??,??????

	//?????NVIC??
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???

	TIM_Cmd(TIM3, ENABLE);  //??TIMx		
}




void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(led_counter>0)led_counter--;
	}
}




extern u8 led_blink;
extern u8 CUTBOX_ID;
void led_lamp_blink(void)
{
	if(led_counter)return;
	led_counter = 1000;
	led_onoff = !led_onoff;
	LED(led_onoff);
    CUTBOX_ID = (CUTBOX_ID_1<<1)+CUTBOX_ID_2;
    
    motor_status = get_motor_status();
    relay_status = get_relay_status();
    
    if(motor_status==motor_command && relay_status==(relay_command&0x3F))
    {
        if(MOTOR_INIT_COMMAND==motor_status && RELAY_INIT_COMMAND==relay_status)
            led_blink = ALWAYS_LIGHT;
        else
            led_blink = BLINK_FAST;
    }
    else
    {
        led_blink = BLINK_FAST;
    }    
	
	if(BLINK_SLOW == led_blink)
	{
		LAMP(led_onoff);
	}else
	if(BLINK_FAST == led_blink)
	{
		LAMP(led_onoff);
		led_counter = 100;
	}else
	if(ALWAYS_LIGHT == led_blink)
	{
        if(led_onoff)
        {
            led_counter = 500;
        }
        else
        {
            led_counter = 2000;
        }
		LAMP(led_onoff);
        
        
	}
}


