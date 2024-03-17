#ifndef _LOT_H_
#define _LOT_H_


#include "iwdg.h"
#include "sys.h"
#include "delay.h"
#include "AD5676.h"
#include "32DIDO.h"
#include "define.h"
#include "udp_demo.h"
#include "DIDOAIAO.h"
#include "RTC.h"
#include "Timer.h"
#include "fattester.h"	
#include "main.h"
#include "SHT30.h"

extern  void Restart_instructions (void);
extern  void Low_Power_Consumption (void);
extern  void AnalogDataInput_Channel_Setting(unsigned short pointer);
extern  void FirmwareUpdate_Flash_Choose(void);
extern  void TimeService_Temperature_Humidity(void);
extern  void  heart_beat_test(void);
#endif
