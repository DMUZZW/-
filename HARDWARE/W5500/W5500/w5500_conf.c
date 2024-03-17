
#include "w5500_conf.h"

#include <stdio.h>
#include <string.h>
#include "delay.h"

#include "w5500.h"

/**
******************************************************************************
* @file    		W5500.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		读写W5500寄存器底层函数
******************************************************************************
*/

unsigned short udp_local_port=0;	       //UDP socket                					/*定义本地端口*/
unsigned short udp_remote_port=0;				 	//UDP socket											/*远端端口号*/
unsigned short tcp_local_port = 0;     //TCP socket 
unsigned short tcp_remote_port = 0;    //TCP socket 

//W5500_NetParameter_Config W5500_Parameter_Configuration;
///*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
//uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

///*定义默认IP信息*/
//uint8 local_ip[4]  ={172,18,137,199};										/*定义W5500默认IP地址*/
//uint8 subnet[4]    ={255,255,0,0};										/*定义W5500默认子网掩码*/
//uint8 gateway[4]   ={172,18,137,1};											/*定义W5500默认网关*/
//uint8 dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/
//unsigned short local_port=5000;	                       					/*定义本地端口*/

/*定义远端IP信息*/
//uint8  remote_ip[4]={172,18,137,20};			  						/*远端IP地址*/
//unsigned short remote_port=9000;																/*远端端口号*/

/*IP配置方法选择，请自行选择*/
uint8	ip_from=IP_FROM_DEFINE;				

uint8         dhcp_ok   = 0;													   			/*dhcp成功获取IP*/
uint32	      ms        = 0;															  	/*毫秒计数*/
uint32	      dhcp_time = 0;															  	/*DHCP运行计数*/
__IO uint8_t  ntptimer  = 0;															  	/*NPT秒计数*/

/*W5500网络参数结构体*/

///*此结构体定义了W5500可供配置的主要参数*/
//typedef struct _W5500_NetParameter_Config											
//{
//   unsigned char MAC_Addr[6];																							/*MAC地址*/
//   unsigned char LOCAL_IP_Addr[4];																							/*local IP本地IP地址*/
//   unsigned char Sub_Mask[4];																							/*子网掩码*/
//   unsigned char Gateway_IP[4];																							/*网关*/	
//   unsigned char REMOTE_Sn_DIP[4];	                                            //端口目的IP地址 
//   
//	 unsigned short Sn_UDP_Port;
//   unsigned short Sn_UDP_DPort;
//   unsigned short Sn_TCP_Port;
//   unsigned short Sn_TCP_DPort;


//	
////	 unsigned char Sn_UDP_Port[2];	                                        //UDP通信端口号
////   unsigned char Sn_UDP_DPort[2] ;	                                    //UDP通信目标端口号  
//  
////   unsigned char Sn_TCP_Port[2];	//tcp通信端口号
////	 unsigned char Sn_TCP_DPort[2] ;	//tcp通信目标端口号
//	 unsigned char dns[4];																							   /*DNS服务器地址*/
////   unsigned char rip[4];																						    	/*remote IP远程IP地址*/
//	 unsigned char SoftWare_Version[2];																					/*软件版本号*/
//	 
//	 unsigned short fw_len;
//	 unsigned  char state;
//	 unsigned char  debug;//用于调试
//	

//}NetParameter_Config;


