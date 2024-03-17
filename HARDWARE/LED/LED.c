#include "LED.h"
#include "delay.h"
#include "iwdg.h"
/*
���İ�LED���Գ���
*/
//��ʼ��PI0��PI1��PI2�������ţ����ó����ģʽ    �͵�ƽ���� �ߵ�ƽϨ��  
//LED IO��ʼ��



void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
	  __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
	  __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
   	HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��PI0��PI1��PI2
  
	GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_11; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
   	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��PI0��PI1��PI2
	
     HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PB1��0
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);	//PB1��1
 	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_SET);	//PB1��1
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);	//PB1��0
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1��1
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);	//PB1��1
	 
}

void LED_Test(void)
{
     DO_LEDI_1(0);
	 DO_LEDB_7(0);
	 delay_ms(50);
	 DO_LEDI_1(1);
	 DO_LEDB_7(1);
	 delay_ms(50);
}
//������ָʾ�Ƶ����������Ƿ�����
void LED_INIT_Test(void)
{
   DO_LEDI_0(0);
	 DO_LEDI_1(0);
	 DO_LEDI_2(0);
	 DO_LEDB_5(0);
	 DO_LEDB_7(0);
	 delay_ms(200);
	 IWDG_Feed(); //ι��	
	 DO_LEDI_0(1);
	 DO_LEDI_1(1);
	 DO_LEDI_2(1);
	 DO_LEDB_5(1);
	 DO_LEDB_7(1);
   delay_ms(200);
}



//����Ͽ�ָʾ��
void Net_LED (void)
{
	  
     DO_LEDI_0(1);
	   DO_LEDB_7(1);
		 IWDG_Feed(); //ι��	 
	   DO_LEDB_5(0);
	   DO_LEDI_0(0);
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET);	//PB1��1  ������
	   delay_ms(100);
	   DO_LEDB_5(1);
	   DO_LEDI_0(1);
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1��1  ������
	   delay_ms(100);
}



//������ ��ʼ��  PI3
void BEEP_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;  
	  __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
	  GPIO_Initure.Pin=GPIO_PIN_3;
	  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
      GPIO_Initure.Pull=GPIO_PULLUP;          //����
      GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
   	  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��PI0��PI1��PI2
	  HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1��1
}

//����������
void BEEP_On(void)
{
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET);	//PB1��1
//		IWDG_Feed(); //ι��	
	 delay_ms(500);
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1��1
}
//���ͺͽ���ָʾ��
void Send_Receive_Led(void)
{
			if(DataReceived==TRUE)
		{
			DO_LEDI_0(0);
			DO_LEDB_5(0);	
      delay_ms(20);	
			DataReceived =0;
		}
		if(DataSent ==TRUE)
		{
			DO_LEDI_0(0);
			DO_LEDB_5(0);	
      delay_ms(20);			
			DataSent =0;
		}
		  DO_LEDI_0(1);
			DO_LEDB_5(1);	
}



