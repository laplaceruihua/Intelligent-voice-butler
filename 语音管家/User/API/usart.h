#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"
#include "stm32f10x.h"

void  USART_Configuration(void);
void PrintCom(USART_TypeDef* USARTx, uint8_t *Data);
void RCC_Configuration(void);
#endif

