#ifndef _RTC_H_
#define _RTC_H_

#include "iwdg.h"
#include "sys.h"
#include "delay.h"
#include "AD5676.h"
#include "32DIDO.h"
#include "define.h"
#include "udp_demo.h"
#include "DIDOAIAO.h"

extern RTC_HandleTypeDef RTC_Handler;  
extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
extern unsigned char  RTC_Init(void);
extern HAL_StatusTypeDef Set_Board_RTC_Time(unsigned char hour,unsigned char min,unsigned char second,unsigned char AMPM);
extern HAL_StatusTypeDef Set_Board_RTC_Date(unsigned char year,unsigned char month,unsigned char date,unsigned char week);
extern HAL_StatusTypeDef Get_Board_Date(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
extern HAL_StatusTypeDef Get_Board_Time(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
extern void RTC_TEST(void);
#endif



