#ifndef PROTOCAL_H_
#define PROTOCAL_H_
#include "stm8l15x.h"

#define NODE_TO_SERVERH  0xAB
#define NODE_TO_SERVERL  0xAB
#define SERVER_TO_NODEH  0xAA
#define SERVER_TO_NODEL  0xAA

#define LOCK_ON            0xa0
#define LOCK_OFF           0xa1
#define HEART              0xa2
#define GET_GNSS           0xa3

#define CHECKSUM_INDEX   0x05

#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 
typedef struct{
  unsigned char id[4];
  unsigned char len;
  unsigned char checksum;
  unsigned char vbat[3];
  unsigned char status;
}Data_Stru;
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;

void module_process(void);
void module_prams_init(void);
unsigned char battery_status(void);
unsigned char uart_analy(void);
void lock_ctrl_process(void);
unsigned char parseGpsBuffer(void);
#endif

