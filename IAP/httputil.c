/**
******************************************************************************
* @file    			httputil.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   	  	http����Ҫ��ʵ�ú���
******************************************************************************
*/
#include "httputil.h"
#include "webpge.h"
#include "utility.h"
#include "iwdg.h"
#include "ff.h"			/* Declarations of FatFs API */
#include "diskio.h"		/* Declarations of disk I/O functions */
#include "fattester.h"
#include "string.h"
#include "malloc.h"
#include "exfuns.h"
#define DEBUG_HTTP

//extern CONFIG_MSG  ConfigMsg;
extern char tx_buf[MAX_URI_SIZE];
extern char rx_buf[MAX_URI_SIZE];
extern uint8 reboot_flag;

uint8 boundary[64];
uint8 tmp_buf[1460]={0xff,};
extern uint8 pub_buf[1024];

/**
*@brief		��������������Ϣ���õ�json_callback
*@param		��
*@return	��
*/
static void make_basic_config_setting_json_callback(int8* buf, W5500_NetParameter_Config W5500_Parameter_Configuration)
{
  sprintf(buf,"settingsCallback({\"ver\":\"%d.%d\",\
                \"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\
                \"ip\":\"%d.%d.%d.%d\",\
                \"gw\":\"%d.%d.%d.%d\",\
                \"sub\":\"%d.%d.%d.%d\",\
                });",W5500_Parameter_Configuration.SoftWare_Version[0],W5500_Parameter_Configuration.SoftWare_Version[1],
                W5500_Parameter_Configuration.MAC_Addr[0],W5500_Parameter_Configuration.MAC_Addr[1],W5500_Parameter_Configuration.MAC_Addr[2],W5500_Parameter_Configuration.MAC_Addr[3],W5500_Parameter_Configuration.MAC_Addr[4],W5500_Parameter_Configuration.MAC_Addr[5],
                W5500_Parameter_Configuration.LOCAL_IP_Addr[0], W5500_Parameter_Configuration.LOCAL_IP_Addr[1], W5500_Parameter_Configuration.LOCAL_IP_Addr[2], W5500_Parameter_Configuration.LOCAL_IP_Addr[3],
                W5500_Parameter_Configuration.Gateway_IP[0], W5500_Parameter_Configuration.Gateway_IP[1], W5500_Parameter_Configuration.Gateway_IP[2], W5500_Parameter_Configuration.Gateway_IP[3],
                W5500_Parameter_Configuration.Sub_Mask[0],W5500_Parameter_Configuration.Sub_Mask[1],W5500_Parameter_Configuration.Sub_Mask[2],W5500_Parameter_Configuration.Sub_Mask[3]
                );
}

/**
*@brief		���http��Ӧ
*@param		��
*@return	��
*/
void do_https(uint32 ADDRESS)
{
	uint8 ch=SOCK_HTTPS;																		/*����һ��socket*/
	uint16 len;
	
	st_http_request *http_request;													/*����һ���ṹָ��*/
	memset(rx_buf,0x00,MAX_URI_SIZE);
	http_request = (st_http_request*)rx_buf;					 
	/* http service start */
	switch(getSn_SR(ch))																		/*��ȡsocket״̬*/
	{
		case SOCK_INIT:																				/*socket���ڳ�ʼ��״̬*/
			listen(ch);
			break;
		
		case SOCK_LISTEN:																			/*socket���ڼ���״̬*/
			break;
		
		case SOCK_ESTABLISHED:																/*socket��������״̬*/
			if(getSn_IR(ch) & Sn_IR_CON)
			{
				setSn_IR(ch, Sn_IR_CON);													/*����жϱ�־λ*/
			}
			if ((len = getSn_RX_RSR(ch)) > 0)		
			{
				len = recv(ch, (uint8*)http_request, len); 				/*����http����*/
				*(((uint8*)http_request)+len) = 0;
				proc_http(ch, (uint8*)http_request,len,ADDRESS);	/*����http���󲢷���http��Ӧ*/
				disconnect(ch);
				
			}
			break;
			
		case SOCK_CLOSE_WAIT:   															/*socket���ڵȴ��ر�״̬*/
			if ((len = getSn_RX_RSR(ch)) > 0)
			{
				len = recv(ch, (uint8*)http_request, len);				/*����http����*/      
				*(((uint8*)http_request)+len) = 0;
			proc_http(ch, (uint8*)http_request,len,ADDRESS);							/*����http���󲢷���http��Ӧ*/
			}
			disconnect(ch);
			break;
			
		case SOCK_CLOSED:                   									/*socket���ڹر�״̬*/
			socket(ch, Sn_MR_TCP, 80, 0x00);   									/*��socket*/
			break;
		
		default:
			break;
	}
}

/**
*@brief		����http�����Ĳ�����http��Ӧ
*@param		s: http������socket
*@param		buf��������������
*@return	��
*/
void proc_http(SOCKET s, uint8 * buf,uint16 len,uint32 ADDRESS)
{
	int8* name; 											
	int8 req_name[32]={0x00,};															/*����һ��http��Ӧ���ĵ�ָ��*/
	unsigned long file_len=0;																/*����http������ͷ�Ľṹ��ָ��*/
	uint16 send_len=0;
	uint8* http_response;
	
	uint16 tmp_len = 0;
	uint32 content_len = 0;
	uint32 rx_len = 0;
	uint32 upload_file_len = 0;
	uint16 fw_offset = 0;//��һ��http���ݰ��У��̼������ƫ�Ƶ�ַ the first packet fw data offset
	uint16 wr_len  = 0; //ʵ����Ҫд�뵽�̼����볤�� the real write length of recevied packet
	
	uint8 remain_buf[3]= {0xff,};//��Ϊflashһ��д��4���ֽڣ�������������������4��ʣ�µ�С�ڻ����3���ֽڵ�����
	uint8 remain_len = 0; //remain bytes length 
	uint16 i=0;
   u32 temp = 0;//�ݴ����
	
	int8 sub[10];
	
	int8* pos1; //ȷ���̼�����ʼ��ַ
	int8* pos2; //ȷ���̼�����ֹ��ַ
	int8* hdr_pos;  //���һ�����ݰ� ȥ�������ַ�
//	int8* post4;
	uint16 hdr_len=0; //�̼�����
	
	uint32 flash_dest = ADDRESS;//the address of backup flash 
	
	unsigned char APP_Running_Flash_Address_Flag = 0;
  char* filenames;
	char filename[2018] = {0};
	u8 *myfatbuf;
	int res = 0;
	int allsize = 0;
	char path[1024] = "0:";
	unsigned int SD_Total,SD_Free;
	
	
	char scanpath[1024] = "0:";
	
	st_http_request *http_request;
	memset(tx_buf,0x00,MAX_URI_SIZE);
	http_response = (uint8*)rx_buf;  //HTTP��Ӧ
	http_request = (st_http_request*)tx_buf; //HTTP��ȡ
	parse_http_request(http_request, buf);    							/*����http������ͷ*/
	
	switch (http_request->METHOD)	
	{
		case METHOD_ERR :	/*������ͷ����*/
				memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
			  send(s, (uint8 *)http_response, strlen((int8 const*)http_response));
			break;
		case METHOD_HEAD:																			/*HEAD����ʽ*/
			break;
		case METHOD_GET :	/*GET����ʽ*/
			name = http_request->URI;
			if(strcmp(name,"/index.htm")==0 || strcmp(name,"/")==0 || (strcmp(name,"/index.html")==0))
			{
				file_len = strlen(INDEX_HTML);
				make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
				send(s,http_response,strlen((char const*)http_response));
				send_len=0;
				while(file_len)
				{
					if(file_len>1024)
					{
						if(getSn_SR(s)!=SOCK_ESTABLISHED)
						{
							return;
						}
						send(s, (uint8 *)INDEX_HTML+send_len, 1024);
						send_len+=1024;
						file_len-=1024;
					}
					else
					{
						send(s, (uint8 *)INDEX_HTML+send_len, file_len);
						send_len+=file_len;
						file_len-=file_len;
					} 
				}
			}
			else if(strcmp(name,"/w5500.js")==0)
			{
				memset(tx_buf,0,MAX_URI_SIZE);
				make_basic_config_setting_json_callback(tx_buf,W5500_Parameter_Configuration);
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
				send(s, (u_char *)http_response, strlen((char const*)http_response));
			}
			//��ȡSD����С��ʣ��ռ�
			else if(strcmp(name,"/getfree")==0)
			{
				IWDG_Feed(); //ι��	
				exf_getfree("0:",&SD_Total,&SD_Free);//��ȡSD���Ĵ�С��ʣ��ռ�
				sprintf (tx_buf,"SD total size :%dMB, SD free size:%dMB",SD_Total,SD_Free);
				printf ("tx_buf = %s",tx_buf);
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);				
	      send(s, (u_char *)http_response, strlen((char const*)http_response));
				
			}						
			//�ļ�����ʾ
			else if(strstr(name,"/getallfile") != NULL)  //  
			{
				//ɨ��Ŀ¼
				strncpy(filename, name+12, strlen(name)-12);//��ȡ�ļ���
				strcat(scanpath,filename);//����·��
				printf("%s\r\n",scanpath);		
      	mf_scan((u8 *)scanpath,&filenames);
//				mf_scan("0:",&filenames);
				IWDG_Feed(); //ι��	
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(filenames),filenames);				
				send(s, (u_char *)http_response, strlen((char const*)http_response));
			}
			//�ļ�����
			else if(strstr(name, "/download/") != NULL)
			{	
				strncpy(filename, name+10, strlen(name)-10);//��ȡ�ļ���
				strcat((u8*)path,filename);//����·��
				mf_open((u8*)path, FA_READ);
				res = get_all(&myfatbuf);
				IWDG_Feed(); //ι��	
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-type: APPLICATION/OCTET-STREAM\r\nContent-Length:%d\r\n\r\n",mf_size());
				send(s, (u_char *)http_response, strlen((char const*)http_response));
				send(s, myfatbuf, res);
				allsize += res;
				while(1)
					{
					if(allsize == mf_size())
						break;
					IWDG_Feed(); //ι��	
					res = get_all(&myfatbuf);
					allsize += res;
					send(s, myfatbuf, res);
				 }
				mf_close();
			}
			
		 //�ļ�ɾ��
			else if(strstr(name, "/deletefile/")!= NULL)
			{
				strncpy(filename, name+11, strlen(name)-11);
				strcat(path,filename);
				printf("path=%s\r\n",path);
				res = mf_unlink((u8*)path);//ֻ��ɾ�������ļ�����ļ���
				IWDG_Feed(); //ι��	

				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(path),path);				
				send(s, (u_char *)http_response, strlen((char const*)http_response));
		/**		
				strncpy(filename, name+11, strlen(name)-11);//��ȡ�ļ���
				strcat(path,filename);//����·��
				mf_open((u8*)path, FA_OPEN_EXISTING);//open existing file
				printf("delete:the file name is %s\r\n",path);
//				res = mf_unlink((u8*)path);//ֻ��ɾ�������ļ�����ļ���
//				res = mf_deletedir((u8*)path);
//				res = FS_DeleteFolderFile((u8*)path);
//				res =mf_deletedir((u8*)path);
//				res =fileanddir_delete((u8*)path);
//				res =scan_files((u8*)path);
				res = mfscan_files((u8*)path);
				if(res ==FR_OK)//ɾ���ɹ�
				{
//			  sprintf (tx_buf,"the file:%s delete succeed!",path);
//				printf ("tx_buf = %s",tx_buf);
				}
				else if(res==FR_NO_FILE)//�ļ�������
				{
//				sprintf (tx_buf,"the file:%s is not exist!",path);
//				printf ("tx_buf = %s",tx_buf);
				}
        else//ɾ��ʧ��
				{
//					sprintf (tx_buf,"the file:%s  fail to delete!",path);
//			  	printf ("tx_buf = %s",tx_buf);
				}
				
				mf_close();**/
			}
			break;
			//POST
		case METHOD_POST :
			mid(http_request->URI, "/", " ", req_name);					/*��ȡ��������ļ���*/
		  if(strcmp(req_name,"config.cgi")==0)		//set the paramenter of W5500
			{
				cgi_ipconfig(http_request);												/*��������Ϣд����Ƭ��eeprom*/
				make_cgi_response(5,(int8*)W5500_Parameter_Configuration.LOCAL_IP_Addr,tx_buf);	/*������Ӧ���ı�����*/        
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
																													//send http response
				send(s, (u_char *)http_response, strlen((char *)http_response));		
				disconnect(s);																	//disconnect the socket			
				reboot_flag=1;																		//reboot flag
				return;				
			}
			if(strcmp(req_name,"firmware.cgi")==0) //update the firmware  //������й̼�����
			{
				IWDG_Feed(); //ι��		
				#ifdef DEBUG_HTTP
				printf("fw update\r\n");  //OK
				#endif
				mid((int8*)http_request->URI,"boundary=", "\r\n", (int8*)boundary);//boundary= OK
				#ifdef DEBUG_HTTP
        printf("boundary=%s\r\n",boundary);
        #endif
				//get the content_length
				mid((int8*)http_request->URI,"Content-Length: ","\r\n",sub);
        content_len=atoi32(sub,10);
				W5500_Parameter_Configuration.fw_len = content_len; //the length of firmware 
				#ifdef DEBUG_HTTP
        printf("content_len=%d\r\n",(int)content_len); //ok
        #endif
				//if(strstr((int8*)http_request->URI,"octet-stream\r\n\r\n"))//the first packet contains all header
				if(strstr((int8*)http_request->URI,"filename="))//��һ��http�� �����˱���ͷ+���ݣ���Ҫ�������
				{
					IWDG_Feed(); //ι��		
					hdr_pos=strstr((int8*)http_request->URI,"\r\n\r\n");
					pos1=strstr(hdr_pos,(int8*)boundary);
					pos2=strstr(pos1,"\r\n\r\n");
					#ifdef DEBUG_HTTP
					printf("@@pos1=%s\r\n",pos1);
					printf("@@pos2=%s\r\n",pos2);
					#endif
					//--boundary\r\n
					//......octet-stream\r\n\r\n
					//data......
					//\r\n--boundary--\r\n 
					hdr_len=strlen((int8*)boundary)+8+(pos2-pos1)+4+2/*+12*/;//8: last boundary start \r\n-- and end --/r/n, 4: /r/n/r/n, 2: start of first boundary --, 12: octet-stream
					upload_file_len=content_len-hdr_len;
					#ifdef DEBUG_HTTP
					printf("@@pos2=%d,boundary len=%d, upload file len=%d, hdr_len=%d\r\n",pos2-pos1,strlen((int8*)boundary),(int)upload_file_len,hdr_len);
					#endif
					rx_len=len-(hdr_pos-(int8*)http_request->URI+5+4);//file header and some file data included length. 5: POST and a space, 4: \r\n\r\n
					#ifdef DEBUG_HTTP
					printf("@@rx_len=%d\r\n",(int)rx_len);
					#endif
					//write fw file to applicaiton backup flash
					fw_offset = (pos2-(int8*)http_request->URI) +4 /*+ 12*/ +5; //fw data start position in the first packet. 4: \r\n\r\n, 12: octet-stream, 5: POST and a space,
					#ifdef DEBUG_HTTP
					printf("@@fw_offset=%d %02x %02x %02x %02x %02x %02x\r\n",fw_offset,*(int8*)(buf+fw_offset),*(int8*)(buf+fw_offset+1),*(int8*)(buf+fw_offset+2),*(int8*)(buf+fw_offset+3),*(int8*)(buf+fw_offset+4),*(int8*)(buf+fw_offset+5));
					#endif
					wr_len = len-fw_offset;
					//make a buf with lengh of multiful 4 and save remain bytes
					remain_len = wr_len % 4;
					memcpy(tmp_buf,&rx_buf[fw_offset],wr_len-remain_len); //correct 2018-07-03
					wr_len = wr_len-remain_len;//correct 2018-07-03 
					if(remain_len!=0)
					{
//							memcpy(tmp_buf,&buf[fw_offset],wr_len-remain_len);
//							wr_len = wr_len - remain_len;//real length
						  IWDG_Feed(); //ι��	
							memset(remain_buf,0xff,sizeof(remain_buf));
							memcpy(remain_buf,&buf[len-remain_len],remain_len);
						
					} //end of 	if(remain_len!=0)
					// program write the data into the backup flash 
					
					for(i=0;i<wr_len;i+=4)
					{
						  temp  = (u32)tmp_buf[i+3]<<24;
							temp |= (u32)tmp_buf[i+2]<<16;
							temp |= (u32)tmp_buf[i+1]<<8;
							temp |= (u32)tmp_buf[i];
							//STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
					  	STMFLASH_Write(flash_dest,&temp,1);
							flash_dest+=4;
					}
					#ifdef DEBUG_HTTP
					printf("@@programed first data packet %d %d\r\n",wr_len,remain_len);
					#endif								
				} //end of if(strstr((int8*)http_request->URI,"filename="))
				else  //http header and file header in 2 packets, and http header have been received in http_request->URI, so the file header should in the next packet.
				{
						#ifdef DEBUG_HTTP
						printf("header in 2 packets\r\n");
						#endif
					
					  tmp_len = getSn_RX_RSR(s);//receive the 2nd http packet
					  if(tmp_len>0)
						{
							if(tmp_len >1460)tmp_len = 1460;
							memset(rx_buf,0x00,sizeof(rx_buf)); // clean the rx_buf
							tmp_len = recv(s,(uint8*)rx_buf,tmp_len);
							
						}//end of  if(tmp_len>0)
						//--boundary\r\n
						//......octet-stream\r\n\r\n
						//data......
						//\r\n--boundary--\r\n 
						pos1=strstr(rx_buf,(int8*)boundary);
            //pos2=strstr(rx_buf,"octet-stream\r\n\r\n");//if it is not binary file, the content-type will not be octet-stream
						pos2=strstr(rx_buf,"\r\n\r\n");			
						#ifdef DEBUG_HTTP
						printf("pos1=%s\r\n",pos1);
						printf("pos2=%s\r\n",pos2);
						#endif                                                                                  
						hdr_len=strlen((int8*)boundary)+6+2+(pos2-pos1)+4+2/*+12*/;
						upload_file_len=content_len-hdr_len;	
						#ifdef DEBUG_HTTP
						printf("pos2=%d,boundary len=%d, upload file len=%d, hdr_len=%d\r\n",pos2-pos1,strlen((int8*)boundary),(int)upload_file_len,hdr_len);
						#endif	
						//rx_len=tmp_len-(hdr_pos-(int8*)http_request->URI+5+4);//file header and some file data included length. 5: POST and a space, 4: \r\n\r\n
						rx_len=tmp_len;
						//write fw file to applicaiton backup flash
						fw_offset = (pos2-pos1) +4 /*+ 12*/ +2 ; //fw data start position in the first packet. 4: \r\n\r\n, 12: octet-stream
						#ifdef DEBUG_HTTP
						printf("fw_offset=%d %02x %02x %02x %02x %02x %02x\r\n",fw_offset,*(int8*)(rx_buf+fw_offset),*(int8*)(rx_buf+fw_offset+1),*(int8*)(rx_buf+fw_offset+2),*(int8*)(rx_buf+fw_offset+3),*(int8*)(rx_buf+fw_offset+4),*(int8*)(rx_buf+fw_offset+5));
						#endif
						wr_len = tmp_len-fw_offset;
						//make a buf with lengh of multiful 4 and save remain bytes
						remain_len = wr_len % 4;
						memcpy(tmp_buf,&rx_buf[fw_offset],wr_len-remain_len); //����
					  wr_len = wr_len-remain_len;//����
						if(remain_len!=0)
						{
//							memcpy(tmp_buf,&rx_buf[fw_offset],wr_len-remain_len);
//							wr_len = wr_len - remain_len;//real length
							IWDG_Feed(); //ι��	
							memset(remain_buf,0xff,sizeof(remain_buf));
							memcpy(remain_buf,&rx_buf[len-remain_len],remain_len);
						}
						for(i=0;i<wr_len;i+=4)
				   	{
						  temp  = (u32)tmp_buf[i+3]<<24;
							temp |= (u32)tmp_buf[i+2]<<16;
							temp |= (u32)tmp_buf[i+1]<<8;
							temp |= (u32)tmp_buf[i];
							//STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
					  	STMFLASH_Write(flash_dest,&temp,1);
							flash_dest+=4;
					  }
						/*
						make_alert_jump_msg(tx_buf,"Upload file failed! please upload the file again.",(int8*)ConfigMsg.lip,80);
						sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
						send(s, (u_char *)http_response, strlen((char const*)http_response));
						return;
						*/									  
				} //end of else
				if(content_len>0x1000) //the length over 4k
				{
					while(rx_len !=content_len)
					{
						tmp_len = getSn_RX_RSR(s); // the receive length
						if(tmp_len>0)
						{
							if(tmp_len>1460)tmp_len = 1460;
							tmp_len = recv(s,(uint8*)rx_buf,tmp_len);
							#ifdef DEBUG_HTTP
								printf("tmp_len=%d\r\n",tmp_len);
              #endif
							if(rx_len+tmp_len==content_len)//the last packet that includes boundary
							{ 
										#ifdef DEBUG_HTTP
										printf("the last one packet with boundary\r\n");
										#endif
										wr_len = tmp_len - strlen((int8*)boundary) - 8;
							} // end of if(rx_len+tmp_len==content_len)
							else
							{
								wr_len = tmp_len;								
							} //end of else
							fw_offset=0;
							// make buffer
							if(remain_len)
							{
								memcpy(tmp_buf,remain_buf,remain_len);								
							} // end of if (remain_len)
							if(wr_len+remain_len >1460)
							{
								memcpy(&tmp_buf[remain_len],rx_buf,wr_len-remain_len);
								//save remain bytes
							  memcpy(remain_buf,&rx_buf[tmp_len-remain_len],remain_len);
								
							}//end of if(wr_len+remain_len >1460)
							else
							{
								memcpy(&tmp_buf[remain_len],rx_buf, wr_len);
								memset(&tmp_buf[wr_len+remain_len],0xff,4-remain_len);
							  wr_len=wr_len+remain_len;
							} //end of else
							//program
							for(i = 0; i<wr_len; i+=4)
							{
								temp  = (u32)tmp_buf[i+3]<<24;
							  temp |= (u32)tmp_buf[i+2]<<16;
							  temp |= (u32)tmp_buf[i+1]<<8;
							  temp |= (u32)tmp_buf[i];
							  //STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
					  	  STMFLASH_Write(flash_dest,&temp,1);
							  flash_dest+=4;								
							}
							rx_len+=tmp_len;
								#ifdef DEBUG_HTTP
								printf("len=%d, rx len=%d, wr len=%d, @%08x\r\n",tmp_len, (int)rx_len, wr_len, (int)flash_dest);
              	#endif
							if(rx_len==content_len)  //���ݽ������
							{
								//��ͻ��˷�����Ӧ
								IWDG_Feed(); //ι��	
							W5500_Parameter_Configuration.state = NEW_APP_IN_BACK;
							W5500_Parameter_Configuration.fw_len = upload_file_len;
							reboot_flag = 1;
							make_cgi_response(6,(int8*)W5500_Parameter_Configuration.LOCAL_IP_Addr,tx_buf);         
              sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
              send(s, (uint8 *)http_response, strlen((char *)http_response));		
							
							//ͨ��UDPЭ������λ������ ������ɱ�־
								AT24CXX_Read(34,&APP_Running_Flash_Address_Flag,1); //��ȡAPP��¼��ַ��־
								Tx_Buffer[0] = 0xEF;
								Tx_Buffer[1] = 0x0b; // 11���ֽ�
								Tx_Buffer[2] = 0x08; //������
								Tx_Buffer[3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0];
								Tx_Buffer[4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1];
								Tx_Buffer[5] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2];
								Tx_Buffer[6] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3];
								Tx_Buffer[7] = 0xaa;//���ճɹ���־��
								Tx_Buffer[8] = APP_Running_Flash_Address_Flag; //APP��¼����Ҳ����������
								Tx_Buffer[9] = GetXor(&Rx_Buffer[1],7);//����־
								Tx_Buffer[10] = 0xFF; //�����ַ�
								
							} //end of if(rx_len==content_len)
							else //else of if(rx_len==content_len)
							{
								//ͨ��UDPЭ������λ������ ������ɱ�־
								AT24CXX_Read(34,&APP_Running_Flash_Address_Flag,1); //��ȡAPP��¼��ַ��־
								Tx_Buffer[0] = 0xEF;
								Tx_Buffer[1] = 0x0b; // 11���ֽ�
								Tx_Buffer[2] = 0x08; //������
								Tx_Buffer[3] = W5500_Parameter_Configuration.LOCAL_IP_Addr[0];
								Tx_Buffer[4] = W5500_Parameter_Configuration.LOCAL_IP_Addr[1];
								Tx_Buffer[5] = W5500_Parameter_Configuration.LOCAL_IP_Addr[2];
								Tx_Buffer[6] = W5500_Parameter_Configuration.LOCAL_IP_Addr[3];
								Tx_Buffer[7] = 0xbb;//����ʧ�ܱ�־
								Tx_Buffer[8] = APP_Running_Flash_Address_Flag; //APP��¼����Ҳ����������
								Tx_Buffer[9] = GetXor(&Rx_Buffer[1],7);//����־
								Tx_Buffer[10] = 0xFF; //�����ַ�
								
							} //end of else   else of if(rx_len==content_len)
						} //end of if(tmp_len>0)
						
					} //end of while(rx_len !=content_len)
					
				} // end of if(content_len>0x1000)
				else //the length is less than 4k
				{
						#ifdef DEBUG_HTTP
						printf("content len error=%d\r\n",(int)content_len);
						#endif
					  IWDG_Feed(); //ι��	
					  make_cgi_response(6,(int8*)W5500_Parameter_Configuration.LOCAL_IP_Addr,tx_buf); 
					  sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
					  send(s, (uint8 *)http_response, strlen((char *)http_response));						  
				} //end of else  if								
			} //end of if(strcmp(req_name,"firmware.cgi")==0) //update the firmware 
			
			break;
		default:
			break;
		
		
		
	}

}
  		

/**
*@brief		��������Ϣд����Ƭ��eeprom
*@param		http_request������һ��http����Ľṹ��ָ��
*@return	��
*/
void cgi_ipconfig(st_http_request *http_request)
{ 
  uint8 * param;
  param = get_http_param_value(http_request->URI,"ip");		/*��ȡ�޸ĺ��IP��ַ*/
  if(param)
  {
    inet_addr_((uint8*)param, W5500_Parameter_Configuration.LOCAL_IP_Addr);	
  }
  param = get_http_param_value(http_request->URI,"gw");		/*��ȡ�޸ĺ������*/
  if(param)
  {
    inet_addr_((uint8*)param, W5500_Parameter_Configuration.Gateway_IP);	
  }
  param = get_http_param_value(http_request->URI,"sub");	/*��ȡ�޸ĺ����������*/
  if(param)
  {
    inet_addr_((uint8*)param, W5500_Parameter_Configuration.Sub_Mask);		
  }

}

/**
*@brief		ִ��http��Ӧ
*@param		��  
*@return	��
*/
void make_cgi_response(uint16 delay, int8* url,int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>iWeb - Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j + ' ';j--;setTimeout('func()',1000);if(j==0)location.href='http://%d.%d.%d.%d/';}</script></head><body onload='func()'>please wait for a while, the module will boot in<span style='color:red;' id='delay'></span> seconds.</body></html>",delay,url[0],url[1],url[2],url[3]);
  return;
}


