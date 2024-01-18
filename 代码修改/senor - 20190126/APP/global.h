#ifndef  __GLOBAL_H
#define __GLOBAL_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef GLOBALS
#define EXT
#else
#define EXT extern
#endif
#define DEBUG  0
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "stm32l1xx.h"
#include "misc.h"
#include "system_stm32l1xx.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_dma.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_flash.h"
#include "stm32l1xx_syscfg.h"
/************************���巢�ͽṹ��**************************/
typedef struct
{
    uint8_t Invalid[5];   //��Ч�ֽ�
    uint8_t Start_Flag;		//����ͷ��־
    uint8_t ID;						//������ID
    uint8_t Type;					//����������
    uint8_t Channel;			//������ͨ��
    uint8_t T_Data_H[4];	//���������ݸ�λ
    uint8_t T_Data_L[4];	//���������ݵ�λ
    uint8_t Battery_Power;//���������
    uint8_t End_Flag;			//����β��־
} CC1110Tx_Msg;
/************************���崫�������Խṹ��**************************/
typedef struct
{
    uint8_t Senor_ID;
    uint8_t Senor_Num;
} SENOR;

typedef enum {Low = 0, High = !Low} PDStatus,WPStatus,CCStatus;

/************************flash��д���*************************/
EXT uint8_t CC1110_Send_Flag;
/************************��Դ�������*************************/
enum {Power_OFF=0,Power_ON=!Power_OFF};//5V��Դ���Ʊ�־
/************************���ú������*************************/
void delay_init(void);      //��ʱ
void delay_ms(uint16_t nms);//��ʱ����
void delay_us(uint32_t nus);//��ʱ����
void Battery_Monitor_Config (void);
uint8_t Battery_Monitor(void);//��ص�ѹ���
void Bsp_Config (void);     //Ӳ����ʼ��
PDStatus PD_WAKEUP_status (void);     //���ջ��ѱ�־
void EnterLowPower(void);
void Wake_Config(void);
void WAKEUP_Config (void);
void RCC_Config(void);
#ifdef __cplusplus
}
#endif
#endif



