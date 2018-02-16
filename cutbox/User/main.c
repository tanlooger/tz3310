#include "stm32f10x.h"
#include "relay.h"
#include "tim3lamp.h"
#include "tim4motor.h"
#include "usart3.h"
#include "stm32.h"
#include "protocol.h"




uint8_t led_blink=0; //0灭 1慢闪 2快闪 3常亮





u8 motor_command = MOTOR_INIT_COMMAND;
u8 relay_command = RELAY_INIT_COMMAND;
u8 motor_status = 0;
u8 relay_status = 0;
u8 motor1_target_position=MOTOR_TAIL;
u8 motor2_target_position=MOTOR_TAIL;
u8 motor3_target_position=MOTOR3_O;

u8 command_flag = 1;

uint8_t usart3_flag=0;

u8 source_id;

uint8_t CUTBOX_ID = 0; //1 high voltage; 2 medium voltage; 3 low voltage
int main(void)
{
	int i=0;
    
    
    //volatile u8 a, b, c, d, e, f, g, h;
	
	stm32_init();
	gpio_init();
	tim3lamp_init();
	tim4_init();
	usart3_init();
	
    motor_stop(1);
    motor_stop(2);
    motor_stop(3);
	
	delay_ms(10);
    CUTBOX_ID = (CUTBOX_ID_1<<1)+CUTBOX_ID_2;
    //motor_run(1, MOTOR_HEAD, 9500);while(1);
    //motor_run(2, MOTOR_TAIL, 10000);while(1);
    //motor_run(3, MOTOR_HEAD, 3000);while(1);
   



	//一个周期83us
	while(1)
    {
        
        /*
        a = MOTOR1_SENSOR_HEAD;
        b = MOTOR1_SENSOR_TAIL;
        c = MOTOR2_SENSOR_HEAD;
        d = MOTOR2_SENSOR_TAIL;
        e = MOTOR3_SENSOR_O;
        f = MOTOR3_SENSOR_A;
        g = MOTOR3_SENSOR_B;
        h = MOTOR3_SENSOR_C;
        */
        
        
		led_lamp_blink();
        
        
		if(usart3_flag)
        {
            usart3_flag = 0;
            usart3_data_action();
        }
		
        if(command_flag)
        {
            motor_ctrl();
            motor_sensor();
        }
        
        

	}

}





void HardFault_Handler(void)
{
	while(1);
	__disable_fault_irq();   
  NVIC_SystemReset();
	return;
}

