#ifndef FILTER_H
#define FILTER_H
#include "main.h"
#define N					20
#define BUFFERSIZE		40
#define X_DIR			0
#define Y_DIR			1
#define Z_DIR			2
#define ALPHA			0.05
#define BETA		    0.05
#define GAMMA			1.25
#define MIN_PERIOD		200
short int MAX_THRES=80;
short int  MIN_THRES=60;
#define NO_VEHICLE_TIME 100
#define SAMPLE_TIME     50
#define SAMPLE_COUNT    40
#define MIN_CAR_CYCLE    50
typedef struct{
 s32 M[3][BUFFERSIZE];//original data
 s32 F[3][BUFFERSIZE];//filter data
 s32 B[3][BUFFERSIZE];//base line data
 s32 VehicleVari;
 s32 BaseLineVari;
unsigned char index;
unsigned char Car_Flag;
short int sTime;
short int eTime;
short int elapseTime;
unsigned char noupdate;
short int count;
}magnetic_str;
void MagneticInit(void);

#endif

