#ifndef CALI_H_
#define CALI_H_
#include "main.h"
#define NOZZLE_H		1  //0.75mm
#define NOZZLE_L		0  //0.2mm
#define FULL_SCALE		100//torr

#define TORR_TO_PA  	133.32237
typedef struct
{
	float p_H[3];
	float p_L[3];
}p_struct;
typedef struct
{
    float detalt;

	float p20;
	float p20_pre_H;
	float p20_pre_L;	
	float p21;
	float T;
	float offset;
	float v1;	
	float v2;
	float vm;
	float R;	
	float p1;
	float n0;
	float n1;
	float detalN;
	float F;
	float cali_flow;
	float bp4;
	float bp3;
	float bp2;
	float bp1;
	float bp0;
	float perror; //swtich nozzle pressure error
	float nozzle_up_p;//nozzle up pressure

	float target_t;//target time
	float targe_p;//target pressure
	float targe_p_H;//target pressure
	float targe_p_L;//target pressure
	float targe_p_drain; //back up
	float p1_max_H;
	float p1_max_L;

//	float alpha;
	unsigned int cail_nozzle_num;
	unsigned int gas_type;
	unsigned char state;//3£ºset ok
}calibration_stru;
typedef struct
{


  float Gerr2_H;
  float Gerr1_H;
  float Gerr0_H;
  float S1_H;
  float S2_H;
  float Tr_H;
  float T1;//K
  float Gerr2_L;
  float Gerr1_L;
  float Gerr0_L;
  float S1_L;
  float S2_L;
  float Tr_L;
  float a_H;
  float a_L;
  unsigned int state;
  unsigned int nozzle_num;
}nozzle_stru;

unsigned char pressORTime_steady_proc(void);
unsigned char nozzle_sel(void);
void cali_proc(void);
calibration_stru *get_calib(void);
unsigned char flow_mode_relay_sel(void);
nozzle_stru *get_nozzle(void);
unsigned char perror_proc(void);
void flow_proc(void);
unsigned char p20error_proc(void);



#endif

