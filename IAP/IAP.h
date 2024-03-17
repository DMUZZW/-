#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  
#include "Flash.h"
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "SPI_W5500.h"
#include "socket.h"
#include "define.h"
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.   
 
extern void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
extern void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin
extern void W5500_IAP_test(void);

//����̼��洢���� BootLoader������������0������1����
//�̼�1 ����� ����2-3
//�̼�2 ��������� 4
//�̼�3 ���������5
//�̼�4 ���������6


//enum APP_ADDR {FLASH_APP1_ADDR,FLASH_APP2_ADDR,FLASH_APP3_ADDR,FLASH_APP4_ADDR};
//unsigned long APP_ADDR[4]={FLASH_APP1_ADDR,FLASH_APP2_ADDR,FLASH_APP3_ADDR,FLASH_APP4_ADDR};

#endif


