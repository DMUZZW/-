#include "define.h"

unsigned char Tx_Buffer[2048]; // 定义一个2k的发送缓存区
unsigned char Rx_Buffer[2048];  //定义一个2K的接收缓存区
unsigned char Connect_OK = 0;
unsigned char reboot_flag = 0;
NetParameter_Config W5500_Parameter_Configuration;
unsigned char Replay = 0;
unsigned char Analog_Data_Ouput_Old[8] = {0x00,};
unsigned char APP_Running_Flash = 0x00;
unsigned int SendandReceiveTimes=0x00;
unsigned char BUFFER_SAVE[26];//数字量采集和模拟量采集最多26个字节
unsigned char Digital_Analog_Inputtimes =0x00;//记录数字量和模拟量采集发送次数 
unsigned char Digital_Analog_Outputtimes =0x00;//记录数字量和模拟量输出处理次数
/***************************************************************
* Name：GetXor
* Function：异或校验
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
//unsigned short udp_local_port=0;	       //UDP socket                					/*定义本地端口*/
//unsigned short udp_remote_port=0;				 	//UDP socket											/*远端端口号*/
//unsigned short tcp_local_port = 0;     //TCP socket 
//unsigned short tcp_remote_port = 0;    //TCP socket 
unsigned int app_running_address = 0;

//数字量输入状态读取标志
//unsigned char  ReadDataInput = FALSE; //如果为TRUE 则发送数字量输入状态
unsigned char  DataSent = FALSE;  //如果为TRUE 则表示数据已经发送完成，可以进行数字量输入采集了
unsigned char  DataReceived = FALSE;//如果为TRUE 则表示数据已经接收完成，可以进行数字量采集了
unsigned char Software_Version[2];
unsigned char firstsendDIAI=0X00;//第一次发送数字量采集和模拟量采集 不让往上发

