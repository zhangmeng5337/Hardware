#include "math.h"
double frand()
{
  ;//  return 2*((rand()/(double)RAND_MAX) - 0.5);  //随机噪声
 }
void Claman()
{
    float x_last=0;
    float p_last=0.02;
    float Q=0.018;
    float R=0.542;
    float kg;
    float x_mid;
    float x_now;
    float p_mid;
    float p_now;
    float z_real=0.56;//0.56
    float z_measure;
    float sumerror_kalman=0;
    float sumerror_measure=0;
    int i;
    x_last=z_real+frand()*0.03;
    x_mid=x_last;
    for(i=0; i<20; i++)
    {
        x_mid=x_last;    //x_last=x(k-1|k-1),x_mid=x(k|k-1)
        p_mid=p_last+Q;  //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
        kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
        z_measure=z_real+frand()*0.03;//测量值
        x_now=x_mid+kg*(z_measure-x_mid);//估计出的最优值
        p_now=(1-kg)*p_mid;//最优值对应的covariance
      //  printf("Real     position: %6.3f \n",z_real);  //显示真值
     //   printf("Mesaured position: %6.3f [diff:%.3f]\n",z_measure,fabs(z_real-z_measure));  //显示测量值以及真值与测量值之间的误差
      //  printf("Kalman   position: %6.3f [diff:%.3f]\n",x_now,fabs(z_real - x_now));  //显示kalman估计值以及真值和卡尔曼估计值的误差
        sumerror_kalman += fabs(z_real - x_now);  //kalman估计值的累积误差
        sumerror_measure += fabs(z_real-z_measure);  //真值与测量值的累积误差
        p_last = p_now;  //更新covariance值
        x_last = x_now;  //更新系统状态值
    }
  //  printf("总体测量误差      : %f\n",sumerror_measure);  //输出测量累积误差
   // printf("总体卡尔曼滤波误差: %f\n",sumerror_kalman);   //输出kalman累积误差
   // printf("卡尔曼误差所占比例: %d%% \n",100-(int)((sumerror_kalman/sumerror_measure)*100));
}

