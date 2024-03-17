#include "main.h"

//重启函数
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" 系统重启中……\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}
//发送网络参数函数 这个函数是在上位机发送了复位指令之后，将板卡的网络数据发送给上位机，其作用是让板卡重新上线
void Send_NetLable(void)
{
		unsigned char Send_IP_Label,Send_IP_Label1;
		AT24CXX_Read(35,&Send_IP_Label,1); //读出标志		
		if((Connect_OK == TRUE)&& Send_IP_Label ==0x01)
		{
			//发送一次网络参数
			Tx_Buffer[0] = 0xfe;
			Tx_Buffer[1] = 0x09;
			Tx_Buffer[2] = 0x83;
			Tx_Buffer[3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0];
			Tx_Buffer[4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1];
			Tx_Buffer[5] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2];
			Tx_Buffer[6] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3];
			Tx_Buffer[7] = GetXor(&Tx_Buffer[1],5);
			Tx_Buffer[8] = 0xff;
			sendto(SOCK_UDPS,Tx_Buffer,9, W5500_Parameter_Configuration.REMOTE_Sn_DIP, W5500_Parameter_Configuration.Sn_UDP_DPort);
			if(1==SD_Check())
		  SD_Creat_CSVFile(0,Tx_Buffer,SendandReceiveTimes++);
			Send_IP_Label = 0x00;
			AT24CXX_Write(35,&Send_IP_Label,1);//写入AT24C02	
			AT24CXX_Read(35,&Send_IP_Label1,1); //读出标志
			if(Send_IP_Label1 != 0x00)
			{
					AT24CXX_Write(35,&Send_IP_Label,1);//写入AT24C02	
			}			
		}
}

//硬件测试函数
void Hardware_Test(void)
{
	//硬件与通信检测代码 
	//输出检测 32路输出逐一输出 8路AO逐一输出 输出后再恢复初始化
//	AODO_test();
	//LED 指示灯点亮
	LED_INIT_Test();
	//蜂鸣器检测
	BEEP_On();	
	while(AT24CXX_Check())//检测AT24C02是否能够工作，不能工作则发送信息到上位机
	{
	  HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1置0 核心板上红色故障灯常亮
		BEEP_On();//硬件与通信检测代码 
	}
	//以太网连接检测代码
  while(0==PHY_Check())//检测以太网接口是否连接上，如果没有，则通过CAN发送到上位机,没有连接成功返回1
	{
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);	//PB1置0	
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1置0 核心板上红色故障灯
	 IWDG_Feed(); //喂狗	
	 delay_ms(500);	
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1置0	
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PB1置0 核心板上红色故障灯
	 IWDG_Feed(); //喂狗	
	 delay_ms(500);

	}

}

//外设初始化函数
void Peripheral_Init(void)
{
//      Cache_Enable();                 //打开L1-Cache 模拟量采集需要注释掉
	    MPU_Memory_Protection();        //保护相关存储区域
      HAL_Init();				        //初始化HAL库
      Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
      delay_init(216);                //延时初始化
	    uart_init(115200);		        //串口初始化
	 
	   /*外设初始化部分代码*/	
     RNG_Init();
	   LED_Init(); //LED初始化
	   BEEP_Init();//蜂鸣器初始化
     Digital_Data_Init();//32DIDO 初始化
	   AT24CXX_Init();//AT24C04初始化
	   W5500_STM32();//W5500初始化
	   SET_W5500_MAC();//
     AT24C02_System_Config_Initialiation();//用AT240C4来配置网络参数	
	   W5500_Start();
     Rheostat_Init();//模拟量采集初始化
     RTC_Init();//实时时钟初始化
	   T_IIC_Init();				//IIC初始化 
	   SHT_Init();
	   AD5676_Init();//模拟量输出初始化
	   BEEP_On();
	  //清除所有输出
	   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE置1
     HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG置1
     SPI2_Send3Byte(0x003028f6);
	   SPI2_Send3Byte(0x003128f6);
	   SPI2_Send3Byte(0x003228f6);
	   SPI2_Send3Byte(0x003328f6);
	   SPI2_Send3Byte(0x003428f6);
	   SPI2_Send3Byte(0x003528f6);
	   SPI2_Send3Byte(0x003628f6);
	   SPI2_Send3Byte(0x003728f6);		
		
		
	   my_mem_init(SRAMIN);		    //初始化内部内存池
	   my_mem_init(SRAMDTCM);		    //初始化CCM内存池 	 
     Hardware_Test();		
	   exfuns_init();	//为fatfs相关变量申请内存				 
  	 f_mount(fs[0],"0:",1); //挂载SD卡 
	   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);	//PB1置1
		 TIM3_Init(10-1,10800-1); //定时器3初始化，定时器时钟为108M，分频系数为1080-1,所以定时器3的频率为108M/10800=10K，自动重装载为100-1，那么定时器周期就是1ms		
	   IWDG_Init(IWDG_PRESCALER_64,500);//1秒

}

//SD卡检测
unsigned char SD_Check(void)
{
	 FRESULT res;	
	 unsigned char SD_state;
	 	 if(0!=SD_Init())//初始化错误
	 {
		 SD_state =0; //SD 卡错误
	 }
	 else //SD卡初始化正确
	 {
		 res =f_mount(fs[0],"0:",1); 	//挂载SD卡
		 if(res !=FR_OK)SD_state =0; //挂载不成功
		 else if(res ==FR_OK)SD_state =1;	
	 }

	return SD_state;
}



void receive_send_led(void)
{
	    if(DataReceived ==TRUE)
		{
			DataReceived=FALSE;
			LED_receive_timers=0;
			LED0(0);
            LED3(0);			
		}
		if(LED_receive_timers>=200)
		{
			LED0(1);
			LED3(1);
			LED_receive_timers=0;
		}
		
		if(DataSent==TRUE)
		{
			DataSent=FALSE;
			LED_send_timers=0;
		  LED0(0);
      LED3(0);	
		}
		if(LED_send_timers>=200)
		{
		    LED0(1);
			LED3(1);
			LED_send_timers=0;
		}
}




int main(void)
{
	 
    unsigned char Analog_channel=0x01;
	  unsigned char APP_Running_Address_Flag = 0x00;
	  unsigned int APP_Running_Address ;
	  unsigned char i;
//	  SCB->VTOR = FLASH_BASE | 0x20000;//设置偏移量
//	unsigned char temp =0x00;
	   Peripheral_Init();//外设初始化	
	   Hardware_Test();
	
	
  	while(1)
	{
	    //******************************************************************************************************/
	   //硬件循环检测
		 while(AT24CXX_Check())//检测AT24C02是否能够工作，不能工作则发送信息到上位机
	   {
	    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1置0 核心板上红色故障灯常亮
		  BEEP_On();//硬件与通信检测代码 
	   }
	   //以太网连接检测代码
       while(0==PHY_Check())//检测以太网接口是否连接上，如果没有，则通过CAN发送到上位机,没有连接成功返回1
	   {
		    BEEP_On();//硬件与通信检测代码 
		    LED0_Toggle;
		    LED3_Toggle;
		    IWDG_Feed(); //喂狗		 
		    delay_ms(500);
		    LED1(1);
		    LED2(1);
		    LED4(1);
	   }
		/*2018--9-12 补充 如果检测到网线已经连接   故障指示灯不亮 蜂鸣器不响了*/
		if(PHY_Check()==1)//如果网线已经连接成功
		{
		 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1置0	
         HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置0 
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1置1 
		}
		//sd卡检测
	 if(0==SD_Check())///SD卡初始化失败或装载失败
	 { 

	 }		

		
/***************************************************************************************************/	
		//应用程序代码

        do_udp();    		
  	    DigitalData_AnalogData_InputProcess();//数字量和模拟量采集	 
        Send_NetLable();
        IWDG_Feed(); //喂狗		
		
	  //首先判断固件大小是否已经发送过来，如果已经发送过来，则进行固件接收
	   for(i=0;i<4;i++)
	   {
			W5500_Parameter_Configuration.FirmwareLength[i] = 0x00; 
		 }
		 AT24CXX_Read(35,W5500_Parameter_Configuration.FirmwareLength,4);//读出
		 if((W5500_Parameter_Configuration.FirmwareLength[0] !=0)||(W5500_Parameter_Configuration.FirmwareLength[1] !=0)||(W5500_Parameter_Configuration.FirmwareLength[2] !=0)||(W5500_Parameter_Configuration.FirmwareLength[3] !=0))
		 {
			 do_tcp_server_for_firmwareupadate();//用于接收固件代码,第一次接收 写入APP区，而不是backup区
		 }



	}
}
