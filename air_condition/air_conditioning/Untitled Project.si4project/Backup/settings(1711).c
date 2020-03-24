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
void fan_ctrl(unsigned char mode)
{
	static uint32_t gettime;
	uint32_t ca,cb;
	ca = settings_params.target_environment_temper-settings_params.target_set_temper;
	cb = settings_params.target_set_temper-settings_params.target_wind_temper;
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
void machine_control(unsigned char mode)
{


    static uint32_t gettime;	  
	  
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
mode_stru mode;
void work_mode_process()
{
    uint32_t gettime;
	key_stru *key_tmp;
	key_tmp=key_process();
	if(key_tmp->key_status == 0x07)//key4 shot hit
	{
		if(mode.status == WORK_OFF)
		{
		  mode.status = WORK_ON;  //system start
		}
		else if(mode.status == WORK_ON)
	    {
			 mode.status = WORK_OFF;//system stop
	    }
	}
	else if(key_tmp->key_status == 0x08)//key4 long hit,power off or power on
	{
		if(mode.status == WORK_OFF)
		{
		  mode.status = POWER_ON;  //power on
		}
		else if(mode.status == WORK_ON)
	    {
			 mode.status = POWER_OFF; //power off
	    }
		else if(mode.status == POWER_OFF)
	    {
			 mode.status = POWER_ON;
	    }
		else if(mode.status == POWER_ON)
	    {
			 mode.status = POWER_OFF;
	    }

	}
	if(key_tmp->key_status == 0x01)    //key1 setting or display mode setting
	{
		if(mode.status == WORK_ON||mode.status == POWER_ON)
		{
		  mode.mode = SETTING_MODE;    //setting mode
		  if(mode.modeNo<(MAX_MODE_NO-1))
		   mode.modeNo = mode.modeNo + 1;
		  else
		  	mode.modeNo = 1;
		}      
	}
	else if(key_tmp->key_status == 0x02)
	{
		if(mode.status == WORK_ON||mode.status == POWER_ON)
		{
		  mode.mode = NORMAL;    //normal mode
		  if(mode.modeNo !=RECYCLE_DISPLAY)
		  	{
		  	  mode.last_mode_no = mode.modeNo;
			  mode.modeNo = RECYCLE_DISPLAY;

		  }
		   
		  else
		  {
			  mode.modeNo = mode.last_mode_no;

		  }	
		}  
	}
	if(key_tmp->key_status == 0x03)  //kye 2 inc key
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
		{

		   	   gettime = HAL_GetTick();
			   if(settings_params.target_set_temper<=MAX_SET_TEMPER)
				settings_params.target_set_temper = settings_params.target_set_temper + 1;
			   else 
				settings_params.target_set_temper = 0;
		}      
	}
	else if(key_tmp->key_status == 0x04)
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
		{
			while((HAL_GetTick()-gettime)>=1000)
			{
				if(settings_params.target_set_temper<=MAX_SET_TEMPER)
					settings_params.target_set_temper = settings_params.target_set_temper + 1;
				else 
					settings_params.target_set_temper = 0;
				if(get_key_level(2)==1)
					break;
			}
		}   

	}

	if(key_tmp->key_status == 0x05)
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
		{

		   	   gettime = HAL_GetTick();
			   if(settings_params.target_set_temper>MIN_SET_TEMPER)
				settings_params.target_set_temper = settings_params.target_set_temper - 1;
			   else 
				settings_params.target_set_temper = MIN_SET_TEMPER;
		}      
	}
	else if(key_tmp->key_status == 0x06)
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
		{
			while((HAL_GetTick()-gettime)>=1000)
			{
				if(settings_params.target_set_temper>MIN_SET_TEMPER)
					else 
					 settings_params.target_set_temper = MIN_SET_TEMPER;
				if(get_key_level(3)==1)
					break;
			}
		}   

	}
	SetKeyNum();


}
void run_process()
{
   uint16_t fault_tmp;
   work_mode_process();

   adc_process();
   get_io();
     	
	if( adc_io.fault_status ==0)//启动正常
	{
		if(mode.mode == COLD)//
		{
			settings_params.present_mode = SETTING_MODE;
			machine_control(COLD);
			
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
