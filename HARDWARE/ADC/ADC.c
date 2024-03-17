#include "ADC.h"



//模拟量采集GPIO设置


__IO unsigned short ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};
unsigned short ADC_CovertValueToReadFromDMA[RHEOSTAT_NOFCHANEL]={0};
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;
uint16_t ADC_ConvertedValue2[RHEOSTAT_NOFCHANEL];


static void Rheostat_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*=====================通道1======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK1_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN1;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);
    
    /*=====================通道2======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK2_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN2;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);
    
    /*=====================通道3======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK3_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN3;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);  
		
		  /*=====================通道4======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK4_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN4;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT4, &GPIO_InitStructure);  
		
		/*=====================通道5======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK5_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN5;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT5, &GPIO_InitStructure);  
		
		 /*=====================通道6======================*/
    // 使能 GPIO 时钟
    RHEOSTAT_ADC_GPIO_CLK6_ENABLE();    
    // 配置 IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN6;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT6, &GPIO_InitStructure);  
}


static void Rheostat_ADC_Mode_Config(void)
{

    // ------------------DMA Init 结构体参数 初始化--------------------------
    // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
    // 开启DMA时钟
    RHEOSTAT_ADC_DMA_CLK_ENABLE();
    // 数据传输通道
    DMA_Init_Handle.Instance = RHEOSTAT_ADC_DMA_STREAM;
    // 数据传输方向为外设到存储器	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // 外设寄存器只有一个，地址不用递增
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // 存储器地址固定
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // 外设数据大小为半字，即两个字节 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	存储器数据大小也为半字，跟外设数据大小相同
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
    // 循环传输模式
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // 禁止DMA FIFO	，使用直连模式
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO 大小，FIFO模式禁止时，这个不用配置	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // 选择 DMA 通道，通道存在于流中
    DMA_Init_Handle.Init.Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
    //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
    HAL_DMA_Init(&DMA_Init_Handle); 

    HAL_DMA_Start (&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,RHEOSTAT_NOFCHANEL);

    // 开启ADC时钟
    RHEOSTAT_ADC_CLK_ENABLE();
    // -------------------ADC Init 结构体 参数 初始化------------------------
    // ADC1
    ADC_Handle.Instance = RHEOSTAT_ADC;
    // 时钟为fpclk 4分频	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC 分辨率
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // 扫描模式，多通道采集才需要	
    ADC_Handle.Init.ScanConvMode = ENABLE; 
    // 连续转换	
    ADC_Handle.Init.ContinuousConvMode = ENABLE;
    // 非连续转换	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // 非连续转换个数
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //禁止外部边沿触发    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //使用软件触发，外部触发不用配置，注释掉即可
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //数据右对齐	
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //转换通道个数
    ADC_Handle.Init.NbrOfConversion = RHEOSTAT_NOFCHANEL;
    //使能连续转换请求
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;
    //转换完成标志
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // 初始化ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    // 配置 ADC 通道1转换顺序为1，第一个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL1;
    ADC_Config.Rank         = 1;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // 配置 ADC 通道2转换顺序为2，第二个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL2;
    ADC_Config.Rank         = 2;
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES; // 采样时间间隔	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // 配置 ADC 通道3转换顺序为3，第三个转换，采样时间为3个时钟周期
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL3;
    ADC_Config.Rank         = 3;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
		    // 配置 ADC 通道4转换顺序为4，
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL4;
    ADC_Config.Rank         = 4;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
				    // 配置 ADC 通道5转换顺序为5，
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL5;
    ADC_Config.Rank         = 5;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
				    // 配置 ADC 通道6转换顺序为6，
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL6;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// 采样时间间隔
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
		

    HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1);
}


void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}
//求指定通道的平均值 ////channel 是要读取的那个通道，函数固定了6个通道 
unsigned short ReadADCAverageValue(unsigned char channel,unsigned char times)
{
	unsigned char i=0x00,j=0x00;
	unsigned int sum[6]={0,};
	unsigned short average[6]={0,};
	for(i=0;i<times;i++)
	{
		HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1);
		for(j=0;j<6;j++)
		{
			sum[j] += ADC_ConvertedValue[j];
		}	
	}
	//取平均
	for(i=0;i<6;i++)
  {
		average[i] = (sum[i]/times) & 0x0ff0;//
	}	
	return average[channel];	
	
}


//打印模拟量采集数据
void PRINT(void)
{
   float ADC_ConvertedValueLocal[RHEOSTAT_NOFCHANEL]={0};
	 unsigned short ADC_NEW[RHEOSTAT_NOFCHANEL];
	 ADC_NEW[0] = ReadADCAverageValue(0,100);
	 ADC_NEW[1] = ReadADCAverageValue(1,100);
	 ADC_NEW[2] = ReadADCAverageValue(2,100);
	 ADC_NEW[3] = ReadADCAverageValue(3,100);
	 ADC_NEW[4] = ReadADCAverageValue(4,100);
	 ADC_NEW[5] = ReadADCAverageValue(5,100);
	 
		ADC_ConvertedValueLocal[0] =(float) ADC_NEW[0]/4096*(float)3.3;
		ADC_ConvertedValueLocal[1] =(float) ADC_NEW[1]/4096*(float)3.3;
		ADC_ConvertedValueLocal[2] =(float) ADC_NEW[2]/4096*(float)3.3;
		ADC_ConvertedValueLocal[3] =(float) ADC_NEW[3]/4096*(float)3.3;
		ADC_ConvertedValueLocal[4] =(float) ADC_NEW[4]/4096*(float)3.3;
		ADC_ConvertedValueLocal[5] =(float) ADC_NEW[5]/4096*(float)3.3;
			
		printf("\r\n CH1_PC0 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
		printf("\r\n CH2_PC1 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
		printf("\r\n CH3_PC2 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
		printf("\r\n CH3_PC3 value = %f V \r\n",ADC_ConvertedValueLocal[3]);
		printf("\r\n CH3_PA1 value = %f V \r\n",ADC_ConvertedValueLocal[4]);
		printf("\r\n CH3_PA2 value = %f V \r\n",ADC_ConvertedValueLocal[5]);
		printf("\r\n\r\n");
	  delay_ms(1000);
	
}










