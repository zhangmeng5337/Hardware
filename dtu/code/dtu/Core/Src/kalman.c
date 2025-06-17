#include "kalman.h"
#include "stdio.h"


//kalman *kalman_kfp_get(void)
//{
//   return g_kfp_st;
//}

/**
 * @name: kalman_init
 * @msg: �������˲���ʼ��
 * @param {kalman} *kfp �������ṹ�����
 * @return {*} �J * @author: TOTHTOT
 * @date:
 */
void kalman_init(kalman *kfp)
{
    kfp->Last_P = 50;
    kfp->Now_P = 0;
    kfp->out = 0;
    kfp->Kg = 0;
    kfp->Q = 0.001;
    kfp->R = 0.05;
}

/**
 * @name: kalman_filter
 * @msg: �������˲���
 * @param {kalman} *kfp �������ṹ�����
 * @param {float} input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 * @return {*} �˲���Ĳ���������ֵ��
 * @author: TOTHTOT
 * @date:
 */
float kalman_filter(kalman *kfp, float input)
{
    // Ԥ��Э����̣�kʱ��ϵͳ����Э����= k-1ʱ�̵�ϵͳЭ���� + ��������Э����    
	kfp->Now_P = kfp->Last_P + kfp->Q;
    // ���������淽�̣����������a= kʱ��ϵͳ����Э����/ ��kʱ��ϵͳ����Э����+ �۲�����Э���
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // ��������ֵ���̣�kʱ��״̬���������ł�= ״̬������Ԥ�₞+ ���������a* ��������- ״̬������Ԥ��ֵ��
    kfp->out = kfp->out + kfp->Kg * (input - kfp->out); // ��Ϊ��һ�ε�Ԥ��ֵ������һ�ε������    // ����Э����� ���ε�ϵͳЭ����� kfp->LastP ����һ������׼��? 
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}
