#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "sys.h"
#include "delay.h"

#define TRUE 1
#define FALSE 0

#define NORMAL_STATE            0
#define NEW_APP_IN_BACK         1 //there is new app in back address
//#define CONFIGTOOL_FW_UP        2 //configtool update f/w in app
/*�������BOOTLOAD��FLASH�ռ�Ϊ128k������ƫ����Ϊ0x20000*/
#define APP_ADDRESS		0x08020000  	//���APP���д��������
#define BACKUP_ADDRESS		0x08080000   //���ݴ�������


//#ifndef AnalogDataStart   //ģ�������������ʼλ ��Э���� ��һ��ģ��������������ڵ�11�� Ҳ����Rx_Buffer[11] ��8��UDP��ͷ
//#define AnalogDataStart 0x04
//#endif





extern unsigned int SendandReceiveTimes;//���������ڼ�¼�����շ������
extern unsigned char Tx_Buffer[2048]; // ����һ��4k�ķ��ͻ�����
extern unsigned char Rx_Buffer[2048];  //����һ��4K�Ľ��ջ�����
extern unsigned char GetXor(volatile unsigned char * buff,unsigned char Length);

/***************----- ��������������� -----***************/

/*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
typedef struct W5500_NetParameter_Config											
{
   unsigned char op[4]; //op����ѡ�����ģʽ����λ�����͵��� FIND SETT�� FIRM
   unsigned char MAC_Addr[6];	/*MAC��ַ*/
   unsigned char LOCAL_IP_Addr[4];/*local IP����IP��ַ*/
   unsigned char Sub_Mask[4];	/*��������*/
   unsigned char Gateway_IP[4];	/*����*/	
   unsigned char REMOTE_Sn_DIP[4];	/*remote IPԶ��IP��ַ*/
   
   unsigned short Sn_UDP_Port;
   unsigned short Sn_UDP_DPort;
   unsigned short Sn_TCP_Port;
   unsigned short Sn_TCP_DPort;
   unsigned char dns[4]; /*DNS��������ַ*/
   unsigned char SoftWare_Version[2];	/*����汾��*/																				
	 
   unsigned char FirmwareLength[4];//���յ��Ĺ̼������ݳ���
   unsigned char  debug;//���ڵ���
   unsigned char state;
	
	

}NetParameter_Config;


extern NetParameter_Config W5500_Parameter_Configuration ; //����W55500�������ýṹ��

extern unsigned char reboot_flag;
extern unsigned char Replay;

//extern unsigned char   I2C_BUFF[37];
//extern unsigned short udp_local_port;	       //UDP socket                					/*���屾�ض˿�*/
//extern unsigned short udp_remote_port;				 	//UDP socket											/*Զ�˶˿ں�*/
//extern unsigned short tcp_local_port;     //TCP socket 
//extern unsigned short tcp_remote_port;    //TCP socket 
extern unsigned int app_running_address; //���ڱ��APP��¼��IAP��ת������
extern unsigned char Analog_Data_Ouput_Old[8];
extern unsigned char APP_Running_Flash;

//extern unsigned char  ReadDataInput ; //���ΪTRUE ��������������״̬
extern unsigned char  DataSent ;  //���ΪFALSH ���ʾ�����Ѿ�������ɣ����Խ�������������ɼ���
extern unsigned char  DataReceived ;//���ΪFALSH ���ʾ�����Ѿ�������ɣ����Խ����������ɼ���
extern unsigned char BUFFER_SAVE[26];//�������ɼ���ģ�����ɼ����26���ֽ�
extern unsigned char Connect_OK;
extern unsigned char Software_Version[2];
extern unsigned char firstsendDIAI;
extern unsigned char Digital_Analog_Inputtimes ;//��¼��������ģ�����ɼ����ʹ��� 
extern unsigned char Digital_Analog_Outputtimes ;//��¼��������ģ��������������
















#endif

