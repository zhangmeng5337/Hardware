#include "fft.h"
#include "arm_math.h"
#include "uart.h"

fft_stru fft_usr;

// FFT���㺯��
// *DATA: �����FFT�����ԭʼ����ָ��
// num���ɼ�������
// N����Ҫ����ĵڼ������ݵ�
float FFT_Calculation(float *DATA, int num, int N)
{
    float array_FFT_output[num];        //����FFT�任���512������
    float array_arm_cmplx_mag[num];     //����FFT�任���512�����ݵķ�ֵ��Ϣ
    float array_f32[num];
	  uint16_t fftSize;
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, fftSize);        //��ʼ���ṹ��S�еĲ���
    arm_rfft_fast_f32(&S, array_f32, array_FFT_output, 0);          //fft���任 
    arm_cmplx_mag_f32(array_FFT_output, array_arm_cmplx_mag, num);  //�����ֵ  
 
    return array_arm_cmplx_mag[N];  
}

// *Data: �����DFT�����ԭʼ����ָ��
// num���ɼ�������
// N����Ҫ����ĵڼ������ݵ�
float DFT_Calculation(unsigned char *Data, int num, int N,uint32_t samp_freq)
{
        unsigned int i;
        float SUM_Re = 0;        //ʵƵ��ֵ
        float SUM_Im = 0;        //��Ƶ��ֵ
        float result = 0;            // ������
        uint16_t tmp_result;
		unsigned int num_con;
		num_con = num/2;
        //FFTչ��ʽ
        for(i=0;i<num_con;i++)
        {
            memcpy(&tmp_result,Data,2);
			result = tmp_result*3.0/4096;
			fft_usr.vol = result;

			
            SUM_Re = SUM_Re + fft_usr.vol*cos(2*3.1415926*i*i/num_con);
            SUM_Im = SUM_Im - fft_usr.vol*sin(2*3.1415926*i*i/num_con);        
			fft_usr.result = sqrt(SUM_Re*SUM_Re + SUM_Im*SUM_Im);
		   fft_usr.f = samp_freq*fft_usr.result/num_con/1000.0;
		  // printf("%.3f\r\n",fft_usr.f);
        }
 
        //�����ֵ

        return fft_usr.result;
}


