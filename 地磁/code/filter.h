#ifndef FILTER_H
#define FILTER_H
#define N					20
#define BUFFERSIZE		50
#define X_DIR			0
#define Y_DIR			1
#define Z_DIR			2
#define ALPHA			0.05
#define BETA		    0.05
#define MAX_THRES		    80
#define MIN_THRES		    60
#define NO_VEHICLE_TIME 100
#define SAMPLE_COUNT    40

typedef struct{
 short int M[3][N];//original data
 short int F[3][N];//filter data
 short int B[3][N];//base line data
 short int T[3][N];
unsigned char F[3][N];
unsigned char index;
unsigned int Flu_arr;
unsigned int Cnt_arr;
unsigned char Car_Flag;
}magnetic_str;
#endif

