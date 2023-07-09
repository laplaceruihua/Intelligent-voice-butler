/***************************乐声电子科技有限公司****************************
**  工程名称：YS-V0.4语音识别开发板程序
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
#include "stm32f10x.h"


/*************端口信息********************
 * 接线说明
 * RST      PB6
 * CS   	PB8
 * WR/SPIS  PC4
 * P2/SDCK  PB3
 * P1/SDO   PB4
 * P0/SDI   PB5
 * IRQ      PC1
 * A0				PB7
 * RD       PA0
*****************************************/



/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/
uint8 nAsrStatus=0;	

void LD3320_Init(void);
void ld3320PlayMp3(char *path);
void mp3Loop(char *path);
uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void recognizeLoop(void);

void mp3Play(char *path);
__mp3Ctrl mp3Ctrl = {0};
__VoiceCtrl voiceCtrl = {0};
/***********************************************************
* 名    称： LD3320_main(void)
* 功    能： 主函数LD3320程序入口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8_t ledStatus;
uint8_t lanternStatus = LANTERN_OFF;
uint8_t ld3320Mode = LD3320_REC;
uint8 nAsrRes=0;
//char mp3Name[20] = {0};

uint8_t Fun_Mode = All_Off;

void  LD3320_main(void)
{
	LD3320_Init();	
	LED_gpio_cfg();
	Beep_Config();
  KEY_Config();
	Lantern_Config();
	Flashlight_Config();
	sd_init();
	Lcd_Init();
	
	PrintCom(USART1," 串口运行中....\r\n"); /*text.....*/
	PrintCom(USART1,"口令：1、流水灯\r\n"); /*text.....*/
	PrintCom(USART1,"2、闪烁\r\n"); /*text.....*/
	PrintCom(USART1,"3、全亮\r\n"); /*text.....*/
	PrintCom(USART1,"4、全灭\r\n"); /*text.....*/
	PrintCom(USART1,"5、开七彩灯\r\n"); /*text.....*/
	PrintCom(USART1,"6、红灯\r\n"); /*text.....*/
	PrintCom(USART1,"7、绿灯\r\n"); /*text.....*/
	PrintCom(USART1,"8、蓝灯\r\n"); /*text.....*/
	PrintCom(USART1,"9、关七彩灯\r\n"); /*text.....*/
	PrintCom(USART1,"10、开照明灯\r\n"); /*text.....*/
	PrintCom(USART1,"11、关照明灯\r\n"); /*text.....*/
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
  nLD_Mode = LD_MODE_ASR_RUN;

	while(1)
	{
		if(nLD_Mode == LD_MODE_ASR_RUN) 
		{
		   if(sysTiming - voiceCtrl.monitor > 1000)
			 {
			    voiceCtrl.start = 0;
			 }
			 recognizeLoop();
		}
		else
		{
		   if(sysTiming - mp3Ctrl.monitor > 200)
			 {
			    nLD_Mode = LD_MODE_ASR_RUN;
					nAsrStatus = LD_ASR_NONE;
			 }
		}
		ledCtrl(ledStatus);
		if(BootRun[0] > BootRun[1])
		{
			BootRun[0] = 0;
			BootTask();
		}
		if(BootRun[1] == 0xffffffff)
		{
			Ui_Task();
		}

	}

}

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
	#if debug
	volatile u8 a = 1,b = 2,c = 3;
	u8 buf[100];

	#endif	
	LD3320_GPIO_Cfg();	
	LD3320_Spi_cfg();	 
  LD_reset();
	LD3320_EXTI_Cfg();
	
	#if debug
	#include "string.h"
	#include "stdio.h"
	printstrCom((u8*)"read the register of LD3320：0x87、0x80、0xff\n");
	a = LD_ReadReg(0x6); 
	a = LD_ReadReg(0x6); 
	b = LD_ReadReg(0x35);
	c = LD_ReadReg(0xb3);
	
	memset(buf,0,sizeof(buf));
	sprintf((char *)buf,"a:0x%0X,b:0x%0X,c:0x%0X\n",a,b,c);
	printstrCom(buf);
	
	printstrCom((u8*)"write some data to LD3320,0x33、0x35、0x38\n");
	LD_WriteReg(0x35, 0x33); 
  LD_WriteReg(0x1b, 0x35); 
  LD_WriteReg(0xb3, 0x38);

	a = LD_ReadReg(0x35);
	b = LD_ReadReg(0x1b);
	c = LD_ReadReg(0xb3);
  printstrCom((u8*)"read the data whote previous,should be：0x33、0x35、0x38\n");
	
	
	memset(buf,0,sizeof(buf));
	sprintf((char *)buf,"a:0x%0X,b:0x%0X,c:0x%0X\n",a,b,c);
	printstrCom(buf);
	printstrCom((u8*)"please check the device\n");
#endif	
}


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



/***********************************************************
* 名    称： void Delay_( int i)
* 功    能： 短延时
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Delay_us(unsigned long i)
 {     
    while( i--)
       {	
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();

        }
 }	
/***********************************************************
* 名    称：	LD3320_delay(unsigned long uldata)
* 功    能：	长延时函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
		Delay_us(uldata * 1000);
}

/***********************************************************
* 名    称：	RunASR(void)
* 功    能：	运行ASR
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

/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
 
	 	GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO结构体
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);

    /* 配置PA1,PA4为推挽输出，PA1作为复位引脚,PA4作为片选引脚 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	  //PC4----SPIS
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

//  	/* 配置SPI引脚: SCK,MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_Init(GPIOA,&GPIO_InitStructure);		
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	   //使能SPI3外设时钟


	LD_CS_H();
	LD_SPIS_L();
//	
//	//spi功能配置
	SPI_Cmd(SPI1, DISABLE);
	/* SPI3 配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7
	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_InitStructure);
	/* 使能SPI3 */
	SPI_Cmd(SPI1, ENABLE);
	
}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void) 
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //中断引脚配置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line5);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //默认拉高中断引脚

	EXTI_ClearFlag(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line5);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* 名    称：  EXTI1_IRQHandler(void)
* 功    能： 外部中断函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET ) 
	{
 		ProcessInt0(); 
		EXTI_ClearFlag(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}


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
												if(voiceCtrl.start == 0 && nAsrRes != CODE_HELLO) goto notstart;  	
												 switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
												  {
													  case CODE_LSD:			/*命令“流水灯”*/
															ld3320PlayMp3("liu.mp3");
												
															ledStatus = LED_GLID;
															Fun_Mode = Liushui;
															PrintCom(USART1,"“流水灯”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_SS:	 /*命令“闪烁”*/
															ld3320PlayMp3("shan.mp3");
															ledStatus = LED_FLASH;
															Fun_Mode = ShanShuo;
															PrintCom(USART1,"“闪烁”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_QL:		/*命令“按键触发”*/
															ld3320PlayMp3("ok.mp3");
															On_LED();
														  ledStatus = LED_ALLON;
															Fun_Mode = QuanLiang;
															PrintCom(USART1,"“按键触发”命令识别成功\r\n"); /*text.....*/
																						break;
														case CODE_QM:		/*命令“全灭”*/
															ld3320PlayMp3("quan.mp3");
															Off_LED();
														  RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(0);
														  PWM_LED(0);
														  lanternStatus = LANTERN_OFF;
												      ledStatus = LED_ALLOFF;
															Fun_Mode = QuanMie;
															PrintCom(USART1,"“全灭”命令识别成功\r\n"); /*text.....*/
																						break;
														case CODE_QCD:			/*命令“全彩打开”*/
//															ld3320PlayMp3("qi.mp3");
															RED_CTL(1);
															GREEN_CTL(1);
															BLUE_CTL(1);
														  lanternStatus = LANTERN_ON;
															Fun_Mode = KaiQiCai;
															PrintCom(USART1,"“全彩打开”命令识别成功\r\n"); /*text.....*/
														ld3320PlayMp3("qi.mp3");
																						 break;
														case CODE_RED:			/*命令“红灯打开”*/
															ld3320PlayMp3("hong.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_RED;
															RED_CTL(1);
															GREEN_CTL(0);
															BLUE_CTL(0);
															Fun_Mode = HongDeng;
															PrintCom(USART1,"“红灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_GREEN:			/*命令“绿灯打开”*/
															ld3320PlayMp3("lv.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_GREEN;
															RED_CTL(0);
															GREEN_CTL(1);
															BLUE_CTL(0);
														Fun_Mode = LvDeng;
															PrintCom(USART1,"“绿灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_BLUE:			/*命令“蓝灯打开”*/
															ld3320PlayMp3("lan.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_BLUE;
															RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(1);
														  Fun_Mode = LanDeng;
															PrintCom(USART1,"“蓝灯打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_FL:			/*命令“灯珠打开”*/
															ld3320PlayMp3("zhao.mp3");
															PWM_LED(500);
															Fun_Mode = KaiZhaoMing;
															PrintCom(USART1,"“灯珠打开”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_QCDO:			/*命令“全彩关闭”*/
															ld3320PlayMp3("qiguan.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_OFF;
															RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(0);
														Fun_Mode = GuanQiCai;
															PrintCom(USART1,"“全彩关闭”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_FLOFF:			/*命令“灯珠关闭”*/
															ld3320PlayMp3("zhaoguan.mp3");
															PWM_LED(0);
														Fun_Mode = GuanZhaoMing;
															PrintCom(USART1,"“灯珠关闭”命令识别成功\r\n"); /*text.....*/
																						 break;
														case CODE_HELLO:
															voiceCtrl.start = 1;
														  voiceCtrl.monitor = sysTiming;
															ld3320PlayMp3("hello.mp3");														
															break;
														case CODE_JOKE:
																ld3320PlayMp3("joke.mp3"); 
															break;
														case CODE_PLAY:
																Fun_Mode = PlayYinYue;
										 						ld3320PlayMp3("music.mp3"); 
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


