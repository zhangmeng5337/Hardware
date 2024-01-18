#include "settings.h"
#include "adc.h"
#include "key.h"
#include "io.h"
#include "display.h"
#include "pid.h"
#include "EEPROM.h"
#include "machine.h"

params_stru settings_params;//���ò���
extern adc_io_str adc_io;//adcģ�����ɼ�
mode_stru mode;//����ģʽ
PID xPID, yPID;

void write_dipay_ram(unsigned char mode);

/*********************************************************************************
���������� ��ʼ��pid����
**********************************************************************************/
void init_Pid()
{
   xPID.Kp = 30;
   xPID.Ki = 10;
   xPID.Kd = 1;
   yPID.Kp = 30;
   yPID.Ki = 10;
   yPID.Kd = 1;

}
mode_stru *get_params_mode()
{
    mode_stru *tmp;
    tmp = &mode;

    return tmp;
}

/*********************************************************************************
���������� ѹ�������ƣ�����pid�㷨���������㣬����ѹ������������
������
    umode��ON������OFF�ػ�
    
**********************************************************************************/
void airMachine_ctrl(unsigned char umode)
{

    settings_params.tar_env_temper =adc_io.adc_result[1];
    settings_params.equip_env_temper = adc_io.adc_result[2];
    settings_params.machine_air_temper = adc_io.adc_result[4];
    settings_params.target_wind_temper =adc_io.adc_result[5];
	   settings_params.humid[0] = adc_io.adc_result[0];
 	  settings_params.humid[1] = adc_io.adc_result[0];	


    float ca,cb;
    int16_t xError, yError;
    int16_t xPWM, yPWM;
	if(umode == ON)
	{
		 if(mode.modeNo == COLD)//����ģʽ������
		 {
			ca = settings_params.tar_env_temper-settings_params.tar_set_temper[0];
			cb = settings_params.target_wind_temper-settings_params.tar_set_temper[0];		 
		 }
		 else if(mode.modeNo == HOT)
		 {
			ca = settings_params.tar_set_temper[1]-settings_params.tar_env_temper;
			cb = settings_params.tar_set_temper[1]-settings_params.target_wind_temper;		 
		 }
		 else if(mode.modeNo == COLD_HUM_COLD)
		 {
			ca = settings_params.tar_env_temper-settings_params.tar_set_temper[2];
			cb = settings_params.target_wind_temper-settings_params.tar_set_temper[2];
		 }			 
		 else if(mode.modeNo == FAST_COLD)
		 {
			ca = settings_params.tar_env_temper-settings_params.tar_set_temper[3];
			cb = settings_params.target_wind_temper-settings_params.tar_set_temper[3];		 
		 }
		// PID calculation
		xError = ca;
		
		yError = cb;
		xPWM = calcPID(&xPID, xError);//pid����������
		yPWM = calcPID(&yPID, yError);
		
		if(ca>=5)
		{
			if(cb<=1)
			{
				TransmitCommand(xPWM,umode);//����ѹ������������
			}
			else if(cb>=2)
			{
				if((xPWM+yPWM)>=MAX_SPEED)
					TransmitCommand(MAX_SPEED,umode);
				else
					TransmitCommand(xPWM+yPWM,umode);
			}
			else
			{
				TransmitCommand(xPWM,umode);
			}
		}
		else if(ca<5&&ca>=0)
		{
			if(cb<=0)
			{
				TransmitCommand(xPWM,umode);
			}
			else if(cb>=1)
			{
				if((xPWM+yPWM)>=MAX_SPEED)
					TransmitCommand(MAX_SPEED,umode);
				else
					TransmitCommand(xPWM+yPWM,umode);
			}
			else
			{
				TransmitCommand(yPWM,umode);
			}
		
		
		}
		else if(ca<0&&ca>=-0.5)
		{
			TransmitCommand(MIN_SPEED,umode);
		
		}
		else if(ca<-0.5)
		{
			TransmitCommand(MIN_SPEED,OFF);
			device_ctrl(DEV_ALL,OFF);
		
		}
		else if(ca>=0.5)
		{
			TransmitCommand(MIN_SPEED,ON);
			mode.flag_mark = 0;
		
		}
		if(settings_params.machine_air_temper>=MAX_MACHINE_AIRT)
		{
			TransmitCommand(MIN_SPEED,OFF);
		}

	}
	else
	{
		TransmitCommand(MIN_SPEED,OFF);

	}



}
/*********************************************************************************
���������� ���ڷ��Ϳ�������
������
    umode����
    
**********************************************************************************/

unsigned char command_tx_time_loop(unsigned char umode)
{
    static uint32_t gettime;


    if(mode.flag_mark == 0x00)//2s�������ȼ�ʱ��־
    {
        mode.flag_mark = 1;
        gettime = HAL_GetTick();
        mode.last_mode_no  = umode;
		    return 0;
    }

    if((HAL_GetTick() -gettime)>=FAN_RUN_TIME&&mode.flag_mark==1)
    {
        mode.flag_mark=2;
        return 1;
    }
    if((HAL_GetTick() -gettime)>=TX_COMMAND_TIME&&mode.flag_mark==2)
    {
        gettime = HAL_GetTick();
        return 2;
    }

}
/*********************************************************************************
���������� ѹ���������߼�����ѹ���������ȵ��豸���е���
������
    umode��
          coldΪ����ģʽ��
          hotΪ����ģʽ��
          COLD_HUM_COLDΪ�����ʪģʽ��
          FAST_COLD��������ģʽ
    
**********************************************************************************/

void machine_mode_control(unsigned char umode)
{

    if(mode.flag_mark == 0x00)
        command_tx_time_loop(umode);

    if(mode.flag_mark)
    {
        if(umode == COLD)
        {
            if(mode.last_mode_no == COLD)
            {

                device_ctrl(FAN_COOL,ON);//�����������
				unsigned char tmp;
			    tmp = command_tx_time_loop(umode);//��ⷢ��ʱ���Ƿ񵽴�
                if(tmp==1||tmp==2)//�����Է��Ϳ�������
                {
					tmp = 0;
                   // mode.flag_mark==2;
                    settings_params.present_status = 0x02;
                    airMachine_ctrl(ON);//����ѹ����
                    device_ctrl(FAN_HOT,ON);//�������ȷ���
					
                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }
        }
        else if(umode == HOT)
        {
            if(mode.last_mode_no == HOT)
            {
                device_ctrl(FAN_HOT,ON);
							  unsigned char tmp;
							  tmp = command_tx_time_loop(umode);
                if(tmp==1||tmp==2)
                {
									tmp = 0;
                    settings_params.present_status = 0x02;
                    device_ctrl(FAN_COOL,ON);
                    airMachine_ctrl(ON);
                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }

        }
        else if(umode == COLD_HUM_COLD)
        {
            if(mode.last_mode_no == COLD_HUM_COLD)
            {
                device_ctrl(PUMP,ON);
							  unsigned char tmp;
							  tmp = command_tx_time_loop(umode);
                if(tmp==1||tmp==2)
                {
									tmp = 0;
                    settings_params.present_status = 0x02;
                    device_ctrl(FAN_COOL,ON);
                    device_ctrl(HUMID,ON);	
                    airMachine_ctrl(ON);
					           device_ctrl(PUMP,ON);

                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }

        }
        else if(umode == FAST_COLD)
        {
            if(mode.last_mode_no == FAST_COLD)
            {
                device_ctrl(PUMP,ON);
							  unsigned char tmp;
							  tmp = command_tx_time_loop(umode);
                if(tmp==1||tmp==2)
                {
									tmp = 0;
									settings_params.present_status = 0x02;
									device_ctrl(FAN_COOL,ON);
									airMachine_ctrl(ON);
									device_ctrl(HUMID,ON);	
									device_ctrl(PUMP,ON);

                }
//                if(command_tx_time_loop(umode)==2)
//                {
//                    settings_params.present_status = 0x02;
//                }

            }

        }
        else
        {

        }

    }



}

unsigned char tar_set_temper[6];//0:target cold temp;1:target hot temp;2:target cold and humid temp;3:fast colde temp;
unsigned char tar_env_temper[4];//4:target cold and humid humid 5:fast humid
/*********************************************************************************
���������� ���� ����
������
    
**********************************************************************************/

void write_params()
{
   flash_init();
	{   
	   flash_write(0,0x5a,1); 
	   flash_write(1,mode.modeNo,1); 
	   flash_write(2,mode.last_mode_no,1); 
	   flash_write(3,settings_params.tar_set_temper[0],1); 
	   flash_write(4,settings_params.tar_set_temper[1],1); 
	   flash_write(5,settings_params.tar_set_temper[2],1); 
	   flash_write(6,settings_params.tar_set_temper[3],1); 
	}

}
/*********************************************************************************
���������� ��ʼ���豸����

**********************************************************************************/

void init_params()
{
  uint32_t p[7];
    flash_read(0,p,7); 
	if(p[0]==0x5a)//���������
	{
		mode.modeNo =  p[1];
		mode.last_mode_no =  p[2];
		settings_params.tar_set_temper[0] = p[3];//0:target cold temp;
		settings_params.tar_set_temper[1]=p[4];
		settings_params.tar_set_temper[2]=p[5];
		settings_params.tar_set_temper[3]=p[6];
	}
	else//����δ�������
	{
		mode.modeNo =  COLD;
		mode.last_mode_no =  COLD;
		settings_params.tar_set_temper[0] = 25;//0:target cold temp;
		settings_params.tar_set_temper[1]=18;
		settings_params.tar_set_temper[2]=25;
		settings_params.tar_set_temper[3]=25;
		settings_params.humid[0]=INIT_SET_HUM;
		settings_params.humid[1]=INIT_SET_HUM;
		write_params();

	}

mode.status=POWER_ON;

}
void work_mode_process()
{
    uint32_t gettime;
    key_stru *key_tmp;
    key_tmp=key_process();

    if(key_tmp->key_status == 0x01)    //key1 setting or display mode setting
    {
		if(mode.status >= WORK_OFF)//����״̬����ϵͳδ����ʱ���Խ��в�������

       // if(mode.status == WORK_OFF||mode.status == POWER_ON)//����״̬����ϵͳδ����ʱ���Խ��в�������
        {
            mode.mode = SETTING_MODE;    //setting mode

            if(mode.modeNo<(MAX_MODE_NO-1))
            {
                mode.modeNo = mode.modeNo + 1;
                mode.last_mode_no = mode.modeNo;
            }
            else
            {
							  mode.modeNo = COLD;
                //mode.mode = NORMAL;	 //normal mode
                mode.last_mode_no = mode.modeNo;
                
            }

        }
        else if(mode.status == WORK_ON)//����״̬����ϵͳ����ʱ���Բ�ѯ����
        {
            if(mode.modeNo<(MAX_MODE_NO-1))
            {
                mode.modeNo = mode.modeNo + 1;
                mode.last_mode_no = mode.modeNo;
            }
            else
            {
                mode.mode = NORMAL;	 //normal mode
                mode.last_mode_no = mode.modeNo;
                mode.modeNo = COLD;
            }
        }
    }
    else if(key_tmp->key_status == 0x02)
    {
//        if(mode.status == WORK_ON)
//        {
//            mode.mode = NORMAL;    //normal mode
//            if(mode.modeNo !=RECYCLE_DISPLAY)
//            {
//                mode.last_mode_no = mode.modeNo;
//                mode.modeNo = RECYCLE_DISPLAY;
//                mode.manual = 0;
//            }
//            else
//            {
//                mode.modeNo =  mode.last_mode_no;
//                mode.manual = 0;

//            }
//        }
//        else if(mode.status == WORK_OFF||mode.status == POWER_ON)
//        {
//            mode.modeNo = mode.last_mode_no;
//            //mode.last_mode_no = 0;
//            mode.mode = NORMAL;
//        }

    }
    static unsigned char i,j;
    switch(mode.modeNo)
    {
    case COLD:
        i=0;
        j=2;
        break;
    case HOT:
        i=1;
        j=2;
        break;
    case COLD_HUM_COLD:
        i=2;
        j=2;
        break;
    /*case COLD_HUM_HUM:
        i=5;
        j=0;
        break;*/
    case FAST_COLD:
        i=3;
        j=2;
        break;
   /* case FAST_COLD_HUM:
        i=5;
        j=1;
        break;*/
    case RECYCLE_DISPLAY:
        break;
        //case COLD:i=0;break;
    }

    if(key_tmp->key_status == 0x03)  //kye 2 inc key
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
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
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
        {
            while(1)
            {
                if((HAL_GetTick()-gettime)>=300)
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
                    //write_dipay_ram(2);

                    if(get_key_level(2)==1)
                        break;
                }

            }
        }

    }

    if(key_tmp->key_status == 0x05)//key3
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
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
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD)
        {
            while(1)
            {
                if((HAL_GetTick()-gettime)>=300)
                {
                    gettime = HAL_GetTick();
                    if(i<5)
                    {
                        if(settings_params.tar_set_temper[i]>MIN_SET_TEMPER)
                            settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] - 1;
                        else
                            settings_params.tar_set_temper[i] = MIN_SET_TEMPER;

                    }
                    //write_dipay_ram(2);

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
           
            if(mode.last_mode_no)
                mode.modeNo = mode.last_mode_no;
            else
                mode.modeNo = COLD;
						write_params();
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
            if(mode.last_mode_no)
                mode.modeNo = mode.last_mode_no;
            else
                mode.modeNo = STAND_MODE;
          	//	mode.modeNo =  STOP_MODE;
		          //mode.last_mode_no =  COLD;
		
        }
        mode.mode = NORMAL;	 //normal mode
        //init_seg();
    }
    else if(key_tmp->key_status == 0x08)//key4 long hit,power off or power on
    {
        if(mode.status == WORK_OFF)
        {
            mode.status = POWER_OFF;  //power off
            mode.mode = NORMAL;
            if(mode.last_mode_no)
                mode.last_mode_no=mode.last_mode_no;
            else
                mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        else if(mode.status == WORK_ON)
        {
            mode.mode = NORMAL;
            mode.status = POWER_OFF; //power off
            mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        else if(mode.status == POWER_OFF)
        {
            mode.status = POWER_ON;
//			 if(mode.last_mode_no == STOP_MODE)
//			 	mode.modeNo=STAND_MODE;
//			 else
//			 mode.modeNo=mode.last_mode_no;
            mode.modeNo = STAND_MODE;

        }
        else if(mode.status == POWER_ON)
        {
            mode.mode = NORMAL;
            mode.status = POWER_OFF;
            mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        //init_seg();

    }


    SetKeyNum();


}


void run_process()
{

    /**************************************�豸����*************************************************/
    work_mode_process();//��������
    adc_process();//ģ�����ɼ�
    get_io();
    settings_params.tar_env_temper =adc_io.adc_result[1];
    settings_params.equip_env_temper = adc_io.adc_result[2];
    settings_params.machine_air_temper = adc_io.adc_result[3];
    settings_params.target_wind_temper =adc_io.adc_result[4];
    settings_params.water_temper =adc_io.adc_result[5];	
	settings_params.humid[0] = adc_io.adc_result[0];
 	settings_params.humid[1] = adc_io.adc_result[0];	
    if( mode.status == WORK_ON)//��������
    {
			machine_mode_control(mode.modeNo);
			if(HAL_GPIO_ReadPin(water_adc_GPIO_Port,water_adc_Pin)==0)
				device_ctrl(GATE,ON);
			else
				device_ctrl(GATE,OFF);				
    }
	else
	{
		static uint32_t getTickTime;
	   mode.flag_mark = 0;
		 device_ctrl(DEV_ALL,OFF);	
		if((HAL_GetTick() -getTickTime)>=TX_COMMAND_TIME)
		{
			airMachine_ctrl(OFF);
			getTickTime = HAL_GetTick();
				
		}


	}
/*****************************************��ʾ����******************************************************/
    if(mode.mode ==NORMAL&&mode.status >=WORK_ON)//����ģʽ
    {
        /*if( adc_io.fault_status!=0)
        {

           display_dat_deal(adc_io.fault_status,1,2);	//��ʾ������
        }
        else*/                                  //��ʾ��������
        {
            display_menu(1);
        }
    }
    else if(get_params_mode()->mode ==SETTING_MODE)//��ʾ��������
    {
        display_menu(2);

    }
    else if(get_params_mode()->status ==POWER_ON)//��ʾ��������
    {
        display_menu(2);

    }
    else if(get_params_mode()->status ==WORK_OFF)//��ʾ��������
    {
        display_menu(1);

    }
    if(mode.modeNo==STOP_MODE&&get_params_mode()->status ==POWER_OFF)
    {
        display_menu(0);

    }

}
