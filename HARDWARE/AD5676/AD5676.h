#ifndef _AD5676_H
#define _AD5676_H

#include "sys.h"
#include "delay.h"




extern void STM32_AD5676_SPI_GPIO (void);
extern void SPI2_Send3Byte(unsigned int Txdata);
extern void AD5676_Init(void);
extern void AD5676_SendData(unsigned char channel,unsigned char AnalogData);
extern void AD5676_Send_AnalogData(unsigned int data);
#endif
