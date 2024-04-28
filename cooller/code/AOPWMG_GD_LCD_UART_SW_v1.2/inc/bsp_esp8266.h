#ifndef  __BSP_ESP8266_H__
#define	 __BSP_ESP8266_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>  /*VA_LIST ����C�����н����������һ���,����ͷ�ļ�:#include <stdarg.h>,���ڻ�ȡ��ȷ�������Ĳ���*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

/* ���Ͷ��� ------------------------------------------------------------------*/
/******************************* ESP8266 �������Ͷ��� ***************************/
typedef enum
{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum
{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum
{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	
typedef enum
{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define RX_BUF_MAX_LEN     1024                                     //�����ջ����ֽ���

extern  uint8_t ucTcpClosedFlag;

/******************************** ESP8266 �������Ŷ��� ***********************************/


/*********************************************** ESP8266 �����궨�� *******************************************/
#define ESP8266_Usart( fmt, ... )           USART_printf (USART2, fmt, ##__VA_ARGS__ ) 
#define PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
//#define PC_Usart( fmt, ... )                

#define ESP8266_RST_HIGH_LEVEL()             HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET)
#define ESP8266_RST_LOW_LEVEL()              HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_RESET)

/* ��չ���� ------------------------------------------------------------------*/
/******************************* ESP8266 �ⲿȫ�ֱ������� ***************************/
extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
  union {
     u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 	
} strEsp8266_Fram_Record;


/* �������� ------------------------------------------------------------------*/
void bsp_InitEsp8266(void);
void ESP8266_stop( void );
void ESP8266_Rst( void );
bool ESP8266_Cmd( char * cmd, char * reply1, char * reply2, u32 waittime );
bool ESP8266_AT_Test( void );
bool ESP8266_Net_Mode_Choose( ENUM_Net_ModeTypeDef enumMode );
bool ESP8266_JoinAP( char * pSSID, char * pPassWord );
bool ESP8266_BuildAP( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool ESP8266_Enable_MultipleId( ControlStatus enumEnUnvarnishTx );
bool ESP8266_Link_Server( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_StartOrShutServer( ControlStatus enumMode, char * pPortNum, char * pTimeOver );
uint8_t ESP8266_Get_LinkStatus( void );
uint8_t ESP8266_Get_IdLinkStatus( void );
uint8_t ESP8266_Inquire_ApIp( char * pApIp, uint8_t ucArrayLength );
bool ESP8266_UnvarnishSend( void );
void ESP8266_ExitUnvarnishSend( void );
bool ESP8266_SendString( ControlStatus enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char * ESP8266_ReceiveString( ControlStatus enumEnUnvarnishTx );

void  USART_printf(ControlStatus * USARTx, char * Data, ... );


#endif

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
