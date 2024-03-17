#ifndef _8AIAO_PROTOCOL_H_
#define _8AIAO_PROTOCOL_H_

#include "sys.h"
#include "delay.h"
#include "AD5676.h"
#include "32DIDO.h"
#include "define.h"
#include "udp_demo.h"

#define AD5676_Command 0x30  //4个控制位的取值 为0011 write to and update DAC Channel n 是高4位
//#define AD5676_DAC1 0x00|AD5676_Command
//#define AD5676_DAC2 0x01|AD5676_Command
//#define AD5676_DAC3 0x03|AD5676_Command
//#define AD5676_DAC4 0x02|AD5676_Command
//#define AD5676_DAC5 0x06|AD5676_Command
//#define AD5676_DAC6 0x07|AD5676_Command
//#define AD5676_DAC7 0x04|AD5676_Command
//#define AD5676_DAC8 0x05|AD5676_Command


#define AD5676_DAC1 0x05|AD5676_Command
#define AD5676_DAC2 0x04|AD5676_Command
#define AD5676_DAC3 0x07|AD5676_Command
#define AD5676_DAC4 0x06|AD5676_Command
#define AD5676_DAC5 0x02|AD5676_Command
#define AD5676_DAC6 0x03|AD5676_Command
#define AD5676_DAC7 0x01|AD5676_Command
#define AD5676_DAC8 0x00|AD5676_Command

//#define AD5676_DAC1 0x05|AD5676_Command
//#define AD5676_DAC2 0x04|AD5676_Command
//#define AD5676_DAC3 0x01|AD5676_Command
//#define AD5676_DAC4 0x00|AD5676_Command
//#define AD5676_DAC5 0x02|AD5676_Command
//#define AD5676_DAC6 0x03|AD5676_Command
//#define AD5676_DAC7 0x06|AD5676_Command
//#define AD5676_DAC8 0x07|AD5676_Command



struct AnalogDataOutput
{
	unsigned char DATA1;
	unsigned char DATA2;
	unsigned char DATA3;
	unsigned char DATA4;
};

union AD5676_AnalogDataOutput
{
	unsigned int DATA;
	struct AnalogDataOutput AnalogData_Output;
};

struct Net_AnalogDataOutput
{
	unsigned char FrameLength;
	unsigned char Analog_Channel;
	union AD5676_AnalogDataOutput Analog_DataOutput[8];
	
	
};

extern struct AnalogDataOutput  AnalogDataOutput_New,AnalogDataOutput_Old;
extern struct Net_AnalogDataOutput Net_AnalogDataOutput_New,Net_AnalogDataOutput_Old;
extern unsigned char AD5676_DAC[8];
extern void AnalogDataInput_Process(unsigned char channel,unsigned char times);

#endif


