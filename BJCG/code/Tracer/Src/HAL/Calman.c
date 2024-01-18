#include "math.h"
#include "calman.h"
#include "sensors.h"
//extern  float adcBuf_ref[N];
//extern	float adcBuf_humid[N];
//extern	float adcBuf_humidf[N];

//extern	float adcBuf_ta[N];
//extern	float adcBuf_tb[N];
#define RAND_MAX  0.5
void jansson_pack_test(void);
;void jansson_unpack_test(void);
/************************************************
r���������˲����������ʵ�����ߵ�����̶ȣ�rԽСԽ�ӽ���

q�������˲��������ƽ���̶ȣ�qԽСԽƽ����
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
    static float P_k_k1 = 1,P_k_k2[SN];

    static float kalman_adc,kalman_adc2[SN];
    static float kalman_adc_old=1;
    unsigned int i;
    Q = sq;
    R = sr;
    kalman_adc_old = pb[0];
	
    for(i=0; i<1; i++)
    {
			  //Z_k = 0;
        //Z_k = adcBuf_humid[i];
      //  Z_k =pb[i];//����ֵ
        pbf = (pb[0]);//*3.0/4096;
		Z_k = pbf;

            kalman_adc = kalman_adc2[seq-1];
            kalman_adc_old = kalman_adc2[seq-1];
			if(kalman_adc ==0)
				kalman_adc= pb[0];
			if(kalman_adc_old ==0)
				kalman_adc_old= pb[0];

            P_k1_k1 = P_k_k2[seq-1];
        if (fabs(kalman_adc_old-pbf)>=10)
        {
            x_k1_k1= pb[0]*0.382 + kalman_adc_old*0.618;
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
				
        kalman_adc2[seq-1] = kalman_adc  ;
        P_k_k2[seq-1]= P_k1_k1 ;

//        float error;
//        error = ADC_OLD_Value - kalman_adc;
        //pb[i] = kalman_adc;
		return 	kalman_adc;
				//jansson_pack_test();
       // printf("%f             %f\n",ADC_OLD_Value,kalman_adc);  //��������ۻ����

    }

}



// printf("����������      : %f\n",sumerror_measure);  //��������ۻ����
// printf("���忨�����˲����: %f\n",sumerror_kalman);   //���kalman�ۻ����
// printf("�����������ռ����: %d%% \n",100-(int)((sumerror_kalman/sumerror_measure)*100));

