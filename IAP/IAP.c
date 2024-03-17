#include "iap.h" 

typedef  void (*pFunction)(void);
/*
IAP 实现步骤
(1)通过W5500从上位机接收固件代码
(2)通过标识位读取固件代码的版本，如果是最新版本，则调用写FLASH函数，将固件代码写入到指定的FLASH里面
(3)Flas分成5个区域，一共存储5个固件代码，5个代码都需要可以接收上位机传送过来的新固件并写入到FLASH中，并把固件版本写入到AT24C02中，
(4)上位机可以选择固件代码存储区域，可以选择不同的版本进行执行，显示当前运行固件是哪个版本
*/


iapfun jump2app; 
unsigned int iapbuf[512]; 	//2K字节缓存 

//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize) ///这个函数用于将存放在串口接收buff里面的APP程序写入到flash里面
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(u32)dfu[3]<<24;   
		temp|=(u32)dfu[2]<<16;    
		temp|=(u32)dfu[1]<<8;
		temp|=(u32)dfu[0];	  
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
	} 
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr) //用于跳转到APP程序中运行
{ 
	if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}		 





