#ifndef _32DIDO_PROTOCOL_H_
#define _32DIDO_PROTOCOL_H_


#include "32DIDO.h"
#include "define.h"
#include "udp_demo.h"
#include "iwdg.h"
extern volatile union DigitalOut DigitalOutNew,DigitalOutOld;
extern volatile struct NET_DigitalTx Net_DigitalDataIn;
extern volatile union DigitalIn DigitalInputNew,DigitalInputOld;
extern volatile struct NET_DigitalRx Net_DigitalDataOut;//上位机给板卡发送数据 

struct  DigitaloutByte
{
	unsigned char  BYTE1:8;  // 31:24
	unsigned char  BYTE2:8;  // 23:16
  unsigned char  BYTE3:8;  // 15:8
  unsigned char  BYTE4:8;	// 7:0
};

struct DigitaoutBit
{
	unsigned int  BIT1:1;
	unsigned int  BIT2:1;
	unsigned int  BIT3:1;
	unsigned int  BIT4:1;
	unsigned int  BIT5:1;
	unsigned int  BIT6:1;
	unsigned int  BIT7:1;
	unsigned int  BIT8:1;
	unsigned int  BIT9:1;
	unsigned int  BIT10:1;
	unsigned int  BIT11:1;
	unsigned int  BIT12:1;
	unsigned int  BIT13:1;
	unsigned int  BIT14:1;
	unsigned int  BIT15:1;
	unsigned int  BIT16:1;
	unsigned int  BIT17:1;
	unsigned int  BIT18:1;
	unsigned int  BIT19:1;
	unsigned int  BIT20:1;
	unsigned int  BIT21:1;
	unsigned int  BIT22:1;
	unsigned int  BIT23:1;
	unsigned int  BIT24:1;
	unsigned int  BIT25:1;
	unsigned int  BIT26:1;
	unsigned int  BIT27:1;
  unsigned int  BIT28:1;
	unsigned int  BIT29:1;
	unsigned int  BIT30:1;
	unsigned int  BIT31:1;
	unsigned int  BIT32:1;
};

union DigitalOut
{
	unsigned int all;
	struct  DigitaloutByte  digitaloutbyte;
	struct  DigitaoutBit    digitaloutbit;
	
};
//数据输出
struct NET_DigitalRx {
	unsigned char FrameLength; //数据帧长度，如果长度不正确则将数据舍弃
	unsigned char AnalogCh_Sign;//模拟量输出标志位
	unsigned char Analog_Data[8];//模拟量输出数据
	union DigitalOut Digital_out;
};

//数字量输出位处理



//新顺序 传0输出高电平 传1输出低电平

//#define DO1(x)  GPIO_Pin_Set(GPIOE,PIN14,x)  
//#define DO2(x)  GPIO_Pin_Set(GPIOE,PIN15,x)
//#define DO3(x)  GPIO_Pin_Set(GPIOG,PIN11,x) 
//#define DO4(x)  GPIO_Pin_Set(GPIOG,PIN10,x) 
//#define DO5(x)  GPIO_Pin_Set(GPIOG,PIN9,x)
//#define DO6(x)  GPIO_Pin_Set(GPIOG,PIN8,x)
//#define DO7(x)  GPIO_Pin_Set(GPIOG,PIN3,x) 
//#define DO8(x)  GPIO_Pin_Set(GPIOG,PIN2,x)
//#define DO9(x)  GPIO_Pin_Set(GPIOE,PIN0,x)
//#define DO10(x) GPIO_Pin_Set(GPIOE,PIN1,x)
//#define DO11(x) GPIO_Pin_Set(GPIOE,PIN6,x)
//#define DO12(x) GPIO_Pin_Set(GPIOE,PIN10,x)
//#define DO13(x) GPIO_Pin_Set(GPIOE,PIN9,x)
//#define DO14(x) GPIO_Pin_Set(GPIOE,PIN8,x)
//#define DO15(x) GPIO_Pin_Set(GPIOE,PIN12,x)
//#define DO16(x)  GPIO_Pin_Set(GPIOE,PIN13,x)
//#define DO17(x) GPIO_Pin_Set(GPIOG,PIN15,x)
//#define DO18(x) GPIO_Pin_Set(GPIOG,PIN14,x)
//#define DO19(x) GPIO_Pin_Set(GPIOG,PIN13,x)
//#define DO20(x) GPIO_Pin_Set(GPIOG,PIN12,x)
//#define DO21(x) GPIO_Pin_Set(GPIOG,PIN7,x)
//#define DO22(x) GPIO_Pin_Set(GPIOG,PIN6,x)
//#define DO23(x)  GPIO_Pin_Set(GPIOG,PIN5,x)
//#define DO24(x) GPIO_Pin_Set(GPIOG,PIN4,x)
//#define DO25(x) GPIO_Pin_Set(GPIOE,PIN3,x)
//#define DO26(x) GPIO_Pin_Set(GPIOE,PIN2,x)
//#define DO27(x)  GPIO_Pin_Set(GPIOE,PIN5,x)
//#define DO28(x) GPIO_Pin_Set(GPIOE,PIN4,x)
//#define DO29(x) GPIO_Pin_Set(GPIOE,PIN7,x)
//#define DO30(x) GPIO_Pin_Set(GPIOG,PIN1,x)
//#define DO31(x) GPIO_Pin_Set(GPIOG,PIN0,x)
//#define DO32(x) GPIO_Pin_Set(GPIOE,PIN11,x)


#define DO1(x)  GPIO_Pin_Set(GPIOG,PIN2,x)
#define DO2(x)  GPIO_Pin_Set(GPIOG,PIN3,x)
#define DO3(x)  GPIO_Pin_Set(GPIOG,PIN8,x) 
#define DO4(x)  GPIO_Pin_Set(GPIOG,PIN9,x) 
#define DO5(x)  GPIO_Pin_Set(GPIOG,PIN10,x)
#define DO6(x)  GPIO_Pin_Set(GPIOG,PIN11,x)
#define DO7(x)  GPIO_Pin_Set(GPIOE,PIN15,x) 
#define DO8(x)  GPIO_Pin_Set(GPIOE,PIN14,x)
#define DO9(x)  GPIO_Pin_Set(GPIOE,PIN13,x)
#define DO10(x) GPIO_Pin_Set(GPIOE,PIN12,x)
#define DO11(x) GPIO_Pin_Set(GPIOE,PIN8,x)
#define DO12(x) GPIO_Pin_Set(GPIOE,PIN9,x)
#define DO13(x) GPIO_Pin_Set(GPIOE,PIN10,x)
#define DO14(x) GPIO_Pin_Set(GPIOE,PIN6,x)
#define DO15(x) GPIO_Pin_Set(GPIOE,PIN1,x)
#define DO16(x)  GPIO_Pin_Set(GPIOE,PIN0,x)
#define DO17(x) GPIO_Pin_Set(GPIOG,PIN4,x)
#define DO18(x) GPIO_Pin_Set(GPIOG,PIN5,x)
#define DO19(x) GPIO_Pin_Set(GPIOG,PIN6,x)
#define DO20(x) GPIO_Pin_Set(GPIOG,PIN7,x)
#define DO21(x) GPIO_Pin_Set(GPIOG,PIN12,x)
#define DO22(x) GPIO_Pin_Set(GPIOG,PIN13,x)
#define DO23(x)  GPIO_Pin_Set(GPIOG,PIN14,x)
#define DO24(x) GPIO_Pin_Set(GPIOG,PIN15,x)
#define DO25(x) GPIO_Pin_Set(GPIOE,PIN11,x)
#define DO26(x) GPIO_Pin_Set(GPIOG,PIN0,x)
#define DO27(x)  GPIO_Pin_Set(GPIOG,PIN1,x)
#define DO28(x) GPIO_Pin_Set(GPIOE,PIN7,x)
#define DO29(x) GPIO_Pin_Set(GPIOE,PIN4,x)
#define DO30(x) GPIO_Pin_Set(GPIOE,PIN5,x)
#define DO31(x) GPIO_Pin_Set(GPIOE,PIN2,x)
#define DO32(x) GPIO_Pin_Set(GPIOE,PIN3,x)


/**************************************************************************************************************************************************************/

struct DigitalinByte 
{
	unsigned char BYTE1:8;
	unsigned char BYTE2:8;
	unsigned char BYTE3:8;
	unsigned char BYTE4:8;
};

struct DigitalinBit 
{
	unsigned int  BIT1:1;
	unsigned int  BIT2:1;
	unsigned int  BIT3:1;
	unsigned int  BIT4:1;
	unsigned int  BIT5:1;
	unsigned int  BIT6:1;
	unsigned int  BIT7:1;
	unsigned int  BIT8:1;
	unsigned int  BIT9:1;
	unsigned int  BIT10:1;
	unsigned int  BIT11:1;
  unsigned int  BIT12:1;
	unsigned int  BIT13:1;
	unsigned int  BIT14:1;
	unsigned int  BIT15:1;
	unsigned int  BIT16:1;
	unsigned int  BIT17:1;
	unsigned int  BIT18:1;
	unsigned int  BIT19:1;
	unsigned int  BIT20:1;
	unsigned int  BIT21:1;
	unsigned int  BIT22:1;
	unsigned int  BIT23:1;
	unsigned int  BIT24:1;
	unsigned int  BIT25:1;
	unsigned int  BIT26:1;
	unsigned int  BIT27:1;
	unsigned int  BIT28:1;
	unsigned int  BIT29:1;
	unsigned int  BIT30:1;
	unsigned int  BIT31:1;
	unsigned int  BIT32:1;
	
};


union DigitalIn
{
  unsigned int all;
	struct DigitalinByte  Digitalin_Byte;
	struct DigitalinBit   Digitalin_Bit;	
};


//下位机向上位机发送数据，数字量采集部分，数据区处理
struct NET_DigitalTx{
	unsigned char NetTrans_Num;//需要发送的数据个数
	union DigitalIn Digital_in;	
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//数字量输入位处理

#define DI1     GPIO_Pin_Get(GPIOD,PIN8)     //如果DI被按下，引脚状态为低电平，则DI的值为0，如果没有被按下，则引脚为高电平，则返回的是1
#define DI2     GPIO_Pin_Get(GPIOD,PIN9)
#define DI3     GPIO_Pin_Get(GPIOD,PIN14)
#define DI4     GPIO_Pin_Get(GPIOD,PIN15)  
#define DI5     GPIO_Pin_Get(GPIOA,PIN15)
#define DI6     GPIO_Pin_Get(GPIOD,PIN0)
#define DI7     GPIO_Pin_Get(GPIOD,PIN6)
#define DI8     GPIO_Pin_Get(GPIOD,PIN7)
#define DI9     GPIO_Pin_Get(GPIOF,PIN10)
#define DI10    GPIO_Pin_Get(GPIOF,PIN9)
#define DI11    GPIO_Pin_Get(GPIOF,PIN15)
#define DI12    GPIO_Pin_Get(GPIOF,PIN14)
#define DI13    GPIO_Pin_Get(GPIOI,PIN11)
#define DI14    GPIO_Pin_Get(GPIOI,PIN10)
#define DI15    GPIO_Pin_Get(GPIOF,PIN5)
#define DI16    GPIO_Pin_Get(GPIOF,PIN4)
#define DI17    GPIO_Pin_Get(GPIOD,PIN10)
#define DI18    GPIO_Pin_Get(GPIOD,PIN11)
#define DI19    GPIO_Pin_Get(GPIOD,PIN12) 
#define DI20    GPIO_Pin_Get(GPIOD,PIN13)
#define DI21    GPIO_Pin_Get(GPIOD,PIN1)
#define DI22    GPIO_Pin_Get(GPIOD,PIN3)
#define DI23    GPIO_Pin_Get(GPIOD,PIN4)
#define DI24    GPIO_Pin_Get(GPIOD,PIN5)   
#define DI25    GPIO_Pin_Get(GPIOF,PIN8)
#define DI26    GPIO_Pin_Get(GPIOF,PIN11)
#define DI27    GPIO_Pin_Get(GPIOF,PIN12)
#define DI28    GPIO_Pin_Get(GPIOF,PIN13)
#define DI29    GPIO_Pin_Get(GPIOF,PIN1)
#define DI30    GPIO_Pin_Get(GPIOF,PIN0)
#define DI31    GPIO_Pin_Get(GPIOF,PIN3)
#define DI32    GPIO_Pin_Get(GPIOF,PIN2)
 

 extern void OUTPUTDATA_INIT(void);
 extern void INPUTDATA_INIT(void);
 extern void DigitalDataInput_NetExchange(void);
 extern void DigitalDataInput_Process(void);
 extern void Read_DigitalData_Input_State(void);

 extern void DigitalData_Output_Process(void);
 extern void AODO_test(void);








extern void INPUTDATA_INIT(void);

#endif
