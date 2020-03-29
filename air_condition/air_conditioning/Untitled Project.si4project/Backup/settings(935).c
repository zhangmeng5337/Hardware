#include "settings.h"
#include "adc.h"
#include "key.h"
#include "io.h"
#include "display.h"
#include "pid.h"
params_stru settings_params;
extern adc_io_str adc_io;
mode_stru mode;
PID xPID, yPID;




mode_stru *get_params_mode()
{
	mode_stru *tmp;
	tmp = &mode;
    
	return tmp;
}
void fan_ctrl(unsigned char umode)
{
		static uint32_t gettime;
		int32_t ca,cb;
		int16_t xError, yError;
		int16_t xPWM, yPWM;
		ca = settings_params.tar_env_temper[0]-settings_params.tar_set_temper[0];
		cb = settings_params.tar_set_temper[0]-settings_params.target_wind_temper[0];
		// PID calculation
		xError = ca;
		//yError = yPID.targetValue - yPos;
		xPWM = calcPID(&xPID, xError);
		//yPWM = calcPID(&yPID, yError);
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
void machine_control(unsigned char umode)
{


    static uint32_t gettime;	  
	  
	if(mode.last_mode_no == 0x00&&(umode||mode.last_mode_no))//2s启动风扇计时标志
  	{
		gettime = HAL_GetTick();
		mode.last_mode_no  = umode;
    }
	if(umode == COLD)
	{
	  if(mode.last_mode_no == COLD)
	  {
	    device_ctrl(FAN_COOL,ON);
		if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		{
			settings_params.present_status = 0x02;
			device_ctrl(FAN_HOT,ON);
		}	
	  }
	}
	else if(umode == HOT)
	{
		if(mode.last_mode_no == HOT)
		{
		  if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		  {
			  settings_params.present_status = 0x02;
		  }
		}

	}
	else if(umode == COLD_HUM_COLD)
	{
		if(mode.last_mode_no == COLD_HUM_COLD)
		{
		  if((HAL_GetTick() -gettime)>=FAN_RUN_TIME)
		  {
			  settings_params.present_status = 0x02;
		  }
		}

	}
	else if(umode == FAST_COLD)
	{
		if(mode.last_mode_no == FAST_COLD)
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


}

unsigned char tar_set_temper[6];//0:target cold temp;1:target hot temp;2:target cold and humid temp;3:fast colde temp; 
unsigned char tar_env_temper[4];//4:target cold and humid humid 5:fast humid

void init_params()
{
	settings_params.tar_set_temper[0] = INIT_SET_TEMPER;//0:target cold temp;
	settings_params.tar_set_temper[1]=INIT_SET_TEMPER;
	settings_params.tar_set_temper[2]=INIT_SET_TEMPER;
	settings_params.tar_set_temper[3]=INIT_SET_TEMPER;
	settings_params.humid[0]=INIT_SET_HUM;
	settings_params.humid[1]=INIT_SET_HUM;
}
void work_mode_process()
{
    uint32_t gettime;
	key_stru *key_tmp;
	key_tmp=key_process();

	if(key_tmp->key_status == 0x01)    //key1 setting or display mode setting
	{
		if(mode.status == WORK_OFF||mode.status == POWER_ON)//开机状态并且系统未运行时可以进行参数设置
		{
		  mode.mode = SETTING_MODE;    //setting mode
		  
		  if(mode.modeNo<(MAX_MODE_NO-1))
			{
				mode.modeNo = mode.modeNo + 1;
				mode.last_mode_no = mode.modeNo;
			}
		  else
		  {
			  mode.mode = NORMAL;	 //normal mode
			  mode.last_mode_no = mode.modeNo;
		  	  mode.modeNo = STAND_MODE;
		  }
		  
		} 
		else if(mode.status == WORK_ON)//开机状态并且系统运行时可以查询参数
		{
			if(mode.manual == 0&&mode.modeNo != RECYCLE_DISPLAY)
			  mode.manual = 1;
		}
	}
	else if(key_tmp->key_status == 0x02)
	{
		if(mode.status == WORK_ON)
		{
		  mode.mode = NORMAL;    //normal mode
		  if(mode.modeNo !=RECYCLE_DISPLAY)
		  	{
		  	  mode.last_mode_no = mode.modeNo;
			  mode.modeNo = RECYCLE_DISPLAY;
			  mode.manual = 0;
		  }
		  else
		  {
			  mode.modeNo =  mode.last_mode_no;
			  mode.manual = 0;

		  }
		}
		else if(mode.status == WORK_OFF||mode.status == POWER_ON)
		{
			mode.modeNo = mode.last_mode_no;
			//mode.last_mode_no = 0;
			mode.mode = NORMAL;
		 }	
		  
	}
	static unsigned char i,j;
	switch(mode.modeNo)
	{
		case COLD:          i=0; j=2;break;
		case HOT:           i=1; j=2;break;
		case COLD_HUM_COLD: i=2; j=2;break;
		case COLD_HUM_HUM:  i=5; j=0;break;		
		case FAST_COLD:     i=3; j=2;break;
		case FAST_COLD_HUM: i=5; j=1;break;		
		case RECYCLE_DISPLAY:break;
		//case COLD:i=0;break;
	}

	if(key_tmp->key_status == 0x03)  //kye 2 inc key
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
		{
             
		   	   if(i<5)
		   	   	{
				   if(settings_params.tar_set_temper[i]<=MAX_SET_TEMPER)
					settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] + 1;
				   else 
					settings_params.tar_set_temper[i] = 0;
			   }
			   else
			   	{
				   if(settings_params.humid[i]<=MAX_SET_HUM)
					settings_params.humid[j] = settings_params.humid[j] + 1;
				   else 
					settings_params.humid[j] = 0;

			   }

		}      
	}
	else if(key_tmp->key_status == 0x04)
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
		{
			while(1)
			{ 
				if((HAL_GetTick()-gettime)>=1000)
				{
					gettime = HAL_GetTick();
					if(i<5)
					{
						if(settings_params.tar_set_temper[i]<=MAX_SET_TEMPER)
							settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] + 1;
						else 
							settings_params.tar_set_temper[i] = 0;
					}
					else
					 {
						if(settings_params.humid[j]<=MAX_SET_HUM)
						 settings_params.humid[j] = settings_params.humid[j] + 1;
						else 
						 settings_params.humid[j] = 0;
					
					}

					if(get_key_level(2)==1)
						break;				
				}

			}
		}   

	}

	if(key_tmp->key_status == 0x05)//key3
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
		{

		   	  if(i<5)
		   	  {
				  if(settings_params.tar_set_temper[i]>MIN_SET_TEMPER)
				   settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] - 1;
				  else 
				   settings_params.tar_set_temper[i] = MIN_SET_TEMPER;

			  }

		}      
	}
	else if(key_tmp->key_status == 0x06)
	{
		if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
		{
			while(1)
			{ 
			    if((HAL_GetTick()-gettime)>=1000)
			    {
					gettime = HAL_GetTick();
					if(i<5)
					{
						if(settings_params.tar_set_temper[i]>MIN_SET_TEMPER)
						 settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] - 1;
						else 
						 settings_params.tar_set_temper[i] = MIN_SET_TEMPER;

					}

					if(get_key_level(3)==1)
						break;

				}

			}
		}   

	}
	if(key_tmp->key_status == 0x07)//key4 shot hit
	{
		if(mode.status == WORK_OFF)
		{
			mode.status = WORK_ON;  //system start
			mode.modeNo = mode.last_mode_no;
			//mode.last_mode_no = mode.modeNo; 			
		}
		else if(mode.status == WORK_ON)
	    {
			 mode.status = WORK_OFF;//system stop
			 mode.last_mode_no = mode.modeNo;
			 mode.modeNo = STAND_MODE;

	    }
		else if(mode.status == POWER_ON)
	    {
			 mode.status = WORK_ON;//system stop
			 mode.modeNo = mode.last_mode_no;
			// mode.last_mode_no = mode.modeNo;
			 
	    }
		mode.mode = NORMAL;	 //normal mode
		init_seg();
	}
	else if(key_tmp->key_status == 0x08)//key4 long hit,power off or power on
	{
		if(mode.status == WORK_OFF)
		{
		   mode.status = POWER_OFF;  //power off
		   mode.last_mode_no=mode.modeNo;
		   mode.modeNo = STOP_MODE;
		  
		}
		else if(mode.status == WORK_ON)
	    {
			 mode.status = POWER_OFF; //power off
			 mode.last_mode_no=mode.modeNo;
			 mode.modeNo = STOP_MODE;

	    }
		else if(mode.status == POWER_OFF)
	    {
			 mode.status = POWER_ON;
			 mode.modeNo=mode.last_mode_no;
			// mode.modeNo = STAND_MODE;

	    }
		else if(mode.status == POWER_ON)
	    {
			 mode.status = POWER_OFF;
			 mode.last_mode_no=mode.modeNo;
			 mode.modeNo = STOP_MODE;	
			 
	    }
		init_seg();

	}

	
	SetKeyNum();


}

void recycle_dis_deal(unsigned char mode_sel,unsigned char cycle_flag)
{
	unsigned char tmp;
   switch(cycle_flag)
   	{
		case 0:
			if(mode_sel ==COLD)
			{
			 tmp = SET_ST;
				display_dat_deal(settings_params.tar_set_temper[0],&tmp,3);
			}
			else if(mode_sel ==HOT)
			{
			 tmp = SET_ST;
				display_dat_deal(settings_params.tar_set_temper[1],&tmp,3);
			}
			else if(mode_sel ==COLD_HUM_COLD||mode_sel ==COLD_HUM_HUM)
			{
			 tmp = SET_ST;
				display_dat_deal(settings_params.tar_set_temper[2],&tmp,3);
			}
			else if(mode_sel ==FAST_COLD||mode_sel ==FAST_COLD_HUM)
			{
			 tmp = SET_ST;
				display_dat_deal(settings_params.tar_set_temper[3],&tmp,3);
			}break;
		case 1:
			if(mode_sel ==COLD)
			{
			 tmp = SET_AT;
				display_dat_deal(settings_params.tar_env_temper[0],&tmp,3);
			}
			else if(mode_sel ==HOT)
			{
			 tmp = SET_AT;
				display_dat_deal(settings_params.tar_env_temper[1],&tmp,3);
			}
			else if(mode_sel ==COLD_HUM_COLD||mode_sel ==COLD_HUM_HUM)
			{
			 tmp = SET_AT;
				display_dat_deal(settings_params.tar_env_temper[2],&tmp,3);
			}
			else if(mode_sel ==FAST_COLD||mode_sel ==FAST_COLD_HUM)
			{
			 tmp = SET_AT;
				display_dat_deal(settings_params.tar_env_temper[3],&tmp,3);
			}break;
		case 2:tmp = SET_AT;
				display_dat_deal(settings_params.machine_air_temper,&tmp,3);break;
		case 3:tmp = SET_AT;
				display_dat_deal(settings_params.speed,&tmp,1);break;
		case 4:tmp = SET_I;
			    display_dat_deal(settings_params.current,&tmp,3);break;

   }
}

void run_process()
{
	uint32_t gettime;
	 unsigned char tcode[9]={ SET_CM, SET_CHM,SET_HM,SET_FHM,SET_H,SET_ST,SET_WL,SET_I,SET_AT};
	static unsigned char toggle_flag=0;
 unsigned char tmp;
   work_mode_process();
   adc_process();
   get_io();
     	
	if( adc_io.fault_status ==0)//启动正常
	{
			machine_control(mode.mode);
	}
   
	if(get_params_mode()->mode ==NORMAL&&get_params_mode()->status >WORK_OFF)//正常模式
	{	
		/*if( adc_io.fault_status!=0)
		{
		    
		   display_dat_deal(adc_io.fault_status,1,2);	//显示故障码		
		}
		else*/                                  //显示正常数据
		{
			if(mode.modeNo == RECYCLE_DISPLAY||mode.manual)//自动循环显示正常数据
			{
			  if((HAL_GetTick()-gettime)>=RECYCLE_TIME)
			  { 
					if(mode.manual == 1)
						mode.manual = 0;
					gettime = HAL_GetTick();
					switch(mode.last_mode_no)
					{
						case STAND_MODE:				
							recycle_dis_deal(COLD,toggle_flag);
							 toggle_flag = toggle_flag +1;
							break; 

						case COLD:				
							recycle_dis_deal(COLD,toggle_flag);
							 toggle_flag = toggle_flag +1;
							break; 
						case HOT:				
							recycle_dis_deal(HOT,toggle_flag);
							 toggle_flag = toggle_flag +1;
							break; 						
						case COLD_HUM_COLD:
						case COLD_HUM_HUM: 	
							recycle_dis_deal(COLD_HUM_COLD,toggle_flag);
							 toggle_flag = toggle_flag +1;
							break; 

 
						case FAST_COLD:
						case FAST_COLD_HUM: 		
							recycle_dis_deal(FAST_COLD,toggle_flag);
							 toggle_flag = toggle_flag +1;
							break; 	

					}
					if(toggle_flag>=5)
						toggle_flag = 0;

			  }
			  
			}
			else
			{
				if((HAL_GetTick()-gettime)>=RECYCLE_TIME)
				{
				  
					gettime = HAL_GetTick();
					switch(mode.modeNo)
					{
						
 
						case STAND_MODE:
						case COLD:				tmp=SET_AT;display_dat_deal(settings_params.tar_env_temper[0],&tmp,3);break;
						case HOT:				  tmp=SET_AT;display_dat_deal(settings_params.tar_env_temper[1],&tmp,3);break;
						case COLD_HUM_COLD:
						case COLD_HUM_HUM: 	
							if(toggle_flag ==0)
							{
							  tmp = SET_AT;
								display_dat_deal(settings_params.tar_env_temper[2],&tmp,3);
							}
							else
							{
							    tmp = SET_AT;
								display_dat_deal(settings_params.humid[0],&tmp,3);
							}
							toggle_flag = ~toggle_flag;
							break; 
 
						case FAST_COLD:
						case FAST_COLD_HUM: 		
							if(toggle_flag ==0)
							{
								tmp = SET_AT;
								display_dat_deal(settings_params.tar_env_temper[3],&tmp,3);
							}
							else
							{
								tmp = SET_H;
								display_dat_deal(settings_params.humid[1],&tmp,3);
							}
							toggle_flag = ~toggle_flag;
							break; 	

					}
				
				}

			}
		}
	}
	else if(get_params_mode()->mode ==SETTING_MODE)//显示设置数据
	{
		if((HAL_GetTick()-gettime)>=DISPLAY_BLINK)
		{
			gettime = HAL_GetTick();
			switch(mode.modeNo)
			{
				case STAND_MODE:				display_dat_deal(settings_params.tar_set_temper[0],&tcode[0],3);break;
				case COLD:				display_dat_deal(settings_params.tar_set_temper[0],&tcode[0],3);break;
				case HOT:				display_dat_deal(settings_params.tar_set_temper[1],&tcode[2],3);break;
				case COLD_HUM_COLD: 	display_dat_deal(settings_params.tar_set_temper[2],&tcode[1],3);break;
				case COLD_HUM_HUM:				 display_dat_deal(settings_params.humid[0],&tcode[4],3);break; 	
				case FAST_COLD: 		display_dat_deal(settings_params.tar_set_temper[3],&tcode[3],3);break;
				case FAST_COLD_HUM: 			 display_dat_deal(settings_params.humid[1],&tcode[4],3);break;	
				case RECYCLE_DISPLAY:break;
				//case COLD:i=0;break;
			}
		
		}
	}
 if(mode.modeNo==STOP_MODE)
	{
		     tmp = SET_AT;
				display_dat_deal(settings_params.tar_env_temper[0],&tmp,4);
	}

}
