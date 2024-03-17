#include "ADC.h"



//ģ�����ɼ�GPIO����


__IO unsigned short ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};
unsigned short ADC_CovertValueToReadFromDMA[RHEOSTAT_NOFCHANEL]={0};
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;
uint16_t ADC_ConvertedValue2[RHEOSTAT_NOFCHANEL];


static void Rheostat_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*=====================ͨ��1======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK1_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN1;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);
    
    /*=====================ͨ��2======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK2_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN2;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);
    
    /*=====================ͨ��3======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK3_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN3;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);  
		
		  /*=====================ͨ��4======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK4_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN4;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT4, &GPIO_InitStructure);  
		
		/*=====================ͨ��5======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK5_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN5;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT5, &GPIO_InitStructure);  
		
		 /*=====================ͨ��6======================*/
    // ʹ�� GPIO ʱ��
    RHEOSTAT_ADC_GPIO_CLK6_ENABLE();    
    // ���� IO
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN6;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT6, &GPIO_InitStructure);  
}


static void Rheostat_ADC_Mode_Config(void)
{

    // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
    // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
    // ����DMAʱ��
    RHEOSTAT_ADC_DMA_CLK_ENABLE();
    // ���ݴ���ͨ��
    DMA_Init_Handle.Instance = RHEOSTAT_ADC_DMA_STREAM;
    // ���ݴ��䷽��Ϊ���赽�洢��	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // �洢����ַ�̶�
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // �������ݴ�СΪ���֣��������ֽ� 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
    // ѭ������ģʽ
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO ��С��FIFOģʽ��ֹʱ�������������	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // ѡ�� DMA ͨ����ͨ������������
    DMA_Init_Handle.Init.Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
    //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
    HAL_DMA_Init(&DMA_Init_Handle); 

    HAL_DMA_Start (&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,RHEOSTAT_NOFCHANEL);

    // ����ADCʱ��
    RHEOSTAT_ADC_CLK_ENABLE();
    // -------------------ADC Init �ṹ�� ���� ��ʼ��------------------------
    // ADC1
    ADC_Handle.Instance = RHEOSTAT_ADC;
    // ʱ��Ϊfpclk 4��Ƶ	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC �ֱ���
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // ɨ��ģʽ����ͨ���ɼ�����Ҫ	
    ADC_Handle.Init.ScanConvMode = ENABLE; 
    // ����ת��	
    ADC_Handle.Init.ContinuousConvMode = ENABLE;
    // ������ת��	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // ������ת������
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //��ֹ�ⲿ���ش���    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //ʹ������������ⲿ�����������ã�ע�͵�����
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //�����Ҷ���	
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //ת��ͨ������
    ADC_Handle.Init.NbrOfConversion = RHEOSTAT_NOFCHANEL;
    //ʹ������ת������
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;
    //ת����ɱ�־
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // ��ʼ��ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    // ���� ADC ͨ��1ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL1;
    ADC_Config.Rank         = 1;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // ���� ADC ͨ��2ת��˳��Ϊ2���ڶ���ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL2;
    ADC_Config.Rank         = 2;
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES; // ����ʱ����	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    
    // ���� ADC ͨ��3ת��˳��Ϊ3��������ת��������ʱ��Ϊ3��ʱ������
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL3;
    ADC_Config.Rank         = 3;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
		    // ���� ADC ͨ��4ת��˳��Ϊ4��
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL4;
    ADC_Config.Rank         = 4;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
				    // ���� ADC ͨ��5ת��˳��Ϊ5��
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL5;
    ADC_Config.Rank         = 5;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
				    // ���� ADC ͨ��6ת��˳��Ϊ6��
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL6;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ����ʱ����
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
		
		

    HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1);
}


void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}
//��ָ��ͨ����ƽ��ֵ ////channel ��Ҫ��ȡ���Ǹ�ͨ���������̶���6��ͨ�� 
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
	//ȡƽ��
	for(i=0;i<6;i++)
  {
		average[i] = (sum[i]/times) & 0x0ff0;//
	}	
	return average[channel];	
	
}


//��ӡģ�����ɼ�����
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










