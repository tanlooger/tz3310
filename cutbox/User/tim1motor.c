#include "tim1motor.h"
#include "motorctrl.h"

extern uint8_t motor_run;
extern int motor_timeout;


void tim1motor_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = 1000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //时钟预分频数 例如 ：时钟频率=72/(时钟预分频+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	//TIM_Cmd(TIM2, ENABLE);  //使能TIMx
	TIM_Cmd(TIM1, DISABLE);  //去使能TIMx
}


uint8_t motor_pulse1;
void TIM1_UP_IRQHandler(void)   
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
      TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
			motor_pulse1 = !motor_pulse1;
			GPIO_WriteBit(GPIOA, GPIO_Pin_11, motor_pulse1);
		}
}


void motor1(uint8_t direction)
{
	while(1){if(0==motor_run)break;}

	delay(10);
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) && MOTOR_HEAD==direction){
	delay(10);		
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) && MOTOR_TAIL==direction){
	delay(10);		
	}else{
	delay(10);		
		motor_run=1;
		motor_timeout = 1000000; //10秒
		GPIO_WriteBit(GPIOA, GPIO_Pin_10, direction);
		GPIO_WriteBit(GPIOA, GPIO_Pin_12, MOTOR_ENABLE);
		TIM_Cmd(TIM1, ENABLE);
	}

}



