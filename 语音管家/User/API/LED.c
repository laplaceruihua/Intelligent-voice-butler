#include "LED.h"
#include "stm32f10x.h"
/***********************************************************
* ��    �ƣ�void LED_gpio_cfg(void)
* ��    �ܣ�LED�˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LED_gpio_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); //������ص�AHP����ʱ��
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
}

/***********************************************************
* ��    �ƣ�void Delayms(uint16 i)
* ��    �ܣ� ��ʱn����
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷�����
**********************************************************/ 
void Delayms(u16 i)
{
	unsigned char a,b;
	for(;i>0;i--)
	        for(b=4;b>0;b--)
	            for(a=113;a>0;a--);	

}

/***********************************************************
* ��    �ƣ�void Glide_LED(void)
* ��    �ܣ� ʵ����ˮ�ƹ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void Glide_LED(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	Delayms(0xfff);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	Delayms(0xfff);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	Delayms(0xfff);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	Delayms(0xfff);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	Delayms(0xfff);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	Delayms(0xfff);
}

/***********************************************************
* ��    �ƣ� void Flicker_LED(void)
* ��    �ܣ�  ʵ�ֵ���˸����
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void Flicker_LED(void)
{
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	Delayms(0XFFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	Delayms(0XFFF);
}
/***********************************************************
* ��    �ƣ�void Off_LED(void)
* ��    �ܣ�  �ر�ȫ��LED
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void Off_LED(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
}

void On_LED(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
}




/***********************************************************
* ��    �ƣ�void Lantern_Config(void)
* ��    �ܣ��߲ʵƿ��ƶ˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void Lantern_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	RED_CTL(0);
	GREEN_CTL(0);
	BLUE_CTL(0);
}

/***********************************************************
* ��    �ƣ�void Beep_Config(void)
* ��    �ܣ�BEEP���ƶ˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/
void Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}


/***********************************************************
* ��    �ƣ�void Flashlight_Config(void)
* ��    �ܣ������ƿ��ƶ˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void Flashlight_Config(void)
{
	 //PB7/TIM4_CH2
  RCC->APB1ENR |= (1<<1);//TIM3
	RCC->APB2ENR |= (1<<0);//TIM3
  RCC->APB2ENR |= (1<<4);//GPIOB
  
  GPIOC->CRL &= (0x0fffffff);
  GPIOC->CRL |= (0xb0000000);//��������
	AFIO->MAPR &= ~(0X03<<10);
	AFIO->MAPR |= (0X03<<10);//TIM3������ȫӳ�䣬��TIM3_CH2ӳ�䵽PC7��
  
  TIM3->CR1 |= (1<<4);//���¼���
  TIM3->CR1 |= (1<<7);//�Զ�װ��Ԥװ��ʹ��
  TIM3->PSC = 72-1;//����Ԥ��Ƶ
  TIM3->ARR = 1000-1;//������װ��ֵ
  
  //����ͨ��2
  TIM3->CCMR1 &=~ (7<<12);
  TIM3->CCMR1 |=  (6<<12);//PWM1ģʽ
  TIM3->CCMR1 |=  (1<<11);//����Ƚ�Ԥװ��ʹ��
  TIM3->CCER &=~ (1<<5);//�ߵ�ƽ��Ч
  
  TIM3->CCER |= (1<<4);//�������
  TIM3->CR1 |= (1<<0);//ʹ�ܼ�����
  
  PWM_LED(0);
}
/***********************************************************
* ��    �ƣ�static void PWM_LED(int16_t led)
* ��    �ܣ������ƿ��ƺ���
* ��ڲ�����  
* ���ڲ�����
* ˵    �����ܹ����ڷ�������
* ���÷����� 
**********************************************************/ 
void PWM_LED(int16_t led)
{
  if (led > 1000) led = 1000;
  led = TIM3->ARR * (led/1000.0f);
  if (led <= 0)
  {
    led = 0;
    TIM3->CCER &=~ (1<<4);//�ر����
  }
  else
    TIM3->CCER |= (1<<4);//�������
  TIM3->CCR2 = led;
}








