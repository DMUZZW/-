#ifndef __SPI_W5500_H
#define __SPI_W5500_H



#include "sys.h"
#include "delay.h"
#include "W5500.h"
#include "24cxx.h" 
#include "define.h"


/**/


/*����SPI��ΪW5500��Ӳ���ӿ�*/
#define WIZ_SPIx                         SPI1                   	     /* ����W5500���õ�SPI�ӿ�       */
#define WIZ_SPIx_RCC_CLK_ENABLE()        __HAL_RCC_SPI1_CLK_ENABLE()   /* ����W5500���õ�SPI�ӿ�ʱ��   */

#define WIZ_SPI_GPIO_ClK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() /* GPIO�˿�ʱ��                 */
#define WIZ_SPIx_GPIO_PORT               GPIOA									  	  /* GPIO�˿�                     */
#define WIZ_SPIx_SCLK_PIN                GPIO_PIN_5						   	    /* ����W5500��ʱ�ӹܽ�          */
#define WIZ_SPIx_MISO_PIN                GPIO_PIN_6						   	    /* ����W5500��MISO�ܽ�          */
#define WIZ_SPIx_MOSI_PIN                GPIO_PIN_7						   	    /* ����W5500��MOSI�ܽ�          */

#define WIZ_SPIx_SCS_PIN                 GPIO_PIN_4						   	  /* ����W5500��Ƭѡ�ܽ�          */
#define WIZ_SPIx_SCS_PORT                GPIOA									   	  /* GPIO�˿�                     */
#define WIZ_SPIx_SCS_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()	/* GPIO�˿�ʱ��                 */

#define WIZ_INT_PIN                      GPIO_PIN_4									  /* ����W5500��INT�ܽ�           */
#define WIZ_INT_PORT                     GPIOH								  	   /* GPIO�˿�                     */
#define WIZ_INT_CLK_ENABLE()             __HAL_RCC_GPIOH_CLK_ENABLE() /* GPIO�˿�ʱ��                 */

#define WIZ_RESET_PIN                   GPIO_PIN_5									  /* ����W5500��RESET�ܽ�           */
#define WIZ_RESET_PORT                  GPIOH								  	    /* GPIO�˿�                     */
#define WIZ_RESET_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()  /* GPIO�˿�ʱ�� */


#define WIZ_SPIx_SCS_ON()     HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS_PIN,GPIO_PIN_RESET)  /*ѡ��оƬ*/
#define WIZ_SPIx_SCS_OFF()   HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS_PIN,GPIO_PIN_SET)   //��ѡ��оƬ

/***************----- ��������������� -----***************/
extern unsigned char Gateway_IP[4];	//����IP��ַ 
extern unsigned char Sub_Mask[4];	//�������� 
extern unsigned char Phy_Addr[6];	//�����ַ(MAC) 
extern unsigned char IP_Addr[4];	//����IP��ַ 



extern unsigned char S0_Port[2];	//�˿�0�Ķ˿ں�(5000) 
extern unsigned char S0_DIP[4];		//�˿�0Ŀ��IP��ַ 
extern unsigned char S0_DPort[2];	//�˿�0Ŀ�Ķ˿ں�(6000) 

extern unsigned char UDP_DIPR[4];	//UDP(�㲥)ģʽ,Ŀ������IP��ַ
extern unsigned char UDP_DPORT[2];	//UDP(�㲥)ģʽ,Ŀ�������˿ں�



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
