#include "LD_API.h"
/***************************乐声电子科技有限公司****************************
**  文件名称：语音模块底层驱动
**	CPU: STM32f103RCT6
**	晶振：8MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.4语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.11
**  说明：本程序 具备语音识别、串口通信、开发板基本功能演示。
***************************乐声电子科技有限公司******************************/

#include "main.h"


uint8 nLD_Mode = LD_MODE_IDLE;		//	用来记录当前是在进行ASR识别还是在播放MP3

uint8 ucRegVal;



/***********************************************************
* 名    称：void LD_reset(void)
* 功    能：LD芯片硬件初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_reset(void)
{
	LD_RST_H();
	LD3320_delay(100);
	LD_RST_L();
	LD3320_delay(100);
	LD_RST_H();
	LD3320_delay(100);
	LD_CS_L();
	LD3320_delay(100);
	LD_CS_H();		
	LD3320_delay(100);
}

/***********************************************************
* 名    称： void LD_Init_Common(void)
* 功    能： 初始化命令
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_Init_Common(void)
{

	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	LD3320_delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	LD3320_delay(5);
	LD_WriteReg(0xCF, 0x43);   
	LD3320_delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	    LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	LD3320_delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	LD3320_delay(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

/***********************************************************
* 名    称：void LD_Init_ASR(void)
* 功    能：初始化ASR
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 	
void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	LD3320_delay( 5 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	LD3320_delay( 1 );
}
#define delay LD3320_delay
void LD_Init_MP3()
{
	nLD_Mode = LD_MODE_MP3;
	LD_Init_Common();
	LD_WriteReg(0xBD,0x02); 
  LD_WriteReg(0x17, 0x48); 
  delay(10);
	LD_WriteReg(0x85,0x52); 
	LD_WriteReg(0x8F,0x00); 
	LD_WriteReg(0x81,0x00); 
	LD_WriteReg(0x83,0x00); 
	LD_WriteReg(0x8E,0xff); 
	LD_WriteReg(0x8D,0xff); 
	delay(1); 
	LD_WriteReg(0x87,0xff); 
	LD_WriteReg(0x89,0xff); 
	delay(1); 
	LD_WriteReg(0x22,0x00); 
	LD_WriteReg(0x23,0x00); 
	LD_WriteReg(0x20,0xef); 
	LD_WriteReg(0x21,0x07); 
	LD_WriteReg(0x24, 0x77); 
	LD_WriteReg(0x25,0x03); 
	LD_WriteReg(0x26, 0xbb);    
	LD_WriteReg(0x27,0x01);  
 
}
/*******************************************
音量分为16级，用4位二进制表示  0-15

设置音量时把 15-音量值  写入8e寄存器的2-5位

*********************************************/
void LD_AdjustMIX2SPVolume(uint8 val) 
{ 
  mp3Ctrl.ucSPVol = val;
	val = ((15-val)&0x0f) << 2;
	LD_WriteReg(0x8E, val | 0xc3); 
	LD_WriteReg(0x87, 0x78); 
}


/***********************************************************
* 名    称：void ProcessInt0(void)
* 功    能：识别处理函数
* 入口参数：  
* 出口参数：
* 说    明：可以利用查询方式，或中断方式进行调用
* 调用方法： 
**********************************************************/ 

void ProcessInt0(void)
{
	uint8 nAsrResCount=0,ucRegVal,ucHighInt,ucLowInt;
	ucRegVal = LD_ReadReg(0x2B);

		// 语音识别产生的中断
			// （有声音输入，不论识别成功或失败都有中断）
	    
	
	 if(nLD_Mode == LD_MODE_MP3) 
	 {  
		  ucRegVal = LD_ReadReg(0x2B); 
      ucHighInt=LD_ReadReg(0x29);
		  ucLowInt=LD_ReadReg(0x02);
		  LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;
			if(LD_ReadReg(0xBA)&CAUSE_MP3_SONG_END) 
			{ 
				 LD_WriteReg(0x2B,  0); 
				 LD_WriteReg(0xBA, 0x00); 
				 LD_WriteReg(0xBC,0x00); 
				 nLD_Mode = LD_MODE_ASR_RUN;
				 nAsrStatus = LD_ASR_NONE;
				 LD_WriteReg(0x08,0x01);
				 delay(5);
				 LD_WriteReg(0x08,0x00); 
				 LD_WriteReg(0x33,0x00);
//         printstrCom("111");					
				 return ; 
				} 
				 if(mp3Ctrl.bMp3ReadOver && mp3Ctrl.nBufpos >= mp3Ctrl.nBufEffectiveSize) 
				 if(mp3Ctrl.nMp3Pos >= mp3Ctrl.nMp3Size)
				 { 
					 nLD_Mode = LD_MODE_ASR_RUN;
				   nAsrStatus = LD_ASR_NONE;
					 LD_WriteReg(0xBC, 0x01); 
					 LD_WriteReg(0x29, 0x10); 
					 printf("222");
					 return; 
				 }  
				 LD_ReloadMp3Data(); 
				 LD_WriteReg(0x29,ucHighInt); 
				 LD_WriteReg(0x02,ucLowInt) ; 
		} 
    else
		{
			LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;
			if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
				{	 
						nAsrResCount = LD_ReadReg(0xba);
			
						if(nAsrResCount>0 && nAsrResCount<=4) 
						{
							nAsrStatus=LD_ASR_FOUNDOK; 				
						}
						else
					    {
							nAsrStatus=LD_ASR_FOUNDZERO;
							}	
				}
			else
			 {
					nAsrStatus=LD_ASR_FOUNDZERO;	//执行没有识别
				}

		  LD_WriteReg(0x2b, 0);
		  LD_WriteReg(0x1C,0);/*写0:ADC不可用*/
		
			LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;
			LD_WriteReg(0x2B,  0);
			LD_WriteReg(0xBA, 0);	
			LD_WriteReg(0xBC,0);	
			LD_WriteReg(0x08,1);	 /*清除FIFO_DATA*/
			LD_WriteReg(0x08,0);	/*清除FIFO_DATA后 再次写0*/

			//LD3320_delay(1);
	 }
}

/***********************************************************
* 名    称：uint8 LD_Check_ASRBusyFlag_b2(void)
* 功    能：检测 ASR 是否忙状态
* 入口参数：flag ---1：空闲状态  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		LD3320_delay(10);		
	}
	#if debug
	if(!flag)
			printstrCom((u8*)"dsp bussy,\n");
	#endif
	return flag;
}
/***********************************************************
* 名    称： void LD_AsrStart(void)
* 功    能：
* 入口参数：  ASR初始化
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/***********************************************************
* 名    称： uint8 LD_AsrRun(void)
* 功    能： ASR执行函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	LD3320_delay( 5 );
	LD_WriteReg(0x08, 0x00);
	LD3320_delay( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	LD_WriteReg(0x37, 0x06);
	LD3320_delay( 5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	
	LD_WriteReg(0xBD, 0x00);   
	return 1;
}

/***********************************************************
* 名    称：uint8 LD_AsrAddFixed(void)
* 功    能：添加识别关键词语
* 入口参数：  
* 出口参数： flag-- 1：success
* 说    明：开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
						关于垃圾词语吸收错误的用法，提高识别率。
* 调用方法： 
**********************************************************/ 
uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 20   /*数组二维数值*/
	#define DATE_B 20		/*数组一维数值*/
	 uint8  sRecog[DATE_A][DATE_B] = {
	 																			"liu shui deng",\
																				"shan shuo",\
																				"quan liang",\
																				 "quan mie",\
																					"kai qi cai deng",\
																					"hong deng",\
																					"lv deng",\
																					"lan deng",\
																					"kai zhao ming deng",\
																					"guan qi cai deng",\
																					"guan zhao ming deng",\
																					"xiao zhi ni hao",\
		                                      "jiang ge xiao hua",\
		                                      "bo fang yin yue",\
		                                      "lv",\
		                                      "ni",\
		                                       "lan",\
		                                       "kai",\
		                                       "kai deng","kai xin"
																				};	/*添加关键词，用户修改*/
	 uint8  pCode[DATE_A] = {
	 															CODE_LSD,\
																CODE_SS,\
																 CODE_QL,\
																  CODE_QM,\
																	CODE_QCD,\
																	CODE_RED,\
																	CODE_GREEN,\
																	CODE_BLUE,\
																	CODE_FL,\
																	CODE_QCDO,\
																	CODE_FLOFF,\
																	CODE_HELLO,\
		                              CODE_JOKE,\
		                              CODE_PLAY,\
																	CODE_ERR,\
		                              CODE_ERR,\
		 		                          CODE_ERR,\
		  		                        CODE_ERR,\
		  		                        CODE_ERR,\
																	CODE_KX
															};	/*添加识别码，用户修改*/
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;

			break;
		}
		
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		LD3320_delay(1);
		LD_WriteReg(0x08, 0x00);
		LD3320_delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
//		LD_WriteReg(0x37, 0x04);
	}	 

    return flag;
}

/***********************************************************
* 名    称： uint8 LD_GetResult(void)
* 功    能： 获取识别结果
* 入口参数：  
* 出口参数： 返回识别码 LD_ReadReg(0xc5 );
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5 );
}




