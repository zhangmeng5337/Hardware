#include "FuzzyPID.h"

range rang;// = { 1000,-1000,800,-800,100,-100,0.1,-0.1,0.01,-0.01 };
Error error;//
 FuzzyPID myfuzzypid;

const int Kp_rule_list[7][7] =
{
    {PB, PB, PB, PB, PS, ZO, ZO}, 	//kp�����
    {PB, PB, PB, PM, ZO, ZO, NS},
    {PB, PB, PM, PS, ZO, NS, NM},
    {PB, PM, PS, ZO, NS, NM, NB},
    {PM, PS, ZO, NS, NM, NB, NB},
    {PS, ZO, ZO, NM, NB, NB, NB},
    {ZO, ZO, NS, NB, NB, NB, NB}
};

//const int Kp_rule_list[7][7] =
//{
//    {PB, PB, PB, PB, PM, PS, ZO}, 	//kp�����
//    {PB, PB, PM, PM, PS, ZO, NS},
//    {PB, PM, PM, PS, ZO, NS, NS},
//    {PM, PM, PS, ZO, NS, NM, NM},
//    {PS, PS, ZO, NS, NS, NM, NM},
//    {PS, ZO, NS, NM, NM, NM, NB},
//    {ZO, ZO, NM, NB, NM, NB, NB}
//};

const int Ki_rule_list[7][7] = { {NB, NM, NM, NM, NM, ZO, ZO}, 	//ki�����
    {NB, NB, NM, NS, NS, ZO, ZO},
    {NM, NM, NS, NS, ZO, PS, PS},
    {NM, NM, ZO, ZO, PS, PM, PS},
    {NS, NS, ZO, PS, PM, PM, PB},
    {ZO, ZO, PS, PM, PM, PB, PB},
    {ZO, ZO, PS, PM, PB, PB, PB}
};

const int Kd_rule_list[7][7] = { {NS, NS, NB, NS, NB, NM, NS}, //kd�����
    {ZO, NS, NB, NS, NM, NM, ZO},
    {ZO, NS, NM, NS, NS, NS, ZO},
    {ZO, ZO, NS, ZO, ZO, ZO, ZO},
    {ZO, ZO, ZO, ZO, ZO, ZO, PS},
    {PS, NS, PS, PS, ZO, PB, PM},
    {PB, PM, PM, PM, PS, PB, PB}
};

const int Fuzzy_rule_list[7][7] =
{
    {PB, PB, PB, PB, PM, ZO, ZO},
    {PB, PB, PB, PM, PM, ZO, ZO},
    {PB, PM, PM, PS, ZO, NS, NM},
    {PM, PM, PS, ZO, NS, NM, NM},
    {PS, PS, ZO, NM, NM, NM, NB},
    {ZO, ZO, ZO, NM, NB, NB, NB},
    {ZO, NS, NB, NB, NB, NB, NB}
};



int  Kp_rule[7][7] = { {PB,PB,PM,PM,PS,ZO,ZO},     //kp�����
    {PB,PB,PM,PS,PS,ZO,NS},
    {PM,PM,PM,PS,ZO,NS,NS},
    {PM,PM,PS,ZO,NS,NM,NM},
    {PS,PS,ZO,NS,NS,NM,NM},
    {PS,ZO,NS,NM,NM,NM,NB},
    {ZO,ZO,NM,NM,NM,NB,NB}
};

int  Ki_rule[7][7] = { {NB,NB,NM,NM,NS,ZO,ZO},     //ki�����
    {NB,NB,NM,NS,NS,ZO,ZO},
    {NB,NM,NS,NS,ZO,PS,PS},
    {NM,NM,NS,ZO,PS,PM,PM},
    {NM,NS,ZO,PS,PS,PM,PB},
    {ZO,ZO,PS,PS,PM,PB,PB},
    {ZO,ZO,PS,PM,PM,PB,PB}
};

int  Kd_rule[7][7] = { {PS,NS,NB,NB,NB,NM,PS},    //kd�����
    {PS,NS,NB,NM,NM,NS,ZO},
    {ZO,NS,NM,NM,NS,NS,ZO},
    {ZO,NS,NS,NS,NS,NS,ZO},
    {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
    {PB,NS,PS,PS,PS,PS,PB},
    {PB,PM,PM,PM,PS,PS,PB}
};

int  Fuzzy_rule[7][7] = { {PB,PB,PB,PB,PM,ZO,ZO},
    {PB,PB,PB,PM,PM,ZO,ZO},
    {PB,PM,PM,PS,ZO,NS,NM},
    {PM,PM,PS,ZO,NS,NM,NM},
    {PS,PS,ZO,NM,NM,NM,NB},
    {ZO,ZO,ZO,NM,NB,NB,NB},
    {ZO,NS,NB,NB,NB,NB,NB}
};
float values[7] = { -3,-2,-1,0,1,2,3 }; //����e������ֵ
void FuzzyPID_Init(FuzzyPID* pid)  //���캯��
{
    int i, j;
    pid->num_area = 8;
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->fuzzy_output = 0;
    pid->qdetail_kp = 0;
    pid->qdetail_ki = 0;
    pid->qdetail_kd = 0;
    pid->qfuzzy_output = 0;
    pid->errosum = 0;
//    for ( i = 0; i < 7; i++)
//    {
//        for ( j = 0; j < 7; j++)
//        {
//            pid->Kp_rule_list[i][j] = Kp_rule[i][j];
//            pid->Ki_rule_list[i][j] = Ki_rule[i][j];
//            pid->Kd_rule_list[i][j] = Kd_rule[i][j];
//            pid->Fuzzy_rule_list[i][j] = Fuzzy_rule[i][j];
//        }
//    }
    for ( i = 0; i < 7; i++)
    {
        pid->e_membership_values[i] = values[i];
        pid->ec_membership_values[i] = values[i];
        pid->kp_menbership_values[i] = values[i];
        pid->ki_menbership_values[i] = values[i];
        pid->kd_menbership_values[i] = values[i];
        pid->fuzzyoutput_menbership_values[i] = values[i];
        pid->gradSums[i] = 0;
        pid->KpgradSums[i] = 0;
        pid->KigradSums[i] = 0;
        pid->KdgradSums[i] = 0;
    }
}




//����e��de/dt�����ȼ��㺯��///
void Get_grad_membership(FuzzyPID* pid,float erro, float erro_c)
{
    int i;
    //������������Χ
    if (erro > pid->e_membership_values[0] && erro < pid->e_membership_values[6])
    {

        //6������
        for ( i = 0; i < pid->num_area - 2; i++)
        {
            //������������������
            if (erro >= pid->e_membership_values[i] && erro <= pid->e_membership_values[i + 1])
            {
                //e��������
                //PM
                pid->e_gradmembership[0] = -(erro - pid->e_membership_values[i + 1]) / (pid->e_membership_values[i + 1] - pid->e_membership_values[i]);
                //PB
                pid->e_gradmembership[1] = 1 + (erro - pid->e_membership_values[i + 1]) / (pid->e_membership_values[i + 1] - pid->e_membership_values[i]);
                //��¼����������������
                pid->e_grad_index[0] = i;
                pid->e_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
        //�������ֹС�ڵ����������Сֵ
        if (erro <= pid->e_membership_values[0])
        {
            pid->e_gradmembership[0] = 1;
            pid->e_gradmembership[1] = 0;
            pid->e_grad_index[0] = 0;
            pid->e_grad_index[1] = -1;
        }//������Χ��
        else if (erro >= pid->e_membership_values[6])
        {
            pid->e_gradmembership[0] = 1;
            pid->e_gradmembership[1] = 0;
            pid->e_grad_index[0] = 6;
            pid->e_grad_index[1] = -1;
        }
    }
    //����΢��
    if (erro_c > pid->ec_membership_values[0] && erro_c < pid->ec_membership_values[6])
    {
        for ( i = 0; i < pid->num_area - 2; i++)
        {
            if (erro_c >= pid->ec_membership_values[i] && erro_c <= pid->ec_membership_values[i + 1])
            {
                pid->ec_gradmembership[0] = -(erro_c - pid->ec_membership_values[i + 1]) / (pid->ec_membership_values[i + 1] - pid->ec_membership_values[i]);
                pid->ec_gradmembership[1] = 1 + (erro_c - pid->ec_membership_values[i + 1]) / (pid->ec_membership_values[i + 1] - pid->ec_membership_values[i]);
                pid->ec_grad_index[0] = i;
                pid->ec_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
        if (erro_c <= pid->ec_membership_values[0])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 0;
            pid->ec_grad_index[1] = -1;
        }
        else if (erro_c >= pid->ec_membership_values[6])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 6;
            pid->ec_grad_index[1] = -1;
        }
    }

}

// //��ȡ�������kp, ki, kd���������� /
void GetSumGrad(FuzzyPID* pid)
{
    int i,j;
    //���ְ˸�����
    for ( i = 0; i <= pid->num_area - 1; i++)
    {
        pid->KpgradSums[i] = 0;
        pid->KigradSums[i] = 0;
        pid->KdgradSums[i] = 0;
        //��PID�ĸ�������ֵ����
    }
    for ( i = 0; i < 2; i++)//ѭ������
    {
        if (pid->e_grad_index[i] == -1)//�����û�б���
        {
            continue;
        }
        for ( j = 0; j < 2; j++)//
        {
            if (pid->ec_grad_index[j] != -1)//����΢����û�б���
            {
                int indexKp = Kp_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
                int indexKi = Ki_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
                int indexKd = Kd_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
                //gradSums[index] = gradSums[index] + (e_gradmembership[i] * ec_gradmembership[j])* Kp_rule_list[e_grad_index[i]][ec_grad_index[j]];
                pid->KpgradSums[indexKp] = pid->KpgradSums[indexKp] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
                pid->KigradSums[indexKi] = pid->KigradSums[indexKi] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
                pid->KdgradSums[indexKd] = pid->KdgradSums[indexKd] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
            }
            else
            {
                continue;
            }

        }
    }

}

//�����������kp, kd, ki��Ӧ����ֵ//
void GetOUT(FuzzyPID* pid)
{
    int i;
    for ( i = 0; i < pid->num_area - 1; i++)
    {
        pid->qdetail_kp +=pid->kp_menbership_values[i] * pid->KpgradSums[i];
        pid->qdetail_ki += pid->ki_menbership_values[i] * pid->KigradSums[i];
        pid->qdetail_kd += pid->kd_menbership_values[i] * pid->KdgradSums[i];
    }
}

//ģ��PID����ʵ�ֺ���/
float u;
float FuzzyPIDcontroller(float Target, float actual)
{

    error.erro_ppre = error.erro_pre;
    error.erro_pre = error.erro;
	myfuzzypid.setvalue = Target;
	myfuzzypid.actual_value = actual;
    error.erro = myfuzzypid.setvalue-myfuzzypid.actual_value;
    error.erro_c = error.erro - error.erro_pre;
    
    //Arear_dipart(e_max, e_min, ec_max, ec_min, kp_max, kp_min,ki_max,ki_min,kd_max,kd_min);
    myfuzzypid.qerro = Quantization(rang.e_max, rang.e_min, -error.erro);//����ӳ��
    myfuzzypid.qerro_c = Quantization(rang.ec_max, rang.ec_min, -error.erro_c);//����ӳ��
    //��������С��0123��Χ��
    Get_grad_membership(&myfuzzypid,myfuzzypid.qerro, myfuzzypid.qerro_c);
    //��ȡ�������kp, ki, kd����������
    GetSumGrad(&myfuzzypid);
    //�����������kp, kd, ki��Ӧ����ֵ//
    GetOUT(&myfuzzypid);
    myfuzzypid.detail_kp = Inverse_quantization(rang.kp_max, rang.kp_min, myfuzzypid.qdetail_kp);
    myfuzzypid.detail_ki = Inverse_quantization(rang.ki_max, rang.ki_min, myfuzzypid.qdetail_ki);
    myfuzzypid.detail_kd = Inverse_quantization(rang.kd_max, rang.kd_min, myfuzzypid.qdetail_kd);
    myfuzzypid.qdetail_kd = 0;
    myfuzzypid.qdetail_ki = 0;
    myfuzzypid.qdetail_kp = 0;
    if (myfuzzypid.qdetail_kp >= rang.kp_max)
        myfuzzypid.qdetail_kp = rang.kp_max;
    else if (myfuzzypid.qdetail_kp <= rang.kp_min)
        myfuzzypid.qdetail_kp = rang.kp_min;
    if (myfuzzypid.qdetail_ki >= rang.ki_max)
        myfuzzypid.qdetail_ki = rang.ki_max;
    else if (myfuzzypid.qdetail_ki <= rang.ki_min)
        myfuzzypid.qdetail_ki = rang.ki_min;
    if (myfuzzypid.qdetail_kd >= rang.kd_max)
        myfuzzypid.qdetail_kd = rang.kd_max;
    else if (myfuzzypid.qdetail_kd <= rang.kd_min)
        myfuzzypid.qdetail_kd = rang.kd_min;
    myfuzzypid.kp =myfuzzypid.detail_kp;
    myfuzzypid.ki = myfuzzypid.ki + myfuzzypid.detail_ki;
    myfuzzypid.kd  =myfuzzypid.kd + myfuzzypid.detail_kd;
    //ȷ����Χ
    if (myfuzzypid.kp < 0)
        myfuzzypid.kp = 10;
	else   if (myfuzzypid.kp>=rang.kp_max)
        myfuzzypid.kp = rang.kp_max;
	
    if (myfuzzypid.ki < 0)
        myfuzzypid.ki = 0;
	else   if (myfuzzypid.ki>=rang.ki_max)
        myfuzzypid.ki = rang.ki_max;

    if (myfuzzypid.kd < 0)
        myfuzzypid.kd = 0;
	else   if (myfuzzypid.kd>=rang.kd_max)
        myfuzzypid.kd = rang.kd_max;

//    myfuzzypid.detail_kp = 0;
//    myfuzzypid.detail_ki = 0;
//    myfuzzypid.detail_kd = 0;
    //����ʽPID
	myfuzzypid.detalO = myfuzzypid.kp * (error.erro - error.erro_pre) + myfuzzypid.ki * error.erro + myfuzzypid.kd * (error.erro - 2 * error.erro_pre + error.erro_ppre);
	    //λ��ʽPID   
	myfuzzypid.kp_u = 4;
	myfuzzypid.ki_u = 0.01;		
	myfuzzypid.kd_u = 0.05;

    if(myfuzzypid.detalO_p>=myfuzzypid.out_max)
    {
      if(error.erro<0)
		myfuzzypid.errosum += error.erro;

	}
	else   if(myfuzzypid.detalO_p<=myfuzzypid.out_min)
    {
      if(error.erro>0)
		myfuzzypid.errosum += error.erro;

	}
	else
		myfuzzypid.errosum += error.erro;
		
    myfuzzypid.detalO_p = myfuzzypid.kp_u * error.erro  + myfuzzypid.ki_u * myfuzzypid.errosum + myfuzzypid.kd_u * (error.erro - error.erro_ppre);
	u = u + myfuzzypid.detalO;
	if(myfuzzypid.detalO_p>=myfuzzypid.out_max)
		myfuzzypid.detalO_p =myfuzzypid.out_max;

	if(myfuzzypid.detalO_p<=myfuzzypid.out_min)
		myfuzzypid.detalO_p =myfuzzypid.out_min;

	
	myfuzzypid.u1 = myfuzzypid.detalO_p;
	myfuzzypid.u2 = myfuzzypid.u2+ myfuzzypid.detalO;

	return 0;
}

///����ӳ�亯��///
float Quantization(float maximum, float minimum, float x)
{
    float qvalues = 6.0 * (x - minimum) / (maximum - minimum) - 3;
    //float qvalues=6.0*()
    return qvalues;

    //qvalues[1] = 3.0 * ecerro / (maximum - minimum);
}

//������ӳ�亯��
float Inverse_quantization(float maximum, float minimum, float qvalues)
{
    float x = (maximum - minimum) * (qvalues + 3) / 6 + minimum;
    return x;
}



void fuzzy_init(void)
{
    rang.e_max=3 ;//������Χ
    rang.e_min=-3 ;//�����С
    rang.ec_max=2;//����΢�ַ�Χ
    rang.ec_min=-2;//����΢�ַ�Χ
    rang.kp_max=50;//p�ķ�Χ
    rang.kp_min=0.1;
    rang.ki_max=0.5 ;//i�ķ�Χ
    rang.ki_min=0;
    rang.kd_max=0.5;//d�ķ�Χ
    rang.kd_min=0;
    rang.erro=0;//���
    rang.erro_c=0;//����΢��
    rang.erro_pre=0;//��һ�ε����
    rang.erro_ppre=0;//���ϴε����
    error.erro = 0;
    error.erro_c= 0;
    error.erro_pre= 0;
    error.erro_ppre= 0;
	myfuzzypid.out_max = 46;
	myfuzzypid.out_min = 15;
    FuzzyPID_Init(&myfuzzypid);
   // float u;
   // u = FuzzyPIDcontroller(Target, actual);
    //actual += u;



}




FuzzyPID *get_fuzzy_pid_params(void)
{
    return &myfuzzypid;

}




