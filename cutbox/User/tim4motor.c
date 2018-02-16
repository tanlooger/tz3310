#include "tim4motor.h"
#include "stm32.h"
#include "relay.h"




extern u8 motor_status;
extern u8 relay_status;
extern u8 relay_command;
extern u8 motor1_target_position;
extern u8 motor2_target_position;
extern u8 motor3_target_position;
extern u8 command_flag;

u8 motor1_current_position=3;
u8 motor2_current_position=3;
u8 motor3_current_position=0;
u8 motor1_run_flag = STOPPING; //STOPPING; RUNNING
u8 motor2_run_flag = STOPPING; //STOPPING; RUNNING
u8 motor3_run_flag = STOPPING; //STOPPING; RUNNING




u8 motor_running_flag = 0;

u8 motor3_action_flag = 0;

u8 motor_sensor_detected = 0;

u8 motor3_adjust_flag = 0;

u16 motor3_adjust_counter = 0;

u32 motor_steps = 0;

u16 motor_pin = 0;




void initial_vars(void)
{
    motor_stop(1);
    motor_stop(2);
    motor_stop(3);
    command_flag = 0;
    motor_running_flag = 0;
    motor3_action_flag = 0;
    motor_sensor_detected = 0;
    motor3_adjust_flag = 0;
    motor3_adjust_counter = 0;
    motor_steps = 0;
    motor_pin = 0;    
}




void motor_ctrl(void)
{
    int a = 0;
    if(motor_running_flag)return;
    delay_ms(10);
    motor_status = get_motor_status();
    relay_status = get_relay_status();
    motor_current_position(motor_status);
    
    if((MOTOR1_SENSOR_HEAD&&MOTOR1_SENSOR_TAIL) || (MOTOR2_SENSOR_HEAD&&MOTOR2_SENSOR_TAIL) ||
        (MOTOR3_SENSOR_O+MOTOR3_SENSOR_A+MOTOR3_SENSOR_B+MOTOR3_SENSOR_C)>1)
    {
        initial_vars();
    }else
    
    if(motor1_target_position != motor1_current_position)
    {
        motor_run(1, motor1_target_position, MOTOR1_TIMOUT_STEPS);
    }else
   
    if(motor2_target_position != motor2_current_position)
    {
        motor_run(2, motor2_target_position, MOTOR2_TIMOUT_STEPS);
    }else
    
    if(motor3_target_position != motor3_current_position)
    {
        if(0 == motor3_action_flag)
        {
            if(!motor3_current_position)
            {
                motor3_action_flag = MOTOR3_FOUND_ACTION;
                motor_run(3, MOTOR_TAIL, MOTOR3_TIMOUT_STEPS);
            }
            else
            {
                motor3_action_flag = MOTOR3_TARGET_TO_ACTION;
                a = MOTOR3_TIMOUT_STEPS*__fabs(motor3_target_position-motor3_current_position);
                if(motor3_target_position > motor3_current_position)
                    motor_run(3, MOTOR_TAIL, a);
                else
                    motor_run(3, MOTOR_HEAD, a);
            }
        }else
        
        if(MOTOR3_FOUND_ACTION==motor3_action_flag)
        {
            motor3_action_flag = MOTOR3_FOUND_TURN_ACTION;
            motor_run(3, MOTOR_HEAD, MOTOR3_TIMOUT_STEPS);
        }
    }else
    if(relay_status != relay_command)
    {
        set_relay(relay_command);
        initial_vars();
    }
    else
    {
        initial_vars();
    }
    
    
}



void motor_sensor(void)
{
    int a = 0;
    motor_status = get_motor_status();
    motor_current_position(motor_status);
    if(RUNNING==motor1_run_flag && !motor_sensor_detected)
    {
        if(motor1_current_position == motor1_target_position)
        {
            delay_ms(10);
            motor_status = get_motor_status();
            motor_current_position(motor_status);
            if(motor1_current_position == motor1_target_position)
            {
                motor_sensor_detected = 1;
                motor_steps = MOTOR_ADD_STEPS;
            }
        }else
        if(!motor_steps)
        {
            initial_vars();
        }
    }
    
    if(RUNNING==motor2_run_flag && !motor_sensor_detected)
    {
        if(motor2_current_position == motor2_target_position)
        {
            delay_ms(10);
            motor_status = get_motor_status();
            motor_current_position(motor_status);
            if(motor2_current_position == motor2_target_position)
            {
                motor_sensor_detected = 1;
                motor_steps = MOTOR_ADD_STEPS;
            }
        }else
        if(!motor_steps)
        {
            initial_vars();
        }
    }
    
    if(RUNNING==motor3_run_flag && MOTOR3_TARGET_END_ACTION!=motor3_action_flag)
    {
        if(motor3_current_position==motor3_target_position && MOTOR3_TARGET_BEGIN_ACTION!=motor3_action_flag)
        {
            delay_ms(10);
            motor_status = get_motor_status();
            motor_current_position(motor_status);
            if(motor3_current_position==motor3_target_position)
            {
                motor3_action_flag = MOTOR3_TARGET_BEGIN_ACTION;
                motor3_adjust_counter = 0;
                motor3_adjust_flag = 1;
            }
        }else
        if(!motor3_current_position && MOTOR3_TARGET_BEGIN_ACTION==motor3_action_flag)
        {
            delay_ms(10);
            motor_status = get_motor_status();
            motor_current_position(motor_status);
            if(!motor3_current_position)
            {
                motor3_action_flag = MOTOR3_TARGET_END_ACTION;
                motor_run(3, !MOTOR3_DIR_IN(), motor3_adjust_counter/2);
            }
        }else
            
        if(motor3_current_position && (MOTOR3_FOUND_ACTION==motor3_action_flag || MOTOR3_FOUND_TURN_ACTION==motor3_action_flag))
        {
            delay_ms(10);
            motor_status = get_motor_status();
            motor_current_position(motor_status);
            motor3_action_flag = MOTOR3_TARGET_TO_ACTION;
            if(motor3_current_position)
            {
                a = MOTOR3_TIMOUT_STEPS*__fabs(motor3_target_position-motor3_current_position);
                if(motor3_target_position > motor3_current_position)
                    motor_run(3, MOTOR_TAIL, a);
                else
                    motor_run(3, MOTOR_HEAD, a);
            }
        }else
        
        if(motor3_action_flag > MOTOR3_FOUND_TURN_ACTION && !motor_steps)
        {
            initial_vars();
        }
    }
}







u8 pulse = 0;
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        if(motor_steps)
        {
            pulse = !pulse;
            MOTOR_PULSE(motor_pin, pulse);
            motor_steps--;
        }
        else if(RUNNING == motor1_run_flag)motor_stop(1);
        else if(RUNNING == motor2_run_flag)motor_stop(2);
        else if(RUNNING == motor3_run_flag)motor_stop(3);
        
        
        if(motor3_adjust_flag)motor3_adjust_counter++;
	}
}



void motor_run(u8 motor_id, u8 direction, u32 steps)
{
    motor_steps = steps;
    motor_sensor_detected = 0;
    motor_running_flag = 1;
    switch(motor_id)
    {
        case 1:
            motor1_run_flag = RUNNING;
            motor_pin=GPIO_Pin_5;
            MOTOR1_DIR_OUT(direction);
            MOTOR1_EN(MOTOR_ENABLE);
            break;
        case 2:
            motor2_run_flag = RUNNING;
            motor_pin=GPIO_Pin_6;
            MOTOR2_DIR_OUT(direction);
            MOTOR2_EN(MOTOR_ENABLE);
            break;
        case 3:
            motor3_run_flag = RUNNING;
            motor_pin=GPIO_Pin_7;
            MOTOR3_DIR_OUT(direction);
            MOTOR3_EN(MOTOR_ENABLE);
            break;
    }
}


void motor_stop(u8 motor_id)
{
    motor_running_flag = 0;
    switch(motor_id)
    {
        case 1:
            motor1_run_flag = STOPPING;
            MOTOR1_EN(MOTOR_DISABLE);
            break;
        case 2:
            motor2_run_flag = STOPPING;
            MOTOR2_EN(MOTOR_DISABLE);
            break;
        case 3:
            motor3_run_flag = STOPPING;
            MOTOR3_EN(MOTOR_DISABLE);
            break;
    }
}


void motor_current_position(u8 status)
{
    motor1_current_position=3;
    motor2_current_position=3;
    motor3_current_position=0;
    if(((status>>0)&0x01))motor1_current_position=MOTOR_HEAD;
    if(((status>>1)&0x01))motor1_current_position=MOTOR_TAIL;
    if(((status>>2)&0x01))motor2_current_position=MOTOR_HEAD;
    if(((status>>3)&0x01))motor2_current_position=MOTOR_TAIL;
    if(((status>>4)&0x01))motor3_current_position=MOTOR3_O;
    if(((status>>5)&0x01))motor3_current_position=MOTOR3_A;
    if(((status>>6)&0x01))motor3_current_position=MOTOR3_B;
    if(((status>>7)&0x01))motor3_current_position=MOTOR3_C;    
}