#ifndef _W5500_H
#define _W5500_H

#include "sys.h"
#include "types.h"
#include "SPI_W5500.h"


#define __GNUC__

typedef  void (*pFunction)(void);

//typedef  void (*pFunction)(void);

//extern uint8    remote_ip[4];                            	  /* Զ��IP��ַ                   */
//extern unsigned short   remote_port;                            	  /* Զ�˶˿ں�                   */
//extern unsigned short   local_port;                             	  /* ���屾�ض˿�                 */
extern uint8    use_dhcp;                              	    /* �Ƿ�ʹ��DHCP��ȡIP           */
extern uint8    use_eeprom;                             	  /* �Ƿ�ʹ��EEPROM�е�IP������Ϣ */

extern unsigned short udp_local_port;	       //UDP socket                					/*���屾�ض˿�*/
extern unsigned short udp_remote_port;				 	//UDP socket											/*Զ�˶˿ں�*/
extern unsigned short tcp_local_port ;     //TCP socket 
extern unsigned short tcp_remote_port ;    //TCP socket 


#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*����ÿ�����ݰ��Ĵ�С*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*ʹ�ó�ʼ�����IP��Ϣ*/
#define IP_FROM_DHCP	          1       			              /*ʹ��DHCP��ȡIP��Ϣ*/
#define IP_FROM_EEPROM	        2       			              /*ʹ��EEPROM�����IP��Ϣ*/
extern uint8	ip_from;											                /*ѡ��IP��Ϣ����Դ*/
#pragma pack(1)


/*W5500��������ṹ��*/

///*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
//typedef struct _W5500_NetParameter_CONFIG											
//{
//   unsigned char MAC_Addr[6];																							/*MAC��ַ*/
//   unsigned char LOCAL_IP_Addr[4];																			/*local IP����IP��ַ*/
////   unsigned char REMOTE_IP_Addr[4];                                     /*Ŀ��IP��ַ*/
//	 unsigned char Sub_Mask[4];																							/*��������*/
//   unsigned char Gateway_IP[4];																							/*����*/	
//   unsigned char REMOTE_Sn_DIP[4];	                                            //�˿�Ŀ��IP��ַ 
//   
//	 unsigned short Sn_UDP_Port;
//   unsigned short Sn_UDP_DPort;
//   unsigned short Sn_TCP_Port;
//   unsigned short Sn_TCP_DPort;

//	 unsigned char dns[4];																							   /*DNS��������ַ*/
//	 unsigned char SoftWare_Version[2];																					/*����汾��*/
//	 
//	 unsigned short fw_len;
//	 unsigned  char state;
//	 unsigned char  debug;//���ڵ���
//	

//}W5500_NetParameter_Config;



//extern W5500_NetParameter_Config W5500_Parameter_Configuration;
extern void gpio_for_w5500_config(void); //W5500��STM32����
extern void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);
extern uint8 IINCHIP_READ(uint32 addrbsb);
extern unsigned short wiz_write_buf(uint32 addrbsb,uint8* buf,unsigned short len);
extern unsigned short wiz_read_buf(uint32 addrbsb, uint8* buf,unsigned short len);

extern void reset_w5500(void);
extern void set_w5500_ip(void);
extern void set_w5500_mac(void);


























/*����ĺ���*/
extern void STM32_W5500_Pins_Init(void);




#endif
