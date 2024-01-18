#ifndef __RT10K3435_H
#define __RT10K3435_H

#include "stm8s.h"
#include "stdio.h"
#include "stm8s103f3P.h"

#include "math.h"



//t=(Rt-B)/K ȡ��������ĵĲ�ͬ�¶ȵ���ȴ��뷽�̣��Ϳ���
//�����K �� B�� 

//��������Rup ȡֵ  Rup=R(Tp)*(B-2Tp)/(B+2Tp)  
//ʽ��BΪBֵ��Tp �ǹ������ĵ��¶ȣ�
//���������ȡֵӰ�����Զ�

//AD=1024*Rt/(Rt+Rup)
//Rt=Rup*AD/(1024-AD)
//t=(Rup*AD/(1024-AD)-B)/K

//���� 1/T1 =ln(Rt/R)/B+1/T2����TI��Ȼ���ټ�ȥ273.15����ʵ���¶ȣ�ͬʱ+0.5��������
extern const unsigned int RtTable[50][2];




#define Rup 10//10K

unsigned int Rttempruture(unsigned int ad);

#endif

