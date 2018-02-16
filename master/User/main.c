#include "stm32f10x.h"
#include "usart2.h"
#include "usart1.h"
#include "ch438q.h"
#include "protocol.h"
#include "cutbox.h"
#include "tim3motor.h"
#include "relay.h"
#include "stm32.h"
#include "stm32f10x_rcc.h" 




//uint8_t device_run=0;

uint8_t usart1_flag;


uint8_t usart1_ch438q_buffer[1024];
uint8_t usart1_ch438q_buffer2[1024];
uint8_t ch438q2PC[1024];
const uint8_t ch438q_buffer2[4]={0xff,0xff,0xff,0xff};
uint8_t usart1_cutbox[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


uint16_t usart1_ch438q_length;
uint16_t usart1_ch438q_length2;
uint16_t usart1_ch438q_counter;

uint8_t ch438q_timeout = 255;
uint8_t ch438q_port = 1;
uint8_t ch438q_flag;

uint8_t usart2_buffer[256];
uint8_t usart2_buffer_bak[256];
uint8_t usart2_flag;
uint8_t usart2_length;
uint8_t usart2_length_bak;
uint8_t usart2_counter;

uint8_t Power_State=0;

struct message usart2_msg;


const uint16_t CH438_BASE_ADDR[8] = {
	0x00, 0x10, 0x20, 0x30, 0x08, 0x18, 0x28, 0x38
};

unsigned char systatue[6]={0x00,0x00,0x00,0X00,0x00,0x00}; //slave master device motor3 motor2 motor1




u8 dc_resist_check_flag = 0;

uint8_t usart2_enable_disable = 0;

u8 slave_ganpower_reset_flag = 0;
u8 device_dc_resist_cutbox_flag = 0;
u8 device_start_flag = 0;


int main(void)
{
    uint16_t i=0;
		//int delay_tim;
		//Power_State=2;
		stm32_init();
		usart2_init();
		usart1_init();
		ch438_init();
		relay_init();
        gpio_init();
		tim3motor_init();
		TIM4LED_init();
		EXTI_ClearITPendingBit(EXTI_Line6);
		
    
    CLS_all_power();
		for(i=0;i<2048;i++)
		{
					usart1_ch438q_buffer[i]=0x00;
					usart1_ch438q_buffer2[i]=0x00;
		}
		usart2_flag = 0;
		usart1_flag=0;
		//开始状态查询
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		delay(300);
		GREEN_LED_SET(Bit_SET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		delay(300);
		GREEN_LED_SET(Bit_SET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		if(slavstate(0xac))
				systatue[0]=0xff;
		GREEN_LED_SET(Bit_SET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		if(slavstate(0xa3))
				systatue[3]=0xff;
		GREEN_LED_SET(Bit_SET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		if(slavstate(0xa2))
				systatue[4]=0xff;
		GREEN_LED_SET(Bit_SET);
		RED_LED_SET(Bit_SET);
		delay(300);
		GREEN_LED_SET(Bit_RESET);
		RED_LED_SET(Bit_RESET);
		if(slavstate(0xa1))
				systatue[5]=0xff;
        delay(100);
        //延时两秒，让继电器放电
		for(i=0; i<10; i++){
            GREEN_LED_SET(Bit_SET);
            RED_LED_SET(Bit_SET);
            delay(100);
            GREEN_LED_SET(Bit_RESET);
            RED_LED_SET(Bit_RESET);   
            delay(100);         	
        }
		delay(300);
		RED_LED_SET(Bit_SET);//结束状态查询
		
		
		//systatue[3]=0xff; //cutbox3
		//systatue[4]=0xff; //cutbox2
		//systatue[5]=0xff; //cutbox1


        
        
		while(1)
		{
            
            
//				for(i=1;i<5;i++)
//					ch438_reg_write(CH438_BASE_ADDR[i]+CH438_REG_RBR_THR_DLL, 0xff);
				time_led();		
				PE7_judge();	
            
            //ch438_reg_write(CH438_BASE_ADDR[3]+CH438_REG_RBR_THR_DLL, 0xAA);
			
				if(usart2_flag)
				{
						usart2_flag = 0;
						usart2_length_bak=usart2_length;
						if(!usart2_length_bak)	continue;						
						for(i=0;i<usart2_length_bak;i++)
							usart2_buffer_bak[i]=usart2_buffer[i];
						usart2_msg.source = usart2_buffer_bak[0];
						usart2_msg.target = usart2_buffer_bak[1];
						usart2_msg.type = usart2_buffer_bak[2];
						usart2_msg.length = ((uint8_t)usart2_buffer_bak[3]<<8)+usart2_buffer_bak[4];
						for(i=0; i<usart2_msg.length; i++)	usart2_msg.data[i] = usart2_buffer_bak[i+5];
						usart2_msg.check = usart2_buffer_bak[usart2_length_bak-1];
					
						
						//Check sum wrong
						if(usart2_msg.check != checksum(usart2_buffer, usart2_length_bak-1))continue;


                        //未上电和心跳
                        if(!Power_State || usart2_msg.target==0x0f)
                        {
                            usart2_enable_disable = 0;
                            //USART_Cmd(USART2, DISABLE);
                            ch438q2PC[0] = 0x0f;
                            ch438q2PC[1] = 0;
                            ch438q2PC[2] = 0x06;
                            ch438q2PC[3] = 0;
                            ch438q2PC[4] = 0x06;
                            for(i=0; i<6; i++) ch438q2PC[i+5] = systatue[i];
                            
                            ch438q2PC[11] = checksum(ch438q2PC, 11);
                            for(i=0; i<12; i++)
                            {
                                    USART_SendData(USART2, ch438q2PC[i]);
                                    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
                            }
                            delay(100);
                            continue; 
                        }else
                        
                        if(usart2_msg.type!=4 && usart2_msg.type!=5)
                        {
                            for(i=0; i<usart2_length_bak; i++)
                            {
                                USART_SendData(USART2, usart2_buffer_bak[i]);
                                while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
                            }
                        }
			
						if(usart2_msg.type==3||usart2_msg.type==0||usart2_msg.type==1||usart2_msg.type==2)
						{
								usart1_ch438q_buffer2[0]=0xff;
								usart1_ch438q_buffer2[1]=0xff;
								usart1_ch438q_buffer2[2]=0xff;
								usart1_ch438q_buffer2[3]=0xff;
								usart1_ch438q_length2=0x04;
						}
                        
                        if(usart2_msg.type==2)
                        {
                            //仪器开启互斥
                            if(DEVICE_RUNNING)
                            {
                                usart1_ch438q_length2=0x06;
								for(i=0; i<6; i++)
										usart1_ch438q_buffer2[i] = systatue[i];
                                
                                device_start_flag = 0;
								continue;
                            }
                        }
                        else 
                        if(usart2_msg.type==3)
						{
                            //仪器关闭检测
                            if(DEVICE_RUNNING && DEVICE_RUNNING!=usart2_msg.target){
                                usart1_ch438q_length2=0x06;
								for(i=0; i<6; i++)
								{
										usart1_ch438q_buffer2[i] = systatue[i];
								}
                                
                                device_start_flag = 0;
								continue;
                            }
                        }
						
						//PC want device message from me
						if(4 == usart2_msg.type)
						{
                            if(0xFF==DEVICE_RUNNING){
                                usart1_ch438q_length2=0x06;
								for(i=0; i<6; i++)usart1_ch438q_buffer2[i] = systatue[i];
                            }else 
                            if(device_start_flag){
                                usart1_ch438q_buffer2[0]=0xff;
								usart1_ch438q_buffer2[1]=0xff;
								usart1_ch438q_buffer2[2]=0xff;
								usart1_ch438q_buffer2[3]=0xff;
								usart1_ch438q_length2=0x04;
                                device_start_flag = 0;
                            }
                            
                            //ch438q2PC[0] = ch438q_port + 3;
                            ch438q2PC[0] = usart2_msg.target;
                            ch438q2PC[1] = 0;
                            ch438q2PC[2] = 0;
                            ch438q2PC[3] = (uint8_t)(usart1_ch438q_length2/256);
                            ch438q2PC[4] = (uint8_t)(usart1_ch438q_length2%256);
                            for(i=0; i<usart1_ch438q_length2; i++)ch438q2PC[i+5] = usart1_ch438q_buffer2[i];
                            ch438q2PC[usart1_ch438q_length2+5] = checksum(ch438q2PC, usart1_ch438q_length2+5);
                            for(i=0; i<usart1_ch438q_length2+6; i++)
                            {
                                    USART_SendData(USART2, ch438q2PC[i]);
                                    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
                            }
                            
                            usart1_ch438q_length2 = 4;
                            usart1_ch438q_buffer2[0] = 0xFF;
                            usart1_ch438q_buffer2[1] = 0xFF;
                            usart1_ch438q_buffer2[2] = 0xFF;
                            usart1_ch438q_buffer2[3] = 0xFF;
                            
                            continue;
						}
						
						//Forward to Low voltage board
                        
						//if(usart2_msg.type==2) systatue[2]=usart2_msg.target;
						//if(usart2_msg.type==3)systatue[2]=0x00;
                        
                        
                        
                        //1和5为切换线
						if(usart2_msg.type!=1 && usart2_msg.type!=5)
						{
							if(usart2_msg.target==8 || usart2_msg.target==9 || usart2_msg.target==10)
							{
                                
								for(i=0; i<usart2_length_bak; i++)
								{
										USART_SendData(USART1, usart2_buffer_bak[i]);
										while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
								}
                                
                                if(usart2_msg.type==2)
                                {
                                    DEVICE_RUNNING=usart2_msg.target;
                                    device_start_flag = 1;
                                    systatue[2]=usart2_msg.target;
                                }else if(usart2_msg.type==3)
                                {
                                    DEVICE_RUNNING = 0;
                                    device_start_flag = 1;
                                    systatue[2]= 0;
                                }
                                
								continue;
							}
                            
                            //else ch438q_port = usart2_msg.target - 3;
						}
						
						//Command to device
						if(0 == usart2_msg.type && DEVICE_RUNNING==usart2_msg.target)
						{
								for(i=0; i<usart2_msg.length; i++)
								{
										ch438_reg_write(CH438_BASE_ADDR[CH438PORT_OPENING]+CH438_REG_RBR_THR_DLL, usart2_msg.data[i]);
								}
								continue;
						}
						
						//Start up one device
						if(2 == usart2_msg.type)
						{
                            
							if(Power_State)
							{
								DEVICE_RUNNING=usart2_msg.target;
                                device_start_flag = 1;
								switch(usart2_msg.target)
								{
								case 4://Million ohm JP3 K34
                                    if(MILLION_OHM_MOTOR_SENSOR_GET()){
                                        TIM_Cmd(TIM3, DISABLE);
                                        //TIM3->CR1&=0<<0;    //关闭定时器3
                                        MOTOR_DISABLE(); //disable motor controller
                                        //delay(1000);
                                        MILLION_OHM_ON();//继电器
                                        DEVICE_RUNNING=4;
                                        device_start_flag = 1;
                                    }else{
                                        TIM_Cmd(TIM3, ENABLE);
                                        MOTOR_MILLION_OHM_DIRECTION();
										MOTOR_ENABLE();
                                        for(i=0; i<100; i++)__NOP;
										//TIM3->CR1|=0x01;    //Enable TIM3
                                        DEVICE_RUNNING = 0xFF;
                                        device_start_flag = 0;
										//GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);	
                                    }
										break;
								case 5://dielectric loss JP5
                                    if(DIELECTRIC_LOSS_MOTOR_SENSOR_GET()){
                                        TIM_Cmd(TIM3, DISABLE);
                                        MOTOR_DISABLE();
                                        DIELECTRIC_LOSS_ON();
                                        DEVICE_RUNNING=5;
                                        device_start_flag = 1;
                                    }else{
										MOTOR_ENABLE();
										MOTOR_DIELECTRIC_LOSS_DIRECTION();
                                        for(i=0; i<100; i++)__NOP;
                                        TIM_Cmd(TIM3, ENABLE);
                                        DEVICE_RUNNING = 0xFF;
                                        device_start_flag = 0;
                                    }
										break;
								case 6://DC resistance JP1
										DC_RESIST_ON();
										DC_RESIST1_ON();
										DC_RESIST2_ON();
										break;
								case 7://Tap switch JP2
										TAP_SWITCH_ON();
										TAP_SWITCH1_ON();
										TAP_SWITCH2_ON();
										break;
								}
								
								continue;
							}
						}
						//Shutdown device
						if(3 == usart2_msg.type)
						{
								switch(usart2_msg.target)
								{
								case 4://Million ohm
										MILLION_OHM_OFF();
										break;
								case 5://dielectric loss
										DIELECTRIC_LOSS_OFF();
										break;
								case 6://DC resistance
										DC_RESIST_OFF();
										DC_RESIST1_OFF();
										DC_RESIST2_OFF();
										break;
								case 7://Tap switch
										TAP_SWITCH_OFF();
										TAP_SWITCH1_OFF();
										TAP_SWITCH2_OFF();
										break;
								}
                                
                                DEVICE_RUNNING = 0;
                                device_start_flag = 1;
                                systatue[2]= 0;
																
								continue;
						}
						
						//对切换线命令数据进行处理，为向三个切换发送命令作准备
						if(usart2_msg.type == 1)
						{
							if(cutbox_command_flag)continue;
                            
                            if(9==usart2_msg.target)
                            {
                                for(i=0; i<usart2_length_bak; i++)
								{
										USART_SendData(USART1, usart2_buffer_bak[i]);
										while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
								}
                                slave_ganpower_reset_flag = 1;
                                if(usart2_msg.data[0]==5 || usart2_msg.data[0]==6 || usart2_msg.data[0]==7)continue;
                                delay_ms(100);
                            }
                            
                            
                            if(DEVICE_RUNNING && (usart2_msg.target != DEVICE_RUNNING))continue;
                            
                                
                           
                    
                            
                            if(0x12==usart2_msg.data[0] && usart2_msg.target==6)
                            {
                                device_dc_resist_cutbox_flag = 1;///DC_RESIST_CURRENT+1;
                            }
                            else
                            {
                                device_dc_resist_cutbox_flag = 0;
                                cutbox_command(usart2_msg.target, usart2_msg.data[0]);
                                if(CUTBOX_HIGH_ONLINE!=0xFF && CUTBOX_HIGH_MUST_ONLINE_GET){
                                    cutbox_command_flag = 0;
                                }else 
                                if(CUTBOX_MID_ONLINE!=0xFF && CUTBOX_MID_MUST_ONLINE_GET){
                                    cutbox_command_flag = 0;
                                }else 
                                if(CUTBOX_LOW_ONLINE!=0xFF && CUTBOX_LOW_MUST_ONLINE_GET){
                                    cutbox_command_flag = 0;
                                }else 
                                if(SLAVE_ONLINE!=0xFF && CUTBOX_SLAVE_MUST_ONLINE_GET){
                                    cutbox_command_flag = 0;
                                }
                            }
						}else 
						
						//查询切换线是否切成功
						if(usart2_msg.type == 5)
						{
                            if(DEVICE_RUNNING && (usart2_msg.target != DEVICE_RUNNING))
                            {
                                ch438q2PC[0] = 0x0f;
                                ch438q2PC[1] = 0;
                                ch438q2PC[2] = 0x06;
                                ch438q2PC[3] = 0;
                                ch438q2PC[4] = 0x06;
                                for(i=0; i<6; i++)
                                {
                                        ch438q2PC[i+5] = systatue[i];
                                }
                                ch438q2PC[11] = checksum(ch438q2PC, 11);
                                for(i=0; i<12; i++)
                                {
                                        USART_SendData(USART2, ch438q2PC[i]);
                                        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
                                }
                            }else
                            if(slave_ganpower_reset_flag)
                            {
                                slave_ganpower_reset_flag++;
                                if(slave_ganpower_reset_flag > 10)
                                {
                                    slave_ganpower_reset_flag = 0;
                                    send_cutbox_msg_to_PC(usart2_msg.target, 0, 0);
                                }
                                else
                                {
                                    send_cutbox_msg_to_PC(usart2_msg.target, 0xFF, 0xFF);
                                }
                            }else
                            if(device_dc_resist_cutbox_flag)
                            {
                                if(DC_RESIST_CURRENT)
                                    send_cutbox_msg_to_PC(usart2_msg.target, 0xFF, 0xFF);
                                else
                                    send_cutbox_msg_to_PC(usart2_msg.target, 0, 0);
                            }else
							//切换盒不在线
							if(CUTBOX_HIGH_ONLINE!=0xFF && CUTBOX_HIGH_MUST_ONLINE_GET){
								send_cutbox_msg_to_PC(usart2_msg.target, 0xEE, 0xE1);
                                cutbox_command_flag = 0;
                                cutbox_all_timeout = 0;
								continue;								
							}else if(CUTBOX_MID_ONLINE!=0xFF && CUTBOX_MID_MUST_ONLINE_GET){
								send_cutbox_msg_to_PC(usart2_msg.target, 0xEE, 0xE2);
                                cutbox_command_flag = 0;
                                cutbox_all_timeout = 0;
								continue;								
							}else if(CUTBOX_LOW_ONLINE!=0xFF && CUTBOX_LOW_MUST_ONLINE_GET){
								send_cutbox_msg_to_PC(usart2_msg.target, 0xEE, 0xE3);
                                cutbox_command_flag = 0;
                                cutbox_all_timeout = 0;
								continue;
							}else if(SLAVE_ONLINE!=0xFF && CUTBOX_SLAVE_MUST_ONLINE_GET){
								send_cutbox_msg_to_PC(usart2_msg.target, 0xEE, 0xEB);
                                cutbox_command_flag = 0;
                                cutbox_all_timeout = 0;
								continue;                                
                            }else{
                                send_cutbox_msg_to_PC(cutbox_device_id, cutbox_motor_result, cutbox_relay_result);
                            }
						}
				}
				
				
				//依次向三个切换箱发送命令
				if(cutbox_command_flag && !cutbox_send_flag)
                {
                    if(0==cutbox_cutting_id)
                    {
                        cutbox_cutting_id = 1;
                        if(CUTBOX_HIGH_ONLINE==0xFF)
                        {
                            command2cutbox(cutbox_device_id, cutbox_cutting_id, cutbox_motor_high, cutbox_relay_high);
                            cutbox_send_flag = 1;
                        }
                    }else
                    if(1==cutbox_cutting_id)
                    {
                        cutbox_cutting_id = 2;
                        if(CUTBOX_MID_ONLINE==0xFF)
                        {
                            command2cutbox(cutbox_device_id, cutbox_cutting_id, cutbox_motor_mid, cutbox_relay_mid);
                            cutbox_send_flag = 1;
                        }
                    }else
                    if(2==cutbox_cutting_id)
                    {
                        cutbox_cutting_id = 3;
                        if(CUTBOX_LOW_ONLINE==0xFF)
                        {
                            command2cutbox(cutbox_device_id, cutbox_cutting_id, cutbox_motor_low, cutbox_relay_low);
                            cutbox_send_flag = 1;
                        }
                    }else
                    if(3==cutbox_cutting_id)
                    {
                        cutbox_motor_result = 0;
                        cutbox_relay_result = 0;
                        cutbox_reset_params();
                    }
				}
				
				//查询切换箱是否切成功
				if(cutbox_send_flag && !cutbox_check_flag)
                {
					if(!cutbox_check_delay)
                    {
                        check_cutbox(cutbox_device_id, cutbox_cutting_id);
                        cutbox_check_flag = 1;
                    }
				}

				
				
				if(usart1_flag)
				{
                    usart1_flag = 0;
                    //Received from Low voltage board, and forward to PC
                    usart1_ch438q_length2=usart1_ch438q_length;
                    for(i=0; i<usart1_ch438q_length; i++){
                            usart1_ch438q_buffer2[i]=usart1_ch438q_buffer[i];
                    }
                    
                    if(cutbox_check_flag)
                    {
                        cutbox_check_flag = 0;
                        

                        //usart1_ch438q_buffer2[6] &= 0x7F;
                        
                        if(usart1_ch438q_buffer2[5] != 0xFF && usart1_ch438q_buffer2[6] != 0xFF)
                        {
                            if(cutbox_cutting_id==1)
                            {
                                if(usart1_ch438q_buffer2[5]==cutbox_motor_high && usart1_ch438q_buffer2[6]==(0x3F & cutbox_relay_high))
                                {
                                    cutbox_send_flag = 0;
                                }
                                else
                                {
                                    cutbox_motor_result = usart1_ch438q_buffer2[5];
                                    cutbox_relay_result = usart1_ch438q_buffer2[6];
                                    cutbox_reset_params();
                                }
                            
                            }else
                            if(cutbox_cutting_id==2)
                            {
                                if(usart1_ch438q_buffer2[5]==cutbox_motor_mid && usart1_ch438q_buffer2[6]==(0x3F & cutbox_relay_mid))
                                {
                                    cutbox_send_flag = 0;
                                }
                                else
                                {
                                    cutbox_motor_result = usart1_ch438q_buffer2[5];
                                    cutbox_relay_result = usart1_ch438q_buffer2[6];
                                    cutbox_reset_params();
                                }
                            }else
                            if(cutbox_cutting_id==3)
                            {
                                if(usart1_ch438q_buffer2[5]==cutbox_motor_low && usart1_ch438q_buffer2[6]==(0x3F & cutbox_relay_low))
                                {
                                    cutbox_send_flag = 0;
                                }
                                else
                                {
                                    cutbox_motor_result = usart1_ch438q_buffer2[5];
                                    cutbox_relay_result = usart1_ch438q_buffer2[6];
                                    cutbox_reset_params();
                                }
                            }
                        }
                    }
				}
				
				
				//delay_tim=delay_438(100);
				//if(!delay_tim) delay_tim=delay_438(100);
				if(ch438q_flag)
				{
                    ch438q_flag = 0;
                    //Received from Low voltage board, and forward to PC
                    
                    if(usart1_ch438q_length>6&&DEVICE_RUNNING==6)
                    {
                            if(usart1_ch438q_buffer[0]==0x11&&usart1_ch438q_buffer[1]==0x0a&&usart1_ch438q_buffer[2]==0x59)	continue;
                            if(usart1_ch438q_buffer[0]==0x11&&usart1_ch438q_buffer[1]==0x0a&&usart1_ch438q_buffer[2]==0x5a)	continue;
                    }
					
                    if(!usart1_ch438q_length)continue;
                    usart1_ch438q_length2=usart1_ch438q_length;
                    for(i=0; i<usart1_ch438q_length; i++)	
                    {
                        usart1_ch438q_buffer2[i]=usart1_ch438q_buffer[i];
                        //__NOP();
                    }
				}
				//delay_tim=delay_438(100);
		}
}


void delay(int delay_time)
{
		int i,j;
		for(i=0;i<delay_time;i++)
			for(j=0;j<0x500;j++)	__NOP();
}


int delay_438(int tim_in)
{
		int delay_tim=tim_in;
		while(ch438_bool&&delay_tim)
		{
						delay(1);
						delay_tim--;
		}
		return(delay_tim);
}

void delay_usrt1(int tim_in)
{
		int delay_tim=tim_in;
		while((!usart1_flag)&&delay_tim)
		{
						delay(1);
						delay_tim--;
		}
}

unsigned char slavstate(unsigned char st_x)
{
		usart1_flag=0;
		USART_SendData(USART1, st_x);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, st_x);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		delay_usrt1(500);
		if(usart1_flag)
		{
					usart1_flag=0;
					return(1);
		}
		else
		{
				RED_LED_SET(Bit_SET);
				USART_SendData(USART1, st_x);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				USART_SendData(USART1, st_x);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				delay_usrt1(500);
				RED_LED_SET(Bit_RESET);
				if(usart1_flag)
				{
					usart1_flag=0;
					return(1);
				}
				else
					return(0);
		}
}

void CLS_all_power(void)
{
//Million ohm
		MILLION_OHM_OFF();
//dielectric loss
		DIELECTRIC_LOSS_OFF();		
//DC resistance
		DC_RESIST_OFF();
		DC_RESIST1_OFF();
		DC_RESIST2_OFF();
//Tap switch
		TAP_SWITCH_OFF();
		TAP_SWITCH1_OFF();
		TAP_SWITCH2_OFF();
}

void PE7_judge(void)
{
		unsigned char i;
        u8 cmd2slave[6] = {0x00, 0x09, 0x03, 0x00, 0x00, 0x0C};
		if(MASTER_RESET_BUTTON_GET())
		{
				if(Power_State==0)	return;
				delay(10);
				if(MASTER_RESET_BUTTON_GET())
				{
						//systatue[1]=0x00;
						//CLS_all_power();
						//GREEN_LED_SET(Bit_RESET);
						//RED_LED_SET(Bit_SET);
						//Power_State=0;
                    //command2cutbox(0, 1, 0x1A, 0x00);
                    
                        //delay(60);
                        //command2cutbox(0, 2, 0x1A, 0x00);
                        //delay(60);
                        //command2cutbox(0, 3, 0x1A, 0x00);
                        //delay(60);
                        for(i=0; i<6; i++)
                        {
                            USART_SendData(USART1, cmd2slave[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
                        }
                        delay(60);
                        cmd2slave[1] = 10;
                        cmd2slave[5] = 0x0D;
                        for(i=0; i<6; i++)
                        {
                            USART_SendData(USART1, cmd2slave[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
                        }
                        delay(10);
                        __set_FAULTMASK(1);
                        NVIC_SystemReset();
				}
		}
		else
		{
				if(Power_State==1)	return;
				systatue[1]=0xff;
				GREEN_LED_SET(Bit_SET);
				RED_LED_SET(Bit_RESET);
				Power_State=1;
            
		}
		
}



char LEDCHANCE;
void time_led(void)
{
		if(time_counter)
		{
            LEDCHANCE=!LEDCHANCE;
            BOARD_LED_SET((BitAction)LEDCHANCE);
			time_counter=0;
		}
}

void HardFault_Handler(void)
{
	while(1);
	//__disable_fault_irq();   
  //NVIC_SystemReset();
	//return;
}


