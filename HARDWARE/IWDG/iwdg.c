#include "iwdg.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////		

IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄

//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;    //设置IWDG分频系数
    IWDG_Handler.Init.Reload=rlr;        //重装载
    IWDG_Handler.Init.Window=IWDG_WINDOW_DISABLE;//关闭窗口功能
    HAL_IWDG_Init(&IWDG_Handler);    
}
    
//喂独立看门狗
void IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); //重装载
}
