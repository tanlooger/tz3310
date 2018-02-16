#ifndef __USER_PROTOCOL_H__
#define __USER_PROTOCOL_H__
#include "stdint.h"


struct message{
	uint8_t source;
	uint8_t target;
	uint8_t type;
	uint16_t length;
	uint8_t data[64];
	uint8_t check;
};




uint8_t checksum(uint8_t data[], uint16_t length);


uint8_t check_relay_command(uint8_t relay_command);


uint8_t check_motor_command(uint8_t motor_command);

#endif




