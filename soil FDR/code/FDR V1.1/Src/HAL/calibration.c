#include "calibration.h"
#include "filter.h"
ratio_stru ratio;

ratio_stru *getRatio()
{
    return &ratio;
}

static float c1,c2,c3,c4,c5;
/************************************************************
功能：设备出厂前校准
输入参数：
				humidCalibration:  建大样机通过485返回的电压值
************************************************************/
ratio_stru *CalibrationRatio(float sVol,float CaliVol)
{
    
    c1 = CaliVol;
    c1 = c1;
    c2 = CaliVol*CaliVol;
    c2 = c2;
    c3 = c2*CaliVol;
    c3 = c3;
    c4 = c3*CaliVol;
    c4 = c4;
    ratio.status = 1;
    ratio.aN = AN;//F
    if(ratio.samplecount < AN)
    {
        ratio.samplecount ++;
        ratio.aF = ratio.aF + c1;//F
        ratio.aG = ratio.aG + c2;//G
        ratio.aH = ratio.aH + c3;//H
        ratio.aI = ratio.aI + c4;//I
        ratio.aB = ratio.aB + sVol;//B
        c5 = sVol*c1;
        ratio.aC = ratio.aC + c5;//C
        c5 = sVol*c2;
        ratio.aD = ratio.aD + c5;//D
        ratio.status = 1;
    }
    else
    {
        static double  K,tmp,L,M,P,denominator;
        K = 0;
        L = 0;
        M = 0;
        P = 0;
		    ratio.aN = AN;
        K = ratio.aF*ratio.aF;//F^2
        K = ratio.aI*K;//I*F^2
        
        tmp =  ratio.aF*ratio.aG;//F*G
        tmp = tmp*ratio.aH;//F*G*H
        tmp = tmp*(-2);//- 2*F*G*H
        K = K + tmp;//I*F^2-2*F*G*H
        
        tmp = ratio.aG*ratio.aG;//G^2
        tmp = tmp*ratio.aG;//G^3
        K = K + tmp;//I*F^2-2*F*G*H+G^3
        
        tmp = ratio.aI*ratio.aN;//I*N
        tmp = tmp*ratio.aG;//I*N*G
        tmp = (-1)*tmp;//-I*N*G
        K = K + tmp;//I*F^2-2*F*G*H+G^3-I*N*G
        
        tmp = ratio.aN*ratio.aH;//N*H
        tmp = tmp*ratio.aH;//N*H^2
        K = K +tmp;//I*F^2-2*F*G*H+G^3-I*N*G+N*H^2



        tmp = ratio.aD*ratio.aG;
        tmp = tmp*ratio.aG;//ratio.aD*ratio.aG^2
        L = L + tmp;
        tmp =  ratio.aC*ratio.aG;
        tmp = (-1)* tmp;		
        tmp = tmp*ratio.aH;//- ratio.aC*ratio.aG*ratio.aH
        L = L + tmp;
        tmp = - ratio.aB;
        tmp = tmp*ratio.aI;
        tmp = tmp*ratio.aG;//- ratio.aB*ratio.aI*ratio.aG
        L = L +tmp;
        tmp = ratio.aB*ratio.aH;
        tmp = tmp*ratio.aH;//ratio.aB*ratio.aH^2
        L = L +tmp;
        tmp = (-1)* ratio.aD;
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
        tmp = (-1)*ratio.aD;
        tmp = tmp*ratio.aF;
        tmp = tmp*ratio.aG;//- atio.aD*ratio.aF*ratio.aG
        M = M + tmp;
        tmp = (-1)*ratio.aC;
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
        tmp = (-1)*ratio.aC;
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
        tmp = (-1)*ratio.aD;
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


