#ifndef FDRALGORITHM_H
#define FDRALGORITHM_H
#define CALIBRATION	1
#define MEASURE     0
#define PT					100
#define SAMPLE_COUNT		24
typedef struct{
	float a0;
	float a1;
	float a2;	
	float humid;
}factor_stru;
float SoilTemperature(unsigned char status,float AdcValueVol1,float AdcValueVol2);
float SoilHumid(unsigned char status,float AdcValueVol);
#endif
