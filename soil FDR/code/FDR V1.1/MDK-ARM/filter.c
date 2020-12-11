#include "filter.h"
unsigned int *value_buf;
extern  unsigned int adcBuf_ref[N];
extern	unsigned int adcBuf_humid[N];
extern	unsigned int adcBuf_ta[N];
extern	unsigned int adcBuf_tb[N];
ratio_stru ratio;
/****************************************
均值滤波
*****************************************/
uint32_t filter(uint32_t seq)
{
    unsigned char count;

    uint32_t  sum=0;
    switch(seq)
    {
    case 0:
        value_buf=adcBuf_ref;
        break;
    case 1:
        value_buf=adcBuf_humid;
        break;
    case 2:
        value_buf=adcBuf_ta;
        break;
    case 3:
        value_buf=adcBuf_tb;
        break;
    }

    for(count=0; count<N; count++)
        sum += value_buf[count];
    return (sum/(N));
}
ratio_stru *getRatio()
{
    return &ratio;
}
/************************************************************
功能：设备出厂前校准
输入参数：
      ahumid:sensor校准前默认参数计算得到的湿度
      chmid:建大样机通过485返回的湿度值
************************************************************/
ratio_stru *CalibrationRatio(float ahumid,float chumid)
{
    float c1,c2,c3,c4;
    c1 = chumid;
    c2 = chumid*chumid;
    c3 = c2*chumid;
    c4 = c3*chumid;
    ratio.status = 1;
    if(ratio.samplecount < AN)
    {
        ratio.samplecount ++;
        ratio.aF = ratio.aF + c1;//F
        ratio.aG = ratio.aG + c2;//G
        ratio.aH = ratio.aH + c3;//H
        ratio.aI = ratio.aI + c4;//I
        ratio.aB = ratio.aB + ahumid;//B
        ratio.aC = ratio.aC + ahumid*c1;//C
        ratio.aD = ratio.aD + ahumid*c2;//D
        ratio.status = 1;
    }
    else
    {
          float K,tmp,L,M,P;
		  K = 0;
		  L = 0;
		  M = 0;
		  P = 0;
		  K = ratio.aF*ratio.aF;
		  K = ratio.aI*K;//ratio.aI*ratio.aF^2
		  tmp =  ratio.aF*ratio.aG;
		  tmp = tmp*ratio.aH;
		  tmp = tmp*(-2);//- 2*ratio.aF*ratio.aG*ratio.aH
		  K = K + tmp;
		  tmp = ratio.aG*ratio.aG;
		  tmp = tmp*ratio.aG;//ratio.aG^3
		  K = K + tmp;
		  tmp = ratio.aI*ratio.aN;
		  tmp = tmp*ratio.aG;
		  tmp = (-1)*tmp;//- ratio.aI*ratio.aN*ratio.aG
		  K = K + tmp;
		  tmp = ratio.aN*ratio.aH;
		  tmp = tmp*ratio.aH;// ratio.aN*ratio.aH^2
		  K = K +tmp;

		  tmp = ratio.aD*ratio.aG;
		  tmp = tmp*ratio.aG;//ratio.aD*ratio.aG^2
		  L = L + tmp;
		  tmp = - ratio.aC*ratio.aG;
		  tmp = tmp*ratio.aH;//- ratio.aC*ratio.aG*ratio.aH
		  L = L + tmp;
		  tmp = - ratio.aB;
		  tmp = tmp*ratio.aI;
		  tmp = tmp*ratio.aG;//- ratio.aB*ratio.aI*ratio.aG
		  L = L +tmp;
		  tmp = ratio.aB*ratio.aH;
		  tmp = tmp*ratio.aH;//ratio.aB*ratio.aH^2
		  L = L +tmp;
		  tmp = - ratio.aD;
		  tmp = tmp*ratio.aF;
		  tmp = tmp*ratio.aH;//- ratio.aD*ratio.aF*ratio.aH
		  L = L+tmp;
		  tmp = ratio.aC*ratio.aF;
		  tmp = tmp*ratio.aI;//ratio.aC*ratio.aF*ratio.aI
		  L = L +tmp;
		  ratio.a0 = L/K;

   
          tmp = ratio.aC*ratio.aG;
		  tmp = tmp*ratio.aG;//ratio.aC*ratio.aG^2
		  M = M+ tmp;
		  tmp = (-1)*ratio.aB;
		  tmp = tmp*ratio.aG;
		  tmp = tmp*ratio.aH;//  - ratio.aB*ratio.aG*ratio.aH
		  M = M + tmp;
		  tmp = ratio.aB*ratio.aF;
		  tmp = tmp*ratio.aI;// + ratio.aB*ratio.aF*ratio.aI
		  M = M +tmp;
		  tmp = -ratio.aD;
		  tmp = tmp*ratio.aF;
		  tmp = tmp*ratio.aG;//- atio.aD*ratio.aF*ratio.aG
		  M = M + tmp;
		  tmp = - ratio.aC;
		  tmp = tmp*ratio.aI;
		  tmp = tmp*ratio.aN;//- ratio.aC*ratio.aI*ratio.aN
		  M = M + tmp;
          tmp = ratio.aD*ratio.aH;
		  tmp = tmp*ratio.aN;//ratio.aD*ratio.aH*ratio.aN
		  M = M + tmp;
   		  ratio.a1 = M / K;		  

		  tmp = ratio.aD*ratio.aF;
		  tmp = tmp*ratio.aF;//ratio.aD*ratio.aF^2
		  P = P+tmp;
		  tmp = - ratio.aC;
		  tmp = tmp*ratio.aF;
		  tmp = tmp*ratio.aG;//- ratio.aC*ratio.aF*ratio.aG
		  P = P + tmp;
		  tmp = - ratio.aB;
		  tmp = tmp*ratio.aH;
		  tmp = tmp*ratio.aF;//- ratio.aB*ratio.aH*ratio.aF
		  P = P + tmp;
          tmp = ratio.aB*ratio.aG;
		  tmp = tmp*ratio.aG;//ratio.aB*ratio.aG^2
		  P = P +tmp;
		  tmp = - ratio.aD;
		  tmp = tmp*ratio.aN;
		  tmp = tmp*ratio.aG;//- ratio.aD*ratio.aN*ratio.aG
		  P = P + tmp;
		  tmp = ratio.aC*ratio.aH;
		  tmp = tmp*ratio.aN;
		  P = P +tmp;//ratio.aC*ratio.aH*ratio.aN
		  ratio.a2 = P/K;
	/*ratio.a0 =(ratio.aD*ratio.aG^2 - ratio.aC*ratio.aG*ratio.aH - ratio.aB*ratio.aI*ratio.aG + 
		      ratio.aB*ratio.aH^2 - ratio.aD*ratio.aF*ratio.aH + ratio.aC*ratio.aF*ratio.aI)/
		      (ratio.aI*ratio.aF^2 - 2*ratio.aF*ratio.aG*ratio.aH + ratio.aG^3 - ratio.aI*ratio.aN*ratio.aG +
		      ratio.aN*ratio.aH^2) */
   /*ratio.a1 =(ratio.aC*ratio.aG^2 - ratio.aB*ratio.aG*ratio.aH + ratio.aB*ratio.aF*ratio.aI - 
               ratio.aD*ratio.aF*ratio.aG - ratio.aC*ratio.aI*ratio.aN + ratio.aD*ratio.aH*ratio.aN)/
              (ratio.aI*ratio.aF^2 - 2*ratio.aF*ratio.aG*ratio.aH + ratio.aG^3 - ratio.aI*ratio.aN*ratio.aG + ratio.aN*ratio.aH^2) 
*/
   /*ratio.a2 =(ratio.aD*ratio.aF^2 - ratio.aC*ratio.aF*ratio.aG - ratio.aB*ratio.aH*ratio.aF +
              ratio.aB*ratio.aG^2 - ratio.aD*ratio.aN*ratio.aG + ratio.aC*ratio.aH*ratio.aN)/
             (ratio.aI*ratio.aF^2 - 2*ratio.aF*ratio.aG*ratio.aH + ratio.aG^3 - ratio.aI*ratio.aN*ratio.aG + ratio.aN*ratio.aH^2)
        humid = ratio.a0 + ratio.a1*c1+ ratio.a2*c2;*/
        ratio.samplecount = 0;
        ratio.status = 0;
        ratio.calibrationFlag = 0;
    }

    return &ratio;
}
