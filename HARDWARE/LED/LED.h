#ifndef _LED_H
#define _LED_H

#include "sys.h"
#include "32DIDO.h"
#include "define.h"
extern void LED_Init(void);
extern void LED_Test(void);
extern void BEEP_Init(void);
extern void BEEP_On(void);
extern void LED_INIT_Test(void);
extern void Net_LED (void);
extern void Send_Receive_Led(void);
extern void ZHENGDIANYUANZI_LED_Init(void);
#define LED0(n)		(n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET))
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0)) //LED0输出电平翻转

#define LED1(n)		(n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_RESET))
#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1)) //LED1输出电平翻转

#define LED2(n)		(n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_RESET))
#define LED2_Toggle (HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_2)) //LED2输出电平翻转

#define LED3(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))
#define LED3_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5)) //LED0输出电平翻转

#define LED4(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))
#define LED4_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7)) //LED1输出电平翻转







#define DO_LEDI_0(x) GPIO_Pin_Set(GPIOI,PIN0,x)
#define DO_LEDI_1(x) GPIO_Pin_Set(GPIOI,PIN1,x)
#define DO_LEDI_2(x) GPIO_Pin_Set(GPIOI,PIN2,x)
#define DO_LEDB_5(x) GPIO_Pin_Set(GPIOB,PIN5,x)
#define DO_LEDB_7(x) GPIO_Pin_Set(GPIOB,PIN7,x)

//#define LEDgreen1_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)) //LED0输出电平翻转
//#define LEDgreen2_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1)) //LED0输出电平翻转


#endif
