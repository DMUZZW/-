#include "SPI_W5500.h"



//STM32΢���������ͨ��SPI�ӿ���W5500����ͨ�ŵ���������SPI��һ�ֳ����Ĵ���ͨ�Žӿڣ���������΢����������Χ�豸����δ�����Ҫ�������¼����£�

//��ʼ��SPI�ӿڣ�����SPI�Ĺ���ģʽ���ٶȵȲ������Ա���W5500��ȷͨ�š�
//��ʼ�����ţ���������SPIͨ�ŵ����ţ��������ݴ������š�Ƭѡ���ŵȡ�
//���ݶ�д���ṩ��ͨ��SPI�ӿ���W5500���ͺͽ������ݵĺ�����
//��λ���жϴ������Կ���W5500�ĸ�λ����������������W5500���ж��źš�
SPI_HandleTypeDef hspi_w5500;
/*SPI-W5500��ʼ��*/
void STM32_W55OO_SPI_GPIO (void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	//�漰����ʱ�ӳ�ʼ��
	
	WIZ_SPI_GPIO_ClK_ENABLE();			//����GPIOAʱ��
	WIZ_SPIx_RCC_CLK_ENABLE(); //��ʼ��SPI1
	
	//���Ź��ܵĳ�ʼ�� ����ΪSPI1
	 GPIO_Initure.Pin=WIZ_SPIx_SCLK_PIN|WIZ_SPIx_MISO_PIN|WIZ_SPIx_MOSI_PIN;
	 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
	 GPIO_Initure.Pull = GPIO_PULLUP;
	 GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 GPIO_Initure.Alternate = GPIO_AF5_SPI1;
	 HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT,&GPIO_Initure);     //��ʼ��spi
	
	//SPI1�ĳ�ʼ�� �����乤��ģʽ
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
W5500Ƭѡ���ų�ʼ������ PA4����
*/
void W5500_CS_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
	   WIZ_SPIx_SCS_CLK_ENABLE() ;			//����GPIOAʱ��
	
		GPIO_Initure.Pin=WIZ_SPIx_SCS_PIN; //NSS->PA4
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;     //���ģʽ             
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //����
   	HAL_GPIO_Init(WIZ_SPIx_SCS_PORT,&GPIO_Initure);     //��ʼ��PA4
	
}


/*
W5500��λ��������
*/

void W5500_Reset_Pin(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		WIZ_RESET_CLK_ENABLE();		
		GPIO_Initure.Pin=WIZ_RESET_PIN; //RESET PH5
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;     //���ģʽ             
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //����
   	HAL_GPIO_Init(WIZ_RESET_PORT,&GPIO_Initure);     //��ʼ��PH5
	
}
/*W5500�ⲿ�ж���������*/
void W5500_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;  
    WIZ_INT_CLK_ENABLE() ;               //����GPIOHʱ�� 
	
    GPIO_Initure.Pin=WIZ_INT_PIN;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;				//����  
    GPIO_Initure.Pin=WIZ_INT_PIN;     
    HAL_GPIO_Init(WIZ_INT_PORT,&GPIO_Initure);
       
		 //�ж���4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //ʹ���ж���0 
	     
}
//�жϷ�����

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //�����жϴ����ú���
}

//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   
    switch(GPIO_Pin)
    {
				case GPIO_PIN_4:
				// W5500_Interrupt= 1;//W5500���ж����ŷ��͹����ж��źţ����������ͣ�W5500�����жϺ�INT����Ϊ�͵�ƽ����W5500���ж���Ч��
            break;     
    }
}



/*W5500��������*/
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
//	 IWDG_Feed(); //ι��	
	 delay_ms(1600);
	
}
/***********************************************SPI��д����***************************************************************************/

/*������W5500ͨ��*/
/*SPI����д���� ȫ˫��*/
unsigned char  SPI_SendWrite_Byte(unsigned char TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi_w5500,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}

unsigned char W5500_SPI_ReadWrite(unsigned char data)
{
	return (SPI_SendWrite_Byte(data));
}



void W5500_Write(unsigned int address,unsigned char data)
{
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x000000F8) + 4);//�����ź�
	W5500_SPI_ReadWrite(data);//����
	WIZ_SPIx_SCS_OFF();
}

unsigned char W5500_Read(unsigned int address)
{
	unsigned char data = 0;
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x000000F8));//�����ź�
	data = W5500_SPI_ReadWrite(0x00);//��һ�������� Ȼ���������
	WIZ_SPIx_SCS_OFF();
	return data;
}




unsigned short W5500_Read_nByte(unsigned int address,unsigned char* data,unsigned short len)
{
		unsigned short i = 0;
	WIZ_SPIx_SCS_ON();
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x000000F8));//�����ź�
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
	W5500_SPI_ReadWrite((address & 0x00FF0000)>>16);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x0000FF00)>> 8);//��ַ��8λ
	W5500_SPI_ReadWrite((address & 0x000000F8) + 4);//�����ź�
	for (i = 0;i<len;i++)
  {
		W5500_SPI_ReadWrite(data[i]);
	}
	WIZ_SPIx_SCS_OFF();
	return len;
}











/**********************************W5500���ͺͽ��պ���**************************************/

/*���ͺͽ��պ���

*/
unsigned char SPI_SendByte(unsigned char byte)
{
  unsigned char d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspi_w5500,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=0XFF;
  
  return d_read; 
}

/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
unsigned char  IINCHIP_SpiSendData(unsigned char dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
*/
void IINCHIP_WRITE( unsigned long addrbsb,  unsigned char data)
{
  	WIZ_SPIx_SCS_ON();  //Ƭѡ�ź�Ϊ�͵�ƽ                            		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
  WIZ_SPIx_SCS_OFF();                           
}

/**
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
*/
unsigned char IINCHIP_READ(unsigned long addrbsb)
{
   uint8 data = 0;
   	WIZ_SPIx_SCS_ON();                          
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);     //���պ���        
  WIZ_SPIx_SCS_OFF();                              
   return data;    
}

/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
//  W5500������ʵ��pingͨ�������������
/********************************************************************************/

void W5500_Start(void)
{

  W5500_Reset();
	//����������� gateway ip   subnetMask address   MAC address  sources IP
	setGAR(W5500_Parameter_Configuration.Gateway_IP);//set the gateway IP address 
	setSUBR(W5500_Parameter_Configuration.Sub_Mask);//��������
	setSHAR(W5500_Parameter_Configuration.MAC_Addr); //MAC��ַ
	setSIPR(W5500_Parameter_Configuration.LOCAL_IP_Addr);//local IP	
	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/	
}












