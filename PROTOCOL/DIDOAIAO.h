#ifndef __DIDOAIAO_H
#define __DIDOAIAO_H


#include "iwdg.h"
#include "sys.h"
#include "delay.h"
#include "AD5676.h"
#include "32DIDO.h"
#include "define.h"
#include "udp_demo.h"
#include "ADC.h"
#include "fattester.h"	 
#include "main.h"
extern unsigned short DataExchangeNum;
extern void DigitalData_AnalogData_InputProcess(void);
extern void DigitalData_AnalogData_QueryState(void);
extern void DigitalData_Output_Process(void);
extern void DigitalData_AnalogData_OutputProcess(unsigned short pointer);
extern void AODO_test(void);
extern void DigitalAnalogData_Compare(unsigned short pointer);

#endif

