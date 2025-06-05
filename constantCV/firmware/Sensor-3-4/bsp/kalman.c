#include "kalman.h"
#include "stdio.h"
static  float   uiChannel1Buffer[ADC_CAPTURE];


float  GetMedianNum(float bArray)
{
    static int i, j; // 循环变量
    float bTemp;
    static float   buf[ADC_CAPTURE];

    // 用冒泡法对数组进行排序
    if(j<ADC_CAPTURE)
    {
	buf[j] = bArray;
    j++;
	        // 数组有偶数个元素，返回中间两个元素平均值
        bTemp = (buf[j / 2] + buf[j / 2 + 1]) / 2;
	}
	else 
	{
    for (j = 0; j < ADC_CAPTURE - 1; j ++)
    {
        for (i = 0; i < ADC_CAPTURE - j - 1; i ++)
        {
            if (buf[i] > buf[i + 1])
            {
                // 互换
                bTemp = buf[i];
                buf[i] = buf[i + 1];
                buf[i + 1] = bTemp;
            }
        }
    }
	j = 0;
        // 数组有奇数个元素，返回中间一个元素
        bTemp = buf[(ADC_CAPTURE + 1) / 2];
	}
  bTemp = SilderFilter(bTemp);

   return bTemp;
}


float SilderFilter(float _value)
{
    char i;
    static unsigned char count = 0;
    float ulChannel1AdcValue;
    ulChannel1AdcValue = 0;
    if (count < ADC_CAPTURE)
        count ++;
    for (char i = 0; i < (ADC_CAPTURE - 1); i++)
    {

        uiChannel1Buffer[i] = uiChannel1Buffer[i + 1];


    }

    uiChannel1Buffer[ADC_CAPTURE - 1] = _value;

    for (char i = 0; i < ADC_CAPTURE; i++)
    {
        ulChannel1AdcValue = uiChannel1Buffer[i] + ulChannel1AdcValue;
    }

    if (count >= ADC_CAPTURE)
    {
        ulChannel1AdcValue = ulChannel1AdcValue / ADC_CAPTURE;

    }
    else
    {
        ulChannel1AdcValue = ulChannel1AdcValue / count;
    }
    return ulChannel1AdcValue;

}




//kalman *kalman_kfp_get(void)
//{
//   return g_kfp_st;
//}

/**
 * @name: kalman_init
 * @msg: 卡尔曼滤波初始化
 * @param {kalman} *kfp 卡尔曼结构体参数
 * @return {*} �? * @author: TOTHTOT
 * @date:
 */
void kalman_init(kalman *kfp)
{
    kfp->Last_P = 100;
    kfp->Now_P = 0;
    kfp->out = 0;
    kfp->Kg = 0;
    kfp->Q = 0.001;
    kfp->R = 0.01;
}

/**
 * @name: kalman_filter
 * @msg: 卡尔曼滤波器
 * @param {kalman} *kfp 卡尔曼结构体参数
 * @param {float} input 需要滤波的参数的测量值（即传感器的采集值）
 * @return {*} 滤波后的参数（最优值）
 * @author: TOTHTOT
 * @date:
 */
float kalman_filter(kalman *kfp, float input)
{
    // 预测协方差方程：k时刻系统估算协方�?= k-1时刻的系统协方差 + 过程噪声协方�?
    kfp->Now_P = kfp->Last_P + kfp->Q;
    // 卡尔曼增益方程：卡尔曼增�?= k时刻系统估算协方�?/ （k时刻系统估算协方�?+ 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // 更新最优值方程：k时刻状态变量的最优�?= 状态变量的预测�?+ 卡尔曼增�?* （测量�?- 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -
                                     kfp->out); // 因为这一次的预测值就是上一次的输出�?    // 更新协方差方�? 本次的系统协方差付给 kfp->LastP 威下一次运算准备�?
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}
