#ifndef __M25P64_H
#define __M25P64_H
#include "stm32l1xx.h"
#define WRITEADDR	0x08080100

typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t u32;

//#define M25P32			1
#define M25XX_DEVICE         M25P32

#if(M25XX_DEVICE == M25P32)

#define M25P32 								0X202016 //ID?
#define M25PXX_PAGE_BYTES_SIZE				256
#define M25PXX_PER_SECTOR_PAGES_SIZE		256
#define M25PXX_SECTOR_SIZE					64
#define M25PXX_SECTOR_BYTES_SIZE 			65536
#define M25PXX_MAX_ADDRESS					0x400000
#elif (M25XX_DEVICE == M25P64)

#define M25P64 	0X202017 //ID��
#define M25PXX_PAGE_BYTES_SIZE				256
#define M25PXX_PER_SECTOR_PAGES_SIZE		256
#define M25PXX_SECTOR_SIZE					128
#define M25PXX_SECTOR_BYTES_SIZE 	M25PXX_PER_SECTOR_PAGES_SIZE	* M25PXX_PAGE_BYTES_SIZE
#endif
extern uint32_t M25PXX_TYPE;					//����M25PXXоƬ�ͺ�

//#define	M25PXX_CS 		PAout(4)  		//M25PXX��Ƭѡ�ź�

//////////////////////////////////////////////////////////////////////////////////
//ָ���
#define M25X_WREN		 0x06  //дʹ��
#define M25X_WRDI		 0x04  //дʧ��
#define M25X_RDID	 	 0x9F  //��ID
#define M25X_RDSR	 	 0x05  //��״̬�Ĵ���
#define M25X_WRSR 		 0x01  //д״̬�Ĵ���
#define M25X_READ		 0x03  //��ȡ����
#define M25X_FAST_READ   0x0B  //���ٶ�ȡ����
#define M25X_PP    	 	 0x02  //дҳ�� 
#define M25X_SE          0xD8  //��������
#define M25X_BE			 0xC7  //������� 
#define M25X_RES		 0xAB  //��ȡ����ǩ��

#define WIP_Flag   0x01  //д���̶���־
#define Dummy_Byte 0xFF  //�����
//--------------------------------------------------------------------------------
void M25PXX_Init(void);			    //M25P64��ʼ������
unsigned char M25PXX_ReadSR(void);  			    //��ȡM25PXX��״̬�Ĵ���
void M25PXX_Write_SR(unsigned char sr);    	    //дM25PXX״̬�Ĵ���
void M25PXX_Wait_Busy(void);    	    //�ȴ�оƬ����
void M25PXX_Write_Enable(void); 	    //M25PXXдʹ��
void M25PXX_Write_Disable(void);	    //M25PXXд��ֹ
void M25PXX_Erase_Sector(uint32_t Dst_Addr); //����һ������
void M25PXX_Erase_Chip(void);           //��������оƬ

//void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);//дһҳ����
void M25PXX_Write_NoCheck(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);//�޼���дSPI FLASH�������Զ���ҳ����
void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite,u8 ease_flag);//дSPI FLASH
unsigned char M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);//��ȡSPI FLASH

uint32_t M25PXX_ReadID(void); //��ȡоƬID����

unsigned char M25PXX_Read_Length(unsigned int* pBuffer);

unsigned char Device_data_store(unsigned char *pbuff,uint32_t WriteAddr,uint32_t len);
unsigned char M25PXX_Read_len(uint32_t *NumByteToRead);
unsigned char Device_data_read(unsigned char *pbuff,uint32_t Raddress,uint32_t len);
unsigned  char M25PXX_Erase_Head(void);
void SPI_FLASH_Write(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite);
void write_all_chip(void);
void SPI_FLASH_READ(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
uint32_t Read_AddressWrite(void);
#endif
