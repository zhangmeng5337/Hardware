#ifndef FDRALGORITHM_H
#define FDRALGORITHM_H
#include "main.h"
#define CALIBRATION	1
#define MEASURE     0
#define PT					100
#define SAMPLE_COUNT		100
typedef struct{
	float a0;
	float a1;
	float a2;	
	float humid;
}factor_stru;
float SoilTemperature(unsigned char status,float AdcValueVol1,float AdcValueVol2);
float SoilHumid(unsigned char status,float AdcValueVol);
float DataMinusProc(uint32_t *p,unsigned char len);
float DataMinusProc2(unsigned char *p,unsigned char len);
uint32_t FloatToCharProc(float p);
float DigitRound(float p,unsigned char digit);
#endif
