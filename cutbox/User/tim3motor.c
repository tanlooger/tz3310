#include "tim3motor.h"
#include "motorctrl.h"

extern uint8_t motor_running;
extern int motor_timeout;
extern uint8_t motor3_position;


void tim3motor_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 1000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
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
	//TIM3->CR1&=0<<0; 
	TIM_Cmd(TIM3, DISABLE);
}


 
uint8_t motor_pulse3=0;
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );

		motor_pulse3 = !motor_pulse3;
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, motor_pulse3);
	}
	
}

/*
void motor3(uint8_t direction)
{
	while(1){if(0==motor_run)break;}
	
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) && MOTOR_HEAD==direction){

	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) && MOTOR_TAIL==direction){

	}else{
		motor_run=3;
		motor_timeout = 1000000;
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, direction);
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, MOTOR_ENABLE);
		TIM_Cmd(TIM3, ENABLE);
	}
}
*/


void motor3(uint8_t position)
{
	uint8_t PE9_1, PE9_2, PE12_1, PE12_2;
	uint8_t direction;
	motor3_position = position;

	while(1){if(0==motor_running)break;}
	PE9_1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
	//delay(10);
	PE9_2 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
	
	PE12_1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);
	//delay(10);	
	PE12_2 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);
	if((PE9_1 || PE9_2) && 1==motor3_position){
		
	}else if((PE12_1 || PE12_2) && 4==motor3_position){

	}else if(motor3_position==1 || motor3_position==2 || motor3_position==3 || motor3_position==4){
		direction = (1==motor3_position) ? MOTOR_HEAD : MOTOR_TAIL;
		
		motor_running=3;
		motor_timeout = 1000000;
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, direction);
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, MOTOR_ENABLE);
		TIM_Cmd(TIM3, ENABLE);
	}
}



