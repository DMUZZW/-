#ifndef	__HTTPUTIL_H__
#define	__HTTPUTIL_H__

#include <stdio.h>
#include <string.h>
#include "w5500.h"
#include "W5500_conf.h"
#include "utility.h"
//#include "bsp_spi_flash.h"
//#include "bsp_usart1.h"
#include "http_server.h"
#include "socket.h"
#include "Flash.h"
//#include "24cxx.h"
#include "24cxx.h"
#include "define.h"

void proc_http(SOCKET s, uint8 * buf,uint16 len,uint32 ADDRESS);
void do_https(uint32 ADDRESS);
void cgi_ipconfig(st_http_request *http_request);
//void trimp(uint8* src, uint8* dst, uint16 len);
uint16 make_msg_response(uint8* buf,int8* msg);

void make_cgi_response(uint16 a,int8* b,int8* c);
void make_pwd_response(int8 isRight,uint16 delay,int8* cgi_response_content, int8 isTimeout);
#endif


