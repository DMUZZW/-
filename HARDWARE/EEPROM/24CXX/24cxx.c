#include "24cxx.h"
#include "delay.h"

unsigned char  I2C_BUFF[37];
//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();//IIC初始化
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(511);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(511,0X55);
	    temp=AT24CXX_ReadOneByte(511);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
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
	
	if((I2C_BUFF[0] == 0xff)||(I2C_BUFF[0] == 0x00)) //如果AT24C02没有别修改过
	{
		I2C_BUFF[0] = 0xaa;		 //已经修改的标志码	
		
		I2C_BUFF[1] = 172;		//本机IP			  	
		I2C_BUFF[2] = 18;					  
		I2C_BUFF[3] = 137;
		I2C_BUFF[4] = 241;	
		
		I2C_BUFF[5] = 172;	//目标IP
		I2C_BUFF[6] = 18;
		I2C_BUFF[7] = 137;
		I2C_BUFF[8] = 31;		
		
	    I2C_BUFF[9] = 255;	//子网掩码		
		I2C_BUFF[10] = 255;
		I2C_BUFF[11] = 0;
		I2C_BUFF[12] = 0;			
		
		I2C_BUFF[13] = 172;				//网关地址		
		I2C_BUFF[14] = 18;
		I2C_BUFF[15] = 137;
		I2C_BUFF[16] = 1;	
		
	    I2C_BUFF[17] = W5500_Parameter_Configuration.MAC_Addr[0];		 //MAC地址
        I2C_BUFF[18] = W5500_Parameter_Configuration.MAC_Addr[1];		
        I2C_BUFF[19] = W5500_Parameter_Configuration.MAC_Addr[2];		
        I2C_BUFF[20] = W5500_Parameter_Configuration.MAC_Addr[3];		
        I2C_BUFF[21] = W5500_Parameter_Configuration.MAC_Addr[4];		
        I2C_BUFF[22] = W5500_Parameter_Configuration.MAC_Addr[5];	
		
		
		
		I2C_BUFF[23] = 0x13; //设置UDP通信端口的端口号为5000  udp_localport
		I2C_BUFF[24] = 0x88; 
		
		I2C_BUFF[25] = 0x17; //设置TCP通信端口的端口号为6000  tcp_localport
		I2C_BUFF[26] = 0x70; 
		
  	    I2C_BUFF[27] = 0x23;						
		I2C_BUFF[28] = 0x28;	//udp_remoteport 设置UDP通信端口的目标端口号 90000	
		
		I2C_BUFF[29] = 0x1f;						
		I2C_BUFF[30] = 0x40;	// 8000   tcp_remoteport 设置TCP通信端口的目标端口号，如果用于HTTP的话，需要设置成80   
		//软件版本信息
		I2C_BUFF[31] = 0x00;//V1.2 中的1
		I2C_BUFF[32] = 0x00; //V1.2中的2
		I2C_BUFF[33] = 0x01;//固件程序烧写位置标志码
		I2C_BUFF[34] = 0x00;//模拟量采集通道设置标志位
		
		
	 //模拟量通道数量,板卡一共有8个模拟量采集，最终将通过上位机来实现通道数量
//	  I2C_BUFF[31] = 0x01;// //默认开启的模拟量采集通道是0 需要通过上位机来实现修改 
		//硬件版本记录
		
//		I2C_BUFF[33] 为模拟量采集通道数量
//		I2C_BUFF[34] = 0x00; //APP运行区域 默认为0x01
//第34个为APP运行区域 ，但这里不能将其初始化，否则每次都是0x00
		
		AT24CXX_Write(0,I2C_BUFF,35);  //把上述网络参数写入到AT24C04中
		
	  for(i =1;i<5;i++)
			W5500_Parameter_Configuration.LOCAL_IP_Addr[i-1] = I2C_BUFF[i];  //设置IP地址				
		for(i=5;i<9;i++)
		  W5500_Parameter_Configuration.REMOTE_Sn_DIP[i-5] = I2C_BUFF[i];  //设置目的IP地址 
		for(i=9;i<13;i++)
		  W5500_Parameter_Configuration.Sub_Mask[i-9] = I2C_BUFF[i];  //子网掩码
		for(i=13;i<17;i++)
		  W5500_Parameter_Configuration.Gateway_IP[i-13] = I2C_BUFF[i]; //网关地址
		for(i=17;i<23;i++)
		  W5500_Parameter_Configuration.MAC_Addr[i-17] = I2C_BUFF[i];//MAC地址
		//设置UDP通信端口的端口号为5000  udp_localport
		W5500_Parameter_Configuration.Sn_UDP_Port = (I2C_BUFF[23]<<8)+I2C_BUFF[24];//设置UDP通信端口的端口号为5000  udp_localport
		W5500_Parameter_Configuration.Sn_TCP_Port = (I2C_BUFF[25]<<8)+I2C_BUFF[26];//设置TCP通信端口的端口号为6000  tcp_localport
		W5500_Parameter_Configuration.Sn_UDP_DPort = (I2C_BUFF[27]<<8)+I2C_BUFF[28];	
	  W5500_Parameter_Configuration.Sn_TCP_DPort = (I2C_BUFF[29]<<8)+I2C_BUFF[30];		
	}
	else //如果AT24C02已经被修改了，则从AT24C02中读出数据 写入相应参数中
	{
    for(i=0;i<34;i++)
     	I2C_BUFF[i] = 0x00; //init the I2C_BUFF
		AT24CXX_Read(0,I2C_BUFF,34);//从AT24C02中读取数据		
		for(i =1;i<5;i++)
			W5500_Parameter_Configuration.LOCAL_IP_Addr[i-1] = I2C_BUFF[i];  //设置IP地址   
		for(i=5;i<9;i++)
		  W5500_Parameter_Configuration.REMOTE_Sn_DIP[i-5] = I2C_BUFF[i];  //设置目的IP地址 		
		for(i=9;i<13;i++)
		  W5500_Parameter_Configuration.Sub_Mask[i-9] = I2C_BUFF[i];  //子网掩码	
		for(i=13;i<17;i++)
		  W5500_Parameter_Configuration.Gateway_IP[i-13] = I2C_BUFF[i]; //网关地址		
		for(i=17;i<23;i++)
		  W5500_Parameter_Configuration.MAC_Addr[i-17] = I2C_BUFF[i];//MAC地址
		//设置UDP通信端口的端口号为5000  udp_localport
		W5500_Parameter_Configuration.Sn_UDP_Port = (I2C_BUFF[23]<<8)+I2C_BUFF[24];//设置UDP通信端口的端口号为5000  udp_localport
		W5500_Parameter_Configuration.Sn_TCP_Port = (I2C_BUFF[25]<<8)+I2C_BUFF[26];//设置TCP通信端口的端口号为6000  tcp_localport		
		W5500_Parameter_Configuration.Sn_UDP_DPort = (I2C_BUFF[27]<<8)+I2C_BUFF[28];			
	    W5500_Parameter_Configuration.Sn_TCP_DPort = (I2C_BUFF[29]<<8)+I2C_BUFF[30];
	}	
	
	//*APP代码运行区域选择设置 在AT24C02的100区域*/
	
}

//35 是IP发送标志位  36是模拟量采集通道设置位

