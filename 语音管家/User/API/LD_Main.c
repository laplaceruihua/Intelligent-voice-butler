#include "LD_Main.h"

uint8 nAsrStatus=0;	

//语音播放
__mp3Ctrl mp3Ctrl = {0};
__VoiceCtrl voiceCtrl = {0};



/***********************************************************
* 名    称：LD3320_Init(void)
* 功    能：模块驱动端口初始配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Init(void)
{

	LD3320_GPIO_Cfg();	
	LD3320_Spi_cfg();	 
  LD_reset();
	LD3320_EXTI_Cfg();

}



/***********************************************************
* 名    称：	RunASR(void)
* 功    能：	运行ASR(启动)
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			#if debug
				printstrCom((u8*)"add key word failed!reset....\n");
			#endif
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		#if debug
				printstrCom((u8*)"add keywords success!\n");
		//LD_reset();
			#endif

		LD3320_delay(10);

		if (LD_AsrRun() == 0)
		{
			#if debug
				printstrCom((u8*)"internal err!reset....\n");
			#endif
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}

	return asrflag;
}

uint8 nAsrRes=0;

void recognizeLoop()
{
    
	  switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
					
												break;
			case LD_ASR_NONE:
											nAsrStatus=LD_ASR_RUNING;
											if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
											{		
												nAsrStatus = LD_ASR_ERROR;
											}
											break;

			case LD_ASR_FOUNDOK:
											  nAsrRes = LD_GetResult( );	//	一次ASR识别流程结束，去取ASR识别结果										 
												PrintCom(USART1,"\r\n识别码:");			 /*text.....*/
								        USART_SendData(USART1,nAsrRes+0x30); /*text.....*/		
//												if(voiceCtrl.start == 0 && nAsrRes != CODE_HELLO) goto notstart;  	
												 switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
												  {
													  case CODE_LSD:			/*命令“流水灯”*/
															ld3320PlayMp3("liu.mp3");
															PrintCom(USART1,"“流水灯”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_SS:	 /*命令“闪烁”*/
															ld3320PlayMp3("shan.mp3");

															PrintCom(USART1,"“闪烁”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_QL:		/*命令“按键触发”*/
															ld3320PlayMp3("ok.mp3");

														
															PrintCom(USART1,"“按键触发”命令识别成功\r\n"); /*text.....*/
																						break;
														case CODE_QM:		/*命令“全灭”*/
															ld3320PlayMp3("quan.mp3");

															PrintCom(USART1,"“全灭”命令识别成功\r\n"); /*text.....*/
																						break;
														case CODE_QCD:			/*命令“全彩打开”*/
															ld3320PlayMp3("qi.mp3");

															PrintCom(USART1,"“全彩打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_RED:			/*命令“红灯打开”*/
															ld3320PlayMp3("hong.mp3");

															PrintCom(USART1,"“红灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_GREEN:			/*命令“绿灯打开”*/
															ld3320PlayMp3("lv.mp3");

															PrintCom(USART1,"“绿灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_BLUE:			/*命令“蓝灯打开”*/
															ld3320PlayMp3("lan.mp3");

															PrintCom(USART1,"“蓝灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_FL:			/*命令“灯珠打开”*/
															ld3320PlayMp3("zhao.mp3");

															PrintCom(USART1,"“灯珠打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_QCDO:			/*命令“全彩关闭”*/
															ld3320PlayMp3("qiguan.mp3");

															PrintCom(USART1,"“全彩关闭”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_FLOFF:			/*命令“灯珠关闭”*/
															ld3320PlayMp3("zhaoguan.mp3");

															PrintCom(USART1,"“灯珠关闭”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_HELLO:

															ld3320PlayMp3("hello.mp3");														
															break;
														case CODE_JOKE:
																ld3320PlayMp3("joke.mp3"); 
															break;
														case CODE_PLAY:
//																Fun_Mode = PlayYinYue;
//										 						ld3320PlayMp3("music.mp3"); 
															break;
														case CODE_KX:
															ld3320PlayMp3("MP33.mp3");
															PrintCom(USART1,"“开心”命令识别成功\r\n"); /*text.....*/

															break;
														default:
															ld3320PlayMp3("recerr.mp3");
														 break;
													}	
notstart:
												nAsrStatus = LD_ASR_NONE;
											break;
			
			case LD_ASR_FOUNDZERO:
			default:
								nAsrStatus = LD_ASR_NONE;
								break;
			}

}


/******************************


语音播放需要的函数接口



*****************************/

void ld3320PlayMp3(char *path)
{
		LD_Init_MP3();
		LD_AdjustMIX2SPVolume(15);
	  memset(mp3Ctrl.mp3Path,0,sizeof(mp3Ctrl.mp3Path));
	  sprintf(mp3Ctrl.mp3Path,"%s",path);
	  mp3Ctrl.monitor = sysTiming;
		mp3Play(path);
	  voiceCtrl.monitor = sysTiming;
}



void mp3Play(char *path)
{
	u8 temp;
	mp3Ctrl.nMp3Pos = 0;
	mp3Ctrl.bMp3Play = 1;
	mp3Ctrl.bMp3ReadOver = 0;
	mp3Ctrl.nBufpos = 0;
	mp3Ctrl.reLoadData = LD_ReloadMp3Data;
	temp = LD_ReadReg(0x1B);
	LD_WriteReg(0x1B, temp | 0x80);
	
	mp3Ctrl.reLoadData();

	LD_WriteReg(0xBA, 0x00); 
	LD_WriteReg(0x17, 0x48); 
	LD_WriteReg(0x33, 0x01); 
	LD_WriteReg(0x29, 0x04); 
  
	LD_WriteReg(0x02, 0x01);  
	LD_WriteReg(0x85, 0x5A); 
}





#define NUMREADONCE 50
int LD_ReloadMp3Data()
{
	
	u8 ucStatus;	
	FIL f_file;
	UINT br;
	char buf[10] = {'\0'};
	u8 val[NUMREADONCE],i;
  if(FR_OK == f_open(&f_file,mp3Ctrl.mp3Path,FA_READ))
	{
		  if(mp3Ctrl.nMp3Pos == 0)   mp3Ctrl.nMp3Size = f_size(&f_file);
		  f_lseek(&f_file,mp3Ctrl.nMp3Pos);
			ucStatus = LD_ReadReg(0x06);
			while ( !(ucStatus & MASK_FIFO_STATUS_AFULL) )
			{
				
					if(FR_OK == f_read(&f_file,val,NUMREADONCE,&br))
					{
						  if(br == 0 || mp3Ctrl.nMp3Pos >= mp3Ctrl.nMp3Size)  break;

							mp3Ctrl.monitor = sysTiming;
						  for(i =0; i < br;i ++ )
							{
								LD_WriteReg(0x01,val[i]);
								mp3Ctrl.nMp3Pos++;
								ucStatus = LD_ReadReg(0x06);
								if(ucStatus & MASK_FIFO_STATUS_AFULL) break;
							}
							
					}
          else 
					{
						#if debug
						  PrintCom(USART1,"SD card read err!\r\n"); /*text.....*/
						#endif
					   break;	
					}				
			}
			
		nLD_Mode = LD_MODE_ASR_RUN;
		nAsrStatus = LD_ASR_NONE;
		f_close(&f_file);	
	}
	#if debug
	else{
	  PrintCom(USART1,"mp3 file open err!\r\n"); /*text.....*/
	}
						 
	#endif
}










