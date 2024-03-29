#include "Flash.h"

//读取指定地址的字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(__IO uint32_t *)faddr; 
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	return FLASH_SECTOR_7;	
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F7的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FF0F000~0X1FF0F41F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	//这个程序将数据写入到指定的FLASH里面
{ 
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;
	u32 addrx=0;
	u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    
 	HAL_FLASH_Unlock();             //解锁	
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
    //擦除操作
    if(addrx<0X1FF00000)
    {
        while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
                FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
                {
                    break;//发生错误了	
                }
                SCB_CleanInvalidateDCache();                            //清除无效的D-Cache
			}else addrx+=4;
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
        }
    }
	//写入操作
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
	if(FlashStatus==HAL_OK)
	{
		while(WriteAddr<endaddr)//写数据
		{
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	HAL_FLASH_Lock();           //上锁
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	  //从指定地址中读出FLASH代码的数据
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}
// flash 擦除扇区程序
unsigned char STMFLASH_Erase(u32 addrx) 
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	u32 SectorError=0;
	u32 state=0;
	HAL_FLASH_Unlock();             //解锁	
	if(addrx<0X1FF00000) //不能擦除
	{
		if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
		  {
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
				FlashEraseInit.NbSectors= 1;                             //一次只擦除一个扇区
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) !=HAL_OK) //擦除出错
				{
				  return 0; //返回0  擦除失败 	
				}
				 SCB_CleanInvalidateDCache();                            //清除无效的D-Cache		
		  }
		 else
		{
		 addrx+=4;
		}
	}
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成	
	return 1; //擦除成功	
}

//flash 写入函数
void FLASH_Program(unsigned int WriteAddr,unsigned int *pBuffer,unsigned int NumToWrite)
{
	  u32 addrx=0;
	  u32 endaddr=0;    	
	  HAL_StatusTypeDef FlashStatus =HAL_OK;
	
	 if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
		//写入操作
	
      //解锁	
   	 HAL_FLASH_Unlock(); 
	 addrx=WriteAddr;                   //起始地址
     endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
	  //接下来判断要写入的地址中的内容是否为FF
     if(STMFLASH_ReadWord(WriteAddr)==0XFFFFFFFF)
	 {
		 FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
		 if(FlashStatus==HAL_OK)
		 {
			 //写入地址
			while(WriteAddr<endaddr)
			{
				if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addrx,*pBuffer) ==HAL_OK)
				{
						WriteAddr+=4;
				        pBuffer++;
				}
				else return;
				
			}				
			 
		 } else return;
      		 
	 }
	 else 
	 {
		return ;
	 }
	 HAL_FLASH_Lock();           //上锁	
}

//从Flash的一个区域拷贝到另一个区域
void Flash_Copy(unsigned int WriteAddr,unsigned int SourceAddress,unsigned int NumToWrite)
{
	unsigned int i,temp=0x00000000;
	
	STMFLASH_Erase(WriteAddr);//擦除app区
	for(i=0;i<NumToWrite;i+=4)
	{
		temp = *(uint32_t*)SourceAddress;
		FLASH_Program(WriteAddr,&temp,1);
		SourceAddress = SourceAddress+4;
		WriteAddr = WriteAddr+4;
	}
}




















































































