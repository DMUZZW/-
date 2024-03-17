#include "udp_demo.h"

/**
*@brief		UDP���Գ���
*@param		��
*@return	��
*/
unsigned char Send_IP_Label=0x00;//����IP���ݱ�ǩ�����ڸ�λ����һ��IP��ַ

void do_udp(void)
{                                                              
    unsigned short len=0,datalength=0,i=0;
	  unsigned char FunctionSign = 0x00;
    unsigned char XorTemp=0x00,FrameLength=0x00,FrameEnd=0x00;//

	  switch(getSn_SR(SOCK_UDPS))                                                /*��ȡsocket��״̬ ͨ����ȡ�˿�Sn_SR�Ĵ��� */
	  {
		   case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			      socket(SOCK_UDPS,Sn_MR_UDP,W5500_Parameter_Configuration.Sn_UDP_Port,0);                              /*��ʼ��socket*/
		   break;
		
		  case SOCK_UDP:                                                           /*socket��ʼ�����*/
		       Connect_OK = TRUE;//���ӱ�־
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*������ж�*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                    /*���յ�����*/
			{				
			  datalength = recvfrom(SOCK_UDPS,Rx_Buffer, len, W5500_Parameter_Configuration.REMOTE_Sn_DIP,&W5500_Parameter_Configuration.Sn_UDP_DPort);               /*W5500���ռ����������������*/		
			  DataReceived = TRUE;//���ݽ��մ������
				for(i=0;i<datalength;i++)//�������յ�������
				{
					if(Rx_Buffer[i] == 0xfe) //�ж���ʼ�ַ�
					{
						//��ȡ֡����
						FrameLength = Rx_Buffer[i+1];//�õ�֡����						
						//��ȡ������
						FunctionSign = Rx_Buffer[i+2];
						//��ȡУ����
						XorTemp = GetXor(&Rx_Buffer[i+1],FrameLength-4);
						//��ȡ�����ַ�
						FrameEnd = Rx_Buffer[i+FrameLength-1];						
						//�жϹ������Ƿ���ȷ
						if((FunctionSign == 0x02)||(FunctionSign == 0x03)||(FunctionSign ==0x81)||(FunctionSign==0x82)||(FunctionSign==0x83)||(FunctionSign == 0x84)
				    ||(FunctionSign==0x85)||(FunctionSign==0x86)||(FunctionSign==0x88)	||(FunctionSign==0x89))
						{
							//�ж�У����
//							if(1)
							if(XorTemp == Rx_Buffer[i+FrameLength-2])
							{
								//�жϽ�����
								if(0xff ==FrameEnd)
								{
											//���յ�����������ȷ�� ��¼��SD����
											if(1==SD_Check())SD_Creat_CSVFile(1,Rx_Buffer,SendandReceiveTimes++);
									    switch(FunctionSign)
													{
													  case 0x01:
														  DigitalAnalogData_Compare(i);
														  IWDG_Feed(); //ι��	
														break;
														case 0x02:
														  DigitalData_AnalogData_QueryState();//��ѯָ��
														  IWDG_Feed(); //ι��
														break;
														case 0x03:
														  DigitalData_AnalogData_OutputProcess(i);//ģ���������������������,Ӧ����һ������ģʽ
														  IWDG_Feed(); //ι��
														break;
														case 0x81:
														  BoardCast();
														  IWDG_Feed(); //ι��
														break;
														case 0x82://��ʱ
														  TimeService_Temperature_Humidity();
														  IWDG_Feed(); //ι��
														break;
														case 0x83://��λָ��
														  Restart_instructions();
															IWDG_Feed(); //ι��
															break;
														case 0x84://�͹���
															Low_Power_Consumption();
															IWDG_Feed(); //ι��
															break;
														case 0x85://�����������
															NetParameter_Setting(i);
															IWDG_Feed(); //ι��
															break;
														case 0x86://�̼���������λ��ѡ��
															if(FrameLength == 9)//����λ����ȡ�̼������С
															{
																	W5500_Parameter_Configuration.FirmwareLength[0] = Rx_Buffer[3];
																	W5500_Parameter_Configuration.FirmwareLength[1] = Rx_Buffer[4];
																	W5500_Parameter_Configuration.FirmwareLength[2] = Rx_Buffer[5];
																	W5500_Parameter_Configuration.FirmwareLength[3] = Rx_Buffer[6];
																	//д�뵽AT24C04��
																 AT24CXX_Write(35,W5500_Parameter_Configuration.FirmwareLength,4);  //�������������д�뵽AT24C04��
																	//����������ԭ������
																 STMFLASH_Erase(BACKUP_ADDRESS);//APP�����ǲ�������������Ϊ���յ��ļ��Ƿ��ڱ�����
//																 STMFLASH_Erase(APP_ADDRESS);	//boot�����ǲ���APP_ADDRESS�����յ��ļ��Ƿ���APP_ADDRESS��
																//sendto(SOCK_UDPS,Rx_Buffer,9, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort); //���͵���λ��	
																}
															break;
														case 0x87://�忨����״̬����						
															break;
														case 0x88://ģ�����ɼ�ͨ������
															AnalogDataInput_Channel_Setting(i);
															IWDG_Feed(); //ι��
															break;
														case 0x89:
															heart_beat_test();
															IWDG_Feed(); //ι��
														default:
															break;
													}	
													IWDG_Feed();//ι��									
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

