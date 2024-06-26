#ifndef  __CANBUS__H___
#define  __CANBUS__H___
/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/
//���ʹ��CAN����ôsys.h  �����������º궨��  #define INTVPACTION
#include "sys.h"

/********************************** ȫ�ֺ� ***********************************/
#define can_baud_125K 0
#define can_baud_250K 1
#define can_baud_500K 2
#define can_baud_1M 3



/********************************** �������� ***********************************/
typedef struct _candataunit
{
	u32  ID;
	u8 status;//bit7   IDE  0��ʾ��׼֡  1��ʾ��չ֡     bit6 RTR  0��ʾ����֡��1��ʾԶ��֡   bit3_0 �������ݳ��ȣ�0-8.	
	u8   candata[8];
}CANUNIT;

typedef struct _candataunitbuf
{
	CANUNIT BusRXbuf[256];
	CANUNIT BusTXbuf[256];
	u8 CanRxHead,CanRxTail;
	u8 CanTxHead,CanTxTail,CanTxFlag;
}CANBUSUNIT;



extern CANBUSUNIT CanData;
extern u32 code CanBustxaddr[27];
extern u32 code CanBusAddr[35];
#define CAN_frame_NUM 35
/********************************** �ⲿ���� ***********************************/





/********************************** �������� ***********************************/


void CanBusInit(u16 bdt);
void CAN_tx(u32 ID, u8 *str);
void CanRx(void);
void CanErrorReset(void);



#endif 

