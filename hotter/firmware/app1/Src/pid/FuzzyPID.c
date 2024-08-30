#include "FuzzyPID.h"

FuzzyPID_stru FuzzyPID_usr;
const int Kp_rule_list[7][7] =
{
    {PB, PB, PB, PB, PS, ZO, ZO}, 	//kp规则表
    {PB, PB, PB, PM, ZO, ZO, NS},
    {PB, PB, PM, PS, ZO, NS, NM},
    {PB, PM, PS, ZO, NS, NM, NB},
    {PM, PS, ZO, NS, NM, NB, NB},
    {PS, ZO, ZO, NM, NB, NB, NB},
    {ZO, ZO, NS, NB, NB, NB, NB}
};

//const int Kp_rule_list[7][7] =
//{
//    {PB, PB, PB, PB, PM, PS, ZO}, 	//kp规则表
//    {PB, PB, PM, PM, PS, ZO, NS},
//    {PB, PM, PM, PS, ZO, NS, NS},
//    {PM, PM, PS, ZO, NS, NM, NM},
//    {PS, PS, ZO, NS, NS, NM, NM},
//    {PS, ZO, NS, NM, NM, NM, NB},
//    {ZO, ZO, NM, NB, NM, NB, NB}
//};

const int Ki_rule_list[7][7] = { {NB, NM, NM, NM, NM, ZO, ZO}, 	//ki规则表
    {NB, NB, NM, NS, NS, ZO, ZO},
    {NM, NM, NS, NS, ZO, PS, PS},
    {NM, NM, ZO, ZO, PS, PM, PS},
    {NS, NS, ZO, PS, PM, PM, PB},
    {ZO, ZO, PS, PM, PM, PB, PB},
    {ZO, ZO, PS, PM, PB, PB, PB}
};

const int Kd_rule_list[7][7] = { {NS, NS, NB, NS, NB, NM, NS}, //kd规则表
    {ZO, NS, NB, NS, NM, NM, ZO},
    {ZO, NS, NM, NS, NS, NS, ZO},
    {ZO, ZO, NS, ZO, ZO, ZO, ZO},
    {ZO, ZO, ZO, ZO, ZO, ZO, PS},
    {PS, NS, PS, PS, ZO, PB, PM},
    {PB, PM, PM, PM, PS, PB, PB}
};

const int Fuzzy_rule_list[7][7] = { {PB, PB, PB, PB, PM, ZO, ZO},
    {PB, PB, PB, PM, PM, ZO, ZO},
    {PB, PM, PM, PS, ZO, NS, NM},
    {PM, PM, PS, ZO, NS, NM, NM},
    {PS, PS, ZO, NM, NM, NM, NB},
    {ZO, ZO, ZO, NM, NB, NB, NB},
    {ZO, NS, NB, NB, NB, NB, NB}
};


void FuzzyPID()  //构造函数
{
    FuzzyPID_usr.kp = 0;
    FuzzyPID_usr.ki = 0;
    FuzzyPID_usr.kd = 0;
    FuzzyPID_usr.fuzzy_output = 0;
    FuzzyPID_usr.qdetail_kp = 0;
    FuzzyPID_usr.qdetail_ki = 0;
    FuzzyPID_usr.qdetail_kd = 0;
    FuzzyPID_usr.qfuzzy_output = 0;
    FuzzyPID_usr.errosum = 0;
}

//////////////////////输入e与de/dt隶属度计算函数///////////////////////////
void Get_grad_membership(float erro, float erro_c)
{
    if (erro > FuzzyPID_usr.e_membership_values[0] && erro < FuzzyPID_usr.e_membership_values[6])
    {
        for (int i = 0; i < FuzzyPID_usr.num_area - 2; i++)
        {
            if (erro >= FuzzyPID_usr.e_membership_values[i] && erro <= FuzzyPID_usr.e_membership_values[i + 1])
            {
                FuzzyPID_usr.e_gradmembership[0] = -(erro - FuzzyPID_usr.e_membership_values[i + 1]) /
                                                   (FuzzyPID_usr.e_membership_values[i + 1] - FuzzyPID_usr.e_membership_values[i]);
                FuzzyPID_usr.e_gradmembership[1] = 1 + (erro - FuzzyPID_usr.e_membership_values[i + 1]) /
                                                   (FuzzyPID_usr.e_membership_values[i + 1] - FuzzyPID_usr.e_membership_values[i]);
                FuzzyPID_usr.e_grad_index[0] = i;
                FuzzyPID_usr.e_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
        if (erro <= FuzzyPID_usr.e_membership_values[0])
        {
            FuzzyPID_usr.e_gradmembership[0] = 1;
            FuzzyPID_usr.e_gradmembership[1] = 0;
            FuzzyPID_usr.e_grad_index[0] = 0;
            FuzzyPID_usr.e_grad_index[1] = -1;
        }
        else if (erro >= FuzzyPID_usr.e_membership_values[6])
        {
            FuzzyPID_usr.e_gradmembership[0] = 1;
            FuzzyPID_usr.e_gradmembership[1] = 0;
            FuzzyPID_usr.e_grad_index[0] = 6;
            FuzzyPID_usr.e_grad_index[1] = -1;
        }
    }

    if (erro_c > FuzzyPID_usr.ec_membership_values[0] && erro_c < FuzzyPID_usr.ec_membership_values[6])
    {
        for (int i = 0; i < FuzzyPID_usr.num_area - 2; i++)
        {
            if (erro_c >= FuzzyPID_usr.ec_membership_values[i] && erro_c <= FuzzyPID_usr.ec_membership_values[i + 1])
            {
                FuzzyPID_usr.ec_gradmembership[0] = -(erro_c - FuzzyPID_usr.ec_membership_values[i + 1]) /
                                                    (FuzzyPID_usr.ec_membership_values[i + 1] - FuzzyPID_usr.ec_membership_values[i]);
                FuzzyPID_usr.ec_gradmembership[1] = 1 + (erro_c - FuzzyPID_usr.ec_membership_values[i + 1]) /
                                                    (FuzzyPID_usr.ec_membership_values[i + 1] - FuzzyPID_usr.ec_membership_values[i]);
                FuzzyPID_usr.ec_grad_index[0] = i;
                FuzzyPID_usr.ec_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
        if (erro_c <= FuzzyPID_usr.ec_membership_values[0])
        {
            FuzzyPID_usr.ec_gradmembership[0] = 1;
            FuzzyPID_usr.ec_gradmembership[1] = 0;
            FuzzyPID_usr.ec_grad_index[0] = 0;
            FuzzyPID_usr.ec_grad_index[1] = -1;
        }
        else if (erro_c >= FuzzyPID_usr.ec_membership_values[6])
        {
            FuzzyPID_usr.ec_gradmembership[0] = 1;
            FuzzyPID_usr.ec_gradmembership[1] = 0;
            FuzzyPID_usr.ec_grad_index[0] = 6;
            FuzzyPID_usr.ec_grad_index[1] = -1;
        }
    }

}

/////////////////////获取输出增量kp,ki,kd的总隶属度/////////////////////////////
void GetSumGrad()
{
    for (int i = 0; i <= FuzzyPID_usr.num_area - 1; i++)
    {
        FuzzyPID_usr. KpgradSums[i] = 0;
        FuzzyPID_usr.KigradSums[i] = 0;
        FuzzyPID_usr.KdgradSums[i] = 0;

    }
    for (int i = 0; i < 2; i++)
    {
        if (FuzzyPID_usr.e_grad_index[i] == -1)
        {
            continue;
        }
        for (int j = 0; j < 2; j++)
        {
            if (FuzzyPID_usr.ec_grad_index[j] != -1)
            {
                int indexKp = Kp_rule_list[FuzzyPID_usr.e_grad_index[i]][FuzzyPID_usr.ec_grad_index[j]] + 3;
                int indexKi = Ki_rule_list[FuzzyPID_usr.e_grad_index[i]][FuzzyPID_usr.ec_grad_index[j]] + 3;
                int indexKd = Kd_rule_list[FuzzyPID_usr.e_grad_index[i]][FuzzyPID_usr.ec_grad_index[j]] + 3;
                //gradSums[index] = gradSums[index] + (e_gradmembership[i] * ec_gradmembership[j])* Kp_rule_list[e_grad_index[i]][ec_grad_index[j]];
                FuzzyPID_usr.KpgradSums[indexKp] = FuzzyPID_usr.KpgradSums[indexKp] + (FuzzyPID_usr.e_gradmembership[i] *
                                                   FuzzyPID_usr.ec_gradmembership[j]);
                FuzzyPID_usr.KigradSums[indexKi] = FuzzyPID_usr.KigradSums[indexKi] + (FuzzyPID_usr.e_gradmembership[i] *
                                                   FuzzyPID_usr.ec_gradmembership[j]);
                FuzzyPID_usr.KdgradSums[indexKd] = FuzzyPID_usr.KdgradSums[indexKd] + (FuzzyPID_usr.e_gradmembership[i] *
                                                   FuzzyPID_usr.ec_gradmembership[j]);
            }
            else
            {
                continue;
            }

        }
    }

}

////////////////////计算输出增量kp,kd,ki对应论域值//////////////////////
void GetOUT()
{
    for (int i = 0; i < FuzzyPID_usr.num_area - 1; i++)
    {
        FuzzyPID_usr.qdetail_kp += FuzzyPID_usr.kp_menbership_values[i] * FuzzyPID_usr.KpgradSums[i];
        FuzzyPID_usr.qdetail_ki += FuzzyPID_usr.ki_menbership_values[i] * FuzzyPID_usr.KigradSums[i];
        FuzzyPID_usr.qdetail_kd += FuzzyPID_usr.kd_menbership_values[i] * FuzzyPID_usr.KdgradSums[i];
    }
}

//////////////////模糊PID控制实现函数/////////////////////////
void FuzzyPIDcontroller(float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro,
                         float erro_c, float ki_max, float ki_min, float kd_max, float kd_min, float erro_pre, float errp_ppre)
{
    FuzzyPID_usr.errosum += erro;
    //Arear_dipart(e_max, e_min, ec_max, ec_min, kp_max, kp_min,ki_max,ki_min,kd_max,kd_min);
    FuzzyPID_usr.qerro = Quantization(e_max, e_min, erro);
    FuzzyPID_usr.qerro_c = Quantization(ec_max, ec_min, erro_c);
    Get_grad_membership(FuzzyPID_usr.qerro, FuzzyPID_usr.qerro_c);
    GetSumGrad();
    GetOUT();
    FuzzyPID_usr.detail_kp = Inverse_quantization(kp_max, kp_min, FuzzyPID_usr.qdetail_kp);
    FuzzyPID_usr.detail_ki = Inverse_quantization(ki_max, ki_min, FuzzyPID_usr.qdetail_ki);
    FuzzyPID_usr.detail_kd = Inverse_quantization(kd_max, kd_min, FuzzyPID_usr.qdetail_kd);
    FuzzyPID_usr.qdetail_kd = 0;
    FuzzyPID_usr.qdetail_ki = 0;
    FuzzyPID_usr.qdetail_kp = 0;

    FuzzyPID_usr.kp = FuzzyPID_usr.kp + FuzzyPID_usr.detail_kp;
    FuzzyPID_usr.ki = FuzzyPID_usr.ki + FuzzyPID_usr.detail_ki;
    FuzzyPID_usr.kd = FuzzyPID_usr.kd + FuzzyPID_usr.detail_kd;
    if (FuzzyPID_usr.kp < 0)
        FuzzyPID_usr.kp = 0;
    if (FuzzyPID_usr.ki < 0)
        FuzzyPID_usr.ki = 0;
    if (FuzzyPID_usr.kd < 0)
        FuzzyPID_usr.kd = 0;
    FuzzyPID_usr.detail_kp = 0;
    FuzzyPID_usr.detail_ki = 0;
    FuzzyPID_usr.detail_kd = 0;

    //return output;
}

///////////////////////////////区间映射函数///////////////////////////////////////////
float Quantization(float maximum, float minimum, float x)
{
    float qvalues = 6.0 * (x - minimum) / (maximum - minimum) - 3;
    //float qvalues=6.0*()
    return qvalues;

    //qvalues[1] = 3.0 * ecerro / (maximum - minimum);
}

//////////////////////////////反区间映射函数////////////////////////////////////////////
float Inverse_quantization(float maximum, float minimum, float qvalues)
{
    float x = (maximum - minimum) * (qvalues + 3) / 6 + minimum;
    return x;
}

void fuzzy_init()
{
    float membership_values[7] = {-3, -2, -1, 0, 1, 2, 3};
    FuzzyPID_usr.num_area = 8; //划分区域个数

    memcpy(FuzzyPID_usr.e_membership_values, membership_values, 7); //输入e的隶属值
    memcpy(FuzzyPID_usr.ec_membership_values, membership_values, 7);		//输入de/dt的隶属值
    memcpy(FuzzyPID_usr.kp_menbership_values, membership_values, 7); //输出增量kp的隶属值
    memcpy(FuzzyPID_usr.ki_menbership_values, membership_values, 7); //输出增量ki的隶属值
    memcpy(FuzzyPID_usr.kd_menbership_values, membership_values, 7); //输出增量kd的隶属值
    memcpy(FuzzyPID_usr.fuzzyoutput_menbership_values, membership_values, 7);

    memcpy(FuzzyPID_usr.gradSums, 0, 7);
    memcpy(FuzzyPID_usr.KpgradSums, 0, 7); //输出增量kp总的隶属度
    memcpy(FuzzyPID_usr.KigradSums, 0, 7); //输出增量ki总的隶属度
    memcpy(FuzzyPID_usr.KdgradSums, 0, 7); //输出增量kd总的隶属度
}
FuzzyPID_stru *get_pid_params()
{
 return &FuzzyPID_usr;
}

