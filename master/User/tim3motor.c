#include "tim3motor.h"
#include "stm32.h"
#include "cutbox.h"

uint8_t time_counter=0;
uint8_t cutbox_timeout = 0;
uint8_t cutbox_all_timeout = 0;

int motor_delay = 0;
void tim3motor_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	
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
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
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
	
	
	MOTOR_DISABLE(); //Disable motor controller
}
 
//uint8_t motor_pulse;
//extern unsigned char systatue[6];
//extern struct message usart2_msg;
extern u8 device_start_flag;
void TIM3_IRQHandler(void)
{
	//uint16_t i;
	//BitAction p_o;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
		
		if(4 != DEVICE_RUNNING && 5 != DEVICE_RUNNING && 0xFF!=DEVICE_RUNNING){
			TIM_Cmd(TIM3, DISABLE);
			//TIM3->CR1&=0<<0;    //关闭定时器3
			MOTOR_DISABLE(); //disable motor controller			
		}
		//Shut down TIM3 interrupt
		//if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) && GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1)){
		//i=GPIO_ReadInputData(GPIOA);
		//if((i&0x0080) && (4==device_run)){
		if(MILLION_OHM_MOTOR_SENSOR_GET() && IS_MOTOR_MILLION_OHM_DIRECTION()){
            motor_delay++;
            if(motor_delay>800){
                motor_delay = 0;
                TIM_Cmd(TIM3, DISABLE);
                //TIM3->CR1&=0<<0;    //关闭定时器3
                MOTOR_DISABLE(); //disable motor controller
                //delay(1000);
                MILLION_OHM_ON();//继电器
                DEVICE_RUNNING=4;
                device_start_flag = 1;
            }
		}else
		
		//if((i&0x0040) && (5==device_run)){
		if(DIELECTRIC_LOSS_MOTOR_SENSOR_GET() && IS_MOTOR_DIELECTRIC_LOSS_DIRECTION())
        {
            motor_delay++;
            if(motor_delay>800){
                motor_delay = 0;
                TIM_Cmd(TIM3, DISABLE);
                //TIM3->CR1&=0<<0;    //关闭定时器3
                MOTOR_DISABLE(); //disable motor controller
                //delay(1000);
                DIELECTRIC_LOSS_ON();//继电器
                DEVICE_RUNNING=5;
                device_start_flag = 1;
            }
		}
		
		//motor_pulse = !motor_pulse;
		//p_o=(BitAction)motor_pulse;
        //MOTOR_PULSE_SET((BitAction)motor_pulse);
        MOTOR_PULSE_SET((BitAction)(!MOTOR_PULSE_GET()));
		//GPIO_WriteBit(GPIOD, GPIO_Pin_2, p_o);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, motor_pulse);
	}
}


























void TIM4LED_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef gpio;
	
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
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
	

	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
}


 

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		time_counter++;
		if(cutbox_timeout <255 && cutbox_timeout)cutbox_timeout++;
		if(cutbox_all_timeout <255 && cutbox_all_timeout)cutbox_all_timeout++;
        if(cutbox_check_delay>0)cutbox_check_delay--;
	}
}



