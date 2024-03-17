#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H

#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "SPI_W5500.h"
#include "32DIDO_PROTOCOL.h"
#include "define.h"
#include "AD5676.h"
#include "8AIAO_PROTOCOL.h"
#include "Net_Parameter_Setting.h"
#include "iwdg.h"
#include "DIDOAIAO.h"
#include "LOT.h"
#include "fattester.h"	 
extern unsigned char Connect_OK;
extern unsigned char Send_IP_Label;
extern unsigned short  udp_port;/*定义UDP的一个端口并初始化*/
extern void do_udp(void);
#endif 


