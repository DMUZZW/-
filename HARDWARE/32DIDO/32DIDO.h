#ifndef _32DIDO_H
#define _32DIDO_H

#include "sys.h"
#include "stm32f7xx_hal_gpio.h"


////////////////////////////////////////////////////////////////////////////////// 
//Ex_NVIC_Config专用定义
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 

#define FTIR   				1  		//下降沿触发
#define RTIR   				2  		//上升沿触发

//GPIO设置专用宏定义
#define GPIO_MODE_IN    	0		//普通输入模式
#define GPIO_MODE_OUT		1		//普通输出模式
#define GPIO_MODE_AF		2		//AF功能模式
#define GPIO_MODE_AIN		3		//模拟输入模式

#define GPIO_SPEED_2M		0		//GPIO速度2Mhz(低速)
#define GPIO_SPEED_25M		1		//GPIO速度25Mhz(中速)
#define GPIO_SPEED_50M		2		//GPIO速度50Mhz(快速)
#define GPIO_SPEED_100M		3		//GPIO速度100Mhz(高速)

#define GPIO_PUPD_NONE		0		//不带上下拉
#define GPIO_PUPD_PU		1		//上拉
#define GPIO_PUPD_PD		2		//下拉
#define GPIO_PUPD_RES		3		//保留 

#define GPIO_OTYPE_PP		0		//推挽输出
#define GPIO_OTYPE_OD		1		//开漏输出 

//GPIO引脚编号定义
#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern void Digital_DataInput_Init(void);
extern void Digital_DataOutput_Init(void);
extern void Digital_Data_Init(void);

extern unsigned char GPIO_Pin_Get(GPIO_TypeDef* GPIOx,unsigned short pinx);
extern void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,unsigned short pinx,unsigned char status);
extern void GPIO_Set(GPIO_TypeDef* GPIOx,unsigned int BITx,unsigned int MODE,unsigned int OTYPE,unsigned int OSPEED,unsigned int PUPD);
extern void GPIO_AF_Set(GPIO_TypeDef* GPIOx,unsigned char BITx,unsigned char AFx);
#endif


