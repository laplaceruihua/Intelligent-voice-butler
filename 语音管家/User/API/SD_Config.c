#include "SD_Config.h"

/*


//Ä£Äâspi
sd_cs----PD2
SD_SCK---PB3
SD_MISO---PB4
SD_MOSI---PB5

*/
void SD_PORT_Init()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC->APB2ENR |= (1 << 5); //PD
	  RCC->APB2ENR |= (1 << 3); //PB
	  RCC->APB2ENR |= (1 << 0); //AFIO
	
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	  //Ä£Ê½ÅäÖÃ

		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

u8 SPI2_ReadWriteByte(u8 byte)
{
    u8 i,Temp=0;
    for(i=0;i<8;i++)                        
    {
        SPI2_SCK = 0;                           
        if(byte&0x80) SPI2_MOSI = 1;                               
        else          SPI2_MOSI = 0;			
        byte <<= 1; 		
        SPI2_SCK = 1;                          
        Temp <<= 1;                           
        if(SPI2_MISO) Temp++;      
        SPI2_SCK = 0; 			
    }
		
    return (Temp);                             
}






