#ifndef __TCP_DEMO_H
#define __TCP_DEMO_H
#include "types.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "SPI_W5500.h"
#include "define.h"
#include "Flash.h"


void do_tcp_server_for_firmwareupadate(void);
void do_tcp_client(void);//TCP Clinet回环演示函数
#endif 

