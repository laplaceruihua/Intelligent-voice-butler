#ifndef _SD_CONFIG_H_
#define _SD_CONFIG_H_

#include "main.h"


#define SPI2_SCK  PBout(3)
#define SPI2_MISO PBin(4)
#define SPI2_MOSI PBout(5)

void SD_PORT_Init();
u8 SPI2_ReadWriteByte(u8 byte);



#endif

