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
       int  num_area ; //划分区域个数
    //float e_max;  //误差做大值
    //float e_min;  //误差最小值
    //float ec_max;  //误差变化最大值
    //float ec_min;  //误差变化最小值
    //float kp_max, kp_min;
    float e_membership_values[7] ; //输入e的隶属值
    float ec_membership_values[7] ;//输入de/dt的隶属值
    float kp_menbership_values[7] ;//输出增量kp的隶属值
    float ki_menbership_values[7] ; //输出增量ki的隶属值
    float kd_menbership_values[7] ;  //输出增量kd的隶属值
    float fuzzyoutput_menbership_values[7];

    //int menbership_values[7] = {-3,-};
    float kp;                       //PID参数kp
    float ki;                       //PID参数ki
    float kd;                       //PID参数kd 
    float output;
	float detalO;
		float detalO_p;
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
    float gradSums[7] ;
    float KpgradSums[7];   //输出增量kp总的隶属度
    float KigradSums[7] ;   //输出增量ki总的隶属度
    float KdgradSums[7] ;   //输出增量kd总的隶属度

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
    float e_max ;//误差最大范围
    float e_min ;//误差最小
    float ec_max;//误差的微分范围
    float ec_min;//误差的微分范围
    float kp_max;//p的范围
    float kp_min;
    float ki_max ;//i的范围
    float ki_min;
    float kd_max;//d的范围
    float kd_min;
    float erro;//误差
    float erro_c;//误差的微分
    float erro_pre;//上一次的误差
    float erro_ppre;//上上次的误差
}range;//修改变量

typedef struct
{
    float erro;
    float erro_c;
    float erro_pre;
    float erro_ppre;


}Error;
extern FuzzyPID myfuzzypid;
void FuzzyPID_Init(FuzzyPID* pid);
void Get_grad_membership(FuzzyPID* pid,float erro, float erro_c);
float Quantization(float maximum, float minimum, float x);
float Inverse_quantization(float maximum, float minimum, float qvalues);
void GetSumGrad(FuzzyPID* pid);
void GetOUT(FuzzyPID* pid);
float FuzzyPIDcontroller(float Target, float actual);

void fuzzy_init(void);
FuzzyPID *get_fuzzy_pid_params(void);

FuzzyPID *get_pid_params(void);



#endif


