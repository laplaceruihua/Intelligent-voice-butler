#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#include "main.h"




/*************端口信息********************
 * 接线说明
 * RST      PA1
 * CS   	  PA4
 * WR/SPIS  PC4
 * P2/SDCK  PA5
 * P1/SDO   PA6
 * P0/SDI   PA7
 * IRQ      PC5
 * CLK  与单片机的晶振共用

*****************************************/
#define LD_RST_H() GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define LD_RST_L() GPIO_ResetBits(GPIOA, GPIO_Pin_1)

#define LD_CS_H()	GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LD_CS_L()	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define LD_SPIS_H()  GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define LD_SPIS_L()  GPIO_ResetBits(GPIOC, GPIO_Pin_4)


/****************************************************************
函数名： LD_WriteReg
功能：写LD3320芯片的寄存器
参数：  address, 8位无符号整数，地址
		dataout，8位无符号整数，要写入的数据
返回值：无
****************************************************************/ 
void LD_WriteReg( unsigned char address, unsigned char dataout );

/****************************************************************
函数名： LD_ReadReg
功能：读LD3320芯片的寄存器
参数：  address, 8位无符号整数，地址
返回值：8位无符号整数，读取的结果
****************************************************************/ 
unsigned char LD_ReadReg( unsigned char address );






void LD3320_GPIO_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_EXTI_Cfg(void);
static uint8 spi_send_byte(uint8 byte);

void Delay_us(unsigned long i);
 void  LD3320_delay(unsigned long uldata);




#endif

