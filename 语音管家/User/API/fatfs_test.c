#include "fatfs_test.h"




void SD_test(void)
{

		FIL fil;
	
		BYTE write_buff[]="ʵѵ�ı����� hello1234567890 ���ܸı��й�";
		BYTE read_buff[sizeof(write_buff)];
		u8 res;
		UINT bw;
		UINT br;
	 /*2.���ô��ļ������� Ȩ�ޣ� �ɶ���д�� ����ļ������򸲸�*/
	 res = f_open(&fil, "0:/hello.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	 if(res==FR_OK)
	 {
	 printf("�ļ������ɹ����� \r\n");
	 }
	 else
	 {
	 printf("�ļ�����ʧ�ܣ��� \r\n");
	 }
	
	 /*3.д����*/
	 res =f_write(&fil,write_buff,strlen((const char*)write_buff),&bw);
	 if(res==FR_OK)
	 {
	 printf("�ɹ�д��%d ���ֽ����ݣ��� \r\n",bw);
	 }
	 else
	 {
	 printf("����д��ʧ�ܣ��� \r\n");
	 }
	 	res = f_close(&fil);
	if(res==FR_OK)
	{
	printf("�ɹ��ر�\r\n");
	} 
	else
	{
	printf("�ر�ʧ��\r\n");
	}
	/*4.��ȡ�ļ�,Ҫ�ȴ򿪲��ܶ�ȡ*/
	res = f_open(&fil, "0:/hello.txt", FA_READ);
	if(res==FR_OK)
	{
	printf("�ɹ����ļ����� \r\n");
	}
	else{
	printf("���ļ�ʧ��\r\n");
	} 
	res=f_read(&fil,read_buff,1024,&br);
	if(res==FR_OK)
	{
	printf("�ɹ���ȡ%d ���ֽ����ݣ��� \r\n",br);
	printf("����������Ϊ%s\r\n",read_buff);
	} 
	else
	{
	printf("��ȡʧ��\r\n");
	} 
	/*5.�ر��ļ�*/
	res = f_close(&fil);
	if(res==FR_OK)
	{
	printf("�ɹ��ر�\r\n");
	} 
	else
	{
	printf("�ر�ʧ��\r\n");
	}

}
