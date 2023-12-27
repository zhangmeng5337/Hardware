#ifndef FuzzyPID_H
#define FuzzyPID_H
#define NB	-3 //论域隶属值
#define NM  -2
#define NS  -1
#define ZO   0
#define PS   1
#define PM   2 
#define PB   3 //论域隶属值

typedef struct 
{
    int  num_area; //划分区域个数
    float e_membership_values[7]; //输入e的隶属值
    float ec_membership_values[7];//输入de/dt的隶属值3
    float kp_menbership_values[7];//输出增量kp的隶属值
    float ki_menbership_values[7]; //输出增量ki的隶属值
    float kd_menbership_values[7];  //输出增量kd的隶属值
    float fuzzyoutput_menbership_values[7] ;

    //int menbership_values[7] = {-3,-};
    float kp;                       //PID参数kp
    float ki;                       //PID参数ki
    float kd;                       //PID参数kd
    float qdetail_kp;               //增量kp对应论域中的值
    float qdetail_ki;               //增量ki对应论域中的值
    float qdetail_kd;               //增量kd对应论域中的值
    float qfuzzy_output;  
    float detail_kp;                //输出增量kp
    float detail_ki;                //输出增量ki
    float detail_kd;                //输出增量kd
    float fuzzy_output;
    float qerro;                    //输入e对应论域中的值
    float qerro_c;                  //输入de/dt对应论域中的值
    float errosum;                  
    float e_gradmembership[2];      //输入e的隶属度
    float ec_gradmembership[2];     //输入de/dt的隶属度
    int e_grad_index[2];            //输入e隶属度在规则表的索引
    int ec_grad_index[2];           //输入de/dt隶属度在规则表的索引
    float gradSums[7]  ;
    float KpgradSums[7];   //输出增量kp总的隶属度
    float KigradSums[7] ;   //输出增量ki总的隶属度
    float KdgradSums[7] ;   //输出增量kd总的隶属度
    
  
		
		
  //  int  Kp_rule_list[7][7] ;//kp规则表
    //int  Ki_rule_list[7][7] ;//ki规则
    //int  Kd_rule_list[7][7];   //kd规则表
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


