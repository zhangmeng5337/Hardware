#ifndef GPS_H_
#define GPS_H_
#include "main.h"
//����
#define true 1
#define false 0
#define USART_REC_LEN  			200  	//�����������ֽ��� 200

//�������鳤��
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
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char altitude[AL_Length];
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;
void Gps_RxCpltCallback(void);

void errorLog(int num);
void printGpsBuffer(void);
void gps_uart_Init(void);
unsigned char gps_powerON(void);
unsigned char  parseGpsBuffer(unsigned char *p);

#endif
