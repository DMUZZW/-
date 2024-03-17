#include "fattester.h"	 

    
		
TCHAR* files; 
TCHAR* lname; 

//Ϊ����ע�Ṥ����	 
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
u8 mf_mount(u8* path,u8 mt)
{		   
	return f_mount(fs[1],(const TCHAR*)path,mt); 
}
//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
u8 mf_open(u8*path,u8 mode)
{
	u8 res;	 
	res=f_open(file,(const TCHAR*)path,mode);//���ļ���
	return res;
} 
//�ر��ļ�
//����ֵ:ִ�н��
u8 mf_close(void)
{
	f_close(file);
	return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
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
		if(res)	//�����ݳ�����
		{
			printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}	 
	}
	
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	printf("Read data over\r\n");	 
	return res;
}

int get_all(u8** content){
	f_read(file,fatbuf,512,&br);
	*content = fatbuf;
	return br;
}

//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
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

//��Ŀ¼
 //path:·��
//����ֵ:ִ�н��
u8 mf_opendir(u8* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//�ر�Ŀ¼ 
//����ֵ:ִ�н��
u8 mf_closedir(void)
{
	return f_closedir(&dir);	
}
//���ȡ�ļ���
//����ֵ:ִ�н��
u8 mf_readdir(void)
{
	u8 res;    
	res=f_readdir(&dir,&fileinfo);	//��ȡһ���ļ�����Ϣ
	if(res!=FR_OK)return res;		//������ 
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

 //�����ļ�
 //path:·��
 //����ֵ:ִ�н��
u8 mf_scan_files(u8 * path)
{
	FRESULT res;	 
    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	{	
		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
	        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
 			printf("%s/", path);//��ӡ·��	
			printf("%s\r\n",fileinfo.fname);//��ӡ�ļ���	  
		} 
    }	   
    return res;	  
}

//�����ļ������ѻ�ȡ���ļ�������filenames��
unsigned char mf_scan(u8* path,char** filenames)
{
	
	FRESULT res;
	char *p1 = NULL;
	char str1[] = ",";
	char json_before[] = "{\"files\":\"";
	char json_end[] = "\"}";
	memset(p1, 0, 1024);
	strcat(p1,json_before);
	res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
	if(res == FR_OK)
	{
//		memset(p2,0,2048);
		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	    if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
			strcat(p1,fileinfo.fname);
			strcat(p1,str1);
		}
		strcat(p1,json_end);
		*filenames = p1;
	}
	mf_closedir();
	return res;	
}
 
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
u32 mf_showfree(u8 *drv)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
	    fre_sect = fre_clust * fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//������С��10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n����������:%d KB\r\n"
		           "���ÿռ�:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n����������:%d MB\r\n"
		           "���ÿռ�:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
u8 mf_lseek(u32 offset)
{
	return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
u32 mf_tell(void)
{
	return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
u32 mf_size(void)
{
	return f_size(file);
} 
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_mkdir(u8*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//path:����·��������"0:"��"1:"
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
u8 mf_fmkfs(u8* path,u8 mode,u16 au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//��ʽ��,drv:�̷�;mode:ģʽ;au:�ش�С
} 
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_unlink(u8 *pname)
{
	return  f_unlink((const TCHAR *)pname);
}


//ɾ���ļ��У������ڲ���ɾ���ļ���ȱ��
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
    res = f_opendir(&mydir,(const TCHAR*)pathname); //��һ��Ŀ¼
		
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
//	//ɾ�����ļ������µ������ļ�
//	res = mf_scan_files_delete((const TCHAR*)path);
//	if(res==FR_OK)
//	{
//		res=f_unlink((const TCHAR*)path);//ɾ���ļ���
//		
//	}
////	else if(FR_NO_PATH==res)
////	{
//		res = f_unlink((const TCHAR*)path);
////	}
//	return res;
//}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
u8 mf_rename(u8 *oldname,u8* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"  
void mf_getlabel(u8 *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		printf("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
		printf("����%s �����к�:%X\r\n\r\n",path,sn); 
	}else printf("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"  
void mf_setlabel(u8 *path)
{
	u8 res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		printf("\r\n�����̷����óɹ�:%s\r\n",path);
	}else printf("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
} 

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void mf_gets(u16 size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//û�����ݶ���
	else
	{
		printf("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}

//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
u8 mf_putc(u8 c)
{
	return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
u8 mf_puts(u8*c)
{
	return f_puts((TCHAR*)c,file);
}


 //�����ļ�
 //path:·��
 //����ֵ:ִ�н��
u8 mfscan_files(u8 * path)
{
	  FRESULT res;	
    unsigned int i;	
    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	 {	
//		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
 	        printf("%s/", path);//��ӡ·��	
			    printf("%s\r\n",fileinfo.fname);//��ӡ�ļ���	
          if(fileinfo.fattrib & AM_DIR)//��Ŀ¼
					{
					  i=strlen(path);
			      sprintf (&path[i],"/%s\r\n",fileinfo.fname);
				    printf("��Ŀ¼::%s\r\n",path);
						if(0 ==strlen(fileinfo.fname))break;
			      if(0==strcmp(fileinfo.fname,"."))continue ;
			      if(0==strcmp(fileinfo.fname,".."))continue ;
						res=mfscan_files(path);
						printf("ִ�е�Ŀ¼\r\n");
						 
					}	
          else//����Ŀ¼
          {
						printf("���ļ�:%s/%s\r\n",path,fileinfo.fname);
					}						
		} 
    }	   
    return res;	  
} 

int CSV_Format(unsigned char ReceiveorSend,unsigned char data[])
{
	//��ȡʱ��
	char FileTimeBuff[20];//�ļ�����
	char RECEIVE[8]={"Receive"};
	char SEND[4]={"Send"};
	unsigned int datalength=0x00;
	char* p;
	 p = mymalloc(SRAMIN,2048);
	HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	sprintf((char*)FileTimeBuff,"Y20%02dM%02dD%02dH%02dM%02dS%02dMS%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //�ϲ�ʱ��	
	if(ReceiveorSend ==0)//��������
	{
		 sprintf(p,"%s,%s,%s\n",FileTimeBuff,SEND,data);
		 printf("send date is:%s\r\n",p);
	}
	else if(ReceiveorSend ==1)//��������
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
	//��������   //1����ȡ��ǰ������ 	//2������һ���ļ���
	
	FRESULT res;	
	char DirTimeBuff[10];//�ļ�����
	char DirPath[20];//�ļ���·��
	char FilePath[25];
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ�����
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ���·��
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //�ļ���
	//����һ���ļ���
	res= f_mkdir(DirTimeBuff);
	res = f_opendir(&dir,(const TCHAR*)DirPath); 
	if(res ==FR_OK)
	{
			//�������ļ�
			res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
			//д����
			if(res ==FR_OK)
       {
				 	//д������
				 res = f_write(file,"Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\n",63,&bw);
			 }
			 	//�ر��ļ�
			f_close(file);
	}
return res;
}

////�ٴ�д����ļ���
u8 SD_Write_Old_File(unsigned char ReceiveorSend, unsigned char buff[])
{
	
	FRESULT res;	
  char DirTimeBuff[10];//�ļ�����
	char DirPath[20];//�ļ���·��
	char FilePath[25];
	char FileTimeBuff[25];//�ļ�����
//	unsigned int datalength=0x00;
	char* p;
  p = mymalloc(SRAMIN,2048);
  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ�����
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ���·��
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //�ļ���
	sprintf((char*)FileTimeBuff,"20%02d:%02d:%02d:%02d:%02d:%02d:%d\r\n",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //�ϲ�ʱ��	
	//���ļ���
  res = f_opendir(&dir,(const TCHAR*)DirPath);
//	printf ("DirPath:%s res=%d\r\n",DirPath,res);	
	//���ļ�
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
		//�����ļ���β
		res= f_lseek(file,f_size(file));	
		//д����
//		sprintf((char*)buff,"%s\r\n",buff);
		res = f_write(file, p, strlen(p), &bw);
//		printf("buff is: %s",p);
//		printf("file %s\r\n",FileName);
		//�ر��ļ�
		f_close(file);
	}
  myfree(SRAMIN,p);//
	return res;
}


////�ۺ�
//u8 SD_Creat_CSVFile(unsigned char ReceiveorSend, unsigned char buff[])
//{
//	FRESULT res;	
//  char DirTimeBuff[10];//�ļ�����
//	char DirPath[20];//�ļ���·��
//	char FilePath[25];
//	char FileTimeBuff[25];//�ļ�����
////	unsigned int datalength=0x00;
//	char* p;
//  p = mymalloc(SRAMIN,2048);
//  HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
//	
//	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ�����
//  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ���·��
//	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.CSV",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //�ļ���
//	sprintf((char*)FileTimeBuff,"20%02d:%02d:%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //�ϲ�ʱ��	
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
//	//����һ���ļ���
//	res= f_mkdir(DirTimeBuff);
//	if(res ==FR_EXIST)//�ļ����Ѿ�����
//	{
//		res = f_opendir(&dir,(const TCHAR*)DirPath); //���ļ���
//		if(res ==FR_OK)
//		{
//				//�������ļ�
//			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
//				if(res ==FR_OK)
//       {
//				 	//д������
//				 res = f_write(file,"Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\n",63,&bw);
//			 }
//			 	//�ر��ļ�
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


////�ۺ�

/*��һ������ ��ʾ�����Ƿ��ͻ��ǽ��ܣ��ڶ�������ʱҪд������� ���ĸ������ǣ��ڼ��β���*/
u8 SD_Creat_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times)
{
	FRESULT res;	
  char DirTimeBuff[10];//�ļ�����
	char DirPath[20];//�ļ���·��
	char FilePath[30];
	char FileTimeBuff[30];//�ļ�����
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;	
	unsigned char temp;
	char* p;
	char* p1;
	
  p = mymalloc(SRAMIN,2048);
	
//	DataInputLength = sizeof(buff);//��������ȷ��ѭ����������0XAD��0XCD��
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
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ�����
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ���·��
	sprintf((char*)FilePath,"%s/Y20%02dM%02dD%02d.csv",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //�ļ���
	sprintf((char*)FileTimeBuff,"20%02d-%02d-%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //�ϲ�ʱ��	
  
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
	//����һ���ļ���
	res= f_mkdir(DirTimeBuff);
	if(res ==FR_EXIST)//�ļ����Ѿ�����
	{
		res = f_opendir(&dir,(const TCHAR*)DirPath); //���ļ���
		if(res ==FR_OK)
		{
				//�������ļ�
			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
				if(res ==FR_OK)
       {
				 	//д������
				 res = f_write(file,",Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\r\n",63,&bw);
			 }
			 	//�ر��ļ�
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




//���ɹ�����ϢCSV�ļ�
u8 SD_Creat_Error_CSVFile(unsigned char ReceiveorSend, unsigned char buff[],unsigned int times)
{
	FRESULT res;	
  char DirTimeBuff[10];//�ļ�����
	char DirPath[20];//�ļ���·��
	char FilePath[30];
	char FileTimeBuff[30];//�ļ�����
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;	
	unsigned char temp;
	char* p;
	char* p1;
  p = mymalloc(SRAMIN,2048);
	
//	DataInputLength = (sizeof((char *)buff));//��������ȷ��ѭ����������0XAD��0XCD��
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
	
	sprintf((char*)DirTimeBuff,"Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ�����
  sprintf((char*)DirPath,"0:/Y20%02dM%02d",RTC_DateStruct.Year,RTC_DateStruct.Month); //�ļ���·��
	sprintf((char*)FilePath,"%s/ERRORY20%02dM%02dD%02d.csv",DirPath,RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); //�ļ���
	sprintf((char*)FileTimeBuff,"20%02d-%02d-%02d:%02d:%02d:%02d:%d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,Timer_Couter); //�ϲ�ʱ��	
  
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
	//����һ���ļ���
	res= f_mkdir(DirTimeBuff);
	if(res ==FR_EXIST)//�ļ����Ѿ�����
	{
		res = f_opendir(&dir,(const TCHAR*)DirPath); //���ļ���
		if(res ==FR_OK)
		{
				//�������ļ�
			 res=f_open(file,FilePath,FA_CREATE_NEW | FA_WRITE);
				if(res ==FR_OK)
       {
				 	//д������
				 res = f_write(file,",Year-Month-Date-Hour-Min-Second-Millisecond,Receive/Send,Data\r\n",63,&bw);
			 }
			 	//�ر��ļ�
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


//ʮ������ת�����ַ���
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
	//��λ�����������ʮ������char������ ��0xAB,��һ���ֽڣ����Ҫ��ʾΪ��AB�� ����2���ֽڣ����һ���ֽڵ�- ��Ҫ�����ֽ�
	unsigned int DataInputLength=0x00,DataOutputLength=0x00,i;
	char* p2;
	unsigned char temp;
	DataInputLength = (sizeof((char *)DataInput));//��������ȷ��ѭ����������0XAD��0XCD��
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







//����ʹ��
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
	 DataInputLength = strlen(data);//��������ȷ��ѭ����������0XAD��0XCD��
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
////	  res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
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
////			if(fileinfo.fattrib & AM_DIR)//������ļ���
////			{
////				
////		    	res =	mf_deletedir(files); //�ݹ���ã�Ҫ���ļ�������ȥ
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
//				break;//�������ļ���Ϊ��
//			}	
//			
//			if(0==strcmp(fileinfo.fname,"."))
//			{
//				printf ("0==strcmp(fileinfo.fname). one");
//				continue ;//�������ļ���Ϊ��ǰ�ļ���
//			}
//			if(0==strcmp(fileinfo.fname,".."))
//			{
//					printf ("0==strcmp(fileinfo.fname). two");
//					continue ;//�������ļ���Ϊ��һ���ļ�
//			}	
//		
//	    if(fileinfo.fattrib & AM_DIR)
//			{
//				res = mf_deletedir(path);
//				
//				printf("digui��%d\r\n",i++);
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
//	//ɾ�����ļ������µ������ļ�
//	res = mf_deletedir(path);
//	if(res==FR_OK)
//	{
//		res=f_unlink((const TCHAR*)path);//ɾ���ļ���
//	}
//	else if(FR_NO_PATH==res)
//	{
//		res = f_unlink((const TCHAR*)path);
//	}
//	return res;
//}


//u8 fileanddir_delete(u8* path)
//{
//	FRESULT res;//����������
////	DIR dir;//����Ŀ�Ķ���
////	FILINFO fno;//���徲̬�ļ���Ϣ�ṹ����
//	unsigned int i;
//	char pathBuff[256];//����·������
//	res=f_opendir(&dir,(const TCHAR*)path);
//	if(res == FR_OK) //�򿪳ɹ�
//	{
//		for(;;) //����
//		{
//			res = f_readdir(&dir,&fileinfo);//��ȡĿ¼������״̬���ļ���Ϣ��ָ��
//			if(res !=FR_OK || fileinfo.fname[0] == 0)
//				break;//����ʧ�ܻ򵽽�β�����˳�
//			if(fileinfo.fattrib & AM_DIR)//��Ŀ¼
//			{
//				i=strlen((const char*)path);
//				sprintf(&path[i],"/%s",fileinfo.fname);//����Ŀ¼��ӵ�ԭĿ¼
//				printf("��Ŀ¼::%s",path);
//				res=f_unlink((const char*)path);
//				{
//					if(res ==FR_OK)
//					{
//						printf("ɾ��Ŀ¼Dir1�ɹ�\r\n");
//					}
//					else if(res==FR_NO_FILE)
//					{
//							printf("û�з����ļ���Ŀ¼ :%s\r\n", path);
//					}
//					else
//					{
//						printf("ɾ��Dir1ʧ��(������� = %d) �ļ�ֻ����Ŀ¼�ǿ�\r\n", res);
//					}
//				}
//				res = fileanddir_delete(path);
//			}
//			else//���ļ�
//			{
//					res=f_unlink((const char*)path);
//			}
//			
//		}
//	}
//	
//	
//}




