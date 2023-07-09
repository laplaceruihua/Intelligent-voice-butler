#include "delay.h"
#include "stm32f10x.h"

vu32 RunTime = 0;
vu32 Run2Time = 0;
vu32 sysTiming =0;


void Delay_init(void)
{
  SysTick_Config(72000-1);
}

void SysTick_Handler(void)
{
  RunTime++;
	sysTiming++;
	Run2Time++;

}
void Delay_ms (uint32_t ms)
{
  uint32_t Tick = RunTime;
  while (Tick + ms > RunTime);
}
