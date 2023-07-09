#include "main.h"



int main()
{
	int ret=0;
	RCC_Configuration();
	Delay_init();
	LED_gpio_cfg();
	Lantern_Config();
	Flashlight_Config();
	USART_Configuration();
	PrintCom(USART1,"串口初始化成功！\r\n");
//LCD显示屏初始化
	Lcd_Init();
	Gui_box(0,0,50,50,BLACK);
	Gui_DrawFont_GBK16(0,50,BLACK,WHITE,"TSET信盈达河南大");
//	UI_Task();
//语音识别芯片初始化
	LD3320_Init();
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
  nLD_Mode = LD_MODE_ASR_RUN;
	
	sd_init();
	while(1)
	{
		recognizeLoop();

	
	}



}
