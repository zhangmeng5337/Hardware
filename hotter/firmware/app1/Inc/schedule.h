#ifndef SCHEDULE_H_
#define SCHEDULE_H_
#include "main.h"
//服务端--->设备(时间计划)
//{
//   "Dev ID":  "866289037465624",
//   "Dev Ctrl": {
//               "Plan1":[1,45,18,14,1,19,23,5],//开机，水温45度，周一到周五，开始时间18:14，结束时间23:01
//               "Plan10":[1,45,18,14,19,23,1],
//
//     }
//}


#define SCHEDULE_SIZE	10 
#define PLAN_SIZE	11 

typedef struct
{
  unsigned char pwr_state;//power on or off
  unsigned char temperature;//set pump temperature
  unsigned char shour; //start hour
  unsigned char sminute;
  unsigned char sweekday;
  unsigned char ehour;  //end hour
  unsigned char eminute;
  unsigned char eweekday;
  unsigned char	update;//correspond shedule time coming  1:update;0:no update
  unsigned char enable;// 1:enbale 0:disable
  unsigned char index;
} schedule_time;
typedef struct
{
  schedule_time buf[SCHEDULE_SIZE];
  unsigned char update_count;
  unsigned char save_flag;
  unsigned char current_plan;
  unsigned char last_paln;
  unsigned char current_plan_pwr_update;
  unsigned char mode;//0:ctrl indoor 1:ctrl pum water temp 2:smart ctrl
} schedule_pack_stru;
schedule_pack_stru *get_schedule(void);
void pwr_schedul_set(void);
void pwr_schedul_init(void);
void schedule_proc(void);

#endif

