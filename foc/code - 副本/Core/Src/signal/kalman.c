#include "kalman.h"
#include "stdio.h"
#include "string.h"
#include "string.h"

/**
 * @name: kalman_init
 * @msg: 卡尔曼滤波初始化
 * @param {kalman} *kfp 卡尔曼结构体参数
 * @return {*} �? * @author: TOTHTOT
 * @date:
 */

kalman kfp;
kalman_pb kfp_p[4];
/*
0:d
1:q
2:encoder
*/
void kalman_init(kalman *kfp)
{



    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        kfp_p[i].pb.Kg  = 0;
        kfp_p[i].pb.Last_P = 1000; // 10
        kfp_p[i].pb.Now_P = 0;
        kfp_p[i].pb.out = 0;
        kfp_p[i].pb.Kg = 0;
        kfp_p[i].pb.Q = 0.001;//0.001
        kfp_p[i].pb.R = 1;//1

    }
}
kalman *get_kalman_p(unsigned char i)
{
    return &(kfp_p[i].pb);
}


float kalman_filter(kalman *kfp, float input)
{
    // 预测协方差方程：k时刻系统估算协方�?= k-1时刻的系统协方差 + 过程噪声协方�?
    kfp->Now_P = kfp->Last_P + kfp->Q;
    // 卡尔曼增益方程：卡尔曼增�?= k时刻系统估算协方�?/ （k时刻系统估算协方�?+ 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // 更新最优值方程：k时刻状态变量的最优�?= 状态变量的预测�?+ 卡尔曼增�?* （测量�?- 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -
                                     kfp->out); //1?    // 更新协方差方�? 本次的系统协方差付给 kfp->LastP 威下一次运算准备�?
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}




