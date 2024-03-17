#include "SHT30.h"

//T_IIC��ʼ��
void T_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //ʹ��GPIOAʱ��
    
    //PA8,9��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;   //PA8->SCL  PA9->SDA
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;    //�������
    GPIO_Initure.Pull=GPIO_PULLUP;            //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;       //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    T_IIC_SDA(1);
    T_IIC_SCL(1);  
}


//����T_IIC��ʼ�ź�
void T_IIC_Start(void)
{
	T_SDA_OUT();     //sda�����
	T_IIC_SDA(1);	  	  
	T_IIC_SCL(1);
	delay_us(4);
 	T_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	T_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

//����T_IICֹͣ�ź�
void T_IIC_Stop(void)
{
	T_SDA_OUT();//sda�����
	T_IIC_SCL(0);
	T_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	T_IIC_SCL(1); 
	delay_us(4);			
	T_IIC_SDA(1);//����I2C���߽����ź�				   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 T_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	T_SDA_IN();      //SDA����Ϊ����  
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
	T_IIC_SCL(0);  //ʱ�����0 
	return 0;  
} 


//����ACKӦ��
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

//������ACKӦ��		    
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

//T_IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void T_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  T_SDA_OUT(); 	    
    T_IIC_SCL(0);    //����ʱ�ӿ�ʼ���ݴ���
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

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 T_IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		T_SDA_IN();//SDA����Ϊ����
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
        T_IIC_NAck();//����nACK
    else
        T_IIC_Ack(); //����ACK   
    return receive;
}


/******************************************************************************/

void SHT_Init(void)
{
	  delay_ms(250);                              
   	T_IIC_Start();
   	T_IIC_Send_Byte(0x88);    //д����ģʽ
   	T_IIC_Wait_Ack();
   	T_IIC_Send_Byte(0x21);		//0x2130  ��ʾ����ģʽ ����1ms
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
  //дһ�������SHT30������SHT30ת������ļĴ���
   T_IIC_Start();
   T_IIC_Send_Byte(0x88);
   T_IIC_Wait_Ack();
   T_IIC_Send_Byte(0xe0);
   T_IIC_Wait_Ack();
   T_IIC_Send_Byte(0x00);
   T_IIC_Wait_Ack();
	
  //�����ǿ�ʼ��ȡ���ݣ����е�����p��Ž����ǰ��������¶�ֵ����������ʪ��ֵ����ǰ�����¶�ֵ���棬
  //p[0]���¶ȵĸ߰�λ��p[1]�ǵͰ�λ��p[2]��CRCУ�顣
   T_IIC_Start();
   T_IIC_Send_Byte(0x89);
   T_IIC_Wait_Ack();
  
   //ǰ��ζ�ȡ��Ҫ����ack�źţ����һ�ξͲ��÷��ˡ�
   p[0] = T_IIC_Read_Byte(1);//�¶ȸ�8λ
   p[1] = T_IIC_Read_Byte(1);//�¶ȵ�8λ
   p[2] = T_IIC_Read_Byte(1);//CRCУ��
   p[3] = T_IIC_Read_Byte(1);//ʪ�ȸ�8λ
   p[4] = T_IIC_Read_Byte(1);//ʪ�ȵ�8λ
   p[5] = T_IIC_Read_Byte(0);//CRCУ��
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


