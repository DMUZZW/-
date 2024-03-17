#ifndef __ADC_H
#define __ADC_H



#include "stm32f7xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "SPI_W5500.h"
#include "W5500.h"
#include "usart.h"
#include "LED.h"
#include "32DIDO.h"
#include "AD5676.h"
#include "24cxx.h"
#include "udp_demo.h"
#include "32DIDO_PROTOCOL.h"
#include "DIDOAIAO.h"
#include "RTC.h"


#define RHEOSTAT_NOFCHANEL      6  //ADC采集通道数量


extern __IO unsigned short ADC_ConvertedValue[RHEOSTAT_NOFCHANEL];
extern unsigned short ADC_CovertValueToReadFromDMA[RHEOSTAT_NOFCHANEL];

/*=====================通道1 IO======================*/
// PC0 4-20mA电流采集1号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT1             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1              GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK1_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL1               ADC_CHANNEL_10
/*=====================通道2 IO ======================*/
// PC1 电阻采集1号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT2             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN2              GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK2_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL2               ADC_CHANNEL_11

/*=====================通道3 IO ======================*/
// PC2  电阻采集3号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT3             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN3              GPIO_PIN_2
#define RHEOSTAT_ADC_GPIO_CLK3_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL3               ADC_CHANNEL_12
/*=====================通道4 IO ======================*/
// PC3  电阻采集4号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT4             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN4              GPIO_PIN_3
#define RHEOSTAT_ADC_GPIO_CLK4_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL4               ADC_CHANNEL_13

/*=====================通道5 IO ======================*/
// PA1  4-20mA电流采集2号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT5             GPIOA
#define RHEOSTAT_ADC_GPIO_PIN5              GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK5_ENABLE()     __GPIOA_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL5               ADC_CHANNEL_1

/*=====================通道6 IO ======================*/
// PA2 电阻采集2号通道
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT6             GPIOA
#define RHEOSTAT_ADC_GPIO_PIN6              GPIO_PIN_2
#define RHEOSTAT_ADC_GPIO_CLK6_ENABLE()     __GPIOA_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL6               ADC_CHANNEL_2

// ADC 序号宏定义
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define RHEOSTAT_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define RHEOSTAT_ADC_DMA_STREAM             DMA2_Stream0





extern void  Rheostat_Init(void);
void PRINT(void);
extern unsigned short ReadADCAverageValue(unsigned char channel,unsigned char times);
//extern  void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
//extern  unsigned short ReadADCAverageValue(unsigned char channel,unsigned char times);
//extern  void ADC_PRINT(void);





#endif





