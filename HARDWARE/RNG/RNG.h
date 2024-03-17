#ifndef _RNG_H
#define _RNG_H

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


extern  u8 RNG_Init(void);			//RNG初始化 
extern  u32 RNG_Get_RandomNum(void);//得到随机数
extern  int RNG_Get_RandomRange(int min,int max);//得到属于某个范围内的随机数



#endif 

