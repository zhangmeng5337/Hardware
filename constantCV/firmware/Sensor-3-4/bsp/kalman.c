#include "kalman.h"
#include "stdio.h"


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
    kfp->Last_P = 50;
    kfp->Now_P = 0;
    kfp->out = 0;
    kfp->Kg = 0;
    kfp->Q = 0.01;
    kfp->R = 0.005;
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
    kfp->out = kfp->out + kfp->Kg * (input - kfp->out); // 因为这一次的预测值就是上一次的输出�?    // 更新协方差方�? 本次的系统协方差付给 kfp->LastP 威下一次运算准备�? 
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}
