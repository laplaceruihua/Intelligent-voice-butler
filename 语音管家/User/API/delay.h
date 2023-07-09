#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"

extern vu32 Run2Time ;
extern vu32 sysTiming ;

void Delay_init(void);
void Delay_ms (uint32_t ms);


#endif
