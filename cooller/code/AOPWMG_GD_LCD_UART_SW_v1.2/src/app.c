#include "app.h"
#include "app_lcd.h"
#include "sensor.h"
#include "devCtrl.h"
#include "sys.h"
#include "bsp_beep.h"
#include "inout.h"
#include "app_esp8266.h"
#include "app_lcd.h"
#include "bsp_cpu_flash.h"
#include "pid.h"
#include<stdlib.h>
#include "cmd_process.h"
#include <string.h>
#include "bsp_beep.h"

void SystemParamsSave(void);
void SystemParamsRead(void);


<<<<<<< HEAD
//ç›®æ ‡tds 2.5----500
//ç›®æ ‡tds 3 ------650
//ç›®æ ‡tds 3.5 ------950   +30----0.1
//ç›®æ ‡tds 4 ------1100
unsigned char First_flag;

uint32_t dstTds=1100;//ç›®æ ‡tds 2.5----500
=======
//Ä¿±êtds 2.5----500
//Ä¿±êtds 3 ------650
//Ä¿±êtds 3.5 ------950   +30----0.1
//Ä¿±êtds 4 ------1100
unsigned char First_flag;

uint32_t dstTds=1100;//Ä¿±êtds 2.5----500
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
//unsigned char module_reset_flag=0;
work_params_stru work_params;
static unsigned char shunt_flag=0;

void app_init()
{
    // SystemParamsRead();
    //adcInit();
    RS485_Init();
    // inout_Init();
    // work_params.work_mode = ON;

}
/*********************************************************************
<<<<<<< HEAD
function:åŠŸèƒ½å¼‚å¸¸è¶…æ—¶åˆ¤æ–­
=======
function:¹¦ÄÜÒì³£³¬Ê±ÅÐ¶Ï
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
params:
**********************************************************************/
unsigned char TickTimeoutAb(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
<<<<<<< HEAD
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//ä»Žæ­£å¸¸è½¬ä¸ºå¼‚å¸¸
    {
        registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
        registerTick(TickNum, timeout, 1,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//´ÓÕý³£×ªÎªÒì³£
    {
        registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
        registerTick(TickNum, timeout, 1,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==2)
        {
<<<<<<< HEAD
            registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
            registerTick(TickNum, timeout, 1,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
            registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
            registerTick(TickNum, timeout, 1,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==1)
    {
        // status = status | BitOper;
<<<<<<< HEAD
        registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
=======
        registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        return BitOper;
    }
    else
    {

        return 0;
    }

}

unsigned char TickTimeoutNor(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
<<<<<<< HEAD
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//æ­£å¸¸
    {
        registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
        registerTick(TickNum, timeout, 2,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//Õý³£
    {
        registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
        registerTick(TickNum, timeout, 2,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        if(GetTickStatus(TickNum)==0)
            return BitOper;
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==1)
        {
<<<<<<< HEAD
            registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
            registerTick(TickNum, timeout, 2,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
            registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
            registerTick(TickNum, timeout, 2,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==2)
    {
<<<<<<< HEAD
        registerTick(TickNum, 0, 0,1);//å®šæ—¶å™¨å¤ä½
=======
        registerTick(TickNum, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        return BitOper;
    }

    return 0xff;

}
//static unsigned char tick_conv_tmp=0;
unsigned char abnormalDec()
{
    static unsigned char status;

<<<<<<< HEAD
    if(work_params.init_flag == 0)//ä¸Šç”µåˆå§‹åŒ–ï¼Œç”µè§£æ°´ä»ŽåºŸæ°´é€šé“æŽ’å‡º
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//æ—¶é—´åˆ°ï¼Œèµ°æ­£å¸¸é€»è¾‘å·¥ä½œ
=======
    if(work_params.init_flag == 0)//ÉÏµç³õÊ¼»¯£¬µç½âË®´Ó·ÏË®Í¨µÀÅÅ³ö
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//Ê±¼äµ½£¬×ßÕý³£Âß¼­¹¤×÷
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        registerTick(StART_TICK_NO, 0,0,1);
    }


    //********************************************************************************

<<<<<<< HEAD
    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//æµé‡å¼‚å¸¸
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//æ°´é¾™å¤´å¼€çš„æƒ…å†µä¸‹æµé‡å¼‚å¸¸
=======
    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//Á÷Á¿Òì³£
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//Ë®ÁúÍ·¿ªµÄÇé¿öÏÂÁ÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
            status = status | TickTimeoutAb(FLOW_TICK_NO,0x01,30000);
        }

        else
            status = status & 0xfe;
    }
    else
    {
        status = status & TickTimeoutNor(FLOW_TICK_NO,0xfe,1000);
    }

//********************************************************************************
<<<<<<< HEAD
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//åŽŸæ°´tdså¼‚å¸¸
=======
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//Ô­Ë®tdsÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {

        status = status | TickTimeoutAb(TDS1_TICK_NO,0x40,30000);

    }
    else
    {
        status = status & TickTimeoutNor(TDS1_TICK_NO,0xbf,2000);
    }

    if(dstTds<400)
        dstTds = 400;

    if(GetSensor()->tds2 <= MIN_TDS_VALUE||(GetSensor()->tds2 <=(dstTds-200)&&
<<<<<<< HEAD
                                            GetSensor()->status[TDS2_INDEX]==0))//tdså¼‚å¸¸
=======
                                            GetSensor()->status[TDS2_INDEX]==0))//tdsÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {


        status = status | TickTimeoutAb(TDS_TICK_NO,0x02,2*MAX_TICK);//
        if(status &0x02)
        {
<<<<<<< HEAD
            registerTick(TDS_TICK_NO, 0, 0,1);//å®šæ—¶å™¨å¤ä½
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//è¶…æ—¶è®¡æ—¶
=======
            registerTick(TDS_TICK_NO, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//³¬Ê±¼ÆÊ±
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }
    }
    else
    {


        GetSensor()->err_flag = 0 ;
        status = status & TickTimeoutNor(TDS_TICK_NO,0xfd,MAX_SHORT_TICK);
    }
    //********************************************************************************

<<<<<<< HEAD
    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//phå¼‚å¸¸
=======
    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//phÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        status = status | TickTimeoutAb(PH_TICK_NO,0x04,MAX_TICK);
    }
    else
    {
        /* registerTick(PH_TICK_NO, 0, 1);
         status = status & 0xfb;*/
        status = status & TickTimeoutNor(PH_TICK_NO,0xfb,MAX_SHORT_TICK);
    }
    //********************************************************************************

<<<<<<< HEAD
    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orpå¼‚å¸¸
=======
    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orpÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        status = status | TickTimeoutAb(ORP_TICK_NO,0x08,MAX_TICK);
    }
    else
    {
        status = status & TickTimeoutNor(ORP_TICK_NO,0xf7,MAX_SHORT_TICK);
    }
    //********************************************************************************

    if(GetSensor()->water_level == WATER_F	||GetSensor()->water_level == WATER_L
<<<<<<< HEAD
            ||GetSensor()->water_level == WATER_M)//æ°´ä½å¼‚å¸¸
=======
            ||GetSensor()->water_level == WATER_M)//Ë®Î»Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        if(GetSensor()->water_level != WATER_H)
        {

            // status = status | 0x20;//
            status = status |TickTimeoutAb(WATER_TICK_NO,0x20,10000);

        }
        //else
        {
            // status = status |TickTimeoutAb(WATER_TICK_NO,0x20,WATER_L_DELAY_TICK);//

        }

        GetSensor()->salt_water_timeout++;

    }
    else
    {
        GetSensor()->salt_water_timeout=0;
        status = status & 0xdf;
    }
    //********************************************************************************
#if LW_ENABLE == 0
<<<<<<< HEAD
    if(GetSensor()->swH ==ON)//é«˜åŽ‹å¼€å…³å¼‚å¸¸
=======
    if(GetSensor()->swH ==ON)//¸ßÑ¹¿ª¹ØÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        status = status | 0x10;
        //status = status | TickTimeoutAb(SW_TICK_NO,0x10,400);
    }
    else
    {
        //status = status & TickTimeoutNor(SW_TICK_NO,0xef,200);
        status = status & 0xef;
    }
#else
<<<<<<< HEAD
    if(GetSensor()->swH ==ON&&GetSensor()->flow<=0)//é«˜åŽ‹å¼€å…³å¼‚å¸¸
=======
    if(GetSensor()->swH ==ON&&GetSensor()->flow<=0)//¸ßÑ¹¿ª¹ØÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        status = status | 0x10;
        //status = status | TickTimeoutAb(SW_TICK_NO,0x10,400);
    }
    else
    {
        //status = status & TickTimeoutNor(SW_TICK_NO,0xef,200);
        status = status & 0xef;
    }

#endif


    if(GetSensor()->salt_water_timeout>= SALT_TIMEOUT)//falut prevent  from water sensor fault
    {
        status = status & 0xdf;
    }

    return status;
}


/***********************************************************************
<<<<<<< HEAD
åŠŸèƒ½ï¼šæ ¹æ®æµé‡èŒƒå›´è°ƒæ•´è¿›æ°´tds
=======
¹¦ÄÜ£º¸ù¾ÝÁ÷Á¿·¶Î§µ÷Õû½øË®tds
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

************************************************************************/
static float flow_switch;// 0-1  1-2  2-3  3-4

void Flow_Init()
{
    // dstTds = 1100;
    static unsigned char start_flag=0;
    if(GetSensor()->flow<=0)
    {
        start_flag = 0;
        registerTick(TDS_SETTING_TICK_NO, 0,0,1);
        switch(FLOW_SIZE)
        {
        case 1:
            flow_switch = TDS_LEVEL1 ;
            break;
        case 2:
            flow_switch = TDS_LEVEL3;
            break;
        case 3:
            flow_switch = TDS_LEVEL3 ;
            break;
        case 4:
            flow_switch = TDS_LEVEL4;
            break;
        default:
            flow_switch = TDS_LEVEL4;
            break;
        }
    }
    else
    {

        registerTick(TDS_SETTING_TICK_NO, 1000,1,0);
<<<<<<< HEAD
        //ç›®æ ‡tds 2.5----500
        //ç›®æ ‡tds 3 ------650
        //ç›®æ ‡tds 3.5 ------950	+30----0.1
        //ç›®æ ‡tds 4 ------1100
=======
        //Ä¿±êtds 2.5----500
        //Ä¿±êtds 3 ------650
        //Ä¿±êtds 3.5 ------950	+30----0.1
        //Ä¿±êtds 4 ------1100
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        //2.5-----500
        //2.5-3--- 500
        //3-3.5----650
        //3.5-4---	950

        if(GetTickResult(TDS_SETTING_TICK_NO)==1)
        {

            switch(FLOW_SIZE)
            {
            case 4:
                if(GetSensor()->flow<=2.5)//450
                    flow_switch = TDS_LEVEL1 ;
                else if(GetSensor()->flow<=3)//2.5---3	480--530
                {
                    flow_switch = (GetSensor()->flow-2.5)*100+TDS_LEVEL1;
                    if(flow_switch<TDS_LEVEL1)
                        flow_switch = TDS_LEVEL1;
                }
                else if(GetSensor()->flow<=3.5)//3---3.5 510-660
                {
                    flow_switch = (GetSensor()->flow-3)*300+TDS_LEVEL2;
                    if(flow_switch<TDS_LEVEL2)
                        flow_switch = TDS_LEVEL2;
                }

                else //(GetSensor()->flow<=4)//3.5---4	  670-970	+30----0.1
                {
                    flow_switch = (GetSensor()->flow-3.5)*600+TDS_LEVEL3;
                    if(flow_switch>=TDS_LEVEL4)//1120
                        flow_switch = TDS_LEVEL4;
                }


                break;
            case 2:
                if(GetSensor()->flow<=1)//400
                    flow_switch = 300 ;
                else if(GetSensor()->flow<=2)//1---2	400---530
                {
                    flow_switch = (GetSensor()->flow-1)*130+TDS_LEVEL1;

                }
                else if(GetSensor()->flow<=3)//2---3 530-700
                {
                    flow_switch = (GetSensor()->flow-2)*200+TDS_LEVEL2;

                }

                else//3---700-800
                {
                    flow_switch = (GetSensor()->flow-3)*100+TDS_LEVEL3;
                    if(flow_switch>=900)//1120
                        flow_switch = 900;
                }
                break;

            }

            //else
            // flow_switch = TDS_LEVEL5 ;//1000
            registerTick(TDS_SETTING_TICK_NO, 0,0,1);

        }
        else
        {
            if(start_flag == 0)
            {
                start_flag = 1;
                switch(FLOW_SIZE)
                {
                case 1:
                    flow_switch = TDS_LEVEL1 ;
                    break;
                case 2:
                    flow_switch = TDS_LEVEL2 ;
                    break;
                case 3:
                    flow_switch = TDS_LEVEL3 ;
                    break;
                case 4:
                    flow_switch = TDS_LEVEL4 ;
                    break;
                default:
                    flow_switch = TDS_LEVEL4 ;
                    break;
                }
            }
        }

    }


    dstTds = flow_switch;
    //  dstTds = 2000;
    pid_init_flow(dstTds);

}
static int32_t pwm2;
unsigned char pid_cnt;
extern unsigned char tds_flag;

void FlowCtrl()
{
    static double tds_out=1200;
    static uint32_t pid_cnt;
    //dstTds = 1100;
    tds_out    = GetSensor()->tds2;

    if(PID_TYPE == 2)
    {

<<<<<<< HEAD
        registerTick(PID_SETTING_TICK_NO, 10, 1,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
        registerTick(PID_SETTING_TICK_NO, 10, 1,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        //  if(GetTickResult(PID_SETTING_TICK_NO)==0)
        //     return ;
        /* if(tds_out<=650)
         tds_out = tds_out + 5;
         if(tds_out>=680)
         	tds_out = tds_out -5;*/
        pwm2 = pid_proc(tds_out);
<<<<<<< HEAD
        registerTick(PID_SETTING_TICK_NO, 0, 0,1);//å®šæ—¶å™¨å¤ä½
=======
        registerTick(PID_SETTING_TICK_NO, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }
    if(PID_TYPE != 2)
    {
<<<<<<< HEAD
        registerTick(PID_SETTING_TICK_NO, 1000, 1,0);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
        registerTick(PID_SETTING_TICK_NO, 1000, 1,0);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        if(GetTickResult==0)
            return ;

        {
            pid_cnt++;
            pid_cnt = 0;
            pwm2 = pid_proc(tds_out);
            tds_flag = 0;
<<<<<<< HEAD
            registerTick(PID_SETTING_TICK_NO, 0, 0,1);//å®šæ—¶å™¨å¤ä½
=======
            registerTick(PID_SETTING_TICK_NO, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }


    }


    //pwm2 = 65535;
    if(pwm2>0)
    {
        ; //DcMotorCtrl(3,PUMP_VALUE);
        // DcMotorCtrl(1,PUMP_VALUE);
    }
    if(First_flag ==1)
<<<<<<< HEAD
        DcMotorCtrl(2,30000);//æ³µ2è°ƒæ•´æµé‡

    else
        DcMotorCtrl(2,pwm2);//æ³µ2è°ƒæ•´æµé‡
=======
        DcMotorCtrl(2,30000);//±Ã2µ÷ÕûÁ÷Á¿

    else
        DcMotorCtrl(2,pwm2);//±Ã2µ÷ÕûÁ÷Á¿
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
}
void fill_pro(unsigned char x,unsigned char y,unsigned char len,unsigned char maxlen)
{
    unsigned char strlen;
    if(strlen<maxlen)
        GUI_RectangleFill(x+len, y, maxlen-len,16	,0xf7be);

}


void module_dis_proc(unsigned char module_num)
{
    unsigned char *str;
    char ptr[20];
    unsigned char str_len;
    unsigned char last_len;

    if(module_num>FLOW_SIZE)
    {
<<<<<<< HEAD
        str = "æ— ";
=======
        str = "ÎÞ";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

#if LCD_ORI == 1
        SetLableValue(Main_PAGE,module_num+54,(unsigned char *)str);


#else
        SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, str);

#endif
    }

    else
    {
        if(GetSensor()->ele_MOnLine[module_num-1] == 1)
        {
            sprintf(ptr,"%4.2f",GetSensor()->ele_Mcurr[module_num-1]);
            str_len = strlen(ptr);
            if(last_len!=str_len)
            {
                last_len=str_len;
                // GUI_RectangleFill(x+2*str_len, y, 64,16  ,BACK_COLOR);
            }
#if LCD_ORI == 1
            SetLableValue(Main_PAGE,module_num+54,(unsigned char *)ptr);


#else
            SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, ptr);

#endif


            //   Display_String(x, y,1,0x10,0,back_color,force_color, (unsigned char *)ptr);//c1


        }
        else
        {
<<<<<<< HEAD
            str = "ç¦»çº¿";
=======
            str = "ÀëÏß";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#if LCD_ORI == 1
            SetLableValue(Main_PAGE,module_num+54,(unsigned char *)str);


#else
            SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, str);

#endif


            // Display_String(x, y,1,0x10,0,back_color,force_color, (unsigned char *)str);//c1

        }

    }



}

void Mode_dis_proc()
{
    unsigned char *str,j;
//    char ptr[20];
//    static unsigned char str_len;
//    static unsigned char last_len;
    static unsigned char status_tmp,i;


    static unsigned char polling_count;
polling_status:

    polling_count++;
    if(i<WORK_STATUS_SIZE)
        status_tmp = GetSensor()->status[i++];
    else
        i = 0;
    if(GetSensor()->status[0] == 0)
    {
        if(status_tmp==0)
        {

            if(i<WORK_STATUS_SIZE)
            {
                //i=0;
                goto polling_status;
            }
            else
                ;


        }
    }
    else
        i=0;



    //while(str==NULL)
    str =malloc(26);
    //str2 = str;
    for(j=0; j<26; j++)
        str[j] = '\0';
    polling_count = 0;
<<<<<<< HEAD
    switch(status_tmp)// 0:æ­£å¸¸  1ï¼štds1 2:tds2 3ï¼šæµé‡ 4ï¼šorp
        //  5:é«˜åŽ‹å¼€å…³6ï¼šæ°´ä½å¼€å…³ 7:ç”µè§£ä¸­
    {
    /*	str = "æ­£å¸¸          ";
    	str = "åŽŸæ°´TDSå¼‚å¸¸	 ";
    	str = "ç¼ºç›		 	 ";
    	str = "æµé‡å¼‚å¸¸ 	 ";
    	str = "ç›æ°´ç®±æ³¨æ°´ä¸­";
    	str = "æ¸…æ´—ä¸­		 ";
    	str = "ç¼ºæ°´			 ";
    	str = "ç³»ç»Ÿæ•…éšœ 	 ";
    	str = "æ­£å¸¸			 ";
    	str = "æ­£å¸¸			 ";*/
=======
    switch(status_tmp)// 0:Õý³£  1£ºtds1 2:tds2 3£ºÁ÷Á¿ 4£ºorp
        //  5:¸ßÑ¹¿ª¹Ø6£ºË®Î»¿ª¹Ø 7:µç½âÖÐ
    {
    /*	str = "Õý³£          ";
    	str = "Ô­Ë®TDSÒì³£	 ";
    	str = "È±ÑÎ		 	 ";
    	str = "Á÷Á¿Òì³£ 	 ";
    	str = "ÑÎË®Ïä×¢Ë®ÖÐ";
    	str = "ÇåÏ´ÖÐ		 ";
    	str = "È±Ë®			 ";
    	str = "ÏµÍ³¹ÊÕÏ 	 ";
    	str = "Õý³£			 ";
    	str = "Õý³£			 ";*/
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643





    case 20:
<<<<<<< HEAD
        str = "æ­£å¸¸";

        break;
    case TDS1_INDEX:
        str = "åŽŸæ°´TDSå¼‚å¸¸";
=======
        str = "Õý³£";

        break;
    case TDS1_INDEX:
        str = "Ô­Ë®TDSÒì³£";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;
    case TDS2_INDEX:
    case SHUNT_INDEX:
        // if(GetSensor()->tds2<dstTds)
<<<<<<< HEAD
        str = "ç¼ºç›";

        break;
    case FLOW_INDEX:
        str = "æµé‡å¼‚å¸¸";
=======
        str = "È±ÑÎ";

        break;
    case FLOW_INDEX:
        str = "Á÷Á¿Òì³£";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;

    case WATER_LEVEL_INDEX:
<<<<<<< HEAD
        str = "ç›æ°´ç®±æ³¨æ°´ä¸­";
        break;

    case WASH_INDEX:
        str = "æ¸…æ´—ä¸­";
=======
        str = "ÑÎË®Ïä×¢Ë®ÖÐ";
        break;

    case WASH_INDEX:
        str = "ÇåÏ´ÖÐ";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;

    case NOWATER_INDEX:
<<<<<<< HEAD
        str = "ç¼ºæ°´";
        break;
    case SYSTEM_INDEX:
        str = "ç³»ç»Ÿæ•…éšœ";
        break;
    case FACTORY_INDEX:
        str = "æ­£å¸¸";
=======
        str = "È±Ë®";
        break;
    case SYSTEM_INDEX:
        str = "ÏµÍ³¹ÊÕÏ";
        break;
    case FACTORY_INDEX:
        str = "Õý³£";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;
    default:

<<<<<<< HEAD
        str = "æ­£å¸¸";
        break;
    }
    //str = "è¿›æ°´TDSå¼‚å¸¸";
=======
        str = "Õý³£";
        break;
    }
    //str = "½øË®TDSÒì³£";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    if(status_tmp!=0)
#if LCD_ORI == 1
        SetLableValue(Main_PAGE,LABEL_WORK_ID,str);
#else
        //  if(status_tmp!=SYSTEM_INDEX&&status_tmp!=FLOW_INDEX&&status_tmp!=TDS2_INDEX&&status_tmp!=TDS1_INDEX&&status_tmp!=SHUNT_INDEX)
        //     SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
        //  else
        //      SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,WARN_BACK_COLOR, str);



        if(status_tmp==20||status_tmp==WASH_INDEX||status_tmp==WATER_LEVEL_INDEX||
                status_tmp==PH_INDEX||status_tmp==ORP_INDEX)
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
        else if(status_tmp==SYSTEM_INDEX||status_tmp==FLOW_INDEX||
                status_tmp==TDS2_INDEX||status_tmp==TDS1_INDEX||status_tmp==SHUNT_INDEX||
                status_tmp==NOWATER_INDEX)
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,WARN_BACK_COLOR, str);

#endif
    //
    //free(str2);

}
lcd_test()
{
    unsigned char *str,j;
    str =malloc(20);
    for(j=0; j<20; j++)
        str[j] = '\0';

<<<<<<< HEAD
    str = "æ­£å¸¸            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "åŽŸæ°´TDSå¼‚å¸¸     ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ç¼ºç›            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "æµé‡å¼‚å¸¸        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ç›æ°´ç®±æ³¨æ°´ä¸­";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "æ¸…æ´—ä¸­          ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ç¼ºæ°´            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ç³»ç»Ÿæ•…éšœ        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "æ­£å¸¸            ";
=======
    str = "Õý³£            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "Ô­Ë®TDSÒì³£     ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "È±ÑÎ            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "Á÷Á¿Òì³£        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ÑÎË®Ïä×¢Ë®ÖÐ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ÇåÏ´ÖÐ          ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "È±Ë®            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ÏµÍ³¹ÊÕÏ        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "Õý³£            ";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);


}
void lcd_proc()
{
//    static unsigned char i;
//    unsigned char *str;
    char ptr[20];
    lcd_boot();
    touch_pro();
    registerTick(LCD_TICK,500,1,0);
    if(GetTickResult(LCD_TICK)==1)
    {
<<<<<<< HEAD
        printf("æ¶ˆæ¯’æ°´æ€»é‡åŠç”µè§£æ—¶é—´ï¼š	%0.2f(å‡)	   %0.1f(å°æ—¶)\n",GetSensor()->water_quantity,GetSensor()->sum_ele_time);


        //GetSensor()->water_quantity = GetSensor()->water_quantity + 0.2;
        if(getTouch()->next_page == Main_PAGE)//ä¸»é¡µé¢
=======
        printf("Ïû¶¾Ë®×ÜÁ¿¼°µç½âÊ±¼ä£º	%0.2f(Éý)	   %0.1f(Ð¡Ê±)\n",GetSensor()->water_quantity,GetSensor()->sum_ele_time);


        //GetSensor()->water_quantity = GetSensor()->water_quantity + 0.2;
        if(getTouch()->next_page == Main_PAGE)//Ö÷Ò³Ãæ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
#if DEV_TYPE == 1

#if LCD_ORI == 1
            sprintf(ptr,"%2.2f",GetSensor()->ph);
            SetLableValue(Main_PAGE,LABEL_PH_ID,(unsigned char *)ptr);
            sprintf(ptr,"%4.0f",GetSensor()->orp);
            SetLableValue(Main_PAGE,LABEL_ORP_ID,(unsigned char *)ptr);//orp

#else
            //SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);


            sprintf(ptr,"%2.2f",GetSensor()->ph);
            SetLableValueColor(Main_PAGE,LABEL_PH_ID,BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);
            sprintf(ptr,"%4.0f",GetSensor()->orp);
            SetLableValueColor(Main_PAGE,LABEL_ORP_ID,BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);//orp

#endif


#endif

            module_dis_proc(1);
            module_dis_proc(2);
            module_dis_proc(3);
            module_dis_proc(4);

#if LCD_ORI == 1
            sprintf(ptr,"%4.2f",GetSensor()->flow);
            SetLableValue(Main_PAGE,LABEL_FLOW_ID,(unsigned char *)ptr);
            // module_dis_proc(2);//c2


            sprintf(ptr,"%4.0f",GetSensor()->tds1);
            SetLableValue(Main_PAGE,LABEL_OWTDS_ID,(unsigned char *)ptr);
            // module_dis_proc(3);//c3


            sprintf(ptr,"%4.0f",GetSensor()->tds2);
            SetLableValue(Main_PAGE,LABEL_ITDS_ID,(unsigned char *)ptr);
            // module_dis_proc(4);//c4


#else
            sprintf(ptr,"%4.2f",GetSensor()->flow);
            SetLableValueColor(Main_PAGE,LABEL_FLOW_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(2);//c2


            sprintf(ptr,"%4.0f",GetSensor()->tds1);
            SetLableValueColor(Main_PAGE,LABEL_OWTDS_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(3);//c3


            sprintf(ptr,"%4.0f",GetSensor()->tds2);
            SetLableValueColor(Main_PAGE,LABEL_ITDS_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(4);//c4

#endif


            Mode_dis_proc();
            // lcd_test();




        }
#if DEV_TYPE == 1
<<<<<<< HEAD
        else if(getTouch()->next_page == CALIBRATION_PAGE)//æ ¡å‡†é¡µé¢
        {
            //SetPage(CALIBRATION_PAGE);//ä¸»é¡µé¢Idå·æ˜¯4
=======
        else if(getTouch()->next_page == CALIBRATION_PAGE)//Ð£×¼Ò³Ãæ
        {
            //SetPage(CALIBRATION_PAGE);//Ö÷Ò³ÃæIdºÅÊÇ4
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            if(getTouch()->control_id == PH1_CAL_ID)
            {
                sprintf(ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,PH1_EDIT_ID,(unsigned char *)ptr);//ph1

            }
            if(getTouch()->control_id == PH2_CAL_ID)
            {
                sprintf(ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,PH2_EDIT_ID,(unsigned char *)ptr);//ph2

            }
            //if(getTouch()->control_id == ORP_CAL_ID&&getTouch()->key !=ENTER_ID)

            if(getTouch()->key ==ENTER_ID&&getTouch()->control_id != PH1_EDIT_ID)
            {
                sprintf(( char*)ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,ORP_EDIT_ID,(unsigned char *)ptr);//ORP
            }
        }
<<<<<<< HEAD
        else  if(getTouch()->next_page == SETTING_PAGE)//è®¾ç½®é¡µé¢
=======
        else  if(getTouch()->next_page == SETTING_PAGE)//ÉèÖÃÒ³Ãæ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
            SetPage(SETTING_PAGE);//
        }
#endif
<<<<<<< HEAD
        else  if(getTouch()->next_page == REBOOT_PAGE)//é‡å¯é¡µé¢
        {
            SetPage(REBOOT_PAGE);//ä¸»é¡µé¢Idå·æ˜¯4
=======
        else  if(getTouch()->next_page == REBOOT_PAGE)//ÖØÆôÒ³Ãæ
        {
            SetPage(REBOOT_PAGE);//Ö÷Ò³ÃæIdºÅÊÇ4
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }
#if DEV_TYPE == 1

        else
        {
            if(getTouch()->next_page != KEYBOARD_PAGE)
            {
<<<<<<< HEAD
                SetPage(Main_PAGE);//ä¸»é¡µé¢Idå·æ˜¯4
=======
                SetPage(Main_PAGE);//Ö÷Ò³ÃæIdºÅÊÇ4
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                getTouch()->page_id = Main_PAGE;
                getTouch()->next_page == Main_PAGE;
            }


        }
#endif

        registerTick(LCD_TICK,0,0,1);

    }


}

unsigned char repeat_flag=0;
static unsigned char addr_tmp = M1_ADDR;
unsigned char get_reset_status()
{
    return repeat_flag;
}
module_reset(unsigned char mode)
{
    static unsigned char buf[2];//,tmp;


    buf[0] = 0x00;
    buf[1] = 0x00;
    //  tmp = 0;

    registerTick(MODULE_RESET_TICK, 300, 1,0);//
    if(GetTickResult(MODULE_RESET_TICK)==1)
    {
        if(repeat_flag<3)//reapte 4 times
        {

            if(addr_tmp<=M4_ADDR) //ele module reset
            {
                if(GetSensor()->ele_MOnLine[addr_tmp-3])
                {
                    if(mode == 1)
<<<<<<< HEAD
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//å¤ä½
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//åœæ­¢ç”µè§£
=======
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//¸´Î»
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//Í£Ö¹µç½â
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

                }
                addr_tmp++;
                //delay_ms(100);
            }
            else
            {
                repeat_flag ++ ;
                addr_tmp = M1_ADDR;

            }

        }
        else
        {
            ;//module_reset_flag = 0;
        }
        registerTick(MODULE_RESET_TICK, 0, 0,1);//

    }



}
void water_levelAbnormal_proc()
{

#if SW_NOCHANGE
<<<<<<< HEAD
    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#endif
    EleSwCtrl(SALT_SW,ON);//ç›ç›’è¿›æ°´é˜€å¼€

#if WATER_L_IGNORE == 0
    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    DcMotorCtrl(2,0);//å…³æ³µ2
=======
    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#endif
    EleSwCtrl(SALT_SW,ON);//ÑÎºÐ½øË®·§¿ª

#if WATER_L_IGNORE == 0
    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
    DcMotorCtrl(2,0);//¹Ø±Ã2
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    DcMotorCtrl(3,0);
    DcMotorCtrl(1,0);
#endif
    GetSensor()->water_status = 1;
    // delay_ms(500);

}
void dev_init_ctrl()
{
<<<<<<< HEAD
    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
#endif
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å¼€
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
=======
    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
#endif
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¿ª
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    // delay_ms(500);

}
void dev_flow_Abnormal_ctrl()
{
#if FLOW_FAULT_ENABLE ==1
    if(abnormalDec()&0x01)
    {

<<<<<<< HEAD
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//æµé‡å¼‚å¸¸
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ç¼ºæ°´
=======
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//Á÷Á¿Òì³£
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//È±Ë®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }

#endif

<<<<<<< HEAD
    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
#endif
    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å…³
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    //DcMotorCtrl(1,OFF);//å…³æ‰€æœ‰ç”µæœº
    DcMotorCtrl(2,OFF);//å…³æ‰€æœ‰ç”µæœº
=======
    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
#endif
    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¹Ø
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
    //DcMotorCtrl(1,OFF);//¹ØËùÓÐµç»ú
    DcMotorCtrl(2,OFF);//¹ØËùÓÐµç»ú
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
}
void dev_normal_ctrl()
{
    GetSensor()->status[FLOW_INDEX] = 0;//
<<<<<<< HEAD
    GetSensor()->status[NORMAL_INDEX] = 20;//æ— å¼‚å¸¸
    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
#endif
    if(GetSensor()->flow >0)
        EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€
=======
    GetSensor()->status[NORMAL_INDEX] = 20;//ÎÞÒì³£
    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
#endif
    if(GetSensor()->flow >0)
        EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    // FlowCtrl();
    // delay_ms(500);

}

void dev_wash_ctrl()
{
<<<<<<< HEAD
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//å¤§äºŽ3æ¬¡åœæœº
    GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2å¼‚å¸¸
    GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//åœæœº

    GetSensor()->err_flag = 0;
    // GetSensor()->status[TDS2_INDEX] = 0;//tds2å¼‚å¸¸
    //GetSensor()->status[ORP_INDEX] = 0;//orpå¼‚å¸¸
    // GetSensor()->status[PH_INDEX] = 0;//phå¼‚å¸¸
    EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
#endif


    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€
=======
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//´óÓÚ3´ÎÍ£»ú
    GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2Òì³£
    GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//Í£»ú

    GetSensor()->err_flag = 0;
    // GetSensor()->status[TDS2_INDEX] = 0;//tds2Òì³£
    //GetSensor()->status[ORP_INDEX] = 0;//orpÒì³£
    // GetSensor()->status[PH_INDEX] = 0;//phÒì³£
    EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
#endif


    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    GetSensor()->wash_time = 3;



    registerTick(WASH_TICK, 0,0,1);
    registerTick(TDS_TICK_NO, 0,0,1);//
    if((abnormalDec()&0x08)) //orp abnormal
        registerTick(ORP_TICK_NO, 0,0,1);//
    if((abnormalDec()&0x04))	//ph abnormal
        registerTick(PH_TICK_NO, 0,0,1);//

}

void power_off()
{
<<<<<<< HEAD
    EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å…³
    DcMotorCtrl(7,OFF);//å…³æ‰€æœ‰ç”µæœº
=======
    EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¹Ø
    DcMotorCtrl(7,OFF);//¹ØËùÓÐµç»ú
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

}
float motor_pwm;
unsigned char pump_result = 0;
unsigned char *getpumpstatus()
{
    return &pump_result;
}
unsigned char  pump_ctrl(unsigned char mode)
{
    static unsigned char pump_flag=0,init_flag;
    static uint32_t delay_cnt = 0;



<<<<<<< HEAD
    registerTick(PUMP_TICK_NO, 18000,1, 0);//æ²¡æ°´å¸3åˆ†é’Ÿ
=======

	#if PUMP_3MIN
    registerTick(PUMP_TICK_NO, 180000,1, 0);

	#else
    registerTick(PUMP_TICK_NO, 15000,1, 0);

	#endif
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    if( GetTickResult(PUMP_TICK_NO)==1)
    {
        if(GetSensor()->flow == 0)
        {
            DcMotorCtrl(3,0);

<<<<<<< HEAD
            pump_result = 1;//è‡ªå¸æ²¡æ°´
        }
        else
            pump_result = 2; //è‡ªå¸æœ‰æ°´
=======
            pump_result = 1;//×ÔÎüÃ»Ë®
        }
        else
            pump_result = 2; //×ÔÎüÓÐË®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        registerTick(PUMP_TICK_NO, 0,0, 1);

    }
    else
    {
        if(GetSensor()->status[NOWATER_INDEX]==0)
        {
<<<<<<< HEAD
            DcMotorCtrl(3,65535);
            pump_result = 0;
=======
            DcMotorCtrl(3,pid_proc_pump(GetSensor()->flow));
            pump_result = 0;
        }

        else
        {
            DcMotorCtrl(3,0);
            pump_result = 0;

>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }
    }
    return pump_result;
}



void pump_auto(unsigned char flag)
{
    /*static unsigned char pum_flag=0;
    static unsigned int delay_cnt=0;
    if(flag == 1)
    {
        delay_cnt = 0;
        pum_flag=0;
    }

    else
    {
<<<<<<< HEAD
        //è‡ªå¸æ¨¡å¼
=======
        //×ÔÎüÄ£Ê½
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        if(GetSensor()->flow > 0)
        {
            delay_cnt  = 0;
            if(pum_flag == 1)
                pump_ctrl(0);
        }
        else
        {
            if(delay_cnt <100000)
                delay_cnt ++;
            if(delay_cnt>=100000)
            {

                if(*getpumpstatus()==0)
<<<<<<< HEAD
                    pump_ctrl(1);//æ²¡æ°´æ³µå·¥ä½œ30s
=======
                    pump_ctrl(1);//Ã»Ë®±Ã¹¤×÷30s
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                else if(*getpumpstatus()==2)
                    pum_flag = 1;

            }

        }
        if(GetSensor()->status[NOWATER_INDEX]==NOWATER_INDEX)
        {
            pum_flag = 0;
        }


    }*/

<<<<<<< HEAD
    /*static unsigned int delay_cnt=0;

    	if(GetSensor()->flow > 0)
    	{
    		delay_cnt  = 0;
    		DcMotorCtrl(3,65535);

    	}
    	else
    	{

    		///if(delay_cnt <100000)
    		{
    			//DcMotorCtrl(3,pid_proc_pump(GetSensor()->flow));
    			delay_cnt ++;

    		}

    		//if(delay_cnt>=100000)
    		{
            //   delay_cnt  = 0;
    			DcMotorCtrl(3,0);


    		}

    	}*/
    registerTick(PUMP_TICK_NO, 18000,1, 0);//æ²¡æ°´å¸3åˆ†é’Ÿ
    if(flag == 0)
    {
        if( GetTickResult(PUMP_TICK_NO)==1)
        {
            DcMotorCtrl(3,0);
            pump_result = 1;
        }
        else
        {
            DcMotorCtrl(3,65535);
        }


    }


    if(GetSensor()->flow > FLOW_SIZE)
    {
        registerTick(PUMP_TICK_NO, 0,0, 1);//æ²¡æ°´å¸3åˆ†é’Ÿ
        pump_result = 0;
    }
=======
    static unsigned int delay_cnt=0;
#if PUMP_3MIN
    registerTick(PUMP_TICK_NO, 180000,1, 0);
	
    if( GetTickResult(PUMP_TICK_NO)==1)
    {
        if(GetSensor()->flow > 0)
        {
			DcMotorCtrl(3,65535);
			registerTick(PUMP_TICK_NO, 0,0,1);
		}
		else
        DcMotorCtrl(3,0);
        //registerTick(PUMP_TICK_NO, 0,0,1);
    }
	else
	{
		DcMotorCtrl(3,65535);

	}

#else
    if(GetSensor()->flow > 0)
    {
        delay_cnt  = 0;
        DcMotorCtrl(3,pid_proc_pump(GetSensor()->flow));

    }
    else
    {

        ///if(delay_cnt <100000)
        {
            //DcMotorCtrl(3,pid_proc_pump(GetSensor()->flow));
            delay_cnt ++;

        }

        //if(delay_cnt>=100000)
        {
            //	 delay_cnt	= 0;
            DcMotorCtrl(3,0);


        }

    }

#endif


>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

}
//uint32_t time_out_sw=0;
//static unsigned char module_flag=0;
static uint32_t flow_low_cnt;
static unsigned char hsw_flag=0;
unsigned char flow_proc()
{
    static unsigned char result;
    unsigned char tmp=0 ;
    result = 0;

    tmp = abnormalDec()&0x10;
<<<<<<< HEAD
    if(tmp==0&&GetSensor()->temperature>=5)//é«˜åŽ‹é˜€å¼‚å¸¸,ä½Žæ¸©ä¸å·¥ä½œ
    {

        if(GetSensor()->flow<FLOW_VALUE)	//æ°´æµé‡ä¸è¶³
        {

            flow_low_cnt++;
            if(flow_low_cnt>=400000)
=======
    if(tmp==0&&GetSensor()->temperature>=5)//¸ßÑ¹·§Òì³£,µÍÎÂ²»¹¤×÷
    {

        if(GetSensor()->flow<FLOW_VALUE||GetSensor()->swH == OFF)	//Ë®Á÷Á¿²»×ã
        {

            flow_low_cnt++;
            if(flow_low_cnt>=400000||GetSensor()->swH == OFF)
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            {
                flow_low_cnt = 0;
#if FLOW_FAULT_ENABLE ==1
                if(GetSensor()->flow==0)
<<<<<<< HEAD
                    GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//æµé‡å¼‚å¸¸
                else
                    GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
#endif
                if(GetSensor()->flow<=MIN_FLOW_SIZE&&GetSensor()->status[TDS2_INDEX] ==0&&GetSensor()->status[WATER_LEVEL_INDEX]==0)
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ç¼ºæ°´
=======
                    GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//Á÷Á¿Òì³£
                else
                    GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
#endif
                if(GetSensor()->flow<=MIN_FLOW_SIZE&&GetSensor()->status[TDS2_INDEX] ==0&&GetSensor()->status[WATER_LEVEL_INDEX]==0)
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//È±Ë®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                result = 1;
            }
        }

        else
        {
            if(*getpumpstatus()!=1)
            {
                result = 0;
<<<<<<< HEAD
                GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
                GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
=======
                GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
                GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                flow_low_cnt =0;
            }


        }
        if(*getpumpstatus()==1&&GetSensor()->flow<=MIN_FLOW_SIZE&&GetSensor()->status[TDS2_INDEX] ==0)
        {
            if(GetSensor()->flow<=MIN_FLOW_SIZE)
                GetSensor()->status[NOWATER_INDEX]=NOWATER_INDEX;

        }
        else if(GetSensor()->flow>MIN_FLOW_SIZE)
        {
            if(*getpumpstatus()==1)
                *getpumpstatus() = 0;
        }
    }


    return result;
}
void tds_proc()
{
    if(abnormalDec()&0x40)
    {
<<<<<<< HEAD
        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1å¼‚å¸¸
    }
    else
    {
        GetSensor()->status[TDS1_INDEX] = 0;//tds1å¼‚å¸¸


    }//tds2å¼‚å¸¸
    if(GetSensor()->err_flag&0x01)//tdsæŠ¥è­¦
    {
        dev_wash_ctrl();

    }//tdsæŠ¥è­¦,è‡ªåŠ¨æ¸…æ´—
    else
    {
        //GetSensor()->status[TDS2_INDEX] = 0;//tds2å¼‚å¸¸
        EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
        if(GetSensor()->flow >0)
            EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
        EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
        EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
=======
        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1Òì³£
    }
    else
    {
        GetSensor()->status[TDS1_INDEX] = 0;//tds1Òì³£


    }//tds2Òì³£
    if(GetSensor()->err_flag&0x01)//tds±¨¾¯
    {
        dev_wash_ctrl();

    }//tds±¨¾¯,×Ô¶¯ÇåÏ´
    else
    {
        //GetSensor()->status[TDS2_INDEX] = 0;//tds2Òì³£
        EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
        if(GetSensor()->flow >0)
            EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
        EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
        EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }

}
void pump_hw(unsigned char flag)
{
    static unsigned int delay_cnt=0;

    if(flag == 0)
    {
        registerTick(HW_TICK, 2000, 1,0);//
        if(delay_cnt<10)
            delay_cnt++;
        else
            delay_cnt = 0;
<<<<<<< HEAD
        if(GetTickResult(HW_TICK)==1)//å¢žåŽ‹æ³µå·¥ä½œï¼Œé©±åŠ¨é«˜åŽ‹å¼€å…³
=======
        if(GetTickResult(HW_TICK)==1)//ÔöÑ¹±Ã¹¤×÷£¬Çý¶¯¸ßÑ¹¿ª¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
            DcMotorCtrl(3,0);
            registerTick(HW_TICK, 0, 0,1);//

        }
        else
        {
            if(delay_cnt>10)
                DcMotorCtrl(3,30000);

        }

    }
    else
    {
        registerTick(HW_TICK, 0, 0,1);//
        delay_cnt = 0;

    }



}
void hsw_proc()
{
<<<<<<< HEAD
    // EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
    //EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å…³
    DcMotorCtrl(7,0);//å…³æ‰€æœ‰ç”µæœº
    //pump_ctrl(1);
    //pump_ctrl(0);
    flow_low_cnt = 0;
    GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//æ°´ä½å¼‚å¸¸
    GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
    GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
    GetSensor()->status[TDS2_INDEX] = 0;//ç¼ºæ°´
    GetSensor()->status[TDS1_INDEX] = 0;//ç¼ºæ°´

    // EleSwCtrl(6,OFF);//å…³æ‰€æœ‰é˜€
    if(abnormalDec()&0x20)//æ°´ä½å¼‚å¸¸
=======
    // EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
    //EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¹Ø
    DcMotorCtrl(7,0);//¹ØËùÓÐµç»ú
    //pump_ctrl(1);
    //pump_ctrl(0);
    flow_low_cnt = 0;
    GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//Ë®Î»Òì³£
    GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
    GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
    GetSensor()->status[TDS2_INDEX] = 0;//È±Ë®
    GetSensor()->status[TDS1_INDEX] = 0;//È±Ë®

    // EleSwCtrl(6,OFF);//¹ØËùÓÐ·§
    if(abnormalDec()&0x20)//Ë®Î»Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {


        if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
        {

            water_levelAbnormal_proc();


        }


<<<<<<< HEAD
    }//end æ°´ä½å¼‚å¸¸
    else //æ°´ä½æ­£å¸¸
    {
        GetSensor()->status[WATER_LEVEL_INDEX] = 0;//é«˜åŽ‹å¼€å…³å¼‚å¸¸
#if SW_NOCHANGE
        //EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
        EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
#else
        EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#endif

        EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
=======
    }//end Ë®Î»Òì³£
    else //Ë®Î»Õý³£
    {
        GetSensor()->status[WATER_LEVEL_INDEX] = 0;//¸ßÑ¹¿ª¹ØÒì³£
#if SW_NOCHANGE
        //EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
        EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
#else
        EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#endif

        EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }

    {

        // #if SW_NOCHANGE
<<<<<<< HEAD
        //EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
        // EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
        //  #else
        ;//EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
        // #endif
        EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
        EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
        EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å…³
=======
        //EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
        // EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
        //  #else
        ;//EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
        // #endif
        EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
        EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
        EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        DcMotorCtrl(2,0);
        DcMotorCtrl(3,0);
        registerTick(HW_TICK, 0, 0,1);

        /*registerTick(HW_TICK, 500, 1,0);//
        if(GetTickResult(HW_TICK)==1)
        {
            DcMotorCtrl(3,0);

            // registerTick(HW_TICK, 0, 0,1);//

        }
        else
        {
            DcMotorCtrl(3,30000);

        }*/

    }



<<<<<<< HEAD
    GetSensor()->status[HSW_INDEX] = 5;//é«˜åŽ‹å¼€å…³å¼‚å¸¸
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//æµé‡å¼‚å¸¸
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//æµé‡æ­£å¸¸
            GetSensor()->status[NOWATER_INDEX] = 0;//æµé‡æ­£å¸¸
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//æµé‡æ­£å¸¸
#if FLOW_FAULT_ENABLE == 1
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//æµé‡å¼‚å¸¸
=======
    GetSensor()->status[HSW_INDEX] = 5;//¸ßÑ¹¿ª¹ØÒì³£
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//Á÷Á¿Òì³£
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Õý³£
            GetSensor()->status[NOWATER_INDEX] = 0;//Á÷Á¿Õý³£
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//Á÷Á¿Õý³£
#if FLOW_FAULT_ENABLE == 1
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//Á÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#endif
        }
        registerTick(FLOW_TICK_NO2, 0,0,1);//

    }
    else
    {
<<<<<<< HEAD
        GetSensor()->status[FLOW_INDEX] = 0;//æµé‡æ­£å¸¸
        GetSensor()->status[NOWATER_INDEX] = 0;//æµé‡æ­£å¸¸
=======
        GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Õý³£
        GetSensor()->status[NOWATER_INDEX] = 0;//Á÷Á¿Õý³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }

    if(GetSensor()->status[FLOW_INDEX])
<<<<<<< HEAD
        GetSensor()->status[HSW_INDEX] =    5;//é«˜åŽ‹å¼€å…³å¼‚å¸¸
=======
        GetSensor()->status[HSW_INDEX] =    5;//¸ßÑ¹¿ª¹ØÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    else
        GetSensor()->status[HSW_INDEX] =    0;//
    if(hsw_flag==0)
        repeat_flag = 0;

    if(repeat_flag <3&&GetInOut()->key_cali_mode == 0)
    {
        module_reset(0);
    }
    else


        work_params.init_flag = 1;
    GetSensor()->err_flag=0;


    hsw_flag = 1;
    registerTick(TDS_TICK_NO, 0,0,1);//
<<<<<<< HEAD
    registerTick(FLOW_TICK_NO, 0, 0,1);//è¶…æ—¶è®¡æ—¶å¼€å§‹
    registerTick(PUMP_TICK_NO, 0,0,1);//æ²¡æ°´å¸3åˆ†é’Ÿ
=======
    registerTick(FLOW_TICK_NO, 0, 0,1);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

}
void normal_proc()
{
    //float flow_tmp;

    if( hsw_flag == 1)
    {
        *getpumpstatus()=0;
        registerTick(PUMP_TICK_NO, 0,0, 1);

        //pump_hw(1);
        //   flow_low_cnt = 0;
        hsw_flag = 0;
        registerTick(TDS_TICK_NO, 0,0,1);//
<<<<<<< HEAD
        registerTick(FLOW_TICK_NO, 0, 0,1);//è¶…æ—¶è®¡æ—¶å¼€å§‹
=======
        registerTick(FLOW_TICK_NO, 0, 0,1);//³¬Ê±¼ÆÊ±¿ªÊ¼
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }
    if(GetSensor()->flow > 0)
    {

        registerTick(HW_TICK, 0, 0,1);//
    }


    registerTick(FLOW_TICK_NO2, 0,0,1);
<<<<<<< HEAD
    if(abnormalDec()&0x10==0)//é«˜åŽ‹å¼€å…³å¼‚å¸¸
        GetSensor()->status[HSW_INDEX] = 0;//é«˜åŽ‹å¼€å…³å¼‚å¸¸

    if(GetSensor()->status[SHUNT_INDEX] == 0)//éžåœæœºæ¨¡å¼
    {
        //if(if(GetSensor()->flow <=0))
        //è‡ªå¸æ¨¡å¼
        pump_auto(0);
        flow_proc();  //2022.10.4
        //else

        if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//æ°´ä½å¼‚å¸¸
=======
    if(abnormalDec()&0x10==0)//¸ßÑ¹¿ª¹ØÒì³£
        GetSensor()->status[HSW_INDEX] = 0;//¸ßÑ¹¿ª¹ØÒì³£

    if(GetSensor()->status[SHUNT_INDEX] == 0)//·ÇÍ£»úÄ£Ê½
    {
        //if(if(GetSensor()->flow <=0))
        //×ÔÎüÄ£Ê½
        pump_auto(0);
        //else

        if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//Ë®Î»Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {

            //pump_ctrl(1);
            // pump_ctrl(0);
            flow_low_cnt = 0;
<<<<<<< HEAD
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//æ°´ä½å¼‚å¸¸
            GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
            GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
            GetSensor()->status[TDS2_INDEX] = 0;//ç¼ºæ°´
            GetSensor()->status[TDS1_INDEX] = 0;//ç¼ºæ°´
=======
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//Ë®Î»Òì³£
            GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
            GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
            GetSensor()->status[TDS2_INDEX] = 0;//È±Ë®
            GetSensor()->status[TDS1_INDEX] = 0;//È±Ë®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {

                water_levelAbnormal_proc();



            }
<<<<<<< HEAD
            else //æ°´ä½å¼€å…³å¼‚å¸¸
            {
                EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
            }

        }//end æ°´ä½å¼‚å¸¸
#if WATER_L_IGNORE == 1
        else//æ°´ä½æ­£å¸¸ï¼Œå…¶ä»–å‚æ•°æ£€æµ‹æ‰æœ‰æ„ä¹‰
        {
            EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
            GetSensor()->water_status = 0;//æ°´ä½å¼‚å¸¸é‡æ–°åŠ æ°´åŠ æ»¡çŠ¶æ€æ ‡å¿—ä½
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//æ°´ä½æ— å¼‚å¸¸
=======
            else //Ë®Î»¿ª¹ØÒì³£
            {
                EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
            }

        }//end Ë®Î»Òì³£
#if WATER_L_IGNORE == 1
        else//Ë®Î»Õý³££¬ÆäËû²ÎÊý¼ì²â²ÅÓÐÒâÒå
        {
            EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
            GetSensor()->water_status = 0;//Ë®Î»Òì³£ÖØÐÂ¼ÓË®¼ÓÂú×´Ì¬±êÖ¾Î»
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//Ë®Î»ÎÞÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }
#endif


#if WATER_L_IGNORE == 0
<<<<<<< HEAD
        else//æ°´ä½æ­£å¸¸ï¼Œå…¶ä»–å‚æ•°æ£€æµ‹æ‰æœ‰æ„ä¹‰
=======
        else//Ë®Î»Õý³££¬ÆäËû²ÎÊý¼ì²â²ÅÓÐÒâÒå
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#endif
        {




#if WATER_L_IGNORE == 0

<<<<<<< HEAD
            EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
            GetSensor()->water_status = 0;//æ°´ä½å¼‚å¸¸é‡æ–°åŠ æ°´åŠ æ»¡çŠ¶æ€æ ‡å¿—ä½
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//æ°´ä½æ— å¼‚å¸¸
#endif


            if(work_params.init_flag == 0)//ä¸Šç”µå¼€æœº
=======
            EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
            GetSensor()->water_status = 0;//Ë®Î»Òì³£ÖØÐÂ¼ÓË®¼ÓÂú×´Ì¬±êÖ¾Î»
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//Ë®Î»ÎÞÒì³£
#endif


            if(work_params.init_flag == 0)//ÉÏµç¿ª»ú
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            {
                dev_init_ctrl();
                if(GetSensor()->flow > 0.1)
                {
<<<<<<< HEAD
                    EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//æµé‡å¼‚å¸¸
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//å®šæ—¶å™¨å¤ä½
=======
                    EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//Á÷Á¿Òì³£
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                    if(GetSensor()->flow == 0)//&&hsw_flag == 1
                    {
                        dev_flow_Abnormal_ctrl();
                        //pump_hw(0);
                        pid_init2(dstTds);
                    }
                }
                else
                {
                    // pump_hw(1);
<<<<<<< HEAD
                    if(GetSensor()->flow != 0)//æµé‡å¼‚å¸¸
                        FlowCtrl();
                    else
                    {
                        DcMotorCtrl(2,0);//æ³µ2è°ƒæ•´æµé‡
=======
                    if(GetSensor()->flow != 0)//Á÷Á¿Òì³£
                        FlowCtrl();
                    else
                    {
                        DcMotorCtrl(2,0);//±Ã2µ÷ÕûÁ÷Á¿
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                        pid_init2(dstTds);

                    }


                }

            }
<<<<<<< HEAD
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//æµé‡å¼‚å¸¸
=======
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//Á÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            {

                if(GetSensor()->flow == 0)//&&hsw_flag == 1
                {
                    dev_flow_Abnormal_ctrl();
                    // pump_hw(0);
                    pid_init2(dstTds);
                }
                else
                {
<<<<<<< HEAD
                    EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
                }

            }
            else  //æµé‡æ­£å¸¸
            {
                // pump_hw(1);
                if(GetSensor()->flow != 0)//æµé‡å¼‚å¸¸
=======
                    EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
                }

            }
            else  //Á÷Á¿Õý³£
            {
                // pump_hw(1);
                if(GetSensor()->flow != 0)//Á÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                    FlowCtrl();
                else
                {
                    // Flow_Init() ;
                    pid_init2(dstTds);
<<<<<<< HEAD
                    DcMotorCtrl(2,0);//æ³µ2è°ƒæ•´æµé‡
                    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€
=======
                    DcMotorCtrl(2,0);//±Ã2µ÷ÕûÁ÷Á¿
                    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

                }


<<<<<<< HEAD
                //GetSensor()->status[NOWATER_INDEX] = 0;//æµé‡å¼‚å¸¸
                // GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
                if((abnormalDec()&0x73)==0)//æ— å¼‚å¸¸ï¼Œèµ°æ­£å¸¸é€»è¾‘ 0x73 0x33
=======
                //GetSensor()->status[NOWATER_INDEX] = 0;//Á÷Á¿Òì³£
                // GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
                if((abnormalDec()&0x73)==0)//ÎÞÒì³££¬×ßÕý³£Âß¼­ 0x73 0x33
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {

                    dev_normal_ctrl();

                }
<<<<<<< HEAD
                else //å‚æ•°æœ‰å¼‚å¸¸è¿›è¡Œè°ƒèŠ‚æŽ§åˆ¶
=======
                else //²ÎÊýÓÐÒì³£½øÐÐµ÷½Ú¿ØÖÆ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {

                    tds_proc();


<<<<<<< HEAD
                }//å‚æ•°æœ‰å¼‚å¸¸è¿›è¡Œè°ƒèŠ‚æŽ§åˆ¶


            } //æµé‡æ­£å¸¸
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //æ°´æµé‡ä¸è¶³


            //flow_proc();  //2022.10.4


        }// end æ°´ä½æ­£å¸¸ï¼Œå…¶ä»–å‚æ•°æ£€æµ‹æ‰æœ‰æ„ä¹‰

    }//éžåœæœºæ¨¡å¼
    else //åœæœºæ¨¡å¼
    {
        if(abnormalDec()&0x20)//æ°´ä½å¼‚å¸¸
=======
                }//²ÎÊýÓÐÒì³£½øÐÐµ÷½Ú¿ØÖÆ


            } //Á÷Á¿Õý³£
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //Ë®Á÷Á¿²»×ã


            flow_proc();


        }// end Ë®Î»Õý³££¬ÆäËû²ÎÊý¼ì²â²ÅÓÐÒâÒå

    }//·ÇÍ£»úÄ£Ê½
    else //Í£»úÄ£Ê½
    {
        if(abnormalDec()&0x20)//Ë®Î»Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {


            //pump_ctrl(0);
            flow_low_cnt = 0;
<<<<<<< HEAD
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//æ°´ä½å¼‚å¸¸
            GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
            GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
            GetSensor()->status[TDS2_INDEX] = 0;//ç¼ºæ°´
            GetSensor()->status[TDS1_INDEX] = 0;//ç¼ºæ°´
=======
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//Ë®Î»Òì³£
            GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
            GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
            GetSensor()->status[TDS2_INDEX] = 0;//È±Ë®
            GetSensor()->status[TDS1_INDEX] = 0;//È±Ë®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {

                water_levelAbnormal_proc();



            }
<<<<<<< HEAD
            else //æ°´ä½å¼€å…³å¼‚å¸¸
            {
                EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
            }


        }////end æ°´ä½å¼‚
#if WATER_L_IGNORE == 1
        else//æ°´ä½æ­£å¸¸ï¼Œå…¶ä»–å‚æ•°æ£€æµ‹æ‰æœ‰æ„ä¹‰
        {
            EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
            GetSensor()->water_status = 0;//æ°´ä½å¼‚å¸¸é‡æ–°åŠ æ°´åŠ æ»¡çŠ¶æ€æ ‡å¿—ä½
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//æ°´ä½æ— å¼‚å¸¸
=======
            else //Ë®Î»¿ª¹ØÒì³£
            {
                EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
            }


        }////end Ë®Î»Òì
#if WATER_L_IGNORE == 1
        else//Ë®Î»Õý³££¬ÆäËû²ÎÊý¼ì²â²ÅÓÐÒâÒå
        {
            EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
            GetSensor()->water_status = 0;//Ë®Î»Òì³£ÖØÐÂ¼ÓË®¼ÓÂú×´Ì¬±êÖ¾Î»
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//Ë®Î»ÎÞÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }
#endif

        if(abnormalDec()&0x40)
        {
<<<<<<< HEAD
            GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1å¼‚å¸¸
        }
        else
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds1å¼‚å¸¸
=======
            GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1Òì³£
        }
        else
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds1Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            /* if(abnormalDec()&0x02==0&&GetSensor()->flow>0)
             {
                 if((GetSensor()->tds2-dstTds)>=150)
                 {
                     GetSensor()->err_flag = 0;
<<<<<<< HEAD
                     GetSensor()->wash_time =0;//GetSensor()->wash_time +1 ;//å¤§äºŽ3æ¬¡åœæœº
                     GetSensor()->status[TDS2_INDEX] = 0;//tds2å¼‚å¸¸
                     GetSensor()->status[SHUNT_INDEX] = 0;//tds2å¼‚å¸¸
                     EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
                     EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
                     EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
                     EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
=======
                     GetSensor()->wash_time =0;//GetSensor()->wash_time +1 ;//´óÓÚ3´ÎÍ£»ú
                     GetSensor()->status[TDS2_INDEX] = 0;//tds2Òì³£
                     GetSensor()->status[SHUNT_INDEX] = 0;//tds2Òì³£
                     EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
                     EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
                     EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
                     EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

                 }

             }*/

<<<<<<< HEAD
            GetSensor()->status[TDS1_INDEX] = 0;//tds2æ— å¼‚å¸¸
            GetSensor()->status[TDS2_INDEX] = 0;//tds2æ— å¼‚å¸¸

            GetSensor()->status[ORP_INDEX] = 0;//tds2æ— å¼‚å¸¸
            GetSensor()->status[PH_INDEX] = 0;//tds2æ— å¼‚å¸¸
            GetSensor()->wash_time =0;
        }
        flow_proc();
    }//åœæœºæ¨¡å¼

}
/*************************************************************************
å¯åŠ¨é€»è¾‘ï¼šé«˜åŽ‹é˜€æ­£å¸¸--->æ°´ä½æ­£å¸¸--->ç”µè§£æµç¨‹
          é«˜åŽ‹é˜€å¼‚å¸¸--->åœæœº
          æ°´ä½å¼‚å¸¸-->åœæœº
=======
            GetSensor()->status[TDS1_INDEX] = 0;//tds2ÎÞÒì³£
            GetSensor()->status[TDS2_INDEX] = 0;//tds2ÎÞÒì³£

            GetSensor()->status[ORP_INDEX] = 0;//tds2ÎÞÒì³£
            GetSensor()->status[PH_INDEX] = 0;//tds2ÎÞÒì³£
            GetSensor()->wash_time =0;
        }
        flow_proc();
    }//Í£»úÄ£Ê½

}
/*************************************************************************
Æô¶¯Âß¼­£º¸ßÑ¹·§Õý³£--->Ë®Î»Õý³£--->µç½âÁ÷³Ì
          ¸ßÑ¹·§Òì³£--->Í£»ú
          Ë®Î»Òì³£-->Í£»ú
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
***************************************************************************/
void ele_dev_proc()
{

    static unsigned char retry_flag=0;


    if(GetSensor()->wash_time>=3||GetSensor()->status[TDS1_INDEX]==TDS1_INDEX||GetSensor()->status[SYSTEM_INDEX]==SYSTEM_INDEX||GetSensor()->status[NOWATER_INDEX]==NOWATER_INDEX||
<<<<<<< HEAD
            GetSensor()->status[TDS2_INDEX] == TDS2_INDEX||GetSensor()->status[SHUNT_INDEX] == SHUNT_INDEX)//æœ‰æ•…éšœçŠ¶æ€
=======
            GetSensor()->status[TDS2_INDEX] == TDS2_INDEX||GetSensor()->status[SHUNT_INDEX] == SHUNT_INDEX)//
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        registerTick(SW_ON_TICK, 0,0, 1);

        if(shunt_flag ==0)
        {
            shunt_flag = 1;
            if(retry_flag==0)
            {
                while(get_reset_status() <3)
                {
                    module_reset(1);
                }
                retry_flag = 1;
                repeat_flag = 0;
            }

        }


        //module_reset(1);
        if(GetSensor()->status[SYSTEM_INDEX] == SYSTEM_INDEX)
        {
            GetSensor()->status[TDS1_INDEX] = 0;
            GetSensor()->status[TDS2_INDEX] = 0;
            GetSensor()->status[FLOW_INDEX] = 0;
            GetSensor()->status[ORP_INDEX] = 0;
            GetSensor()->status[PH_INDEX] = 0;
            GetSensor()->status[NOWATER_INDEX] = 0;
        }
<<<<<<< HEAD
        pump_auto(1);
=======
        else
        {

        }
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        flow_proc();
        if(GetSensor()->status[TDS1_INDEX]==TDS1_INDEX||GetSensor()->status[SYSTEM_INDEX] == SYSTEM_INDEX||GetSensor()->status[TDS2_INDEX] == TDS2_INDEX||GetSensor()->status[SHUNT_INDEX] == SHUNT_INDEX||
                GetSensor()->status[NOWATER_INDEX] == NOWATER_INDEX)
        {
            if((GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M))
            {

                water_levelAbnormal_proc();

            }
            else
            {
<<<<<<< HEAD
                EleSwCtrl(SALT_SW,OFF);//å…³æ‰€æœ‰é˜€

                //#if SW_NOCHANGE
                //EleSwCtrl(WATER_SW,OFF);//å…³æ‰€æœ‰é˜€
=======
                EleSwCtrl(SALT_SW,OFF);//¹ØËùÓÐ·§

                //#if SW_NOCHANGE
                //EleSwCtrl(WATER_SW,OFF);//¹ØËùÓÐ·§
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                //#endif

            }
            if(GetSensor()->status[NOWATER_INDEX] != NOWATER_INDEX)
            {
                GetSensor()->status[NORMAL_INDEX] = 0;//
                GetSensor()->status[WASH_INDEX] = 0;//
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//
<<<<<<< HEAD
                // EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
                //EleSwCtrl(6,OFF);//å…³æ‰€æœ‰é˜€
                DcMotorCtrl(7,OFF);//å…³æ‰€æœ‰ç”µæœº

                // EleSwCtrl(WATER_SW,OFF);//å…³æ‰€æœ‰é˜€
#if SW_NOCHANGE
                //if((GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M))

                //EleSwCtrl(WATER_SW,ON);//å…³æ‰€æœ‰é˜€
                EleSwCtrl(WATER_SW,OFF);//å…³æ‰€æœ‰é˜€
#else
                EleSwCtrl(WATER_SW,OFF);//å…³æ‰€æœ‰é˜€

                //EleSwCtrl(SALT_SW,OFF);//å…³æ‰€æœ‰é˜€
#endif
                EleSwCtrl(WASTE_SW,OFF);//å…³æ‰€æœ‰é˜€
                EleSwCtrl(WASH_SW,OFF);//å…³æ‰€æœ‰é˜€
                EleSwCtrl(HCILO_SW,OFF);//å…³æ‰€æœ‰é˜€
=======
                // EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
                //EleSwCtrl(6,OFF);//¹ØËùÓÐ·§
                DcMotorCtrl(7,OFF);//¹ØËùÓÐµç»ú

                // EleSwCtrl(WATER_SW,OFF);//¹ØËùÓÐ·§
#if SW_NOCHANGE
                //if((GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M))

                //EleSwCtrl(WATER_SW,ON);//¹ØËùÓÐ·§
                EleSwCtrl(WATER_SW,OFF);//¹ØËùÓÐ·§
#else
                EleSwCtrl(WATER_SW,OFF);//¹ØËùÓÐ·§

                //EleSwCtrl(SALT_SW,OFF);//¹ØËùÓÐ·§
#endif
                EleSwCtrl(WASTE_SW,OFF);//¹ØËùÓÐ·§
                EleSwCtrl(WASH_SW,OFF);//¹ØËùÓÐ·§
                EleSwCtrl(HCILO_SW,OFF);//¹ØËùÓÐ·§
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            }
            else
            {
<<<<<<< HEAD
                EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
                DcMotorCtrl(7,OFF);//å…³æ‰€æœ‰ç”µæœº
                EleSwCtrl(WASTE_SW,OFF);//å…³æ‰€æœ‰é˜€
                EleSwCtrl(WASH_SW,OFF);//å…³æ‰€æœ‰é˜€
                EleSwCtrl(HCILO_SW,OFF);//å…³æ‰€æœ‰é˜€
=======
                EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
                DcMotorCtrl(7,OFF);//¹ØËùÓÐµç»ú
                EleSwCtrl(WASTE_SW,OFF);//¹ØËùÓÐ·§
                EleSwCtrl(WASH_SW,OFF);//¹ØËùÓÐ·§
                EleSwCtrl(HCILO_SW,OFF);//¹ØËùÓÐ·§
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            }

        }
        else
        {
<<<<<<< HEAD
            EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
#if WATER_L_IGNORE == 0
            EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
#endif
            EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å¼€
            EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
=======
            EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
#if WATER_L_IGNORE == 0
            EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
#endif
            EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¿ª
            EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }


        //delay_ms(500);
    }
    else
    {
        retry_flag = 0;
<<<<<<< HEAD
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//é«˜åŽ‹é˜€å¼‚å¸¸,ä½Žæ¸©ä¸å·¥ä½œ
=======
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//¸ßÑ¹·§Òì³£,µÍÎÂ²»¹¤×÷
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {

            hsw_proc();
            registerTick(SW_ON_TICK, 0,0, 1);
            First_flag = 0;
        }
<<<<<<< HEAD
        else //æ­£å¸¸å·¥ä½œæ¨¡å¼
=======
        else //Õý³£¹¤×÷Ä£Ê½
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
            //state_flag=1;

            registerTick(SW_ON_TICK, 4000,1, 0);
            if( GetTickResult(SW_ON_TICK)==1)
            {
                First_flag = 0;

            }
            else
                First_flag = 1;
            Flow_Init();
            normal_proc();

        }

    }



}
static unsigned char state_flag=0;
static uint32_t params_tick;

/*************************************************************************

**************************************************************************/
void ele_process()
{
    unsigned char i=0;

    if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
    {

        water_levelAbnormal_proc();

    }
<<<<<<< HEAD
    else //æ°´ä½å¼€å…³å¼‚å¸¸
    {
        EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
    }


    if(GetInOut()->key_reset_mode )//å¤ä½é‡å¯
    {
        EleSwCtrl(6,OFF);//å…³æ‰€æœ‰é˜€
        DcMotorCtrl(7,OFF);//å…³æ‰€æœ‰ç”µæœº
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//ä¸»æœºå’Œæ¨¡å—å‡å¤ä½
        Soft_Reset();
    }

    if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)//è®¾ç½®é¡µé¢å…³é—­æ‰€æœ‰é˜€é—¨
=======
    else //Ë®Î»¿ª¹ØÒì³£
    {
        EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
    }


    if(GetInOut()->key_reset_mode )//¸´Î»ÖØÆô
    {
        EleSwCtrl(6,OFF);//¹ØËùÓÐ·§
        DcMotorCtrl(7,OFF);//¹ØËùÓÐµç»ú
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//Ö÷»úºÍÄ£¿é¾ù¸´Î»
        Soft_Reset();
    }

    if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        GetInOut()->key_wash_mode = 0;
        // registerTick(WASH_TICK, 0,0,1);
        //GetSensor()->status[WASH_INDEX] = 0;
<<<<<<< HEAD
        EleSwCtrl(6,OFF);//å…³æ‰€æœ‰é˜€
        DcMotorCtrl(7,OFF);//å…³æ‰€æœ‰ç”µæœº
        registerTick(FLOW_TICK_NO, 0, 0,1);//å®šæ—¶å™¨å¤ä½
        //delay_ms(500);

    }
    else if(GetInOut()->key_wash_mode)//å†²æ´—çŠ¶æ€
=======
        EleSwCtrl(6,OFF);//¹ØËùÓÐ·§
        DcMotorCtrl(7,OFF);//¹ØËùÓÐµç»ú
        registerTick(FLOW_TICK_NO, 0, 0,1);//¶¨Ê±Æ÷¸´Î»
        //delay_ms(500);

    }
    else if(GetInOut()->key_wash_mode)//³åÏ´×´Ì¬
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        if( GetSensor()->status[SYSTEM_INDEX]!=SYSTEM_INDEX&& GetSensor()->status[TDS2_INDEX]!=TDS2_INDEX&&
                GetSensor()->status[SHUNT_INDEX]!=SHUNT_INDEX)
        {
            //pump_ctrl(0);
            pump_auto(0);
<<<<<<< HEAD
            flow_proc();
            if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//æ°´ä½å¼‚å¸¸
            {

                GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//æ°´ä½å¼‚å¸¸
                GetSensor()->status[FLOW_INDEX] = 0;//æµé‡å¼‚å¸¸
                GetSensor()->status[NOWATER_INDEX] = 0;//ç¼ºæ°´
=======
            if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//Ë®Î»Òì³£
            {

                GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//Ë®Î»Òì³£
                GetSensor()->status[FLOW_INDEX] = 0;//Á÷Á¿Òì³£
                GetSensor()->status[NOWATER_INDEX] = 0;//È±Ë®
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                GetSensor()->status[TDS2_INDEX] = 0;//
                GetSensor()->status[TDS1_INDEX] = 0;//

                if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
                {

                    water_levelAbnormal_proc();

                }
<<<<<<< HEAD
                else //æ°´ä½å¼€å…³å¼‚å¸¸
                {
                    EleSwCtrl(SALT_SW,OFF);//å…³é˜€2
                }

            }//end æ°´ä½å¼‚å¸¸
#if WATER_L_IGNORE == 1
            else//æ°´ä½æ­£å¸¸ï¼Œå…¶ä»–å‚æ•°æ£€æµ‹æ‰æœ‰æ„ä¹‰
            {
                EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å…³
                GetSensor()->water_status = 0;//æ°´ä½å¼‚å¸¸é‡æ–°åŠ æ°´åŠ æ»¡çŠ¶æ€æ ‡å¿—ä½
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//æ°´ä½æ— å¼‚å¸¸
=======
                else //Ë®Î»¿ª¹ØÒì³£
                {
                    EleSwCtrl(SALT_SW,OFF);//¹Ø·§2
                }

            }//end Ë®Î»Òì³£
#if WATER_L_IGNORE == 1
            else//Ë®Î»Õý³££¬ÆäËû²ÎÊý¼ì²â²ÅÓÐÒâÒå
            {
                EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¹Ø
                GetSensor()->water_status = 0;//Ë®Î»Òì³£ÖØÐÂ¼ÓË®¼ÓÂú×´Ì¬±êÖ¾Î»
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//Ë®Î»ÎÞÒì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            }
#endif

#if WATER_L_IGNORE == 0
            else
#endif
            {

<<<<<<< HEAD
                EleSwCtrl(WATER_SW,ON);//å¼€é˜€1
#if WATER_L_IGNORE == 0
                EleSwCtrl(SALT_SW,OFF);//å¼€é˜€2
                GetSensor()->water_status = 0;//æ°´ä½å¼‚å¸¸é‡æ–°åŠ æ°´åŠ æ»¡çŠ¶æ€æ ‡å¿—ä½
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//æ°´ä½æ— å¼‚å¸¸

#endif

                EleSwCtrl(WASTE_SW,ON);//å¼€é˜€3
                EleSwCtrl(WASH_SW,ON);//å¼€é˜€4
                EleSwCtrl(HCILO_SW,OFF);//å¼€é˜€5
                GetSensor()->status[TDS1_INDEX] = 0;//tds1å¼‚å¸¸
                GetSensor()->status[TDS2_INDEX] = 0;//
                if(GetSensor()->flow != 0)//æµé‡å¼‚å¸¸
=======
                EleSwCtrl(WATER_SW,ON);//¿ª·§1
#if WATER_L_IGNORE == 0
                EleSwCtrl(SALT_SW,OFF);//¿ª·§2
                GetSensor()->water_status = 0;//Ë®Î»Òì³£ÖØÐÂ¼ÓË®¼ÓÂú×´Ì¬±êÖ¾Î»
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//Ë®Î»ÎÞÒì³£

#endif

                EleSwCtrl(WASTE_SW,ON);//¿ª·§3
                EleSwCtrl(WASH_SW,ON);//¿ª·§4
                EleSwCtrl(HCILO_SW,OFF);//¿ª·§5
                GetSensor()->status[TDS1_INDEX] = 0;//tds1Òì³£
                GetSensor()->status[TDS2_INDEX] = 0;//
                if(GetSensor()->flow != 0)//Á÷Á¿Òì³£
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {
                    Flow_Init();
                    FlowCtrl();

                }
                else
                {
                    pid_init2(dstTds);
<<<<<<< HEAD
                    DcMotorCtrl(2,0);//æ³µ2è°ƒæ•´æµé‡
=======
                    DcMotorCtrl(2,0);//±Ã2µ÷ÕûÁ÷Á¿
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                }

                registerTick(WASH_TICK, WASH_TIME_SETTING,1, 0);
                if( GetTickResult(WASH_TICK)==1)
                {
                    *getpumpstatus()=0;
                    registerTick(PUMP_TICK_NO, 0,0, 1);
                    GetInOut()->key_wash_mode = 0;
                    registerTick(WASH_TICK, 0,0,1);
                    GetSensor()->status[WASH_INDEX] = 0;//washing
<<<<<<< HEAD
                    EleSwCtrl(WASTE_SW,OFF);//å¼€é˜€3
                    //DcMotorCtrl(2,0);//å…³æ³µ2
=======
                    EleSwCtrl(WASTE_SW,OFF);//¿ª·§3
                    //DcMotorCtrl(2,0);//¹Ø±Ã2
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643


                }

            }

        }
        else
            GetSensor()->status[WASH_INDEX] = 0;

    }

    else
        ele_dev_proc();

<<<<<<< HEAD
    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>=MIN_FLOW_SIZE&&  //åŠ é€Ÿå¯åŠ¨ç”µè§£é€Ÿåº¦
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//é«˜åŽ‹å¼€å…³å’Œæ°´ä½æ­£å¸¸ä¸”ä¸åœ¨æ ¡å‡†æ¨¡å¼æ—¶å¯åŠ¨ç”µè§£
=======
    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>=MIN_FLOW_SIZE&&  //¼ÓËÙÆô¶¯µç½âËÙ¶È
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//¸ßÑ¹¿ª¹ØºÍË®Î»Õý³£ÇÒ²»ÔÚÐ£×¼Ä£Ê½Ê±Æô¶¯µç½â
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        if(state_flag == 0)
        {
            state_flag = 1;
            setState(4, M1_ADDR);
            setTickResult(SENSOR_TICK_NO,1);

        }
    }
    else
    {
        state_flag = 0;
    }
<<<<<<< HEAD
    for(i=1; i<WORK_STATUS_SIZE; i++)    //æ•…éšœçŠ¶æ€å¤„ç†
=======
    for(i=1; i<WORK_STATUS_SIZE; i++)    //¹ÊÕÏ×´Ì¬´¦Àí
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        //GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;

        if(GetSensor()->status[i]!=0)
        {
#if WATER_L_IGNORE == 0
            if(i!=ORP_INDEX&&i!=PH_INDEX)
#else
            if(i!=ORP_INDEX&&i!=PH_INDEX)
#endif

            {
                GetSensor()->status[NORMAL_INDEX] = 0;
                if(i==NORMAL_INDEX||i==WASH_INDEX)
                {
                    shunt_flag=0;
                    //GetSensor()->status[NORMAL_INDEX] = 20;
                    GetSensor()->status[SHUNT_INDEX] = 0;
                    GetSensor()->wash_time =0;
                }
                // if(i!=WATER_LEVEL_INDEX&&i!=WASH_INDEX&&GetInOut()->key_cali_mode == 0)
                //   beep_pro(6,0);
                break;
            }

        }
        else
        {
            if(i>=(WORK_STATUS_SIZE-1))
            {
                GetSensor()->status[NORMAL_INDEX] = 20;

                //beep_pro(3,1);

            }


        }

    }
<<<<<<< HEAD
    /*****************************************é£Žæ‰‡æŽ§åˆ¶***********************************************/
=======
    /*****************************************·çÉÈ¿ØÖÆ***********************************************/
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    if(GetSensor()->flow>=MIN_FLOW_SIZE)
    {
        if(GetSensor()->status[WASH_INDEX]||GetSensor()->status[NORMAL_INDEX] == 20)
            DcMotorCtrl(1, 50000);
        if((HAL_GetTick()-params_tick)>=60000)
        {
            SystemParamsSave();
            //GetSensor()->water_quantity=GetSensor()->water_quantity+0.15;
            //GetSensor()->sum_ele_time= GetSensor()->sum_ele_time+0.1;
            params_tick =  HAL_GetTick();
        }
    }
    else
    {
        params_tick =  HAL_GetTick();
        if(GetSensor()->status[WATER_LEVEL_INDEX]!=WATER_LEVEL_INDEX)
            DcMotorCtrl(1, 0);

    }


}
/*void work_process()
{
    if(work_params.work_mode == ON)
    {

        //delay_ms(100);
        //setAddr_ORP_PH(0x02);
        //	recovery_ORP_PH(ORP_ADDR);
        //recovery_ORP_PH(PH_ADDR);
        //DcMotorCtrl(1,65535);

        sensor_process();//
        ele_process();
        lcd_proc();

    }
}*/
void app_test()
{
<<<<<<< HEAD
    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
    EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    // EleSwCtrl(WASH_SW,ON);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    //EleSwCtrl(SALT_SW,ON);//ç›ç›’è¿›æ°´é˜€å¼€
    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å¼€
    //EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³


    // EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€

    EleSwCtrl(WATER_SW,ON);//åŽŸæ°´è¿›æ°´é˜€å¼€
    // EleSwCtrl(SALT_SW,ON);//ç›ç›’è¿›æ°´é˜€å¼€
    //  EleSwCtrl(WASTE_SW,ON);//åºŸæ°´å‡ºæ°´é˜€å¼€

    //  EleSwCtrl(WASH_SW,ON);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    // EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
    EleSwCtrl(HCILO_SW,ON);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    //  EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
    //  EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
//
//
//
//    EleSwCtrl(SALT_SW,OFF);//ç›ç›’è¿›æ°´é˜€å¼€
//    EleSwCtrl(WASTE_SW,OFF);//åºŸæ°´å‡ºæ°´é˜€å¼€
//    EleSwCtrl(WASH_SW,OFF);//æ¶ˆæ¯’æ°´æŽ’å‡ºåˆ°åºŸæ°´é˜€å…³
//    EleSwCtrl(HCILO_SW,OFF);//æ¶ˆæ¯’æ°´å‡ºæ°´é˜€å…³
//  EleSwCtrl(WATER_SW,OFF);//åŽŸæ°´è¿›æ°´é˜€å¼€
//    DcMotorCtrl(2,0);//å…³æ³µ2
=======
    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
    EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
    EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
    // EleSwCtrl(WASH_SW,ON);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    //EleSwCtrl(SALT_SW,ON);//ÑÎºÐ½øË®·§¿ª
    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¿ª
    //EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø


    // EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª

    EleSwCtrl(WATER_SW,ON);//Ô­Ë®½øË®·§¿ª
    // EleSwCtrl(SALT_SW,ON);//ÑÎºÐ½øË®·§¿ª
    //  EleSwCtrl(WASTE_SW,ON);//·ÏË®³öË®·§¿ª

    //  EleSwCtrl(WASH_SW,ON);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    // EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
    EleSwCtrl(HCILO_SW,ON);//Ïû¶¾Ë®³öË®·§¹Ø
    //  EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
    //  EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
//
//
//
//    EleSwCtrl(SALT_SW,OFF);//ÑÎºÐ½øË®·§¿ª
//    EleSwCtrl(WASTE_SW,OFF);//·ÏË®³öË®·§¿ª
//    EleSwCtrl(WASH_SW,OFF);//Ïû¶¾Ë®ÅÅ³öµ½·ÏË®·§¹Ø
//    EleSwCtrl(HCILO_SW,OFF);//Ïû¶¾Ë®³öË®·§¹Ø
//  EleSwCtrl(WATER_SW,OFF);//Ô­Ë®½øË®·§¿ª
//    DcMotorCtrl(2,0);//¹Ø±Ã2
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643


}


uint32_t tmp3;
void SystemParamsSave()
{
    uint32_t addr;
    uint32_t tmp;

    FlashEease(0);
    addr = 0;
<<<<<<< HEAD
    tmp = 0x5a; //é…ç½‘æ ‡å¿—ï¼Œä»£è¡¨å·²ç»é…è¿‡ç½‘ç»œ
=======
    tmp = 0x5a; //ÅäÍø±êÖ¾£¬´ú±íÒÑ¾­Åä¹ýÍøÂç
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    bsp_WriteCpuFlash(addr, &tmp, 1);
    addr = addr +4;
    //GetSensor()->water_quantity = 0;
    bsp_WriteCpuFlash(addr,(uint32_t *)(&GetSensor()->water_quantity), 1);
    addr = addr +4;
    //GetSensor()->sum_ele_time = 0.1111111;
    tmp3 = GetSensor()->sum_ele_time*10;
    GetSensor()->sum_ele_time = tmp3/10.0;
    bsp_WriteCpuFlash(addr,(uint32_t *)(&GetSensor()->sum_ele_time), 1);

}
void SystemParamsRead()
{
    uint32_t addr;
    uint32_t tmp;


    addr = 0;
    bsp_ReadCpuFlash(addr, &tmp, 1);
    if(tmp == 0x5a||tmp == 0x5b)
    {
        addr = addr +4;
        bsp_ReadCpuFlash(addr,(uint32_t *)(&GetSensor()->last_water_quantity), 1);
        addr = addr +4;
        bsp_ReadCpuFlash(addr,(uint32_t *)(&GetSensor()->last_sum_ele_time), 1);


    }
    else
    {
        SystemParamsSave();
    }


}
unsigned char factory_mode()
{
    unsigned char result ;
    static unsigned char flag;
    result = 1;
    if(GetTouchInOut()->factory_count>=8)
    {
        beep_pro(6,0);
        if(flag == 0)
        {
            flag = 1;
            while(get_reset_status()!=3)
<<<<<<< HEAD
                module_reset(1);//ä¸»æœºå’Œæ¨¡å—å‡å¤ä½
=======
                module_reset(1);//Ö÷»úºÍÄ£¿é¾ù¸´Î»
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

        //Soft_Reset();

        GetSensor()->status[FACTORY_INDEX] = FACTORY_INDEX;
<<<<<<< HEAD
        EleSwCtrl(6,OFF);//æ‰€æœ‰é˜€
        DcMotorCtrl(7,OFF);//æ‰€æœ‰ç”µæœº
=======
        EleSwCtrl(6,OFF);//ËùÓÐ·§
        DcMotorCtrl(7,OFF);//ËùÓÐµç»ú
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        //GetTouchInOut()->factory_count = 0;
        result = 0;
    }
    return result;

}
//static uint32_t pwm3=21000;
void app()
{
    static unsigned char flag;

    if(flag == 0)
    {
        GetSensor()->temperature = 6;
        flag = 1;
    }


    sensor_process();//
#if TDS_DEBUG == 0
    if(factory_mode() == 1)
        ele_process();
#endif
    lcd_proc();
    //pwm3=pwm3+100;
    //	if(pwm3>65536)
    //	pwm3 = 65535;
<<<<<<< HEAD
    //DcMotorCtrl(2,21000);//æ³µ2è°ƒæ•´æµé‡
=======
    //DcMotorCtrl(2,21000);//±Ã2µ÷ÕûÁ÷Á¿
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
//FlowCtrl();


}
