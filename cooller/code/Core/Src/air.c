#include "air.h"
#include "pwm.h"
#include "pid.h"
#include "adc.h"
#include "modbus.h"
#include "config.h"

air_stru air_usr;

void air_init()
{
    HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
    pid_init(-25, -100);
}
void air_pwr_ctrl(unsigned char flag)
{
    if (flag == ON)
        HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_SET);

    else
        HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
}
/***************************************
stop:300-500hz
start:1000-10000hz
****************************************/
void air_freq_ctrl()
{
    float result;
    if (air_usr.pid_out >= PID_OUT_MAX)
    {
        if (air_usr.pid_out == PID_OUT_MAX)
            air_usr.freq = 10000;
        else
        {
            air_usr.freq = 500;
            air_pwr_ctrl(OFF);
        }
    }

    else
    {
        air_pwr_ctrl(ON);
        if (air_usr.pid_out > PID_OUT_MIN)
            air_usr.freq = FREQ_RATIO * air_usr.pid_out + FREQ_B;
        else
            air_usr.freq =   500;
        air_pwm_set(air_usr.freq);
    }




}
void air_temperature_ctrl()
{
    float tmep_T;
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= PID_POLL_T)
    {
        tick_start = HAL_GetTick();
        tmep_T = get_temperature()->average_T;
        air_usr.pid_out = pid_proc(tmep_T);
        air_freq_ctrl();
    }

}
void heater_cooller_ctrl()
{
    float tmep_T;
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= PID_POLL_T)
    {
        tick_start = HAL_GetTick();
        tmep_T = get_temperature()->heater_T;
        if (tmep_T <= getConfig()->min_T)
        {
            air_usr.heat_pid_out =  pid_proc_fan_heat(1, tmep_T);
            pwm_set(HEATER_1,  air_usr.heat_pid_out);

            air_usr.heat_pid_out =  pid_proc_fan_heat(2, tmep_T);
            pwm_set(HEATER_2,  air_usr.heat_pid_out);

            air_usr.heat_pid_out =  pid_proc_fan_heat(3, tmep_T);
            pwm_set(HEATER_3,  air_usr.heat_pid_out);

            air_usr.heat_pid_out =  pid_proc_fan_heat(4, tmep_T);
            pwm_set(HEATER_4,  air_usr.heat_pid_out);

        }
        else
        {
            pwm_set(HEATER_1,  0);
            pwm_set(HEATER_2,  0);
            pwm_set(HEATER_3,  0);
            pwm_set(HEATER_4,  0);

        }

    }

}

