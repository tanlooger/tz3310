#include "usart1.h"
#include "stm32f10x.h"



extern uint8_t usart1_ch438q_buffer[2048];
extern uint16_t usart1_ch438q_counter;
extern uint8_t usart1_flag;
extern uint16_t usart1_ch438q_length;
void USART1_IRQHandler(void)
{
	//uint8_t Clear;
	__IO uint16_t Clear;
	if(USART_GetITStatus(USART1, USART_IT_IDLE)!= RESET){
		USART_ClearITPendingBit(USART1,USART_IT_IDLE); 
		Clear=USART1->SR;
		Clear=USART1->DR;
		/*
		for(i=0; i<usart1_ch438q_counter; i++){
			USART_SendData(USART1, usart1_ch438q_buffer[i]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
		*/
		usart1_ch438q_length = usart1_ch438q_counter;
		usart1_ch438q_counter = 0;
		usart1_flag = 1;
	}else if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
		if(usart1_ch438q_counter<2047)
		usart1_ch438q_buffer[usart1_ch438q_counter++] = USART_ReceiveData(USART1);
	}
}





void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//RCC_DeInit();
	//(1)使能被重新映射到的I/O端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//(2)使能被重新映射的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//(3)使能AFIO功能的时钟(勿忘！)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//PAD5作为US2的TX端，打开复用，负责发送数据
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	//PD6作为US2的RX端，负责接收数据
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//将结构体设置为缺省状态
	USART_StructInit(&USART_InitStructure);
	//波特率设置为115200
	USART_InitStructure.USART_BaudRate = 115200;
	//一帧数据的宽度设置为8bits
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//在帧结尾传输1个停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//奇偶失能模式，无奇偶校验
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//发送/接收使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//硬件流控制失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//设置串口2
	USART_Init(USART1, &USART_InitStructure);
	//打开串口2的中断响应函数，接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	//打开串口2
	USART_Cmd(USART1, ENABLE);


	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//选择串口2中断
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
	NVIC_Init(&NVIC_InitStructure);	
}









