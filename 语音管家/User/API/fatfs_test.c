#include "fatfs_test.h"




void SD_test(void)
{

		FIL fil;
	
		BYTE write_buff[]="实训改变命运 hello1234567890 技能改变中国";
		BYTE read_buff[sizeof(write_buff)];
		u8 res;
		UINT bw;
		UINT br;
	 /*2.调用打开文件函数， 权限， 可读可写， 如果文件存在则覆盖*/
	 res = f_open(&fil, "0:/hello.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	 if(res==FR_OK)
	 {
	 printf("文件创建成功！！ \r\n");
	 }
	 else
	 {
	 printf("文件创建失败！！ \r\n");
	 }
	
	 /*3.写数据*/
	 res =f_write(&fil,write_buff,strlen((const char*)write_buff),&bw);
	 if(res==FR_OK)
	 {
	 printf("成功写入%d 个字节数据！！ \r\n",bw);
	 }
	 else
	 {
	 printf("数据写入失败！！ \r\n");
	 }
	 	res = f_close(&fil);
	if(res==FR_OK)
	{
	printf("成功关闭\r\n");
	} 
	else
	{
	printf("关闭失败\r\n");
	}
	/*4.读取文件,要先打开才能读取*/
	res = f_open(&fil, "0:/hello.txt", FA_READ);
	if(res==FR_OK)
	{
	printf("成功打开文件！！ \r\n");
	}
	else{
	printf("打开文件失败\r\n");
	} 
	res=f_read(&fil,read_buff,1024,&br);
	if(res==FR_OK)
	{
	printf("成功读取%d 个字节数据！！ \r\n",br);
	printf("读到的数据为%s\r\n",read_buff);
	} 
	else
	{
	printf("读取失败\r\n");
	} 
	/*5.关闭文件*/
	res = f_close(&fil);
	if(res==FR_OK)
	{
	printf("成功关闭\r\n");
	} 
	else
	{
	printf("关闭失败\r\n");
	}

}
