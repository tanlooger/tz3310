#include "stm32f10x.h"
#include "lowvoltage.h"
#include "protocol.h"
#include "tim1motor.h"
#include "tim2motor.h"
#include "tim3motor.h"
#include "relay.h"


extern uint8_t motor_run;
extern struct message usart3_msg;
void low_voltage(void)
{
	//TIM_Cmd(TIM4, ENABLE);
	motor3(MOTOR3_O);
	set_relay(0x37);
	if(usart3_msg.source == 4){//兆欧表
		switch(usart3_msg.data[0]){
			case 0x02: //低压绕组对地
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_TAIL);
				break;
			case 0x0C: //低压末屏A对地
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_A);
				break;
			case 0x0D: //B
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_B);
				break;
			case 0x0E: //C
				motor1(MOTOR_HEAD);
			  motor2(MOTOR_HEAD);
				motor3(MOTOR3_C);
				break;
			case 0x0F: //0
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
			case 0x02: //低压绕组对地
				break;
			case 0x0B: //A
				motor3(MOTOR3_A);
				break;
			case 0x0C: //低压末屏B对地
				motor3(MOTOR3_B);
				break;
			case 0x0D: //C
				motor3(MOTOR3_C);
				break;
			case 0x0E: //0
				motor3(MOTOR3_O);
				break;
			default:break;
		}
	}else if(usart3_msg.source == 6){//直流电阻
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		switch(usart3_msg.data[0]){
			case 0x0C:set_relay(0x3E);break; //低压侧Rab
			case 0x0D:set_relay(0x3C);break; //低压侧Rbc
			case 0x0E:set_relay(0x3A);break; //低压侧Rac
			case 0x0F:set_relay(0x3B);break; //低压侧Rao
			case 0x10:set_relay(0x3D);break; //低压侧Rbo
			case 0x11:set_relay(0x39);break; //低压侧Rco
			default  :set_relay(0x37);break; //Shutdown all
		}
	}else if(usart3_msg.source == 7){//有载分接
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		if(2 == usart3_msg.data[0])set_relay(0x27);
	}else if(usart3_msg.source == 8){//变比(高电流,中低电压)
		set_relay(0x37);
		motor1(MOTOR_TAIL);
		motor2(MOTOR_TAIL);
		if(1 == usart3_msg.data[0])set_relay(0x27);
	}else if(usart3_msg.source == 9){//变参
		motor2(MOTOR_TAIL);
		set_relay(0x37);
		//set_relay(0x27);
		//set_relay(0x17);
		
		if(usart3_msg.data[0]==3){//短路
			motor1(MOTOR_HEAD);
		}else{//空载
			motor1(MOTOR_TAIL);
			set_relay(0x07);
		}
		
	}
	
}