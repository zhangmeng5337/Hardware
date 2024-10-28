#include "config.h"
#include "FuzzyPID.h"
#include "pid.h"
#include "ai_proc.h"
indoor_temp_stru indoor_temp_usr;


float Target;
float actual = 0;
float e_max = 60;
float e_min = -50;
float ec_max = 110;
float ec_min = -110;
float kp_max = 50;
float kp_min = 10;


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
void get_temp_cal(float *buf)
{
    indoor_temp_usr.temp_average = buf[0]; //
    indoor_temp_usr.low_temp_percent =  buf[1];
    indoor_temp_usr.temp_error = 	buf[2];
}
float get_pid_output()
{
    float  u;
//    if (indoor_temp_usr.output >= PID_MIN && indoor_temp_usr.output <= PID_MAX)//换算成设备温�?
//        u = MACHINE_RATIO * indoor_temp_usr.output + MACHINE_RATIO_B;
//    else
//    {
//        if (indoor_temp_usr.output < PID_MIN) //换算成设备温�?
//            u = MACHINE_MIN_T;
//        if (indoor_temp_usr.output > PID_MAX)//换算成设备温�?
//            u = MACHINE_MAX_T;
//
//    }

        u =  indoor_temp_usr.output ;



    return u;
}
void pid_init()
{

    // Target = get_config()->set_tindoor;
    fuzzy_init();

    //u = FuzzyPIDcontroller(e_max, e_min, ec_max, ec_min, kp_max, kp_min, erro, erro_c,ki_max,ki_min,kd_max,kd_min,erro_pre,erro_ppre);

    //printf("i:%d  target:%f  actual:%f  kp:%f  ki:%f  kd:%f\r\n",i,Target,actual,FuzzyPID_usr.detail_kp,FuzzyPID_usr.detail_ki,FuzzyPID_usr.detail_kd);
}
void pid_cal(unsigned char mode)
{
    float u;
//    indoor_temp_usr.output = get_pid_params()->kp * (erro - erro_pre) + get_pid_params()->ki * erro +
//                             get_pid_params()->kd * (erro - 2 * erro_pre + erro_ppre);
    indoor_temp_usr.output = get_pid_params()->kp ;

    erro_ppre = erro_pre;
    erro_pre = erro;
    if (mode == 0)//智能控制 smart ctrl
    {
        if (indoor_temp_usr.temp_average >= (0.95 * get_config()->set_tindoor)) //平均温度达标
        {
            if (indoor_temp_usr.low_temp_percent >= 0.2) //末端温度不达�?
            {
                float tmp;
                tmp = 1 - indoor_temp_usr.low_temp_percent;
                tmp = tmp * indoor_temp_usr.temp_average;
                erro = get_config()->set_tindoor - tmp;//温度加权
            }
            else

                erro = get_config()->set_tindoor - indoor_temp_usr.temp_average;
        }
        else
            erro = get_config()->set_tindoor - indoor_temp_usr.temp_average;

    }
    else  //本地控制  native ctrl
    {
        erro = MACHINE_IN_T - get_ai_data()->temp[AI_WATER_T_IN_INDEX];//温度加权
    }

    erro_c = erro - erro_pre;
    actual += u;
}
void fuzzy_proc(unsigned char mode)
{

    FuzzyPIDcontroller(e_max, e_min, ec_max, ec_min, kp_max, kp_min, erro, erro_c, ki_max, ki_min, kd_max, kd_min,
                       erro_pre, erro_ppre);
    pid_cal(mode);
}


