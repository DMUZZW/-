#include "32DIDO.h"



//数字量输入端口初始化
//数字量输入端口是: PA15、PD0、PD1、PD3、PD4、PD5、PD6、PD7、PD8、PD9、PD10、PD11、PD12、PD13、PD14、PD15、PI10、PI11、PF0 PF1 PF2 PF3 PF4 PF5 PF8 PF9 PF10 PF11 PF12 PF13 PF14 PF15
void Digital_DataInput_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	  __HAL_RCC_GPIOD_CLK_ENABLE();     //开启GPIOD时钟
 	  __HAL_RCC_GPIOF_CLK_ENABLE();     //开启GPIOF时钟
	  __HAL_RCC_GPIOI_CLK_ENABLE();     //开启GPIOI时钟
	
	 //初始化PD端口 除了PD2没有被初始化 其他引脚设置成输入上拉模式
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; //PA15
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);    // 初始化 PD端口
	
	//初始化PF端口 除了PF6和PF7 其他引脚都被初始化成输入上拉模式
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; //PA15
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);    //初始化PF端口
		
	//初始化PI10和PI11引脚，设置成输入上拉模式，在外部引脚为低电平时有效
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11; //PI10和PI11
	  GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);    //初始化PI10和PI11
}

//数字量输出引脚初始化
//数字量输出的引脚是：PE和PG口
void Digital_DataOutput_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOE时钟
	  __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOG时钟
	 
	//PE口初始化为输出
    GPIO_Initure.Pin=GPIO_PIN_All; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE置1
	
	//PG口初始化为输出
  	GPIO_Initure.Pin=GPIO_PIN_All; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //
	  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG置1

}
//初始化输入输出引脚
void Digital_Data_Init(void)
{
	Digital_DataInput_Init();
	Digital_DataOutput_Init();
}










//GPIO复用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,代表IO引脚编号.
//AFx:0~15,代表AF0~AF15.
//AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,unsigned char BITx,unsigned char AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIO通用设置 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,低速;1,中速;2,快速;3,高速. 
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void GPIO_Set(GPIO_TypeDef* GPIOx,unsigned int BITx,unsigned int MODE,unsigned int OTYPE,unsigned int OSPEED,unsigned int PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//一个个位检查 
		curpin=BITx&pos;//检查引脚是否要设置
		if(curpin==pos)	//需要设置
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式 
			if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
} 
//设置GPIO某个引脚的输出状态
//GPIOx:GPIOA~GPIOI.
//pinx:引脚编号,范围:0~15
//status:引脚状态(仅最低位有效),0,输出低电平;1,输出高电平
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,unsigned short pinx,unsigned char status)
{
	if(status&0X01)GPIOx->BSRR=pinx;	//设置GPIOx的pinx为1
	else GPIOx->BSRR=pinx<<16;			//设置GPIOx的pinx为0
}
//读取GPIO某个引脚的状态
//GPIOx:GPIOA~GPIOI.
//pinx:引脚编号,范围:0~15
//返回值:引脚状态,0,引脚低电平;1,引脚高电平
unsigned char GPIO_Pin_Get(GPIO_TypeDef* GPIOx,unsigned short pinx)
{ 
	if(GPIOx->IDR&pinx)return 1;		//pinx的状态为1
	else return 0;						//pinx的状态为0
}





