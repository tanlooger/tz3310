#include "ch438q.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
#include "stm32f10x_gpio.h"
#include "usart2.h"
/* Library includes. */
//#include "stm32f10x_it.h"
//#include "stm32f10x_exti.h"

extern const uint16_t CH438_BASE_ADDR[8];



extern uint8_t usart1_ch438q_buffer[2048];
extern uint8_t ch438q_flag;
extern uint16_t usart1_ch438q_length;
extern uint16_t usart1_ch438q_counter;

extern uint8_t ch438q_timeout;
extern uint8_t ch438q_port;
char ch438_bool;


void EXTI9_5_IRQHandler(void)
{
	uint8_t port,d_bak;
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		port = ch438_ssr_get_port();
		d_bak=ch438_reg_read(CH438_BASE_ADDR[port] + CH438_REG_RBR_THR_DLL);
	//	ch438q_timeout = 50;
		if(ch438q_port == port)
		{
			ch438_bool=1;
			ch438q_timeout = 30;
			usart1_ch438q_buffer[usart1_ch438q_counter++] = d_bak;//ch438_reg_read(CH438_BASE_ADDR[port] + CH438_REG_RBR_THR_DLL);
		}
		//else	ch438_reg_write(CH438_BASE_ADDR[port]+CH438_REG_RBR_THR_DLL, ch438_reg_read(CH438_BASE_ADDR[port] + CH438_REG_RBR_THR_DLL));	
		//}
		return;
	}
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
		ch438q_flag = 0;
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)){
				GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
				GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
		}else{
				GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_SET);
				GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
		}
	}
}




void SysTick_Handler(void)
{
		//ch438_reg_write(0x10, 0xaa);
		if(0 == ch438q_timeout){
			u8 n=0;
			ch438q_flag = 1;
			ch438_bool=0x00;
			usart1_ch438q_length = usart1_ch438q_counter;
			usart1_ch438q_counter = 0;
			ch438q_timeout = 255;
			
			/*
			for(n=0; n<usart1_ch438q_length; n++){
				ch438_reg_write(CH438_BASE_ADDR[ch438q_port]+CH438_REG_RBR_THR_DLL, usart1_ch438q_buffer[n]);
			}
			*/
			//ch438_reg_write(0x10, 0xaa);
		}else if(ch438q_timeout>0 && ch438q_timeout<200){
			ch438q_timeout--;		
		}
	
}


void ch438_stm32_gpio_input(void)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = CH438_Pin_D0 |CH438_Pin_D1 |CH438_Pin_D2 |
		CH438_Pin_D3 |CH438_Pin_D4 |CH438_Pin_D5 |CH438_Pin_D6 |CH438_Pin_D7;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( GPIOD, &gpio );
}

void ch438_stm32_gpio_output(void)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = CH438_Pin_D0 |CH438_Pin_D1 |CH438_Pin_D2 |
		CH438_Pin_D3 |CH438_Pin_D4 |CH438_Pin_D5 |CH438_Pin_D6 |CH438_Pin_D7;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( GPIOD, &gpio );	
}

uint8_t ch438_reg_read(uint8_t reg)
{
	uint8_t data=0;
	uint16_t P_data;
	GPIO_WriteBit(GPIOB, CH438_Pin_ALE, Bit_SET);
	GPIO_WriteBit(GPIOB, CH438_Pin_WR, Bit_SET);
	GPIO_WriteBit(GPIOB, CH438_Pin_RD, Bit_SET);
	
	GPIO_WriteBit(GPIOD, CH438_Pin_D0, (reg>>0)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D1, (reg>>1)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D2, (reg>>2)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D3, (reg>>3)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D4, (reg>>4)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D5, (reg>>5)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D6, (reg>>6)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D7, (reg>>7)&0x01);

	GPIO_WriteBit(GPIOB, CH438_Pin_ALE, Bit_RESET);
	
	ch438_stm32_gpio_input();
	GPIO_WriteBit(GPIOB, CH438_Pin_RD, Bit_RESET);
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D0)<<0;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D1)<<1;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D2)<<2;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D3)<<3;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D4)<<4;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D5)<<5;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D6)<<6;
	data += GPIO_ReadInputDataBit(GPIOD, CH438_Pin_D7)<<7;
	GPIO_WriteBit(GPIOB, CH438_Pin_RD, Bit_SET);
	ch438_stm32_gpio_output();
	return data;
}

void ch438_reg_write(uint8_t reg, uint8_t value)
{
	GPIO_WriteBit(GPIOB, CH438_Pin_ALE, Bit_SET);
	GPIO_WriteBit(GPIOB, CH438_Pin_WR, Bit_SET);
	GPIO_WriteBit(GPIOB, CH438_Pin_RD, Bit_SET);
	
	GPIO_WriteBit(GPIOD, CH438_Pin_D0, (reg>>0)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D1, (reg>>1)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D2, (reg>>2)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D3, (reg>>3)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D4, (reg>>4)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D5, (reg>>5)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D6, (reg>>6)&0x01);	
	GPIO_WriteBit(GPIOD, CH438_Pin_D7, (reg>>7)&0x01);

	GPIO_WriteBit(GPIOB, CH438_Pin_ALE, Bit_RESET);
	ch438_stm32_gpio_output();
	
	GPIO_WriteBit(GPIOD, CH438_Pin_D0, (value>>0)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D1, (value>>1)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D2, (value>>2)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D3, (value>>3)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D4, (value>>4)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D5, (value>>5)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D6, (value>>6)&0x01);
	GPIO_WriteBit(GPIOD, CH438_Pin_D7, (value>>7)&0x01);
	
	GPIO_WriteBit(GPIOB, CH438_Pin_WR, Bit_RESET);
	GPIO_WriteBit(GPIOB, CH438_Pin_WR, Bit_SET);
}

uint8_t ch438_ssr_get_port(void)
{
	u8 data=0;
	int i=0;
	data = ch438_reg_read(0x4F);
	for(i=0; i<8; i++){
		if(i<6){
			if((data>>i) & 1)return i;
		}else{
			return 0;
		}
	}
	return 0;
}



void ch438_clock(uint8_t serial_port, int baudrate)
{
	uint8_t time_code = 1843200/16/baudrate;
	////LCR---- DLAB BREAKEN PARMODE1 PARMODE0 PAREN STOPBIT WORDSZ1 WORDSZ0 
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_LCR, 0x83);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_RBR_THR_DLL, time_code);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_IER_DLM, 0x00);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_LCR, 0x03);
}

void ch438_interrupt(uint8_t serial_port)
{
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_IIR_FCR, 0xC1);//FIFO

	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_LCR, 0x03);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_IER_DLM, 0x01);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438_REG_MCR, 0x08);
}


//    (1)、初始化该IO口作为输入，一般为上拉输入或下拉输入；
//    (2)、开启IO口复用时钟，设置IO口与中断线的映射关系（_AFIO时钟）；
//    (3)、初始化线上中断，设置触发条件（RTIR上升沿，FTIR下降沿）；
//    (4)、配置中断分组（NVIC），并使能中断；
//    (5)、编写中断服务函数；
void stm32_pc6_interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
	//1、配置触发源GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能时钟
	GPIO_InitStructure.GPIO_Pin  = CH438_Pin_INT;//选择IO口   PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//使用结构体信息进行初始化IO口
	
	//2、使能AFIO复用时钟功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//3、将GPIO口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
	
	//4、中断线上进行中断初始化
	EXTI_InitStructure.EXTI_Line=EXTI_Line6; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
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



void systick_init(void)
{
	//1/10s=100ms
	//if (SysTick_Config((SystemCoreClock) / 1000)){
	if (SysTick_Config(72000)){
		while (1);
	}
	NVIC_SetPriority(SysTick_IRQn, 0x0);
}




void ch438_init(void)
{
	int i=0;
	
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能时钟

	gpio.GPIO_Pin = CH438_Pin_ALE | CH438_Pin_WR | CH438_Pin_RD | CH438_Pin_CS;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );
	
	GPIO_WriteBit(GPIOB, CH438_Pin_CS, Bit_RESET);
	GPIO_WriteBit(GPIOB, CH438_Pin_ALE, Bit_RESET);
	GPIO_WriteBit(GPIOB, CH438_Pin_WR, Bit_SET);
	GPIO_WriteBit(GPIOB, CH438_Pin_RD, Bit_SET);

	stm32_pc6_interrupt();
		
	for(i=0; i<8; i++){
		ch438_clock(i, 9600);
		ch438_interrupt(i);
	}
	
	ch438_stm32_gpio_output();
	
	systick_init();
	
	for(i=0; i<8; i++){
		ch438_reg_read(CH438_BASE_ADDR[i] + CH438_REG_RBR_THR_DLL);
	}
}















void vCh438Task( void *pvParameters )
{
	ch438_init();
	
	while(1){
		//if(!GPIO_ReadInputDataBit(GPIOA, CH438_Pin_INT)){
			//a = ch438_ssr_get_port();
			//ch438_reg_write(CH438_BASE_ADDR[a]+CH438_REG_RBR_THR_DLL, a);
			//ch438_reg_write(CH438_BASE_ADDR[a]+CH438_REG_RBR_THR_DLL, ch438_reg_read(CH438_BASE_ADDR[a] + CH438_REG_RBR_THR_DLL));
		//}
		
		//vTaskDelay(100);
	}
}
