#include "usart.h"
#include "stm32f10x.h"


/***********************************************************
* 名    称： void  USART_Configuration(void)
* 功    能： 串口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void  USART_Configuration(void)
{
	float usartdiv;
	u32 div_m,div_f;
	RCC->APB2ENR |= (0X1<<2);//打开A时钟
	RCC->APB2ENR |= (0X1<<14);//串口1时钟
	GPIOA->CRH &=~(0xff<<4);//清零
	GPIOA->CRH |=(0xb<<4);//浮空输入
	GPIOA->CRH |=(0x4<<8);//复用推挽输出
	

	USART1->CR1 |= (0x3<<2);//使能接收发送

	usartdiv=72*1000000/(16*9600);//得到波特率的计算公式中的除数
	div_m=(u32)usartdiv;
	div_f = (u32)((usartdiv-div_m)*16);
	USART1->BRR = (div_m<<4)|div_f;
	USART1->CR1 |= (0x1<<13);//使能串口

}
/***********************************************************
* 名    称：void PrintCom(USART_TypeDef* USARTx, uint8_t *Data)
* 功    能：串口数据打印
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void PrintCom(USART_TypeDef* USARTx, uint8_t *Data)
{ 
	
		while(*Data)
		{
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);/* 检查指定的USART标志位即RESET＝1时发送完成*/
			USART_SendData(USARTx, *Data++);    /*发送单个数据 */
			
		}													
}

int fputc(int ch,FILE *p)
{
	//等待数据寄存器为空
	while((USART1->SR &(1<<7)) == 0);
	USART1->DR = ch;
	return ch;
}


void RCC_Configuration(void)
{
  SystemInit();  
  /* Enable USART1, GPIOA, GPIOx and AFIO clocks */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE); //开启相关的AHP外设时钟
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
