#ifndef	_SOCKET_H_
#define	_SOCKET_H_

#include "sys.h"
#include "types.h"
#include "w5500_conf.h"
#include "stdio.h"
#include "w5500.h"
#include "SPI_W5500.h"

/*Socket 端口选择，可按自己的习惯定义*/
#define SOCK_TCPS             0
#define SOCK_HUMTEM			      0
#define SOCK_PING			        0
#define SOCK_TCPC             1
#define SOCK_UDPS             2
#define SOCK_WEIBO      	    2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_SMTP             6
#define SOCK_NTP              7
//#define NETBIOS_SOCK    6 //在netbios.c已定义

extern uint8 socket(SOCKET s, uint8 protocol, unsigned short port, uint8 flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void close(SOCKET s); // Close socket
extern uint8 connect(SOCKET s, uint8 * addr, unsigned short port); // Establish TCP connection (Active connection)
extern void disconnect(SOCKET s); // disconnect the connection
extern uint8 listen(SOCKET s);	// Establish TCP connection (Passive connection)
extern unsigned short send(SOCKET s, const uint8 * buf, unsigned short len); // Send data (TCP)
extern unsigned short recv(SOCKET s, uint8 * buf, unsigned short len);	// Receive data (TCP)
extern unsigned short sendto(SOCKET s, const uint8 * buf, unsigned short len, uint8 * addr, unsigned short port); // Send data (UDP/IP RAW)
extern unsigned short recvfrom(SOCKET s, uint8 * buf, unsigned short len, uint8 * addr, unsigned short  *port); // Receive data (UDP/IP RAW)

#ifdef __MACRAW__
void macraw_open(void);
unsigned short macraw_send( const uint8 * buf, unsigned short len ); //Send data (MACRAW)
unsigned short macraw_recv( uint8 * buf, unsigned short len ); //Recv data (MACRAW)
#endif

#endif
/* _SOCKET_H_ */

