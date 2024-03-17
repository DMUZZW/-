#ifndef _W5500_H
#define _W5500_H

#include "sys.h"
#include "types.h"
#include "SPI_W5500.h"


#define __GNUC__

typedef  void (*pFunction)(void);

//typedef  void (*pFunction)(void);

//extern uint8    remote_ip[4];                            	  /* 远端IP地址                   */
//extern unsigned short   remote_port;                            	  /* 远端端口号                   */
//extern unsigned short   local_port;                             	  /* 定义本地端口                 */
extern uint8    use_dhcp;                              	    /* 是否使用DHCP获取IP           */
extern uint8    use_eeprom;                             	  /* 是否使用EEPROM中的IP配置信息 */

extern unsigned short udp_local_port;	       //UDP socket                					/*定义本地端口*/
extern unsigned short udp_remote_port;				 	//UDP socket											/*远端端口号*/
extern unsigned short tcp_local_port ;     //TCP socket 
extern unsigned short tcp_remote_port ;    //TCP socket 


#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*使用初始定义的IP信息*/
#define IP_FROM_DHCP	          1       			              /*使用DHCP获取IP信息*/
#define IP_FROM_EEPROM	        2       			              /*使用EEPROM定义的IP信息*/
extern uint8	ip_from;											                /*选择IP信息配置源*/
#pragma pack(1)


/*W5500网络参数结构体*/

///*此结构体定义了W5500可供配置的主要参数*/
//typedef struct _W5500_NetParameter_CONFIG											
//{
//   unsigned char MAC_Addr[6];																							/*MAC地址*/
//   unsigned char LOCAL_IP_Addr[4];																			/*local IP本地IP地址*/
////   unsigned char REMOTE_IP_Addr[4];                                     /*目的IP地址*/
//	 unsigned char Sub_Mask[4];																							/*子网掩码*/
//   unsigned char Gateway_IP[4];																							/*网关*/	
//   unsigned char REMOTE_Sn_DIP[4];	                                            //端口目的IP地址 
//   
//	 unsigned short Sn_UDP_Port;
//   unsigned short Sn_UDP_DPort;
//   unsigned short Sn_TCP_Port;
//   unsigned short Sn_TCP_DPort;

//	 unsigned char dns[4];																							   /*DNS服务器地址*/
//	 unsigned char SoftWare_Version[2];																					/*软件版本号*/
//	 
//	 unsigned short fw_len;
//	 unsigned  char state;
//	 unsigned char  debug;//用于调试
//	

//}W5500_NetParameter_Config;



//extern W5500_NetParameter_Config W5500_Parameter_Configuration;
extern void gpio_for_w5500_config(void); //W5500与STM32引脚
extern void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);
extern uint8 IINCHIP_READ(uint32 addrbsb);
extern unsigned short wiz_write_buf(uint32 addrbsb,uint8* buf,unsigned short len);
extern unsigned short wiz_read_buf(uint32 addrbsb, uint8* buf,unsigned short len);

extern void reset_w5500(void);
extern void set_w5500_ip(void);
extern void set_w5500_mac(void);


























/*定义的函数*/
extern void STM32_W5500_Pins_Init(void);




#endif
