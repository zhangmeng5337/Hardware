#ifndef FILTER_H
#define FILTER_H
#define N					20
#define BUFFERSIZE		50
#define X_DIR			0
#define Y_DIR			1
#define Z_DIR			2
#define ALPHA			0.05
#define BETA			        0.05
typedef struct{
 short int M[3][N];
 short int A[3][N];
 short int B[3][N];
 short int T[3][N];
unsigned char F[3][N];
unsigned char index;
unsigned int Flu_arr;
unsigned int Cnt_arr;
}manetic_str;
#endif

