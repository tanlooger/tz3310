#include "relay.h"
#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"


void relay_init(void)
{
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
//	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );

	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
//	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &gpio );	
	
	
	gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &gpio );
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
	
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &gpio );
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	
	gpio.GPIO_Pin  = GPIO_Pin_7;//选择IO口   PE7
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &gpio);//使用结构体信息进行初始化IO口
	//stm32_pe7_interrupt();
}




void stm32_pe7_interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
	//1、配置触发源GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;//选择IO口   PE7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//使用结构体信息进行初始化IO口
	
	//2、使能AFIO复用时钟功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//3、将GPIO口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
	
	//4、中断线上进行中断初始化
	EXTI_InitStructure.EXTI_Line=EXTI_Line7; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
	
	//5、中断优先级配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //使能外部中断所在的通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级 2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
	NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化 
}







