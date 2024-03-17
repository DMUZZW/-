#include "Timer.h"


unsigned int  Timer_Couter=0x00;//���ں������
unsigned char Timer_Couter_Minute=0x00;//���ڼ���1����
unsigned int  timers=0;
unsigned char LED_receive_timers=0;
unsigned char LED_send_timers=0;


TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE    
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}  
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//��ʱ��3�жϷ���������  1ms����һ���ж�
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//�����ж�
    if(htim==(&TIM3_Handler))
    {
        timers++;
		LED_receive_timers++;
		LED_send_timers++;
		if(timers==500)
		 {
			   timers=0;
//			   LED1_Toggle;
			   LED2_Toggle;
//			   LED4_Toggle;
         IWDG_Feed(); //ι��				 
		 }
		

		if(LED_receive_timers==100)
		{
			 LED0(1);
			 LED_receive_timers=0;
		 }
		 if(LED_send_timers==100)
		 {
			LED3(1);	
			LED_send_timers=0;
		 }

		if(Timer_Couter==1000) //1s���ۼ�1000ms
		{
			IWDG_Feed(); //ι��	
			Timer_Couter=0;
			Timer_Couter_Minute++;			
		}
		if(Timer_Couter>=1000)Timer_Couter=0;
		if(Timer_Couter_Minute ==60)//1����
		{
			Timer_Couter_Minute=0;
            IWDG_Feed(); //ι��	
		}
		if(Timer_Couter_Minute>=60)Timer_Couter_Minute=0;		
    }	
}




























