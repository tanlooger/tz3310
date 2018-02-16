#include "usart3.h"


extern u8 ch438q_buffer[2048];
extern u8 ch438q_length;
extern u8 ch438q_flag;

u16 usart3_counter=0;
void USART3_IRQHandler(void)
{
	__IO uint16_t Clear;
	if(USART_GetITStatus(USART3, USART_IT_IDLE)!= RESET){
		USART_ClearITPendingBit(USART3,USART_IT_IDLE); 
		Clear=USART3->SR;
		Clear=USART3->DR;

		ch438q_length = usart3_counter;
		usart3_counter = 0;
		ch438q_flag = 1;

	}else if(USART_GetITStatus(USART3, USART_IT_RXNE)!= RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
        if(usart3_counter<2048)
			ch438q_buffer[usart3_counter++] = USART_ReceiveData(USART3);        
	}
}




void usart3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//RCC_DeInit();
	//(1)使能被重新映射到的I/O端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//(3)使能被重新映射的外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//(3)使能AFIO功能的时钟(勿忘！)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /*
	//PAD5作为US3的TX端，打开复用，负责发送数据
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	//PD6作为US3的RX端，负责接收数据
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    */
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    /* Configure USART3 Tx (PD.8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* Configure USART3 Rx (PD.9) as input Up */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


	//将结构体设置为缺省状态
	USART_StructInit(&USART_InitStructure);
	//波特率设置为115200
	USART_InitStructure.USART_BaudRate = 9600;
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
	//设置串口3
	USART_Init(USART3, &USART_InitStructure);
	//打开串口3的中断响应函数，接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	//打开串口3
	USART_Cmd(USART3, ENABLE);


	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择中断分组3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//选择串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占式中断优先级设置为
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应式中断优先级设置为
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
	NVIC_Init(&NVIC_InitStructure);	
}