#include "LD_Config.h"

/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
 
	 	GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO结构体
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);

    /* 配置PA1,PA4为推挽输出，PA1作为复位引脚,PA4作为片选引脚 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	  //PC4----SPIS
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

//  	/* 配置SPI引脚: SCK,MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_Init(GPIOA,&GPIO_InitStructure);		
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	   //使能SPI3外设时钟


	LD_CS_H();
	LD_SPIS_L();
//	
//	//spi功能配置
	SPI_Cmd(SPI1, DISABLE);
	/* SPI3 配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7
	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_InitStructure);
	/* 使能SPI3 */
	SPI_Cmd(SPI1, ENABLE);
	
}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void) 
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //中断引脚配置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line5);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //默认拉高中断引脚

	EXTI_ClearFlag(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line5);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* 名    称：  EXTI1_IRQHandler(void)
* 功    能： 外部中断函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET ) 
	{
 		ProcessInt0(); 
		EXTI_ClearFlag(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}


/***********************************************************
* 名    称：static uint8 spi_send_byte(uint8 byte)
* 功    能： 写SPI
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
static uint8 spi_send_byte(uint8 byte)
{
	/* 循环检测发送缓冲区是否是空 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*通过SPI3外设发出数据*/
	SPI_I2S_SendData(SPI1,byte);

	/* 等待接收数据，循环检查接收数据缓冲区 */
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);

	/* 返回读出的数据 */
	return SPI_I2S_ReceiveData(SPI1);
}
/***********************************************************
* 名    称：void LD_WriteReg(uint8 data1,uint8 data2)
* 功    能： 写ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_SPIS_L();
	LD_CS_L();
	
//	Delay_us(10);

	spi_send_byte(0x04);

	spi_send_byte(data1);

	spi_send_byte(data2);

//	Delay_us(10);
	
	LD_CS_H();

}
/***********************************************************
* 名    称：uint8 LD_ReadReg(uint8 reg_add)
* 功    能：读ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	
	LD_SPIS_L();
	
	LD_CS_L();
	
	Delay_us(10);

	spi_send_byte(0x05);

	spi_send_byte(reg_add);

	i=spi_send_byte(0x00);	/*读SPI*/

	Delay_us(10);
	
	LD_CS_H();

	return(i);
}




/***********************************************************
* 名    称： void Delay_( int i)
* 功    能： 短延时
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Delay_us(unsigned long i)
 {     
    while( i--)
       {	
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
				 __nop();__nop();

        }
 }	
/***********************************************************
* 名    称：	LD3320_delay(unsigned long uldata)
* 功    能：	长延时函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
		Delay_us(uldata * 1000);
}
















