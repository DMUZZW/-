#include "udp_demo.h"

/**
*@brief		UDP测试程序
*@param		无
*@return	无
*/
unsigned char Send_IP_Label=0x00;//发送IP数据标签，用于复位后发送一次IP地址

void do_udp(void)
{                                                              
    unsigned short len=0,datalength=0,i=0;
	  unsigned char FunctionSign = 0x00;
    unsigned char XorTemp=0x00,FrameLength=0x00,FrameEnd=0x00;//

	  switch(getSn_SR(SOCK_UDPS))                                                /*获取socket的状态 通过读取端口Sn_SR寄存器 */
	  {
		   case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
			      socket(SOCK_UDPS,Sn_MR_UDP,W5500_Parameter_Configuration.Sn_UDP_Port,0);                              /*初始化socket*/
		   break;
		
		  case SOCK_UDP:                                                           /*socket初始化完成*/
		       Connect_OK = TRUE;//连接标志
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*清接收中断*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                    /*接收到数据*/
			{				
			  datalength = recvfrom(SOCK_UDPS,Rx_Buffer, len, W5500_Parameter_Configuration.REMOTE_Sn_DIP,&W5500_Parameter_Configuration.Sn_UDP_DPort);               /*W5500接收计算机发送来的数据*/		
			  DataReceived = TRUE;//数据接收处理完成
				for(i=0;i<datalength;i++)//遍历接收到的数据
				{
					if(Rx_Buffer[i] == 0xfe) //判断起始字符
					{
						//获取帧长度
						FrameLength = Rx_Buffer[i+1];//得到帧长度						
						//获取功能码
						FunctionSign = Rx_Buffer[i+2];
						//获取校验码
						XorTemp = GetXor(&Rx_Buffer[i+1],FrameLength-4);
						//获取结束字符
						FrameEnd = Rx_Buffer[i+FrameLength-1];						
						//判断功能码是否正确
						if((FunctionSign == 0x02)||(FunctionSign == 0x03)||(FunctionSign ==0x81)||(FunctionSign==0x82)||(FunctionSign==0x83)||(FunctionSign == 0x84)
				    ||(FunctionSign==0x85)||(FunctionSign==0x86)||(FunctionSign==0x88)	||(FunctionSign==0x89))
						{
							//判断校验码
//							if(1)
							if(XorTemp == Rx_Buffer[i+FrameLength-2])
							{
								//判断结束符
								if(0xff ==FrameEnd)
								{
											//接收到的数据是正确的 记录在SD卡中
											if(1==SD_Check())SD_Creat_CSVFile(1,Rx_Buffer,SendandReceiveTimes++);
									    switch(FunctionSign)
													{
													  case 0x01:
														  DigitalAnalogData_Compare(i);
														  IWDG_Feed(); //喂狗	
														break;
														case 0x02:
														  DigitalData_AnalogData_QueryState();//查询指令
														  IWDG_Feed(); //喂狗
														break;
														case 0x03:
														  DigitalData_AnalogData_OutputProcess(i);//模拟量、数字量输出处理函数,应设置一个调试模式
														  IWDG_Feed(); //喂狗
														break;
														case 0x81:
														  BoardCast();
														  IWDG_Feed(); //喂狗
														break;
														case 0x82://授时
														  TimeService_Temperature_Humidity();
														  IWDG_Feed(); //喂狗
														break;
														case 0x83://复位指令
														  Restart_instructions();
															IWDG_Feed(); //喂狗
															break;
														case 0x84://低功耗
															Low_Power_Consumption();
															IWDG_Feed(); //喂狗
															break;
														case 0x85://网络参数设置
															NetParameter_Setting(i);
															IWDG_Feed(); //喂狗
															break;
														case 0x86://固件程序升级位置选择
															if(FrameLength == 9)//从上位机获取固件代码大小
															{
																	W5500_Parameter_Configuration.FirmwareLength[0] = Rx_Buffer[3];
																	W5500_Parameter_Configuration.FirmwareLength[1] = Rx_Buffer[4];
																	W5500_Parameter_Configuration.FirmwareLength[2] = Rx_Buffer[5];
																	W5500_Parameter_Configuration.FirmwareLength[3] = Rx_Buffer[6];
																	//写入到AT24C04中
																 AT24CXX_Write(35,W5500_Parameter_Configuration.FirmwareLength,4);  //把上述网络参数写入到AT24C04中
																	//擦除备份区原有资料
																 STMFLASH_Erase(BACKUP_ADDRESS);//APP程序是擦除备份区，因为接收的文件是放在备份区
//																 STMFLASH_Erase(APP_ADDRESS);	//boot程序是擦除APP_ADDRESS，接收的文件是放在APP_ADDRESS区
																//sendto(SOCK_UDPS,Rx_Buffer,9, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort); //发送到上位机	
																}
															break;
														case 0x87://板卡故障状态返回						
															break;
														case 0x88://模拟量采集通道设置
															AnalogDataInput_Channel_Setting(i);
															IWDG_Feed(); //喂狗
															break;
														case 0x89:
															heart_beat_test();
															IWDG_Feed(); //喂狗
														default:
															break;
													}	
													IWDG_Feed();//喂狗									
								}
								else
								{
									
								}
							}
							else
                {
									
								}
						}
            else
               {
								 
							 }						
					}					
				}				
			}
			break;
	}
}

