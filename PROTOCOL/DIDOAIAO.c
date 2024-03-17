#include "DIDOAIAO.h"

//unsigned char AD7606_DataInput_Old[8]={0,0,0,0,0,0,0,0};
unsigned short DataExchangeNum =0x0000;
unsigned short ADC_Convert_new[6],ADC_Convert_old[6]={0,};

/* ���ļ����ڴ����°�ͨ��Э�顪�����������������ģ�����������*/

/*
ģ�����ɼ����������ɼ�������
*/
void DigitalData_AnalogData_InputProcess(void)
{
	unsigned char i=0,temp1=1,temp2=2;

	unsigned char AnalogDataInputNumber = 0x00; //ģ������������
	unsigned char NumberToSend = 0x00;//ģ�����ɼ����������ɼ����͵����ݸ���
	unsigned char AnalogDataSignAddress = 3;//ģ���������־λ��ַ
	unsigned char AnalogDataSign=0x00;//ģ�����ɼ�ͨ����־λ
    unsigned char AnalogDataSendSign=0x00; //���ڱ�Ƿ��͵�ģ�����ɼ�ͨ������
	
	//ģ�����ɼ� ����ģ�����ɼ�ͨ����־λ	(36λ)	
		memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
		AT24CXX_Read(34,&AnalogDataSign,1); //
	 if(AnalogDataSign)//��ȡ�����õ�ģ�����ɼ�ͨ������λ���õĶ�ȡͨ��
	 {
		 	for(i=0;i<8;i++)//��ȡͨ��ֵ
	    {
		    if(AnalogDataSign & (0x01<<i))//��������˸�ͨ����ģ�����ɼ�
		   {
			   ADC_Convert_new[i] = ReadADCAverageValue(i,250); //��ȡͨ������
				 //�������жϸ����ݺ;���ֵ�Ƿ���ͬ �����ͬ������Ҫ����
				 if(ADC_Convert_old[i] != ADC_Convert_new[i])
				 {
					 AnalogDataSendSign = AnalogDataSendSign | (AnalogDataSign & (0x01<<i)); 
					 AnalogDataInputNumber++;//����ģ�������ͨ��	
			     ADC_Convert_old[i] = ADC_Convert_new[i];
					 Tx_Buffer[AnalogDataSignAddress+temp1] = (unsigned char)(ADC_Convert_old[i]>>8);
					 Tx_Buffer[AnalogDataSignAddress+temp2] = (unsigned char)ADC_Convert_old[i];
					 temp1+=2;
					 temp2+=2; 					 
				 }				 
	 	   }			
	    }
   }

		//�������ɼ�	 
	DigitalInputNew.Digitalin_Bit.BIT1  = DI8;
	DigitalInputNew.Digitalin_Bit.BIT2  = DI7;
	DigitalInputNew.Digitalin_Bit.BIT3  = DI6;
	DigitalInputNew.Digitalin_Bit.BIT4  = DI5;
	DigitalInputNew.Digitalin_Bit.BIT5  = DI4;
	DigitalInputNew.Digitalin_Bit.BIT6  = DI3;
	DigitalInputNew.Digitalin_Bit.BIT7  = DI2;
	DigitalInputNew.Digitalin_Bit.BIT8  = DI1; 
	DigitalInputNew.Digitalin_Bit.BIT9  = DI16;
	DigitalInputNew.Digitalin_Bit.BIT10 = DI15;
	DigitalInputNew.Digitalin_Bit.BIT11 = DI14;
	DigitalInputNew.Digitalin_Bit.BIT12 = DI13;
	DigitalInputNew.Digitalin_Bit.BIT13 = DI12;
	DigitalInputNew.Digitalin_Bit.BIT14 = DI11;
	DigitalInputNew.Digitalin_Bit.BIT15 = DI10;
	DigitalInputNew.Digitalin_Bit.BIT16 = DI9;
	DigitalInputNew.Digitalin_Bit.BIT17 = DI24;
	DigitalInputNew.Digitalin_Bit.BIT18 = DI23;
	DigitalInputNew.Digitalin_Bit.BIT19 = DI22;
	DigitalInputNew.Digitalin_Bit.BIT20 = DI21;
	DigitalInputNew.Digitalin_Bit.BIT21 = DI20;
	DigitalInputNew.Digitalin_Bit.BIT22 = DI19;
	DigitalInputNew.Digitalin_Bit.BIT23 = DI18;
	DigitalInputNew.Digitalin_Bit.BIT24 = DI17;
	DigitalInputNew.Digitalin_Bit.BIT25 = DI32;
	DigitalInputNew.Digitalin_Bit.BIT26 = DI31;
	DigitalInputNew.Digitalin_Bit.BIT27 = DI30;
	DigitalInputNew.Digitalin_Bit.BIT28 = DI29;
	DigitalInputNew.Digitalin_Bit.BIT29 = DI28;
	DigitalInputNew.Digitalin_Bit.BIT30 = DI27;
	DigitalInputNew.Digitalin_Bit.BIT31 = DI26;
	DigitalInputNew.Digitalin_Bit.BIT32 = DI25;	
	if((DigitalInputNew.all != DigitalInputOld.all)||(AnalogDataSendSign!=0))
	{
		delay_ms(10);//��������
		 if((DigitalInputNew.all != DigitalInputOld.all)||(AnalogDataSendSign!=0))//�ٴ��ж��Ƿ�������״̬�����ı�
		 {
			/*������Ҫ�ж� �Ǵ����￪ʼ �����������״̬�ͷ����ı�*/
			if(DigitalInputNew.Digitalin_Byte.BYTE4 ^ DigitalInputOld.Digitalin_Byte.BYTE4)//���а�λ����������ֵ��ȫ��ͬ��������Ľ��Ϊ0������ͬ�������������0
			{
				Net_DigitalDataIn.NetTrans_Num = 4;
			}
			else if(DigitalInputNew.Digitalin_Byte.BYTE3^DigitalInputOld.Digitalin_Byte.BYTE3)
			{
				Net_DigitalDataIn.NetTrans_Num = 3;
			}
			else if(DigitalInputNew.Digitalin_Byte.BYTE2^DigitalInputOld.Digitalin_Byte.BYTE2)
			{
				Net_DigitalDataIn.NetTrans_Num = 2;
			}
			else if(DigitalInputNew.Digitalin_Byte.BYTE1^DigitalInputOld.Digitalin_Byte.BYTE1)
			{
				Net_DigitalDataIn.NetTrans_Num = 1;
			}
			else Net_DigitalDataIn.NetTrans_Num = 0;
		}
	}
	//������Ҫ���µ�����������ֵ��ֵ����������
		Net_DigitalDataIn.Digital_in.all = DigitalInputNew.all; //�Ѳɼ�����������Ϣ��ֵ���������ݣ����ڷ��͵���λ��
		DigitalInputOld.all = DigitalInputNew.all;//����һ�βɼ������ݸ�ֵ��DigitalInputOld����������Ա��´ζ�DigitalInputNew��DigitalInputOld���бȽ�
	
	//��һ�����  ��������  ģ����û��
	if((Net_DigitalDataIn.NetTrans_Num)&&(AnalogDataSendSign==0))
	{
		switch(Net_DigitalDataIn.NetTrans_Num)
		{
			case 1:
			  Tx_Buffer[4] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;		
        NumberToSend =	7;	//6+1	
				break;
			case 2:
				Tx_Buffer[4] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
				Tx_Buffer[5] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
			  NumberToSend =	8;	
				break;
			case 3:
				Tx_Buffer[4] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
				Tx_Buffer[5] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
				Tx_Buffer[6] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE3;
				NumberToSend = 9;
				break;
			case 4:
				Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
				Tx_Buffer[5+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
				Tx_Buffer[6+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE3;
				Tx_Buffer[7+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE4;	
   			NumberToSend = 10; //4���������ɼ�����������2*AnalogDataInputNumber��ģ������������ 6����ʼ�ַ���
				break;
			default:
				break;
		}
		//���ϴ������ģ�����ɼ����������ɼ������������ɼ����ݽ��з���
			Tx_Buffer[0] = 0xfe;//��ʼ�ַ�������Ϊ0xfe
			Tx_Buffer[1] = NumberToSend;//֡����
			Tx_Buffer[2] = 0x01;//�����룬����Ϊ0x01
			Tx_Buffer[3] = AnalogDataSendSign;
			Tx_Buffer[NumberToSend-2]  = GetXor(&Tx_Buffer[1],NumberToSend-4);
			Tx_Buffer[NumberToSend-1] = 0xff;//�����ַ�
			//д��SD��
			if(1==SD_Check()&&firstsendDIAI) //��һ�β����Ͳ���¼
			SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
			if(firstsendDIAI)//��һ�β��÷���
			{
				sendto(SOCK_UDPS,Tx_Buffer,NumberToSend, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);				
				DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
				Net_DigitalDataIn.NetTrans_Num = 0;
				DataSent = TRUE; 										
			}
			firstsendDIAI = 0x01;		
	}	
	//�ڶ������   ������û��  ģ������
	if((Net_DigitalDataIn.NetTrans_Num==0)&&(AnalogDataSendSign))
	{		
		NumberToSend = 2*AnalogDataInputNumber+6;//���㷢�����ݵĸ���
				//���ϴ������ģ�����ɼ����������ɼ������������ɼ����ݽ��з���
		Tx_Buffer[0] = 0xfe;//��ʼ�ַ�������Ϊ0xfe
		Tx_Buffer[1] = NumberToSend;//֡����
		Tx_Buffer[2] = 0x01;//�����룬����Ϊ0x01
		Tx_Buffer[3] = AnalogDataSendSign;
		Tx_Buffer[NumberToSend-2]  = GetXor(&Tx_Buffer[1],NumberToSend-4);
		Tx_Buffer[NumberToSend-1] = 0xff;//�����ַ�
		//д��SD��
		if(1==SD_Check()&&firstsendDIAI) //��һ�β����Ͳ���¼
		SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
		if(firstsendDIAI)//��һ�β��÷���
		{
			sendto(SOCK_UDPS,Tx_Buffer,NumberToSend, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);				
			DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
			Net_DigitalDataIn.NetTrans_Num = 0;
			DataSent = TRUE; 									
		}
		firstsendDIAI = 0x01;	
	}	
	//���������  �������� ģ���� ��
	if((Net_DigitalDataIn.NetTrans_Num)&&(AnalogDataSendSign))
	{
			 //����������Э�鷢�ͳ�������
			//           DigitalDataInput_NetExchange();
			 switch (Net_DigitalDataIn.NetTrans_Num) //���������뷢�ʹ���
			 {
				 case 4:
					 Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
					 Tx_Buffer[5+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
					 Tx_Buffer[6+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE3;
					 Tx_Buffer[7+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE4;	
					 NumberToSend = 4+2*AnalogDataInputNumber+6; //4���������ɼ�����������2*AnalogDataInputNumber��ģ������������ 6����ʼ�ַ���
					 break;
				 case 3:
					 Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
					 Tx_Buffer[5+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
					 Tx_Buffer[6+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE3;
						NumberToSend = 3+2*AnalogDataInputNumber+6;
					break;
				 case 2:
					 Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
					 Tx_Buffer[5+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
					 NumberToSend = 2+2*AnalogDataInputNumber+6;
					 break;
				 case 1:
					 Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
					 NumberToSend = 1+2*AnalogDataInputNumber+6;
					 break;
				 case 0:
					NumberToSend = 2*AnalogDataInputNumber+6;
				 default:
					 break;						 
				}				
				//���ϴ������ģ�����ɼ����������ɼ������������ɼ����ݽ��з���
				Tx_Buffer[0] = 0xfe;//��ʼ�ַ�������Ϊ0xfe
				Tx_Buffer[1] = NumberToSend;//֡����
				Tx_Buffer[2] = 0x01;//�����룬����Ϊ0x01
				Tx_Buffer[3] = AnalogDataSendSign;
				Tx_Buffer[NumberToSend-2]  = GetXor(&Tx_Buffer[1],NumberToSend-4);
				Tx_Buffer[NumberToSend-1] = 0xff;//�����ַ�
				//д��SD��
				if(1==SD_Check()&&firstsendDIAI) //��һ�β����Ͳ���¼
				SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
				if(firstsendDIAI)//��һ�β��÷���
				{
					sendto(SOCK_UDPS,Tx_Buffer,NumberToSend, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);				
					DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
					Net_DigitalDataIn.NetTrans_Num = 0;
					DataSent = TRUE; 											
				}
				firstsendDIAI = 0x01;
     }	
}


/*
�忨����״̬��ѯ
*/
void DigitalData_AnalogData_QueryState(void)
{
	unsigned char i=0,temp1=1,temp2=2;
	unsigned short ADC_Convert[6]={0,};
	unsigned char AnalogDataInputNumber = 0x00; //ģ������������
	unsigned char NumberToSend = 0x00;//ģ�����ɼ����������ɼ����͵����ݸ���
	unsigned char AnalogDataSignAddress = 3;//ģ���������־λ��ַ
	unsigned char AnalogDataSign=0x00;//ģ�����ɼ�ͨ����־λ
	
//	unsigned char a=0;
		memset(Tx_Buffer,0x00,strlen(Tx_Buffer));
	//ģ�����ɼ� ����ģ�����ɼ�ͨ����־λ	(34λ)	
		AT24CXX_Read(34,&AnalogDataSign,1); //
	 for(i=0;i<8;i++)//��ȡͨ��ֵ
	 {
		 if(AnalogDataSign & (0x01<<i))
		 {
			 ADC_Convert[i] = ReadADCAverageValue(i,200); //��ȡģ�����ɼ�����
		 }			
	 }
		if(AnalogDataSign)//�����ģ����ͨ���ɼ�
		{
			for(i=0;i<8;i++)
			{			
				if(AnalogDataSign & (0x01<<i))//�ж����ͨ��
				{					
//						a = AnalogDataSign & (0x01<<i);
					  AnalogDataInputNumber++;//����ģ�������ͨ��	
						Tx_Buffer[AnalogDataSignAddress+temp1] = (unsigned char)(ADC_Convert[i]>>8);
						Tx_Buffer[AnalogDataSignAddress+temp2] = (unsigned char)ADC_Convert[i];
						temp1+=2;
						temp2+=2;				
				}
			}
		}
    //�������ɼ�		 
  DigitalInputNew.Digitalin_Bit.BIT1  = DI8;
	DigitalInputNew.Digitalin_Bit.BIT2  = DI7;
	DigitalInputNew.Digitalin_Bit.BIT3  = DI6;
	DigitalInputNew.Digitalin_Bit.BIT4  = DI5;
	DigitalInputNew.Digitalin_Bit.BIT5  = DI4;
	DigitalInputNew.Digitalin_Bit.BIT6  = DI3;
	DigitalInputNew.Digitalin_Bit.BIT7  = DI2;
	DigitalInputNew.Digitalin_Bit.BIT8  = DI1; 
	DigitalInputNew.Digitalin_Bit.BIT9  = DI16;
	DigitalInputNew.Digitalin_Bit.BIT10 = DI15;
	DigitalInputNew.Digitalin_Bit.BIT11 = DI14;
	DigitalInputNew.Digitalin_Bit.BIT12 = DI13;
	DigitalInputNew.Digitalin_Bit.BIT13 = DI12;
	DigitalInputNew.Digitalin_Bit.BIT14 = DI11;
	DigitalInputNew.Digitalin_Bit.BIT15 = DI10;
	DigitalInputNew.Digitalin_Bit.BIT16 = DI9;
	DigitalInputNew.Digitalin_Bit.BIT17 = DI24;
	DigitalInputNew.Digitalin_Bit.BIT18 = DI23;
	DigitalInputNew.Digitalin_Bit.BIT19 = DI22;
	DigitalInputNew.Digitalin_Bit.BIT20 = DI21;
	DigitalInputNew.Digitalin_Bit.BIT21 = DI20;
	DigitalInputNew.Digitalin_Bit.BIT22 = DI19;
	DigitalInputNew.Digitalin_Bit.BIT23 = DI18;
	DigitalInputNew.Digitalin_Bit.BIT24 = DI17;
	DigitalInputNew.Digitalin_Bit.BIT25 = DI32;
	DigitalInputNew.Digitalin_Bit.BIT26 = DI31;
	DigitalInputNew.Digitalin_Bit.BIT27 = DI30;
	DigitalInputNew.Digitalin_Bit.BIT28 = DI29;
	DigitalInputNew.Digitalin_Bit.BIT29 = DI28;
	DigitalInputNew.Digitalin_Bit.BIT30 = DI27;
	DigitalInputNew.Digitalin_Bit.BIT31 = DI26;
	DigitalInputNew.Digitalin_Bit.BIT32 = DI25;	
	
	Tx_Buffer[4+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE1;
	Tx_Buffer[5+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE2;
	Tx_Buffer[6+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE3;
	Tx_Buffer[7+2*AnalogDataInputNumber] = Net_DigitalDataIn.Digital_in.Digitalin_Byte.BYTE4;	
	NumberToSend = 4+2*AnalogDataInputNumber+6; //4���������ɼ�����������2*AnalogDataInputNumber��ģ������������ 6����ʼ�ַ���
	
	  //���ϴ������ģ�����ɼ����������ɼ������������ɼ����ݽ��з���
	Tx_Buffer[0] = 0xfe;//��ʼ�ַ�������Ϊ0xfe
	Tx_Buffer[1] = NumberToSend;//֡����
	Tx_Buffer[2] = 0x02;//�����룬����Ϊ0x02 ��ѯ״̬�Ĺ�����Ϊ0x02
	Tx_Buffer[3] = AnalogDataSign;
	Tx_Buffer[NumberToSend-2]  = GetXor(&Tx_Buffer[1],NumberToSend-4);
	Tx_Buffer[NumberToSend-1] = 0xff;//�����ַ�

	//д��SD��
  if(1==SD_Check())	
	SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
	sendto(SOCK_UDPS,Tx_Buffer,NumberToSend, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);	
  DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���	
	Net_DigitalDataIn.NetTrans_Num = 0;
	DataSent = TRUE; 
	
		 		 
	}	


/*************************************************************************************************************/
/**                                 ���������������                                                  ***/
/*�˴��ص���Ҫ������������ݵ�˳����DO���ݵ�˳��*/
/*************************************************************************************************************/
void DigitalData_Output_Process(void)
{
	if(Net_DigitalDataOut.Digital_out.all !=DigitalOutOld.all)//���緢�͹�����������ԭ�����ݲ�ͬ��˵������������и���
	{
			DigitalOutOld.all = Net_DigitalDataOut.Digital_out.all;
			delay_ms(10);
			DO1(DigitalOutOld.digitaloutbit.BIT1);
			DO2(DigitalOutOld.digitaloutbit.BIT2);
			DO3(DigitalOutOld.digitaloutbit.BIT3);
			DO4(DigitalOutOld.digitaloutbit.BIT4);
			DO5(DigitalOutOld.digitaloutbit.BIT5);
			DO6(DigitalOutOld.digitaloutbit.BIT6);
			DO7(DigitalOutOld.digitaloutbit.BIT7);
			DO8(DigitalOutOld.digitaloutbit.BIT8);
			DO9(DigitalOutOld.digitaloutbit.BIT9);
			DO10(DigitalOutOld.digitaloutbit.BIT10);
			DO11(DigitalOutOld.digitaloutbit.BIT11);
			DO12(DigitalOutOld.digitaloutbit.BIT12);
			DO13(DigitalOutOld.digitaloutbit.BIT13);
			DO14(DigitalOutOld.digitaloutbit.BIT14);
			DO15(DigitalOutOld.digitaloutbit.BIT15);
			DO16(DigitalOutOld.digitaloutbit.BIT16);
			DO17(DigitalOutOld.digitaloutbit.BIT17);
			DO18(DigitalOutOld.digitaloutbit.BIT18);
			DO19(DigitalOutOld.digitaloutbit.BIT19);
			DO20(DigitalOutOld.digitaloutbit.BIT20);
			DO21(DigitalOutOld.digitaloutbit.BIT21);
			DO22(DigitalOutOld.digitaloutbit.BIT22);
			DO23(DigitalOutOld.digitaloutbit.BIT23);
			DO24(DigitalOutOld.digitaloutbit.BIT24);
			DO25(DigitalOutOld.digitaloutbit.BIT25);
			DO26(DigitalOutOld.digitaloutbit.BIT26);
			DO27(DigitalOutOld.digitaloutbit.BIT27);
			DO28(DigitalOutOld.digitaloutbit.BIT28);
			DO29(DigitalOutOld.digitaloutbit.BIT29);
			DO30(DigitalOutOld.digitaloutbit.BIT30);
			DO31(DigitalOutOld.digitaloutbit.BIT31);
			DO32(DigitalOutOld.digitaloutbit.BIT32);
			
	}	
}


/*
ģ��������������������������������λ�����͵����ָ���������
*/
void DigitalData_AnalogData_OutputProcess(unsigned short pointer)
{	
	unsigned int Temp1=0,Temp2=0;
	unsigned int XorDataTemp,AnalogCh_Num=0,DigitalDataByte_Num=0;
	unsigned char Digital_Data_Output_Start=0x00,i=0;
	unsigned char AnalogDataStart=0x00;
	Net_DigitalDataOut.FrameLength = Rx_Buffer[pointer+1]; //֡����
	Net_DigitalDataOut.AnalogCh_Sign = Rx_Buffer[pointer+3]; //ģ����ͨ����־λ
	AnalogDataStart = pointer+4;
    Temp2 = AnalogDataStart; //ģ�������������ʼλ��
	
     if(Net_DigitalDataOut.AnalogCh_Sign) //�����ģ�������
		 {
			  for (Temp1=0;Temp1<8;Temp1++)
			 {
				 if(Net_DigitalDataOut.AnalogCh_Sign &(0x01<<Temp1)) //�ж�ͨ���Ƿ������
				  {
					  AnalogCh_Num++;	//ͨ��ѡͨ�ĸ���	
					  if(Net_DigitalDataOut.Analog_Data[Temp1] != Rx_Buffer[AnalogCh_Num+3])
					  {
						  Net_DigitalDataOut.Analog_Data[Temp1] = Rx_Buffer[AnalogCh_Num+3];
						  AD5676_SendData(AD5676_DAC[Temp1],Net_DigitalDataOut.Analog_Data[Temp1]);
					  }
				  }				
			 }	
        // if(Net_DigitalDataOut.FrameLength<6) return ;//�������ݴ���			 
		    	
		}
		//���������
		DigitalDataByte_Num = Net_DigitalDataOut.FrameLength-6-AnalogCh_Num;//��������������ֽ��� ��5 �Ǽ�ȥ ��ʼ�ַ���֡���ȡ�ģ������־λ�������顢�����ַ���Ȼ���ȥģ����������ݣ�ʣ�µ����������������
		Digital_Data_Output_Start = AnalogCh_Num+4;
		if(DigitalDataByte_Num)//��������������
		{
			//�ж�DigitalDataByte_Num�ĸ���
			switch (DigitalDataByte_Num)
			{
				case 0x04:
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE1 = Rx_Buffer[Digital_Data_Output_Start];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE2 = Rx_Buffer[Digital_Data_Output_Start+1];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE3 = Rx_Buffer[Digital_Data_Output_Start+2];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE4 = Rx_Buffer[Digital_Data_Output_Start+3];
					break;
				case 0x03:
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE1 = Rx_Buffer[Digital_Data_Output_Start];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE2 = Rx_Buffer[Digital_Data_Output_Start+1];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE3 = Rx_Buffer[Digital_Data_Output_Start+2];
					break;
				case 0x02:
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE1 = Rx_Buffer[Digital_Data_Output_Start];
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE2 = Rx_Buffer[Digital_Data_Output_Start+1];
					break;
				case 0x01:
					Net_DigitalDataOut.Digital_out.digitaloutbyte.BYTE1 = Rx_Buffer[Digital_Data_Output_Start];					
					break;
				default:
					break;
			 }	
			DigitalData_Output_Process();		
	    }	
			 //�ѽ��յ������ݷ��͸���λ��
			 for(i=0;i<Net_DigitalDataOut.FrameLength;i++)
			 {
				 Tx_Buffer[i] = Rx_Buffer[i];
			 }
			 if(1==SD_Check())	
	     SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
//	     sendto(SOCK_UDPS,Tx_Buffer,Net_DigitalDataOut.FrameLength, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);	
//       DataExchangeNum=DataExchangeNum+1;//��¼�������ݵĴ���			
}

/*
�ϵ��λ����Զ����Թ��̳���
*/
void AODO_test(void)
{
	
//	DO1(1);
//	delay_ms(10);
//	DO2(1);
//	delay_ms(10);
//	DO3(1);
//  delay_ms(10);
//  DO4(1);
//  delay_ms(10);
//	DO5(1);
//	delay_ms(10);
//	DO6(1);
//  delay_ms(10);
//	DO7(1);
//	delay_ms(10);
//  DO8(1);
//	delay_ms(10);
//	DO9(1);
//  delay_ms(10);
//  DO10(1);
//  delay_ms(10);
//	DO11(1);
//  delay_ms(10);
//	DO11(1);
//	delay_ms(10);
//	DO12(1);
//  delay_ms(10);
//	DO13(1);
//  delay_ms(10);
//	DO14(1);
//  delay_ms(10);
//	DO15(1);
//  delay_ms(10);
//	DO16(1);
//  delay_ms(10);
//	DO17(1);	
//  delay_ms(10);
//	DO18(1);
//  delay_ms(10);
//	DO19(1);
//  delay_ms(10);
//	DO20(1);
//	delay_ms(10);
//	DO21(1);
//  delay_ms(10);
//	DO22(1);
//  delay_ms(10);
//	DO23(1);
//  delay_ms(10);
//  DO25(1);
//  delay_ms(10);
//	DO26(1);
//  delay_ms(10);
//	DO27(1);
//  delay_ms(10);
//	DO28(1);
//  delay_ms(10);
//	DO29(1);
//  delay_ms(10);
//	DO30(1);
//  delay_ms(10);
//	DO31(1);
//  delay_ms(10);
//	DO32(1);
//  delay_ms(10);
  DO1(0);
	DO2(0);
	DO3(0);
	DO4(0);
	DO5(0);
	DO6(0);
	DO7(0);
	DO8(0);
	DO9(0);
	DO10(0);
	DO11(0);
	DO12(0);
	DO13(0);
	DO14(0);
	DO15(0);
	DO16(0);
	DO17(0);
	DO18(0);
	DO19(0);
	DO20(0);
	DO21(0);
	DO22(0);
	DO23(0);
	DO24(0);
	DO25(0);
	DO26(0);
	DO27(0);
	DO28(0);
	DO29(0);
	DO30(0);
	DO31(0);
	DO32(0);
	SPI2_Send3Byte(0x0030cccc);
	SPI2_Send3Byte(0x0031cccc);
	SPI2_Send3Byte(0x0032cccc);
	SPI2_Send3Byte(0x0033cccc);
	SPI2_Send3Byte(0x0034cccc);
	SPI2_Send3Byte(0x0035cccc);
	SPI2_Send3Byte(0x0036cccc);
	SPI2_Send3Byte(0x0037cccc);
	delay_ms(100);
	SPI2_Send3Byte(0x00300000);
	SPI2_Send3Byte(0x00310000);
	SPI2_Send3Byte(0x00320000);
	SPI2_Send3Byte(0x00330000);
	SPI2_Send3Byte(0x00340000);
	SPI2_Send3Byte(0x00350000);
	SPI2_Send3Byte(0x00360000);
	SPI2_Send3Byte(0x00370000);
	delay_ms(100);
}

//���ݲɼ��ȽϺ��� ��H�������뷢�͵�ģ�����������ɼ����жԱ�
//pointer ��ʾ֡�����ݵĵ�ַ
void DigitalAnalogData_Compare(unsigned short pointer)
{
  unsigned char i;
	for(i=0;i<Net_DigitalDataIn.NetTrans_Num;i++)
	{
		if(BUFFER_SAVE[i] != Rx_Buffer[pointer++]){pointer++; break;}
	}
	if(i<Net_DigitalDataIn.NetTrans_Num)//���պͷ��͵����ݲ�һ��
	{
		for(i=0;i<Net_DigitalDataIn.NetTrans_Num;i++)
		{
			Tx_Buffer[i] = BUFFER_SAVE[i];//���ϴη��͵���ֵ���·���һ��
		}
		if(Digital_Analog_Inputtimes<3)
		{
		   sendto(SOCK_UDPS,Tx_Buffer,Net_DigitalDataIn.NetTrans_Num, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);				
       Digital_Analog_Inputtimes++;
		}
		else//�Աȴ�������2��
		{
			memset(BUFFER_SAVE,0,26);
			Tx_Buffer[0]= 0xfe;
			Tx_Buffer[1]= 0x07;
			Tx_Buffer[2]= 0x87;
			Tx_Buffer[3]= 0x01;//�������ɼ���ģ�����ɼ�����֡����
			Tx_Buffer[4]= 0x06;
			Tx_Buffer[5]= GetXor(&Tx_Buffer[1],2);
			Tx_Buffer[6]= 0xff;
			sendto(SOCK_UDPS,Tx_Buffer,7, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);						
		}
	}
}


