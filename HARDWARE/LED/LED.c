#include "LED.h"
#include "delay.h"
#include "iwdg.h"
/*
核心板LED测试程序
*/
//初始化PI0、PI1和PI2三个引脚，设置成输出模式    低电平点亮 高电平熄灭  
//LED IO初始化



void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
	  __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
	  __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
   	HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化PI0、PI1和PI2
  
	GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_11; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
   	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化PI0、PI1和PI2
	
     HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PB1置0
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1
 	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_SET);	//PB1置1
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);	//PB1置0
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1置1
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);	//PB1置1
	 
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
//把所有指示灯点亮，检验是否正常
void LED_INIT_Test(void)
{
   DO_LEDI_0(0);
	 DO_LEDI_1(0);
	 DO_LEDI_2(0);
	 DO_LEDB_5(0);
	 DO_LEDB_7(0);
	 delay_ms(200);
	 IWDG_Feed(); //喂狗	
	 DO_LEDI_0(1);
	 DO_LEDI_1(1);
	 DO_LEDI_2(1);
	 DO_LEDB_5(1);
	 DO_LEDB_7(1);
   delay_ms(200);
}



//网络断开指示灯
void Net_LED (void)
{
	  
     DO_LEDI_0(1);
	   DO_LEDB_7(1);
		 IWDG_Feed(); //喂狗	 
	   DO_LEDB_5(0);
	   DO_LEDI_0(0);
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET);	//PB1置1  蜂鸣器
	   delay_ms(100);
	   DO_LEDB_5(1);
	   DO_LEDI_0(1);
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1置1  蜂鸣器
	   delay_ms(100);
}



//蜂鸣器 初始化  PI3
void BEEP_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;  
	  __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
	  GPIO_Initure.Pin=GPIO_PIN_3;
	  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
      GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
      GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
   	  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化PI0、PI1和PI2
	  HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1置1
}

//蜂鸣器长响
void BEEP_On(void)
{
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET);	//PB1置1
//		IWDG_Feed(); //喂狗	
	 delay_ms(500);
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1置1
}
//发送和接收指示灯
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



