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
	PrintCom(USART1,"���ڳ�ʼ���ɹ���\r\n");
//LCD��ʾ����ʼ��
	Lcd_Init();
	Gui_box(0,0,50,50,BLACK);
	Gui_DrawFont_GBK16(0,50,BLACK,WHITE,"TSET��ӯ����ϴ�");
//	UI_Task();
//����ʶ��оƬ��ʼ��
	LD3320_Init();
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
  nLD_Mode = LD_MODE_ASR_RUN;
	
	sd_init();
	while(1)
	{
		recognizeLoop();

	
	}



}
