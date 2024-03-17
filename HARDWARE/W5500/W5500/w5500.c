/**
******************************************************************************
���������������������W5500��IP��ַ���������롢���ص�ַ��MAC��ַ����Щ��������ͨ���б���Ҫ���õĲ�����
���ݴ��䣺�ṩ�˷��ͺͽ������ݵĺ�����ʹ��W5500�����������豸�������ݽ�����
���ӹ������Կ���W5500���������ӣ������������˿ڣ����ܿͻ��˵����ӣ�������Ϊ�ͻ������ӵ���������
״̬��أ����Լ��W5500�Ĺ���״̬�������Ƿ��յ��µ����ݣ��Ƿ�ɹ��������ӵȡ�

* @file    		W5500.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		��дW5500�Ĵ����ײ㺯��
******************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "RNG.h"
#include "w5500_conf.h"
#ifdef __DEF_IINCHIP_PPP__
#include "md5.h"
#endif

unsigned short SSIZE[8]={0,0,0,0,0,0,0,0}; // Max Tx buffer
unsigned short RSIZE[8]={0,0,0,0,0,0,0,0}; // Max Rx buffer
uint8 txsize[8] = {2,2,2,2,2,2,2,2};//tx buffer set	K bits
uint8 rxsize[8] = {2,2,2,2,2,2,2,2};//rx buffet set  K bits

/**
*@brief		This function is to get the Max size to receive.
*@param		s: socket number
*@return	This function return the RX buffer size has been setted
*/
void iinchip_init(void)
{
  setMR( MR_RST );
#ifdef __DEF_IINCHIP_DBG__
  printf("MR value is %02x \r\n",IINCHIP_READ_COMMON(MR));
#endif
}

/**
*@brief		This function is to get the Max size to receive.
*@param		s: socket number
*@return	This function return the RX buffer size has been setted
*/
unsigned short getIINCHIP_RxMAX(SOCKET s)
{
	return RSIZE[s];
}	

/**
*@brief		This function is to get the Max size to receive.
*@param		s: socket number
*@return	This function return the RX buffer size has been setted
*/
unsigned short getIINCHIP_TxMAX(SOCKET s)
{
	return SSIZE[s];
}

/**
*@brief		This function is to set up gateway IP address.
*@param		addr: a pointer to a 4 -byte array responsible to set the Gateway IP address 
*@return	None
*/
void setGAR(uint8 * addr )
{
    wiz_write_buf(GAR0, addr, 4);
}

/**
*@brief		This function is to get gateway IP address.
*@param		addr: a pointer to a 4 -byte array responsible to get the Gateway IP address  
*@return	None
*/
void getGAR(uint8 * addr)
{
    wiz_read_buf(GAR0, addr, 4);
}

/**
*@brief 	This function is to set up SubnetMask address
*@param		addr: a pointer to a 4 -byte array responsible to set the subway IP address.  
*@return	None
*/
void setSUBR(uint8 * addr)
{   
    wiz_write_buf(SUBR0, addr, 4);
}
/**
*@brief		This function is to set up MAC address.
*@param		addr: a pointer to a 6 -byte array responsible to set the MAC address.  
*@return	None
*/
void setSHAR(uint8 * addr)
{
  wiz_write_buf(SHAR0, addr, 6);  
}

/**
*@brief		This function is to set up Source IP address.
*@param		addr:a pointer to a 4 -byte array responsible to set the Source IP addres.  
*@return	None
*/
void setSIPR(uint8 * addr)
{
    wiz_write_buf(SIPR0, addr, 4);  
}

/**
*@brief		This function is to get Subnet mask.
*@param		addr:a pointer to a 4 -byte array responsible to set the Subnet mask.  
*@return	None
*/
void getSUBR(uint8 * addr)
{
    wiz_read_buf(SUBR0, addr, 4);
}

/**
*@brief		This function is to get up Source MAC .
*@param		addr: a pointer to a 6 -byte array responsible to get the MAC  
*@return	None
*/
void getSHAR(uint8 * addr)
{
    wiz_read_buf(SHAR0, addr, 6);
}

/**
*@brief		This function is to get up Source IP .
*@param		addr: a pointer to a 4 -byte array responsible to get the Source IP  
*@return	None
*/
void getSIPR(uint8 * addr)
{
    wiz_read_buf(SIPR0, addr, 4);
}
/**
*@brief		This function is to set the MR register.
*@param		val: the value to set to MR  
*@return	None
*/
void setMR(uint8 val)
{
  IINCHIP_WRITE(MR,val);
}

/**
*@brief		This function is to get Interrupt register in common register.
*@param		None  
*@return	The value read from the IR register
*/
uint8 getIR( void )
{
   return IINCHIP_READ(IR);
}

/**
@brief		This function is to set up Retransmission time.
					If there is no response from the peer or delay in response then retransmission
					will be there as per RTR (Retry Time-value Register)setting
*@param		timeout: The value write to  the RTR0 register 
*@return	None
*/
void setRTR(unsigned short timeout)
{
  IINCHIP_WRITE(RTR0,(uint8)((timeout & 0xff00) >> 8));
  IINCHIP_WRITE(RTR1,(uint8)(timeout & 0x00ff));
}

/**
@brief		This function is to set the number of Retransmission.
					If there is no response from the peer or delay in response then recorded time
					as per RTR & RCR register seeting then time out will occur.
*@param		retry: Times to  retry 
*@return	None
*/
void setRCR(uint8 retry)
{
  IINCHIP_WRITE(WIZ_RCR,retry);
}

/**
*@brief		This function is to the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)
					If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
					set in IR register.
*@param		mask: the bits to clear
*@return	None
*/
void clearIR(uint8 mask)
{
  IINCHIP_WRITE(IR, ~mask | getIR() ); 
}

/**
*@brief  	This function is to set the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*@param		s: socket number
*@param		Sn_MSSR: the maximum segment size
*@return	None
*/
void setSn_MSS(SOCKET s, unsigned short Sn_MSSR)
{
  IINCHIP_WRITE( Sn_MSSR0(s), (uint8)((Sn_MSSR & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_MSSR1(s), (uint8)(Sn_MSSR & 0x00ff));
}

/**
*@brief  	This function is to set the source port register
*@param		s: socket number
*@param		Sn_PORT0(ch) Sn_PORT1(ch) 
*@return	None
*/
void setSn_PORT(SOCKET s,unsigned short Sn_port)
{
	IINCHIP_WRITE(Sn_PORT0(s),(unsigned char)((Sn_port &0xff00)>>8));
	IINCHIP_WRITE(Sn_PORT1(s),(unsigned char)(Sn_port & 0x00ff));
}


/**
*@brief  	This function is to set the IP Time to live(TTL) Register
*@param		s: socket number
*@param		Sn_MSSR: the IP Time to live
*@return	None
*/
void setSn_TTL(SOCKET s, uint8 ttl)
{    
   IINCHIP_WRITE( Sn_TTL(s) , ttl);
}

/**
*@brief		This function is to read the Interrupt & Soket Status registe
*@param		s: socket number
*@return	socket interrupt status
*/
uint8 getSn_IR(SOCKET s)
{
   return IINCHIP_READ(Sn_IR(s));
}

/**
*@brief 	This function is to write the Interrupt & Soket Status register to clear the interrupt
*@param		s: socket number
*@return  socket interrupt status
*/
void setSn_IR(uint8 s, uint8 val)
{
    IINCHIP_WRITE(Sn_IR(s), val);
}

/**
*@brief 	This function is to get socket status
*@param		s: socket number
*@return  socket status
*/
uint8 getSn_SR(SOCKET s)
{
   return IINCHIP_READ(Sn_SR(s));
}

/**
*@brief		This fuction is to get socket TX free buf size
					This gives free buffer size of transmit buffer. This is the data size that user can transmit.
					User shuold check this value first and control the size of transmitting data
*@param		s: socket number
*@return  socket TX free buf size
*/
unsigned short getSn_TX_FSR(SOCKET s)
{
  unsigned short val=0,val1=0;
  do
  {
    val1 = IINCHIP_READ(Sn_TX_FSR0(s));
    val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
      if (val1 != 0)
    {
        val = IINCHIP_READ(Sn_TX_FSR0(s));
        val = (val << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
    }
  } while (val != val1);
   return val;
}

/**
*@brief		This fuction is to give size of received data in receive buffer.
*@param		s: socket number
*@return  socket TX free buf size
*/
unsigned short getSn_RX_RSR(SOCKET s)
{
  unsigned short val=0,val1=0;
  do
  {
    val1 = IINCHIP_READ(Sn_RX_RSR0(s));
    val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR1(s));
    if(val1 != 0)
    {
        val = IINCHIP_READ(Sn_RX_RSR0(s));
        val = (val << 8) + IINCHIP_READ(Sn_RX_RSR1(s));
    }
  } while (val != val1);
   return val;
}

/**
*@brief   This function is being called by send() and sendto() function also.

					This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
					register. User should read upper byte first and lower byte later to get proper value.
*@param		s: socket number
*@param		data: data buffer to send
*@param		len: data length
*@return  socket TX free buf size
*/
void send_data_processing(SOCKET s, uint8 *data, unsigned short len)
{
  unsigned short ptr =0;
  uint32 addrbsb =0;
  if(len == 0)
  {
    printf("CH: %d Unexpected1 length 0\r\n", s);
    return;
  }
   
  ptr = IINCHIP_READ( Sn_TX_WR0(s) );
  ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR1(s));

  addrbsb = (uint32)(ptr<<8) + (s<<5) + 0x10;
  wiz_write_buf(addrbsb, data, len);
  
  ptr += len;
  IINCHIP_WRITE( Sn_TX_WR0(s) ,(uint8)((ptr & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_TX_WR1(s),(uint8)(ptr & 0x00ff));
}

/**
*@brief  	This function is being called by recv() also.
					This function read the Rx read pointer register
					and after copy the data from receive buffer update the Rx write pointer register.
					User should read upper byte first and lower byte later to get proper value.
*@param		s: socket number
*@param		data: data buffer to receive
*@param		len: data length
*@return  None
*/
void recv_data_processing(SOCKET s, uint8 *data, unsigned short len)
{
  unsigned short ptr = 0;
  uint32 addrbsb = 0;
  
  if(len == 0)
  {
    printf("CH: %d Unexpected2 length 0\r\n", s);
    return;
  }

  ptr = IINCHIP_READ( Sn_RX_RD0(s) );
  ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ( Sn_RX_RD1(s) );

  addrbsb = (uint32)(ptr<<8) + (s<<5) + 0x18;
  wiz_read_buf(addrbsb, data, len);
  ptr += len;

  IINCHIP_WRITE( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
}

/**
*@brief		This function set the transmit & receive buffer size as per the channels is used
*@Note: 	TMSR and RMSR bits are as follows\n
					Maximum memory size for Tx, Rx in the W5500 is 16K Bytes,\n
					In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
					Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
					and to data transmission and receiption from non-allocated channel may cause some problems.\n
					If the 16KBytes memory is already  assigned to centain channel, \n
					other 3 channels couldn't be used, for there's no available memory.\n
					If two 4KBytes memory are assigned to two each channels, \n
					other 2 channels couldn't be used, for there's no available memory.\n
*@param		tx_size: tx buffer size to set=tx_size[s]*(1024)
*@param		rx_size: rx buffer size to set=rx_size[s]*(1024)
*@return	None
*/
void socket_buf_init( uint8 * tx_size, uint8 * rx_size  )
{
  int16 i;
  int16 ssum=0,rsum=0;
 
  for (i = 0 ; i < 8; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
  {
          IINCHIP_WRITE( (Sn_TXMEM_SIZE(i)), tx_size[i]);
          IINCHIP_WRITE( (Sn_RXMEM_SIZE(i)), rx_size[i]);
          
#ifdef __DEF_IINCHIP_DBG__
         printf("tx_size[%d]: %d, Sn_TXMEM_SIZE = %d\r\n",i, tx_size[i], IINCHIP_READ(Sn_TXMEM_SIZE(i)));
         printf("rx_size[%d]: %d, Sn_RXMEM_SIZE = %d\r\n",i, rx_size[i], IINCHIP_READ(Sn_RXMEM_SIZE(i)));
#endif
    SSIZE[i] = (int16)(0);
    RSIZE[i] = (int16)(0);
	
    if (ssum <= 16384)
    {
			SSIZE[i] = (int16)tx_size[i]*(1024);
    }

		if (rsum <= 16384)
		{
			RSIZE[i]=(int16)rx_size[i]*(1024);
    }
			ssum += SSIZE[i];
			rsum += RSIZE[i];

  }
}


/*��MCU��ΨһID ��ΪW5500��MAC��ַ*/
void SET_W5500_MAC(void)
{

	unsigned char ID0=0,ID1=0,ID2=0,ID3=0,ID4=0,ID5=0;
	//����6��0-255��Χ�ڵ�Ӳ�������
	if(RNG_Init()==0)
	{
		//RNG_Get_RandomNum()%(max-min+1)+min;
			ID0= (unsigned char)RNG_Get_RandomRange(0,255);
		  if(ID0%2 ==1){ID0 = ID0+1;};
		  delay_ms(50);
      ID1= (unsigned char)RNG_Get_RandomRange(0,255);
		  delay_ms(50);
	    ID2= (unsigned char)RNG_Get_RandomRange(0,255);
		  delay_ms(50);
	    ID3= (unsigned char)RNG_Get_RandomRange(0,255);
		  delay_ms(50);
	    ID4= (unsigned char)RNG_Get_RandomRange(0,255);
		  delay_ms(50);
	    ID5= (unsigned char)RNG_Get_RandomRange(0,255);
	}
	else
	{
			ID0= 0x00;
      ID1= 0x11;
	    ID2= 0x22;
	    ID3= 0x33;
	    ID4= 0x44;
	    ID5= 0x55;
		
	}
	W5500_Parameter_Configuration.MAC_Addr[0] = ID0;
	W5500_Parameter_Configuration.MAC_Addr[1] = ID1;
	W5500_Parameter_Configuration.MAC_Addr[2] = ID2;
	W5500_Parameter_Configuration.MAC_Addr[3] = ID3;
	W5500_Parameter_Configuration.MAC_Addr[4] = ID4;
	W5500_Parameter_Configuration.MAC_Addr[5] = ID5;

	setSHAR(W5500_Parameter_Configuration.MAC_Addr);
}

//��ȡ�������ӼĴ���
unsigned char getPHYStatus(void)
{
	return IINCHIP_READ(PHYCFGR);
}

/*�����̫�����Ƿ����Ӵ���*/
unsigned char PHY_Check(void)
{
	unsigned char PHY_connect = 0;//�Ƿ����ӵ�flag
	PHY_connect = 0x01&getPHYStatus();
	if(PHY_connect ==0x00)//��̫����û������
	{
//		delay_ms(500);
		while(PHY_connect==0)
		{
			delay_ms(500);
			return 0;//����û�����ӷ���1
			
		}		
	}
	else
	
		{return 1;}//�������ӷ���0	
}




/**
*@brief		This function is to set up destination IP address.
*@param		addr:a pointer to a 4 -byte array responsible to set the Source IP addres.  
*@return	None
���ö˿ڵ�Ŀ��IP��ַ
*/
void setDIPR(SOCKET s,unsigned char* buff)
{
   IINCHIP_WRITE( Sn_DIPR0(s), buff[0]);
   IINCHIP_WRITE( Sn_DIPR1(s), buff[1]);
   IINCHIP_WRITE( Sn_DIPR2(s), buff[2]);
   IINCHIP_WRITE( Sn_DIPR3(s), buff[3]);	
}
/*����Ŀ��˿ں�*/
void setDPORT(SOCKET s,unsigned short port)
{
	unsigned char Data[2] = {0x00,};
	Data[0] = (unsigned char)(port>>8);
	Data[1] = (unsigned char)(port);
	
	IINCHIP_WRITE( Sn_DPORT0(s), Data[0]); //д��˿ں�
	IINCHIP_WRITE( Sn_DPORT0(s), Data[1]);
}



