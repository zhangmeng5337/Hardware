#include "fft.h"
#include "arm_math.h"
#include "uart.h"

fft_stru fft_usr;

// FFT计算函数
// *DATA: 导入待FFT计算的原始数组指针
// num：采集样本量
// N：需要保存的第几个数据点
float FFT_Calculation(float *DATA, int num, int N)
{
    float array_FFT_output[num];        //储存FFT变换后的512个数据
    float array_arm_cmplx_mag[num];     //储存FFT变换后的512个数据的幅值信息
    float array_f32[num];
	  uint16_t fftSize;
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, fftSize);        //初始化结构体S中的参数
    arm_rfft_fast_f32(&S, array_f32, array_FFT_output, 0);          //fft正变换 
    arm_cmplx_mag_f32(array_FFT_output, array_arm_cmplx_mag, num);  //计算幅值  
 
    return array_arm_cmplx_mag[N];  
}

// *Data: 导入待DFT计算的原始数组指针
// num：采集样本量
// N：需要保存的第几个数据点
float DFT_Calculation(unsigned char *Data, int num, int N,uint32_t samp_freq)
{
        unsigned int i;
        float SUM_Re = 0;        //实频数值
        float SUM_Im = 0;        //虚频数值
        float result = 0;            // 计算结果
        uint16_t tmp_result;
		unsigned int num_con;
		num_con = num/2;
        //FFT展开式
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
 
        //计算幅值

        return fft_usr.result;
}


