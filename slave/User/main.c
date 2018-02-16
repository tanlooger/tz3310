#include "stm32f10x.h"
#include "usart2.h"
#include "usart1.h"
#include "usart3.h"
#include "ch438q.h"
#include "protocol.h"
#include "cutbox.h"
#include "tim3motor.h"
#include "relay.h"
#include "stm32.h"
#include "stm32f10x_rcc.h" 

#define PC4_PARAM_GND_SET(X)    (GPIO_WriteBit(GPIOC, GPIO_Pin_4, X))

uint8_t device_run=0;


u8 usart1_flag;


//uint8_t ch438q_buffer[2048];
uint8_t ch438q_buffer2[2048];

//uint16_t ch438q_length;
uint16_t ch438q_length2;
//uint16_t ch438q_counter;

uint8_t ch438q_timeout = 255;
uint8_t ch438q_port = 1;
//uint8_t ch438q_flag;

uint8_t usart1_buffer[256];
uint8_t usart1_buffer_bak[256];
uint8_t usart1_flag;
uint8_t usart1_length;
uint8_t usart1_length_bak;
uint8_t usart1_counter;

char LEDCHANCE;

//uint8_t usart2_buffer[256];
//uint8_t usart2_buffer_bak[256];
//uint8_t usart2_flag;
//uint8_t usart2_length;
//uint8_t usart2_length_bak;
//uint8_t usart2_counter;

uint8_t Power_State;
uint8_t RUN_State;

struct message usart1_msg;


//const uint16_t CH438_BASE_ADDR[8] = {
//	0x00, 0x10, 0x20, 0x30, 0x08, 0x18, 0x28, 0x38
//};


void set_gan_power_zero(void);

int main(void)
{
		int delay_tim;
//		uint16_t P_data;
		Power_State=2;
		stm32_init();
		RUN_State=0;
		usart2_init();
		//usart3_init();
//		usart1_init();
		ch438_init();
		relay_init();
//		tim3motor_init();
		TIM4LED_init();
		EXTI_ClearITPendingBit(EXTI_Line6);
		uint16_t i=0;
		for(i=0;i<2048;i++)
		{
					ch438q_buffer[i]=0x00;
					ch438q_buffer2[i]=0x00;
		}
		GPIO_WriteBit(GPIOC, LED_GREEN, Bit_RESET);
		GPIO_WriteBit(GPIOB, LED_RED, Bit_SET);	
		delay(10);
//delay_ms(5000);
		set_gan_power_zero();
        //delay(200);
		while(1)
		{
				time_led();	
            
            //ch438_put_char(3, 0x5A);
            
            //delay_ms(1);
//				ch438_reg_write(CH438_BASE_ADDR[3]+CH438_REG_RBR_THR_DLL, 0xff);
//				P_data=GPIO_ReadInputData(GPIOA);
//				if(!P_data)	PE7_judge();
				//Power_judge();
				if(usart1_flag)
				{
						usart1_flag = 0;
						usart1_length_bak=usart1_length;
						for(i=0; i<usart1_length_bak; i++){usart1_buffer_bak[i]=usart1_buffer[i];}
						if(usart1_buffer_bak[0]==0xAC&&usart1_buffer_bak[1]==0xAC&&usart1_length_bak==2)
						{
								USART_SendData(USART2, 0xac);
								while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
								continue;
						}
						if(usart1_buffer_bak[0]==0xff&&usart1_buffer_bak[1]==0xff&&usart1_buffer_bak[2]==0xff&&usart1_buffer_bak[3]==0xff)
						{
								CLS_all_power();
								continue;
						}
						//Received from Low voltage board, and forward to PC
						usart1_msg.source = usart1_buffer_bak[0];
						usart1_msg.target = usart1_buffer_bak[1];
						usart1_msg.type = usart1_buffer_bak[2];
						usart1_msg.length = ((uint8_t)usart1_buffer_bak[3]<<8)+usart1_buffer_bak[4];
						if(usart1_msg.length>250)	continue;
						for(i=0; i<usart1_msg.length; i++)	usart1_msg.data[i] = usart1_buffer_bak[i+5];	
						//Forward to Low voltage board
						if(usart1_buffer[1]<8)	continue;
						//else			ch438q_port = usart1_msg.target - 7;
                        
                        //for(i=0; i<2048; i++)usart1_buffer_bak[i] = 0;
						//Command to device
						if(0 == usart1_msg.type)
						{
                            if(8==device_run)
                            {
                                for(i=0; i<usart1_msg.length; i++)
								{
									led_show();
									//ch438_reg_write(CH438_BASE_ADDR[3]+CH438_REG_RBR_THR_DLL, usart1_msg.data[i]);
                                    ch438_put_char(3, usart1_msg.data[i]);
                                }
                            }else
                            if(9==device_run)
                            {
                                if(usart1_msg.target==0x0a)
                                {
                                    for(i=0; i<usart1_msg.length; i++)
                                    {
                                        led_show();
                                        //ch438_reg_write(CH438_BASE_ADDR[1]+CH438_REG_RBR_THR_DLL, usart1_msg.data[i]);
                                        ch438_put_char(1, usart1_msg.data[i]);
                                    }
                                    /*
                                    for(i=0; i<usart1_msg.length; i++)
                                    {
                                        led_show();
                                        ch438_reg_write(CH438_BASE_ADDR[1]+CH438_REG_RBR_THR_DLL, usart1_msg.data[i]);
                                    }
                                    */
                                }
                                else
                                {
                                    for(i=0; i<usart1_msg.length; i++)
                                    {
                                        led_show();
                                        //USART_SendData(USART2, usart1_msg.data[i]);
                                        //while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
                                        //ch438_reg_write(CH438_BASE_ADDR[2]+CH438_REG_RBR_THR_DLL, usart1_msg.data[i]);
                                        ch438_put_char(2, usart1_msg.data[i]);
                                    }
                                }
                            }else
                            if(10==device_run)
                            {
                                for(i=0; i<usart1_msg.length; i++)
                                {
                                    led_show();
                                    //ch438_reg_write(CH438_BASE_ADDR[1]+CH438_REG_RBR_THR_DLL, usart1_msg.data[i]);
                                    ch438_put_char(1, usart1_msg.data[i]);
                                }
                            }
							delay_ms(50);
							continue;
						}
						
                        if(1 == usart1_msg.type && 9==usart1_msg.target)
                        {
                            if(5==usart1_msg.data[0])
                            {
                                TRANS_PARAMS_SWITCH_SET(Bit_RESET);
                                delay_ms(1000);
                                TRANS_PARAMS_SWITCH_SET(Bit_SET);
                            }else
                            if(6==usart1_msg.data[0])
                            {
                                PC4_PARAM_GND_SET(1);
                            }else
                            if(7==usart1_msg.data[0])
                            {
                                PC4_PARAM_GND_SET(0);
                            }
                        }
						//Start up one device
						if(2 == usart1_msg.type)
						{
								GPIO_WriteBit(GPIOC, LED_GREEN, Bit_SET);
								GPIO_WriteBit(GPIOB, LED_RED, Bit_RESET);
								device_run=usart1_msg.target;
								RUN_State=1;
								CLS_all_power();
								switch(usart1_msg.target)
								{
								case 8:////变比
                                        TRANS_RATIO_SWITCH_SET(Bit_SET);
										TRANS_RATIO1(Bit_SET); 
										TRANS_RATIO2(Bit_SET); 
										TRANS_RATIO3(Bit_SET); 
										TRANS_RATIO4(Bit_SET); 
										break;
								case 9:////变参
										TRANS_PARAMS_SWITCH_SET(Bit_SET);
                                        TRANS_PARAMS1(Bit_SET);
                                        TRANS_PARAMS2(Bit_SET);
                                        TRANS_PARAMS3(Bit_SET);
                                        TRANS_PARAMS4(Bit_SET);
                                        POWER_SWITCH_IN_SET(Bit_SET);
                                        POWER_SWITCH_OUT_SET(Bit_SET);
										break;
								case 10://外施耐压
                                        POWER_SWITCH_IN_SET(Bit_SET);
                                        POWER_SWITCH_OUT_SET(Bit_SET);
                                        delay(10);
										//Reset Gan Power 电源复位 0x41 00 00 00 00 00 00 41
										//set_gan_power_zero();
										//delay(200);
										break;
								}
								continue;
						}
						//Shutdown device
						if(3 == usart1_msg.type)
						{
								RUN_State=0;
								GPIO_WriteBit(GPIOC, LED_GREEN, Bit_RESET);
								GPIO_WriteBit(GPIOB, LED_RED, Bit_SET);
								set_gan_power_zero();
								CLS_all_power();
								device_run = 0;
								continue;
						}
				}

				if(ch438q_flag)
				{
						ch438q_flag = 0;
						//Received from Low voltage board, and forward to PC
						ch438q_length2=ch438q_length;
						for(i=0; i<ch438q_length2; i++)	ch438q_buffer2[i]=ch438q_buffer[i];
						for(i=0; (i<ch438q_length2)&&(RUN_State); i++)
						{
										USART_SendData(USART2, ch438q_buffer2[i]);
										while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
						}
						delay(10);
//						for(i=0; i<ch438q_length2; i++)//for(i=0; (i<ch438q_length2)&&RUN_State; i++)
//									ch438_reg_write(CH438_BASE_ADDR[ch438q_port]+CH438_REG_RBR_THR_DLL, ch438q_buffer2[i]);
//						ch438_reg_write(CH438_BASE_ADDR[ch438q_port]+CH438_REG_RBR_THR_DLL, ch438q_port);
//								continue;
				}

//				if(usart2_flag)continue;
		}
}


void delay(int delay_time)
{
		int i,j;
		for(i=0;i<delay_time;i++)
			for(j=0;j<0x500;j++)	__NOP();
}



void CLS_all_power(void)
{
    TRANS_RATIO_SWITCH_SET(Bit_RESET);
    TRANS_RATIO1(Bit_RESET); 
    TRANS_RATIO2(Bit_RESET); 
    TRANS_RATIO3(Bit_RESET); 
    TRANS_RATIO4(Bit_RESET); 

    TRANS_PARAMS_SWITCH_SET(Bit_RESET);
    TRANS_PARAMS1(Bit_RESET);
    TRANS_PARAMS2(Bit_RESET);
    TRANS_PARAMS3(Bit_RESET);
    TRANS_PARAMS4(Bit_RESET);
    POWER_SWITCH_IN_SET(Bit_RESET);
    POWER_SWITCH_OUT_SET(Bit_RESET);
    PC4_PARAM_GND_SET(Bit_RESET);
}

void Power_judge(void)
{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
		{
				delay(10);
				if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
				{
						CLS_all_power();
						Power_State=0;
						device_run = 0;
				}
		}
}



void led_show(void)
{
	LEDCHANCE=!LEDCHANCE;
	GPIO_WriteBit(GPIOB, GPIO_RUN_LED, (BitAction)(!LEDCHANCE));

}

void time_led(void)
{
		if(time_counter)
		{
				LEDCHANCE=!LEDCHANCE;
				if(LEDCHANCE)
						GPIO_WriteBit(GPIOB, GPIO_RUN_LED, Bit_RESET);
				else
						GPIO_WriteBit(GPIOB, GPIO_RUN_LED, Bit_SET);
				time_counter=0;
		}
}

void set_gan_power_zero(void)
{
		uint8_t i=0;
		uint8_t gan_power_zero[8] = {
				0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41
		};
								
		for(i=0; i<8; i++){
				led_show();
				//ch438_reg_write(CH438_BASE_ADDR[3]+CH438_REG_RBR_THR_DLL, gan_power_zero[i]);
            ch438_put_char(1, gan_power_zero[i]);
		}
}
	


void HardFault_Handler(void)
{
	while(1);
//	__disable_fault_irq();   
//  NVIC_SystemReset();
//	return;
}


