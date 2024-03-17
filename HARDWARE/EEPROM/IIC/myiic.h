#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"



#define SCL_PIN GPIO_PIN_14
#define SDA_PIN GPIO_PIN_15
#define IIC_PORT  GPIOH


//#define SDA_IN()  {IIC_PORT->MODER&= 0x3fffffff;  IIC_PORT->MODER|=0x00000000;}//���ȶ�MODER�Ĵ����������㣬Ȼ����30λ��31λд��00��
//#define SDA_OUT() {IIC_PORT->MODER&= 0x3fffffff;  IIC_PORT->MODER|=0x40000000;}//���ȶ�MODER�Ĵ����������㣬Ȼ����д��01

//SDA�������ǽ�MODER�Ĵ������� �ȶ�Ӧ��λ���� Ȼ��д�� ���Ҳ�� PH15
#define SDA_IN()  {GPIOH->MODER&=~(3<<(15*2));GPIOH->MODER|=0<<15*2;}	//PH5����ģʽ
#define SDA_OUT() {GPIOH->MODER&=~(3<<(15*2));GPIOH->MODER|=1<<15*2;} //PH5���ģʽ


//#define SDA_IN()  {GPIOH->MODER&=0xfffff3ff;GPIOH->MODER|=0x00000000;}	//PH5����ģʽ
//#define SDA_OUT() {GPIOH->MODER&=0xfffff3ff;GPIOH->MODER|=0x00000400;} //PH5���ģʽ


//IO����
//IO����
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(IIC_PORT,SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_PORT,SCL_PIN,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(IIC_PORT,SDA_PIN,GPIO_PIN_SET): HAL_GPIO_WritePin(IIC_PORT,SDA_PIN,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(IIC_PORT,SDA_PIN)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

