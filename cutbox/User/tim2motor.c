#include "tim2motor.h"
#include "motorctrl.h"
extern uint8_t motor_run;
extern int motor_timeout;

void tim2motor_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = 1000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //时钟预分频数 例如 ：时钟频率=72/(时钟预分频+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	//TIM_Cmd(TIM2, ENABLE);  //使能TIMx
	TIM_Cmd(TIM2, DISABLE);  //去使能TIMx		
}

uint8_t motor_pulse2;
void TIM2_IRQHandler(void)
{
	uint16_t i;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		motor_pulse2 = !motor_pulse2;
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, motor_pulse2);
	}
}


void motor2(uint8_t direction)
{
	while(1){if(0==motor_run)break;}
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15) && MOTOR_HEAD==direction){
		
	}else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) && MOTOR_TAIL==direction){
		
	}else{
		motor_run=2;
		motor_timeout = 1000000; //10秒
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, direction);
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, MOTOR_ENABLE);
		TIM_Cmd(TIM2, ENABLE);
	}
}










