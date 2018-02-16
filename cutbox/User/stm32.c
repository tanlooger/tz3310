#include "stm32.h"


void stm32_init(void)
{
	GPIO_InitTypeDef gpio;
    
    
    //SystemInit();
    
    
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//选择中断分组3
}






void gpio_init(void)
{
	GPIO_InitTypeDef gpio;
	
	/***********Cutbox ID******************************************/
	gpio.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);

	
	/***********Current check***************************************/
	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	
	/***********Led&Lamp******************************************/
	gpio.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
	
	
	/***********USART3**********************************************/
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&gpio);

	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&gpio);
	
	
	/***********Motor controller************************************/
	//M1 JP9 PA11-DIR PB5-STEP PA12-EN
	gpio.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	//M2 JP13 PA9-DIR PB6-STEP PA10-EN
	gpio.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_6;
	//gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);

	//M3 JP15 PC9-DIR PB7-STEP PA8-EN
	gpio.GPIO_Pin=GPIO_Pin_8;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_9;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	
	/***********Motor sensor************************************/
	//JP6  PC6--MOTOR1_HEAD PD15--MOTOR1_TAIL PD14--PD13
	gpio.GPIO_Pin=GPIO_Pin_6;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);

	//JP8
	gpio.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
	
	
	/***********Relay*******************************************/
	gpio.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP; //Because no pull-up resistor
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	gpio.GPIO_Pin=GPIO_Pin_8; //Dielectric loss release current
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);    
    
    
    
    LED(1);
    LAMP(1);
    RELAY_WRITE_0(0);
    RELAY_WRITE_1(0);
    RELAY_WRITE_2(0);
    RELAY_WRITE_3(0);
    RELAY_WRITE_4(0);
    RELAY_WRITE_5(0);
    //RELAY_WRITE_6(0);
    BOARD_GROUND(0);
    MOTOR1_EN(MOTOR_DISABLE);
    MOTOR2_EN(MOTOR_DISABLE);
    MOTOR3_EN(MOTOR_DISABLE);
}





void tim4_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 1000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //时钟预分频数 例如 ：时钟频率=72/(时钟预分频+1)  1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx
	//TIM_Cmd(TIM4, DISABLE);	
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
	//设置串口3
	USART_Init(USART3, &USART_InitStructure);
	//打开串口3的中断响应函数，接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	//打开串口3
	USART_Cmd(USART3, ENABLE);


	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择中断分组3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//选择串口3中断
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占式中断优先级设置为
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应式中断优先级设置为
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
	NVIC_Init(&NVIC_InitStructure);	
}

u8 get_relay_status(void)
{
	uint8_t status=0x00;
	
	status |= (RELAY_READ_0 << 0);
	status |= (RELAY_READ_1 << 1);
	status |= (RELAY_READ_2 << 2);
	status |= (RELAY_READ_3 << 3);
	status |= (RELAY_READ_4 << 4);
	status |= (RELAY_READ_5 << 5);
	//status |= (RELAY_READ_6 << 6);
	//status |= (DISCHARGE << 7);
	
	return status;
}


u8 get_motor_status(void)
{
	uint8_t status=0x00;
	
	status |= (MOTOR1_SENSOR_HEAD << 0);
	status |= (MOTOR1_SENSOR_TAIL << 1);
	status |= (MOTOR2_SENSOR_HEAD << 2);
	status |= (MOTOR2_SENSOR_TAIL << 3);
	status |= (MOTOR3_SENSOR_O << 4);
	status |= (MOTOR3_SENSOR_A << 5);
	status |= (MOTOR3_SENSOR_B << 6);
	status |= (MOTOR3_SENSOR_C << 7);
	
	return status;
}



void delay_ms(u16 time)
{
   u16 i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}


u8 checksum(u8 data[], u16 length)
{
	int sum=0;
	int i=0;
	for(i=0;i<length;i++){
		sum+=(int)data[i];
	}
	return sum%256; //return the last 8 bits;
}
