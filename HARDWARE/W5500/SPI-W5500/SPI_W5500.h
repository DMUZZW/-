#ifndef __SPI_W5500_H
#define __SPI_W5500_H



#include "sys.h"
#include "delay.h"
#include "W5500.h"
#include "24cxx.h" 
#include "define.h"


/**/


/*定义SPI作为W5500的硬件接口*/
#define WIZ_SPIx                         SPI1                   	     /* 定义W5500所用的SPI接口       */
#define WIZ_SPIx_RCC_CLK_ENABLE()        __HAL_RCC_SPI1_CLK_ENABLE()   /* 定义W5500所用的SPI接口时钟   */

#define WIZ_SPI_GPIO_ClK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() /* GPIO端口时钟                 */
#define WIZ_SPIx_GPIO_PORT               GPIOA									  	  /* GPIO端口                     */
#define WIZ_SPIx_SCLK_PIN                GPIO_PIN_5						   	    /* 定义W5500的时钟管脚          */
#define WIZ_SPIx_MISO_PIN                GPIO_PIN_6						   	    /* 定义W5500的MISO管脚          */
#define WIZ_SPIx_MOSI_PIN                GPIO_PIN_7						   	    /* 定义W5500的MOSI管脚          */

#define WIZ_SPIx_SCS_PIN                 GPIO_PIN_4						   	  /* 定义W5500的片选管脚          */
#define WIZ_SPIx_SCS_PORT                GPIOA									   	  /* GPIO端口                     */
#define WIZ_SPIx_SCS_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()	/* GPIO端口时钟                 */

#define WIZ_INT_PIN                      GPIO_PIN_4									  /* 定义W5500的INT管脚           */
#define WIZ_INT_PORT                     GPIOH								  	   /* GPIO端口                     */
#define WIZ_INT_CLK_ENABLE()             __HAL_RCC_GPIOH_CLK_ENABLE() /* GPIO端口时钟                 */

#define WIZ_RESET_PIN                   GPIO_PIN_5									  /* 定义W5500的RESET管脚           */
#define WIZ_RESET_PORT                  GPIOH								  	    /* GPIO端口                     */
#define WIZ_RESET_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()  /* GPIO端口时钟 */


#define WIZ_SPIx_SCS_ON()     HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS_PIN,GPIO_PIN_RESET)  /*选中芯片*/
#define WIZ_SPIx_SCS_OFF()   HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS_PIN,GPIO_PIN_SET)   //不选中芯片

/***************----- 网络参数变量定义 -----***************/
extern unsigned char Gateway_IP[4];	//网关IP地址 
extern unsigned char Sub_Mask[4];	//子网掩码 
extern unsigned char Phy_Addr[6];	//物理地址(MAC) 
extern unsigned char IP_Addr[4];	//本机IP地址 



extern unsigned char S0_Port[2];	//端口0的端口号(5000) 
extern unsigned char S0_DIP[4];		//端口0目的IP地址 
extern unsigned char S0_DPort[2];	//端口0目的端口号(6000) 

extern unsigned char UDP_DIPR[4];	//UDP(广播)模式,目的主机IP地址
extern unsigned char UDP_DPORT[2];	//UDP(广播)模式,目的主机端口号



extern void W5500_STM32(void );
extern unsigned char W5500_SPI_ReadWrite(unsigned char data);
extern void W5500_Reset(void);
extern unsigned char  SPI_SendWrite_Byte(unsigned char TxData);
extern void W5500_Write(unsigned int address,unsigned char data);
extern unsigned char W5500_Read(unsigned int address);
extern unsigned short W5500_Read_nByte(unsigned int address,unsigned char* data,unsigned short len);
extern unsigned short W5500_Write_nByte(unsigned int address,unsigned char* data,unsigned short len);

extern void IINCHIP_WRITE( unsigned long addrbsb,  unsigned char data);
extern unsigned char IINCHIP_READ(unsigned long addrbsb);
extern unsigned short wiz_write_buf(unsigned long addrbsb,unsigned char* buf,unsigned short len);
extern unsigned short wiz_read_buf(unsigned long addrbsb, unsigned char* buf,unsigned short len);
extern void W5500_Start(void);



#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
