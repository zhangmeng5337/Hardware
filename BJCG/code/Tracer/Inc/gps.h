#ifndef GPS_H_
#define GPS_H_
#include "main.h"
//声明
#define true 1
#define false 0
#define USART_REC_LEN  			200  	//定义最大接收字节数 200

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 
#define AL_Length 6 

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
	char altitude[AL_Length];
	char isUsefull;		//定位信息是否有效
} _SaveData;
void Gps_RxCpltCallback(void);

void errorLog(int num);
void printGpsBuffer(void);
void gps_uart_Init(void);
unsigned char gps_powerON(void);
unsigned char  parseGpsBuffer(unsigned char *p);

#endif
