#include "settings.h"
#include "adc.h"
#include "key.h"
#include "io.h"

params_stru settings_params;

params_stru *get_params_mode()
{
	params_stru *tmp;
	tmp = &settings_params;
    
	return tmp;
}
void machine_control(unsigned char mode)
{


    static uint32_t gettime;	  
	  uint32_t ca,cb;
	if(settings_params.present_status == 0x00&&(mode||settings_params.present_mode))//2s启动风扇计时标志
  	{
		gettime = HAL_GetTick();
		settings_params.present_status = 0x01;
    }
	if(mode == COLD||settings_params.present_mode ==COLD)
	{
      settings_params.present_mode = COLD;
      
	  if(settings_params.present_status == 0x01)
	  {
	    device_ctrl(FAN_COOL,ON);
		if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		{
			settings_params.present_status = 0x02;
			device_ctrl(FAN_HOT,ON);
		}
		ca = target_environment_temper-target_set_temper;
		cb = target_set_temper-target_wind_tempe;
		if(ca>MAX_ERROR_CA)
		{
			if(cb>=MAX_ERROR_CB)
			{
        
			}
			else if(cb<MAX_ERROR_CB&&cb>=MID_ERROR_CB)
			{

			}
			else if(cb<MID_ERROR_CB)
			{

			}

		}
		else if(ca<=MAX_ERROR_CA&&ca>MID_ERROR_CA)
		{
			if(cb>=MID_ERROR_CB)
			{
        
			}
			else if(cb<MID_ERROR_CB&&cb>=MIN_ERROR_CB)
			{

			}
			else if(cb<MIN_ERROR_CB)
			{

			}

		}
		else if(ca<=MID_ERROR_CA&&ca>=MIND_ERROR_CA)
		{

		}
		else if(ca<=MIND_ERROR_CA)
		{

		}	
		else if(ca>=MIDU_ERROR_CA)
		{

		}	

	  }
	}
	else if(mode == HOT||settings_params.present_mode ==HOT)
	{
		if(settings_params.present_status == 0x01)
		{
		  if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		  {
			  settings_params.present_status = 0x02;
		  }
		}

	}
	else if(mode == COLD_HUM||settings_params.present_mode ==COLD_HUM)
	{
		if(settings_params.present_status == 0x01)
		{
		  if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		  {
			  settings_params.present_status = 0x02;
		  }
		}

	}
	else if(mode == FAST_COLD||settings_params.present_mode ==FAST_COLD)
	{
		if(settings_params.present_status == 0x01)
		{
		  if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		  {
			  settings_params.present_status = 0x02;
		  }
		}

	}
	else
   {
		
   }
	if(mode)
		settings_params.present_mode = mode;

}
extern adc_io_str adc_io;

void run_process()
{
   uint16_t fault_tmp;
   key_stru *key_tmp;
   
   key_tmp=key_process();
   adc_process();
   get_io();
  
	if( adc_io.fault_status ==0)//启动正常
	{
		if(key_tmp->key_status )//设置参数
		{
			settings_params.present_mode = SETTING_MODE;
			machine_control(COLD);
			SetKeyNum();
		}
		else
		{
			machine_control(0);
		} 
	}
   
	if(get_params_mode()->present_mode==NORMAL)//正常模式
	{	
		if( adc_io.fault_status!=0)
		{
		    
		    display_dat_deal(fault_tmp,1,2);	//显示故障码		
		}
		else                                  //显示正常数据
		{

		}

	}
	else   //显示设置数据
	{

	}

}
