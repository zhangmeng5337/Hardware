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
//目标tds 2.5----500
//目标tds 3 ------650
//目标tds 3.5 ------950   +30----0.1
//目标tds 4 ------1100
unsigned char First_flag;

uint32_t dstTds=1100;//目标tds 2.5----500
=======
//Ŀ��tds 2.5----500
//Ŀ��tds 3 ------650
//Ŀ��tds 3.5 ------950   +30----0.1
//Ŀ��tds 4 ------1100
unsigned char First_flag;

uint32_t dstTds=1100;//Ŀ��tds 2.5----500
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
function:功能异常超时判断
=======
function:�����쳣��ʱ�ж�
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
params:
**********************************************************************/
unsigned char TickTimeoutAb(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
<<<<<<< HEAD
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//从正常转为异常
    {
        registerTick(TickNum, 0, 0,1);//定时器复位
        registerTick(TickNum, timeout, 1,0);//超时计时开始
=======
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//������תΪ�쳣
    {
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        registerTick(TickNum, timeout, 1,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==2)
        {
<<<<<<< HEAD
            registerTick(TickNum, 0, 0,1);//定时器复位
            registerTick(TickNum, timeout, 1,0);//超时计时开始
=======
            registerTick(TickNum, 0, 0,1);//��ʱ����λ
            registerTick(TickNum, timeout, 1,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==1)
    {
        // status = status | BitOper;
<<<<<<< HEAD
        registerTick(TickNum, 0, 0,1);//定时器复位
=======
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
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
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//正常
    {
        registerTick(TickNum, 0, 0,1);//定时器复位
        registerTick(TickNum, timeout, 2,0);//超时计时开始
=======
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//����
    {
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        registerTick(TickNum, timeout, 2,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        if(GetTickStatus(TickNum)==0)
            return BitOper;
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==1)
        {
<<<<<<< HEAD
            registerTick(TickNum, 0, 0,1);//定时器复位
            registerTick(TickNum, timeout, 2,0);//超时计时开始
=======
            registerTick(TickNum, 0, 0,1);//��ʱ����λ
            registerTick(TickNum, timeout, 2,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==2)
    {
<<<<<<< HEAD
        registerTick(TickNum, 0, 0,1);//定时器复位
=======
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
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
    if(work_params.init_flag == 0)//上电初始化，电解水从废水通道排出
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//时间到，走正常逻辑工作
=======
    if(work_params.init_flag == 0)//�ϵ��ʼ�������ˮ�ӷ�ˮͨ���ų�
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//ʱ�䵽���������߼�����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        registerTick(StART_TICK_NO, 0,0,1);
    }


    //********************************************************************************

<<<<<<< HEAD
    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//流量异常
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//水龙头开的情况下流量异常
=======
    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//�����쳣
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//ˮ��ͷ��������������쳣
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
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//原水tds异常
=======
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//ԭˮtds�쳣
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
                                            GetSensor()->status[TDS2_INDEX]==0))//tds异常
=======
                                            GetSensor()->status[TDS2_INDEX]==0))//tds�쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {


        status = status | TickTimeoutAb(TDS_TICK_NO,0x02,2*MAX_TICK);//
        if(status &0x02)
        {
<<<<<<< HEAD
            registerTick(TDS_TICK_NO, 0, 0,1);//定时器复位
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//超时计时
=======
            registerTick(TDS_TICK_NO, 0, 0,1);//��ʱ����λ
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//��ʱ��ʱ
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
    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//ph异常
=======
    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//ph�쳣
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
    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orp异常
=======
    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orp�쳣
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
            ||GetSensor()->water_level == WATER_M)//水位异常
=======
            ||GetSensor()->water_level == WATER_M)//ˮλ�쳣
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
    if(GetSensor()->swH ==ON)//高压开关异常
=======
    if(GetSensor()->swH ==ON)//��ѹ�����쳣
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
    if(GetSensor()->swH ==ON&&GetSensor()->flow<=0)//高压开关异常
=======
    if(GetSensor()->swH ==ON&&GetSensor()->flow<=0)//��ѹ�����쳣
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
功能：根据流量范围调整进水tds
=======
���ܣ�����������Χ������ˮtds
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
        //目标tds 2.5----500
        //目标tds 3 ------650
        //目标tds 3.5 ------950	+30----0.1
        //目标tds 4 ------1100
=======
        //Ŀ��tds 2.5----500
        //Ŀ��tds 3 ------650
        //Ŀ��tds 3.5 ------950	+30----0.1
        //Ŀ��tds 4 ------1100
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
        registerTick(PID_SETTING_TICK_NO, 10, 1,0);//超时计时开始
=======
        registerTick(PID_SETTING_TICK_NO, 10, 1,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        //  if(GetTickResult(PID_SETTING_TICK_NO)==0)
        //     return ;
        /* if(tds_out<=650)
         tds_out = tds_out + 5;
         if(tds_out>=680)
         	tds_out = tds_out -5;*/
        pwm2 = pid_proc(tds_out);
<<<<<<< HEAD
        registerTick(PID_SETTING_TICK_NO, 0, 0,1);//定时器复位
=======
        registerTick(PID_SETTING_TICK_NO, 0, 0,1);//��ʱ����λ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }
    if(PID_TYPE != 2)
    {
<<<<<<< HEAD
        registerTick(PID_SETTING_TICK_NO, 1000, 1,0);//超时计时开始
=======
        registerTick(PID_SETTING_TICK_NO, 1000, 1,0);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        if(GetTickResult==0)
            return ;

        {
            pid_cnt++;
            pid_cnt = 0;
            pwm2 = pid_proc(tds_out);
            tds_flag = 0;
<<<<<<< HEAD
            registerTick(PID_SETTING_TICK_NO, 0, 0,1);//定时器复位
=======
            registerTick(PID_SETTING_TICK_NO, 0, 0,1);//��ʱ����λ
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
        DcMotorCtrl(2,30000);//泵2调整流量

    else
        DcMotorCtrl(2,pwm2);//泵2调整流量
=======
        DcMotorCtrl(2,30000);//��2��������

    else
        DcMotorCtrl(2,pwm2);//��2��������
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
        str = "无";
=======
        str = "��";
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
            str = "离线";
=======
            str = "����";
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
    switch(status_tmp)// 0:正常  1：tds1 2:tds2 3：流量 4：orp
        //  5:高压开关6：水位开关 7:电解中
    {
    /*	str = "正常          ";
    	str = "原水TDS异常	 ";
    	str = "缺盐		 	 ";
    	str = "流量异常 	 ";
    	str = "盐水箱注水中";
    	str = "清洗中		 ";
    	str = "缺水			 ";
    	str = "系统故障 	 ";
    	str = "正常			 ";
    	str = "正常			 ";*/
=======
    switch(status_tmp)// 0:����  1��tds1 2:tds2 3������ 4��orp
        //  5:��ѹ����6��ˮλ���� 7:�����
    {
    /*	str = "����          ";
    	str = "ԭˮTDS�쳣	 ";
    	str = "ȱ��		 	 ";
    	str = "�����쳣 	 ";
    	str = "��ˮ��עˮ��";
    	str = "��ϴ��		 ";
    	str = "ȱˮ			 ";
    	str = "ϵͳ���� 	 ";
    	str = "����			 ";
    	str = "����			 ";*/
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643





    case 20:
<<<<<<< HEAD
        str = "正常";

        break;
    case TDS1_INDEX:
        str = "原水TDS异常";
=======
        str = "����";

        break;
    case TDS1_INDEX:
        str = "ԭˮTDS�쳣";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;
    case TDS2_INDEX:
    case SHUNT_INDEX:
        // if(GetSensor()->tds2<dstTds)
<<<<<<< HEAD
        str = "缺盐";

        break;
    case FLOW_INDEX:
        str = "流量异常";
=======
        str = "ȱ��";

        break;
    case FLOW_INDEX:
        str = "�����쳣";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;

    case WATER_LEVEL_INDEX:
<<<<<<< HEAD
        str = "盐水箱注水中";
        break;

    case WASH_INDEX:
        str = "清洗中";
=======
        str = "��ˮ��עˮ��";
        break;

    case WASH_INDEX:
        str = "��ϴ��";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;

    case NOWATER_INDEX:
<<<<<<< HEAD
        str = "缺水";
        break;
    case SYSTEM_INDEX:
        str = "系统故障";
        break;
    case FACTORY_INDEX:
        str = "正常";
=======
        str = "ȱˮ";
        break;
    case SYSTEM_INDEX:
        str = "ϵͳ����";
        break;
    case FACTORY_INDEX:
        str = "����";
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        break;
    default:

<<<<<<< HEAD
        str = "正常";
        break;
    }
    //str = "进水TDS异常";
=======
        str = "����";
        break;
    }
    //str = "��ˮTDS�쳣";
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
    str = "正常            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "原水TDS异常     ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "缺盐            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "流量异常        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "盐水箱注水中";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "清洗中          ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "缺水            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "系统故障        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "正常            ";
=======
    str = "����            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ԭˮTDS�쳣     ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ȱ��            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "�����쳣        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "��ˮ��עˮ��";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "��ϴ��          ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ȱˮ            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "ϵͳ����        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "����            ";
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
        printf("消毒水总量及电解时间：	%0.2f(升)	   %0.1f(小时)\n",GetSensor()->water_quantity,GetSensor()->sum_ele_time);


        //GetSensor()->water_quantity = GetSensor()->water_quantity + 0.2;
        if(getTouch()->next_page == Main_PAGE)//主页面
=======
        printf("����ˮ���������ʱ�䣺	%0.2f(��)	   %0.1f(Сʱ)\n",GetSensor()->water_quantity,GetSensor()->sum_ele_time);


        //GetSensor()->water_quantity = GetSensor()->water_quantity + 0.2;
        if(getTouch()->next_page == Main_PAGE)//��ҳ��
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
        else if(getTouch()->next_page == CALIBRATION_PAGE)//校准页面
        {
            //SetPage(CALIBRATION_PAGE);//主页面Id号是4
=======
        else if(getTouch()->next_page == CALIBRATION_PAGE)//У׼ҳ��
        {
            //SetPage(CALIBRATION_PAGE);//��ҳ��Id����4
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
        else  if(getTouch()->next_page == SETTING_PAGE)//设置页面
=======
        else  if(getTouch()->next_page == SETTING_PAGE)//����ҳ��
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {
            SetPage(SETTING_PAGE);//
        }
#endif
<<<<<<< HEAD
        else  if(getTouch()->next_page == REBOOT_PAGE)//重启页面
        {
            SetPage(REBOOT_PAGE);//主页面Id号是4
=======
        else  if(getTouch()->next_page == REBOOT_PAGE)//����ҳ��
        {
            SetPage(REBOOT_PAGE);//��ҳ��Id����4
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }
#if DEV_TYPE == 1

        else
        {
            if(getTouch()->next_page != KEYBOARD_PAGE)
            {
<<<<<<< HEAD
                SetPage(Main_PAGE);//主页面Id号是4
=======
                SetPage(Main_PAGE);//��ҳ��Id����4
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
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//复位
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//停止电解
=======
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//��λ
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//ֹͣ���
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
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#endif
    EleSwCtrl(SALT_SW,ON);//盐盒进水阀开

#if WATER_L_IGNORE == 0
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    DcMotorCtrl(2,0);//关泵2
=======
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#endif
    EleSwCtrl(SALT_SW,ON);//�κн�ˮ����

#if WATER_L_IGNORE == 0
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    DcMotorCtrl(2,0);//�ر�2
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
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀开
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
=======
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
#endif
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    // delay_ms(500);

}
void dev_flow_Abnormal_ctrl()
{
#if FLOW_FAULT_ENABLE ==1
    if(abnormalDec()&0x01)
    {

<<<<<<< HEAD
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//缺水
=======
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ȱˮ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }

#endif

<<<<<<< HEAD
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    //DcMotorCtrl(1,OFF);//关所有电机
    DcMotorCtrl(2,OFF);//关所有电机
=======
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
#endif
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
    //DcMotorCtrl(1,OFF);//�����е��
    DcMotorCtrl(2,OFF);//�����е��
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
}
void dev_normal_ctrl()
{
    GetSensor()->status[FLOW_INDEX] = 0;//
<<<<<<< HEAD
    GetSensor()->status[NORMAL_INDEX] = 20;//无异常
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    if(GetSensor()->flow >0)
        EleSwCtrl(WASTE_SW,ON);//废水出水阀开
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀
=======
    GetSensor()->status[NORMAL_INDEX] = 20;//���쳣
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
#endif
    if(GetSensor()->flow >0)
        EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ��
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    // FlowCtrl();
    // delay_ms(500);

}

void dev_wash_ctrl()
{
<<<<<<< HEAD
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//大于3次停机
    GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2异常
    GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//停机

    GetSensor()->err_flag = 0;
    // GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
    //GetSensor()->status[ORP_INDEX] = 0;//orp异常
    // GetSensor()->status[PH_INDEX] = 0;//ph异常
    EleSwCtrl(WATER_SW,OFF);//原水进水阀开
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif


    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀
=======
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//����3��ͣ��
    GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2�쳣
    GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//ͣ��

    GetSensor()->err_flag = 0;
    // GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
    //GetSensor()->status[ORP_INDEX] = 0;//orp�쳣
    // GetSensor()->status[PH_INDEX] = 0;//ph�쳣
    EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
#endif


    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ��
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
    EleSwCtrl(WATER_SW,OFF);//原水进水阀开
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    DcMotorCtrl(7,OFF);//关所有电机
=======
    EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    DcMotorCtrl(7,OFF);//�����е��
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
    registerTick(PUMP_TICK_NO, 18000,1, 0);//没水吸3分钟
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
            pump_result = 1;//自吸没水
        }
        else
            pump_result = 2; //自吸有水
=======
            pump_result = 1;//����ûˮ
        }
        else
            pump_result = 2; //������ˮ
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
        //自吸模式
=======
        //����ģʽ
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
                    pump_ctrl(1);//没水泵工作30s
=======
                    pump_ctrl(1);//ûˮ�ù���30s
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
    registerTick(PUMP_TICK_NO, 18000,1, 0);//没水吸3分钟
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
        registerTick(PUMP_TICK_NO, 0,0, 1);//没水吸3分钟
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
    if(tmp==0&&GetSensor()->temperature>=5)//高压阀异常,低温不工作
    {

        if(GetSensor()->flow<FLOW_VALUE)	//水流量不足
        {

            flow_low_cnt++;
            if(flow_low_cnt>=400000)
=======
    if(tmp==0&&GetSensor()->temperature>=5)//��ѹ���쳣,���²�����
    {

        if(GetSensor()->flow<FLOW_VALUE||GetSensor()->swH == OFF)	//ˮ��������
        {

            flow_low_cnt++;
            if(flow_low_cnt>=400000||GetSensor()->swH == OFF)
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            {
                flow_low_cnt = 0;
#if FLOW_FAULT_ENABLE ==1
                if(GetSensor()->flow==0)
<<<<<<< HEAD
                    GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
                else
                    GetSensor()->status[FLOW_INDEX] = 0;//流量异常
#endif
                if(GetSensor()->flow<=MIN_FLOW_SIZE&&GetSensor()->status[TDS2_INDEX] ==0&&GetSensor()->status[WATER_LEVEL_INDEX]==0)
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//缺水
=======
                    GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
                else
                    GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
#endif
                if(GetSensor()->flow<=MIN_FLOW_SIZE&&GetSensor()->status[TDS2_INDEX] ==0&&GetSensor()->status[WATER_LEVEL_INDEX]==0)
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ȱˮ
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
                GetSensor()->status[NOWATER_INDEX] = 0;//缺水
                GetSensor()->status[FLOW_INDEX] = 0;//流量异常
=======
                GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
                GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
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
        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1异常
    }
    else
    {
        GetSensor()->status[TDS1_INDEX] = 0;//tds1异常


    }//tds2异常
    if(GetSensor()->err_flag&0x01)//tds报警
    {
        dev_wash_ctrl();

    }//tds报警,自动清洗
    else
    {
        //GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
        EleSwCtrl(WATER_SW,ON);//原水进水阀开
        if(GetSensor()->flow >0)
            EleSwCtrl(WASTE_SW,ON);//废水出水阀开
        EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
        EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
=======
        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1�쳣
    }
    else
    {
        GetSensor()->status[TDS1_INDEX] = 0;//tds1�쳣


    }//tds2�쳣
    if(GetSensor()->err_flag&0x01)//tds����
    {
        dev_wash_ctrl();

    }//tds����,�Զ���ϴ
    else
    {
        //GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
        EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
        if(GetSensor()->flow >0)
            EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
        EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
        EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
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
        if(GetTickResult(HW_TICK)==1)//增压泵工作，驱动高压开关
=======
        if(GetTickResult(HW_TICK)==1)//��ѹ�ù�����������ѹ����
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
    // EleSwCtrl(WATER_SW,OFF);//原水进水阀开
    //EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    DcMotorCtrl(7,0);//关所有电机
    //pump_ctrl(1);
    //pump_ctrl(0);
    flow_low_cnt = 0;
    GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
    GetSensor()->status[FLOW_INDEX] = 0;//流量异常
    GetSensor()->status[NOWATER_INDEX] = 0;//缺水
    GetSensor()->status[TDS2_INDEX] = 0;//缺水
    GetSensor()->status[TDS1_INDEX] = 0;//缺水

    // EleSwCtrl(6,OFF);//关所有阀
    if(abnormalDec()&0x20)//水位异常
=======
    // EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
    //EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    DcMotorCtrl(7,0);//�����е��
    //pump_ctrl(1);
    //pump_ctrl(0);
    flow_low_cnt = 0;
    GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
    GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
    GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
    GetSensor()->status[TDS2_INDEX] = 0;//ȱˮ
    GetSensor()->status[TDS1_INDEX] = 0;//ȱˮ

    // EleSwCtrl(6,OFF);//�����з�
    if(abnormalDec()&0x20)//ˮλ�쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {


        if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
        {

            water_levelAbnormal_proc();


        }


<<<<<<< HEAD
    }//end 水位异常
    else //水位正常
    {
        GetSensor()->status[WATER_LEVEL_INDEX] = 0;//高压开关异常
#if SW_NOCHANGE
        //EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
        EleSwCtrl(WATER_SW,OFF);//原水进水阀开
#else
        EleSwCtrl(WATER_SW,ON);//原水进水阀开
#endif

        EleSwCtrl(SALT_SW,OFF);//关阀2
=======
    }//end ˮλ�쳣
    else //ˮλ����
    {
        GetSensor()->status[WATER_LEVEL_INDEX] = 0;//��ѹ�����쳣
#if SW_NOCHANGE
        //EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
        EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
#else
        EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#endif

        EleSwCtrl(SALT_SW,OFF);//�ط�2
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }

    {

        // #if SW_NOCHANGE
<<<<<<< HEAD
        //EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
        // EleSwCtrl(WATER_SW,OFF);//原水进水阀开
        //  #else
        ;//EleSwCtrl(WATER_SW,OFF);//原水进水阀开
        // #endif
        EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
        EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
        EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
=======
        //EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
        // EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
        //  #else
        ;//EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
        // #endif
        EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
        EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
        EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
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
    GetSensor()->status[HSW_INDEX] = 5;//高压开关异常
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//流量异常
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//流量正常
            GetSensor()->status[NOWATER_INDEX] = 0;//流量正常
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//流量正常
#if FLOW_FAULT_ENABLE == 1
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
=======
    GetSensor()->status[HSW_INDEX] = 5;//��ѹ�����쳣
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//�����쳣
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//��������
            GetSensor()->status[NOWATER_INDEX] = 0;//��������
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//��������
#if FLOW_FAULT_ENABLE == 1
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#endif
        }
        registerTick(FLOW_TICK_NO2, 0,0,1);//

    }
    else
    {
<<<<<<< HEAD
        GetSensor()->status[FLOW_INDEX] = 0;//流量正常
        GetSensor()->status[NOWATER_INDEX] = 0;//流量正常
=======
        GetSensor()->status[FLOW_INDEX] = 0;//��������
        GetSensor()->status[NOWATER_INDEX] = 0;//��������
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

    }

    if(GetSensor()->status[FLOW_INDEX])
<<<<<<< HEAD
        GetSensor()->status[HSW_INDEX] =    5;//高压开关异常
=======
        GetSensor()->status[HSW_INDEX] =    5;//��ѹ�����쳣
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
    registerTick(FLOW_TICK_NO, 0, 0,1);//超时计时开始
    registerTick(PUMP_TICK_NO, 0,0,1);//没水吸3分钟
=======
    registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ��ʱ��ʼ
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
        registerTick(FLOW_TICK_NO, 0, 0,1);//超时计时开始
=======
        registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ��ʱ��ʼ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    }
    if(GetSensor()->flow > 0)
    {

        registerTick(HW_TICK, 0, 0,1);//
    }


    registerTick(FLOW_TICK_NO2, 0,0,1);
<<<<<<< HEAD
    if(abnormalDec()&0x10==0)//高压开关异常
        GetSensor()->status[HSW_INDEX] = 0;//高压开关异常

    if(GetSensor()->status[SHUNT_INDEX] == 0)//非停机模式
    {
        //if(if(GetSensor()->flow <=0))
        //自吸模式
        pump_auto(0);
        flow_proc();  //2022.10.4
        //else

        if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//水位异常
=======
    if(abnormalDec()&0x10==0)//��ѹ�����쳣
        GetSensor()->status[HSW_INDEX] = 0;//��ѹ�����쳣

    if(GetSensor()->status[SHUNT_INDEX] == 0)//��ͣ��ģʽ
    {
        //if(if(GetSensor()->flow <=0))
        //����ģʽ
        pump_auto(0);
        //else

        if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//ˮλ�쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {

            //pump_ctrl(1);
            // pump_ctrl(0);
            flow_low_cnt = 0;
<<<<<<< HEAD
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
            GetSensor()->status[FLOW_INDEX] = 0;//流量异常
            GetSensor()->status[NOWATER_INDEX] = 0;//缺水
            GetSensor()->status[TDS2_INDEX] = 0;//缺水
            GetSensor()->status[TDS1_INDEX] = 0;//缺水
=======
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
            GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
            GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
            GetSensor()->status[TDS2_INDEX] = 0;//ȱˮ
            GetSensor()->status[TDS1_INDEX] = 0;//ȱˮ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {

                water_levelAbnormal_proc();



            }
<<<<<<< HEAD
            else //水位开关异常
            {
                EleSwCtrl(SALT_SW,OFF);//关阀2
            }

        }//end 水位异常
#if WATER_L_IGNORE == 1
        else//水位正常，其他参数检测才有意义
        {
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
=======
            else //ˮλ�����쳣
            {
                EleSwCtrl(SALT_SW,OFF);//�ط�2
            }

        }//end ˮλ�쳣
#if WATER_L_IGNORE == 1
        else//ˮλ������������������������
        {
            EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
            GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }
#endif


#if WATER_L_IGNORE == 0
<<<<<<< HEAD
        else//水位正常，其他参数检测才有意义
=======
        else//ˮλ������������������������
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#endif
        {




#if WATER_L_IGNORE == 0

<<<<<<< HEAD
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
#endif


            if(work_params.init_flag == 0)//上电开机
=======
            EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
            GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣
#endif


            if(work_params.init_flag == 0)//�ϵ翪��
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            {
                dev_init_ctrl();
                if(GetSensor()->flow > 0.1)
                {
<<<<<<< HEAD
                    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//流量异常
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//定时器复位
=======
                    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//�����쳣
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
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
                    if(GetSensor()->flow != 0)//流量异常
                        FlowCtrl();
                    else
                    {
                        DcMotorCtrl(2,0);//泵2调整流量
=======
                    if(GetSensor()->flow != 0)//�����쳣
                        FlowCtrl();
                    else
                    {
                        DcMotorCtrl(2,0);//��2��������
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                        pid_init2(dstTds);

                    }


                }

            }
<<<<<<< HEAD
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//流量异常
=======
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//�����쳣
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
                    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                }

            }
            else  //流量正常
            {
                // pump_hw(1);
                if(GetSensor()->flow != 0)//流量异常
=======
                    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                }

            }
            else  //��������
            {
                // pump_hw(1);
                if(GetSensor()->flow != 0)//�����쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                    FlowCtrl();
                else
                {
                    // Flow_Init() ;
                    pid_init2(dstTds);
<<<<<<< HEAD
                    DcMotorCtrl(2,0);//泵2调整流量
                    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
=======
                    DcMotorCtrl(2,0);//��2��������
                    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

                }


<<<<<<< HEAD
                //GetSensor()->status[NOWATER_INDEX] = 0;//流量异常
                // GetSensor()->status[NOWATER_INDEX] = 0;//缺水
                if((abnormalDec()&0x73)==0)//无异常，走正常逻辑 0x73 0x33
=======
                //GetSensor()->status[NOWATER_INDEX] = 0;//�����쳣
                // GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
                if((abnormalDec()&0x73)==0)//���쳣���������߼� 0x73 0x33
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {

                    dev_normal_ctrl();

                }
<<<<<<< HEAD
                else //参数有异常进行调节控制
=======
                else //�������쳣���е��ڿ���
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {

                    tds_proc();


<<<<<<< HEAD
                }//参数有异常进行调节控制


            } //流量正常
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //水流量不足


            //flow_proc();  //2022.10.4


        }// end 水位正常，其他参数检测才有意义

    }//非停机模式
    else //停机模式
    {
        if(abnormalDec()&0x20)//水位异常
=======
                }//�������쳣���е��ڿ���


            } //��������
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //ˮ��������


            flow_proc();


        }// end ˮλ������������������������

    }//��ͣ��ģʽ
    else //ͣ��ģʽ
    {
        if(abnormalDec()&0x20)//ˮλ�쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {


            //pump_ctrl(0);
            flow_low_cnt = 0;
<<<<<<< HEAD
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
            GetSensor()->status[FLOW_INDEX] = 0;//流量异常
            GetSensor()->status[NOWATER_INDEX] = 0;//缺水
            GetSensor()->status[TDS2_INDEX] = 0;//缺水
            GetSensor()->status[TDS1_INDEX] = 0;//缺水
=======
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
            GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
            GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
            GetSensor()->status[TDS2_INDEX] = 0;//ȱˮ
            GetSensor()->status[TDS1_INDEX] = 0;//ȱˮ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {

                water_levelAbnormal_proc();



            }
<<<<<<< HEAD
            else //水位开关异常
            {
                EleSwCtrl(SALT_SW,OFF);//关阀2
            }


        }////end 水位异
#if WATER_L_IGNORE == 1
        else//水位正常，其他参数检测才有意义
        {
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
=======
            else //ˮλ�����쳣
            {
                EleSwCtrl(SALT_SW,OFF);//�ط�2
            }


        }////end ˮλ��
#if WATER_L_IGNORE == 1
        else//ˮλ������������������������
        {
            EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
            GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }
#endif

        if(abnormalDec()&0x40)
        {
<<<<<<< HEAD
            GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1异常
        }
        else
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds1异常
=======
            GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1�쳣
        }
        else
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds1�쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            /* if(abnormalDec()&0x02==0&&GetSensor()->flow>0)
             {
                 if((GetSensor()->tds2-dstTds)>=150)
                 {
                     GetSensor()->err_flag = 0;
<<<<<<< HEAD
                     GetSensor()->wash_time =0;//GetSensor()->wash_time +1 ;//大于3次停机
                     GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
                     GetSensor()->status[SHUNT_INDEX] = 0;//tds2异常
                     EleSwCtrl(WATER_SW,ON);//原水进水阀开
                     EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                     EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
                     EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
=======
                     GetSensor()->wash_time =0;//GetSensor()->wash_time +1 ;//����3��ͣ��
                     GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
                     GetSensor()->status[SHUNT_INDEX] = 0;//tds2�쳣
                     EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
                     EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                     EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
                     EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

                 }

             }*/

<<<<<<< HEAD
            GetSensor()->status[TDS1_INDEX] = 0;//tds2无异常
            GetSensor()->status[TDS2_INDEX] = 0;//tds2无异常

            GetSensor()->status[ORP_INDEX] = 0;//tds2无异常
            GetSensor()->status[PH_INDEX] = 0;//tds2无异常
            GetSensor()->wash_time =0;
        }
        flow_proc();
    }//停机模式

}
/*************************************************************************
启动逻辑：高压阀正常--->水位正常--->电解流程
          高压阀异常--->停机
          水位异常-->停机
=======
            GetSensor()->status[TDS1_INDEX] = 0;//tds2���쳣
            GetSensor()->status[TDS2_INDEX] = 0;//tds2���쳣

            GetSensor()->status[ORP_INDEX] = 0;//tds2���쳣
            GetSensor()->status[PH_INDEX] = 0;//tds2���쳣
            GetSensor()->wash_time =0;
        }
        flow_proc();
    }//ͣ��ģʽ

}
/*************************************************************************
�����߼�����ѹ������--->ˮλ����--->�������
          ��ѹ���쳣--->ͣ��
          ˮλ�쳣-->ͣ��
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
***************************************************************************/
void ele_dev_proc()
{

    static unsigned char retry_flag=0;


    if(GetSensor()->wash_time>=3||GetSensor()->status[TDS1_INDEX]==TDS1_INDEX||GetSensor()->status[SYSTEM_INDEX]==SYSTEM_INDEX||GetSensor()->status[NOWATER_INDEX]==NOWATER_INDEX||
<<<<<<< HEAD
            GetSensor()->status[TDS2_INDEX] == TDS2_INDEX||GetSensor()->status[SHUNT_INDEX] == SHUNT_INDEX)//有故障状态
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
                EleSwCtrl(SALT_SW,OFF);//关所有阀

                //#if SW_NOCHANGE
                //EleSwCtrl(WATER_SW,OFF);//关所有阀
=======
                EleSwCtrl(SALT_SW,OFF);//�����з�

                //#if SW_NOCHANGE
                //EleSwCtrl(WATER_SW,OFF);//�����з�
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                //#endif

            }
            if(GetSensor()->status[NOWATER_INDEX] != NOWATER_INDEX)
            {
                GetSensor()->status[NORMAL_INDEX] = 0;//
                GetSensor()->status[WASH_INDEX] = 0;//
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//
<<<<<<< HEAD
                // EleSwCtrl(SALT_SW,OFF);//关阀2
                //EleSwCtrl(6,OFF);//关所有阀
                DcMotorCtrl(7,OFF);//关所有电机

                // EleSwCtrl(WATER_SW,OFF);//关所有阀
#if SW_NOCHANGE
                //if((GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M))

                //EleSwCtrl(WATER_SW,ON);//关所有阀
                EleSwCtrl(WATER_SW,OFF);//关所有阀
#else
                EleSwCtrl(WATER_SW,OFF);//关所有阀

                //EleSwCtrl(SALT_SW,OFF);//关所有阀
#endif
                EleSwCtrl(WASTE_SW,OFF);//关所有阀
                EleSwCtrl(WASH_SW,OFF);//关所有阀
                EleSwCtrl(HCILO_SW,OFF);//关所有阀
=======
                // EleSwCtrl(SALT_SW,OFF);//�ط�2
                //EleSwCtrl(6,OFF);//�����з�
                DcMotorCtrl(7,OFF);//�����е��

                // EleSwCtrl(WATER_SW,OFF);//�����з�
#if SW_NOCHANGE
                //if((GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M))

                //EleSwCtrl(WATER_SW,ON);//�����з�
                EleSwCtrl(WATER_SW,OFF);//�����з�
#else
                EleSwCtrl(WATER_SW,OFF);//�����з�

                //EleSwCtrl(SALT_SW,OFF);//�����з�
#endif
                EleSwCtrl(WASTE_SW,OFF);//�����з�
                EleSwCtrl(WASH_SW,OFF);//�����з�
                EleSwCtrl(HCILO_SW,OFF);//�����з�
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            }
            else
            {
<<<<<<< HEAD
                EleSwCtrl(WATER_SW,ON);//原水进水阀开
                DcMotorCtrl(7,OFF);//关所有电机
                EleSwCtrl(WASTE_SW,OFF);//关所有阀
                EleSwCtrl(WASH_SW,OFF);//关所有阀
                EleSwCtrl(HCILO_SW,OFF);//关所有阀
=======
                EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
                DcMotorCtrl(7,OFF);//�����е��
                EleSwCtrl(WASTE_SW,OFF);//�����з�
                EleSwCtrl(WASH_SW,OFF);//�����з�
                EleSwCtrl(HCILO_SW,OFF);//�����з�
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

            }

        }
        else
        {
<<<<<<< HEAD
            EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
            EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀开
            EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
=======
            EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
#if WATER_L_IGNORE == 0
            EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
#endif
            EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
            EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643

        }


        //delay_ms(500);
    }
    else
    {
        retry_flag = 0;
<<<<<<< HEAD
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//高压阀异常,低温不工作
=======
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//��ѹ���쳣,���²�����
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        {

            hsw_proc();
            registerTick(SW_ON_TICK, 0,0, 1);
            First_flag = 0;
        }
<<<<<<< HEAD
        else //正常工作模式
=======
        else //��������ģʽ
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
    else //水位开关异常
    {
        EleSwCtrl(SALT_SW,OFF);//关阀2
    }


    if(GetInOut()->key_reset_mode )//复位重启
    {
        EleSwCtrl(6,OFF);//关所有阀
        DcMotorCtrl(7,OFF);//关所有电机
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//主机和模块均复位
        Soft_Reset();
    }

    if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)//设置页面关闭所有阀门
=======
    else //ˮλ�����쳣
    {
        EleSwCtrl(SALT_SW,OFF);//�ط�2
    }


    if(GetInOut()->key_reset_mode )//��λ����
    {
        EleSwCtrl(6,OFF);//�����з�
        DcMotorCtrl(7,OFF);//�����е��
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//������ģ�����λ
        Soft_Reset();
    }

    if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        GetInOut()->key_wash_mode = 0;
        // registerTick(WASH_TICK, 0,0,1);
        //GetSensor()->status[WASH_INDEX] = 0;
<<<<<<< HEAD
        EleSwCtrl(6,OFF);//关所有阀
        DcMotorCtrl(7,OFF);//关所有电机
        registerTick(FLOW_TICK_NO, 0, 0,1);//定时器复位
        //delay_ms(500);

    }
    else if(GetInOut()->key_wash_mode)//冲洗状态
=======
        EleSwCtrl(6,OFF);//�����з�
        DcMotorCtrl(7,OFF);//�����е��
        registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
        //delay_ms(500);

    }
    else if(GetInOut()->key_wash_mode)//��ϴ״̬
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
    {
        if( GetSensor()->status[SYSTEM_INDEX]!=SYSTEM_INDEX&& GetSensor()->status[TDS2_INDEX]!=TDS2_INDEX&&
                GetSensor()->status[SHUNT_INDEX]!=SHUNT_INDEX)
        {
            //pump_ctrl(0);
            pump_auto(0);
<<<<<<< HEAD
            flow_proc();
            if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//水位异常
            {

                GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
                GetSensor()->status[FLOW_INDEX] = 0;//流量异常
                GetSensor()->status[NOWATER_INDEX] = 0;//缺水
=======
            if(abnormalDec()&0x20&&GetSensor()->status[TDS2_INDEX]==0)//ˮλ�쳣
            {

                GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
                GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
                GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                GetSensor()->status[TDS2_INDEX] = 0;//
                GetSensor()->status[TDS1_INDEX] = 0;//

                if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
                {

                    water_levelAbnormal_proc();

                }
<<<<<<< HEAD
                else //水位开关异常
                {
                    EleSwCtrl(SALT_SW,OFF);//关阀2
                }

            }//end 水位异常
#if WATER_L_IGNORE == 1
            else//水位正常，其他参数检测才有意义
            {
                EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
                GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
=======
                else //ˮλ�����쳣
                {
                    EleSwCtrl(SALT_SW,OFF);//�ط�2
                }

            }//end ˮλ�쳣
#if WATER_L_IGNORE == 1
            else//ˮλ������������������������
            {
                EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
                GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
            }
#endif

#if WATER_L_IGNORE == 0
            else
#endif
            {

<<<<<<< HEAD
                EleSwCtrl(WATER_SW,ON);//开阀1
#if WATER_L_IGNORE == 0
                EleSwCtrl(SALT_SW,OFF);//开阀2
                GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常

#endif

                EleSwCtrl(WASTE_SW,ON);//开阀3
                EleSwCtrl(WASH_SW,ON);//开阀4
                EleSwCtrl(HCILO_SW,OFF);//开阀5
                GetSensor()->status[TDS1_INDEX] = 0;//tds1异常
                GetSensor()->status[TDS2_INDEX] = 0;//
                if(GetSensor()->flow != 0)//流量异常
=======
                EleSwCtrl(WATER_SW,ON);//����1
#if WATER_L_IGNORE == 0
                EleSwCtrl(SALT_SW,OFF);//����2
                GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
                GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣

#endif

                EleSwCtrl(WASTE_SW,ON);//����3
                EleSwCtrl(WASH_SW,ON);//����4
                EleSwCtrl(HCILO_SW,OFF);//����5
                GetSensor()->status[TDS1_INDEX] = 0;//tds1�쳣
                GetSensor()->status[TDS2_INDEX] = 0;//
                if(GetSensor()->flow != 0)//�����쳣
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
                {
                    Flow_Init();
                    FlowCtrl();

                }
                else
                {
                    pid_init2(dstTds);
<<<<<<< HEAD
                    DcMotorCtrl(2,0);//泵2调整流量
=======
                    DcMotorCtrl(2,0);//��2��������
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
                    EleSwCtrl(WASTE_SW,OFF);//开阀3
                    //DcMotorCtrl(2,0);//关泵2
=======
                    EleSwCtrl(WASTE_SW,OFF);//����3
                    //DcMotorCtrl(2,0);//�ر�2
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
    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>=MIN_FLOW_SIZE&&  //加速启动电解速度
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//高压开关和水位正常且不在校准模式时启动电解
=======
    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>=MIN_FLOW_SIZE&&  //������������ٶ�
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//��ѹ���غ�ˮλ�����Ҳ���У׼ģʽʱ�������
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
    for(i=1; i<WORK_STATUS_SIZE; i++)    //故障状态处理
=======
    for(i=1; i<WORK_STATUS_SIZE; i++)    //����״̬����
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
    /*****************************************风扇控制***********************************************/
=======
    /*****************************************���ȿ���***********************************************/
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
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    // EleSwCtrl(WASH_SW,ON);//消毒水排出到废水阀关
    //EleSwCtrl(SALT_SW,ON);//盐盒进水阀开
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀开
    //EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关


    // EleSwCtrl(WASTE_SW,OFF);//废水出水阀开

    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    // EleSwCtrl(SALT_SW,ON);//盐盒进水阀开
    //  EleSwCtrl(WASTE_SW,ON);//废水出水阀开

    //  EleSwCtrl(WASH_SW,ON);//消毒水排出到废水阀关
    // EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    //  EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    //  EleSwCtrl(WATER_SW,OFF);//原水进水阀开
//
//
//
//    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀开
//    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
//    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
//    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
//  EleSwCtrl(WATER_SW,OFF);//原水进水阀开
//    DcMotorCtrl(2,0);//关泵2
=======
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    // EleSwCtrl(WASH_SW,ON);//����ˮ�ų�����ˮ����
    //EleSwCtrl(SALT_SW,ON);//�κн�ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    //EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����


    // EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����

    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    // EleSwCtrl(SALT_SW,ON);//�κн�ˮ����
    //  EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����

    //  EleSwCtrl(WASH_SW,ON);//����ˮ�ų�����ˮ����
    // EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
    //  EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    //  EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
//
//
//
//    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
//    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
//    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
//    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
//  EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
//    DcMotorCtrl(2,0);//�ر�2
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
    tmp = 0x5a; //配网标志，代表已经配过网络
=======
    tmp = 0x5a; //������־�������Ѿ��������
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
                module_reset(1);//主机和模块均复位
=======
                module_reset(1);//������ģ�����λ
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
        }

        //Soft_Reset();

        GetSensor()->status[FACTORY_INDEX] = FACTORY_INDEX;
<<<<<<< HEAD
        EleSwCtrl(6,OFF);//所有阀
        DcMotorCtrl(7,OFF);//所有电机
=======
        EleSwCtrl(6,OFF);//���з�
        DcMotorCtrl(7,OFF);//���е��
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
    //DcMotorCtrl(2,21000);//泵2调整流量
=======
    //DcMotorCtrl(2,21000);//��2��������
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
//FlowCtrl();


}
