#ifndef FuzzyPID_H
#define FuzzyPID_H
#define NB	-3 //��������ֵ
#define NM  -2
#define NS  -1
#define ZO   0
#define PS   1
#define PM   2 
#define PB   3 //��������ֵ

typedef struct 
{
    int  num_area; //�����������
    float e_membership_values[7]; //����e������ֵ
    float ec_membership_values[7];//����de/dt������ֵ3
    float kp_menbership_values[7];//�������kp������ֵ
    float ki_menbership_values[7]; //�������ki������ֵ
    float kd_menbership_values[7];  //�������kd������ֵ
    float fuzzyoutput_menbership_values[7] ;

    //int menbership_values[7] = {-3,-};
    float kp;                       //PID����kp
    float ki;                       //PID����ki
    float kd;                       //PID����kd
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
    float gradSums[7]  ;
    float KpgradSums[7];   //�������kp�ܵ�������
    float KigradSums[7] ;   //�������ki�ܵ�������
    float KdgradSums[7] ;   //�������kd�ܵ�������
    
  
		
		
  //  int  Kp_rule_list[7][7] ;//kp�����
    //int  Ki_rule_list[7][7] ;//ki����
    //int  Kd_rule_list[7][7];   //kd�����
   // int  Fuzzy_rule_list[7][7];
}FuzzyPID_stru;
void FuzzyPID(void);
void Get_grad_membership(float erro, float erro_c);
float Quantization(float maximum, float minimum, float x);
float Inverse_quantization(float maximum, float minimum, float qvalues);
void GetSumGrad(void);
void GetOUT(void);
void FuzzyPIDcontroller(float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro, float erro_c, float ki_max, float ki_min,float kd_max, float kd_min,float erro_pre, float errp_ppre);
void fuzzy_init(void);
FuzzyPID_stru *get_pid_params(void);


#endif


