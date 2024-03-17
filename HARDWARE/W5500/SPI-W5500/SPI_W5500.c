#include "SPI_W5500.h"



//STM32微控制器如何通过SPI接口与W5500进行通信的驱动程序。SPI是一种常见的串行通信接口，用于连接微控制器和外围设备。这段代码主要做了以下几件事：

//初始化SPI接口：设置SPI的工作模式、速度等参数，以便与W5500正确通信。
//初始化引脚：设置用于SPI通信的引脚，包括数据传输引脚、片选引脚等。
//数据读写：提供了通过SPI接口向W5500发送和接收数据的函数。
//复位和中断处理：可以控制W5500的复位操作，并处理来自W5500的中断信号。
SPI_HandleTypeDef hspi_w5500;
/*SPI-W5500初始化*/
void STM32_W55OO_SPI_GPIO (void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	//涉及到的时钟初始化
	
	WIZ_SPI_GPIO_ClK_ENABLE();			//开启GPIOA时钟
	WIZ_SPIx_RCC_CLK_ENABLE(); //初始化SPI1
	
	//引脚功能的初始化 复用为SPI1
	 GPIO_Initure.Pin=WIZ_SPIx_SCLK_PIN|WIZ_SPIx_MISO_PIN|WIZ_SPIx_MOSI_PIN;
	 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
	 GPIO_Initure.Pull = GPIO_PULLUP;
	 GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 GPIO_Initure.Alternate = GPIO_AF5_SPI1;
	 HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT,&GPIO_Initure);     //初始化spi
	
	//SPI1的初始化 设置其工作模式
	hspi_w5500.Instance = WIZ_SPIx;
	hspi_w5500.Init.Mode = SPI_MODE_MASTER;
	hspi_w5500.Init.Direction = SPI_DIRECTION_2LINES;
	hspi_w5500.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi_w5500.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi_w5500.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi_w5500.Init.NSS = SPI_NSS_SOFT;
	hspi_w5500.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi_w5500.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi_w5500.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi_w5500.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi_w5500.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hspi_w5500);
	
}
/*
W5500片选引脚初始化函数 PA4引脚
*/
void W5500_CS_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
	   WIZ_SPIx_SCS_CLK_ENABLE() ;			//开启GPIOA时钟
	
		GPIO_Initure.Pin=WIZ_SPIx_SCS_PIN; //NSS->PA4
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;     //输出模式             
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //快速
   	HAL_GPIO_Init(WIZ_SPIx_SCS_PORT,&GPIO_Initure);     //初始化PA4
	
}


/*
W5500复位引脚设置
*/

void W5500_Reset_Pin(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		WIZ_RESET_CLK_ENABLE();		
		GPIO_Initure.Pin=WIZ_RESET_PIN; //RESET PH5
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;     //输出模式             
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //快速
   	HAL_GPIO_Init(WIZ_RESET_PORT,&GPIO_Initure);     //初始化PH5
	
}
/*W5500外部中断引脚设置*/
void W5500_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;  
    WIZ_INT_CLK_ENABLE() ;               //开启GPIOH时钟 
	
    GPIO_Initure.Pin=WIZ_INT_PIN;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;				//上拉  
    GPIO_Initure.Pin=WIZ_INT_PIN;     
    HAL_GPIO_Init(WIZ_INT_PORT,&GPIO_Initure);
       
		 //中断线4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //使能中断线0 
	     
}
//中断服务函数

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   
    switch(GPIO_Pin)
    {
				case GPIO_PIN_4:
				// W5500_Interrupt= 1;//W5500的中断引脚发送过来中断信号，引脚是拉低，W5500发生中断后，INT引脚为低电平，即W5500的中断生效，
            break;     
    }
}



/*W5500引脚设置*/
void W5500_STM32(void )
{
	STM32_W55OO_SPI_GPIO();
	W5500_CS_Init();
	W5500_Reset_Pin();
	W5500_EXTI_Init();
}



void W5500_Reset(void)
{
	 HAL_GPIO_WritePin(WIZ_RESET_PORT,WIZ_RESET_PIN,GPIO_PIN_RESET);	
	 delay_ms(10);
	 HAL_GPIO_WritePin(WIZ_RESET_PORT,WIZ_RESET_PIN,GPIO_PIN_SET);	
//	 IWDG_Feed(); //喂狗	
	 delay_ms(1600);
	
}
/***********************************************SPI读写函数***************************************************************************/

/*以下是W5500通信*/
/*SPI读与写函数 全双工*/
unsigned char  SPI_SendWrite_Byte(unsigned char TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi_w5500,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}

unsigned char W5500_SPI_ReadWrite(unsigned char data)
{
	return (SPI_SendWrite_Byte(data));
}



void W5500_Write(unsigned int address,unsigned char data)
{
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//地址高8位
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//地址低8位
	W5500_SPI_ReadWrite((address & 0x000000F8) + 4);//控制信号
	W5500_SPI_ReadWrite(data);//数据
	WIZ_SPIx_SCS_OFF();
}

unsigned char W5500_Read(unsigned int address)
{
	unsigned char data = 0;
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//地址高8位
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//地址低8位
	W5500_SPI_ReadWrite((address & 0x000000F8));//控制信号
	data = W5500_SPI_ReadWrite(0x00);//传一个空数据 然后接受数据
	WIZ_SPIx_SCS_OFF();
	return data;
}




unsigned short W5500_Read_nByte(unsigned int address,unsigned char* data,unsigned short len)
{
		unsigned short i = 0;
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//地址高8位
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//地址低8位
	W5500_SPI_ReadWrite((address & 0x000000F8));//控制信号
	for(i=0;i<len;i++)
	{
		data[i] = W5500_SPI_ReadWrite(0x00);
	}
		WIZ_SPIx_SCS_OFF();
	return len;
}
	

unsigned short W5500_Write_nByte(unsigned int address,unsigned char* data,unsigned short len)
{
	unsigned short i = 0;
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//地址高8位
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//地址低8位
	W5500_SPI_ReadWrite((address & 0x000000F8) + 4);//控制信号
	for (i = 0;i<len;i++)
  {
		W5500_SPI_ReadWrite(data[i]);
	}
	WIZ_SPIx_SCS_OFF();
	return len;
}











/**********************************W5500发送和接收函数**************************************/

/*发送和接收函数

*/
unsigned char SPI_SendByte(unsigned char byte)
{
  unsigned char d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspi_w5500,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=0XFF;
  
  return d_read; 
}

/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
unsigned char  IINCHIP_SpiSendData(unsigned char dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( unsigned long addrbsb,  unsigned char data)
{
  	WIZ_SPIx_SCS_ON();  //片选信号为低电平                            		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
  WIZ_SPIx_SCS_OFF();                           
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
unsigned char IINCHIP_READ(unsigned long addrbsb)
{
   uint8 data = 0;
   	WIZ_SPIx_SCS_ON();                          
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);     //接收函数        
  WIZ_SPIx_SCS_OFF();                              
   return data;    
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
unsigned short wiz_write_buf(unsigned long addrbsb,unsigned char* buf,unsigned short len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
  	WIZ_SPIx_SCS_ON();                              
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
  WIZ_SPIx_SCS_OFF();                         
   return len;  
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
unsigned short wiz_read_buf(unsigned long addrbsb, unsigned char* buf,unsigned short len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
 	WIZ_SPIx_SCS_ON();                               
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
 WIZ_SPIx_SCS_OFF();                                  
  return len;
}


/********************************************************************************/
//  W5500操作，实现ping通，网络参数设置
/********************************************************************************/

void W5500_Start(void)
{

  W5500_Reset();
	//设置网络参数 gateway ip   subnetMask address   MAC address  sources IP
	setGAR(W5500_Parameter_Configuration.Gateway_IP);//set the gateway IP address 
	setSUBR(W5500_Parameter_Configuration.Sub_Mask);//子网掩码
	setSHAR(W5500_Parameter_Configuration.MAC_Addr); //MAC地址
	setSIPR(W5500_Parameter_Configuration.LOCAL_IP_Addr);//local IP	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/	
}












