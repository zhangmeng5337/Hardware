#ifndef __BSP_IAP_H__
#define        __BSP_IAP_H__
 
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
 
/* ���Ͷ��� ------------------------------------------------------------------*/
/************************** IAP �������Ͷ���********************************/
typedef  void ( * pIapFun_TypeDef ) ( void ); //����һ���������͵Ĳ���.
 
/* �궨�� --------------------------------------------------------------------*/
/************************** IAP ���������********************************/
 //�Ƿ���� APP �� FLASH��������µ� RAM
#define User_Flash                                        
 
#ifdef User_Flash
  #define APP_START_ADDR       0x8010000          //Ӧ�ó�����ʼ��ַ(�����FLASH)
#else
  #define APP_START_ADDR       0x20001000          //Ӧ�ó�����ʼ��ַ(�����RAM)
#endif
 
/************************** IAP �ⲿ����********************************/
#define APP_FLASH_LEN                             56320u       //���� APP �̼����������55kB=55*1024=56320
 
/* ��չ���� ------------------------------------------------------------------*/
extern struct  STRUCT_IAP_RECIEVE      //��������֡�Ĵ���ṹ��
{
        uint8_t   ucDataBuf[APP_FLASH_LEN];
        uint16_t  usLength;
} strAppBin;
 
/* �������� ------------------------------------------------------------------*/
void IAP_Write_App_Bin( uint32_t appxaddr, uint8_t * appbuf, uint32_t applen);        //��ָ����ַ��ʼ,д��bin
void IAP_ExecuteApp( uint32_t appxaddr );                                                      //ִ��flash�����app����
 
#endif /* __BSP_IAP_H__ */
 
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/