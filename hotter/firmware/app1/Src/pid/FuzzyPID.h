#ifndef FuzzyPID_H
#define FuzzyPID_H
#include "main.h"
#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3


typedef struct FuzzyPID
{
       int  num_area ; //�����������
    //float e_max;  //�������ֵ
    //float e_min;  //�����Сֵ
    //float ec_max;  //���仯���ֵ
    //float ec_min;  //���仯��Сֵ
    //float kp_max, kp_min;
    float e_membership_values[7] ; //����e������ֵ
    float ec_membership_values[7] ;//����de/dt������ֵ
    float kp_menbership_values[7] ;//�������kp������ֵ
    float ki_menbership_values[7] ; //�������ki������ֵ
    float kd_menbership_values[7] ;  //�������kd������ֵ
    float fuzzyoutput_menbership_values[7];

    //int menbership_values[7] = {-3,-};
    float kp;                       //PID����kp
    float ki;                       //PID����ki
    float kd;                       //PID����kd 
    float output;
	float detalO;
		float detalO_p;
    float qdetail_kp;               //����kp��Ӧ�����е�ֵ
    float qdetail_ki;               //����ki��Ӧ�����е�ֵ
    float qdetail_kd;               //����kd��Ӧ�����е�ֵ
    float qfuzzy_output;
    float detail_kp;                //�������kp
    float detail_ki;                //�������ki
    float detail_kd;                //�������kd
    float fuzzy_output;
    float qerro;                    //����e��Ӧ�����е�ֵ
    float qerro_c;                  //����de/dt��Ӧ�����е�ֵ
    float errosum;
    float e_gradmembership[2];      //����e��������
    float ec_gradmembership[2];     //����de/dt��������
    int e_grad_index[2];            //����e�������ڹ���������
    int ec_grad_index[2];           //����de/dt�������ڹ���������
    float gradSums[7] ;
    float KpgradSums[7];   //�������kp�ܵ�������
    float KigradSums[7] ;   //�������ki�ܵ�������
    float KdgradSums[7] ;   //�������kd�ܵ�������

    float kp_u;
    float ki_u;	
   float kd_u; 

   float u1,u2;
   float setvalue;
   float actual_value;
   float out_max;
   float out_min;
//    int  Kp_rule_list[7][7];
//
//    int  Ki_rule_list[7][7];
//
//    int  Kd_rule_list[7][7];
//
//    int  Fuzzy_rule_list[7][7];


    //private:

}FuzzyPID;
typedef struct
{
    float e_max ;//������Χ
    float e_min ;//�����С
    float ec_max;//����΢�ַ�Χ
    float ec_min;//����΢�ַ�Χ
    float kp_max;//p�ķ�Χ
    float kp_min;
    float ki_max ;//i�ķ�Χ
    float ki_min;
    float kd_max;//d�ķ�Χ
    float kd_min;
    float erro;//���
    float erro_c;//����΢��
    float erro_pre;//��һ�ε����
    float erro_ppre;//���ϴε����
}range;//�޸ı���

typedef struct
{
    float erro;
    float erro_c;
    float erro_pre;
    float erro_ppre;


}Error;

void FuzzyPID_Init(FuzzyPID* pid);
void Get_grad_membership(FuzzyPID* pid,float erro, float erro_c);
float Quantization(float maximum, float minimum, float x);
float Inverse_quantization(float maximum, float minimum, float qvalues);
void GetSumGrad(FuzzyPID* pid);
void GetOUT(FuzzyPID* pid);
float FuzzyPIDcontroller(float Target, float actual);

void fuzzy_init(void);
FuzzyPID *get_fuzzy_pid_params(void);




#endif


