#ifndef __FATTESTER_H
#define __FATTESTER_H 			   
#include "sys.h"	   
#include "ff.h"
#include "SD.h"
#include "usmart.h"
#include "usart.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
#include "string.h"
#include "define.h"
#include "sys.h"
#include "types.h"
#include "w5500_conf.h"
#include "stdio.h"
#include "w5500.h"
#include "SPI_W5500.h"
#include "socket.h"
#include "RTC.h" 
#include "malloc.h"

u8 mf_mount(u8* path,u8 mt);
u8 mf_open(u8*path,u8 mode);
u8 mf_close(void);
u8 mf_read(u16 len);
u8 mf_write(u8*dat,u16 len);
u8 mf_opendir(u8* path);
u8 mf_closedir(void);
u8 mf_readdir(void);
u8 mf_scan_files(u8 * path);
u32 mf_showfree(u8 *drv);
u8 mf_lseek(u32 offset);
u32 mf_tell(void);
u32 mf_size(void);
u8 mf_mkdir(u8*pname);
u8 mf_fmkfs(u8* path,u8 mode,u16 au);
u8 mf_unlink(u8 *pname);
u8 mf_rename(u8 *oldname,u8* newname);
void mf_getlabel(u8 *path);
void mf_setlabel(u8 *path); 
void mf_gets(u16 size);
u8 mf_putc(u8 c);
u8 mf_puts(u8*c);
u8 mf_scan_files_getnames(u8 * path); 

unsigned char mf_scan(u8* path,char** filenames);

//	FRESULT exf_deldir(TCHAR* path);
u8 mf_deletedir(u8* path);
u8 FS_DeleteFolderFile(u8* path);
int get_all(u8** content);
u8 fileanddir_delete(u8* path);
FRESULT scan_files(u8* path);
u8 mfscan_files(u8 * path);
u8 mf_scan_files_delete(const TCHAR * path);

//u8 SD_Write_One_File(unsigned char* buff);

void SD_File_Time(char* FileTimeBuff);
void SD_Dir_Time(char* DirTimeBuff);
u8 SD_Write_One_Dir(void);
u8 SD_Write_New_File(void);
//u8 SD_Write_Old_File(unsigned char* buff);


u8 sd_CSV_NEW(void);
u8 SD_CSV_OLD(unsigned char data[]);


u8 SD_Write_New_File(void);
u8 SD_Write_Old_File(unsigned char ReceiveorSend, unsigned char buff[]);
int CSV_Format(unsigned char ReceiveorSend,unsigned char data[]);


u8 SD_Creat_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times);
u8 SD_Creat_Error_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times);


unsigned char IntToHexChar(unsigned char c);
void BuffToHex(unsigned char DataInput[],unsigned char DataOutput[]);
void BUFFTOhexTEST(unsigned char DataInput[]);





#endif





























