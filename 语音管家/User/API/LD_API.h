#ifndef _LD_API_H_
#define _LD_API_H_

#include "main.h"

/************************************************************************************
**	��Ȩ���У�Copyright (c) 2005 - 2010 ICRoute INC.
** Ӳ��˵����STM32f103vet6  8MHZ  �ڲ�ʱ��Ϊ72MHZ 
** 					 LD3320 ��Դ���� 12MHZ  ��ͬ�ľ������޸�LDChip.h �еĶ���ֵ����define CLK_IN  12��
** ���˵�������ж�д�Ĵ�����ʽ�������������ʽ���޸�Reg_RW.c �ļ����ɣ�
** �޸���:  http://shop64790118.taobao.com/
** ʱ�䣺2011.10.15
** ˵�����������޸��� ICRoute��˾ ���׵�Դ���򣬰�Ȩ�������С�
************************************************************************************/
#define MP3BUFSZIE 256

typedef __packed struct 
{
   u32 nMp3Pos; 
	 u32 nMp3Size; //�ļ���С
	 u32 monitor;
	 u16 nBufpos;
	 u16 nBufEffectiveSize;
	 u8 bMp3Play;
	 u8 ucSPVol;
	 u8 bMp3ReadOver;
	 char mp3Path[20];
	 u8 databuf[MP3BUFSZIE];
	 int (*reLoadData)(void);

}__mp3Ctrl;
extern __mp3Ctrl mp3Ctrl;

typedef __packed struct 
{
   u8 start;
   u32 monitor;

}__VoiceCtrl;
extern __VoiceCtrl voiceCtrl;







extern uint8  nAsrStatus;

//#define uint8 unsigned char
//#define uint16 unsigned int
//#define uint32 unsigned long

//	��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE		0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 	0x40


//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE				0x00	//	��ʾû������ASRʶ��
#define LD_ASR_RUNING			0x01	//	��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//	��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//	��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 		0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


//#define CLK_IN   	22.1184	/* user need modify this value according to clock in */
#define CLK_IN   	25.000
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

// LD chip fixed values.
#define        RESUM_OF_MUSIC               0x01
#define        CAUSE_MP3_SONG_END           0x20

#define        MASK_INT_SYNC				0x10
#define        MASK_INT_FIFO				0x04
#define    	   MASK_AFIFO_INT				0x01
#define        MASK_FIFO_STATUS_AFULL		0x08



void LD_reset(void);

void LD_Init_Common(void);
void LD_Init_ASR(void);
void LD_Init_MP3(void);
void LD_AdjustMIX2SPVolume(uint8 val) ;
int LD_ReloadMp3Data(void);
char readMp3Once(void);

uint8 LD_ProcessAsr(uint32 RecogAddr);
void LD_AsrStart(void);
uint8 LD_AsrRun(void);
uint8 LD_AsrAddFixed(void);
uint8 LD_GetResult(void);

void LD_ReadMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void LD_WriteMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void ProcessInt0(void);
extern uint8  nLD_Mode;


//ʶ���루�ͻ��޸Ĵ���
#define CODE_LSD	1	 /*��ˮ��*/
#define CODE_SS	  2	 /*��˸*/
#define CODE_QL	3	 /*��������*/
#define CODE_QM	  4	 /*ȫ��*/
#define CODE_QCD  5 /*ȫ�ʴ�*/
#define CODE_RED  6 /*��ƴ�*/
#define CODE_GREEN  7 /*�̵ƴ�*/
#define CODE_BLUE  8 /*���ƴ�*/
#define CODE_FL  9 /*�����*/
#define CODE_QCDO  10 /*ȫ�ʹر�*/
#define CODE_FLOFF  11 /*����ر�*/
#define CODE_HELLO  12 
#define CODE_JOKE   13
#define CODE_PLAY   14
#define CODE_ERR  15 
#define CODE_KX    16

void  LD3320_delay(unsigned long uldata);
#define MIC_VOL 0x43

#endif
