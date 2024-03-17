#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "sys.h"
#include "delay.h"

#define TRUE 1
#define FALSE 0

#define NORMAL_STATE            0
#define NEW_APP_IN_BACK         1 //there is new app in back address
//#define CONFIGTOOL_FW_UP        2 //configtool update f/w in app
/*本程序给BOOTLOAD的FLASH空间为128k，所以偏移量为0x20000*/
#define APP_ADDRESS		0x08020000  	//存放APP运行代码的区域
#define BACKUP_ADDRESS		0x08080000   //备份代码区域


//#ifndef AnalogDataStart   //模拟量数据输出起始位 在协议中 第一个模拟量输出数据是在第11个 也就是Rx_Buffer[11] 含8个UDP报头
//#define AnalogDataStart 0x04
//#endif





extern unsigned int SendandReceiveTimes;//该数据用于记录数据收发的序号
extern unsigned char Tx_Buffer[2048]; // 定义一个4k的发送缓存区
extern unsigned char Rx_Buffer[2048];  //定义一个4K的接收缓存区
extern unsigned char GetXor(volatile unsigned char * buff,unsigned char Length);

/***************----- 网络参数变量定义 -----***************/

/*此结构体定义了W5500可供配置的主要参数*/
typedef struct W5500_NetParameter_Config											
{
   unsigned char op[4]; //op用于选择操作模式，上位机发送的是 FIND SETT和 FIRM
   unsigned char MAC_Addr[6];	/*MAC地址*/
   unsigned char LOCAL_IP_Addr[4];/*local IP本地IP地址*/
   unsigned char Sub_Mask[4];	/*子网掩码*/
   unsigned char Gateway_IP[4];	/*网关*/	
   unsigned char REMOTE_Sn_DIP[4];	/*remote IP远程IP地址*/
   
   unsigned short Sn_UDP_Port;
   unsigned short Sn_UDP_DPort;
   unsigned short Sn_TCP_Port;
   unsigned short Sn_TCP_DPort;
   unsigned char dns[4]; /*DNS服务器地址*/
   unsigned char SoftWare_Version[2];	/*软件版本号*/																				
	 
   unsigned char FirmwareLength[4];//接收到的固件的数据长度
   unsigned char  debug;//用于调试
   unsigned char state;
	
	

}NetParameter_Config;


extern NetParameter_Config W5500_Parameter_Configuration ; //定义W55500网络配置结构体

extern unsigned char reboot_flag;
extern unsigned char Replay;

//extern unsigned char   I2C_BUFF[37];
//extern unsigned short udp_local_port;	       //UDP socket                					/*定义本地端口*/
//extern unsigned short udp_remote_port;				 	//UDP socket											/*远端端口号*/
//extern unsigned short tcp_local_port;     //TCP socket 
//extern unsigned short tcp_remote_port;    //TCP socket 
extern unsigned int app_running_address; //用于标记APP烧录和IAP跳转的区域
extern unsigned char Analog_Data_Ouput_Old[8];
extern unsigned char APP_Running_Flash;

//extern unsigned char  ReadDataInput ; //如果为TRUE 则发送数字量输入状态
extern unsigned char  DataSent ;  //如果为FALSH 则表示数据已经发送完成，可以进行数字量输入采集了
extern unsigned char  DataReceived ;//如果为FALSH 则表示数据已经接收完成，可以进行数字量采集了
extern unsigned char BUFFER_SAVE[26];//数字量采集和模拟量采集最多26个字节
extern unsigned char Connect_OK;
extern unsigned char Software_Version[2];
extern unsigned char firstsendDIAI;
extern unsigned char Digital_Analog_Inputtimes ;//记录数字量和模拟量采集发送次数 
extern unsigned char Digital_Analog_Outputtimes ;//记录数字量和模拟量输出处理次数
















#endif

