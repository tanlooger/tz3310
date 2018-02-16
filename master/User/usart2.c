#include "usart2.h"
#include "stm32f10x.h"



extern uint8_t usart2_buffer[256];
extern uint8_t usart2_counter;
extern uint8_t usart2_flag;
extern uint8_t usart2_length;
void USART2_IRQHandler(void)
{
	//uint8_t Clear;
	__IO uint16_t Clear;
	if(USART_GetITStatus(USART2, USART_IT_IDLE)!= RESET){
		USART_ClearITPendingBit(USART2,USART_IT_IDLE); 
		Clear=USART2->SR;
		Clear=USART2->DR;
		/*
		for(i=0; i<usart2_counter; i++){
			USART_SendData(USART2, usart2_buffer[i]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		}
		*/
		usart2_length = usart2_counter;
		usart2_counter = 0;
		usart2_flag = 1;
	}else if(USART_GetITStatus(USART2, USART_IT_RXNE)!= RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
		if(usart2_counter<255)
		usart2_buffer[usart2_counter++] = USART_ReceiveData(USART2);
	}
}




void usart2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//RCC_DeInit();
	//(1)使能被重新映射到的I/O端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//(2)使能被重新映射的外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//(3)使能AFIO功能的时钟(勿忘！)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//PAD5作为US2的TX端，打开复用，负责发送数据
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	//PD6作为US2的RX端，负责接收数据
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

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
	USART_Init(USART2, &USART_InitStructure);
	//打开串口2的中断响应函数，接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	//打开串口2
	USART_Cmd(USART2, ENABLE);


	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//选择串口2中断
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
	NVIC_Init(&NVIC_InitStructure);	
}
