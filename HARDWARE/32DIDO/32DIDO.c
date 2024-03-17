#include "32DIDO.h"



//����������˿ڳ�ʼ��
//����������˿���: PA15��PD0��PD1��PD3��PD4��PD5��PD6��PD7��PD8��PD9��PD10��PD11��PD12��PD13��PD14��PD15��PI10��PI11��PF0 PF1 PF2 PF3 PF4 PF5 PF8 PF9 PF10 PF11 PF12 PF13 PF14 PF15
void Digital_DataInput_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	  __HAL_RCC_GPIOD_CLK_ENABLE();     //����GPIODʱ��
 	  __HAL_RCC_GPIOF_CLK_ENABLE();     //����GPIOFʱ��
	  __HAL_RCC_GPIOI_CLK_ENABLE();     //����GPIOIʱ��
	
	 //��ʼ��PD�˿� ����PD2û�б���ʼ�� �����������ó���������ģʽ
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; //PA15
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);    // ��ʼ�� PD�˿�
	
	//��ʼ��PF�˿� ����PF6��PF7 �������Ŷ�����ʼ������������ģʽ
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; //PA15
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);    //��ʼ��PF�˿�
		
	//��ʼ��PI10��PI11���ţ����ó���������ģʽ�����ⲿ����Ϊ�͵�ƽʱ��Ч
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11; //PI10��PI11
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);    //��ʼ��PI10��PI11
}

//������������ų�ʼ��
//����������������ǣ�PE��PG��
void Digital_DataOutput_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();			//����GPIOEʱ��
	  __HAL_RCC_GPIOG_CLK_ENABLE();			//����GPIOGʱ��
	 
	//PE�ڳ�ʼ��Ϊ���
    GPIO_Initure.Pin=GPIO_PIN_All; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE��1
	
	//PG�ڳ�ʼ��Ϊ���
  	GPIO_Initure.Pin=GPIO_PIN_All; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //
	  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG��1

}
//��ʼ�������������
void Digital_Data_Init(void)
{
	Digital_DataInput_Init();
	Digital_DataOutput_Init();
}










//GPIO��������
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,����IO���ű��.
//AFx:0~15,����AF0~AF15.
//AF0~15�������(��������г����õ�,��ϸ�����429/746�����ֲ�,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,unsigned char BITx,unsigned char AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIOͨ������ 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ��IO,��0λ����Px0,��1λ����Px1,��������.����0X0101,����ͬʱ����Px0��Px8.
//MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
//OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
//OSPEED:0~3;����ٶ�����,0,����;1,����;2,����;3,����. 
//PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
//ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
void GPIO_Set(GPIO_TypeDef* GPIOx,unsigned int BITx,unsigned int MODE,unsigned int OTYPE,unsigned int OSPEED,unsigned int PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//һ����λ��� 
		curpin=BITx&pos;//��������Ƿ�Ҫ����
		if(curpin==pos)	//��Ҫ����
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->MODER|=MODE<<(pinpos*2);	//�����µ�ģʽ 
			if((MODE==0X01)||(MODE==0X02))	//��������ģʽ/���ù���ģʽ
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//���ԭ��������
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//�����µ��ٶ�ֵ  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//���ԭ��������
				GPIOx->OTYPER|=OTYPE<<pinpos;		//�����µ����ģʽ
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//�����µ�������
		}
	}
} 
//����GPIOĳ�����ŵ����״̬
//GPIOx:GPIOA~GPIOI.
//pinx:���ű��,��Χ:0~15
//status:����״̬(�����λ��Ч),0,����͵�ƽ;1,����ߵ�ƽ
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,unsigned short pinx,unsigned char status)
{
	if(status&0X01)GPIOx->BSRR=pinx;	//����GPIOx��pinxΪ1
	else GPIOx->BSRR=pinx<<16;			//����GPIOx��pinxΪ0
}
//��ȡGPIOĳ�����ŵ�״̬
//GPIOx:GPIOA~GPIOI.
//pinx:���ű��,��Χ:0~15
//����ֵ:����״̬,0,���ŵ͵�ƽ;1,���Ÿߵ�ƽ
unsigned char GPIO_Pin_Get(GPIO_TypeDef* GPIOx,unsigned short pinx)
{ 
	if(GPIOx->IDR&pinx)return 1;		//pinx��״̬Ϊ1
	else return 0;						//pinx��״̬Ϊ0
}





