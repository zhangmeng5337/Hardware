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
    indoor_temp_usr.temp_error = 	buf[2];
}
float get_pid_output()
{
    float  u;
        if (get_schedule()->mode == 1)
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
	if(get_schedule()->current_plan<SCHEDULE_SIZE)
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
    if (mode == 0&&get_schedule()->mode == 0)//智能控制 smart ctrl
    {
        if (indoor_temp_usr.temp_average >= (0.95 * get_config()->set_tindoor)) //平均温度达标
        {
            if (indoor_temp_usr.low_temp_percent >= 0.2) //末端温度不达??
            {
                float tmp;
                tmp = 1 - indoor_temp_usr.low_temp_percent;
                tmp = tmp * indoor_temp_usr.temp_average;
                erro = tmp-get_config()->set_tindoor ;//温度加权
            }
            else

                erro = indoor_temp_usr.temp_average-get_config()->set_tindoor ;
        }
        else
            erro = indoor_temp_usr.temp_average-get_config()->set_tindoor;
		 setval = get_config()->set_tindoor;
		measure_val =  indoor_temp_usr.temp_average;

    }
    else  //本地控制  native ctrl
    {
       if (get_schedule()->mode == 1)
       	{
       	;
//		   erro=get_schedule()->buf[get_schedule()->current_plan].temperature-get_config()->set_tindoor;
//		setval = get_config()->set_tindoor;
//	   measure_val =  indoor_temp_usr.temp_average;

	   }
	   else
	   	{
	   erro = get_ai_data()->temp[AI_WATER_T_IN_INDEX]-MACHINE_IN_T ;//温度加权
		setval = MACHINE_IN_T;
	   measure_val =  get_ai_data()->temp[AI_WATER_T_IN_INDEX]; 	

	   }

    }
	
	//get_fuzzy_pid_params()->e = erro;
	//get_fuzzy_pid_params()->de = erro_c;
	
	 FuzzyPIDcontroller(setval, measure_val);
	indoor_temp_usr.output = get_fuzzy_pid_params()->kp;
	last_val = measure_val;

    erro_c = erro - erro_pre;

}
//PID *get_pid_params()
//{
//  return &sPID;
//}

//PID *get_pid_params()
//{
//  return &sPID;
//}


void fuzzy_proc(unsigned char mode)
{

    pid_cal(mode);
}


