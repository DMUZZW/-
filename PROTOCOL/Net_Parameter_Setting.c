#include "Net_Parameter_Setting.h"

/*�㲥ָ���*/
void BoardCast(void)
{
	unsigned char  i=0;
//  Net_DigitalDataOut.FrameLength = Rx_Buffer[pointer+1]; //֡����  //������� ��9 �ĳ�7����
//	if(Rx_Buffer[pointer] == 0xFE)
//	{
//		if(Net_DigitalDataOut.FrameLength ==5)//
//		{
//			if(Rx_Buffer[2] == 0x81) //������·��������λ��
//			{
				Tx_Buffer[0] = 0xfe;
				Tx_Buffer[1] = 0x1d;// frame length 29
				Tx_Buffer[2] = 0x81;
				for (i=3;i<9;i++)
				{
					Tx_Buffer[i] = W5500_Parameter_Configuration.MAC_Addr[i-3];//MAC��ַ
				}
				for(i=9;i<13;i++) //local IP
				{
				  Tx_Buffer[i] = W5500_Parameter_Configuration.LOCAL_IP_Addr[i-9];
				}
				for (i=13;i<17;i++) //remote ip
				{
					Tx_Buffer[i] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[i-13];
				}
				for (i=17;i<21;i++) //submask
				{
					Tx_Buffer[i] = W5500_Parameter_Configuration.Sub_Mask[i-17];
				}
				for (i=21;i<25;i++) //gateway
				{
					Tx_Buffer[i] = W5500_Parameter_Configuration.Gateway_IP[i-21];
				}
				Tx_Buffer[25] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port>>8);//��8λ
				Tx_Buffer[26] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port&0x00ff);//��8λ
//        AT24CXX_Read(31,W5500_Parameter_Configuration.SoftWare_Version,2);
//	      Tx_Buffer[27] = W5500_Parameter_Configuration.SoftWare_Version[0];
//	      Tx_Buffer[28] = W5500_Parameter_Configuration.SoftWare_Version[1];
	      Tx_Buffer[27] = GetXor(&Tx_Buffer[1],25);
	      Tx_Buffer[28] = 0xff;
				sendto(SOCK_UDPS,Tx_Buffer,29, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort); //���͵���λ��
				if(1==SD_Check())
				SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
				DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
//			}

//		}
		
//	}

	
}
/*
����������� 2020-6-4
*/
void NetParameter_Setting(unsigned short pointer)
{
	  unsigned int i =0,j=0,XorDataTemp=0;	
	  unsigned char IP_label,MAC_label,gateway_label,submask_label,port_lable;//���ڱ�ǲ�����ǩ
	  unsigned char IP_lable_address,MAC_lable_address,gateway_lable_address,submask_lable_address,port_lable_address,Xor_address;//���ڱ�Ǹ�����������ʼ��ַ
	  unsigned short temp1=0x00;//�����ݴ�˿ں�
	  unsigned char I2C_BUFF_TEMP[10];
//	  volatile unsigned char a = 0x00;
	  Net_DigitalDataOut.FrameLength = Rx_Buffer[pointer+1]; //֡����  	
	  XorDataTemp = GetXor(&Rx_Buffer[pointer+1],Net_DigitalDataOut.FrameLength-4);//���У��

	  if(Net_DigitalDataOut.FrameLength <5)return ;// the length error	
	  if(Rx_Buffer[pointer] == 0xfe)//��ʼ�ַ�
		{
		
			 if(Rx_Buffer[pointer+2] == 0x85)//��������ȷ
		   {
				  IP_label = Rx_Buffer[pointer+3];
				  IP_lable_address = pointer+3;
				 if(IP_label!=0x00)
				 {
					 switch (IP_label)
					 {
						 case 0x01:
						 for(i=0;i<4;i++)
						{
						  if(Rx_Buffer[IP_lable_address+i+1]!=W5500_Parameter_Configuration.LOCAL_IP_Addr[i]) break;	 //���IP��ַ�����紫���ĵ�ַ��һ��						 					 	 
						}
						if(i<4)//��Ҫ�޸�IP
						{
						  for(i=0;i<4;i++)
						  {
							W5500_Parameter_Configuration.LOCAL_IP_Addr[i] = Rx_Buffer[IP_lable_address+i+1];
							I2C_BUFF_TEMP[i] = Rx_Buffer[IP_lable_address+i+1];
						  }	
						 setSIPR(W5500_Parameter_Configuration.LOCAL_IP_Addr);//local IP
						 AT24CXX_Write(1,I2C_BUFF_TEMP,4);//д��AT24C02
						
						 memset(I2C_BUFF_TEMP,0x00,10);
						 AT24CXX_Read(1,I2C_BUFF_TEMP,4); //����д���IP	
						 for(i=0;i<4;i++)
						{
						 if(W5500_Parameter_Configuration.LOCAL_IP_Addr[i] !=I2C_BUFF_TEMP[i])
						  break;
						}
						if(i<4)//�޸�ʧ��,���ر���ԭIP��ַ
						{
						Tx_Buffer[IP_lable_address] = 0x01;
						Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
						Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
						Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
						Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
						Tx_Buffer[IP_lable_address+5] = 0x55;//fail to set Ip
						}
						else //����޸ĳɹ��������޸ĺ�ĵ�ַ
					    {
						Tx_Buffer[IP_lable_address] = 0x01;
						Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
						Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
						Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
						Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
						Tx_Buffer[IP_lable_address+5] = 0xAA;//succeed to set ip
					    }					 
					  }
					else//����Ҫ����IP
					{
						Tx_Buffer[IP_lable_address] = 0x01;
						Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
						Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
						Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
						Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
						Tx_Buffer[IP_lable_address+5] = 0xAA;//succeed to set ip
					 }			 
						MAC_lable_address = pointer+8;//H�˴�����MAC��ַ��־λ�ĵ�ַ
						MAC_label = Rx_Buffer[pointer+8];
					 break;
					 
					 
					case 0x02://�޸�Ŀ��IP
					for(i=0;i<4;i++)
					{
					if(W5500_Parameter_Configuration.REMOTE_Sn_DIP[i] != Rx_Buffer[IP_lable_address+i+1])
					 break;
					}
					if(i<4)//��Ҫ�޸�Ŀ��IP ԭ����ֵ������ֵ��ͬ
					{
					for(i=0;i<4;i++)
					{
					 W5500_Parameter_Configuration.REMOTE_Sn_DIP[i] = Rx_Buffer[IP_lable_address+i+1];
					 I2C_BUFF_TEMP[i] = Rx_Buffer[IP_lable_address+i+1];
					}
					setDIPR(SOCK_UDPS,W5500_Parameter_Configuration.REMOTE_Sn_DIP);//����Ŀ��IP��ַ
					AT24CXX_Write(5,I2C_BUFF_TEMP,4);//д��AT24C02
					
				    memset(I2C_BUFF_TEMP,0x00,10);
					AT24CXX_Read(5,I2C_BUFF_TEMP,4); //����д���IP	
					for(i=0;i<4;i++)
					{
					if(W5500_Parameter_Configuration.REMOTE_Sn_DIP[i] != I2C_BUFF_TEMP[i])
					break;
					}
					if(i<4)//fail to set destination ip
					{
					 Tx_Buffer[IP_lable_address] = 0x02;
					 Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
					 Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
					 Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
					 Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
					 Tx_Buffer[IP_lable_address+5] = 0x55;//fail to set Ip
					}
					 else //succeed to set destination ip
					 {
					 Tx_Buffer[IP_lable_address] = 0x02;
					 Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
					 Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
					 Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
					 Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
					 Tx_Buffer[IP_lable_address+5] = 0xaa;//succeed to set Ip
						 
					 }
											 
					}	
					
					else //����Ҫ����IP���¾�IPһ�����������ݺ��޸ĳɹ�һ��
					{
					 Tx_Buffer[IP_lable_address] = 0x02;
					 Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0];
					 Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1];
					 Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2];
					 Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3];
					 Tx_Buffer[IP_lable_address+5] = 0xaa;//succeed to set Ip						
					}	
					 MAC_lable_address = pointer+8;
					 MAC_label = Rx_Buffer[pointer+8];
					break;
					
					
				 case 0x03:
						for(i=0;i<4;i++)
						{
							if(W5500_Parameter_Configuration.LOCAL_IP_Addr[i] != Rx_Buffer[IP_lable_address+i+1])
								break;	// need to change the source ip					
						}	
					 for(j=0;j<4;j++)
					 {
						 if(W5500_Parameter_Configuration.REMOTE_Sn_DIP[j] != Rx_Buffer[IP_lable_address+j+5])
							 break;//need to change the destination ip
					 }
					 if((i<4)&(j<4))//change the source ip and destination ip
					 {
						 for(i=0;i<4;i++)
						 {
							 W5500_Parameter_Configuration.LOCAL_IP_Addr[i] = Rx_Buffer[IP_lable_address+i+1];
							 W5500_Parameter_Configuration.REMOTE_Sn_DIP[i] = Rx_Buffer[IP_lable_address+i+5];
							 I2C_BUFF_TEMP[i] = Rx_Buffer[IP_lable_address+i+1];
							 I2C_BUFF_TEMP[i+4] = Rx_Buffer[IP_lable_address+i+5];
						 }
						 setSIPR(W5500_Parameter_Configuration.LOCAL_IP_Addr);//
						 setDIPR(SOCK_UDPS,W5500_Parameter_Configuration.REMOTE_Sn_DIP);//����Ŀ��IP��ַ
						 AT24CXX_Write(1,I2C_BUFF_TEMP,8);
						 
                         memset(I2C_BUFF_TEMP,0x00,10);
						 AT24CXX_Read(1,I2C_BUFF_TEMP,8); //
						 for(i=0;i<4;i++)
							{
								if((W5500_Parameter_Configuration.LOCAL_IP_Addr[i] != I2C_BUFF_TEMP[i+1])|(W5500_Parameter_Configuration.REMOTE_Sn_DIP[i]!=I2C_BUFF_TEMP[i+5]))
									break;//fail to set the source ip and destination ip
							}						 
							if(i<4)//failure
							{
								Tx_Buffer[IP_lable_address] = 0x03;
								Tx_Buffer[IP_lable_address+1] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0] ;
								Tx_Buffer[IP_lable_address+2] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1] ;
								Tx_Buffer[IP_lable_address+3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2] ;
								Tx_Buffer[IP_lable_address+4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3] ;
								Tx_Buffer[IP_lable_address+5] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[0] ;
								Tx_Buffer[IP_lable_address+6] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[1] ;
								Tx_Buffer[IP_lable_address+7] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[2] ;
								Tx_Buffer[IP_lable_address+8] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[3] ;
								Tx_Buffer[IP_lable_address+9] = 0x55;
							}
						 else//success
						 {
								Tx_Buffer[IP_lable_address] = 0x03;
								Tx_Buffer[IP_lable_address+1] = I2C_BUFF_TEMP[0] ;
								Tx_Buffer[IP_lable_address+2] = I2C_BUFF_TEMP[1] ;
								Tx_Buffer[IP_lable_address+3] = I2C_BUFF_TEMP[2] ;
								Tx_Buffer[IP_lable_address+4] = I2C_BUFF_TEMP[3] ;
								Tx_Buffer[IP_lable_address+5] = I2C_BUFF_TEMP[4] ;
								Tx_Buffer[IP_lable_address+6] = I2C_BUFF_TEMP[5] ;
								Tx_Buffer[IP_lable_address+7] = I2C_BUFF_TEMP[6] ;
								Tx_Buffer[IP_lable_address+8] = I2C_BUFF_TEMP[7] ;
								Tx_Buffer[IP_lable_address+9] = 0xaa;
						 }
					 }
					 else //��������IP
					 {
								Tx_Buffer[IP_lable_address] = 0x03;
								Tx_Buffer[IP_lable_address+1] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0] ;
								Tx_Buffer[IP_lable_address+2] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1] ;
								Tx_Buffer[IP_lable_address+3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2] ;
								Tx_Buffer[IP_lable_address+4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3] ;
								Tx_Buffer[IP_lable_address+5] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[0] ;
								Tx_Buffer[IP_lable_address+6] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[1] ;
								Tx_Buffer[IP_lable_address+7] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[2] ;
								Tx_Buffer[IP_lable_address+8] = W5500_Parameter_Configuration.REMOTE_Sn_DIP[3] ;
								Tx_Buffer[IP_lable_address+9] = 0xaa;
						 
					 }
					 MAC_lable_address = pointer+12;
					 MAC_label = Rx_Buffer[pointer+12]; 						 
					 break;
						 default:

						 break;
					 }
				 }	
				 //����Ҫ����IP
                 else if(IP_label == 0x00)
				{
					MAC_label = Rx_Buffer[pointer+4];
					MAC_lable_address = pointer+4;	
				}
				


				
				 if(MAC_label == 0x01) //��Ҫ����MAC
				 {
					 for(i=0;i<6;i++)
					 {
						 if( W5500_Parameter_Configuration.MAC_Addr[i] != Rx_Buffer[MAC_lable_address+i+1])
							 break;					 
					 }
					 if(i<6)//need to change the mac
					 {
						 for(i=0;i<6;i++)
						 {
							 W5500_Parameter_Configuration.MAC_Addr[i] = Rx_Buffer[MAC_lable_address+i+1];
							 I2C_BUFF_TEMP[i] = Rx_Buffer[MAC_lable_address+i+1];
						 }
						 setSHAR(W5500_Parameter_Configuration.MAC_Addr);
						 AT24CXX_Write(17,I2C_BUFF_TEMP,6);//д��AT24C02
					     memset(I2C_BUFF_TEMP,0x00,10);
					     AT24CXX_Read(17,I2C_BUFF_TEMP,6); //����д���IP	
						 for(i=0;i<6;i++)
						 {
							 if(W5500_Parameter_Configuration.MAC_Addr[i] != I2C_BUFF_TEMP[i])
								 break;
						 }
						 if(i<6)//�޸�ʧ�� ����ԭ����MAC��ַ
						 {
							 Tx_Buffer[MAC_lable_address] = 0x01;
							 Tx_Buffer[MAC_lable_address+1]  = W5500_Parameter_Configuration.MAC_Addr[0];
							 Tx_Buffer[MAC_lable_address+2]  = W5500_Parameter_Configuration.MAC_Addr[1];
							 Tx_Buffer[MAC_lable_address+3]  = W5500_Parameter_Configuration.MAC_Addr[2];
							 Tx_Buffer[MAC_lable_address+4]  = W5500_Parameter_Configuration.MAC_Addr[3];
							 Tx_Buffer[MAC_lable_address+5]  = W5500_Parameter_Configuration.MAC_Addr[4];
							 Tx_Buffer[MAC_lable_address+6]  = W5500_Parameter_Configuration.MAC_Addr[5];
							 Tx_Buffer[MAC_lable_address+7] = 0x55;
							 
						 }
						 else//�޸ĳɹ�
						 {
							 Tx_Buffer[MAC_lable_address] = 0x01;
							 Tx_Buffer[MAC_lable_address+1]  = I2C_BUFF_TEMP[0];
							 Tx_Buffer[MAC_lable_address+2]  = I2C_BUFF_TEMP[1];
							 Tx_Buffer[MAC_lable_address+3]  = I2C_BUFF_TEMP[2];
							 Tx_Buffer[MAC_lable_address+4]  = I2C_BUFF_TEMP[3];
							 Tx_Buffer[MAC_lable_address+5]  = I2C_BUFF_TEMP[4];
							 Tx_Buffer[MAC_lable_address+6]  = I2C_BUFF_TEMP[5];
							 Tx_Buffer[MAC_lable_address+7] = 0xaa;
						 }						 
					 }
					 else // ����Ҫ�޸�MAC
					 {
						 	 Tx_Buffer[MAC_lable_address] = 0x01;
							 Tx_Buffer[MAC_lable_address+1]  = W5500_Parameter_Configuration.MAC_Addr[0];
							 Tx_Buffer[MAC_lable_address+2]  = W5500_Parameter_Configuration.MAC_Addr[1];
							 Tx_Buffer[MAC_lable_address+3]  = W5500_Parameter_Configuration.MAC_Addr[2];
							 Tx_Buffer[MAC_lable_address+4]  = W5500_Parameter_Configuration.MAC_Addr[3];
							 Tx_Buffer[MAC_lable_address+5]  = W5500_Parameter_Configuration.MAC_Addr[4];
							 Tx_Buffer[MAC_lable_address+6]  = W5500_Parameter_Configuration.MAC_Addr[5];
							 Tx_Buffer[MAC_lable_address+7] = 0xaa;
					 }
						 
					 gateway_label = Rx_Buffer[MAC_lable_address+7]; //		MAC 6λ +������
                     gateway_lable_address = MAC_lable_address+7;
					 
				 }
				 else if(MAC_label == 0x00)//����Ҫ����MAC
				 {
					 
					 gateway_label = Rx_Buffer[MAC_lable_address+1]; //	������
                      gateway_lable_address = MAC_lable_address+1; 
				 }

				 
				 
				 
				 
                if(gateway_label==0x01)//����GATEWAY
				 {
					 for(i=0;i<4;i++)
					 {
						 if(W5500_Parameter_Configuration.Gateway_IP[i] != Rx_Buffer[gateway_lable_address+1+i] )
							 break;
					 }
					 if(i<4) //need to change
					 {
						 for(i=0;i<4;i++)
						 {
							 W5500_Parameter_Configuration.Gateway_IP[i] = Rx_Buffer[gateway_lable_address+1+i];
							 I2C_BUFF_TEMP[i] = Rx_Buffer[gateway_lable_address+1+i];
						 }
						  setGAR(W5500_Parameter_Configuration.Gateway_IP);
						  AT24CXX_Write(13,I2C_BUFF_TEMP,4);//д��AT24C02
						   memset(I2C_BUFF_TEMP,0x00,10);
					      AT24CXX_Read(13,I2C_BUFF_TEMP,4); //����д���IP	
						 for(i=0;i<4;i++)
						 {
							 if(W5500_Parameter_Configuration.Gateway_IP[i] != I2C_BUFF_TEMP[i])
								 break;
						 }
						 if(i<4)//�޸�ʧ��
						 { 
							  Tx_Buffer[gateway_lable_address] = 0x01;
							  Tx_Buffer[gateway_lable_address+1] = W5500_Parameter_Configuration.Gateway_IP[0] ;
							  Tx_Buffer[gateway_lable_address+2] = W5500_Parameter_Configuration.Gateway_IP[1] ;
							  Tx_Buffer[gateway_lable_address+3] = W5500_Parameter_Configuration.Gateway_IP[2] ;
							  Tx_Buffer[gateway_lable_address+4] = W5500_Parameter_Configuration.Gateway_IP[3] ;
							  Tx_Buffer[gateway_lable_address+4] = 0x55;							 
						 }
							 else //�޸ĳɹ�
							 {
							  Tx_Buffer[gateway_lable_address] = 0x01;
							  Tx_Buffer[gateway_lable_address+1] = I2C_BUFF_TEMP[0] ;
							  Tx_Buffer[gateway_lable_address+2] = I2C_BUFF_TEMP[1] ;
							  Tx_Buffer[gateway_lable_address+3] = I2C_BUFF_TEMP[2] ;
							  Tx_Buffer[gateway_lable_address+4] = I2C_BUFF_TEMP[3] ;
							  Tx_Buffer[gateway_lable_address+4] = 0xaa;	
								 
							 }						 
					 }
					 else //do not need to change
					 {						 
						 	  Tx_Buffer[gateway_lable_address] = 0x01;
							  Tx_Buffer[gateway_lable_address+1] = W5500_Parameter_Configuration.Gateway_IP[0] ;
							  Tx_Buffer[gateway_lable_address+2] = W5500_Parameter_Configuration.Gateway_IP[1] ;
							  Tx_Buffer[gateway_lable_address+3] = W5500_Parameter_Configuration.Gateway_IP[2] ;
							  Tx_Buffer[gateway_lable_address+4] = W5500_Parameter_Configuration.Gateway_IP[3] ;
							  Tx_Buffer[gateway_lable_address+4] = 0xaa;	
					 }
					 submask_lable_address=gateway_lable_address+5;
					 submask_label = Rx_Buffer[gateway_lable_address+5];
					 
				 } 
				 else if(gateway_label==0x00)//����Ҫ����GATEWAY
				 {
					 submask_lable_address=gateway_lable_address+1;
					 submask_label = Rx_Buffer[gateway_lable_address+1]; 
				 }

				 
				 
				 
				 
				 if(submask_label ==0x01)
				 {
					 for(i=0;i<4;i++)
					 {
						 if(W5500_Parameter_Configuration.Sub_Mask[i] !=  Rx_Buffer[submask_lable_address+1+i])
							 break;
					 }
					 if(i<4)//��Ҫ�޸�submask
					 {
						 for(i=0;i<4;i++)
						 {
							 W5500_Parameter_Configuration.Sub_Mask[i] =  Rx_Buffer[submask_lable_address+1+i];
							 I2C_BUFF_TEMP[i] = Rx_Buffer[submask_lable_address+1+i];
						 }
						 setSUBR(W5500_Parameter_Configuration.Sub_Mask);
						 AT24CXX_Write(9,I2C_BUFF_TEMP,4);//д��AT24C02						 
						
						 memset(I2C_BUFF_TEMP,0x00,10);
					     AT24CXX_Read(9,I2C_BUFF,4); //����д���Sub_Mask
						 for(i=0;i>4;i++)
						 {
							 if(I2C_BUFF_TEMP[i] != W5500_Parameter_Configuration.Sub_Mask[i])
								 break;
						 }
						 if(i<4)//�޸�ʧ��
						 {
							  Tx_Buffer[submask_lable_address] = 0x01;
							  Tx_Buffer[submask_lable_address+1] = W5500_Parameter_Configuration.Sub_Mask[0];
							  Tx_Buffer[submask_lable_address+2] = W5500_Parameter_Configuration.Sub_Mask[1];
							  Tx_Buffer[submask_lable_address+3] = W5500_Parameter_Configuration.Sub_Mask[2];
							  Tx_Buffer[submask_lable_address+4] = W5500_Parameter_Configuration.Sub_Mask[3];
							  Tx_Buffer[submask_lable_address+4] = 0x55;	
							 
						 }
						 else//�޸ĳɹ�
						 {
							 	Tx_Buffer[submask_lable_address] = 0x01;
							  Tx_Buffer[submask_lable_address+1] = I2C_BUFF_TEMP[0];
							  Tx_Buffer[submask_lable_address+2] = I2C_BUFF_TEMP[1];
							  Tx_Buffer[submask_lable_address+3] = I2C_BUFF_TEMP[2];
							  Tx_Buffer[submask_lable_address+4] = I2C_BUFF_TEMP[3];
							  Tx_Buffer[submask_lable_address+4] = 0xaa;								 
						 }
					 }
					 else//not need
					 {
						 		Tx_Buffer[submask_lable_address] = 0x01;
							  Tx_Buffer[submask_lable_address+1] = W5500_Parameter_Configuration.Sub_Mask[0];
							  Tx_Buffer[submask_lable_address+2] = W5500_Parameter_Configuration.Sub_Mask[1];
							  Tx_Buffer[submask_lable_address+3] = W5500_Parameter_Configuration.Sub_Mask[2];
							  Tx_Buffer[submask_lable_address+4] = W5500_Parameter_Configuration.Sub_Mask[3];
							  Tx_Buffer[submask_lable_address+4] = 0xaa;	
						 
					 }
					 port_lable_address = submask_lable_address+5;
					 port_lable  = Rx_Buffer[submask_lable_address+5];
					 
				 }
				 else if(submask_label==0x00)
				 {
					 port_lable_address = submask_lable_address+1;
					 port_lable  = Rx_Buffer[submask_lable_address+1];
				 }

				 if(port_lable ==0x01)//need to change the port
				 {
                    if((((W5500_Parameter_Configuration.Sn_UDP_Port&0xff00)>>8)!=Rx_Buffer[port_lable_address+1]) | (((unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port&0x00ff))!=Rx_Buffer[port_lable_address+2]))
					 {
						 W5500_Parameter_Configuration.Sn_UDP_Port = (Rx_Buffer[port_lable_address+1]<<8)|Rx_Buffer[port_lable_address+2];
						 I2C_BUFF_TEMP[0] = Rx_Buffer[port_lable_address+1];
						 I2C_BUFF_TEMP[1] = Rx_Buffer[port_lable_address+2];
					 }
					  setSn_PORT(SOCK_UDPS,W5500_Parameter_Configuration.Sn_UDP_Port);	
					  AT24CXX_Write(23,I2C_BUFF_TEMP,2);//д��AT24C02	
                      
					  memset(I2C_BUFF_TEMP,0x00,10);
                      AT24CXX_Read(23,I2C_BUFF_TEMP,2); //����д���IP	
					  temp1 = (I2C_BUFF_TEMP[0]<<8)+ I2C_BUFF_TEMP[1];
					  if(temp1 != W5500_Parameter_Configuration.Sn_UDP_Port )//�޸Ĳ��ɹ�
						{
							  Tx_Buffer[port_lable_address] = 0x01;
							  Tx_Buffer[port_lable_address+1] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port>>8);
							  Tx_Buffer[port_lable_address+2] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port & 0x00ff);
							  Tx_Buffer[port_lable_address+3] = 0x55;	
						}
						else//�޸ĳɹ�
						{
								Tx_Buffer[port_lable_address] = 0x01;
							  Tx_Buffer[port_lable_address+1] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port>>8);
							  Tx_Buffer[port_lable_address+2] = (unsigned char)(W5500_Parameter_Configuration.Sn_UDP_Port & 0x00ff);
							  Tx_Buffer[port_lable_address+3] = 0xaa;	
						}
					Xor_address = port_lable_address+4;	
				 }
				 else if(port_lable ==0x00)
				 {
					 Xor_address = port_lable_address+2;
				 }
				 Tx_Buffer[0] =0xfe;
				 Tx_Buffer[1] =Xor_address+2;
				 Tx_Buffer[2] =0x85;
				 Tx_Buffer[Xor_address] = GetXor(&Tx_Buffer[1],Xor_address-2);
				 Tx_Buffer[Xor_address+1] =0xff;
				 sendto(SOCK_UDPS,Tx_Buffer,Xor_address+2, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);
				 if(1==SD_Check())
				SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
				 DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
		   }
			//����֮�� �Զ����������λ
			SoftRestart(); 	
		}

			
	 
	
}

















































//void Net_Parameter_Setting(void)
//{
//		unsigned int i =0,XorDataTemp=0;	
//	  Net_DigitalDataOut.FrameLength = Rx_Buffer[1]; //֡����  //������� ��9 �ĳ�7����
//	  if(Net_DigitalDataOut.FrameLength ==0x00) return;	
//	  if(Net_DigitalDataOut.FrameLength>20)return;	
//	   XorDataTemp = GetXor(&Rx_Buffer[1],Net_DigitalDataOut.FrameLength-3); //���У�� 
//	  if(XorDataTemp !=Rx_Buffer[Net_DigitalDataOut.FrameLength-1])return;
//	   switch(Rx_Buffer[2])
//	   {
//		  case 0x01://��ȡ��̫��������Ϣ
//				if(Net_DigitalDataOut.FrameLength ==4)
//				{
//						Tx_Buffer[0] = 0xEF;
//						Tx_Buffer[1] = 0x12;
//						Tx_Buffer[2] = 0x01;
//						for(i=3;i<7;i++)
//						Tx_Buffer[i] = W5500_Parameter_Configuration.LOCAL_IP_Addr[i-3];
//						for (i=7;i<11;i++)
//						Tx_Buffer[i] = W5500_Parameter_Configuration.Sub_Mask[i-7];
//						for(i=11;i<17;i++)
//						Tx_Buffer[i]= W5500_Parameter_Configuration.MAC_Addr[i-11];
//						Tx_Buffer[17] = GetXor(&Tx_Buffer[1],15);
//					  if(Connect_OK ==TRUE)
//						{
//							sendto(SOCK_UDPS,Tx_Buffer,18, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort); //���͵���λ��
//////              Read_DigitalData_Input_State();							
//						}		         						
//				}	
//				else if(Net_DigitalDataOut.FrameLength ==5)
//				{
//						if (Rx_Buffer[11] !=W5500_Parameter_Configuration.MAC_Addr[5]) break;
//						Tx_Buffer[0] = 0xEF;
//						Tx_Buffer[1] = 0x12;
//						Tx_Buffer[2] = 0x01;
//						for(i=3;i<7;i++)
//						Tx_Buffer[i] = W5500_Parameter_Configuration.LOCAL_IP_Addr[i-3];
//						for (i=7;i<11;i++)
//						Tx_Buffer[i] = W5500_Parameter_Configuration.Sub_Mask[i-7];
//						for(i=11;i<17;i++)
//						Tx_Buffer[i]= W5500_Parameter_Configuration.MAC_Addr[i-11];
//						Tx_Buffer[17] = GetXor(&Tx_Buffer[1],15);
//					 if(Connect_OK ==TRUE)
//						{
//							sendto(SOCK_UDPS,Tx_Buffer,18, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort); //���͵���λ               
////							Read_DigitalData_Input_State();
//						}
//				}		
//			break;
//		  case 0x02: //����IP��ַ
//				
//				if(W5500_Parameter_Configuration.MAC_Addr[5] != Rx_Buffer[7])return ;			  
//			for (i=3;i<7;i++)
//		    {
// 				 if(W5500_Parameter_Configuration.LOCAL_IP_Addr[i-3] != Rx_Buffer[i]) //���IP��ַ�뷢�͹�����IP��ַ��һ��
//					  
//						break;
//				}
//				if(i<7)
//				{
//					for(i=3;i<7;i++)
//					{
//						W5500_Parameter_Configuration.LOCAL_IP_Addr[i-3] = Rx_Buffer[i]; //д��IP
//						I2C_BUFF[i-3] = Rx_Buffer[i];
//					}
//					AT24CXX_Write(1,I2C_BUFF,4);//д��AT24C02
//					setSIPR(W5500_Parameter_Configuration.LOCAL_IP_Addr);//local IP
//					AT24CXX_Read(1,I2C_BUFF,4); //����д���IP					
//					for(i=0;i<4;i++)
//					{
//						if(W5500_Parameter_Configuration.LOCAL_IP_Addr[i] != I2C_BUFF[i]) //�鿴�Ƿ��޸�IP�ɹ�
//						break;
//					}					
//					if(i<4) //�޸�ʧ��
//					{
//						Tx_Buffer[0] = 0xEF;
//						Tx_Buffer[1] = 0x02;
//						Tx_Buffer[2] = 0x55;
//						Tx_Buffer[3] = 0x57; 
//					}
//					else//�޸ĳɹ�
//					{
//						Tx_Buffer[0] = 0xEF;
//						Tx_Buffer[1] = 0x02;
//						Tx_Buffer[2] = 0xAA;
//						Tx_Buffer[3] = 0xa8;
//					}					
//				}
//				else //���IP��ַ����Ҫ�޸� i>15
//				{
//					Tx_Buffer[0] = 0xEF;
//					Tx_Buffer[1] = 0x02;
//					Tx_Buffer[2] = 0xAA;
//					Tx_Buffer[3] = 0xa8;
//				}
//				if(Connect_OK ==TRUE)
//				{
//				sendto(SOCK_UDPS,Tx_Buffer,4, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);
//				}
//        break;
//		  case 0x03: //������������
//			for(i=3;i<7;i++)
//				{
//					if(W5500_Parameter_Configuration.Sub_Mask[i-3] != Rx_Buffer[i])break;		//�ж����������Ƿ�һ��	 
//				}
//			if(i<7) ////������е����������뷢�͹����Ĳ�һ��	
//			{
//				for(i=3;i<7;i++)
//				{
//					W5500_Parameter_Configuration.Sub_Mask[i-3] = Rx_Buffer[i];
//					I2C_BUFF[i-3] = Rx_Buffer[i];
//				}
//				AT24CXX_Write(9,I2C_BUFF,4);//д��AT24C02
//				setSUBR(W5500_Parameter_Configuration.Sub_Mask);//��������
//				AT24CXX_Read(9,I2C_BUFF,4); //����д���IP
//				for (i=0;i<4;i++)
//				{
//					if(I2C_BUFF[i] !=W5500_Parameter_Configuration.Sub_Mask[i])
//						break;
//				}
//				if(i<4) //�޸Ĳ��ɹ�
//				{
//					Tx_Buffer[0] = 0xEF;
//					Tx_Buffer[1] = 0x03;
//					Tx_Buffer[2] = 0x55;
//					Tx_Buffer[3] = 0x56;
//				}
//				else //�޸ĳɹ�
//				{
//					Tx_Buffer[0] = 0xEF;
//					Tx_Buffer[1] = 0x03;
//					Tx_Buffer[2] = 0xaa;
//					Tx_Buffer[3] = 0xa9;					
//				}
//			}
//			else //���һ�� �����޸ĳɹ���־
//			{
//				Tx_Buffer[0] = 0xEF;
//				Tx_Buffer[1] = 0x03;
//				Tx_Buffer[2] = 0xaa;
//				Tx_Buffer[3] =0xa9;				
//			}
//			if(Connect_OK ==TRUE)
//			{
//					sendto(SOCK_UDPS,Tx_Buffer,4, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);
//				delay_ms(500);
//			}	
//			break;
//		case 0x04: //����MAC��ַ ��������²���Ҫ�����޸�
//      for (i=3;i<9;i++)
//         {
//           if(W5500_Parameter_Configuration.MAC_Addr[i-3] !=Rx_Buffer[i])
//             break;
//         }
//       if(i<9)
//       {
//         for(i=3;i<9;i++)
//		     {
//					W5500_Parameter_Configuration.MAC_Addr[i-3] = Rx_Buffer[i];
//					I2C_BUFF[i-3] = Rx_Buffer[i];					
//				 }
//				AT24CXX_Write(30,I2C_BUFF,6);//д��AT24C02  
//			  setSHAR(W5500_Parameter_Configuration.MAC_Addr); //MAC��ַ
//				AT24CXX_Read(30,I2C_BUFF,6); //����д���IP
//			  for (i=0;i<6;i++)
//				 {
//					 if(I2C_BUFF[i] !=W5500_Parameter_Configuration.MAC_Addr[i])break;
//				 }
//				 if(i<6) //�޸Ĳ��ɹ�
//				 {
//					Tx_Buffer[0] = 0xEF;
//					Tx_Buffer[1] = 0x04;
//					Tx_Buffer[2] = 0x55;
//					Tx_Buffer[3] = 0x51;
//				 }
//				 else //�޸ĳɹ�
//				 {
//          Tx_Buffer[0] = 0xEF;
//					Tx_Buffer[1] = 0x04;
//					Tx_Buffer[2] = 0xAA;
//					Tx_Buffer[3] = 0xae;
//				 }     
//       }
//       else //ֵ���䣬�൱���޸ĳɹ�
//       {
//         Tx_Buffer[0] = 0xEF;
//				 Tx_Buffer[1] = 0x04;
//				 Tx_Buffer[2] = 0xAA;
//				 Tx_Buffer[3] = 0xae;        
//       }		
//				 if(Connect_OK ==TRUE)
//				 {
//					 	sendto(SOCK_UDPS,Tx_Buffer,4, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort);
//					 	delay_ms(500);
//				 }
//			break;
//		default:
//			break;
//	}
//}























































///*
//ģ�����ɼ�ͨ������ ��������0-8ͨ��
//*/
//void AnalogData_Input_Channel_Setting(void)
//{
//	unsigned char XorDataTemp=0x00;
//	unsigned char Analog_channel=0x00;
//	unsigned char Analog_Temp=0x00;
//	if(Rx_Buffer[0] !=0xEF)return ;//
//	if(Rx_Buffer[1] != 0x06)return ;//���ȴ���
//	if(Rx_Buffer[5] != 0xff)return ;
//  XorDataTemp = GetXor(&Rx_Buffer[1],2);//���ֵ
//	if(Rx_Buffer[4] != XorDataTemp)return;
//	if(Rx_Buffer[2] ==0x05)//������
//	{
//		if((Rx_Buffer[3]<0) ||(Rx_Buffer[3]>8)) return ;//ģ����ͨ�����Ȳ���
//	  //д�뵽AT24C02
//	  Analog_channel = Rx_Buffer[3];
//	  AT24CXX_Write(31,&Analog_channel,1);//д��
//	  AT24CXX_Read(31,&Analog_Temp,1);//����
//	  if(Analog_Temp ==Analog_channel)//д��ɹ�
//	  {
//		  Tx_Buffer[0] = 0xef;
//		  Tx_Buffer[1] = 0x07;
//			Tx_Buffer[2] = 0x05;
//		  Tx_Buffer[3] = Analog_Temp;//ͨ����
//		  Tx_Buffer[4] = 0xaa;//״̬��
//		  Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
//		  Tx_Buffer[6] = 0xff;//�����ַ�
//		  if(Connect_OK == TRUE)
//		  {
//			  sendto(SOCK_UDPS,Tx_Buffer,7, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort);
//		  }	
//	  }
//	  else //�޸Ĳ��ɹ�
//	  {
//	    Tx_Buffer[0] = 0xef;
//		  Tx_Buffer[1] = 0x07;
//			Tx_Buffer[2] = 0x05;
//		  Tx_Buffer[3] = Analog_Temp;//ͨ����
//		  Tx_Buffer[4] = 0x55;//״̬��
//		  Tx_Buffer[5] = GetXor(&Tx_Buffer[1],3);
//		  Tx_Buffer[6] = 0xff;//�����ַ�
//		  if(Connect_OK == TRUE)
//		  {
//			  sendto(SOCK_UDPS,Tx_Buffer,7, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort);
//		  }	
//	 }			
//	}	
//}

///*�忨��ǩ��������Ϣ��ȡ*/
//void Get_Parameter(void)
//{
//	unsigned char XorDataTemp=0x00;
//	unsigned char Analog_DataInput_Channel=0x00;
//	if(Rx_Buffer[0] !=0xEF)return ;//
//	if(Rx_Buffer[1] != 0x05)return ;//���ȴ���
//	if(Rx_Buffer[4] != 0xff)return ;
//  XorDataTemp = GetXor(&Rx_Buffer[1],1);//���ֵ
//	if(Rx_Buffer[3] != XorDataTemp)return;
//	if(Rx_Buffer[2] ==0x06)//�鿴������
//	{
//		Tx_Buffer[0] = 0xef;
//		Tx_Buffer[1] = 0x16;//22���ֽ�
//		Tx_Buffer[2] = 0x06;
//		
//		Tx_Buffer[3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0];
//		Tx_Buffer[4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1];
//		Tx_Buffer[5] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2];
//		Tx_Buffer[6] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3];
//		
//		Tx_Buffer[7] = W5500_Parameter_Configuration.MAC_Addr[0];
//		Tx_Buffer[8] = W5500_Parameter_Configuration.MAC_Addr[1];
//		Tx_Buffer[9] = W5500_Parameter_Configuration.MAC_Addr[2];
//		Tx_Buffer[10] = W5500_Parameter_Configuration.MAC_Addr[3];
//		Tx_Buffer[11] = W5500_Parameter_Configuration.MAC_Addr[4];
//		Tx_Buffer[12] = W5500_Parameter_Configuration.MAC_Addr[5];
//		
//		AT24CXX_Read(31,&Analog_DataInput_Channel,1);//��AT24C02�ж�ȡ����		
//		Tx_Buffer[13] = Analog_DataInput_Channel ;//ģ�����ɼ�ͨ������
//		
//		Tx_Buffer[14] = 0x06;	//����汾��Ϣ
//		Tx_Buffer[15] = 0x06;
//		
//		Tx_Buffer[16] = 0x06;		//�����¶�
//		Tx_Buffer[17] = 0x06;
//		
//		Tx_Buffer[18] = 0x06;	//����ʪ��
//		Tx_Buffer[19] = 0x06;
//		
//		Tx_Buffer[20] = GetXor(&Rx_Buffer[1],1);
//		
//		Tx_Buffer[21] = 0xff;
//		
//	  if(Connect_OK == TRUE)
//		 {
//			 sendto(SOCK_UDPS,Tx_Buffer,22, W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_UDP_DPort);
//		 }
//		
//	}
//		
//	
//}


///*�忨״̬ɨ�裬����ģ�������Startʱ�̣���ȡȫ����ģ��������״̬*/
//void DataInput_State(void)
//{
//	unsigned char XorDataTemp=0x00;
//	unsigned char Analog_channel=0x00;
//	 AT24CXX_Read(31,&Analog_channel,1);//����
//	if(Rx_Buffer[0] !=0xEF)return ;//
//	if(Rx_Buffer[1] != 0x05)return ;//���ȴ���
//	if(Rx_Buffer[4] != 0xff)return ;
//  XorDataTemp = GetXor(&Rx_Buffer[1],1);//���ֵ
//	if(Rx_Buffer[3] != XorDataTemp)return;
//	if(Rx_Buffer[2] ==0x07)//�жϹ�����
//	{
////		Read_DigitalData_Input_State();//��������״̬���͵���λ��
//	//	AnalogDataInput_Process(Analog_channel,10);//��ģ����״̬���͵���λ��
//	}
//	
//}

///*
//Զ����������Э��
//������������ѡ�񣬸�ѡ��д��AT24C02�ĵ�34λ��ַ�ϣ� BOOT����ÿ�δ��ж�ȡ�������λ�ã���ʵ����ת
//*/
//void STM32_IAP_Protocol_Process(void)
//{
//	//����֡�ж�
//  volatile	unsigned char XorData=0x00;
//	volatile unsigned char data=0x00;
//	unsigned char Version_High = 0x00;
//	unsigned char Version_Low = 0x00;
//	Net_DigitalDataOut.FrameLength = Rx_Buffer[1];
//	if(Rx_Buffer[0] != 0xef) return; 
//	if(Net_DigitalDataOut.FrameLength<3)return ;
//	if(Net_DigitalDataOut.FrameLength>8)return;
//	XorData = GetXor(&Rx_Buffer[1],4);//��ȡ��������
//	if(XorData != Rx_Buffer[6])return ;//���У�鲻����
//	if(Rx_Buffer[Net_DigitalDataOut.FrameLength-1] !=0xff)return;//�����ַ�������
//	if(Rx_Buffer[2] ==0x08)//�������ж�
//	{
//		APP_Running_Flash = Rx_Buffer[3];//�����������Ǵ�����������
//		AT24CXX_Write(34,&APP_Running_Flash,1);//д�뵽AT24C02
//		Version_High = Rx_Buffer[4];
//		Version_Low = Rx_Buffer[5];
//	  AT24CXX_Write(32,&Version_High,1);//д�뵽AT24C02
//		AT24CXX_Write(33,&Version_Low,1);//д�뵽AT24C02
//	}
//	
//	
//	
//	
//	
//	

//	if(Rx_Buffer[2] ==0x05)//�鿴������
//	{
//		APP_Running_Flash = Rx_Buffer[3];
//		AT24CXX_Write(100,&APP_Running_Flash,1);//д�뵽AT24C02
//		Version_High = Rx_Buffer[4];
//		Version_Low = Rx_Buffer[5];
//	  AT24CXX_Write(32,&Version_High,1);//д�뵽AT24C02
//		AT24CXX_Write(33,&Version_Low,1);//д�뵽AT24C02
//	}
//	
//}




