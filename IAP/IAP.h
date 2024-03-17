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
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
 
extern void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
extern void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
extern void W5500_IAP_test(void);

//定义固件存储扇区 BootLoader程序存放在扇区0和扇区1里面
//固件1 存放在 扇区2-3
//固件2 存放在扇区 4
//固件3 存放在扇区5
//固件4 存放在扇区6


//enum APP_ADDR {FLASH_APP1_ADDR,FLASH_APP2_ADDR,FLASH_APP3_ADDR,FLASH_APP4_ADDR};
//unsigned long APP_ADDR[4]={FLASH_APP1_ADDR,FLASH_APP2_ADDR,FLASH_APP3_ADDR,FLASH_APP4_ADDR};

#endif


