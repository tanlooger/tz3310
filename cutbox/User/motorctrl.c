#include "motorctrl.h"
#include "protocol.h"

void motor_init(void)
{
	GPIO_InitTypeDef gpio;

/***********Motor controller************************************/
	//M1 JP9   PA10-DIR  PA11-STEP  PA12-EN
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &gpio );
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, MOTOR_DISABLE);//Disable
	
	//M2 JP13   PC9-DIR  PA8-STEP  PA9-EN
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &gpio );
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	GPIO_WriteBit(GPIOA, GPIO_Pin_9, MOTOR_DISABLE);//Disable
	
	//M3 JP15   PC6-DIR  PC7-STEP  PC8-EN
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	GPIO_WriteBit(GPIOC, GPIO_Pin_8, MOTOR_DISABLE);//Disable

	/*
	//M4 JP15   PD13-DIR  PD14-STEP  PD15-EN
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpio);
	GPIO_WriteBit(GPIOD, GPIO_Pin_15, 1);//Disable
	*/
	
	
/***********Motor sensor************************************/
	//JP4
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOD, &gpio );
	
	//JP6   PE13--PE14      PE15--PB10
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio );
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpio);

	//JP8   PE9--PE10       PE11--PE12
	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOE, &gpio );
}



extern uint8_t usart3_buffer[64];
extern struct message usart3_msg;
extern uint8_t usart3_length;
void motor_error(void)
{
	uint8_t i=0;
	usart3_buffer[0] = usart3_msg.target;
	usart3_buffer[1] = usart3_msg.source;
	usart3_buffer[2] = 0x07; 
	usart3_buffer[3] = 0x00;
	usart3_buffer[4] = 0x00;
	usart3_buffer[5] = checksum(usart3_buffer, 5);

	for(i=0; i<6; i++){
		USART_SendData(USART3, usart3_buffer[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}


uint8_t get_motor_status(void)
{
	uint8_t motor_status=0x00;
	uint8_t pe13 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);
	uint8_t pe14 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);
	uint8_t pe15 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15);
	uint8_t pb10 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
	
	uint8_t pe9 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);		
	uint8_t pe10 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10);
	uint8_t pe11 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11);
	uint8_t pe12 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);	

	motor_status |= (pe13 << 0);
	motor_status |= (pe14 << 1);
	motor_status |= (pe15 << 2);
	motor_status |= (pb10 << 3);
	motor_status |= (pe9 << 4);
	motor_status |= (pe10 << 5);
	motor_status |= (pe11 << 6);
	motor_status |= (pe12 << 7);
	
	return motor_status;
}




void delay(int delay_time)
{
		int i,j;
		for(i=0;i<delay_time;i++)
			for(j=0;j<0x500;j++)	__NOP();
}

