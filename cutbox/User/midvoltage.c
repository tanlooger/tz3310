#include "stm32f10x.h"
#include "midvoltage.h"
#include "protocol.h"
#include "tim1motor.h"
#include "tim2motor.h"
#include "tim3motor.h"
#include "relay.h"



extern struct message usart3_msg;
void mid_voltage(void)
{
	//TIM_Cmd(TIM4, ENABLE);
	motor3(MOTOR3_O);
	set_relay(0x37);
	if(usart3_msg.source == 4){//兆欧表
		switch(usart3_msg.data[0]){
			case 0x01: //中压绕组对地
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_TAIL);
				//motor3(MOTOR3_A);
				break;
			case 0x08: //高压末屏A对地
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_A);
				break;
			case 0x09: //B
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_B);
				break;
			case 0x0A: //C
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_C);
				break;
			case 0x0B: //0
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_O);
				break;
			default:break;
		}
	}else if(usart3_msg.source == 5){//介损
		motor1(MOTOR_HEAD);
		motor2(MOTOR_TAIL);
		switch(usart3_msg.data[0]){
			case 0x01: //高压绕组对地
				break;
			case 0x07: //A
				motor3(MOTOR3_A);
				break;
			case 0x08: //中压末屏B对地
				motor3(MOTOR3_B);
				break;
			case 0x09: //C
				motor3(MOTOR3_C);
				break;
			case 0x0A: //0
				motor3(MOTOR3_O);
				break;
			default:break;
		}
	}else if(usart3_msg.source == 6){//直流电阻
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		switch(usart3_msg.data[0]){
			case 0x06:set_relay(0x3E);break; //中压侧Rab
			case 0x07:set_relay(0x3C);break; //中压侧Rbc
			case 0x08:set_relay(0x3A);break; //中压侧Rac
			case 0x09:set_relay(0x3B);break; //中压侧Rao
			case 0x0A:set_relay(0x3D);break; //中压侧Rbo
			case 0x0B:set_relay(0x39);break; //中压侧Rco
			default  :set_relay(0x37);break; //Shutdown all
		}
	}else if(usart3_msg.source == 7){//有载分接
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		if(1 == usart3_msg.data[0])set_relay(0x27);
	}else if(usart3_msg.source == 8){//变比(高电流,中低电压)
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		if(0 == usart3_msg.data[0])set_relay(0x27);
	}else if(usart3_msg.source == 9){//变参（电压、电流）
		set_relay(0x37);
		motor2(MOTOR_TAIL);		
		if(usart3_msg.data[0]==2){//短路
			motor1(MOTOR_HEAD);
		}else{ //空载
			motor1(MOTOR_TAIL);
			set_relay(0x07);
		}
		
	}
	
}