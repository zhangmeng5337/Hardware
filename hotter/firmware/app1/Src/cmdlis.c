#include "config.h"
#include "mqtt_analy.h"
#include "modbus.h"
#include "FuzzyPID.h"
#include "do.h"
#include "schedule.h"
//    unsigned int cmd_info;
//	unsigned char Lfind_string[STRING_SIZE];
//	unsigned char Rfind_string[STRING_SIZE];	
//	unsigned char find_flag; //1:match id 0:not need match
//	unsigned char ret_dat_type;//0:unsigned char 1:unsigned int 2:float 3:int 4:string
//	unsigned char set_flag;//1:call update
//	void *memptr1; //update	
//	void *memptr2; 
//	void *memptr3; 


cmd_stru mqtt_cmd_list[]=
{
{0,"\"Updat Frimware\":",     "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.reboot},
{1,"\"Reboot Dev\":",         "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.reboot},
{2,"\"heatPumpAll\":",        "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},
{3,"\"outmax\":",             "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.out_max},
{4,"\"outmin\":",             "\r\n",NO_MATCH_ID,FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.out_min},
{5,"\"kp\":",                 "\r\n",NO_MATCH_ID,FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.kp},
{6,"\"ki\":",                "\r\n",NO_MATCH_ID, FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.ki},
{7,"\"kd\":",                "\r\n",NO_MATCH_ID, FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.kd},
{8,"\"scheMode\":",           "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.mode},
{9,"\"offlineMode\":",        "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.mode},
{10,"\"nativeMode\":",        "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.mode},
{11,"\"smartMode\":",         "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.mode},
{12,"\"dev_size\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.dev_size},
{13,"\"Set Out Temp\":",      "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_tout,&config_usr.set_tout_tmp},
{14,"\"Set Room Temp\":",     "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_tindoor},
{15,"timestamp",               ","   ,NO_MATCH_ID,UINT64_TYP ,UPDATE_SET},
{16,"\"Set Upload Period\":", "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_up_period},
{17,"\"do_num\":"           , "\r\n",NO_MATCH_ID,UINT_TYP ,NOUPDATE_SET,&config_usr.update_setting,&do_usr.do_num},
{18,"\"do_set\":"           , "\r\n",NO_MATCH_ID,UINT_TYP ,NOUPDATE_SET,&config_usr.update_setting,&(do_usr.do_out),&(do_usr.do_update)},
{19,"\"Room Temp\":["        , "Room",NO_MATCH_ID,FLOAT_TYP ,NOUPDATE_SET,&config_usr.update_setting},
{20,"\"Plan1\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[0]},
{21,"\"Plan2\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[1]},
{22,"\"Plan3\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[2]},
{23,"\"Plan4\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[3]},
{24,"\"Plan5\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[4]},
{25,"\"Plan6\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[5]},
{26,"\"Plan7\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[6]},
{27,"\"Plan8\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[7]},
{28,"\"Plan9\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[8]},
{29,"\"Plan10\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[9]},

{40,"\heatPump1\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},
{41,"\heatPump2\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},
{42,"\heatPump2\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},
{43,"\heatPump2\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},
{44,"\heatPump2\":",          "\r\n",NO_MATCH_ID,UINT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode}

};
