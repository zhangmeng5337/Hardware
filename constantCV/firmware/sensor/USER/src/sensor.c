#include "sensor.h"
#include "cs1237.h"
#include "main.h"
#include "kalman.h"

struct cs1237_device g_cs1237_device_st = {0};
adc_stru adc_usr;

void adc_init(void)
{
	cs1237_init(&g_cs1237_device_st, DEV_FREQUENCY_640, DEV_PGA_1, DEV_CH_A);

}
void adc_proc(void)
{
	adc_usr.adc_ori = g_cs1237_device_st.get_adc_data(&g_cs1237_device_st);
	adc_usr.adc_filter = kalman_filter(kalman_kfp_get(), (float)g_cs1237_device_st.adc_calculate_deal_data);
     printf(".3f%   .3f%\r\n",adc_usr.adc_ori,adc_usr.adc_filter);
}

