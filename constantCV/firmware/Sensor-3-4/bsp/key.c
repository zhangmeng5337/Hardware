#include "key.h"
#include "reg.h"
#include "display.h"
#include "sensor.h"
#include "uart_hal.h"
#include "cs1237.h"




unsigned char task_index = 0;
key_irq_stru key_irq_usr;
//{
//  unsigned char menu_num;
//  unsigned char enter;
//  unsigned char next;
//  unsigned char return;
//  void (*current_operation)(void);
//}meun_task_stru;
input_params_stru input_dat[1] = {0};
void normal_menu(void);//password menu
void password_menu(void);//password menu
void addr_menu(void);//password menu
void baud_menu(void);//password menu
void par_menu(void);//password menu
void spd_menu(void);//password menu
void unit_menu(void);//password menu
void dot_menu(void);//password menu
void oft_menu(void);//password menu
void coe_menu(void);//password menu
void save_menu(void);//password menu
void addr2_menu(void);//password menu
void baud2_menu(void);//password menu
void par2_menu(void);//password menu
void spd2_menu(void);//password menu
void unit2_menu(void);//password menu
void dot2_menu(void);//password menu
void oft2_menu(void);//password menu
void coe2_menu(void);//password menu
void save2_menu(void);//password menu
void zero_menu(void);//password menu
void ver_menu(void);//password menu
void dev_addr_menu(void);//password menu


//typedef struct
//{
//	unsigned char menu_num;
//	unsigned char enter;
//	unsigned char next;
//	unsigned char retur;
//	unsigned char move;//光标
//	void (*current_operation)(void);
//}meun_task_stru;

//显示逻辑：上电显示版本号，3s后切换到正常显示
meun_task_stru task_table[] =
{
    {0, 0, 0, 0, 0, normal_menu},
    {1, 1, 1, 1, 0, password_menu},
    {2, 11, 3, 2, 0, addr_menu},
    {3, 12, 4, 2, 0, baud_menu},
    {4, 13, 5, 3, 0, par_menu},
    {5, 14, 6, 4, 0, spd_menu},
    {6, 15, 7, 5, 0, unit_menu},
    {7, 16, 8, 6, 0, dot_menu},
    {8, 17, 9, 7, 0, oft_menu},
    {9, 18, 10, 8, 0, coe_menu},
    {10, 19, 10, 9, 0, save_menu},

    {11, 2, 11, 11, 0, addr2_menu},
    {12, 3, 12, 12, 0, baud2_menu},
    {13, 4, 13, 13, 0, par2_menu},
    {14, 5, 14, 14, 0, spd2_menu},
    {15, 6, 15, 15, 0, unit2_menu},
    {16, 7, 16, 16, 0, dot2_menu},
    {17, 8, 17, 17, 0, oft2_menu},
    {18, 9, 18, 18, 0, coe2_menu},
    {19, 19, 19, 19, 0, save2_menu},
    {20, 20, 20, 20, 0, zero_menu},
    {21, 21, 21, 21, 0, ver_menu},
    {22, 22, 22, 22, 0, dev_addr_menu}

};
key_irq_stru *getKey()
{
    return &key_irq_usr;
}

unsigned char log_out(void)
{
    //static unsigned char last_index;
    static uint32_t tick;
#if LOG_ENABLE
    if ((GetTick() - tick) >= 1000)
    {
        tick =  GetTick();
        return 1;
    }
    else
        return 0;
#endif
    return 0;
}
uint32_t tick_tmp;
void ver_menu()
{
    if (task_index == 21)
    {
        if ((GetTick() - tick_tmp) >= 1000)
        {
            task_index = 0;
        }
        if (key_irq_usr.double_check == INC)
        {

            GetDisp()->dis_buf[0] = A;
            key_irq_usr.indat[0].passw[1] =  0;
            key_irq_usr.indat[0].passw[2] =  VERSION / 10;
            key_irq_usr.indat[0].passw[3] =  VERSION % 10;


            // GetDisp()->dis_buf[0] =  GetSeg(key_irq_usr.indat[0].passw[1]);
            GetDisp()->dis_buf[1] =  GetSeg(key_irq_usr.indat[0].passw[1]) | 0x80;
            GetDisp()->dis_buf[2] =  GetSeg(key_irq_usr.indat[0].passw[2]);
            GetDisp()->dis_buf[3] =  GetSeg(key_irq_usr.indat[0].passw[3]);
            key_irq_usr.bit_sel = BIT_SEL;
            GetDisp()->cusor = 0;

        }
    }
}
void dev_addr_menu()
{
    if (task_index == 22)
    {
        if ((GetTick() - tick_tmp) >= 1000)
        {
            task_index = 0;
        }
        if (key_irq_usr.double_check == MOVE)
        {


            GetDisp()->dis_buf[0] = A;
            key_irq_usr.indat[0].passw[1] =
                GetReg()->pb[eREG_DEV_ADDR].val_u32ToFloat / 100;
            key_irq_usr.indat[0].passw[2] =
                GetReg()->pb[eREG_DEV_ADDR].val_u32ToFloat % 100 / 10;
            key_irq_usr.indat[0].passw[3] =
                GetReg()->pb[eREG_DEV_ADDR].val_u32ToFloat % 10;


            // GetDisp()->dis_buf[0] =  GetSeg(key_irq_usr.indat[0].passw[1]);
            GetDisp()->dis_buf[1] =  GetSeg(key_irq_usr.indat[0].passw[1]);
            GetDisp()->dis_buf[2] =  GetSeg(key_irq_usr.indat[0].passw[2]);
            GetDisp()->dis_buf[3] =  GetSeg(key_irq_usr.indat[0].passw[3]);
            key_irq_usr.bit_sel = BIT_SEL;
            GetDisp()->cusor = 0;

        }
    }
}

void zero_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].status >= 1 && key_irq_usr.indat[0].status < 3)
        {
            key_irq_usr.indat[0].status ++;

        }
        else
            key_irq_usr.indat[0].status = 1;
    }

    if (key_irq_usr.indat[0].status == 1)//yes
    {

        GetDisp()->dis_buf[0] = 0x00;
        GetDisp()->dis_buf[1] = 0x6e;
        GetDisp()->dis_buf[2] = 0x79;
        GetDisp()->dis_buf[3] = 0x6d;
        display_led(1, key_irq_usr.bit_sel);
        if (key_irq_usr.double_check == SHORT_PRE)
        {
            key_irq_usr.double_check = NO_PRE;
            key_irq_usr.update = 1;
            GetRegPrivate()->zero_cmd = 1;
            GetRegPrivate()->zero_value = getadc()->dat_unit_factory;
            task_index = 0;
		   key_irq_usr.bit_sel = BIT_SEL;

        }

    }
    else if (key_irq_usr.indat[0].status == 2)//yes //no
    {
        GetDisp()->dis_buf[0] = 0x00;
        GetDisp()->dis_buf[1] = 0x00;
        GetDisp()->dis_buf[2] = 0x37;
        GetDisp()->dis_buf[3] = 0x5c;
        display_led(1, key_irq_usr.bit_sel);
        if (key_irq_usr.double_check == SHORT_PRE)
        {
            key_irq_usr.update = 1;
            key_irq_usr.update = 1;
            key_irq_usr.double_check = NO_PRE;
			GetRegPrivate()->zero_value = 0;
            GetRegPrivate()->zero_cmd = 0;
            task_index = 0;
			   key_irq_usr.bit_sel = BIT_SEL;
        }
    }
    else //rst
    {
        GetDisp()->dis_buf[0] = 0x00;
        GetDisp()->dis_buf[1] = 0x50;
        GetDisp()->dis_buf[2] = 0x6d;
        GetDisp()->dis_buf[3] = 0x78;
        // display_led(1, key_irq_usr.bit_sel);
        if (key_irq_usr.double_check == SHORT_PRE)
        {
            key_irq_usr.double_check = NO_PRE;
            //GetRegPrivate()->zero_cmd = 1;
            task_index = 0;
			   key_irq_usr.bit_sel = BIT_SEL;
        }
    }

    GetDisp()->cusor = 0;
}

void save2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].status >= 1 && key_irq_usr.indat[0].status < 2)
        {
            key_irq_usr.indat[0].status ++;

        }
        else
            key_irq_usr.indat[0].status = 1;
    }

    if (key_irq_usr.indat[0].status == 1)//yes
    {

        GetDisp()->dis_buf[0] = 0x00;
        GetDisp()->dis_buf[1] = 0x6e;
        GetDisp()->dis_buf[2] = 0x79;
        GetDisp()->dis_buf[3] = 0x6d;
        display_led(1, key_irq_usr.bit_sel);
        if (key_irq_usr.double_check == SHORT_PRE)
        {
            key_irq_usr.double_check = NO_PRE;

            // if (log_out() == 1)
            {
                key_irq_usr.update = 1;
                if (key_irq_usr.indat[0].coe >= 0 && key_irq_usr.indat[0].coe <= 1.9999)
                    GetRegPrivate()->coe = key_irq_usr.indat[0].coe;
                if (key_irq_usr.indat[0].oft <= 50000)
                    GetRegPrivate()->offset = key_irq_usr.indat[0].oft;
                GetReg()->pb[eREG_DEV_ADDR].val_u32ToFloat = key_irq_usr.indat[0].addr;
                GetReg()->pb[eREG_RATE].val_u32ToFloat = key_irq_usr.indat[0].baud;
                GetReg()->pb[eREG_CHECK].val_u32ToFloat = key_irq_usr.indat[0].par;
                GetReg()->pb[eREG_DECM_BIT].val_u32ToFloat = key_irq_usr.indat[0].dot;
                GetReg()->pb[eREG_UNIT].val_u32ToFloat = key_irq_usr.indat[0].unit;
				if(key_irq_usr.indat[0].spd == 0)
                GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat = 10;
				else
				GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat = 40;	


                //                printf("addr=%d\r\n", key_irq_usr.indat[0].addr);
                //                SysTickDelay(100);
                //                printf("baud=%d\r\n", key_irq_usr.indat[0].baud);
                //                SysTickDelay(100);
                //                printf("par=%d\r\n", key_irq_usr.indat[0].par);
                //                SysTickDelay(100);
                //                printf("spd=%d\r\n", key_irq_usr.indat[0].spd);
                //                SysTickDelay(100);
                //                printf("unit=%d\r\n", key_irq_usr.indat[0].unit);
                //                SysTickDelay(100);
                //                printf("dot=%d\r\n", key_irq_usr.indat[0].dot);
                //                SysTickDelay(100);
                //                printf("oft=%d\r\n", key_irq_usr.indat[0].oft);
                //                SysTickDelay(100);
                //                printf("coe=.4%f\r\n", key_irq_usr.indat[0].coe);
                //                SysTickDelay(100);

            }
            task_index = 0;

        }

    }
    else
    {
        GetDisp()->dis_buf[0] = 0x00;
        GetDisp()->dis_buf[1] = 0x00;
        GetDisp()->dis_buf[2] = 0x37;
        GetDisp()->dis_buf[3] = 0x5c;
        display_led(1, key_irq_usr.bit_sel);
        if (key_irq_usr.indat[0].status == 2)//no
        {

            if (key_irq_usr.double_check == SHORT_PRE)
            {
                key_irq_usr.double_check = NO_PRE;
                task_index = 0;
            }
            if (log_out() == 1)
                printf("%d\r\n", key_irq_usr.indat[0].status);

        }
        else
        {
            if(key_irq_usr.indat[0].status == 0 && key_irq_usr.double_check == SHORT_PRE)
        {
            key_irq_usr.indat[0].status = 2;
                key_irq_usr.double_check = NO_PRE;

            }

        }

    }


    //display_led(GetDisp()->cusor, getKey()->bit_sel);
    GetDisp()->cusor = 0;



}

void coe2_menu(void)//password menu
{

    uint32_t tmp = 0;
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 1)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;
    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] =
            key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] + 1;

        if (key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] > 9)
            key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] = 0;
        //printf("%d,%d\r\n", key_irq_usr.bit_sel,key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1]);
        // key_irq_usr.indat[0].coe = 0;
        // if(log_out() == 1)
        //printf("%.4f\r\n",key_irq_usr.indat[0].coe);


        tmp = key_irq_usr.indat[0].passw[0] * 1000 ;
        tmp = tmp + key_irq_usr.indat[0].passw[1] * 100;
        tmp = tmp + key_irq_usr.indat[0].passw[2] * 10;
        tmp = tmp + key_irq_usr.indat[0].passw[3];
        key_irq_usr.indat[0].coe  = tmp / 1000.0 ;




        if (key_irq_usr.indat[0].coe >= 0.0001 && key_irq_usr.indat[0].coe < 1.9999)
        {
            ;//0.000-1.999 0.0001  1.9999 1.981 0.123   0.1230 0.124 0.1231

            //0.123  0.0123  0.124 0.0124 0.125 0.0125 1230 0.1234 0.01234 0.123 1000
            //0.001  0.0001 0.002 0.0002 0.123 123/
        } //0.123  0.124  0.1230 0.1231 0.125 0.1232

        //        else
        //        {
        //            key_irq_usr.indat[0].coe = 0.0001;
        //            key_irq_usr.indat[0].passw[0] = 1;
        //            key_irq_usr.indat[0].passw[1] = 0;
        //            key_irq_usr.indat[0].passw[2] = 0;
        //            key_irq_usr.indat[0].passw[3] = 0;
        //
        //        }


        // if(log_out() == 1)
        printf("%.4f\r\n", key_irq_usr.indat[0].coe);

    }

    //    key_irq_usr.indat[0].passw[0] = tmp / 1000;
    //    key_irq_usr.indat[0].passw[1] = tmp % 1000 / 100;
    //    key_irq_usr.indat[0].passw[2] = tmp % 100 / 10;
    //    key_irq_usr.indat[0].passw[3] = tmp % 10;

    GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]) | 0x80;
    GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;



}

key_irq_stru *GetKeyStatus()
{
    return &key_irq_usr;
}
void oft2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;
        key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] ++;

        if (key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] > 9)
            key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] = 0;


        key_irq_usr.indat[0].oft = key_irq_usr.indat[0].passw[0] * 1000 ;
        key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft +
                                   key_irq_usr.indat[0].passw[1] * 100;
        key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft +
                                   key_irq_usr.indat[0].passw[2] * 10;
        key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft +
                                   key_irq_usr.indat[0].passw[3];
        if (key_irq_usr.indat[0].dot == 0)
        {


            key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft;

        }
        else   if (key_irq_usr.indat[0].dot == 1)//999.4
        {

            key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft / 10.0;

        }
        else   if (key_irq_usr.indat[0].dot == 2)//99.23
        {

            key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft / 100.0;


        }
        else   if (key_irq_usr.indat[0].dot == 3)//9.234
        {

            key_irq_usr.indat[0].oft = key_irq_usr.indat[0].oft / 1000.0;

        }
        else
            ;

        //        if (key_irq_usr.indat[0].oft >= -2 && key_irq_usr.indat[0].oft < 2)
        //        {
        //            ;//key_irq_usr.indat[0].addr ++;
        //
        //
        //        }
        //        else
        //        {
        //            key_irq_usr.indat[0].oft = 0;
        //            key_irq_usr.indat[0].passw[0] = 0;
        //            key_irq_usr.indat[0].passw[1] = 0;
        //            key_irq_usr.indat[0].passw[2] = 0;
        //            key_irq_usr.indat[0].passw[3] = 0;
        //
        //        }

        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].oft);

    }


    //    key_irq_usr.indat[0].passw[0] = key_irq_usr.indat[0].oft / 1000;
    //    key_irq_usr.indat[0].passw[1] = key_irq_usr.indat[0].oft % 1000 / 100;
    //    key_irq_usr.indat[0].passw[2] = key_irq_usr.indat[0].oft % 100 / 10;
    //    key_irq_usr.indat[0].passw[3] = key_irq_usr.indat[0].oft % 10;
    //GetRegPrivate()->offset=input_dat[0].oft  ;

    if (key_irq_usr.indat[0].dot == 0)
    {
        GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]);
        GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
        GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
        GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);

    }
    else if (key_irq_usr.indat[0].dot == 1)
    {
        GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]);
        GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
        GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]) | 0x80;
        GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);

    }
    else if (key_irq_usr.indat[0].dot == 2)
    {
        GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]);
        GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
        GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]) | 0x80;
        GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);

    }
    else if (key_irq_usr.indat[0].dot == 3)
    {
        GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]) | 0x80;
        GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
        GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
        GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]) ;

    }
    //
    //    GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]);
    //    GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
    //    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    //    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;



}

void dot2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].dot < 3)
        {
            key_irq_usr.indat[0].dot ++;

        }
        else
            key_irq_usr.indat[0].dot = 0;
        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].dot);


    }


    key_irq_usr.indat[0].passw[3] = (key_irq_usr.indat[0].dot) % 10;
    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = 0;
    GetDisp()->dis_buf[2] = 0;
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    //  display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;


}

void unit2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].unit < 13)
        {
            key_irq_usr.indat[0].unit ++;

        }
        else
            key_irq_usr.indat[0].unit = 0;
        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].unit);

    }


    key_irq_usr.indat[0].passw[2] = (key_irq_usr.indat[0].unit) / 10;
    key_irq_usr.indat[0].passw[3] = (key_irq_usr.indat[0].unit) % 10;

    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = 0;
    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;


}

void spd2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].spd == 0)
        {
            key_irq_usr.indat[0].spd = 1;

        }
        else
            key_irq_usr.indat[0].spd = 0;
        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].spd);
        *getAdcReconfig() = 1;


        //        key_irq_usr.indat[0].spd = key_irq_usr.indat[0].passw[2] * 10;
        //        key_irq_usr.indat[0].spd = key_irq_usr.indat[0].spd +
        //                                   key_irq_usr.indat[0].passw[3];
        if (key_irq_usr.indat[0].spd == 1 ||
                key_irq_usr.indat[0].spd == 0)
            ;
        else
            key_irq_usr.indat[0].spd == 0;



    }

    if (key_irq_usr.indat[0].spd == 0)
    {
        GetDisp()->dis_buf[0] = 0;
        GetDisp()->dis_buf[1] = 0;
        GetDisp()->dis_buf[2] = GetSeg(0x01);
        GetDisp()->dis_buf[3] = GetSeg(0x00);
    }
    else
    {
        GetDisp()->dis_buf[0] = 0;
        GetDisp()->dis_buf[1] = 0;
        GetDisp()->dis_buf[2] = GetSeg(0x04);
        GetDisp()->dis_buf[3] = GetSeg(0x00);

    }
    //  display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;


}

void par2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].par < 3)
        {
            key_irq_usr.indat[0].par ++;

        }
        else
            key_irq_usr.indat[0].par = 0;
        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].par);
        getuart()->reconfig = 1;
    }


    if (key_irq_usr.indat[0].par == 1)
    {
        GetDisp()->dis_buf[3] = N;

    }
    else    if (key_irq_usr.indat[0].par == 2)
    {
        GetDisp()->dis_buf[3] = O ;

    }
    else    if (key_irq_usr.indat[0].par == 3)
    {
        GetDisp()->dis_buf[3] = E;

    }
    else
    {
        GetDisp()->dis_buf[3] = N;

    }


    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = 0;
    GetDisp()->dis_buf[2] = 00;
    //GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, 4);
    GetDisp()->cusor = 1;


}

void dat_conv(uint32_t dat)//123  12
{

    key_irq_usr.indat[0].passw[0] = dat / 1000;
    key_irq_usr.indat[0].passw[1] = dat % 1000 / 100;
    key_irq_usr.indat[0].passw[2] = dat % 100 / 10;
    key_irq_usr.indat[0].passw[3] = dat % 10;

}
unsigned int baud_sel(unsigned char sel)
{
    unsigned int result;
    switch (sel)
    {
        case 0:
            result = 1200;
            break;
        case 1:
            result = 2400;
            break;
        case 2:
            result = 4800;
            break;
        case 3:
            result = 9600;
            break;
        case 4:
            result = 19200;
            break;
        case 5:
            result = 38400;
            break;
        case 6:
            result = 57600;
            break;
        case 7:
            result = 115200;
            break;
        default:
            result = 9600;
            break;
    }
    return result;
}
void baud2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        if (key_irq_usr.indat[0].baud < 7)
        {
            key_irq_usr.indat[0].baud ++;

        }
        else
            key_irq_usr.indat[0].baud = 1;
        unsigned int result;
        result = baud_sel(key_irq_usr.indat[0].baud);
        result = result / 100;
        key_irq_usr.indat[0].passw[0] = result / 1000;
        key_irq_usr.indat[0].passw[1] = result % 1000 / 100;
        key_irq_usr.indat[0].passw[2] = result % 100 / 10;
        key_irq_usr.indat[0].passw[3] = result % 10;
        getuart()->reconfig = 1;
        if (log_out() == 1)
            printf("%d\r\n", result);
    }



    GetDisp()->dis_buf[0] =  GetSeg(key_irq_usr.indat[0].passw[0]);
    GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;


}

void addr2_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 2)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;
    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;

        key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] ++;
        if (key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] > 9)
            key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] = 0;

        key_irq_usr.indat[0].addr = key_irq_usr.indat[0].passw[1] * 100 ;
        key_irq_usr.indat[0].addr = key_irq_usr.indat[0].addr +
                                    key_irq_usr.indat[0].passw[2] * 10;
        key_irq_usr.indat[0].addr = key_irq_usr.indat[0].addr +
                                    key_irq_usr.indat[0].passw[3];

        if (key_irq_usr.indat[0].addr >= 1 && key_irq_usr.indat[0].addr < 247)
        {
            ;//key_irq_usr.indat[0].addr ++;


        }
        else
        {
            key_irq_usr.indat[0].addr = 1;

            key_irq_usr.indat[0].passw[0] = 0;
            key_irq_usr.indat[0].passw[1] = 0;
            key_irq_usr.indat[0].passw[2] = 0;
            key_irq_usr.indat[0].passw[3] = 1;

        }
        if (log_out() == 1)
            printf("%d\r\n", key_irq_usr.indat[0].addr);


    }


    // key_irq_usr.indat[0].passw[1] = key_irq_usr.indat[0].addr / 100;
    // key_irq_usr.indat[0].passw[2] = key_irq_usr.indat[0].addr % 100 / 10;
    // key_irq_usr.indat[0].passw[3] = key_irq_usr.indat[0].addr % 10;

    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    // display_led(1, key_irq_usr.bit_sel);
    GetDisp()->cusor = 1;


}

void save_menu(void)//password menu
{
    GetDisp()->dis_buf[0] = S;
    GetDisp()->dis_buf[1] = A;
    GetDisp()->dis_buf[2] = U;
    GetDisp()->dis_buf[3] = E;
    GetDisp()->cusor = 0;
    key_irq_usr.indat[0].status = 0;
    //  display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void coe_menu(void)//password menu
{
    uint16_t tmp;
    tmp = key_irq_usr.indat[0].coe * 1000;//0.1234  0.123
    key_irq_usr.indat[0].passw[0] = tmp / 1000;
    key_irq_usr.indat[0].passw[1] = tmp % 1000 / 100;
    key_irq_usr.indat[0].passw[2] = tmp % 100 / 10;
    key_irq_usr.indat[0].passw[3] = tmp % 10;



    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = C;
    GetDisp()->dis_buf[2] = O;
    GetDisp()->dis_buf[3] = E;
    GetDisp()->cusor = 0;
    // display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void oft_menu(void)//password menu
{
    int32_t tmp;
    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = O;
    GetDisp()->dis_buf[2] = F;
    GetDisp()->dis_buf[3] = T;
    GetDisp()->cusor = 0;
    // input_dat[0].oft = GetRegPrivate()->offset;
    if (key_irq_usr.indat[0].dot == 0)
    {
        key_irq_usr.indat[0].passw[0] = key_irq_usr.indat[0].oft;
        key_irq_usr.indat[0].passw[1] = 0;
        key_irq_usr.indat[0].passw[2] = 0;
        key_irq_usr.indat[0].passw[3] = 0;
    }
    else   if (key_irq_usr.indat[0].dot == 1)//999.4
    {
        tmp =  key_irq_usr.indat[0].oft * 10;
        key_irq_usr.indat[0].passw[0] = tmp / 1000;
        key_irq_usr.indat[0].passw[1] =  tmp % 1000 / 100;
        key_irq_usr.indat[0].passw[2] =  tmp % 100 / 10;
        key_irq_usr.indat[0].passw[3] =  tmp % 10;
    }
    else   if (key_irq_usr.indat[0].dot == 2)//99.23
    {
        tmp =  key_irq_usr.indat[0].oft * 100;
        key_irq_usr.indat[0].passw[0] = tmp / 1000;
        key_irq_usr.indat[0].passw[1] =  tmp % 1000 / 100;
        key_irq_usr.indat[0].passw[2] =  tmp % 100 / 10;
        key_irq_usr.indat[0].passw[3] =  tmp % 10;



    }
    else   if (key_irq_usr.indat[0].dot == 3)//9.234
    {

        tmp =  key_irq_usr.indat[0].oft * 1000;
        key_irq_usr.indat[0].passw[0] = tmp / 1000;
        key_irq_usr.indat[0].passw[1] =  tmp % 1000 / 100;
        key_irq_usr.indat[0].passw[2] =  tmp % 100 / 10;
        key_irq_usr.indat[0].passw[3] =  tmp % 10;

    }



    //display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void dot_menu(void)//password menu
{
    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = D;
    GetDisp()->dis_buf[2] = O;
    GetDisp()->dis_buf[3] = T;
    GetDisp()->cusor = 0;
    /// display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void unit_menu(void)//password menu
{



    GetDisp()->dis_buf[0] = U;
    GetDisp()->dis_buf[1] = N;
    GetDisp()->dis_buf[2] = I;
    GetDisp()->dis_buf[3] = T;
    GetDisp()->cusor = 0;
    //display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void spd_menu(void)//password menu
{
    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = S;
    GetDisp()->dis_buf[2] = P;
    GetDisp()->dis_buf[3] = D;
    GetDisp()->cusor = 0;
    // display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void par_menu(void)//password menu
{

    GetDisp()->dis_buf[0] = 0;
    GetDisp()->dis_buf[1] = P;
    GetDisp()->dis_buf[2] = A;
    GetDisp()->dis_buf[3] = R;
    GetDisp()->cusor = 0;
    // display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;


}

void baud_menu(void)//password menu
{
    unsigned result;
    result = baud_sel(key_irq_usr.indat[0].baud);
    result = result / 100;
    key_irq_usr.indat[0].passw[0] = result / 1000;
    key_irq_usr.indat[0].passw[1] = result % 1000 / 100;
    key_irq_usr.indat[0].passw[2] = result % 100 / 10;
    key_irq_usr.indat[0].passw[3] = result % 10;




    GetDisp()->dis_buf[0] = B;
    GetDisp()->dis_buf[1] = A;
    GetDisp()->dis_buf[2] = U;
    GetDisp()->dis_buf[3] = D;
    GetDisp()->cusor = 0;
    //display_led(0, key_irq_usr.bit_sel);
    key_irq_usr.bit_sel = BIT_SEL;

}

void addr_menu(void)//password menu
{
    key_irq_usr.indat[0].passw[0] = 0;
    key_irq_usr.indat[0].passw[1] = key_irq_usr.indat[0].addr / 100;
    key_irq_usr.indat[0].passw[2] = key_irq_usr.indat[0].addr % 100 / 10;
    key_irq_usr.indat[0].passw[3] = key_irq_usr.indat[0].addr % 10;


    GetDisp()->dis_buf[0] = A;
    GetDisp()->dis_buf[1] = D;
    GetDisp()->dis_buf[2] = D;
    GetDisp()->dis_buf[3] = R;
    key_irq_usr.bit_sel = BIT_SEL;
    GetDisp()->cusor = 0;
    // display_led(0, key_irq_usr.bit_sel);

}
void normal_menu(void)//password menu
{
    ;//display();
    key_irq_usr.indat[0].passw[0] = 0;
    key_irq_usr.indat[0].passw[1] = 0;
    key_irq_usr.indat[0].passw[2] = 0;
    key_irq_usr.indat[0].passw[3] = 0;
	
	key_irq_usr.bit_sel = BIT_SEL;
}


void password_menu(void)//password menu
{
    if (task_table[task_index].move == MOVE)
    {
        task_table[task_index].move = NO_PRE;
        if (key_irq_usr.bit_sel > 0)
            key_irq_usr.bit_sel = key_irq_usr.bit_sel - 1;
        else
            key_irq_usr.bit_sel = BIT_SEL;

    }
    if (key_irq_usr.double_check == INC)
    {
        key_irq_usr.double_check = NO_PRE;
        key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] ++;
        if (key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] > 9)
            key_irq_usr.indat[0].passw[key_irq_usr.bit_sel - 1] = 0;
    }
    if (key_irq_usr.double_check == SHORT_PRE)
    {
        key_irq_usr.double_check = NO_PRE;
        // dat_conv(16);
        if (key_irq_usr.indat[0].passw[3] == 0x06 &&
                key_irq_usr.indat[0].passw[2] == 0x01 &&
                key_irq_usr.indat[0].passw[1] == 0x00 &&
                key_irq_usr.indat[0].passw[0] == 0x00
           )
            task_index = 2;
        else  if (key_irq_usr.indat[0].passw[3] == 0x06 &&
                  key_irq_usr.indat[0].passw[2] == 0x07 &&
                  key_irq_usr.indat[0].passw[1] == 0x08 &&
                  key_irq_usr.indat[0].passw[0] == 0x09
                 )
        {
            task_index = 0;
            GetRegPrivate()->mode = 2;
            getuart()->reconfig = 1;
            *getAdcReconfig() = 1;
            GetRegPrivate()->save = 1;
            key_irq_usr.update = 1;
			key_irq_usr.indat[0].status = 0;

        }
        else  if (key_irq_usr.indat[0].passw[3] == 0x06 &&
                  key_irq_usr.indat[0].passw[2] == 0x03 &&
                  key_irq_usr.indat[0].passw[1] == 0x00 &&
                  key_irq_usr.indat[0].passw[0] == 0x00
                 )
        {
            task_index = 20;
            // GetRegPrivate()->mode = 2;
            getuart()->reconfig = 1;
            *getAdcReconfig() = 1;
            GetDisp()->dis_buf[0] = 0x00;
            GetDisp()->dis_buf[1] = 0x6e;
            GetDisp()->dis_buf[2] = 0x79;
            GetDisp()->dis_buf[3] = 0x6d;
			key_irq_usr.indat[0].status = 1;
			key_irq_usr.key_irq_status = 0;
        }

    }


    GetDisp()->dis_buf[0] = GetSeg(key_irq_usr.indat[0].passw[0]);
    GetDisp()->dis_buf[1] = GetSeg(key_irq_usr.indat[0].passw[1]);
    GetDisp()->dis_buf[2] = GetSeg(key_irq_usr.indat[0].passw[2]);
    GetDisp()->dis_buf[3] = GetSeg(key_irq_usr.indat[0].passw[3]);
    GetDisp()->cusor = 1;

    //display_led(1, key_irq_usr.bit_sel);
}
void key_init(void)
{
    display_init();
    input_dat[0].addr = GetReg()->pb[eREG_DEV_ADDR].val_u32ToFloat;
    input_dat[0].baud = GetReg()->pb[eREG_RATE].val_u32ToFloat;
    input_dat[0].baud = input_dat[0].baud ;
    input_dat[0].par  =  GetReg()->pb[eREG_CHECK].val_u32ToFloat;
	if(GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat == 10)
    input_dat[0].spd  =  0;
	else
    input_dat[0].spd  =  1;		
    input_dat[0].unit = GetReg()->pb[eREG_UNIT].val_u32ToFloat;
    input_dat[0].dot  =  GetReg()->pb[eREG_DECM_BIT].val_u32ToFloat;
    if (input_dat[0].dot > 3)
        input_dat[0].dot = 1;
    input_dat[0].coe  =  GetRegPrivate()->coe;
    input_dat[0].oft = GetRegPrivate()->offset;


    key_irq_usr.indat[0].passw[0] = 0;
    key_irq_usr.indat[0].passw[1] = 0;
    key_irq_usr.indat[0].passw[2] = 0;
    key_irq_usr.indat[0].passw[3] = 0;

    key_irq_usr.double_check = NO_PRE;
    task_index = 0;
    key_irq_usr.bit_sel = BIT_SEL;
    key_irq_usr.state = 0;
    key_irq_usr.update = 0;
    key_irq_usr.indat = input_dat;
}

void KeySw1Irq(unsigned key_irq_num)
{
    switch (key_irq_num)
    {
        case KEY1_NUM://-
            key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
            key_irq_usr.delay_tick = GetTick();
            break;
        case KEY2_NUM:
            key_irq_usr.key_irq_status  = key_irq_usr.key_irq_status | 0x02;
            key_irq_usr.delay_tick = GetTick();
            break;
        case KEY3_NUM:
            key_irq_usr.key_irq_status  = key_irq_usr.key_irq_status | 0x04;
            key_irq_usr.delay_tick = GetTick();
            break;
    }
    //key_irq_usr.delay_tick = GetTick();
    key_irq_usr.update = 1;
}
void KeyProc(void)
{
    uint32_t time_tick;
    time_tick = GetTick() - key_irq_usr.delay_tick;
    if (time_tick >= KEY_TIMEOUT)
    {
        task_index = 0;
        key_irq_usr.key_irq_status = 0;
        key_irq_usr.state = 0 ;
        key_irq_usr.update = 0;
        if (*getAdcReconfig() == 1)
            *getAdcReconfig() = 0;
        if (getuart()->reconfig == 1)
            getuart()->reconfig = 0;

    }
    else
    {
        switch (key_irq_usr.state)
        {
            case 0:
                if ((key_irq_usr.key_irq_status & 0x07)) //double press
                {
                    if ((GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)  &&
                            GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)  &&
                            GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)) == 0)
                    {
                        if ((time_tick) >= 20) //5s
                        {
                            key_irq_usr.state ++ ;
                        }
                        else
                        {
                            //key2 3 1
                            if (GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN))
                            {
                                key_irq_usr.key_irq_status = key_irq_usr.key_irq_status & 0xfb;

                            }
                            if (GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN))
                            {
                                key_irq_usr.key_irq_status = key_irq_usr.key_irq_status & 0xfe;

                            }
                            if (GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN))
                            {
                                key_irq_usr.key_irq_status = key_irq_usr.key_irq_status & 0xfd;

                            }



                        }
                    }
                }

                break;
            case 1:
                if ((GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) &&
                        GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) &&
                        GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)))
                {

                    if ((time_tick) <= SHORT_PRES)  //5s
                    {
                        if ((key_irq_usr.key_irq_status & 0x03) == 0x03)
                        {
                            key_irq_usr.update = 0;
                            key_irq_usr.double_check = SHORT_PRE;//short pre
                            task_index = task_table[task_index].enter;
                        }
                        else
                        {
                            if ((key_irq_usr.key_irq_status & 0x02))
                            {
                                key_irq_usr.update = 0;
                                key_irq_usr.double_check = INC;
                                if (task_index == 0 || task_index == 22)
                                {
                                    tick_tmp = GetTick();
                                    task_index = 21;
                                }
                                task_index = task_table[task_index].next;
                                key_irq_usr.key_irq_status = 0;

                            }
                            else if ((key_irq_usr.key_irq_status & 0x01))
                            {
                                key_irq_usr.update = 0;
                                key_irq_usr.double_check = MOVE;
                                if (task_index == 0 || task_index == 21)
                                {
                                    tick_tmp = GetTick();
                                    task_index = 22;
                                }
                                task_table[task_index].move = MOVE;
                                task_index = task_table[task_index].retur;
                                key_irq_usr.key_irq_status = 0;
                            }
                        }
                    }
                    else if ((time_tick) >= LONG_PRES)  //5s
                    {
                        if ((key_irq_usr.key_irq_status & 0x03) == 0x03)
                        {
                            key_irq_usr.update = 0;
                            task_index = 1;
                            key_irq_usr.double_check = LONG_PRE;//long pre
                            key_irq_usr.key_irq_status = 0;
                        }
                    }
                    key_irq_usr.state = 0 ;
                }
                if ((time_tick) >= LONG_PRES)  //5s
                {
                    if ((key_irq_usr.key_irq_status & 0x03) == 0x03)
                    {
                        key_irq_usr.update = 0;
                        task_index = 1;
                        key_irq_usr.double_check = LONG_PRE;//long pre
                        key_irq_usr.key_irq_status = 0;
                    }
                    key_irq_usr.state = 0 ;
                }

                break;
        }
    }
}

unsigned char getTaskIndex()
{
    return task_index;
}
void display_menu(void)
{

    // KeyProc();
    task_table[task_index].current_operation();

}
void GPIOA_IRQHandlerCallback(void)
{

    static uint32_t tick_tmp;
	tick_tmp = GetTick();
    if (CW_GPIOA->ISR_f.PIN3)
    {
        GPIOA_INTFLAG_CLR(bv3);
		if( GPIO_ReadPin(KEY1_GPIO_PORT,KEY1_GPIO_PIN)== 0)
		{
			while((GetTick()-tick_tmp)<KEY_FILTER)
				;
			if( GPIO_ReadPin(KEY1_GPIO_PORT,KEY1_GPIO_PIN)== 0)
			{
			key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x04;
			key_irq_usr.delay_tick = GetTick();

			}
		}

    }
    if (CW_GPIOA->ISR_f.PIN4)
    {
        GPIOA_INTFLAG_CLR(bv4);
		if( GPIO_ReadPin(KEY3_GPIO_PORT,KEY3_GPIO_PIN)== 0)
		{
			while((GetTick()-tick_tmp)<KEY_FILTER)
				;
			if( GPIO_ReadPin(KEY3_GPIO_PORT,KEY3_GPIO_PIN)== 0)
			{
			key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x02;
			key_irq_usr.delay_tick = GetTick();

			}
		}

    }




}
void GPIOB_IRQHandlerCallback(void)
{
    //    if (CW_GPIOB->ISR_f.PIN2)
    //    {
    //        GPIOB_INTFLAG_CLR(bv2);
    //        key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
    //        key_irq_usr.delay_tick = GetTick();
    //
    //    }

#ifdef HW_VER1

    if (CW_GPIOB->ISR_f.PIN2)
    {
        GPIOB_INTFLAG_CLR(bv2);
        key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
        key_irq_usr.delay_tick = GetTick();
		if( GPIO_ReadPin(KEY2_GPIO_PORT,KEY2_GPIO_PIN)== 0)
		{
			while((GetTick()-tick_tmp)<KEY_FILTER)
				;
			if( GPIO_ReadPin(KEY2_GPIO_PORT,KEY2_GPIO_PIN)== 0)
			{
			key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
			key_irq_usr.delay_tick = GetTick();
		
			}
		}

    }

#endif

#ifdef HW_VER2

    if (CW_GPIOB->ISR_f.PIN7)
    {
        GPIOB_INTFLAG_CLR(bv7);
		if( GPIO_ReadPin(KEY2_GPIO_PORT,KEY2_GPIO_PIN)== 0)
		{
			while((GetTick()-tick_tmp)<KEY_FILTER)
				;
			if( GPIO_ReadPin(KEY2_GPIO_PORT,KEY2_GPIO_PIN)== 0)
			{
			key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
			key_irq_usr.delay_tick = GetTick();
		
			}
		}

    }
    if (CW_GPIOB->ISR_f.PIN6)
    {
        GPIOB_INTFLAG_CLR(bv6);
        get_adc_update(1);
        // __disable_irq();

        //   __enable_irq();

    }



#endif

}




