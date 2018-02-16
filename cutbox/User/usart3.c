#include "usart3.h"
#include "stm32.h"
#include "relay.h"
#include "tim4motor.h"

extern uint8_t CUTBOX_ID;
extern u8 motor_command;
extern u8 relay_command;
extern u8 motor_status;
extern u8 relay_status;
extern u8 usart3_flag;
extern u8 source_id;

extern u8 motor1_target_position;
extern u8 motor2_target_position;
extern u8 motor3_target_position;


u8 feedback_data[8] = {0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00};
uint8_t usart3_buffer[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t usart3_length=0;
uint8_t usart3_counter=0;
void USART3_IRQHandler(void)
{
	__IO uint16_t Clear;
	if(USART_GetITStatus(USART3, USART_IT_IDLE)!= RESET){
		USART_ClearITPendingBit(USART3,USART_IT_IDLE); 
		Clear=USART3->SR;
		Clear=USART3->DR;

		usart3_length = usart3_counter;
		usart3_counter = 0;
		usart3_flag = 1;			
	

	}else if(USART_GetITStatus(USART3, USART_IT_RXNE)!= RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
		if(usart3_counter<64)
			usart3_buffer[usart3_counter++] = USART_ReceiveData(USART3);
	}
}



extern u8 command_flag;
void usart3_data_action(void)
{
    int i = 0;
    feedback_data[0] = CUTBOX_ID;
    if(2 == usart3_length)
    {
        if(usart3_buffer[0]==0xA1 && usart3_buffer[1]==0xA1 && CUTBOX_ID==1){
            USART_SendData(USART3, 0xA1);
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }else 
        if(usart3_buffer[0]==0xA2 && usart3_buffer[1]==0xA2 && CUTBOX_ID==2){
            USART_SendData(USART3, 0xA2);
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }else 
        if(usart3_buffer[0]==0xA3 && usart3_buffer[1]==0xA3 && CUTBOX_ID==3){
            USART_SendData(USART3, 0xA3);
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }
    }else
    if(6 == usart3_length && usart3_buffer[1]==CUTBOX_ID&&
        (usart3_buffer[0]+usart3_buffer[1]+usart3_buffer[2]+usart3_buffer[3]+usart3_buffer[4]==usart3_buffer[5])&&
        4 == usart3_buffer[2])
    {
        /*
        if(DISCHARGE)
        {
            feedback_data[5] = 0xDD;
            feedback_data[6] = 0xDD;
        }else
        */
        feedback_data[1] = source_id;
        
        if(command_flag)
        {
            feedback_data[5] = 0xFF;
            feedback_data[6] = 0xFF;
        }
        else
        {
            feedback_data[5] = get_motor_status();
            feedback_data[6] = get_relay_status();
        }
        
        feedback_data[7] = checksum(feedback_data, 7);
        for(i=0; i<8; i++)
        {
            USART_SendData(USART3, feedback_data[i]);
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }   
    }
    else
    if(8 == usart3_length && usart3_buffer[1] == CUTBOX_ID &&
        (checksum(usart3_buffer, 7) == usart3_buffer[7]) &&
        1 == usart3_buffer[2])
    {  
        for(i=0; i<8; i++)
        {
            USART_SendData(USART3, usart3_buffer[i]);
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }
        initial_vars();
        source_id = usart3_buffer[0];
        motor_command = usart3_buffer[5];
        relay_command = usart3_buffer[6];
        motor_status = get_motor_status();
        relay_status = get_relay_status();
        motor_target_position(motor_command);
        //set_relay(0x00);
        if(usart3_buffer[0]==4 || usart3_buffer[0]==5)
            BOARD_GROUND(1);
        else 
            BOARD_GROUND(0);
        command_flag = 1;
    }
    usart3_buffer[0]=0x00;
    usart3_buffer[1]=0x00;
    usart3_buffer[2]=0x00;
    usart3_buffer[3]=0x00;
    usart3_buffer[4]=0x00;
    usart3_buffer[5]=0x00;
    usart3_buffer[6]=0x00;
    usart3_buffer[7]=0x00;
    usart3_buffer[8]=0x00;
    usart3_buffer[9]=0x00;
}



void motor_target_position(u8 command)
{
    motor1_target_position=2;
    motor2_target_position=2;
    motor3_target_position=2;
    if(((command>>0)&0x01))motor1_target_position=MOTOR_HEAD;
    if(((command>>1)&0x01))motor1_target_position=MOTOR_TAIL;
    if(((command>>2)&0x01))motor2_target_position=MOTOR_HEAD;
    if(((command>>3)&0x01))motor2_target_position=MOTOR_TAIL;
    if(((command>>4)&0x01))motor3_target_position=MOTOR3_O;
    if(((command>>5)&0x01))motor3_target_position=MOTOR3_A;
    if(((command>>6)&0x01))motor3_target_position=MOTOR3_B;
    if(((command>>7)&0x01))motor3_target_position=MOTOR3_C;
    
}
