#include "config.h"
#include "FuzzyPID.h"
#include "pid.h"
#include "ai_proc.h"
#include "schedule.h"

indoor_temp_stru indoor_temp_usr;



float Target;
float actual = 0;
float e_max = 40;
float e_min = 0;
float ec_max = 40;
float ec_min = -40;
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
    indoor_temp_usr.temp_error =    buf[2];
}
indoor_temp_stru *set_indoor_temp()
{
	return &indoor_temp_usr;
}
float get_indoor_temp()
{
	return indoor_temp_usr.temp_average;
}

float get_pid_output()
{
    float  u;
    if (get_schedule()->mode == 1&&get_config()->mode == NATIVE_MODE)//ctrl water 
    {

//        u = get_schedule()->buf[get_schedule()->current_plan].temperature;
		u = get_fuzzy_pid_params()->u1;
    }
    else  if (get_config()->mode == SMART_MODE)//ctrl indoor temp
    {

        u = get_fuzzy_pid_params()->u1;
    }
    else  if (get_config()->mode == SCHE_MODE)//ctrl schedule temp
    {

        u = get_schedule()->buf[get_schedule()->current_plan].temperature;
    }

    else
        u =  get_fuzzy_pid_params()->kp ;



    return u;
}
float Tsel_proc()
{
    float result;
    if (get_schedule()->current_plan < SCHEDULE_SIZE)
    {
        result = get_schedule()->buf[get_schedule()->current_plan].temperature;
    }
    else
    {
        result = get_config()->set_tindoor;

    }
    return result;
}
void pid_cal(unsigned char mode)
{
    float u;
    float setval;
    float measure_val;
    static float last_val;

    erro_ppre = erro_pre;
    erro_pre = erro;
    if (mode <= SCHE_MODE ) //智能控制 smart ctrl
    {
        if (indoor_temp_usr.temp_average >= (0.95 *
                                             get_config()->set_tindoor)) //平均温度达标
        {
            if (indoor_temp_usr.low_temp_percent >= 0.2) //末端温度不达�?
            {
                float tmp;
                tmp = 1 - indoor_temp_usr.low_temp_percent;
                tmp = tmp * indoor_temp_usr.temp_average;
                erro = tmp - get_schedule()->buf[get_schedule()->current_plan].temperature; ; //温度加权
            }
            else

                erro = indoor_temp_usr.temp_average - 
                get_schedule()->buf[get_schedule()->current_plan].temperature; ;
        }
        else
            erro = indoor_temp_usr.temp_average - 
            get_schedule()->buf[get_schedule()->current_plan].temperature;;
		if (mode == NATIVE_MODE) //smart mode plan start
		{
			measure_val =	get_ai_data()->temp[get_config()->tin_index];
			setval = get_schedule()->buf[get_schedule()->current_plan].temperature;

		}
			
		else// mode = 2
		{
				setval = get_schedule()->buf[get_schedule()->current_plan].temperature;;//get_config()->set_tindoor;
				measure_val =  indoor_temp_usr.temp_average;

		}


        //measure_val =   get_ai_data()->temp[get_config()->tin_index];

    }
//    else  //本地智能控制  native ctrl
//    {
//            erro = get_ai_data()->temp[AI_WATER_T_IN_INDEX] - MACHINE_IN_T ; //温度加权
//            setval = MACHINE_IN_T;
//            measure_val =  get_ai_data()->temp[AI_WATER_T_IN_INDEX];
//
//
//    }

    //get_fuzzy_pid_params()->e = erro;
    //get_fuzzy_pid_params()->de = erro_c;
    if(measure_val>=100||measure_val<=-40)
		measure_val = 0;
    if(setval>=100||setval<=-40)
		setval = 40;

    FuzzyPIDcontroller(setval, measure_val);
    indoor_temp_usr.output = get_fuzzy_pid_params()->kp;
    last_val = measure_val;

    erro_c = erro - erro_pre;

}
//PID *get_pid_params()
//{
//  return &sPID;
//}


void fuzzy_proc(unsigned char mode)
{

    pid_cal(mode);
}


