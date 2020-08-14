#ifndef __RT10K3435_H
#define __RT10K3435_H

#include "stm8s.h"
#include "stdio.h"
#include "stm8s103f3P.h"

#include "math.h"



//t=(Rt-B)/K 取任意两点的的不同温度的阻度代入方程，就可以
//计算出K 和 B， 

//上拉电阻Rup 取值  Rup=R(Tp)*(B-2Tp)/(B+2Tp)  
//式中B为B值，Tp 是工作中心点温度，
//上拉电阻的取值影响线性度

//AD=1024*Rt/(Rt+Rup)
//Rt=Rup*AD/(1024-AD)
//t=(Rup*AD/(1024-AD)-B)/K

//或者 1/T1 =ln(Rt/R)/B+1/T2，求TI，然后再减去273.15就是实际温度，同时+0.5的误差矫正
extern const unsigned int RtTable[50][2];




#define Rup 10//10K

unsigned int Rttempruture(unsigned int ad);

#endif

