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

{0,"\"Updat Frimware\":",     "\r\n",NO_MATCH_ID,UCHAR_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.reboot},
{1,"\"Reboot Dev\":",         "\r\n",NO_MATCH_ID,UCHAR_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.reboot},
{2,"\"heatPumpCtrl\":",        "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.machine,&modbus_tx.ctrl_mode},//mode = 0,machine=255--all  mode = 1,machine = 1,2,4,8,16,32
{3,"\"outmax\":",             "\r\n",NO_MATCH_ID,INT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.out_max},
{4,"\"outmin\":",             "\r\n",NO_MATCH_ID,FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.out_min},
{5,"\"kp\":",                 "\r\n",NO_MATCH_ID,FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.kp},
{6,"\"ki\":",                "\r\n",NO_MATCH_ID, FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.ki},
{7,"\"kd\":",                "\r\n",NO_MATCH_ID, FLOAT_TYP,UPDATE_SET,&config_usr.update_setting,&myfuzzypid.kd},
{8,"\"ctrlMode\":",           "\r\n",NO_MATCH_ID,UCHAR_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.mode},//mode = 1,2,3,4---NATIVE_MODE  SMART_MODE  SCHE_MODE  OFF_MODE
{9,"\"dev_size\":",          "\r\n",NO_MATCH_ID,UCHAR_TYP,UPDATE_SET,&config_usr.update_setting,&config_usr.dev_size},
{10,"\"Set Out Temp\":",      "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_tout,&config_usr.set_tout_tmp},
{11,"\"Set Room Temp\":",     "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_tindoor},
{12,"timestamp",               ","   ,NO_MATCH_ID,UINT64_TYP ,UPDATE_SET},
{13,"\"Set Upload Period\":", "\r\n",NO_MATCH_ID,FLOAT_TYP ,UPDATE_SET,&config_usr.update_setting,&config_usr.set_up_period},
{14,"\"do_num\":"            , "\r\n",NO_MATCH_ID,UCHAR_TYP ,NOUPDATE_SET,&config_usr.update_setting,&do_usr.do_num},
{15,"\"do_set\":"           , "\r\n",NO_MATCH_ID,UCHAR_TYP ,NOUPDATE_SET,&config_usr.update_setting,&(do_usr.do_out),&(do_usr.do_update)},
{16,"\"Room Temp\":["        , "Room",NO_MATCH_ID,INT_ARRAY_TYP ,NOUPDATE_SET,&config_usr.update_setting,&config_usr.indoor_temperature},
{17,"\"Plan1\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[0]},
{18,"\"Plan2\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[1]},
{19,"\"Plan3\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[2]},
{20,"\"Plan4\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[3]},
{21,"\"Plan5\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[4]},
{22,"\"Plan6\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[5]},
{23,"\"Plan7\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[6]},
{24,"\"Plan8\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[7]},
{25,"\"Plan9\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[8]},
{26,"\"Plan10\":["            , "\r\n",NO_MATCH_ID,INT_ARRAY_TYP,NOUPDATE_SET,&schedule_u.buf[9]},


};
