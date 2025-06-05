#ifndef __LCD__
#define __LCD__

#include "main.h"

#define LCD_CS_H  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
#define LCD_CS_L  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

#define LCD_CLK_H  HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_SET);
#define LCD_CLK_L  HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_RESET);

#define LCD_SID_R  HAL_GPIO_ReadPin(LCD_SID_GPIO_Port, LCD_SID_Pin)
#define LCD_SID_H  HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_SET);
#define LCD_SID_L  HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_RESET);



#define   u8    uint8_t 
#define   u16    uint16_t
#define   u32    uint32_t

/*******************************************************************************
RS(����ģʽ:CS��
����ģʽ��
RS=��H��,��ʾDB7����DB0Ϊ��ʾ����
RS=��L��,��ʾDB7����DB0Ϊ��ʾָ������

R/W(����ģʽ:SID)
����ģʽ��
R/W=��H��,E=��H��,���ݱ�����DB7����DB0
R/W=��L��,E=��H��L��, DB7����DB0�����ݱ�д��IR��DR

E(����ģʽ:SCLK)
H/Lʹ���ź�
*******************************************************************************/

// PA6--MISO--(not used) // you omit this IO
// PB13--SCK--CLK--E
// PB14--NSS--CS--RS
// PB15--MOSI--SDIO--R/W

// PB1--BLK

//********��ֲ��ʱ��ֻ���޸����¶��弴��***************************************/



/*********************��ֲ�����޸� end ***************************************/
typedef unsigned char uchar;
typedef unsigned int  unit;  //�궨��


#define LCD_SID             HAL_GPIO_ReadPin( LCD_SID_GPIO_Port, LCD_SID_Pin)  // Read Pin
#define LCD_SID_HIGH        HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_SET)    //R/W
#define LCD_SID_LOW         HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_RESET) 

#define LCD_CS_HIGH         HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)   // RS
#define LCD_CS_LOW          HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET) 

#define LCD_CLK_HIGH        HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_SET)    //E
#define LCD_CLK_LOW         HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_RESET) 

#define BLK_ON             HAL_GPIO_WritePin(LCD_BKL_GPIO_Port, LCD_BKL_Pin, GPIO_PIN_SET)              
#define BLK_OFF            HAL_GPIO_WritePin(LCD_BKL_GPIO_Port, LCD_BKL_Pin, GPIO_PIN_RESET) 

   
                   

/* ���崮�з������ݵ�ʱ���ָ�� */
#define writecommand            0xF8
#define writedata               0xFA
#define READ_STATE              0xFC
#define readdata                0xFE

#define	LCD_ADDRESS_RESET      	        0x02	//��ַ����	 
#define	LCD_BASIC_FUNCTION	  	0x30	//����ָ�
#define	LCD_EXTEND_FUNCTION	   	0x34	//����ָ�

//��չָ�Ԥ����
#define	LCD_AWAIT_MODE			0x01	 	//����ģʽ	
#define LCD_ROLLADDRESS_ON		0x03		//�������봹ֱ����ַ
#define LCD_IRAMADDRESS_ON		0x02		//��������IRAM��ַ	
#define	LCD_SLEEP_MODE			0x08	 	//����˯��ģʽ
#define	LCD_NO_SLEEP_MODE		0x0c	 	//����˯��ģʽ
#define LCD_GRAPH_ON			0x36		//�򿪻�ͼģʽ
#define LCD_GRAPH_OFF			0x34		//�رջ�ͼģʽ(��������ָ�����)

#define LCD_CURSOR_LEFT			0x14		//�α�����
#define LCD_CURSOR_RIGHT  	        0x10		//�α�����

static void SendByte(u8 dat);
static u8 ReceiveByte(void);
static u8 readBF(void);

void LCD_WriteCMD(u8 data);
void LCD_WriteDATA(u8 data);

void LCD_Clear_DDRAM(void);
void LCD_Clear_GDRAM(void);
void LCD_Init(void);

void LCD12864_Write_SuperString(uchar x,uchar y,uchar *s);
void Lcd_Fill(uchar color); //�������RAM
void LCD12864_Write_Numberd(unsigned char X,unsigned char Y,int number);
void LCD12864_Write_Numberf(unsigned char X,unsigned char Y,float number);

#endif




