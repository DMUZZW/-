#ifndef _24CXX_H
#define _24CXX_H
#include "sys.h"
#include "myiic.h"
#include "w5500_conf.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//STM32 F746开发板使用的是24c04，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C04



extern unsigned char  I2C_BUFF[37];



					  
extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
extern void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
extern void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
extern u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
extern void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

extern u8 AT24CXX_Check(void);  //检查器件
extern void AT24CXX_Init(void); //初始化IIC
//extern void AT24C02_Test(void);
extern void AT24C02_System_Config_Initialiation(void);
#endif
