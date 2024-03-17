/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP 演示函数
* @attention  
************************************************************************************************
**/
#include "tcp_demo.h"
#include "iap.h" 


/**
*@brief		TCP Server回环演示函数。
*@param		无
*@return	无
*/
//boot版本的接收程序与app中的接收程序存在一些差别，boot程序为第一次接收程序，写入地址为APP_ADDRESS,app版本的为备份区地址
//第二个差别是，boot程序接收之后，是W5500_Parameter_Configuration.state = NORMAL_STATE，app程序则是W5500_Parameter_Configuration.state = NEW_APP_IN_BACK
unsigned int address = BACKUP_ADDRESS;//第一次写到APP_ADDRESS,以后写入要写入到BACKUP_ADDRESS
unsigned char isErased = 0;//是否擦除了flash
void do_tcp_server_for_firmwareupadate(void)
{	
	  unsigned short len=0;  
	  unsigned short i = 0,j=0;
    unsigned int temp=0x00;//用来存放暂时变量 
    unsigned int TCP_Receive_Length= 0x00,length_temp;
	  unsigned char UpdateSign;
	switch(getSn_SR(SOCK_TCPS))											            	/*获取socket的状态*/
	{
		case SOCK_CLOSED:													                  /*socket处于关闭状态*/
			socket(SOCK_TCPS ,Sn_MR_TCP,W5500_Parameter_Configuration.Sn_TCP_Port,Sn_MR_ND);	        /*打开socket*/
		  break;     
    
		case SOCK_INIT:														                  /*socket已初始化状态*/
			listen(SOCK_TCPS);												                /*socket建立监听*/
		  break;
		  
		case SOCK_ESTABLISHED:												              /*socket处于连接建立状态*/
		
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*清除接收中断标志位*/
			}
												          
			if((len=getSn_RX_RSR(SOCK_TCPS))>0)  /*定义len为已接收数据的长度*/
			{
                //1、接收代码并写入flash
				recv(SOCK_TCPS,Rx_Buffer,len);/*接收来自Client的数据*/
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
				//2、比较接收的数据大小是否与UDP传入的一致	   
				//以下代码需要进行测试 20200803  未测试
				TCP_Receive_Length +=len;//记录接收到的数据长度				
		  }
			
			for(j=0;j<4;j++)
	        {
			      W5500_Parameter_Configuration.FirmwareLength[j] = 0x00; 
		      }
					AT24CXX_Read(35,W5500_Parameter_Configuration.FirmwareLength,4);//读出
					length_temp = (W5500_Parameter_Configuration.FirmwareLength[0]<<24)+(W5500_Parameter_Configuration.FirmwareLength[1]<<16)+(W5500_Parameter_Configuration.FirmwareLength[2]<<8)+(W5500_Parameter_Configuration.FirmwareLength[3]);
					if(length_temp == TCP_Receive_Length)
					{
						//改变标志，重启后进入
						W5500_Parameter_Configuration.state = NEW_APP_IN_BACK;
						AT24CXX_Write(33,&W5500_Parameter_Configuration.state,1);//改变标志
						UpdateSign = 0xaa;
						AT24CXX_Write(39,&UpdateSign,1);//写入39位置
						delay_ms(10000);
						iap_load_app(APP_ADDRESS);//跳转
					}
					break;
		
		case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
			close(SOCK_TCPS);
		  break;
	}
}


/**
*@brief		TCP Client回环演示函数。
*@param		无
*@return	无
*/
void do_tcp_client(void)
{	
   unsigned short len=0;	

	switch(getSn_SR(SOCK_TCPC))								  				         /*获取socket的状态*/
	{
		case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
			socket(SOCK_TCPC,Sn_MR_TCP,W5500_Parameter_Configuration.Sn_TCP_Port,Sn_MR_ND);
		  break;
		
		case SOCK_INIT:													        	         /*socket处于初始化状态*/
			connect(SOCK_TCPC,W5500_Parameter_Configuration.REMOTE_Sn_DIP,W5500_Parameter_Configuration.Sn_TCP_DPort);                /*socket连接服务器*/ 
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*清除接收中断标志位*/
			}
		
			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*定义len为已接收数据的长度*/
			if(len>0)
			{
				recv(SOCK_TCPC,Rx_Buffer,len); 							   		         /*接收来自Server的数据*/
				Rx_Buffer[len]=0x00;  											                 /*添加字符串结束符*/
				printf("%s\r\n",Rx_Buffer);
				send(SOCK_TCPC,Rx_Buffer,len);								     	         /*向Server发送数据*/
			}		  
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
			close(SOCK_TCPC);
		  break;

	}
}


