#ifndef __USER_CUTBOX_H__
#define __USER_CUTBOX_H__
#include "stm32f10x.h"



extern u8 cutbox_motor_high;
extern u8 cutbox_motor_mid;
extern u8 cutbox_motor_low;
extern u8 cutbox_relay_high;
extern u8 cutbox_relay_mid;
extern u8 cutbox_relay_low;

extern u8 cutbox_cutting_id;
extern u8 cutbox_device_id;

extern u8 cutbox_check_delay;

extern u8 cutbox_motor_result;
extern u8 cutbox_relay_result;


extern u8 cutbox_command_flag;
extern u8 cutbox_send_flag;
extern u8 cutbox_check_flag;


extern u8 cutbox_must_online;

#define CUTBOX_HIGH_MUST_ONLINE_GET  ((cutbox_must_online>>0)&0x01)
#define CUTBOX_MID_MUST_ONLINE_GET   ((cutbox_must_online>>1)&0x01)
#define CUTBOX_LOW_MUST_ONLINE_GET   ((cutbox_must_online>>2)&0x01)
#define CUTBOX_SLAVE_MUST_ONLINE_GET ((cutbox_must_online>>5)&0x01)

#define CUTBOX_HIGH_MUST_ONLINE_SET  (cutbox_must_online |= (0x01<<0))
#define CUTBOX_MID_MUST_ONLINE_SET   (cutbox_must_online |= (0x01<<1))
#define CUTBOX_LOW_MUST_ONLINE_SET   (cutbox_must_online |= (0x01<<2))
#define CUTBOX_SLAVE_MUST_ONLINE_SET (cutbox_must_online |= (0x01<<5))



void cutbox_command(uint8_t target, uint8_t command);


void command2cutbox(u8 device_id, u8 cutbox_id, u8 motor_value, u8 relay_value);


void check_cutbox(u8 device_id, u8 cutbox_id);


void send_cutbox_msg_to_PC(uint8_t cutbox_target, uint8_t motor_status, uint8_t relay_status);


void cutbox_reset_params(void);


#endif
