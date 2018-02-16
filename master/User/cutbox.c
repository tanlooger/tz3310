#include "cutbox.h"
#include "stm32.h"

u8 cutbox_motor_high;
u8 cutbox_motor_mid;
u8 cutbox_motor_low;
u8 cutbox_relay_high;
u8 cutbox_relay_mid;
u8 cutbox_relay_low;

u8 cutbox_cutting_id;
u8 cutbox_device_id;

u8 cutbox_check_delay;

u8 cutbox_must_online;
u8 cutbox_command_flag;
u8 cutbox_send_flag;
u8 cutbox_check_flag;

u8 cutbox_motor_result;
u8 cutbox_relay_result;

void cutbox_command(uint8_t target, uint8_t command)
{
    cutbox_cutting_id = 0;
    cutbox_device_id = target;
    cutbox_command_flag = 1;
    cutbox_send_flag = 0;
    cutbox_check_flag = 0;
    
    cutbox_motor_result = 0xFF;
    cutbox_relay_result = 0xFF;
    
    cutbox_must_online = 0;
    
    
	if(4 == target)//兆欧表
    {
        cutbox_must_online = 0;
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x40;
        cutbox_relay_mid = 0x40;
        cutbox_relay_low = 0x40;
        
		switch(command)
        {
            case 0x10://(高+中)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_MID_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_mid = 0x19;
                break;
            case 0x11://(中+低)对地
                CUTBOX_MID_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_mid = 0x19;
                cutbox_motor_low = 0x19;
                break;
            case 0x12://(高+中+低)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_MID_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_mid = 0x19;
                cutbox_motor_low = 0x19;
                break;
            case 0x13://(高+低)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_low = 0x19;
                break;
            
			case 0x00://高压绕组对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x19;
				break;
			case 0x07://高压末屏O对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x16;
				break;
			case 0x04://高压末屏A对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x26;
				break;
			case 0x05://高压末屏B对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x46;
				break;
			case 0x06://高压末屏C对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x86;
				break;

			
			case 0x01://中压绕组对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x19;
				break;	
			case 0x0B://中压末屏O对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x16;
				break;            
			case 0x08://中压末屏A对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x26;
				break;				
			case 0x09://中压末屏B对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x46;
				break;				
			case 0x0A://中压末屏C对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x86;
				break;				

			
			case 0x02://低压绕组对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x19;
				break;		
			case 0x0F://低压末屏O对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x16;
				break;            
			case 0x0C://低压末屏A对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x26;
				break;				
			case 0x0D://低压末屏B对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x46;
				break;				
			case 0x0E://低压末屏C对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x86;
				break;				
		}
	}else 
    if(5 == target)//介损
    { 
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x40;
        cutbox_relay_mid = 0x40;
        cutbox_relay_low = 0x40;
		switch(command){
            case 0x0F://(高+中)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_MID_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_mid = 0x19;
                break;
            case 0x10://(中+低)对地
                CUTBOX_MID_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_mid = 0x19;
                cutbox_motor_low = 0x19;
                break;
            case 0x11://(高+中+低)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_MID_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_mid = 0x19;
                cutbox_motor_low = 0x19;
                break;
            case 0x12://(高+低)对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
                CUTBOX_LOW_MUST_ONLINE_SET;
                cutbox_motor_high = 0x19;
                cutbox_motor_low = 0x19;
                break;
            
			case 0x00://高压绕组对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x19;
				break;
			case 0x06://高压末屏O对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x19;		
				break;
			case 0x03://高压末屏A对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x29;
				break;				
			case 0x04://高压末屏B对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x49;
				break;				
			case 0x05://高压末屏C对地
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_motor_high = 0x89;
				break;				
			
			case 0x01://中压绕组对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x19;
				break;	
			case 0x0A://中压末屏O对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x19;			
				break;			
			case 0x07://中压末屏A对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x29;
				break;				
			case 0x08://中压末屏B对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x49;		
				break;				
			case 0x09://中压末屏C对地
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_motor_mid = 0x89;			
				break;				
			
			case 0x02://低压绕组对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x19;
				break;
			case 0x0E://低压末屏O对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x19;
				break;
			case 0x0B://低压末屏A对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x29;
				break;
			case 0x0C://低压末屏B对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x49;
				break;
			case 0x0D://低压末屏C对地
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_motor_low = 0x89;
				break;
		}
	}else 
    if(6 == target){ //直流电阻
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x00;
        cutbox_relay_mid = 0x00;
        cutbox_relay_low = 0x00;
		switch(command){
			case 0x00: //高压侧Rab
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x0E;
				break;
			case 0x01://BC
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x0C;
				break;
			case 0x02: //AC
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x0A;
				break;
			case 0x03:
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x0B;
				break;
			case 0x04:
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x0D;
				break;
			case 0x05:
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x09;
				break;
			
			case 0x06: //中压侧Rab
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x0E;
				break;
			case 0x07:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x0C;
				break;
			case 0x08:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x0A;
				break;
			case 0x09:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x0B;
				break;
			case 0x0A:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x0D;
				break;
			case 0x0B:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x09;
				break;
			
			
			case 0x0C:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x0E;
				break;
			case 0x0D:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x0C;
				break;
			case 0x0E:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x0A;
				break;
			case 0x0F:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x0B;
				break;
			case 0x10:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x0D;
				break;
			case 0x11:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x09;
				break;
		}
	}else 
    if(7 == target)//有载分接 (只电压)
    { 
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x00;
        cutbox_relay_mid = 0x00;
        cutbox_relay_low = 0x00;
		switch(command){
			case 0x00:
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x10;
				break;
			case 0x01:
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x10;
				break;
			case 0x02:
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x10;
				break;
		}
	}else 
    if(8 == target)//变比
    { 
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x00;
        cutbox_relay_mid = 0x00;
        cutbox_relay_low = 0x00;
		if(command == 0){ //高(电流)对中(电压)
            CUTBOX_HIGH_MUST_ONLINE_SET;
            CUTBOX_MID_MUST_ONLINE_SET;
			cutbox_relay_high = 0x20;
            cutbox_relay_mid = 0x10;
		}else if(command == 1){ //高(电流)对低(电压)
            CUTBOX_HIGH_MUST_ONLINE_SET;
            CUTBOX_LOW_MUST_ONLINE_SET;
			cutbox_relay_high = 0x20;
            cutbox_relay_low = 0x10;
		}else if(command == 2){ //中(电流)对低(电压)
            CUTBOX_MID_MUST_ONLINE_SET;
            CUTBOX_LOW_MUST_ONLINE_SET;
			cutbox_relay_mid = 0x20;
            cutbox_relay_low = 0x10;	
		}
	}else 
    if(9 == target)//变参
    {
        cutbox_motor_high = 0x1A;
        cutbox_motor_mid = 0x1A;
        cutbox_motor_low = 0x1A;
        cutbox_relay_high = 0x00;
        cutbox_relay_mid = 0x00;
        cutbox_relay_low = 0x00;
		switch(command)
        {
			case 0: //高对中空载(高浮空，中电压流)
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x30;
				break;
			case 1: //高/中对低空载（高浮空，低电压流）
                CUTBOX_LOW_MUST_ONLINE_SET;
				cutbox_relay_low = 0x30;
				break;
			case 2: //高对中短路（高电流电压，中短路）
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x30;
				break;
			case 3: //高对低短路（高电流电压，低短路）
                CUTBOX_HIGH_MUST_ONLINE_SET;
				cutbox_relay_high = 0x30;
				break;
			case 4: //中对低短路(高浮空，中电压流，低短路)
                CUTBOX_MID_MUST_ONLINE_SET;
				cutbox_relay_mid = 0x30;
				break;
		}
	}
}




void command2cutbox(u8 device_id, u8 cutbox_id, u8 motor_value, u8 relay_value)
{
    u8 i;
    u8 command[8] = {0, 0, 1, 0, 2, 0, 0, 0};
    command[0] = device_id;
    command[1] = cutbox_id;
    command[5] = motor_value;
    command[6] = relay_value;
    command[7] = checksum(command, 7);
    for(i=0; i<8; i++)
	{
        USART_SendData(USART1, command[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
    cutbox_check_delay = 1;
}



void send_to_PC(uint8_t data[], uint8_t length)
{
	uint8_t i=0;
	int sum=0;
	for(i=0; i<length; i++)
	{
			sum += (int)data[i];
			USART_SendData(USART2, data[i]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	USART_SendData(USART2, (uint8_t)(sum%256));
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void check_cutbox(u8 device_id, u8 cutbox_id)
{
    u8 i;
	u8 check_data[6];
	check_data[0] = device_id;
	check_data[1] = cutbox_id;
	check_data[2] = 0x04;
	check_data[3] = 0x00;
	check_data[4] = 0x00;
    check_data[5] = checksum(check_data, 4);
    for(i=0; i<6; i++)
	{
        USART_SendData(USART1, check_data[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
    cutbox_check_delay = 1;
}


void send_cutbox_msg_to_PC(uint8_t cutbox_target, uint8_t motor_status, uint8_t relay_status)
{
	uint8_t check_data[7];
	check_data[0] = cutbox_target;
	check_data[1] = 0x00;
	check_data[2] = 0x07;
	check_data[3] = 0x00;
	check_data[4] = 0x02;
	check_data[5] = motor_status;	
	check_data[6] = relay_status;
	
	send_to_PC(check_data, 7);
}



void cutbox_reset_params(void)
{
    cutbox_cutting_id = 0;
    cutbox_device_id = 0;

    cutbox_check_delay = 0; 
    cutbox_command_flag = 0;
    cutbox_send_flag = 0;
    cutbox_check_flag = 0;
}


