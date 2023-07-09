#ifndef __FATFS_APP_H
#define __FATFS_APP_H
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include  "string.h"
#include "stdlib.h"
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////

//f_typetell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.
#define F_BIN		0X00	//bin�ļ�
#define F_LRC		0X10	//lrc�ļ�
#define F_NES		0X20	//nes�ļ�
#define F_TEXT	0X30	//.txt�ļ�
#define F_C			0X31	//.c�ļ�
#define F_H			0X32    //.h�ļ�
#define F_FLAC	0X4C	//flac�ļ�
#define F_BMP		0X50	//bmp�ļ�
#define F_JPG		0X51	//jpg�ļ�
#define F_JPEG	0X52	//jpeg�ļ�		 
#define F_GIF		0X53	//gif�ļ�  
#define F_MUSIC 0X40  //�����ļ�

void sd_init();
extern u8 f_typetell(u8 *fname);						//ʶ���ļ�����
extern u8 mf_getfree(u8 *drv,u32 *total,u32 *free);	//�õ�������������ʣ������
extern u32 f_fdsize(u8 *fdname);						//�õ��ļ��д�С
extern FRESULT f_scanfiles(char* path,char *strbuf,u8 mode);  //ɨ���ļ���������ļ�����
extern u16 f_getflietnum(char *path,u8 ftpye);                //��ȡ�ļ������ļ�������
extern u8 f_getfileindext(char *path,u16 *flieindextbl,u8 ftpye); //��ȡ�ļ������ļ�����
extern FRESULT f_getfile_spec_data(char *path,u8 *outbuf,u32 offset,u32 fsize);//��ȡ�ļ���ָ��λ��ָ�����ȵ�����

int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);



#endif


