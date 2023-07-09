#include "LD_Config.h"

/***********************************************************
* ��    �ƣ�LD3320_GPIO_Cfg(void)
* ��    �ܣ���ʼ����Ҫ�õ���IO��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
 
	 	GPIO_InitTypeDef GPIO_InitStructure; //����GPIO�ṹ��
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);

    /* ����PA1,PA4Ϊ���������PA1��Ϊ��λ����,PA4��ΪƬѡ���� */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	  //PC4----SPIS
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

//  	/* ����SPI����: SCK,MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
    GPIO_Init(GPIOA,&GPIO_InitStructure);		
}
/***********************************************************
* ��    �ƣ�LD3320_Spi_cfg(void)
* ��    �ܣ�����SPI���ܺͶ˿ڳ�ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	   //ʹ��SPI3����ʱ��


	LD_CS_H();
	LD_SPIS_L();
//	
//	//spi��������
	SPI_Cmd(SPI1, DISABLE);
	/* SPI3 ���� */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;   //�����ʿ��� SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7
	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_InitStructure);
	/* ʹ��SPI3 */
	SPI_Cmd(SPI1, ENABLE);
	
}
/***********************************************************
* ��    �ƣ� LD3320_EXTI_Cfg(void) 
* ��    �ܣ� �ⲿ�жϹ������ú���ض˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //�ж���������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//�ⲿ�ж�������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line5);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //Ĭ�������ж�����

	EXTI_ClearFlag(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line5);
	//�ж�Ƕ������
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* ��    �ƣ�  EXTI1_IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�static uint8 spi_send_byte(uint8 byte)
* ��    �ܣ� дSPI
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
static uint8 spi_send_byte(uint8 byte)
{
	/* ѭ����ⷢ�ͻ������Ƿ��ǿ� */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*ͨ��SPI3���跢������*/
	SPI_I2S_SendData(SPI1,byte);

	/* �ȴ��������ݣ�ѭ�����������ݻ����� */
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);

	/* ���ض��������� */
	return SPI_I2S_ReceiveData(SPI1);
}
/***********************************************************
* ��    �ƣ�void LD_WriteReg(uint8 data1,uint8 data2)
* ��    �ܣ� дld3320�Ĵ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�uint8 LD_ReadReg(uint8 reg_add)
* ��    �ܣ���ld3320�Ĵ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	
	LD_SPIS_L();
	
	LD_CS_L();
	
	Delay_us(10);

	spi_send_byte(0x05);

	spi_send_byte(reg_add);

	i=spi_send_byte(0x00);	/*��SPI*/

	Delay_us(10);
	
	LD_CS_H();

	return(i);
}




/***********************************************************
* ��    �ƣ� void Delay_( int i)
* ��    �ܣ� ����ʱ
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�	LD3320_delay(unsigned long uldata)
* ��    �ܣ�	����ʱ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
		Delay_us(uldata * 1000);
}
















