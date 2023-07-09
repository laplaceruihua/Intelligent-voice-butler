#include "LED.h"
#include "stm32f10x.h"
/***********************************************************
* 名    称：void LED_gpio_cfg(void)
* 功    能：LED端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LED_gpio_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); //开启相关的AHP外设时钟
		
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
}

/***********************************************************
* 名    称：void Delayms(uint16 i)
* 功    能： 延时n毫秒
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法：
**********************************************************/ 
void Delayms(u16 i)
{
	unsigned char a,b;
	for(;i>0;i--)
	        for(b=4;b>0;b--)
	            for(a=113;a>0;a--);	

}

/***********************************************************
* 名    称：void Glide_LED(void)
* 功    能： 实现流水灯功能
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
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
* 名    称： void Flicker_LED(void)
* 功    能：  实现灯闪烁功能
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
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
* 名    称：void Off_LED(void)
* 功    能：  关闭全部LED
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
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
* 名    称：void Lantern_Config(void)
* 功    能：七彩灯控制端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
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
* 名    称：void Beep_Config(void)
* 功    能：BEEP控制端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
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
* 名    称：void Flashlight_Config(void)
* 功    能：照明灯控制端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Flashlight_Config(void)
{
	 //PB7/TIM4_CH2
  RCC->APB1ENR |= (1<<1);//TIM3
	RCC->APB2ENR |= (1<<0);//TIM3
  RCC->APB2ENR |= (1<<4);//GPIOB
  
  GPIOC->CRL &= (0x0fffffff);
  GPIOC->CRL |= (0xb0000000);//复用推挽
	AFIO->MAPR &= ~(0X03<<10);
	AFIO->MAPR |= (0X03<<10);//TIM3进行完全映射，把TIM3_CH2映射到PC7上
  
  TIM3->CR1 |= (1<<4);//向下计数
  TIM3->CR1 |= (1<<7);//自动装载预装载使能
  TIM3->PSC = 72-1;//设置预分频
  TIM3->ARR = 1000-1;//设置重装载值
  
  //配置通道2
  TIM3->CCMR1 &=~ (7<<12);
  TIM3->CCMR1 |=  (6<<12);//PWM1模式
  TIM3->CCMR1 |=  (1<<11);//输出比较预装载使能
  TIM3->CCER &=~ (1<<5);//高电平有效
  
  TIM3->CCER |= (1<<4);//开启输出
  TIM3->CR1 |= (1<<0);//使能计数器
  
  PWM_LED(0);
}
/***********************************************************
* 名    称：static void PWM_LED(int16_t led)
* 功    能：照明灯控制函数
* 入口参数：  
* 出口参数：
* 说    明：能够调节发光亮度
* 调用方法： 
**********************************************************/ 
void PWM_LED(int16_t led)
{
  if (led > 1000) led = 1000;
  led = TIM3->ARR * (led/1000.0f);
  if (led <= 0)
  {
    led = 0;
    TIM3->CCER &=~ (1<<4);//关闭输出
  }
  else
    TIM3->CCER |= (1<<4);//开启输出
  TIM3->CCR2 = led;
}








