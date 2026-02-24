#ifndef HARDWARE_H
#define HARDWARE_H
#include "stc8h.h"       //包含此头文件后，不需要再包含"reg51.h"头文件
//0-5 1-6






#define S1_AN 		P30
#define S1_FP 		P30
#define S1_GN 		P30
#define S1_DN 	    P30
#define S1_DP_P     P30
#define S1_EP 		P30

#define S1_FN 		P34
#define S1_GP 		P34
#define S1_BN       P34

#define S1_BP 		P31
#define S1_DP 		P31
#define S1_DP_N 	P31
#define S1_CP 		P31

#define S1_CN   P32
#define S1_EN   P32

#define S1_AP P35




#define 		S4_AP P35
#define 		S4_AN P33
#define 		S5_VP P35
#define 		S5_VN P34

#define 		S2_AP  P32
#define 		S2_AN  P35
#define 		S2_BP  P32
#define 		S2_BN  P34
#define 		S2_CP  P32
#define 		S2_CN  P31
#define 		S2_DP  P32
#define 		S2_DN  P30
#define 		S2_EP  P31
#define 		S2_EN  P33
#define 		S2_FP  P31
#define 		S2_FN  P35
#define 		S2_GP  P32
#define 		S2_GN  P35
#define 		S2_DP_P  P33
#define 		S2_DP_N  P32		

#define 		S3_AP   P34
#define 		S3_AN   P32
#define 		S3_BP   P35
#define 		S3_BN   P32
#define 		S3_CP   P34
#define 		S3_CN   P31
#define 		S3_DP   P34
#define 		S3_DN   P33
#define 		S3_EP   P33
#define 		S3_EN   P31
#define 		S3_FP   P33
#define 		S3_FN   P34
#define 		S3_GP   P35
#define 		S3_GN   P31





void display(unsigned char digit,unsigned char dat,unsigned char seg_num);
void Delay5ms(void);	//@11.0592MHz
void Delay200ms(void);	//@11.0592MHz
#endif

