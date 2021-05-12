#include "math.h"
#include "calman.h"

//extern  float adcBuf_ref[N];
//extern	float adcBuf_humid[N];
//extern	float adcBuf_humidf[N];

//extern	float adcBuf_ta[N];
//extern	float adcBuf_tb[N];
#define RAND_MAX  0.5
void jansson_pack_test(void);
;void jansson_unpack_test(void);
/************************************************
r参数调整滤波后的曲线与实测曲线的相近程度，r越小越接近。

q参数调滤波后的曲线平滑程度，q越小越平滑。
*-***********************************************/
float Claman(float *pb,float sq,float sr,unsigned char seq)
{
    float x_k1_k1,x_k_k1;
    static float ADC_OLD_Value;
    float Z_k,pbf;
    static float P_k1_k1;

    float Q = 0.008;  // Q = 0.0003;
    float R = 10;
    static float Kg = 0;
    static float P_k_k1 = 1,P_k_k2,P_k_k3,P_k_k4;

    static float kalman_adc,kalman_adc2,kalman_adc3,kalman_adc4;
    static float kalman_adc_old=1;
    unsigned int i;
    Q = sq;
    R = sr;
    //kalman_adc_old = adcBuf_humid[0];
    for(i=0; i<1; i++)
    {
			  //Z_k = 0;
        //Z_k = adcBuf_humid[i];
      //  Z_k =pb[i];//测量值
        pbf = pb[i]*3.0/4096;
		Z_k = pbf;
        switch(seq)
        {
        case 1:
            kalman_adc = kalman_adc2;
            kalman_adc_old = kalman_adc2;
            P_k1_k1 = P_k_k2;
			//Z_k = adcBuf_humidf[i];	//湿度电压缓冲区
            break;
        case 2:
            kalman_adc = kalman_adc3;
            kalman_adc_old = kalman_adc3;
            P_k1_k1 = P_k_k3;
            break;
        case 3:
            kalman_adc = kalman_adc4;
            kalman_adc_old = kalman_adc4;
            P_k1_k1 = P_k_k4;
            break;
        }
        if (abs(kalman_adc_old-pbf)>=10)
        {
            x_k1_k1= pb[i]*0.382 + kalman_adc_old*0.618;
        }
        else
        {
            x_k1_k1 = kalman_adc_old;
        }
        x_k1_k1 = kalman_adc_old;

        x_k_k1 = x_k1_k1;
        P_k_k1 = P_k1_k1 + Q;

        Kg = P_k_k1/(P_k_k1 + R);

        kalman_adc = x_k_k1 + Kg * (Z_k - kalman_adc_old);
        P_k1_k1 = (1 - Kg)*P_k_k1;
        P_k_k1 = P_k1_k1;

        ADC_OLD_Value = pbf;//pb[i];
        kalman_adc_old = kalman_adc;
        switch(seq)
        {
        case 1:
            kalman_adc2 = kalman_adc ;
            P_k_k2 = P_k1_k1;
            break;
        case 2:
            kalman_adc3 = kalman_adc;
            P_k_k3 = P_k1_k1;
            break;
        case 3:
            kalman_adc4 = kalman_adc;
            P_k_k4 = P_k1_k1;
            break;
        }
        float error;
        error = ADC_OLD_Value - kalman_adc;
        //pb[i] = kalman_adc;
		return 	kalman_adc;
				//jansson_pack_test();
       // printf("%f             %f\n",ADC_OLD_Value,kalman_adc);  //输出测量累积误差

    }

}


// printf("总体测量误差      : %f\n",sumerror_measure);  //输出测量累积误差
// printf("总体卡尔曼滤波误差: %f\n",sumerror_kalman);   //输出kalman累积误差
// printf("卡尔曼误差所占比例: %d%% \n",100-(int)((sumerror_kalman/sumerror_measure)*100));

