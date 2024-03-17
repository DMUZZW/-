#include "Flash.h"

//��ȡָ����ַ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(__IO uint32_t *)faddr; 
}

//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
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

//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F7������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FF0F000~0X1FF0F41F
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	//�����������д�뵽ָ����FLASH����
{ 
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;
	u32 addrx=0;
	u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
    
 	HAL_FLASH_Unlock();             //����	
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
    //��������
    if(addrx<0X1FF00000)
    {
        while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //Ҫ����������
                FlashEraseInit.NbSectors=1;                             //һ��ֻ����һ������
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
                {
                    break;//����������	
                }
                SCB_CleanInvalidateDCache();                            //�����Ч��D-Cache
			}else addrx+=4;
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������
        }
    }
	//д�����
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //�ȴ��ϴβ������
	if(FlashStatus==HAL_OK)
	{
		while(WriteAddr<endaddr)//д����
		{
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	HAL_FLASH_Lock();           //����
} 

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	  //��ָ����ַ�ж���FLASH���������
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}
// flash ������������
unsigned char STMFLASH_Erase(u32 addrx) 
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	u32 SectorError=0;
	u32 state=0;
	HAL_FLASH_Unlock();             //����	
	if(addrx<0X1FF00000) //���ܲ���
	{
		if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
		  {
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //Ҫ����������
				FlashEraseInit.NbSectors= 1;                             //һ��ֻ����һ������
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) !=HAL_OK) //��������
				{
				  return 0; //����0  ����ʧ�� 	
				}
				 SCB_CleanInvalidateDCache();                            //�����Ч��D-Cache		
		  }
		 else
		{
		 addrx+=4;
		}
	}
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������	
	return 1; //�����ɹ�	
}

//flash д�뺯��
void FLASH_Program(unsigned int WriteAddr,unsigned int *pBuffer,unsigned int NumToWrite)
{
	  u32 addrx=0;
	  u32 endaddr=0;    	
	  HAL_StatusTypeDef FlashStatus =HAL_OK;
	
	 if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
		//д�����
	
      //����	
   	 HAL_FLASH_Unlock(); 
	 addrx=WriteAddr;                   //��ʼ��ַ
     endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
	  //�������ж�Ҫд��ĵ�ַ�е������Ƿ�ΪFF
     if(STMFLASH_ReadWord(WriteAddr)==0XFFFFFFFF)
	 {
		 FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //�ȴ��ϴβ������
		 if(FlashStatus==HAL_OK)
		 {
			 //д���ַ
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
	 HAL_FLASH_Lock();           //����	
}

//��Flash��һ�����򿽱�����һ������
void Flash_Copy(unsigned int WriteAddr,unsigned int SourceAddress,unsigned int NumToWrite)
{
	unsigned int i,temp=0x00000000;
	
	STMFLASH_Erase(WriteAddr);//����app��
	for(i=0;i<NumToWrite;i+=4)
	{
		temp = *(uint32_t*)SourceAddress;
		FLASH_Program(WriteAddr,&temp,1);
		SourceAddress = SourceAddress+4;
		WriteAddr = WriteAddr+4;
	}
}



















































































