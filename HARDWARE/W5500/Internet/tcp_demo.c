/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP ��ʾ����
* @attention  
************************************************************************************************
**/
#include "tcp_demo.h"
#include "iap.h" 


/**
*@brief		TCP Server�ػ���ʾ������
*@param		��
*@return	��
*/
//boot�汾�Ľ��ճ�����app�еĽ��ճ������һЩ���boot����Ϊ��һ�ν��ճ���д���ַΪAPP_ADDRESS,app�汾��Ϊ��������ַ
//�ڶ�������ǣ�boot�������֮����W5500_Parameter_Configuration.state = NORMAL_STATE��app��������W5500_Parameter_Configuration.state = NEW_APP_IN_BACK
unsigned int address = BACKUP_ADDRESS;//��һ��д��APP_ADDRESS,�Ժ�д��Ҫд�뵽BACKUP_ADDRESS
unsigned char isErased = 0;//�Ƿ������flash
void do_tcp_server_for_firmwareupadate(void)
{	
	  unsigned short len=0;  
	  unsigned short i = 0,j=0;
    unsigned int temp=0x00;//���������ʱ���� 
    unsigned int TCP_Receive_Length= 0x00,length_temp;
	  unsigned char UpdateSign;
	switch(getSn_SR(SOCK_TCPS))											            	/*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(SOCK_TCPS ,Sn_MR_TCP,W5500_Parameter_Configuration.Sn_TCP_Port,Sn_MR_ND);	        /*��socket*/
		  break;     
    
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(SOCK_TCPS);												                /*socket��������*/
		  break;
		  
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
		
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*��������жϱ�־λ*/
			}
												          
			if((len=getSn_RX_RSR(SOCK_TCPS))>0)  /*����lenΪ�ѽ������ݵĳ���*/
			{
                //1�����մ��벢д��flash
				recv(SOCK_TCPS,Rx_Buffer,len);/*��������Client������*/
				send(SOCK_TCPS,Rx_Buffer,len);		
				printf("receive length %d: .......................................... \r\n",len);
			    for(i=0;i<len;i+=4)
			   {
				 temp = (unsigned int)Rx_Buffer[i+3]<<24;
				 temp |= (unsigned int)Rx_Buffer[i+2]<<16;
				 temp |= (unsigned int)Rx_Buffer[i+1]<<8;
				 temp |= (unsigned int)Rx_Buffer[i]; 
				 FLASH_Program(address,&temp,1);
				 address+=4;
			   }
				//2���ȽϽ��յ����ݴ�С�Ƿ���UDP�����һ��	   
				//���´�����Ҫ���в��� 20200803  δ����
				TCP_Receive_Length +=len;//��¼���յ������ݳ���				
		  }
			
			for(j=0;j<4;j++)
	        {
			      W5500_Parameter_Configuration.FirmwareLength[j] = 0x00; 
		      }
					AT24CXX_Read(35,W5500_Parameter_Configuration.FirmwareLength,4);//����
					length_temp = (W5500_Parameter_Configuration.FirmwareLength[0]<<24)+(W5500_Parameter_Configuration.FirmwareLength[1]<<16)+(W5500_Parameter_Configuration.FirmwareLength[2]<<8)+(W5500_Parameter_Configuration.FirmwareLength[3]);
					if(length_temp == TCP_Receive_Length)
					{
						//�ı��־�����������
						W5500_Parameter_Configuration.state = NEW_APP_IN_BACK;
						AT24CXX_Write(33,&W5500_Parameter_Configuration.state,1);//�ı��־
						UpdateSign = 0xaa;
						AT24CXX_Write(39,&UpdateSign,1);//д��39λ��
						delay_ms(10000);
						iap_load_app(APP_ADDRESS);//��ת
					}
					break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPS);
		  break;
	}
}


/**
*@brief		TCP Client�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_client(void)
{	
   unsigned short len=0;	

	switch(getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			socket(SOCK_TCPC,Sn_MR_TCP,W5500_Parameter_Configuration.Sn_TCP_Port,Sn_MR_ND);
		  break;
		
		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			connect(SOCK_TCPC,W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_TCP_DPort);                /*socket���ӷ�����*/ 
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}
		
			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPC,Rx_Buffer,len); 							   		         /*��������Server������*/
				Rx_Buffer[len]=0x00;  											                 /*����ַ���������*/
				printf("%s\r\n",Rx_Buffer);
				send(SOCK_TCPC,Rx_Buffer,len);								     	         /*��Server��������*/
			}		  
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPC);
		  break;

	}
}


