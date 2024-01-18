#ifndef __INFRARED_H
#define __INFRARED_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l1xx.h"

/**********************���ⰴ��**********************/
#define  IR_Fun1    0x01fe45ba      //����1�����÷�վ
#define  IR_Fun2    0x01ff847a      //����2�����ô�����
#define  IR_Up      0x01fed12e
#define  IR_Down    0x01ff609e
#define  IR_OK      0x01fe21de
#define  IR_Cancel  0x01feb54a
#define	 IR_CODE_LEN	 33

// NEC��ʽ�������,Ϊ�˼�,ֻ�ж��û���,���жϻ�����ͷ���. 1.125ms ��ʾ11
//	2.25ms ��ʾ22   13.5ms��ʾ135
#define NEC_lead  135	// NEC������   13.5ms
#define NEC_H	    22    // NEC������1  2.25ms
#define NEC_L	    11    // NEC������0  1.125ms

#define  IR_STATE_NO	   0	// ����ֵ
#define	 IR_STATE_READ	 1	// ��ȡ��ֵ
#define	 IR_STATE_END	   2	// ��ȡ����
#define	 IR_STATE_OK	   3	// ��ֵok
/* Private macro -------------------------------------------------------------*/

uint32_t IR_KEY_Scan(void);
void Infrared_Config(uint32_t arr,uint16_t psc);
uint8_t IR_Process(void);

#endif


/*******************************************************************************/
