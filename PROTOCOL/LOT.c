#include "LOT.h"
/***
����ļ����ڱ�д������ָ��
�������̣�������λ�����͵Ŀ���ָ��Կ���ָ����н��
(1) ��ʼ�ַ��Ƿ�����Э��
(2) �ж�֡����
(3) �ж�У����
(4) �жϹ�����

**/


//�͹���ģʽ
void Low_Power_Consumption (void)
{
//	unsigned char XorTemp=0x00,FrameLength=0x00;
//	if(Rx_Buffer[0] != 0xfe)
//	{ 
//		return ;	
//	}		
//	FrameLength = Rx_Buffer[1];
//	if(FrameLength != 0X05)
//  {
//			return;
//	}	
//	XorTemp = GetXor(&Rx_Buffer[1],1);
//	if(XorTemp != Rx_Buffer[3])
//	{
//		return;
//	}
//	if(Rx_Buffer[2] ==0x84)//��������ȷ
//	{
		//���������Ϊ0.��Ϊ������ö���˿ڣ��ʲ������ַ�ʽ����
	 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE��1
   HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG��1
   SPI2_Send3Byte(0x00300000);
	 SPI2_Send3Byte(0x00310000);
	 SPI2_Send3Byte(0x00320000);
	 SPI2_Send3Byte(0x00330000);
	 SPI2_Send3Byte(0x00340000);
	 SPI2_Send3Byte(0x00350000);
	 SPI2_Send3Byte(0x00360000);
	 SPI2_Send3Byte(0x00370000);				
//	}	
}


//�����λʵ�ִ���
void SoftRestart(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();	
}

//��λָ��
void Restart_instructions (void)
{
//	unsigned char XorTemp=0x00,FrameLength=0x00;
	unsigned char IP_Lable[2] = {0x00,0x00};
//	if(Rx_Buffer[0] != 0xfe) 
//	{
//		return;
//	}
//	FrameLength = Rx_Buffer[1];
//	if(FrameLength != 0x05)
//	{
//		return ;
//	}
//	if(Rx_Buffer[FrameLength-1] != 0xff)
//	{
//	 return;	
//	}
//	XorTemp = GetXor(&Rx_Buffer[1],1);
//	if(XorTemp != Rx_Buffer[3])
//	{
//		return;
//	}
//	if(Rx_Buffer[2] == 0x83)//��������ȷ
//	{
		IP_Lable[0] = 0x01;
		AT24CXX_Write(35,&IP_Lable[0],1);//д���־
		AT24CXX_Read(35,&IP_Lable[1],1); //������־
		if(IP_Lable[1] == 0x01)//�����־��ȷ
		{
					//���������Ϊ0.��Ϊ������ö���˿ڣ��ʲ������ַ�ʽ����
	   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE��1
       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG��1
       SPI2_Send3Byte(0x00300000);
	   SPI2_Send3Byte(0x00310000);
	   SPI2_Send3Byte(0x00320000);
	   SPI2_Send3Byte(0x00330000);
	   SPI2_Send3Byte(0x00340000);
	   SPI2_Send3Byte(0x00350000);
	   SPI2_Send3Byte(0x00360000);
	   SPI2_Send3Byte(0x00370000);
			
			SoftRestart();
		}		
//	}	
}

//�忨��ʱ�뽻�� 
void TimeService_Temperature_Humidity(void)
{
//	unsigned char FrameLength=0x00;
	unsigned char year,mon,date,week,hour,min,second;
	unsigned char SHT31_DATA[4];//�����¶Ⱥ�ʪ��
	Temperature_Humidity(2,SHT31_DATA);
//	if(Rx_Buffer[2] == 0x82)
//	{
		 year = Rx_Buffer[3];
		 mon  = Rx_Buffer[4];
		 date = Rx_Buffer[5];
		 week = Rx_Buffer[6];;
		 hour = Rx_Buffer[7];
		 min  = Rx_Buffer[8];
		 second = Rx_Buffer[9];
		 //����RTC�������ú���  
		 Set_Board_RTC_Date(year,mon,date,week);
		 //RTC_HOURFORMAT12_PM
		 //����RTCʱ��
		 Set_Board_RTC_Time(hour,min,second,RTC_HOURFORMAT12_PM);

		//������óɹ������¶ȡ�ʪ�ȵ���Ϣ����  û�������¶ȣ�ʪ�ȵȣ���ԭ��Ϣ����
		if((Set_Board_RTC_Date(year,mon,date,week)==HAL_OK)&(Set_Board_RTC_Time(hour,min,second,RTC_HOURFORMAT12_PM)==HAL_OK))
		{
			    memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
      		HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
		      HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
  			  Tx_Buffer[0] = 0xfe;
		      Tx_Buffer[1] = 0x0e;
			    Tx_Buffer[2] = 0x82;
			    Tx_Buffer[3] = (unsigned char)(DataExchangeNum >>8);//һ�����ڵ����ݽ�����
			    Tx_Buffer[4] = (unsigned char)(DataExchangeNum & 0x00ff);//һ�����ڵ����ݽ�����
			   //4��5�����������¶ȵ�
    			Tx_Buffer[5] = SHT31_DATA[0];
			    Tx_Buffer[6] = SHT31_DATA[1];
			    //6��7����������ʪ��
			    Tx_Buffer[7] = SHT31_DATA[2];
			    Tx_Buffer[8] = SHT31_DATA[3];
			    //8��9 �Ƿ��͹̼��汾��
			    Tx_Buffer[9] = Software_Version[0];
			    Tx_Buffer[10] = Software_Version[1];
			
			    //�Ƿ����SD��
			    Tx_Buffer[11]= SD_Check();
			
          //���У��			   
		      Tx_Buffer[12]= GetXor(&Tx_Buffer[1],8);
			    Tx_Buffer[13] = 0xff;
			    sendto(SOCK_UDPS,Tx_Buffer,14, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
          DataSent = TRUE; 
					// д��SD�� 	
				  if(1==SD_Check())
					SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
				  DataExchangeNum =0x00;//�Ѽ���������			 
		}		
//	}	
}

//�̼����� FlASH����ѡ��
//Ӧ�ó����������ڵ�һ��APP FLASH�����У�����Ҫ���£�����Ҫ�����ڵڶ��� APP FLASH �绹��Ҫ���£������ڵ�һ��APP FLASH���� ��������
void FirmwareUpdate_Flash_Choose(void)
{
	
//	unsigned char XorTemp=0x00;//,FrameLength=0x00
	unsigned char APP_Flash_New = 0x00,APP_Flash_Old=0x00;//APP_Flash_New����ָʾ�¹̼�����дλ�ãAPP_Flash_Old����ָʾ�����д��������
	unsigned char App_Flash_Temp = 0x00;//�ݴ�
//	if(Rx_Buffer[0] != 0xfe)
//	{
//	 //�Ƿ��͵�H��
//		return;
//	}		
//	FrameLength = Rx_Buffer[1];
//	if(FrameLength != 0x06)
//	{
//		return ;
//	}
//	if(Rx_Buffer[FrameLength-1] != 0xff)
//	{
//		return;
//	}
//	XorTemp = GetXor(&Rx_Buffer[1],2);
//	if(XorTemp != Rx_Buffer[4])
//	{
//		return;
//	}
//	if(Rx_Buffer[2] == 0x86)
//	{
		 memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
		 APP_Flash_New = Rx_Buffer[3];
		 AT24CXX_Read(33,&APP_Flash_Old,1);
		if(APP_Flash_New ==APP_Flash_Old)//����������ͬ�����ѻ�һ������
		{
			Tx_Buffer[7] = 0xBB;//
			Tx_Buffer[8] = APP_Flash_New;
		}
		else if(APP_Flash_New != APP_Flash_Old)//�������ͬ��˵��������¼
		{
			AT24CXX_Write(33,&APP_Flash_New,1);//
			AT24CXX_Read(33,&App_Flash_Temp,1);
			if(APP_Flash_New ==App_Flash_Temp)//д��ɹ�
			{
				Tx_Buffer[7] = 0xAA;//
				Tx_Buffer[8] = APP_Flash_New;
				
			}
			else//д��ʧ��
			{
				Tx_Buffer[7] = 0x55;//
				Tx_Buffer[8] = App_Flash_Temp;
			}		
		}
		Tx_Buffer[0] = 0xfe;
		Tx_Buffer[1] = 0x0b;
	  Tx_Buffer[2] = 0x86;
		Tx_Buffer[3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0];
		Tx_Buffer[4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1];
		Tx_Buffer[5] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2];
		Tx_Buffer[6] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3];		
		Tx_Buffer[9] = GetXor(&Tx_Buffer[1],7);
		Tx_Buffer[10] = 0xff;
		sendto(SOCK_UDPS,Tx_Buffer,11, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
    DataSent = TRUE; 
		DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
    		//д��SD��
		if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);		
//	}
	
}

//ģ�����ɼ�ͨ������
void AnalogDataInput_Channel_Setting(unsigned short pointer)
{
	unsigned char AnalogDataInputSign,temp;

	AnalogDataInputSign = Rx_Buffer[pointer+3];
//	if(Rx_Buffer[2] ==0x88)//��������ȷ
//	{
			//д��AT24C02�е�34
		 AT24CXX_Write(34,&AnalogDataInputSign,1);
		  delay_ms(50);
		  AT24CXX_Read(34,&temp,1);
		  memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
		 if(AnalogDataInputSign == temp)//���óɹ�
			{
				Tx_Buffer[0] = 0xfe; 
				Tx_Buffer[1] = 0x07;
				Tx_Buffer[2] = 0x88;
				Tx_Buffer[3] = AnalogDataInputSign;
				Tx_Buffer[4] = 0xaa;
				Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
			  Tx_Buffer[6] = 0xff;				
			}
			else//���ò��ɹ�
			{
				Tx_Buffer[0] = 0xfe;
				Tx_Buffer[1] = 0x07;
				Tx_Buffer[2] = 0x88;
				Tx_Buffer[3] = AnalogDataInputSign;
				Tx_Buffer[4] = 0x55;
				Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
			  Tx_Buffer[6] = 0xff;				
			}			
		 sendto(SOCK_UDPS,Tx_Buffer,7, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
     DataSent = TRUE; 
			DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
			// д��SD��
	   if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);		
//	}	
}


//��������
void  heart_beat_test(void)
{
//	unsigned char XorTemp=0x00;
//	FrameLength = Rx_Buffer[1];
//	XorTemp = GetXor(&Rx_Buffer[1],1);
//	if(FrameLength != 0x05)
//	{
//		return ;
//	}	
//	if(XorTemp != Rx_Buffer[3])
//	{
//		return;
//	}	
//	if(Rx_Buffer[0] != 0xfe) 
//	{
//		return ;
//	}
//	if(Rx_Buffer[4] != 0xff)
//	{
//		return;
//	}	
//	if(Rx_Buffer[2] ==0x89)
//	{
		memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
		Tx_Buffer[0] = 0xfe;
		Tx_Buffer[1] = 0x05;
		Tx_Buffer[2] = 0x89;
		Tx_Buffer[3] = GetXor(&Tx_Buffer[1],1);;
		Tx_Buffer[4] = 0xff;
		sendto(SOCK_UDPS,Tx_Buffer,5, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500�ѽ��յ������ݷ��͸�Remote*/ 
	  DataSent = TRUE; 
		DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
		//	д��SD
		if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
//	}
	
}


//�忨����ʱ���¼
//����ʱ��ʼ��¼����SD����




