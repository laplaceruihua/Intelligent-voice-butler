#ifndef _SD_DRIVER_H_
#define _SD_DRIVER_H_

#include "main.h"

//////////////////////////////////////////////////////////////////////////////////	 

//SD�� ��������	   
								  
//////////////////////////////////////////////////////////////////////////////////
 						    	 
// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
//�ⲿ��Ӧ���ݾ�����������޸�!
//ս��STM32������ʹ�õ���PD2��ΪSD����CS��.
#define	SD_CS  PDout(2) //SD��Ƭѡ����					    	  

extern u8  SD_Type;//SD��������
//���������� 
extern u8 SD_SPI_ReadWriteByte(u8 data);
extern void SD_SPI_SpeedLow(void);
extern void SD_SPI_SpeedHigh(void);
extern u8 SD_WaitReady(void);							      //�ȴ�SD��׼��
extern u8 SD_GetResponse(u8 Response);					//�����Ӧ
extern u8 SD_Initialize(void);							         //��ʼ��
extern u8 SD_ReadDisk(u8*buf,u32 sector,u16 cnt);		  //����
extern u8 SD_WriteDisk(u8*buf,u32 sector,u16 cnt);		//д��
extern u32 SD_GetSectorCount(void);   					      //��������
extern u8 SD_GetCID(u8 *cid_data);                     //��SD��CID
extern u8 SD_GetCSD(u8 *csd_data);                     //��SD��CSD


#endif
