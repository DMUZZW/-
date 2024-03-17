#include "LOT.h"
/***
这个文件用于编写控制类指令
程序流程：接收上位机发送的控制指令：对控制指令进行解读
(1) 起始字符是否满足协议
(2) 判断帧长度
(3) 判断校验码
(4) 判断功能码

**/


//低功耗模式
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
//	if(Rx_Buffer[2] ==0x84)//功能码正确
//	{
		//所有输出变为0.因为输出采用多个端口，故采用这种方式处理
	 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE置1
   HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG置1
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


//软件复位实现代码
void SoftRestart(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();	
}

//复位指令
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
//	if(Rx_Buffer[2] == 0x83)//功能码正确
//	{
		IP_Lable[0] = 0x01;
		AT24CXX_Write(35,&IP_Lable[0],1);//写入标志
		AT24CXX_Read(35,&IP_Lable[1],1); //读出标志
		if(IP_Lable[1] == 0x01)//如果标志正确
		{
					//所有输出变为0.因为输出采用多个端口，故采用这种方式处理
	   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE置1
       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG置1
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

//板卡授时与交互 
void TimeService_Temperature_Humidity(void)
{
//	unsigned char FrameLength=0x00;
	unsigned char year,mon,date,week,hour,min,second;
	unsigned char SHT31_DATA[4];//定义温度和湿度
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
		 //调用RTC日期设置函数  
		 Set_Board_RTC_Date(year,mon,date,week);
		 //RTC_HOURFORMAT12_PM
		 //设置RTC时间
		 Set_Board_RTC_Time(hour,min,second,RTC_HOURFORMAT12_PM);

		//如果设置成功，则将温度、湿度等信息返回  没有设置温度，湿度等，将原信息返回
		if((Set_Board_RTC_Date(year,mon,date,week)==HAL_OK)&(Set_Board_RTC_Time(hour,min,second,RTC_HOURFORMAT12_PM)==HAL_OK))
		{
			    memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
      		HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
		      HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
  			  Tx_Buffer[0] = 0xfe;
		      Tx_Buffer[1] = 0x0e;
			    Tx_Buffer[2] = 0x82;
			    Tx_Buffer[3] = (unsigned char)(DataExchangeNum >>8);//一分钟内的数据交互量
			    Tx_Buffer[4] = (unsigned char)(DataExchangeNum & 0x00ff);//一分钟内的数据交互量
			   //4和5是用来发送温度的
    			Tx_Buffer[5] = SHT31_DATA[0];
			    Tx_Buffer[6] = SHT31_DATA[1];
			    //6和7是用来发送湿度
			    Tx_Buffer[7] = SHT31_DATA[2];
			    Tx_Buffer[8] = SHT31_DATA[3];
			    //8和9 是发送固件版本的
			    Tx_Buffer[9] = Software_Version[0];
			    Tx_Buffer[10] = Software_Version[1];
			
			    //是否存在SD卡
			    Tx_Buffer[11]= SD_Check();
			
          //异或校验			   
		      Tx_Buffer[12]= GetXor(&Tx_Buffer[1],8);
			    Tx_Buffer[13] = 0xff;
			    sendto(SOCK_UDPS,Tx_Buffer,14, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500把接收到的数据发送给Remote*/
          DataSent = TRUE; 
					// 写入SD卡 	
				  if(1==SD_Check())
					SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
				  DataExchangeNum =0x00;//把计数量归零			 
		}		
//	}	
}

//固件升级 FlASH区域选择
//应用程序首先是在第一个APP FLASH中运行，若需要更新，则需要更新在第二个 APP FLASH 如还需要更新，则是在第一个APP FLASH区域 依次类推
void FirmwareUpdate_Flash_Choose(void)
{
	
//	unsigned char XorTemp=0x00;//,FrameLength=0x00
	unsigned char APP_Flash_New = 0x00,APP_Flash_Old=0x00;//APP_Flash_New用于指示新固件的烧写位置PP_Flash_Old用于指示现运行代码的区域
	unsigned char App_Flash_Temp = 0x00;//暂存
//	if(Rx_Buffer[0] != 0xfe)
//	{
//	 //是否发送到H端
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
		if(APP_Flash_New ==APP_Flash_Old)//两个区域相同，提醒换一个区域
		{
			Tx_Buffer[7] = 0xBB;//
			Tx_Buffer[8] = APP_Flash_New;
		}
		else if(APP_Flash_New != APP_Flash_Old)//如果区域不同。说明可以烧录
		{
			AT24CXX_Write(33,&APP_Flash_New,1);//
			AT24CXX_Read(33,&App_Flash_Temp,1);
			if(APP_Flash_New ==App_Flash_Temp)//写入成功
			{
				Tx_Buffer[7] = 0xAA;//
				Tx_Buffer[8] = APP_Flash_New;
				
			}
			else//写入失败
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
		sendto(SOCK_UDPS,Tx_Buffer,11, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500把接收到的数据发送给Remote*/
    DataSent = TRUE; 
		DataExchangeNum=DataExchangeNum+1;//记录发送数据的次数	
    		//写入SD卡
		if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);		
//	}
	
}

//模拟量采集通道设置
void AnalogDataInput_Channel_Setting(unsigned short pointer)
{
	unsigned char AnalogDataInputSign,temp;

	AnalogDataInputSign = Rx_Buffer[pointer+3];
//	if(Rx_Buffer[2] ==0x88)//功能码正确
//	{
			//写入AT24C02中的34
		 AT24CXX_Write(34,&AnalogDataInputSign,1);
		  delay_ms(50);
		  AT24CXX_Read(34,&temp,1);
		  memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
		 if(AnalogDataInputSign == temp)//设置成功
			{
				Tx_Buffer[0] = 0xfe; 
				Tx_Buffer[1] = 0x07;
				Tx_Buffer[2] = 0x88;
				Tx_Buffer[3] = AnalogDataInputSign;
				Tx_Buffer[4] = 0xaa;
				Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
			  Tx_Buffer[6] = 0xff;				
			}
			else//设置不成功
			{
				Tx_Buffer[0] = 0xfe;
				Tx_Buffer[1] = 0x07;
				Tx_Buffer[2] = 0x88;
				Tx_Buffer[3] = AnalogDataInputSign;
				Tx_Buffer[4] = 0x55;
				Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
			  Tx_Buffer[6] = 0xff;				
			}			
		 sendto(SOCK_UDPS,Tx_Buffer,7, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500把接收到的数据发送给Remote*/
     DataSent = TRUE; 
			DataExchangeNum=DataExchangeNum+1;//记录发送数据的次数	
			// 写入SD卡
	   if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);		
//	}	
}


//心跳测试
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
		sendto(SOCK_UDPS,Tx_Buffer,5, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);                /*W5500把接收到的数据发送给Remote*/ 
	  DataSent = TRUE; 
		DataExchangeNum=DataExchangeNum+1;//记录发送数据的次数	
		//	写入SD
		if(1==SD_Check())
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
//	}
	
}


//板卡运行时间记录
//启动时开始记录进入SD卡中




