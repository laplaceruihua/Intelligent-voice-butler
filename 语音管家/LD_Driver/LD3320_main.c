/***************************�������ӿƼ����޹�˾****************************
**  �������ƣ�YS-V0.4����ʶ�𿪷������
**	CPU: STM32f103RCT6
**	����8MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-V0.4����ʶ�𿪷���
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2012.4.11
**  ˵���������� �߱�����ʶ�𡢴���ͨ�š����������������ʾ��
***************************�������ӿƼ����޹�˾******************************/

#include "main.h"
#include "stm32f10x.h"


/*************�˿���Ϣ********************
 * ����˵��
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
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
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
* ��    �ƣ� LD3320_main(void)
* ��    �ܣ� ������LD3320�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
	
	PrintCom(USART1," ����������....\r\n"); /*text.....*/
	PrintCom(USART1,"���1����ˮ��\r\n"); /*text.....*/
	PrintCom(USART1,"2����˸\r\n"); /*text.....*/
	PrintCom(USART1,"3��ȫ��\r\n"); /*text.....*/
	PrintCom(USART1,"4��ȫ��\r\n"); /*text.....*/
	PrintCom(USART1,"5�����߲ʵ�\r\n"); /*text.....*/
	PrintCom(USART1,"6�����\r\n"); /*text.....*/
	PrintCom(USART1,"7���̵�\r\n"); /*text.....*/
	PrintCom(USART1,"8������\r\n"); /*text.....*/
	PrintCom(USART1,"9�����߲ʵ�\r\n"); /*text.....*/
	PrintCom(USART1,"10����������\r\n"); /*text.....*/
	PrintCom(USART1,"11����������\r\n"); /*text.....*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
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
* ��    �ƣ�LD3320_Init(void)
* ��    �ܣ�ģ�������˿ڳ�ʼ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
	printstrCom((u8*)"read the register of LD3320��0x87��0x80��0xff\n");
	a = LD_ReadReg(0x6); 
	a = LD_ReadReg(0x6); 
	b = LD_ReadReg(0x35);
	c = LD_ReadReg(0xb3);
	
	memset(buf,0,sizeof(buf));
	sprintf((char *)buf,"a:0x%0X,b:0x%0X,c:0x%0X\n",a,b,c);
	printstrCom(buf);
	
	printstrCom((u8*)"write some data to LD3320,0x33��0x35��0x38\n");
	LD_WriteReg(0x35, 0x33); 
  LD_WriteReg(0x1b, 0x35); 
  LD_WriteReg(0xb3, 0x38);

	a = LD_ReadReg(0x35);
	b = LD_ReadReg(0x1b);
	c = LD_ReadReg(0xb3);
  printstrCom((u8*)"read the data whote previous,should be��0x33��0x35��0x38\n");
	
	
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
* ��    �ƣ� void Delay_( int i)
* ��    �ܣ� ����ʱ
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�	LD3320_delay(unsigned long uldata)
* ��    �ܣ�	����ʱ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
		Delay_us(uldata * 1000);
}

/***********************************************************
* ��    �ƣ�	RunASR(void)
* ��    �ܣ�	����ASR
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

/***********************************************************
* ��    �ƣ�LD3320_GPIO_Cfg(void)
* ��    �ܣ���ʼ����Ҫ�õ���IO��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
 
	 	GPIO_InitTypeDef GPIO_InitStructure; //����GPIO�ṹ��
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);

    /* ����PA1,PA4Ϊ���������PA1��Ϊ��λ����,PA4��ΪƬѡ���� */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	  //PC4----SPIS
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

//  	/* ����SPI����: SCK,MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
    GPIO_Init(GPIOA,&GPIO_InitStructure);		
}
/***********************************************************
* ��    �ƣ�LD3320_Spi_cfg(void)
* ��    �ܣ�����SPI���ܺͶ˿ڳ�ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	   //ʹ��SPI3����ʱ��


	LD_CS_H();
	LD_SPIS_L();
//	
//	//spi��������
	SPI_Cmd(SPI1, DISABLE);
	/* SPI3 ���� */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;   //�����ʿ��� SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7
	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_InitStructure);
	/* ʹ��SPI3 */
	SPI_Cmd(SPI1, ENABLE);
	
}
/***********************************************************
* ��    �ƣ� LD3320_EXTI_Cfg(void) 
* ��    �ܣ� �ⲿ�жϹ������ú���ض˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //�ж���������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//�ⲿ�ж�������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line5);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //Ĭ�������ж�����

	EXTI_ClearFlag(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line5);
	//�ж�Ƕ������
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* ��    �ƣ�  EXTI1_IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
											if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
											{		
												nAsrStatus = LD_ASR_ERROR;
											}
											break;

			case LD_ASR_FOUNDOK:
											  nAsrRes = LD_GetResult( );	//	һ��ASRʶ�����̽�����ȥȡASRʶ����										 
												PrintCom(USART1,"\r\nʶ����:");			 /*text.....*/
								        USART_SendData(USART1,nAsrRes+0x30); /*text.....*/		
												if(voiceCtrl.start == 0 && nAsrRes != CODE_HELLO) goto notstart;  	
												 switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
												  {
													  case CODE_LSD:			/*�����ˮ�ơ�*/
															ld3320PlayMp3("liu.mp3");
												
															ledStatus = LED_GLID;
															Fun_Mode = Liushui;
															PrintCom(USART1,"����ˮ�ơ�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_SS:	 /*�����˸��*/
															ld3320PlayMp3("shan.mp3");
															ledStatus = LED_FLASH;
															Fun_Mode = ShanShuo;
															PrintCom(USART1,"����˸������ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_QL:		/*�������������*/
															ld3320PlayMp3("ok.mp3");
															On_LED();
														  ledStatus = LED_ALLON;
															Fun_Mode = QuanLiang;
															PrintCom(USART1,"����������������ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														case CODE_QM:		/*���ȫ��*/
															ld3320PlayMp3("quan.mp3");
															Off_LED();
														  RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(0);
														  PWM_LED(0);
														  lanternStatus = LANTERN_OFF;
												      ledStatus = LED_ALLOFF;
															Fun_Mode = QuanMie;
															PrintCom(USART1,"��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														case CODE_QCD:			/*���ȫ�ʴ򿪡�*/
//															ld3320PlayMp3("qi.mp3");
															RED_CTL(1);
															GREEN_CTL(1);
															BLUE_CTL(1);
														  lanternStatus = LANTERN_ON;
															Fun_Mode = KaiQiCai;
															PrintCom(USART1,"��ȫ�ʴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
														ld3320PlayMp3("qi.mp3");
																						 break;
														case CODE_RED:			/*�����ƴ򿪡�*/
															ld3320PlayMp3("hong.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_RED;
															RED_CTL(1);
															GREEN_CTL(0);
															BLUE_CTL(0);
															Fun_Mode = HongDeng;
															PrintCom(USART1,"����ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_GREEN:			/*����̵ƴ򿪡�*/
															ld3320PlayMp3("lv.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_GREEN;
															RED_CTL(0);
															GREEN_CTL(1);
															BLUE_CTL(0);
														Fun_Mode = LvDeng;
															PrintCom(USART1,"���̵ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_BLUE:			/*������ƴ򿪡�*/
															ld3320PlayMp3("lan.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_BLUE;
															RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(1);
														  Fun_Mode = LanDeng;
															PrintCom(USART1,"�����ƴ򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_FL:			/*�������򿪡�*/
															ld3320PlayMp3("zhao.mp3");
															PWM_LED(500);
															Fun_Mode = KaiZhaoMing;
															PrintCom(USART1,"������򿪡�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_QCDO:			/*���ȫ�ʹرա�*/
															ld3320PlayMp3("qiguan.mp3");
															if(lanternStatus == LANTERN_OFF) break;
														  lanternStatus = LANTERN_OFF;
															RED_CTL(0);
															GREEN_CTL(0);
															BLUE_CTL(0);
														Fun_Mode = GuanQiCai;
															PrintCom(USART1,"��ȫ�ʹرա�����ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_FLOFF:			/*�������رա�*/
															ld3320PlayMp3("zhaoguan.mp3");
															PWM_LED(0);
														Fun_Mode = GuanZhaoMing;
															PrintCom(USART1,"������رա�����ʶ��ɹ�\r\n"); /*text.....*/
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


