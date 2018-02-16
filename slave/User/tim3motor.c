#include "stm32f10x.h"
#include "tim3motor.h"

int time_counter;

void tim3motor_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 500; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //时钟预分频数 例如 ：时钟频率=72/(时钟预分频+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
	TIM_Cmd(TIM3, DISABLE);  //去使能TIMx		
	
	//Motor sensor
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &gpio );
	//Motor controller
	gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOD, &gpio );
	
	TIM3->CR1&=0<<0; 
	
	
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET); //Disable motor controller
}
 
extern uint8_t device_run;
uint8_t motor_pulse;
void TIM3_IRQHandler(void)
{
	uint16_t i;
	BitAction p_o;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
		
		if(4 != device_run && 5 != device_run){
			TIM_Cmd(TIM3, DISABLE);
			//TIM3->CR1&=0<<0;    //关闭定时器3
			GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET); //disable motor controller			
		}
		//Shut down TIM3 interrupt
		//if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) && GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1)){
		i=GPIO_ReadInputData(GPIOA);
		if((i&0x0080) && (5==device_run)){
			TIM_Cmd(TIM3, DISABLE);
			//TIM3->CR1&=0<<0;    //关闭定时器3
			GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET); //disable motor controller
		}
		
		if((i&0x0040) && (4==device_run)){
			TIM_Cmd(TIM3, DISABLE);
			//TIM3->CR1&=0<<0;    //关闭定时器3
			GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET); //disable motor controller
		}
		
		motor_pulse = !motor_pulse;
		p_o=(BitAction)motor_pulse;
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, p_o);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, motor_pulse);
	}
}


























void TIM4LED_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 9999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //时钟预分频数 例如 ：时钟频率=72/(时钟预分频+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
	
}


 

void TIM4_IRQHandler(void)
{
		
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		time_counter++;
		
	}
}



