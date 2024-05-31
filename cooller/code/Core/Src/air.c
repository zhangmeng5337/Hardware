#include "air.h"
#include "pwm.h"
#include "pid.h"
#include "adc.h"
#include "modbus.h"
#include "config.h"
#include "adc.h"
#include "usb_ctrl.h"
#include "rf_drv.h"
#include "flash.h"
#include "lcd.h"

air_stru air_usr;

void air_init()
{
    HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
    pid_init(31 , -40);
	
}
void air_pwr_ctrl(unsigned char flag)
{
    if (flag == ON)
        HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_SET);

    else
        HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
}
void app_init(void)
{

    adc_init();
    config_init();

    air_init();
    usb_init();
    uart_init();
    lcd_init();

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
        tmep_T = get_rf_status()->average_T;
        air_usr.pid_out = pid_proc(tmep_T);//�����¶ȿ���
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
        if (get_rf_status()->vaild_flag == 1)
            tmep_T = get_rf_status()->average_T;
        else
            tmep_T = getConfig()->max_T + 1;
        float tmp_v;
        tmp_v = get_temperature()->T_value[1] * BATTERY_V;
        air_usr.ratio = 12 / tmp_v;
        if (tmep_T <= getConfig()->min_T)//heater ctrl
        {
            getConfig()->mode = 1;//����
            air_usr.heat_pid_out =  pid_proc_fan_heat(1, tmep_T);//����������¶ȿ���
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
            getConfig()->mode = 0;//����
            pwm_set(HEATER_1,  0);
            pwm_set(HEATER_2,  0);
            pwm_set(HEATER_3,  0);
            pwm_set(HEATER_4,  0);

            pwm_set(COOLER_3,  150 * air_usr.ratio); //��ȴ�����¶���
            pwm_set(COOLER_4,  150 * air_usr.ratio); //��ȴ�����¶���
            air_temperature_ctrl();
            air_freq_ctrl();
        }
        if (get_temperature()->T_value[0] >= FAN_RUN_T)//charger temperature ctrl
        {

            pwm_set(COOLER_1,  1000 * air_usr.ratio);
            pwm_set(COOLER_2,  1000 * air_usr.ratio);
        }
        else
        {
            pwm_set(COOLER_1,  0);
            pwm_set(COOLER_2,  0);

        }


    }

}
void dat_record_proc(void)
{
    uint32_t  data_tmp;
    float tmp;
    unsigned char pb[4];

    if (getConfig()->update_T == 1)
    {
        if (get_flash_status()->vailabe_len > 0)
        {

            if (get_flash_status()->used_len == 0)
                Erase_page(Application_2_Addr, 1);
            floatTouint32(get_rf_status()->average_T, pb); //�¶�
            WriteFlash(getConfig()->addr, pb, 4);
            getConfig()->addr = getConfig()->addr + 4;
            get_flash_status()->used_len =  get_flash_status()->used_len + 4;

        }
        else
        {
            Erase_page(Application_2_Addr, 1);

            get_flash_status()->used_len = 0;
            floatTouint32(get_rf_status()->average_T, pb); //�����¶�
            WriteFlash(getConfig()->addr, pb, 4);
            getConfig()->addr = getConfig()->addr + 4;
            get_flash_status()->used_len =  get_flash_status()->used_len + 4;
        }
        getConfig()->update_T = 0;
		flash_save();
    }

	if(getConfig()->update_params==1)
	{
		flash_save();
		getConfig()->update_params = 0;
	}

}
void PVD_config(void)
{
	//����PWR
	PWR_PVDTypeDef sConfigPVD;                    
	sConfigPVD.PVDLevel = PWR_PVDLEVEL_7;            //����2.9V���������ж�
	sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;        //�����PVDO����һ�����ѡ�������ش���
	HAL_PWR_ConfigPVD(&sConfigPVD);                  //HAL������PVD����
 
	//ʹ��PVD
	HAL_PWR_EnablePVD();        //���������ж�
}


void app()
{
    lcd_proc();
    adc_proc();
    heater_cooller_ctrl();
    usb_ctrl();
    rf_ctrl_proc();
    dat_record_proc();
}
