#include "ch438q.h"
#include "stm32f10x_gpio.h"
#include "stm32.h"

extern const uint16_t CH438_BASE_ADDR[8];


//char ch438_bool;
//u8 ch438q_timeout = 255;
/*
void EXTI9_5_IRQHandler(void)
{
	uint8_t port,d_bak;
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
			EXTI_ClearITPendingBit(EXTI_Line6);
			port = ch438_ssr_get_port();
			d_bak=ch438_reg_read(CH438_BASE_ADDR[port] + CH438_REG_RBR_THR_DLL);
			if(!ch438q_counter)
					ch438q_port=port;
		if(port)
		{
			ch438_bool=1;
			ch438q_timeout = 50;
			ch438q_buffer[ch438q_counter++] = d_bak;//ch438_reg_read(CH438_BASE_ADDR[port] + CH438_REG_RBR_THR_DLL);
		}
		return;
	}
}
*/


const uint16_t CH438_BASE_ADDR[8] = {
	0x00, 0x10, 0x20, 0x30, 0x08, 0x18, 0x28, 0x38
};



u8 ch438q_buffer[256];
u8 ch438q_flag;
u8 ch438q_length;
u8 ch438q_counter;

void EXTI9_5_IRQHandler(void)
{
	uint8_t port,d_bak,IIRstatus;
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		port = ch438_ssr_get_port();
        IIRstatus = CH438Q_GetITStatus(port);
        
        //if(port) ch438q_timeout = 100;
        
        if(IIRstatus == CH438Q_IT_IDLE)
        {
            while(ch438_reg_read(CH438_BASE_ADDR[port] + CH438Q_LSR_OFFSET) & 0x01)
            {
                d_bak = ch438_reg_read(CH438_BASE_ADDR[port] + CH438Q_RBR_OFFSET);
                //ch438_put_char(port, d_bak);
                ch438q_buffer[ch438q_counter++] = d_bak;
            }
            ch438q_length = ch438q_counter;
            ch438q_counter = 0;
            ch438q_flag = 1;
        }else
        if(IIRstatus == CH438Q_IT_RXNE)
        {
            d_bak = ch438_reg_read(CH438_BASE_ADDR[port] + CH438Q_RBR_OFFSET);
            ch438q_buffer[ch438q_counter++] = d_bak;
        }
	}
}




/*
void SysTick_Handler(void)
{
		//ch438_reg_write(0x10, 0xaa);
		if(0 == ch438q_timeout)
        {
//			u8 n=0;
			ch438q_flag = 1;
			ch438_bool=0x00;
			ch438q_length = ch438q_counter;
			ch438q_counter = 0;
			ch438q_timeout = 255;
			
			
			//for(n=0; n<usart1_ch438q_length; n++){
			//	ch438_reg_write(CH438_BASE_ADDR[ch438q_port]+CH438_REG_RBR_THR_DLL, usart1_ch438q_buffer[n]);
			//}
			
			//ch438_reg_write(0x10, 0xaa);
		}else if(ch438q_timeout>0 && ch438q_timeout<200){
			ch438q_timeout--;		
		}
	
}
*/




void ch438_put_char(u8 port, u8 value)
{
    ch438_reg_write(CH438_BASE_ADDR[port]+CH438Q_THR_OFFSET, value);
    delay_ms(1);
}

void ch438_stm32_gpio_input(void)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                    GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( GPIOD, &gpio );
}

void ch438_stm32_gpio_output(void)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                    GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( GPIOD, &gpio );	
}

uint8_t ch438_reg_read(uint8_t reg)
{
	uint8_t data=0;
    
    CH438Q_ALE_SET(Bit_SET);
	CH438Q_WR_SET(Bit_SET);
    CH438Q_RD_SET(Bit_SET);
	
    CH438Q_D_SET(reg);

    CH438Q_ALE_SET(Bit_RESET);
	ch438_stm32_gpio_input();
    CH438Q_RD_SET(Bit_RESET);
	data=CH438Q_D_GET();

    CH438Q_RD_SET(Bit_SET);
	ch438_stm32_gpio_output();
	return data;
}

void ch438_reg_write(uint8_t reg, uint8_t value)
{
    CH438Q_ALE_SET(Bit_SET);
	CH438Q_WR_SET(Bit_SET);
    CH438Q_RD_SET(Bit_SET);
    CH438Q_D_SET(reg);
    CH438Q_ALE_SET(Bit_RESET);
	ch438_stm32_gpio_output();

    CH438Q_D_SET(value);
	CH438Q_WR_SET(Bit_RESET);
	CH438Q_WR_SET(Bit_SET);
}

uint8_t ch438_ssr_get_port(void)
{
	u8 data=0;
	int i=0;
	data = ch438_reg_read(0x4F);
	for(i=0; i<8; i++)
		if((data>>i) & 1)return i;
	return i;
}



void ch438_clock(uint8_t serial_port, int baudrate)
{
	uint8_t time_code = 1843200/16/baudrate;
	//LCR---- DLAB BREAKEN PARMODE1 PARMODE0 PAREN STOPBIT WORDSZ1 WORDSZ0 
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_LCR_OFFSET, 0x83);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_DLL_OFFSET, time_code);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_DLM_OFFSET, 0x00);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_LCR_OFFSET, 0x03);
    
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_FCR_OFFSET, 0xC1);//FIFO

	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_IER_OFFSET, 0x01);
	ch438_reg_write(CH438_BASE_ADDR[serial_port] + CH438Q_MCR_OFFSET, 0x08);
}




void stm32_pc6_interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
	//1、配置触发源GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//选择IO口   PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//使用结构体信息进行初始化IO口
	
	//2、使能AFIO复用时钟功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//3、将GPIO口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
	
	//4、中断线上进行中断初始化
	EXTI_InitStructure.EXTI_Line=EXTI_Line6; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
	
	
	//5、中断优先级配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //使能外部中断所在的通道
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级 2， 
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //子优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
	NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化 
}




void ch438_init(void)
{	
	GPIO_InitTypeDef gpio;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能时钟

	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );
	
    CH438Q_CS_SET(Bit_RESET);

	stm32_pc6_interrupt();
	
	ch438_clock(0, 9600);
	ch438_clock(1, 9600);
	ch438_clock(2, 9600);
	ch438_clock(3, 9600);
	ch438_clock(4, 115200);
	ch438_clock(5, 9600);
	ch438_clock(6, 9600);
	ch438_clock(7, 9600);
	
	ch438_stm32_gpio_output();
}


