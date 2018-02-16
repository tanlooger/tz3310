#ifndef __USER_RELAY_H__
#define __USER_RELAY_H__

#define LED_RED 		GPIO_Pin_0    //B RED
#define LED_GREEN 	GPIO_Pin_5			//C GREEN

//#define TRPOWER_ON	 	GPIO_Pin_2			//B 三相电源开启
//#define TRPOWER_OUT 	GPIO_Pin_7			//E 三相电源输出

//#define TRANSFORMER 	GPIO_Pin_8			//E 变压器测量仪器开启

#define GPIO_RUN_LED 	GPIO_Pin_8				//运行LED指示灯闪烁


//#define TRANSFORMER_P1 	GPIO_Pin_10		//B变参
//#define TRANSFORMER_P2 	GPIO_Pin_10		//E
//#define TRANSFORMER_P3 	GPIO_Pin_15		//E
//#define TRANSFORMER_P4 	GPIO_Pin_9		//E

//#define TRANSFORMER_R1 	GPIO_Pin_11		//E变压器变比
//#define TRANSFORMER_R2 	GPIO_Pin_12		//E
//#define TRANSFORMER_R3 	GPIO_Pin_13		//E
//#define TRANSFORMER_R4 	GPIO_Pin_14		//E

//#define OUTPOWERTEST	 	GPIO_Pin_11		//B外施耐压






#define POWER_SWITCH_IN_SET(X)   (GPIO_WriteBit(GPIOE, GPIO_Pin_8, X))
#define POWER_SWITCH_OUT_SET(X)   (GPIO_WriteBit(GPIOE, GPIO_Pin_7, X))
#define TRANS_RATIO_SWITCH_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_2, X))
#define TRANS_PARAMS_SWITCH_SET(X)   (GPIO_WriteBit(GPIOB, GPIO_Pin_1, X))


#define TRANS_RATIO1(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_11, X))
#define TRANS_RATIO2(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_12, X))
#define TRANS_RATIO3(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_13, X))
#define TRANS_RATIO4(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_14, X))

#define TRANS_PARAMS1(X)    (GPIO_WriteBit(GPIOB, GPIO_Pin_10, X))
#define TRANS_PARAMS2(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_15, X))
#define TRANS_PARAMS3(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_9, X))
#define TRANS_PARAMS4(X)    (GPIO_WriteBit(GPIOE, GPIO_Pin_10, X))


void relay_init(void);





void stm32_pe7_interrupt(void);




#endif
