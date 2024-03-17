#ifndef _32DIDO_H
#define _32DIDO_H

#include "sys.h"
#include "stm32f7xx_hal_gpio.h"


////////////////////////////////////////////////////////////////////////////////// 
//Ex_NVIC_Configר�ö���
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 

#define FTIR   				1  		//�½��ش���
#define RTIR   				2  		//�����ش���

//GPIO����ר�ú궨��
#define GPIO_MODE_IN    	0		//��ͨ����ģʽ
#define GPIO_MODE_OUT		1		//��ͨ���ģʽ
#define GPIO_MODE_AF		2		//AF����ģʽ
#define GPIO_MODE_AIN		3		//ģ������ģʽ

#define GPIO_SPEED_2M		0		//GPIO�ٶ�2Mhz(����)
#define GPIO_SPEED_25M		1		//GPIO�ٶ�25Mhz(����)
#define GPIO_SPEED_50M		2		//GPIO�ٶ�50Mhz(����)
#define GPIO_SPEED_100M		3		//GPIO�ٶ�100Mhz(����)

#define GPIO_PUPD_NONE		0		//����������
#define GPIO_PUPD_PU		1		//����
#define GPIO_PUPD_PD		2		//����
#define GPIO_PUPD_RES		3		//���� 

#define GPIO_OTYPE_PP		0		//�������
#define GPIO_OTYPE_OD		1		//��©��� 

//GPIO���ű�Ŷ���
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


