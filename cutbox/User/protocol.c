#include "protocol.h"
extern uint8_t cutbox_id;
extern struct message usart3_msg;


uint8_t checksum(uint8_t data[], uint16_t length)
{
	int sum=0;
	int i=0;
	for(i=0;i<length;i++){
		sum+=(int)data[i];
	}
	return sum%256; //return the last 8 bits;
}

uint8_t check_relay_command(uint8_t relay_command)
{
	uint8_t i=0;
	uint8_t relay[10] = {0x37, 0x3C, 0x3A, 0x3E, 0x39, 0x3D, 0x3B, 0x27, 0x17, 0x07};
	for(i=0; i<10; i++){
		if(relay[i] == relay_command)return 1;
	}
	return 0;
}

/*
uint8_t check_motor_command(uint8_t motor_command)
{
	uint8_t a, b, c, d, e, f, g, h;
	a = (motor_command >> 0) & 0x01;
	b = (motor_command >> 1) & 0x01;
	c = (motor_command >> 2) & 0x01;
	d = (motor_command >> 3) & 0x01;
	e = (motor_command >> 4) & 0x01;
	f = (motor_command >> 5) & 0x01;
	g = (motor_command >> 6) & 0x01;
	h = (motor_command >> 7) & 0x01;

	if(a+b != 1)return 0;
	if(c+d != 1)return 0;
	if(e+f+g+h != 1)return 0;
	return 1;
}
*/


uint8_t check_motor_command(uint8_t motor_command)
{
	uint8_t i=0;
	uint8_t motor[16] = {
		0x1A, 0X2A,	0X4A,	0X8A,	0x16,	0x26,	0x46,	0x86,	0x19,	0x29,	0x49,	0x89,	0x15,	0x25,	0x45,	0x85
	};

	for(i=0; i<16; i++){
		if(motor[i] == motor_command)return 1;
	}
	return 0;
}

/*
void motor_timeout_error(uint8_t motor_id)
{
	uint8_t error_data[64];
	error_data[0] = cutbox_id;
	error_data[1] = usart3_msg.source;
	error_data[2] = 
}
*/

