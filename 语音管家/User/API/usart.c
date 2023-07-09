#include "usart.h"
#include "stm32f10x.h"


/***********************************************************
* ��    �ƣ� void  USART_Configuration(void)
* ��    �ܣ� ��������
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void  USART_Configuration(void)
{
	float usartdiv;
	u32 div_m,div_f;
	RCC->APB2ENR |= (0X1<<2);//��Aʱ��
	RCC->APB2ENR |= (0X1<<14);//����1ʱ��
	GPIOA->CRH &=~(0xff<<4);//����
	GPIOA->CRH |=(0xb<<4);//��������
	GPIOA->CRH |=(0x4<<8);//�����������
	

	USART1->CR1 |= (0x3<<2);//ʹ�ܽ��շ���

	usartdiv=72*1000000/(16*9600);//�õ������ʵļ��㹫ʽ�еĳ���
	div_m=(u32)usartdiv;
	div_f = (u32)((usartdiv-div_m)*16);
	USART1->BRR = (div_m<<4)|div_f;
	USART1->CR1 |= (0x1<<13);//ʹ�ܴ���

}
/***********************************************************
* ��    �ƣ�void PrintCom(USART_TypeDef* USARTx, uint8_t *Data)
* ��    �ܣ��������ݴ�ӡ
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void PrintCom(USART_TypeDef* USARTx, uint8_t *Data)
{ 
	
		while(*Data)
		{
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);/* ���ָ����USART��־λ��RESET��1ʱ�������*/
			USART_SendData(USARTx, *Data++);    /*���͵������� */
			
		}													
}

int fputc(int ch,FILE *p)
{
	//�ȴ����ݼĴ���Ϊ��
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
                         RCC_APB2Periph_GPIOE, ENABLE); //������ص�AHP����ʱ��
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
