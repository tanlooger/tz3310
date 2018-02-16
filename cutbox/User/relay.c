#include "relay.h"
#include "stm32.h"

//extern u8 relay_manipulate_flag;

void set_relay(uint8_t value)
{
	RELAY_WRITE_0((value>>0)&0x01);
	delay_ms(20);
	RELAY_WRITE_1((value>>1)&0x01);
	delay_ms(20);
	RELAY_WRITE_2((value>>2)&0x01);
	delay_ms(20);
	RELAY_WRITE_3((value>>3)&0x01);
	delay_ms(20);
	RELAY_WRITE_4((value>>4)&0x01);
	delay_ms(20);
	RELAY_WRITE_5((value>>5)&0x01);
	delay_ms(20);
    //value ? BOARD_GROUND(0) : BOARD_GROUND(1);
    //BOARD_GROUND(!value);
}








