#include "24cxx.h"
#include "delay.h"

unsigned char  I2C_BUFF[37];
//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init();//IIC��ʼ��
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(511);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(511,0X55);
	    temp=AT24CXX_ReadOneByte(511);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}




void AT24C02_System_Config_Initialiation(void)
{
	unsigned char i =0;
	for(i=0;i<34;i++)
	{
		I2C_BUFF[i] = 0x00; //init the I2C_BUFF
	}
	 I2C_BUFF[0] = AT24CXX_ReadLenByte(0,1);
	
	if((I2C_BUFF[0] == 0xff)||(I2C_BUFF[0] == 0x00)) //���AT24C02û�б��޸Ĺ�
	{
		I2C_BUFF[0] = 0xaa;		 //�Ѿ��޸ĵı�־��	
		
		I2C_BUFF[1] = 172;		//����IP			  	
		I2C_BUFF[2] = 18;					  
		I2C_BUFF[3] = 137;
		I2C_BUFF[4] = 241;	
		
		I2C_BUFF[5] = 172;	//Ŀ��IP
		I2C_BUFF[6] = 18;
		I2C_BUFF[7] = 137;
		I2C_BUFF[8] = 31;		
		
	    I2C_BUFF[9] = 255;	//��������		
		I2C_BUFF[10] = 255;
		I2C_BUFF[11] = 0;
		I2C_BUFF[12] = 0;			
		
		I2C_BUFF[13] = 172;				//���ص�ַ		
		I2C_BUFF[14] = 18;
		I2C_BUFF[15] = 137;
		I2C_BUFF[16] = 1;	
		
	    I2C_BUFF[17] = W5500_Parameter_Configuration.MAC_Addr[0];		 //MAC��ַ
        I2C_BUFF[18] = W5500_Parameter_Configuration.MAC_Addr[1];		
        I2C_BUFF[19] = W5500_Parameter_Configuration.MAC_Addr[2];		
        I2C_BUFF[20] = W5500_Parameter_Configuration.MAC_Addr[3];		
        I2C_BUFF[21] = W5500_Parameter_Configuration.MAC_Addr[4];		
        I2C_BUFF[22] = W5500_Parameter_Configuration.MAC_Addr[5];	
		
		
		
		I2C_BUFF[23] = 0x13; //����UDPͨ�Ŷ˿ڵĶ˿ں�Ϊ5000  udp_localport
		I2C_BUFF[24] = 0x88; 
		
		I2C_BUFF[25] = 0x17; //����TCPͨ�Ŷ˿ڵĶ˿ں�Ϊ6000  tcp_localport
		I2C_BUFF[26] = 0x70; 
		
  	    I2C_BUFF[27] = 0x23;						
		I2C_BUFF[28] = 0x28;	//udp_remoteport ����UDPͨ�Ŷ˿ڵ�Ŀ��˿ں� 90000	
		
		I2C_BUFF[29] = 0x1f;						
		I2C_BUFF[30] = 0x40;	// 8000   tcp_remoteport ����TCPͨ�Ŷ˿ڵ�Ŀ��˿ںţ��������HTTP�Ļ�����Ҫ���ó�80   
		//����汾��Ϣ
		I2C_BUFF[31] = 0x00;//V1.2 �е�1
		I2C_BUFF[32] = 0x00; //V1.2�е�2
		I2C_BUFF[33] = 0x01;//�̼�������дλ�ñ�־��
		I2C_BUFF[34] = 0x00;//ģ�����ɼ�ͨ�����ñ�־λ
		
		
	 //ģ����ͨ������,�忨һ����8��ģ�����ɼ������ս�ͨ����λ����ʵ��ͨ������
//	  I2C_BUFF[31] = 0x01;// //Ĭ�Ͽ�����ģ�����ɼ�ͨ����0 ��Ҫͨ����λ����ʵ���޸� 
		//Ӳ���汾��¼
		
//		I2C_BUFF[33] Ϊģ�����ɼ�ͨ������
//		I2C_BUFF[34] = 0x00; //APP�������� Ĭ��Ϊ0x01
//��34��ΪAPP�������� �������ﲻ�ܽ����ʼ��������ÿ�ζ���0x00
		
		AT24CXX_Write(0,I2C_BUFF,35);  //�������������д�뵽AT24C04��
		
	  for(i =1;i<5;i++)
			W5500_Parameter_Configuration.LOCAL_IP_Addr[i-1] = I2C_BUFF[i];  //����IP��ַ				
		for(i=5;i<9;i++)
		  W5500_Parameter_Configuration.REMOTE_Sn_DIP[i-5] = I2C_BUFF[i];  //����Ŀ��IP��ַ 
		for(i=9;i<13;i++)
		  W5500_Parameter_Configuration.Sub_Mask[i-9] = I2C_BUFF[i];  //��������
		for(i=13;i<17;i++)
		  W5500_Parameter_Configuration.Gateway_IP[i-13] = I2C_BUFF[i]; //���ص�ַ
		for(i=17;i<23;i++)
		  W5500_Parameter_Configuration.MAC_Addr[i-17] = I2C_BUFF[i];//MAC��ַ
		//����UDPͨ�Ŷ˿ڵĶ˿ں�Ϊ5000  udp_localport
		W5500_Parameter_Configuration.Sn_UDP_Port = (I2C_BUFF[23]<<8)+I2C_BUFF[24];//����UDPͨ�Ŷ˿ڵĶ˿ں�Ϊ5000  udp_localport
		W5500_Parameter_Configuration.Sn_TCP_Port = (I2C_BUFF[25]<<8)+I2C_BUFF[26];//����TCPͨ�Ŷ˿ڵĶ˿ں�Ϊ6000  tcp_localport
		W5500_Parameter_Configuration.Sn_UDP_DPort = (I2C_BUFF[27]<<8)+I2C_BUFF[28];	
	  W5500_Parameter_Configuration.Sn_TCP_DPort = (I2C_BUFF[29]<<8)+I2C_BUFF[30];		
	}
	else //���AT24C02�Ѿ����޸��ˣ����AT24C02�ж������� д����Ӧ������
	{
    for(i=0;i<34;i++)
     	I2C_BUFF[i] = 0x00; //init the I2C_BUFF
		AT24CXX_Read(0,I2C_BUFF,34);//��AT24C02�ж�ȡ����		
		for(i =1;i<5;i++)
			W5500_Parameter_Configuration.LOCAL_IP_Addr[i-1] = I2C_BUFF[i];  //����IP��ַ   
		for(i=5;i<9;i++)
		  W5500_Parameter_Configuration.REMOTE_Sn_DIP[i-5] = I2C_BUFF[i];  //����Ŀ��IP��ַ 		
		for(i=9;i<13;i++)
		  W5500_Parameter_Configuration.Sub_Mask[i-9] = I2C_BUFF[i];  //��������	
		for(i=13;i<17;i++)
		  W5500_Parameter_Configuration.Gateway_IP[i-13] = I2C_BUFF[i]; //���ص�ַ		
		for(i=17;i<23;i++)
		  W5500_Parameter_Configuration.MAC_Addr[i-17] = I2C_BUFF[i];//MAC��ַ
		//����UDPͨ�Ŷ˿ڵĶ˿ں�Ϊ5000  udp_localport
		W5500_Parameter_Configuration.Sn_UDP_Port = (I2C_BUFF[23]<<8)+I2C_BUFF[24];//����UDPͨ�Ŷ˿ڵĶ˿ں�Ϊ5000  udp_localport
		W5500_Parameter_Configuration.Sn_TCP_Port = (I2C_BUFF[25]<<8)+I2C_BUFF[26];//����TCPͨ�Ŷ˿ڵĶ˿ں�Ϊ6000  tcp_localport		
		W5500_Parameter_Configuration.Sn_UDP_DPort = (I2C_BUFF[27]<<8)+I2C_BUFF[28];			
	    W5500_Parameter_Configuration.Sn_TCP_DPort = (I2C_BUFF[29]<<8)+I2C_BUFF[30];
	}	
	
	//*APP������������ѡ������ ��AT24C02��100����*/
	
}

//35 ��IP���ͱ�־λ  36��ģ�����ɼ�ͨ������λ

