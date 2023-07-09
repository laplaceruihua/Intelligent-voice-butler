#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#include "main.h"




/*************�˿���Ϣ********************
 * ����˵��
 * RST      PA1
 * CS   	  PA4
 * WR/SPIS  PC4
 * P2/SDCK  PA5
 * P1/SDO   PA6
 * P0/SDI   PA7
 * IRQ      PC5
 * CLK  �뵥Ƭ���ľ�����

*****************************************/
#define LD_RST_H() GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define LD_RST_L() GPIO_ResetBits(GPIOA, GPIO_Pin_1)

#define LD_CS_H()	GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LD_CS_L()	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define LD_SPIS_H()  GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define LD_SPIS_L()  GPIO_ResetBits(GPIOC, GPIO_Pin_4)


/****************************************************************
�������� LD_WriteReg
���ܣ�дLD3320оƬ�ļĴ���
������  address, 8λ�޷�����������ַ
		dataout��8λ�޷���������Ҫд�������
����ֵ����
****************************************************************/ 
void LD_WriteReg( unsigned char address, unsigned char dataout );

/****************************************************************
�������� LD_ReadReg
���ܣ���LD3320оƬ�ļĴ���
������  address, 8λ�޷�����������ַ
����ֵ��8λ�޷�����������ȡ�Ľ��
****************************************************************/ 
unsigned char LD_ReadReg( unsigned char address );






void LD3320_GPIO_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_EXTI_Cfg(void);
static uint8 spi_send_byte(uint8 byte);

void Delay_us(unsigned long i);
 void  LD3320_delay(unsigned long uldata);




#endif

