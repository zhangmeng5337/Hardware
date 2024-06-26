#ifndef  __CANBUS__H___
#define  __CANBUS__H___
/********************************** 非标准库头文件 ***********************************/
//如果使用CAN，那么sys.h  必须增加如下宏定义  #define INTVPACTION
#include "sys.h"

/********************************** 全局宏 ***********************************/
#define can_baud_125K 0
#define can_baud_250K 1
#define can_baud_500K 2
#define can_baud_1M 3



/********************************** 数据类型 ***********************************/
typedef struct _candataunit
{
	u32  ID;
	u8 status;//bit7   IDE  0表示标准帧  1表示扩展帧     bit6 RTR  0表示数据帧，1表示远程帧   bit3_0 发送数据长度，0-8.	
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
/********************************** 外部变量 ***********************************/





/********************************** 函数声明 ***********************************/


void CanBusInit(u16 bdt);
void CAN_tx(u32 ID, u8 *str);
void CanRx(void);
void CanErrorReset(void);



#endif 

