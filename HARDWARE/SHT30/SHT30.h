#ifndef _SHT30_H
#define _SHT30_H

#include "stm32f7xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "SPI_W5500.h"
#include "W5500.h"
#include "LED.h"
#include "32DIDO.h"
#include "AD5676.h"
#include "24cxx.h"
#include "CAN.h"
#include "tcp_demo.h"
#include "udp_demo.h"
#include "32DIDO_PROTOCOL.h"
#include "Flash.h"
#include "http_server.h"
#include "httputil.h"
#include "iap.h" 
#include "iwdg.h"
#include "DIDOAIAO.h"
#include "RTC.h"
#include "ADC.h"
#include "SD.h"
#include "Timer.h"
#include "usmart.h"
#include "malloc.h"  
#include "ff.h"
#include "exfuns.h"
#include "mpu.h"
#include "fattester.h"	



//IO方向设置
#define T_SDA_IN()  {GPIOA->MODER&=~(3<<(9*2));GPIOA->MODER|=0<<9*2;}	//PA9输入模式
#define T_SDA_OUT() {GPIOA->MODER&=~(3<<(9*2));GPIOA->MODER|=1<<9*2;} //PA9输出模式

//IO操作
#define T_IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)) //SCL  PA8
#define T_IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET)) //SDA  PA9
#define T_READ_SDA    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)  //读输入SDA PA9

//T_IIC所有操作函数
extern void T_IIC_Init(void);         //初始化IIC的IO口				 
extern void T_IIC_Start(void);				//发送IIC开始信号
extern void T_IIC_Stop(void);	  			//发送IIC停止信号
extern void T_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
extern u8 T_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
extern u8 T_IIC_Wait_Ack(void); 				//IIC等待ACK信号
extern void T_IIC_Ack(void);					//IIC发送ACK信号
extern void T_IIC_NAck(void);				//IIC不发送ACK信号

extern void T_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 T_IIC_Read_One_Byte(u8 daddr,u8 addr);	 
extern void SHT_Init(void);
extern void sht30_read_temp_humi(char Temperature[2], char Humidity[2]);

extern void Temperature_Humidity(unsigned char times,unsigned char SHT31_DATA[]);





#endif

