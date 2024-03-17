#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
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
#include "SHT30.h"
#include "RNG.h"
//#include "cJSON.h"
/* FatFs includes component */
//#include "ff.h"
//#include "ff_gen_drv.h"
//#include "sd_diskio.h"

extern unsigned char SD_Check(void);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
