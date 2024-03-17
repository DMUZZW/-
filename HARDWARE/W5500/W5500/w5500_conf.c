
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
* @brief   		��дW5500�Ĵ����ײ㺯��
******************************************************************************
*/

unsigned short udp_local_port=0;	       //UDP socket                					/*���屾�ض˿�*/
unsigned short udp_remote_port=0;				 	//UDP socket											/*Զ�˶˿ں�*/
unsigned short tcp_local_port = 0;     //TCP socket 
unsigned short tcp_remote_port = 0;    //TCP socket 

//W5500_NetParameter_Config W5500_Parameter_Configuration;
///*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
//uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

///*����Ĭ��IP��Ϣ*/
//uint8 local_ip[4]  ={172,18,137,199};										/*����W5500Ĭ��IP��ַ*/
//uint8 subnet[4]    ={255,255,0,0};										/*����W5500Ĭ����������*/
//uint8 gateway[4]   ={172,18,137,1};											/*����W5500Ĭ������*/
//uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/
//unsigned short local_port=5000;	                       					/*���屾�ض˿�*/

/*����Զ��IP��Ϣ*/
//uint8  remote_ip[4]={172,18,137,20};			  						/*Զ��IP��ַ*/
//unsigned short remote_port=9000;																/*Զ�˶˿ں�*/

/*IP���÷���ѡ��������ѡ��*/
uint8	ip_from=IP_FROM_DEFINE;				

uint8         dhcp_ok   = 0;													   			/*dhcp�ɹ���ȡIP*/
uint32	      ms        = 0;															  	/*�������*/
uint32	      dhcp_time = 0;															  	/*DHCP���м���*/
__IO uint8_t  ntptimer  = 0;															  	/*NPT�����*/

/*W5500��������ṹ��*/

///*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
//typedef struct _W5500_NetParameter_Config											
//{
//   unsigned char MAC_Addr[6];																							/*MAC��ַ*/
//   unsigned char LOCAL_IP_Addr[4];																							/*local IP����IP��ַ*/
//   unsigned char Sub_Mask[4];																							/*��������*/
//   unsigned char Gateway_IP[4];																							/*����*/	
//   unsigned char REMOTE_Sn_DIP[4];	                                            //�˿�Ŀ��IP��ַ 
//   
//	 unsigned short Sn_UDP_Port;
//   unsigned short Sn_UDP_DPort;
//   unsigned short Sn_TCP_Port;
//   unsigned short Sn_TCP_DPort;


//	
////	 unsigned char Sn_UDP_Port[2];	                                        //UDPͨ�Ŷ˿ں�
////   unsigned char Sn_UDP_DPort[2] ;	                                    //UDPͨ��Ŀ��˿ں�  
//  
////   unsigned char Sn_TCP_Port[2];	//tcpͨ�Ŷ˿ں�
////	 unsigned char Sn_TCP_DPort[2] ;	//tcpͨ��Ŀ��˿ں�
//	 unsigned char dns[4];																							   /*DNS��������ַ*/
////   unsigned char rip[4];																						    	/*remote IPԶ��IP��ַ*/
//	 unsigned char SoftWare_Version[2];																					/*����汾��*/
//	 
//	 unsigned short fw_len;
//	 unsigned  char state;
//	 unsigned char  debug;//���ڵ���
//	

//}NetParameter_Config;


