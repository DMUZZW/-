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
//STM32 F746������ʹ�õ���24c04�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE AT24C04



extern unsigned char  I2C_BUFF[37];



					  
extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
extern void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
extern void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
extern u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
extern void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

extern u8 AT24CXX_Check(void);  //�������
extern void AT24CXX_Init(void); //��ʼ��IIC
//extern void AT24C02_Test(void);
extern void AT24C02_System_Config_Initialiation(void);
#endif
