#ifndef __TIMER_H
#define __TIMER_H


#include "stm32f7xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "DIDOAIAO.h"


extern unsigned int Timer_Couter;
extern unsigned char Timer_Couter_Minute;
extern unsigned int timers;
extern unsigned char LED_receive_timers;
extern unsigned char LED_send_timers;


extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 

void TIM3_Init(u16 arr,u16 psc);    //��ʱ����ʼ��


#endif

