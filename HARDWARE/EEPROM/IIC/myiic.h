#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"



#define SCL_PIN GPIO_PIN_14
#define SDA_PIN GPIO_PIN_15
#define IIC_PORT  GPIOH


//#define SDA_IN()  {IIC_PORT->MODER&= 0x3fffffff;  IIC_PORT->MODER|=0x00000000;}//首先对MODER寄存器进行清零，然后在30位和31位写入00；
//#define SDA_OUT() {IIC_PORT->MODER&= 0x3fffffff;  IIC_PORT->MODER|=0x40000000;}//首先对MODER寄存器进行清零，然后在写入01

//SDA的输入是将MODER寄存器设置 先对应两位清零 然后写入 输出也是 PH15
#define SDA_IN()  {GPIOH->MODER&=~(3<<(15*2));GPIOH->MODER|=0<<15*2;}	//PH5输入模式
#define SDA_OUT() {GPIOH->MODER&=~(3<<(15*2));GPIOH->MODER|=1<<15*2;} //PH5输出模式


//#define SDA_IN()  {GPIOH->MODER&=0xfffff3ff;GPIOH->MODER|=0x00000000;}	//PH5输入模式
//#define SDA_OUT() {GPIOH->MODER&=0xfffff3ff;GPIOH->MODER|=0x00000400;} //PH5输出模式


//IO操作
//IO操作
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(IIC_PORT,SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_PORT,SCL_PIN,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(IIC_PORT,SDA_PIN,GPIO_PIN_SET): HAL_GPIO_WritePin(IIC_PORT,SDA_PIN,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(IIC_PORT,SDA_PIN)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

