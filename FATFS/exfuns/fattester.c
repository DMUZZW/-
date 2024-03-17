#include "fattester.h"	 

    
		
TCHAR* files; 
TCHAR* lname; 

//为磁盘注册工作区	 
//path:磁盘路径，比如"0:"、"1:"
//mt:0，不立即注册（稍后注册）；1，立即注册
//返回值:执行结果
u8 mf_mount(u8* path,u8 mt)
{		   
	return f_mount(fs[1],(const TCHAR*)path,mt); 
}
//打开路径下的文件
//path:路径+文件名
//mode:打开模式
//返回值:执行结果
u8 mf_open(u8*path,u8 mode)
{
	u8 res;	 
	res=f_open(file,(const TCHAR*)path,mode);//打开文件夹
	return res;
} 
//关闭文件
//返回值:执行结果
u8 mf_close(void)
{
	f_close(file);
	return 0;
}
//读出数据
//len:读出的长度
//返回值:执行结果
u8 mf_read(u16 len)
{
	u16 i,t;
	u8 res=0;
	u16 tlen=0;
	printf("\r\nRead file data is:\r\n");
	
	for(i=0;i<len/512;i++)
	{
		res=f_read(file,fatbuf,512,&br);
		if(res)
		{
			printf("Read Error:%d\r\n",res);
			break;
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
			//printf("content=%s\r\n",fatbuf);
		}
	}
	
	if(len%512)
	{
		res=f_read(file,fatbuf,len%512,&br);
		if(res)	//读数据出错了
		{
			printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}	 
	}
	
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//读到的数据长度
	printf("Read data over\r\n");	 
	return res;
}

int get_all(u8** content){
	f_read(file,fatbuf,512,&br);
	*content = fatbuf;
	return br;
}

//写入数据
//dat:数据缓存区
//len:写入长度
//返回值:执行结果
u8 mf_write(u8*dat,u16 len)
{			    
	u8 res;	   					   

	printf("\r\nBegin Write file...\r\n");
	printf("Write data len:%d\r\n",len);	 
	res=f_write(file,dat,len,&bw);
	if(res)
	{
		printf("Write Error:%d\r\n",res);   
	}else printf("Writed data len:%d\r\n",bw);
	printf("Write data over.\r\n");
	return res;
}

//打开目录
 //path:路径
//返回值:执行结果
u8 mf_opendir(u8* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//关闭目录 
//返回值:执行结果
u8 mf_closedir(void)
{
	return f_closedir(&dir);	
}
//打读取文件夹
//返回值:执行结果
u8 mf_readdir(void)
{
	u8 res;    
	res=f_readdir(&dir,&fileinfo);	//读取一个文件的信息
	if(res!=FR_OK)return res;		//出错了 
	printf("\r\n DIR info:\r\n");

	printf("dir.dptr:%d\r\n",dir.dptr);
	printf("dir.obj.id:%d\r\n",dir.obj.id);
	printf("dir.obj.sclust:%d\r\n",dir.obj.sclust);
	printf("dir.obj.objsize:%lld\r\n",dir.obj.objsize);
	printf("dir.obj.c_ofs:%d\r\n",dir.obj.c_ofs);
	printf("dir.clust:%d\r\n",dir.clust);
	printf("dir.sect:%d\r\n",dir.sect);	  
	printf("dir.blk_ofs:%d\r\n",dir.blk_ofs);	  

	printf("\r\n");
	printf("File Name is:%s\r\n",fileinfo.fname);
	printf("File Size is:%lld\r\n",fileinfo.fsize);
	printf("File data is:%d\r\n",fileinfo.fdate);
	printf("File time is:%d\r\n",fileinfo.ftime);
	printf("File Attr is:%d\r\n",fileinfo.fattrib);
	printf("\r\n"); 
	return 0;
}			 

 //遍历文件
 //path:路径
 //返回值:执行结果
u8 mf_scan_files(u8 * path)
{
	FRESULT res;	 
    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
 			printf("%s/", path);//打印路径	
			printf("%s\r\n",fileinfo.fname);//打印文件名	  
		} 
    }	   
    return res;	  
}

//遍历文件，并把获取的文件名放在filenames中
unsigned char mf_scan(u8* path,char** filenames)
{
	
	FRESULT res;
	char *p1 = NULL;
	char str1[] = ",";
	char json_before[] = "{\"files\":\"";
	char json_end[] = "\"}";
	memset(p1, 0, 1024);
	strcat(p1,json_before);
	res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
	if(res == FR_OK)
	{
//		memset(p2,0,2048);
		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	    if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
			strcat(p1,fileinfo.fname);
			strcat(p1,str1);
		}
		strcat(p1,json_end);
		*filenames = p1;
	}
	mf_closedir();
	return res;	
}
 
//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
u32 mf_showfree(u8 *drv)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
	    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//总容量小于10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n磁盘总容量:%d KB\r\n"
		           "可用空间:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n磁盘总容量:%d MB\r\n"
		           "可用空间:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//文件读写指针偏移
//offset:相对首地址的偏移量
//返回值:执行结果.
u8 mf_lseek(u32 offset)
{
	return f_lseek(file,offset);
}
//读取文件当前读写指针的位置.
//返回值:位置
u32 mf_tell(void)
{
	return f_tell(file);
}
//读取文件大小
//返回值:文件大小
u32 mf_size(void)
{
	return f_size(file);
} 
//创建目录
//pname:目录路径+名字
//返回值:执行结果
u8 mf_mkdir(u8*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//格式化
//path:磁盘路径，比如"0:"、"1:"
//mode:模式
//au:簇大小
//返回值:执行结果
u8 mf_fmkfs(u8* path,u8 mode,u16 au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//格式化,drv:盘符;mode:模式;au:簇大小
} 
//删除文件/目录
//pname:文件/目录路径+名字
//返回值:执行结果
u8 mf_unlink(u8 *pname)
{
	return  f_unlink((const TCHAR *)pname);
}


//删除文件夹，但存在不能删除文件的缺陷
u8 mf_scan_files_delete(const TCHAR * path)
{
		TCHAR *files;
	  FRESULT res;
		FILINFO fno;
		DIR   mydir;
		char pathname[50];
    strcpy(pathname,path);
		printf("path==============================%s\r\n",pathname);
#if _USE_LFN  
    fno.fsize = _MAX_LFN;
#endif 
    res = f_opendir(&mydir,(const TCHAR*)pathname); //打开一个目录
		
    while((res == FR_OK) && (FR_OK == f_readdir(&mydir, &fno)))//
			{
				 if(0 == strlen(fno.fname))          
						break;
				 if(0 == strcmp(fno.fname, "."))     
						continue;  
				 if(0 == strcmp(fno.fname, ".."))    
						continue;  
				 
				 memset(files, 0, _MAX_LFN + 2);  
#if _USE_LFN  
        sprintf((char*)files, "%s/%s", pathname, (*fno.fname) ? fno.fname : fno.fname);  
#else  
        sprintf((char*)files, "%s/%s", pathname, fno.fname);  
#endif
				 if (fno.fattrib & AM_DIR) 
					 {
						  printf ("fno.fattrib & AM_DIR");
						  res = mf_scan_files_delete(files);
				   }
					 else
					 {
						 		printf("files name:%s\r\n",files);
								res = f_unlink(files);  
				        printf("res=%d\r\n",res);
					 }

          
		}	   
		if(res == FR_OK)    
		{
			res = f_unlink(pathname);
			printf("delete file itself:%s\r\n",files);
		}
    return res;	  
}


//u8 FS_DeleteFolderFile(u8* path)
//{
//	FRESULT res;
//	//删除此文件夹以下的所有文件
//	res = mf_scan_files_delete((const TCHAR*)path);
//	if(res==FR_OK)
//	{
//		res=f_unlink((const TCHAR*)path);//删除文件夹
//		
//	}
////	else if(FR_NO_PATH==res)
////	{
//		res = f_unlink((const TCHAR*)path);
////	}
//	return res;
//}

//修改文件/目录名字(如果目录不同,还可以移动文件哦!)
//oldname:之前的名字
//newname:新名字
//返回值:执行结果
u8 mf_rename(u8 *oldname,u8* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void mf_getlabel(u8 *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		printf("\r\n磁盘%s 的盘符为:%s\r\n",path,buf);
		printf("磁盘%s 的序列号:%X\r\n\r\n",path,sn); 
	}else printf("\r\n获取失败，错误码:%X\r\n",res);
}
//设置盘符（磁盘名字），最长11个字符！！，支持数字和大写字母组合以及汉字等
//path:磁盘号+名字，比如"0:ALIENTEK"、"1:OPENEDV"  
void mf_setlabel(u8 *path)
{
	u8 res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		printf("\r\n磁盘盘符设置成功:%s\r\n",path);
	}else printf("\r\n磁盘盘符设置失败，错误码:%X\r\n",res);
} 

//从文件里面读取一段字符串
//size:要读取的长度
void mf_gets(u16 size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//没有数据读到
	else
	{
		printf("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}

//需要_USE_STRFUNC>=1
//写一个字符到文件
//c:要写入的字符
//返回值:执行结果
u8 mf_putc(u8 c)
{
	return f_putc((TCHAR)c,file);
}
//写字符串到文件
//c:要写入的字符串
//返回值:写入的字符串长度
u8 mf_puts(u8*c)
{
	return f_puts((TCHAR*)c,file);
}


 //遍历文件
 //path:路径
 //返回值:执行结果
u8 mfscan_files(u8 * path)
{
	  FRESULT res;	
    unsigned int i;	
    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	 {	
//		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
 	        printf("%s/", path);//打印路径	
			    printf("%s\r\n",fileinfo.fname);//打印文件名	
          if(fileinfo.fattrib & AM_DIR)//是目录
					{
					  i=strlen(path);
			      sprintf (&path[i],"/%s\r\n",fileinfo.fname);
				    printf("是目录::%s\r\n",path);
						if(0 ==strlen(fileinfo.fname))break;
			      if(0==strcmp(fileinfo.fname,"."))continue ;
			      if(0==strcmp(fileinfo.fname,".."))continue ;
						res=mfscan_files(path);
						printf("执行到目录\r\n");
						 
					}	
          else//不是目录
          {
						printf("是文件:%s/%s\r\n",path,fileinfo.fname);
					}						
		} 
    }	   
    return res;	  
} 

int CSV_Format(unsigned char ReceiveorSend,unsigned char data[])
{
	//获取时间
	char FileTimeBuff[20];//文件夹名
	char RECEIVE[8]={"Receive"};
	char SEND[4]={"Send"};
	unsigned int datalength=0x00;
	char* p;
	 p = mymalloc(SRAMIN,2048);
	HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	sprintf((char*)FileTimeBuff,"Y20%02dM%02dD%02dH%02dM%02dS%02dMS%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //合并时间	
	if(ReceiveorSend ==0)//发送数据
	{
		 sprintf(p,"%s,%s,%s\n",FileTimeBuff,SEND,data);
		 printf("send date is:%s\r\n",p);
	}
	else if(ReceiveorSend ==1)//接收数据
	{
		sprintf(p,"%s,%s,%s\n",FileTimeBuff,RECEIVE,data);
		 printf("receive date is:%s\r\n",p);
	}
	else
	{
		printf("error");
	}
	datalength = strlen(p);
	myfree(SRAMIN,p);//
	return datalength;
}



//make file
u8 SD_Write_New_File(void)
{
	//程序流程   //1、读取当前年月日 	//2、创建一个文件夹
	
	FRESULT res;	
	char DirTimeBuff[10];//文件夹名
	char DirPath[20];//文件夹路径
	char FilePath[25];
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹名
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹路径
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //文件名
	//创建一个文件夹
	res= f_mkdir(DirTimeBuff);
	res = f_opendir(&dir,(const TCHAR*)DirPath); 
	if(res ==FR_OK)
	{
			//创建新文件
			res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
			//写数据
			if(res ==FR_OK)
       {
				 	//写入数据
				 res = f_write(file,"Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\n",63,&bw);
			 }
			 	//关闭文件
			f_close(file);
	}
return res;
}

////再次写入旧文件中
u8 SD_Write_Old_File(unsigned char ReceiveorSend, unsigned char buff[])
{
	
	FRESULT res;	
  char DirTimeBuff[10];//文件夹名
	char DirPath[20];//文件夹路径
	char FilePath[25];
	char FileTimeBuff[25];//文件夹名
//	unsigned int datalength=0x00;
	char* p;
  p = mymalloc(SRAMIN,2048);
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹名
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹路径
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //文件名
	sprintf((char*)FileTimeBuff,"20%02d:%02d:%02d:%02d:%02d:%02d:%d\r\n",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //合并时间	
	//打开文件夹
  res = f_opendir(&dir,(const TCHAR*)DirPath);
//	printf ("DirPath:%s res=%d\r\n",DirPath,res);	
	//打开文件
	if(res ==FR_OK)
	{
		if(ReceiveorSend==0)//send
		{
			sprintf(p,"%s,Send,%s\n",FileTimeBuff,buff);
			printf("send    date is:%s\r\n",p);
		}
		else if(ReceiveorSend==1)//receive
		{
		 sprintf(p,"%s,Receive,%s\n",FileTimeBuff,buff);
		 printf("receive date is:%s\r\n",p);
		}
		res=f_open(file,FilePath,FA_OPEN_EXISTING|FA_WRITE);
		//查找文件结尾
		res= f_lseek(file,f_size(file));	
		//写数据
//		sprintf((char*)buff,"%s\r\n",buff);
		res = f_write(file, p, strlen(p), &bw);
//		printf("buff is: %s",p);
//		printf("file %s\r\n",FileName);
		//关闭文件
		f_close(file);
	}
  myfree(SRAMIN,p);//
	return res;
}


////综合
//u8 SD_Creat_CSVFile(unsigned char ReceiveorSend, unsigned char buff[])
//{
//	FRESULT res;	
//  char DirTimeBuff[10];//文件夹名
//	char DirPath[20];//文件夹路径
//	char FilePath[25];
//	char FileTimeBuff[25];//文件夹名
////	unsigned int datalength=0x00;
//	char* p;
//  p = mymalloc(SRAMIN,2048);
//  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
//	
//	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹名
//  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹路径
//	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //文件名
//	sprintf((char*)FileTimeBuff,"20%02d:%02d:%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //合并时间	
//  
//		if(ReceiveorSend==0)//send
//		{
//			sprintf(p,"%s,Send,%s\n",FileTimeBuff,buff);
//			printf("send date is:%s\r\n",p);
//		}
//		else if(ReceiveorSend==1)//receive
//		{
//		  sprintf(p,"%s,Receive,%s\n",FileTimeBuff,buff);
//		  printf("receive date is:%s\r\n",p);
//		}
//	//创建一个文件夹
//	res= f_mkdir(DirTimeBuff);
//	if(res ==FR_EXIST)//文件夹已经存在
//	{
//		res = f_opendir(&dir,(const TCHAR*)DirPath); //打开文件夹
//		if(res ==FR_OK)
//		{
//				//创建新文件
//			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
//				if(res ==FR_OK)
//       {
//				 	//写入数据
//				 res = f_write(file,"Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\n",63,&bw);
//			 }
//			 	//关闭文件
//			f_close(file);
//			 
//		}
//		
//		res=f_open(file,FilePath,FA_OPEN_EXISTING|FA_WRITE);
//    res= f_lseek(file,f_size(file));	
//		res = f_write(file, p, strlen(p), &bw);			
//		f_close(file);
//	}
////	else 
//	myfree(SRAMIN,p);//
//	return res;
//}


////综合

/*第一个参数 表示数据是发送还是接受，第二个参数时要写入的数据 第四个数据是，第几次操作*/
u8 SD_Creat_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times)
{
	FRESULT res;	
  char DirTimeBuff[10];//文件夹名
	char DirPath[20];//文件夹路径
	char FilePath[30];
	char FileTimeBuff[30];//文件夹名
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;	
	unsigned char temp;
	char* p;
	char* p1;
	
  p = mymalloc(SRAMIN,2048);
	
//	DataInputLength = sizeof(buff);//其作用是确定循环次数，如0XAD，0XCD，
//  DataInputLength = strlen((const char*)buff)/2;
	DataInputLength = strlen((const char*)buff);
	DataOutputLength = 3*DataInputLength;
	p1 = mymalloc(SRAMIN,DataOutputLength);
	memset(p1,0x00,DataOutputLength);
	for(i=0;i<DataInputLength;i++)
	{
		temp = buff[i]&0xf0;
		p1[3*i] = IntToHexChar(temp>>4);
		temp = buff[i]&0x0f;
		p1[3*i+1] = IntToHexChar(temp);
		if(i!=(DataInputLength-1))p1[3*i+2] = 0X2D;
	}	
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹名
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹路径
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.csv",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //文件名
	sprintf((char*)FileTimeBuff,"20%02d-%02d-%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //合并时间	
  
		if(ReceiveorSend==0)//send
		{
			sprintf(p,"%d,%s,Send, %s\r\n",times,FileTimeBuff,p1);
			printf("send date is:%s\r\n",p);
		}
		else if(ReceiveorSend==1)//receive
		{
		  sprintf(p,"%d,%s,Receive, %s\r\n",times,FileTimeBuff,p1);
		  printf("receive date is:%s\r\n",p);
		}
	//创建一个文件夹
	res= f_mkdir(DirTimeBuff);
	if(res ==FR_EXIST)//文件夹已经存在
	{
		res = f_opendir(&dir,(const TCHAR*)DirPath); //打开文件夹
		if(res ==FR_OK)
		{
				//创建新文件
			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
				if(res ==FR_OK)
       {
				 	//写入数据
				 res = f_write(file,",Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\r\n",63,&bw);
			 }
			 	//关闭文件
			f_close(file);
			 
		}
		
		res=f_open(file,FilePath,FA_OPEN_EXISTING|FA_WRITE);
    res= f_lseek(file,f_size(file));	
		res = f_write(file, p, strlen(p), &bw);			
		f_close(file);
	}

	myfree(SRAMIN,p1);//
	myfree(SRAMIN,p);//
	delay_ms(15);
	return res;
}




//生成故障信息CSV文件
u8 SD_Creat_Error_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times)
{
	FRESULT res;	
  char DirTimeBuff[10];//文件夹名
	char DirPath[20];//文件夹路径
	char FilePath[30];
	char FileTimeBuff[30];//文件夹名
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;	
	unsigned char temp;
	char* p;
	char* p1;
  p = mymalloc(SRAMIN,2048);
	
//	DataInputLength = (sizeof((char *)buff));//其作用是确定循环次数，如0XAD，0XCD，
//	DataInputLength = strlen((const char*)buff)/2;
	DataInputLength = strlen((const char*)buff);
	DataOutputLength = 3*DataInputLength;
	p1 = mymalloc(SRAMIN,DataOutputLength);
	memset(p1,0x00,DataOutputLength);
	for(i=0;i<DataInputLength;i++)
	{
		temp = buff[i]&0xf0;
		p1[3*i] = IntToHexChar(temp>>4);
		temp = buff[i]&0x0f;
		p1[3*i+1] = IntToHexChar(temp);
		if(i!=(DataInputLength-1))p1[3*i+2] = 0X2D;
	}
	
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹名
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //文件夹路径
	sprintf((char*)FilePath,"%s/ERRORY20%02dM%02dD%02d.csv",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //文件名
	sprintf((char*)FileTimeBuff,"20%02d-%02d-%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //合并时间	
  
		if(ReceiveorSend==0)//send
		{
			sprintf(p,"	%d,%s,Send,%s\r\n",times,FileTimeBuff,p1);
			printf("send date is:%s\r\n",p);
		}
		else if(ReceiveorSend==1)//receive
		{
		  sprintf(p," %d,%s,Receive,%s\r\n",times,FileTimeBuff,p1);
		  printf("receive date is:%s\r\n",p);
		}
	//创建一个文件夹
	res= f_mkdir(DirTimeBuff);
	if(res ==FR_EXIST)//文件夹已经存在
	{
		res = f_opendir(&dir,(const TCHAR*)DirPath); //打开文件夹
		if(res ==FR_OK)
		{
				//创建新文件
			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
				if(res ==FR_OK)
       {
				 	//写入数据
				 res = f_write(file,",Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\r\n",63,&bw);
			 }
			 	//关闭文件
			f_close(file);
			 
		}
		
		res=f_open(file,FilePath,FA_OPEN_EXISTING|FA_WRITE);
    res= f_lseek(file,f_size(file));	
		res = f_write(file, p, strlen(p), &bw);			
		f_close(file);
	}
//	else 
	myfree(SRAMIN,p1);//
	myfree(SRAMIN,p);//
	delay_ms(15);
	return res;
}


//十六进制转换成字符串
unsigned char IntToHexChar(unsigned char c)
{
	if(c>9)
		return (c+55);
	else
		return(c+0x30);
}
void BuffToHex(unsigned char DataInput[],unsigned char DataOutput[])
{
	unsigned int DataInputLength=0x00,i=0x00;
	unsigned char temp;
	DataInputLength = (strlen((char *)DataInput))/2;
	for(i=0;i<DataInputLength;i++)
	{
		temp = DataInput[i]&0xf0;
		DataOutput[3*i] = IntToHexChar(temp>>4);
		temp = DataInput[i]&0x0f;
		DataOutput[3*i+1] = IntToHexChar(temp);
		DataOutput[3*i+2] = 0X2D;
	}
	printf("*******%s*********\r\n",DataOutput);
}


void BUFFTOhexTEST(unsigned char DataInput[])
{
	//上位机传输过来的十六进制char型数组 如0xAB,是一个字节，如果要显示为“AB” 则是2个字节，外加一个字节的- 需要三个字节
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;
	char* p2;
	unsigned char temp;
	DataInputLength = (sizeof((char *)DataInput));//其作用是确定循环次数，如0XAD，0XCD，
	DataOutputLength = 3*DataInputLength;
  p2 = mymalloc(SRAMIN,DataOutputLength);
	memset(p2,0x00,DataOutputLength);
	for(i=0;i<DataInputLength;i++)
	{
		temp = DataInput[i]&0xf0;
		p2[3*i] = IntToHexChar(temp>>4);
		temp = DataInput[i]&0x0f;
		p2[3*i+1] = IntToHexChar(temp);
		if(i!=(DataInputLength-1))p2[3*i+2] = 0X2D;
	}
	printf("*******%s*********\r\n",p2);
	myfree(SRAMIN,p2);//
}







//测试使用
u8 sd_CSV_NEW(void)
{
	 FRESULT res;
	res=f_open(file,"0:/DATA.txt",FA_CREATE_NEW|FA_WRITE);
	res = f_write(file,"Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\r\n",66,&bw);
	f_close(file);
	return res;
}

u8 SD_CSV_OLD(unsigned char data[])
{
	  FRESULT res;
	 char time[] ={"2019-02-12-22-22-59-456"};
	 char RT[] ={"receive"} ;
	 unsigned long times=0x00;
//	 char data2[50];
	 char* p;
	 char* p2;
	 unsigned char temp;
	 unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;
	 DataInputLength = strlen(data);//其作用是确定循环次数，如0XAD，0XCD，
	 DataOutputLength = 3*DataInputLength;
   p2 = mymalloc(SRAMIN,DataOutputLength);
	 memset(p2,0x00,DataOutputLength);
	 for(i=0;i<DataInputLength;i++)
	 {
		 temp = data[i]&0xf0;
		 p2[3*i] = IntToHexChar(temp>>4);
		 temp = data[i]&0x0f;
		 p2[3*i+1] = IntToHexChar(temp);
		 if(i!=(DataInputLength-1))p2[3*i+2] = 0X2D;
	 }
	 printf("*******%s*********\r\n",p2);	
	 p = mymalloc(SRAMIN,2048);
//	 sprintf(data2,"%s",p2);
//	 printf("data2 is :%s",data2);
	 times=times+1;
	 sprintf(p,"%d,%s,%s,%s\r\n",times,time,RT,p2);
	 printf("the file is:%s,length=%d\n",p,strlen(p));	 
	 res=f_open(file,"0:/DATA.txt",FA_OPEN_EXISTING|FA_WRITE);
	 res= f_lseek(file,f_size(file));
	 res= f_write(file,p,strlen(p),&bw);
	 f_close(file);
	  myfree(SRAMIN,p2);//
	  myfree(SRAMIN,p);// 
	 return res;
}




////u8 mf_deletedir(u8* path)
////{
////    FRESULT res;
////	  char pathname[50];
////	  strcpy(pathname,(char*)path);
////	  res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
//////	printf("dir %s res:%d\r\n",path,res);
////	  printf("the dir:%s openres:%d\r\n ",path,res);
////		while((res==FR_OK)&&(res == f_readdir(&dir, &fileinfo)))
////		{
////			printf("into while\r\n");
////		  if(0 ==strlen(fileinfo.fname))break;
////			if(0==strcmp(fileinfo.fname,"."))continue ;
////			if(0==strcmp(fileinfo.fname,".."))continue ;
////			memset(files,0,_MAX_LFN+2);
////			printf ("pathname:%s\r\n",pathname);
////			printf("fileinfo.fname:%s\r\n",fileinfo.fname);
//////			sprintf((char*)files,"%s/%s",pathname,fileinfo.fname);
////			printf("files:%s\r\n",files);
////			if(fileinfo.fattrib & AM_DIR)//如果是文件夹
////			{
////				
////		    	res =	mf_deletedir(files); //递归调用，要把文件名传进去
////				  printf("di gui\r\n");
////			}
////			else
////			{
////				res = f_unlink(files);
////				
////				printf("res=%d,pathname=%s,files=%s\r\n",res,pathname,files);
////			}
////			
////			
////		}
////	 if(res==FR_OK) res = f_unlink(pathname);
////	 printf("shanchuwenjain benshen %s\r\n",pathname);
////	return res;
////}
//	unsigned int i=0;
//u8 mf_deletedir(u8* path)
//{

//	FRESULT res;
//	
//	res=f_opendir(&dir,(const TCHAR*)path);
//	printf("the dir:%s openres:%d\r\n ",path,res);
//  while((res==FR_OK)&&(res == f_readdir(&dir, &fileinfo)))
//	{
//			printf("into while\r\n");
//		  if(0 ==strlen(fileinfo.fname))
//			{
//				printf ("0 ==strlen(fileinfo.fname)");
//				break;//读到的文件名为空
//			}	
//			
//			if(0==strcmp(fileinfo.fname,"."))
//			{
//				printf ("0==strcmp(fileinfo.fname). one");
//				continue ;//读到的文件名为当前文件夹
//			}
//			if(0==strcmp(fileinfo.fname,".."))
//			{
//					printf ("0==strcmp(fileinfo.fname). two");
//					continue ;//读到的文件名为上一级文件
//			}	
//		
//	    if(fileinfo.fattrib & AM_DIR)
//			{
//				res = mf_deletedir(path);
//				
//				printf("digui：%d\r\n",i++);
//			}
//			else
//			{
//					res = f_unlink(files);
//				if(res==FR_OK)printf("delete ok");
//			}
//			
//		
//			
//	}
//	return res;	
//}


//u8 FS_DeleteFolderFile(u8* path)
//{
//	FRESULT res;
//	//删除此文件夹以下的所有文件
//	res = mf_deletedir(path);
//	if(res==FR_OK)
//	{
//		res=f_unlink((const TCHAR*)path);//删除文件夹
//	}
//	else if(FR_NO_PATH==res)
//	{
//		res = f_unlink((const TCHAR*)path);
//	}
//	return res;
//}


//u8 fileanddir_delete(u8* path)
//{
//	FRESULT res;//定义结果对象
////	DIR dir;//定义目的对象
////	FILINFO fno;//定义静态文件信息结构对象
//	unsigned int i;
//	char pathBuff[256];//定义路径数组
//	res=f_opendir(&dir,(const TCHAR*)path);
//	if(res == FR_OK) //打开成功
//	{
//		for(;;) //遍历
//		{
//			res = f_readdir(&dir,&fileinfo);//读取目录，返回状态和文件信息的指针
//			if(res !=FR_OK || fileinfo.fname[0] == 0)
//				break;//若打开失败或到结尾，则退出
//			if(fileinfo.fattrib & AM_DIR)//是目录
//			{
//				i=strlen((const char*)path);
//				sprintf(&path[i],"/%s",fileinfo.fname);//将新目录添加到原目录
//				printf("是目录::%s",path);
//				res=f_unlink((const char*)path);
//				{
//					if(res ==FR_OK)
//					{
//						printf("删除目录Dir1成功\r\n");
//					}
//					else if(res==FR_NO_FILE)
//					{
//							printf("没有发现文件或目录 :%s\r\n", path);
//					}
//					else
//					{
//						printf("删除Dir1失败(错误代码 = %d) 文件只读或目录非空\r\n", res);
//					}
//				}
//				res = fileanddir_delete(path);
//			}
//			else//是文件
//			{
//					res=f_unlink((const char*)path);
//			}
//			
//		}
//	}
//	
//	
//}




