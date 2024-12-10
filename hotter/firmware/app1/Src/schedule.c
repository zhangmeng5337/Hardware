#include "schedule.h"
#include "stmflash.h"
#include "rtc.h"
#include "config.h"
#include "sys.h"


schedule_pack_stru schedule_u;

void pwr_schedul_reset(void)
{
    ;//restore paln

}
unsigned char  boot_params_proc(unsigned char oper)
{
    unsigned char buf[512], result;
    uint32_t  i, j;

    if (oper == WRITE)
    {
        __disable_irq();
        memset(buf, 0, 512);
        ReadFlash(SYS_PARAMS_Addr, buf, 512);
        // ReadFlash(FACTORY_ADDR, buf, 2048);//read original data
        Erase_page(SYS_PARAMS_Addr, 1);
        //  memset(buf, 0, 512);
        buf[0] = 0x5a;

        j = 1;
        for (i = 0; i < SCHEDULE_SIZE; i++)
        {
            buf[j++] = get_schedule()->buf[i].pwr_state;
            buf[j++] = get_schedule()->buf[i].temperature ;
            buf[j++] = get_schedule()->buf[i].shour;
            buf[j++] = get_schedule()->buf[i].sminute;
            buf[j++] = get_schedule()->buf[i].sweekday;
            buf[j++] = get_schedule()->buf[i].ehour;
            buf[j++] = get_schedule()->buf[i].eminute;
            buf[j++] = get_schedule()->buf[i].eweekday;
            buf[j++] = get_schedule()->buf[i].enable;
            buf[j++] = get_schedule()->buf[i].index ;
        }


        WriteFlashBytes(SYS_PARAMS_Addr, buf, 512);
        __enable_irq();
        result = 0;

    }
    else
    {
        memset(buf, 0, 512);
        ReadFlash(SYS_PARAMS_Addr, buf, 512);
        i = sizeof(schedule_u.buf);
        if (buf[0]  == 0x5a)
        {
            j = 1;
            for (i = 0; i < SCHEDULE_SIZE; i++)
            {

                get_schedule()->buf[i].pwr_state = buf[j ++];
                get_schedule()->buf[i].temperature = buf[j ++];
                get_schedule()->buf[i].shour = buf[j ++];
                get_schedule()->buf[i].sminute = buf[j ++];
                get_schedule()->buf[i].sweekday = buf[j ++];
                get_schedule()->buf[i].ehour = buf[j ++];
                get_schedule()->buf[i].eminute = buf[j ++];
                get_schedule()->buf[i].eweekday = buf[j ++];
                get_schedule()->buf[i].enable = buf[j ++];
                get_schedule()->buf[i].index = buf[j ++];

            }
            result = 0;


        }
        else
        {
            result = 1;
            pwr_schedul_reset();
        }
    }
    return result;
}
void pwr_schedul_set(void)
{
    static uint32_t stick;
    if (schedule_u.save_flag == 1)
    {
        if ((HAL_GetTick() - stick) >= 3000)
        {
            boot_params_proc(WRITE);
            schedule_u.save_flag = 0;

        }
    }
    else
    {
        stick = HAL_GetTick();
    }
}


void pwr_schedul_init(void)
{
    if (boot_params_proc(READ) == 1)
    {
        pwr_schedul_reset();
    }
    schedule_u.last_paln = SCHEDULE_SIZE + 2;
    schedule_u.mode = 1;
    schedule_u.current_plan_pwr_update = 1;

}
schedule_pack_stru *get_schedule(void)
{
    return &schedule_u;
}

//void plan_query(void)
//{
//    unsigned char i,day,hour,minute,sec;
////6:31-16:30 16:31--6:30
//    for(i=0; i<SCHEDULE_SIZE; i++)
//    {
//        if((getRtcTime()->Hours==schedule_u.buf[i].shour&&getRtcTime()->Minutes>=schedule_u.buf[i].sminute)  ||
//                (getRtcTime()->Hours==schedule_u.buf[i].ehour&&getRtcTime()->Minutes<=schedule_u.buf[i].eminute)
//          )
//        {
//
//            if(getRtcDate()->WeekDay>=schedule_u.buf[i].sweekday  &&
//                    getRtcDate()->WeekDay<=schedule_u.buf[i].eweekday )
//            {
//                if(schedule_u.buf[i].enable == 1)
//                {
//                    schedule_u.current_plan= i;
//                    schedule_u.mode = 1;
//                    //schedule_u.last_paln = i;
//
//                }
//
//                break;
//            }
//        }
//        else//6:31-16:30 16:31--6:30
//        {
////            if((getRtcTime()->Hours>schedule_u.buf[i].shour&&
////                    getRtcTime()->Hours<schedule_u.buf[i].ehour)
//            if(getRtcTime()->Hours<schedule_u.buf[i].ehour)
//            {
//                if(getRtcDate()->WeekDay>=schedule_u.buf[i].sweekday  &&
//                        getRtcDate()->WeekDay<=schedule_u.buf[i].eweekday )
//                {
//                    if(schedule_u.buf[i].enable == 1)
//                    {
//                        schedule_u.current_plan= i;
//                        schedule_u.mode = 1;
//                                  get_config()->set_tout = schedule_u.buf[i].temperature;
//                    }
//                                      else
//                                      {
//                                          i=SCHEDULE_SIZE;
//                                      }
//
//                    break;
//                }
//
//            }
//
//
//
//        }
//
//    }
//    if(i>=SCHEDULE_SIZE)
//    {
//        schedule_u.last_paln = SCHEDULE_SIZE+2;
//        schedule_u.current_plan= SCHEDULE_SIZE;
//        schedule_u.current_plan_pwr_update= 0;
//        schedule_u.mode = 0;
//       get_config()->set_tout = 45;
//    }
//    else
//    {
//        if(schedule_u.last_paln != schedule_u.current_plan)
//        {
//            schedule_u.last_paln = schedule_u.current_plan;
//            schedule_u.current_plan_pwr_update= 1;
//        }
//    }
//
//}
void plan_query(void)
{
    unsigned char i, day, hour, minute, sec;
//6:31-16:30 16:31--6:30
    for (i = 0; i < SCHEDULE_SIZE; i++)
    {
        if ((getRtcTime()->Hours == schedule_u.buf[i].shour
                && getRtcTime()->Minutes >= schedule_u.buf[i].sminute)  ||
                (getRtcTime()->Hours == schedule_u.buf[i].ehour
                 && getRtcTime()->Minutes <= schedule_u.buf[i].eminute)
           )
        {
            if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                    getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
            {
                if (schedule_u.buf[i].enable == 1)
                {
                    schedule_u.current_plan = i;
                    if (get_config()->mode <= 2)
                    {
                        schedule_u.mode = 1;
                        if (get_config()->mode == 1)
                            get_config()->set_tout = schedule_u.buf[i].temperature;

                    }


                }
                else
                {
                    i = SCHEDULE_SIZE;
                }

                break;
            }

        }
        else
        {
            if (getRtcTime()->Hours < 12)
            {
                if (schedule_u.buf[i].shour < 12 || schedule_u.buf[i].ehour < 12)
                {
                    if (schedule_u.buf[i].shour < 12) //
                    {
                        if (getRtcTime()->Hours > schedule_u.buf[i].shour)
                        {
                            if (schedule_u.buf[i].ehour < 12)

                            {
                                if (getRtcTime()->Hours < schedule_u.buf[i].ehour)
                                {
                                    if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                            getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                                    {
                                        if (schedule_u.buf[i].enable == 1)
                                        {

                                            if (get_config()->mode <= 2)
                                            {
                                                schedule_u.current_plan = i;
                                                schedule_u.mode = 1;
                                                if (get_config()->mode == 1)
                                                    get_config()->set_tout = schedule_u.buf[i].temperature;

                                            }

                                        }
                                        else
                                        {
                                            i = SCHEDULE_SIZE;
                                        }

                                        break;
                                    }

                                }


                            }//end ehour < 12
                            else //ehour>=12
                            {
                                if (getRtcTime()->Hours < schedule_u.buf[i].ehour)
                                {
                                    if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                            getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                                    {
                                        if (schedule_u.buf[i].enable == 1)
                                        {
                                            if (get_config()->mode <= 2)
                                            {
                                                schedule_u.current_plan = i;
                                                schedule_u.mode = 1;
                                                if (get_config()->mode == 1)
                                                    get_config()->set_tout = schedule_u.buf[i].temperature;

                                            }

                                        }
                                        else
                                        {
                                            i = SCHEDULE_SIZE;
                                        }

                                        break;
                                    }

                                }

                            }//end ehour>=12

                        }
                    }
                    else if (schedule_u.buf[i].ehour < 12) //end shour<12
                    {
                        if (getRtcTime()->Hours < schedule_u.buf[i].ehour)
                        {
                            if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                    getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                            {
                                if (schedule_u.buf[i].enable == 1)
                                {

                                    if (get_config()->mode <= 2)
                                    {
                                        schedule_u.current_plan = i;
                                        schedule_u.mode = 1;
                                        if (get_config()->mode == 1)
                                            get_config()->set_tout = schedule_u.buf[i].temperature;

                                    }

                                }
                                else
                                {
                                    i = SCHEDULE_SIZE;
                                }

                                break;
                            }

                        }

                    }
                }

            }
            else //Hours>=12
            {
                if (schedule_u.buf[i].shour >= 12 || schedule_u.buf[i].ehour >= 12)
                {
                    if (schedule_u.buf[i].shour >= 12) //
                    {
                        if (getRtcTime()->Hours > schedule_u.buf[i].shour)
                        {
                            if (schedule_u.buf[i].ehour < 12)

                            {
                                //   if(getRtcTime()->Hours<schedule_u.buf[i].ehour)
                                {
                                    if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                            getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                                    {
                                        if (schedule_u.buf[i].enable == 1)
                                        {

                                            if (get_config()->mode <= 2)
                                            {
                                                schedule_u.current_plan = i;
                                                schedule_u.mode = 1;
                                                if (get_config()->mode == 1)
                                                    get_config()->set_tout = schedule_u.buf[i].temperature;

                                            }

                                        }
                                        else
                                        {
                                            i = SCHEDULE_SIZE;
                                        }

                                        break;
                                    }

                                }


                            }//end ehour < 12
                            else //ehour>=12
                            {
                                if (getRtcTime()->Hours < schedule_u.buf[i].ehour)
                                {
                                    if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                            getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                                    {
                                        if (schedule_u.buf[i].enable == 1)
                                        {

                                            if (get_config()->mode <= 2)
                                            {
                                                schedule_u.current_plan = i;
                                                schedule_u.mode = 1;
                                                if (get_config()->mode == 1)
                                                    get_config()->set_tout = schedule_u.buf[i].temperature;

                                            }

                                        }
                                        else
                                        {
                                            i = SCHEDULE_SIZE;
                                        }

                                        break;
                                    }

                                }

                            }//end ehour>=12

                        }
                    }
                    else if (schedule_u.buf[i].ehour >= 12) //end shour<12
                    {
                        if (getRtcTime()->Hours < schedule_u.buf[i].ehour)
                        {
                            if (getRtcDate()->WeekDay >= schedule_u.buf[i].sweekday  &&
                                    getRtcDate()->WeekDay <= schedule_u.buf[i].eweekday)
                            {
                                if (schedule_u.buf[i].enable == 1)
                                {

                                    if (get_config()->mode <= 2)
                                    {
                                        schedule_u.current_plan = i;
                                        schedule_u.mode = 1;
                                        if (get_config()->mode == 1)
                                            get_config()->set_tout = schedule_u.buf[i].temperature;

                                    }

                                }
                                else
                                {
                                    i = SCHEDULE_SIZE;
                                }

                                break;
                            }

                        }

                    }
                }

            }

        }


    }
    if (i >= SCHEDULE_SIZE)
    {
        schedule_u.last_paln = SCHEDULE_SIZE + 2;
        schedule_u.current_plan = SCHEDULE_SIZE;
        schedule_u.current_plan_pwr_update = 0;
        schedule_u.mode = 0;
        get_config()->set_tout = 45;
    }
    else
    {
        if (schedule_u.last_paln != schedule_u.current_plan)
        {
            schedule_u.last_paln = schedule_u.current_plan;
            schedule_u.current_plan_pwr_update = 1;
            reset_registerTick(SCHEDU_TICK_NO);
            registerTick(SCHEDU_TICK_NO, SCHEDU_POLL_TIME);

        }
        else
        {
            if (GetTickResult(SCHEDU_TICK_NO) == 1)
            {
                schedule_u.last_paln = schedule_u.current_plan;
                schedule_u.current_plan_pwr_update = 1;
                reset_registerTick(SCHEDU_TICK_NO);
                registerTick(SCHEDU_TICK_NO, SCHEDU_POLL_TIME);

            }

        }

    }
    if (get_config()->mode == 2)
        get_config()->set_tout = get_config()->set_tindoor;

    if (get_config()->mode == 3)
        get_config()->set_tout = 40;

}

void schedule_proc(void)
{
    urtc_proc();
    plan_query();
}







