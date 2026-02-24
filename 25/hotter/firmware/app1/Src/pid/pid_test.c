
#include "FuzzyPID.h"
float Target = 600;
float actual = 0;
float e_max = 100;
float e_min = -50;
float ec_max = 800;
float ec_min = -800;
float kp_max = 100;
float kp_min = -100;
float ki_max = 0.1;
float ki_min = -0.1;
float kd_max = 0.01;
float kd_min = -0.01;
float erro;
float erro_c;
float erro_pre = 0;
float erro_ppre = 0;
//  erro =Target - actual;
//    erro_c = erro - erro_pre;
//#include "stdio.h"
//extern FuzzyPID_stru FuzzyPID_usr;
void fuzzy_initp()
{


    fuzzy_init();

    //u = FuzzyPIDcontroller(e_max, e_min, ec_max, ec_min, kp_max, kp_min, erro, erro_c,ki_max,ki_min,kd_max,kd_min,erro_pre,erro_ppre);

    //printf("i:%d  target:%f  actual:%f  kp:%f  ki:%f  kd:%f\r\n",i,Target,actual,FuzzyPID_usr.detail_kp,FuzzyPID_usr.detail_ki,FuzzyPID_usr.detail_kd);
}
void fuzzy_proc()
{
    float u;
    u = FuzzyPIDcontroller(e_max, e_min, ec_max, ec_min, kp_max, kp_min, erro, erro_c, ki_max, ki_min, kd_max, kd_min,
                           erro_pre, erro_ppre);
    actual += u;
    erro_ppre = erro_pre;
    erro_pre = erro;
    erro = Target - actual;
    erro_c = erro - erro_pre;
}


