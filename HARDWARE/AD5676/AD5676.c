#include "AD5676.h"


/*
SPI通信
PB12、PB13、PB14、PB15
*/


SPI_HandleTypeDef hspi_ad5676;
void STM32_AD5676_SPI_GPIO (void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	//涉及到的时钟初始化
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	__HAL_RCC_SPI2_CLK_ENABLE(); //初始化SPI2
	//引脚功能的初始化 复用为SPI2
	 GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
	 GPIO_Initure.Pull = GPIO_PULLUP;
	 GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;
	 GPIO_Initure.Alternate = GPIO_AF5_SPI2;
	 HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化spi
	
	//SPI1的初始化 设置其工作模式
	hspi_ad5676.Instance = SPI2;
	hspi_ad5676.Init.Mode = SPI_MODE_MASTER; //STM32主模式
	hspi_ad5676.Init.Direction = SPI_DIRECTION_2LINES; //双向
	hspi_ad5676.Init.DataSize = SPI_DATASIZE_8BIT; //数据位数为8位数据帧
	hspi_ad5676.Init.CLKPolarity = SPI_POLARITY_HIGH; //串行同步时钟空闲状态电平为高电平
	hspi_ad5676.Init.CLKPhase = SPI_PHASE_1EDGE; //第一个跳变沿数据被采集
	hspi_ad5676.Init.NSS = SPI_NSS_SOFT;
	hspi_ad5676.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi_ad5676.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi_ad5676.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi_ad5676.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi_ad5676.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hspi_ad5676);
	
}
void STM32_AD5676_CS_GPIO(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_12; 
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化PB12
	
}

void AD5676_Init(void)
{
	STM32_AD5676_SPI_GPIO();
	STM32_AD5676_CS_GPIO();
	
}
/*
SPI 发送接收一个字节函数
两个变量 Txdata 需要发送的数
Rxdata 接收到的数
这个是发送函数 是把Txdata这个8位数据通过SPI1发送出去
*/

unsigned char SPI2_SendByte(unsigned char Txdata)
{
	unsigned char Rxdata;
	if(HAL_SPI_TransmitReceive(&hspi_ad5676,&Txdata,&Rxdata,1,0xFFFFFF)!=HAL_OK)
	Rxdata = 0xff; //发送数据固定为0xff
	return Rxdata;
}

/*
SPI发送三个字节的数据
AD5676 的操作有三个字节 第一个字节的高4位为控制信号位 这里固定为 0011（二进制），低4位为地址位 可以选择为0000-0111 后面两个字节为数据位 共16个二进制
*/
void SPI2_Send3Byte(unsigned int Txdata)
{
	Txdata = Txdata &0x00FFFFFFFF;//舍去高8位
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	//PB1置0
	SPI2_SendByte((unsigned char)(Txdata>>16));//发送DB16-DB23
	SPI2_SendByte((unsigned char)(Txdata>>8));//发送DB8-DB15 
	SPI2_SendByte((unsigned char)(Txdata));//发送DB0-DB7
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	//PB1置1
}


//旧协议使用       8位数据
void AD5676_SendData(unsigned char channel,unsigned char AnalogData)
{
	double temp_double = 0;
	unsigned short int temp_u16;
	temp_double = 10486 + AnalogData * 41943 / 254;
	temp_u16 = (unsigned short int)temp_double; 
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);               //片选线拉低，选中AD5676
	SPI2_SendByte(channel);         //DB23-DB20通道号
	SPI2_SendByte((u8)(temp_u16>>8));//发送DB15-DB8
	SPI2_SendByte((u8)temp_u16);	 //发送DB7-DB0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);              //片选线拉高，不选中AD5676
}



void AD5676_Send_AnalogData(unsigned int data)
{
		
//	
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	//PB1置0
//	SPI2_SendByte((unsigned char)(data>>16));//发送DB16-DB23
//	SPI2_SendByte((unsigned char)(data>>8));//发送DB8-DB15 
//	SPI2_SendByte((unsigned char)(data));//发送DB0-DB7
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	//PB1置1
	
	double temp_double = 0.1;
	unsigned short int temp_u16;
	data = data &0x00FFFFFFFF;//舍去高8位
	temp_double = 10486 +((u16)data * 41943 )/ 255;
	temp_u16 = (unsigned short int)temp_double; 
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	SPI2_SendByte((unsigned char)(data>>16));         //DB23-DB20通道号
	SPI2_SendByte((u8)(temp_u16>>8));//发送DB15-DB8
	SPI2_SendByte((u8)temp_u16);	 //发送DB7-DB0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	
	
}

