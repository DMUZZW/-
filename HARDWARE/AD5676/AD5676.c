#include "AD5676.h"


/*
SPIͨ��
PB12��PB13��PB14��PB15
*/


SPI_HandleTypeDef hspi_ad5676;
void STM32_AD5676_SPI_GPIO (void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	//�漰����ʱ�ӳ�ʼ��
	__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	__HAL_RCC_SPI2_CLK_ENABLE(); //��ʼ��SPI2
	//���Ź��ܵĳ�ʼ�� ����ΪSPI2
	 GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
	 GPIO_Initure.Pull = GPIO_PULLUP;
	 GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;
	 GPIO_Initure.Alternate = GPIO_AF5_SPI2;
	 HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��spi
	
	//SPI1�ĳ�ʼ�� �����乤��ģʽ
	hspi_ad5676.Instance = SPI2;
	hspi_ad5676.Init.Mode = SPI_MODE_MASTER; //STM32��ģʽ
	hspi_ad5676.Init.Direction = SPI_DIRECTION_2LINES; //˫��
	hspi_ad5676.Init.DataSize = SPI_DATASIZE_8BIT; //����λ��Ϊ8λ����֡
	hspi_ad5676.Init.CLKPolarity = SPI_POLARITY_HIGH; //����ͬ��ʱ�ӿ���״̬��ƽΪ�ߵ�ƽ
	hspi_ad5676.Init.CLKPhase = SPI_PHASE_1EDGE; //��һ�����������ݱ��ɼ�
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
	__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_12; 
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��PB12
	
}

void AD5676_Init(void)
{
	STM32_AD5676_SPI_GPIO();
	STM32_AD5676_CS_GPIO();
	
}
/*
SPI ���ͽ���һ���ֽں���
�������� Txdata ��Ҫ���͵���
Rxdata ���յ�����
����Ƿ��ͺ��� �ǰ�Txdata���8λ����ͨ��SPI1���ͳ�ȥ
*/

unsigned char SPI2_SendByte(unsigned char Txdata)
{
	unsigned char Rxdata;
	if(HAL_SPI_TransmitReceive(&hspi_ad5676,&Txdata,&Rxdata,1,0xFFFFFF)!=HAL_OK)
	Rxdata = 0xff; //�������ݹ̶�Ϊ0xff
	return Rxdata;
}

/*
SPI���������ֽڵ�����
AD5676 �Ĳ����������ֽ� ��һ���ֽڵĸ�4λΪ�����ź�λ ����̶�Ϊ 0011�������ƣ�����4λΪ��ַλ ����ѡ��Ϊ0000-0111 ���������ֽ�Ϊ����λ ��16��������
*/
void SPI2_Send3Byte(unsigned int Txdata)
{
	Txdata = Txdata &0x00FFFFFFFF;//��ȥ��8λ
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	//PB1��0
	SPI2_SendByte((unsigned char)(Txdata>>16));//����DB16-DB23
	SPI2_SendByte((unsigned char)(Txdata>>8));//����DB8-DB15 
	SPI2_SendByte((unsigned char)(Txdata));//����DB0-DB7
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	//PB1��1
}


//��Э��ʹ��       8λ����
void AD5676_SendData(unsigned char channel,unsigned char AnalogData)
{
	double temp_double = 0;
	unsigned short int temp_u16;
	temp_double = 10486 + AnalogData * 41943 / 254;
	temp_u16 = (unsigned short int)temp_double; 
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);               //Ƭѡ�����ͣ�ѡ��AD5676
	SPI2_SendByte(channel);         //DB23-DB20ͨ����
	SPI2_SendByte((u8)(temp_u16>>8));//����DB15-DB8
	SPI2_SendByte((u8)temp_u16);	 //����DB7-DB0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);              //Ƭѡ�����ߣ���ѡ��AD5676
}



void AD5676_Send_AnalogData(unsigned int data)
{
		
//	
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	//PB1��0
//	SPI2_SendByte((unsigned char)(data>>16));//����DB16-DB23
//	SPI2_SendByte((unsigned char)(data>>8));//����DB8-DB15 
//	SPI2_SendByte((unsigned char)(data));//����DB0-DB7
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	//PB1��1
	
	double temp_double = 0.1;
	unsigned short int temp_u16;
	data = data &0x00FFFFFFFF;//��ȥ��8λ
	temp_double = 10486 +((u16)data * 41943 )/ 255;
	temp_u16 = (unsigned short int)temp_double; 
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	SPI2_SendByte((unsigned char)(data>>16));         //DB23-DB20ͨ����
	SPI2_SendByte((u8)(temp_u16>>8));//����DB15-DB8
	SPI2_SendByte((u8)temp_u16);	 //����DB7-DB0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	
	
}

