#include "define.h"

unsigned char Tx_Buffer[2048]; // ����һ��2k�ķ��ͻ�����
unsigned char Rx_Buffer[2048];  //����һ��2K�Ľ��ջ�����
unsigned char Connect_OK = 0;
unsigned char reboot_flag = 0;
NetParameter_Config W5500_Parameter_Configuration;
unsigned char Replay = 0;
unsigned char Analog_Data_Ouput_Old[8] = {0x00,};
unsigned char APP_Running_Flash = 0x00;
unsigned int SendandReceiveTimes=0x00;
unsigned char BUFFER_SAVE[26];//�������ɼ���ģ�����ɼ����26���ֽ�
unsigned char Digital_Analog_Inputtimes =0x00;//��¼��������ģ�����ɼ����ʹ��� 
unsigned char Digital_Analog_Outputtimes =0x00;//��¼��������ģ��������������
/***************************************************************
* Name��GetXor
* Function�����У��
* Author: zheng yuyong
* Date:   2010-11-10
****************************************************************/
unsigned char GetXor(volatile unsigned char * buff,unsigned char Length)
{
	unsigned char XorTemp=*buff++; 
	while(Length--) 
		XorTemp ^= *buff++;
	XorTemp &= 0x0000000FF;
	return XorTemp; 
}
//unsigned char  I2C_BUFF[37];
//unsigned short udp_local_port=0;	       //UDP socket                					/*���屾�ض˿�*/
//unsigned short udp_remote_port=0;				 	//UDP socket											/*Զ�˶˿ں�*/
//unsigned short tcp_local_port = 0;     //TCP socket 
//unsigned short tcp_remote_port = 0;    //TCP socket 
unsigned int app_running_address = 0;

//����������״̬��ȡ��־
//unsigned char  ReadDataInput = FALSE; //���ΪTRUE ��������������״̬
unsigned char  DataSent = FALSE;  //���ΪTRUE ���ʾ�����Ѿ�������ɣ����Խ�������������ɼ���
unsigned char  DataReceived = FALSE;//���ΪTRUE ���ʾ�����Ѿ�������ɣ����Խ����������ɼ���
unsigned char Software_Version[2];
unsigned char firstsendDIAI=0X00;//��һ�η����������ɼ���ģ�����ɼ� �������Ϸ�

