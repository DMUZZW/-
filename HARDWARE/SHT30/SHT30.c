#include "SHT30.h"

//T_IIC初始化
void T_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOA时钟
    
    //PA8,9初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;   //PA8->SCL  PA9->SDA
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;    //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;            //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;       //快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    T_IIC_SDA(1);
    T_IIC_SCL(1);  
}


//产生T_IIC起始信号
void T_IIC_Start(void)
{
	T_SDA_OUT();     //sda线输出
	T_IIC_SDA(1);	  	  
	T_IIC_SCL(1);
	delay_us(4);
 	T_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	T_IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  

//产生T_IIC停止信号
void T_IIC_Stop(void)
{
	T_SDA_OUT();//sda线输出
	T_IIC_SCL(0);
	T_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	T_IIC_SCL(1); 
	delay_us(4);			
	T_IIC_SDA(1);//发送I2C总线结束信号				   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 T_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	T_SDA_IN();      //SDA设置为输入  
	T_IIC_SDA(1);delay_us(1);	   
	T_IIC_SCL(1);delay_us(1);	 
	while(T_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			T_IIC_Stop();
			return 1;
		}
	}
	T_IIC_SCL(0);  //时钟输出0 
	return 0;  
} 


//产生ACK应答
void T_IIC_Ack(void)
{
	T_IIC_SCL(0);
	T_SDA_OUT();
	T_IIC_SDA(0);
	delay_us(2);
	T_IIC_SCL(1);
	delay_us(2);
	T_IIC_SCL(0);
}

//不产生ACK应答		    
void T_IIC_NAck(void)
{
	T_IIC_SCL(0);
	T_SDA_OUT();
	T_IIC_SDA(1);
	delay_us(2);
	T_IIC_SCL(1);
	delay_us(2);
	T_IIC_SCL(0);
}					 				

//T_IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void T_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  T_SDA_OUT(); 	    
    T_IIC_SCL(0);    //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        T_IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		    delay_us(2);  
				T_IIC_SCL(1);
				delay_us(2); 
				T_IIC_SCL(0);	
				delay_us(2);
    }	 
		delay_ms(50);
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 T_IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		T_SDA_IN();//SDA设置为输入
		for(i=0;i<8;i++ )
		{
        T_IIC_SCL(0); 
        delay_us(2);
				T_IIC_SCL(1);
        receive<<=1;
        if(T_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        T_IIC_NAck();//发送nACK
    else
        T_IIC_Ack(); //发送ACK   
    return receive;
}


/******************************************************************************/

void SHT_Init(void)
{
	  delay_ms(250);                              
   	T_IIC_Start();
   	T_IIC_Send_Byte(0x88);    //写命令模式
   	T_IIC_Wait_Ack();
   	T_IIC_Send_Byte(0x21);		//0x2130  表示周期模式 周期1ms
   	T_IIC_Wait_Ack();
   	T_IIC_Send_Byte(0x30);
   	T_IIC_Wait_Ack();
    T_IIC_Stop();
  
}


void sht30_read_temp_humi(char Temperature[2], char Humidity[2])
{
	float temperature =  0;
  float humidity = 0 ;
	unsigned char p[6]={0,};
	unsigned int a = 65535;
  //写一个命令给SHT30，访问SHT30转换结果的寄存器
   T_IIC_Start();
   T_IIC_Send_Byte(0x88);
   T_IIC_Wait_Ack();
   T_IIC_Send_Byte(0xe0);
   T_IIC_Wait_Ack();
   T_IIC_Send_Byte(0x00);
   T_IIC_Wait_Ack();
	
  //下面是开始读取数据，其中的数组p存放结果，前三个存放温度值，后三个是湿度值，在前三个温度值里面，
  //p[0]是温度的高八位，p[1]是低八位，p[2]是CRC校验。
   T_IIC_Start();
   T_IIC_Send_Byte(0x89);
   T_IIC_Wait_Ack();
  
   //前五次读取都要发送ack信号，最后一次就不用发了。
   p[0] = T_IIC_Read_Byte(1);//温度高8位
   p[1] = T_IIC_Read_Byte(1);//温度低8位
   p[2] = T_IIC_Read_Byte(1);//CRC校验
   p[3] = T_IIC_Read_Byte(1);//湿度高8位
   p[4] = T_IIC_Read_Byte(1);//湿度低8位
   p[5] = T_IIC_Read_Byte(0);//CRC校验
   T_IIC_Stop();
	 Temperature[0] = p[0];
	 Temperature[1] = p[1];
	 Humidity[0] =  p[3];
	 Humidity[1] =  p[4];
	 
	 
	 temperature = (float)((175*((p[0]<<8)+p[1])/a)-45);    
   humidity = (float)((100*((p[3]<<8)+p[4]))/a);
	 

}

void Temperature_Humidity(unsigned char times,unsigned char SHT31_DATA[])
{
	unsigned char SHT31_DATA_SUM[4],i=0;
	char Temperature[2], Humidity[2];
	for(i=0;i<times;i++)
	{
		delay_ms(5);
		sht30_read_temp_humi(Temperature,Humidity);
		SHT31_DATA_SUM[0] += Temperature[0];
		SHT31_DATA_SUM[1] += Temperature[1];
		SHT31_DATA_SUM[2] += Humidity[0];
		SHT31_DATA_SUM[3] += Humidity[1];
	}
	SHT31_DATA[0] = SHT31_DATA_SUM[0]/times;
	SHT31_DATA[1] = SHT31_DATA_SUM[1]/times;
	SHT31_DATA[2] = SHT31_DATA_SUM[2]/times;
	SHT31_DATA[3] = SHT31_DATA_SUM[3]/times;
	
}


