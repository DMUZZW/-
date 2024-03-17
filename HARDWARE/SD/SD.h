#ifndef __SD_H
#define __SD_H


#include "stm32f7xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

#define SD_TIMEOUT      ((uint32_t)100000000)       //��ʱʱ��
#define SD_DMA_MODE     1                        //1��DMAģʽ��0����ѯģʽ   

extern SD_HandleTypeDef        SDCARD_Handler;      //SD�����
extern HAL_SD_CardInfoTypedef  SDCardInfo;          //SD����Ϣ�ṹ��


unsigned char SD_Init(void);
unsigned char SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);
unsigned char SD_ReadDisk(u8* buf,u32 sector,u8 cnt);
unsigned char SD_WriteDisk(u8 *buf,u32 sector,u8 cnt);
unsigned char SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
unsigned char SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);

#endif 

