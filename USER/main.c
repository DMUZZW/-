#include "main.h"

//��������
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" ϵͳ�����С���\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}
//��������������� �������������λ�������˸�λָ��֮�󣬽��忨���������ݷ��͸���λ�������������ð忨��������
void Send_NetLable(void)
{
		unsigned char Send_IP_Label,Send_IP_Label1;
		AT24CXX_Read(35,&Send_IP_Label,1); //������־		
		if((Connect_OK == TRUE)&& Send_IP_Label ==0x01)
		{
			//����һ���������
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
			AT24CXX_Write(35,&Send_IP_Label,1);//д��AT24C02	
			AT24CXX_Read(35,&Send_IP_Label1,1); //������־
			if(Send_IP_Label1 != 0x00)
			{
					AT24CXX_Write(35,&Send_IP_Label,1);//д��AT24C02	
			}			
		}
}

//Ӳ�����Ժ���
void Hardware_Test(void)
{
	//Ӳ����ͨ�ż����� 
	//������ 32·�����һ��� 8·AO��һ��� ������ٻָ���ʼ��
//	AODO_test();
	//LED ָʾ�Ƶ���
	LED_INIT_Test();
	//���������
	BEEP_On();	
	while(AT24CXX_Check())//���AT24C02�Ƿ��ܹ����������ܹ���������Ϣ����λ��
	{
	  HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1��0 ���İ��Ϻ�ɫ���ϵƳ���
		BEEP_On();//Ӳ����ͨ�ż����� 
	}
	//��̫�����Ӽ�����
  while(0==PHY_Check())//�����̫���ӿ��Ƿ������ϣ����û�У���ͨ��CAN���͵���λ��,û�����ӳɹ�����1
	{
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);	//PB1��0	
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1��0 ���İ��Ϻ�ɫ���ϵ�
	 IWDG_Feed(); //ι��	
	 delay_ms(500);	
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1��0	
	 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PB1��0 ���İ��Ϻ�ɫ���ϵ�
	 IWDG_Feed(); //ι��	
	 delay_ms(500);

	}

}

//�����ʼ������
void Peripheral_Init(void)
{
//      Cache_Enable();                 //��L1-Cache ģ�����ɼ���Ҫע�͵�
	    MPU_Memory_Protection();        //������ش洢����
      HAL_Init();				        //��ʼ��HAL��
      Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
      delay_init(216);                //��ʱ��ʼ��
	    uart_init(115200);		        //���ڳ�ʼ��
	 
	   /*�����ʼ�����ִ���*/	
     RNG_Init();
	   LED_Init(); //LED��ʼ��
	   BEEP_Init();//��������ʼ��
     Digital_Data_Init();//32DIDO ��ʼ��
	   AT24CXX_Init();//AT24C04��ʼ��
	   W5500_STM32();//W5500��ʼ��
	   SET_W5500_MAC();//
     AT24C02_System_Config_Initialiation();//��AT240C4�������������	
	   W5500_Start();
     Rheostat_Init();//ģ�����ɼ���ʼ��
     RTC_Init();//ʵʱʱ�ӳ�ʼ��
	   T_IIC_Init();				//IIC��ʼ�� 
	   SHT_Init();
	   AD5676_Init();//ģ���������ʼ��
	   BEEP_On();
	  //����������
	   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PE��1
     HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);	//PG��1
     SPI2_Send3Byte(0x003028f6);
	   SPI2_Send3Byte(0x003128f6);
	   SPI2_Send3Byte(0x003228f6);
	   SPI2_Send3Byte(0x003328f6);
	   SPI2_Send3Byte(0x003428f6);
	   SPI2_Send3Byte(0x003528f6);
	   SPI2_Send3Byte(0x003628f6);
	   SPI2_Send3Byte(0x003728f6);		
		
		
	   my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	   my_mem_init(SRAMDTCM);		    //��ʼ��CCM�ڴ�� 	 
     Hardware_Test();		
	   exfuns_init();	//Ϊfatfs��ر��������ڴ�				 
  	 f_mount(fs[0],"0:",1); //����SD�� 
	   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);	//PB1��1
		 TIM3_Init(10-1,10800-1); //��ʱ��3��ʼ������ʱ��ʱ��Ϊ108M����Ƶϵ��Ϊ1080-1,���Զ�ʱ��3��Ƶ��Ϊ108M/10800=10K���Զ���װ��Ϊ100-1����ô��ʱ�����ھ���1ms		
	   IWDG_Init(IWDG_PRESCALER_64,500);//1��

}

//SD�����
unsigned char SD_Check(void)
{
	 FRESULT res;	
	 unsigned char SD_state;
	 	 if(0!=SD_Init())//��ʼ������
	 {
		 SD_state =0; //SD ������
	 }
	 else //SD����ʼ����ȷ
	 {
		 res =f_mount(fs[0],"0:",1); 	//����SD��
		 if(res !=FR_OK)SD_state =0; //���ز��ɹ�
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
//	  SCB->VTOR = FLASH_BASE | 0x20000;//����ƫ����
//	unsigned char temp =0x00;
	   Peripheral_Init();//�����ʼ��	
	   Hardware_Test();
	
	
  	while(1)
	{
	    //******************************************************************************************************/
	   //Ӳ��ѭ�����
		 while(AT24CXX_Check())//���AT24C02�Ƿ��ܹ����������ܹ���������Ϣ����λ��
	   {
	    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1��0 ���İ��Ϻ�ɫ���ϵƳ���
		  BEEP_On();//Ӳ����ͨ�ż����� 
	   }
	   //��̫�����Ӽ�����
       while(0==PHY_Check())//�����̫���ӿ��Ƿ������ϣ����û�У���ͨ��CAN���͵���λ��,û�����ӳɹ�����1
	   {
		    BEEP_On();//Ӳ����ͨ�ż����� 
		    LED0_Toggle;
		    LED3_Toggle;
		    IWDG_Feed(); //ι��		 
		    delay_ms(500);
		    LED1(1);
		    LED2(1);
		    LED4(1);
	   }
		/*2018--9-12 ���� �����⵽�����Ѿ�����   ����ָʾ�Ʋ��� ������������*/
		if(PHY_Check()==1)//��������Ѿ����ӳɹ�
		{
		 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB1��0	
         HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);	//PB1��0 
		 HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);	//PB1��1 
		}
		//sd�����
	 if(0==SD_Check())///SD����ʼ��ʧ�ܻ�װ��ʧ��
	 { 

	 }		

		
/***************************************************************************************************/	
		//Ӧ�ó������

        do_udp();    		
  	    DigitalData_AnalogData_InputProcess();//��������ģ�����ɼ�	 
        Send_NetLable();
        IWDG_Feed(); //ι��		
		
	  //�����жϹ̼���С�Ƿ��Ѿ����͹���������Ѿ����͹���������й̼�����
	   for(i=0;i<4;i++)
	   {
			W5500_Parameter_Configuration.FirmwareLength[i] = 0x00; 
		 }
		 AT24CXX_Read(35,W5500_Parameter_Configuration.FirmwareLength,4);//����
		 if((W5500_Parameter_Configuration.FirmwareLength[0] !=0)||(W5500_Parameter_Configuration.FirmwareLength[1] !=0)||(W5500_Parameter_Configuration.FirmwareLength[2] !=0)||(W5500_Parameter_Configuration.FirmwareLength[3] !=0))
		 {
			 do_tcp_server_for_firmwareupadate();//���ڽ��չ̼�����,��һ�ν��� д��APP����������backup��
		 }



	}
}
