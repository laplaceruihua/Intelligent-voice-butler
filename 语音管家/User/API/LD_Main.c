#include "LD_Main.h"

uint8 nAsrStatus=0;	

//��������
__mp3Ctrl mp3Ctrl = {0};
__VoiceCtrl voiceCtrl = {0};



/***********************************************************
* ��    �ƣ�LD3320_Init(void)
* ��    �ܣ�ģ�������˿ڳ�ʼ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_Init(void)
{

	LD3320_GPIO_Cfg();	
	LD3320_Spi_cfg();	 
  LD_reset();
	LD3320_EXTI_Cfg();

}



/***********************************************************
* ��    �ƣ�	RunASR(void)
* ��    �ܣ�	����ASR(����)
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();			//��ʼ��ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//��ӹؼ����ﵽLD3320оƬ��
		{
			#if debug
				printstrCom((u8*)"add key word failed!reset....\n");
			#endif
			LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
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
			LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}

		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
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
											if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
											{		
												nAsrStatus = LD_ASR_ERROR;
											}
											break;

			case LD_ASR_FOUNDOK:
											  nAsrRes = LD_GetResult( );	//	һ��ASRʶ�����̽�����ȥȡASRʶ����										 
												PrintCom(USART1,"\r\nʶ����:");			 /*text.....*/
								        USART_SendData(USART1,nAsrRes+0x30); /*text.....*/		
//												if(voiceCtrl.start == 0 && nAsrRes != CODE_HELLO) goto notstart;  	
												 switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
												  {
													  case CODE_LSD:			/*�����ˮ�ơ�*/
															ld3320PlayMp3("liu.mp3");
															PrintCom(USART1,"����ˮ�ơ�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_SS:	 /*�����˸��*/
															ld3320PlayMp3("shan.mp3");

															PrintCom(USART1,"����˸������ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_QL:		/*�������������*/
															ld3320PlayMp3("ok.mp3");

														
															PrintCom(USART1,"����������������ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														case CODE_QM:		/*���ȫ��*/
															ld3320PlayMp3("quan.mp3");

															PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														case CODE_QCD:			/*���ȫ�ʴ򿪡�*/
															ld3320PlayMp3("qi.mp3");

															PrintCom(USART1,"��ȫ�ʴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_RED:			/*�����ƴ򿪡�*/
															ld3320PlayMp3("hong.mp3");

															PrintCom(USART1,"����ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_GREEN:			/*����̵ƴ򿪡�*/
															ld3320PlayMp3("lv.mp3");

															PrintCom(USART1,"���̵ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_BLUE:			/*������ƴ򿪡�*/
															ld3320PlayMp3("lan.mp3");

															PrintCom(USART1,"�����ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_FL:			/*�������򿪡�*/
															ld3320PlayMp3("zhao.mp3");

															PrintCom(USART1,"������򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_QCDO:			/*���ȫ�ʹرա�*/
															ld3320PlayMp3("qiguan.mp3");

															PrintCom(USART1,"��ȫ�ʹرա�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_FLOFF:			/*�������رա�*/
															ld3320PlayMp3("zhaoguan.mp3");

															PrintCom(USART1,"������رա�����ʶ��ɹ�\r\n"); /*text.....*/
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
															PrintCom(USART1,"�����ġ�����ʶ��ɹ�\r\n"); /*text.....*/

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


����������Ҫ�ĺ����ӿ�



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










