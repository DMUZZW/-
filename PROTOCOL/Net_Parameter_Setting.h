#ifndef _NET_PARAMETER_SETTING_H
#define _NET_PARAMETER_SETTING_H


#include "stm32f7xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "SPI_W5500.h"
#include "W5500.h"
#include "usart.h"
#include "LED.h"
#include "32DIDO.h"
#include "AD5676.h"
//#include "AD7606.h"
#include "24cxx.h"
#include "CAN.h"
#include "tcp_demo.h"
#include "udp_demo.h"
#include "32DIDO_PROTOCOL.h"
#include "8AIAO_PROTOCOL.h"
#include "fattester.h"	 
#include "LOT.h" 
extern void NetParameter_Setting(unsigned short pointer);
extern void Net_Parameter_Setting(void);
extern void Data_Output_Process(void);
extern void STM32_IAP_Protocol_Process(void);
extern void DigitalDataOutput(void);
extern void AnalogData_Input_Channel_Setting(void);
extern void Get_Parameter(void);
extern void BoardCast(void);
















#endif











