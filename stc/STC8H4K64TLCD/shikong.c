/***********************************
*包括6个按键, 12864LCD显示
*1路ttl通信9600
*创建：xielungui
*日期：2018-10-28 10:28
************************************/
#include "stc8h.h"       //包含此头文件后，不需要再包含"reg51.h"头文件

//#include "reg51.h"
#include "intrins.h"
#include "stdio.h"
#include <math.h>
#include <24c02.h>
//#include <ds1302.h>
#include <RTC.h>
#include <12864.h>
#include <sun.h>
#include <canshu.h>
#include <pwr.h>

void jies_data()
{
    switch(rp[1])
    {
    case 0x11:
    {
        if(ipdz!=rp[3])
        {
            read_ee_data(7,0);
            IapErase(7,0);
            ipdz=rp[3];
            buf[8] =rp[3];
            write_add(7,0,rp[3]);
#if DEBUG_EN == 1
            com_data_tx(0x5e);
            com_data_tx(read_add(7, 8));
#endif
            delaya(50);
        }
        yzbl=rp[4];
        jsll=rp[5];
        gm_tt=rp[6]&0x80;
        gzdu=rp[6]&0x7f;
        if(xf_fig!=rp[7])
        {
            xf_fig=rp[7];
            read_ee_data(7,0);
            IapErase(7,0);
            buf[15] =xf_fig;
            write_add(7,0,xf_fig);
            delaya(50);
#if DEBUG_EN == 1
            com_data_tx(0x5f);
            com_data_tx(read_add(7, 15));
#endif

        }
        if(botelv_fig!=rp[8])
        {
            botelv_fig=rp[8];
            read_ee_data(7,0);
            IapErase(7,0);
            buf[16] =botelv_fig;
            write_add(7,0,botelv_fig);
            delaya(50);
#if DEBUG_EN == 1
            com_data_tx(0x51);
            com_data_tx(read_add(7, 16));
#endif

        }
    }
    break;
    case 0x12:
    {
        if(ipdz!=rp[3])
        {
            ipdz=rp[3];
            read_ee_data(7,0);
            IapErase(7,0);
            buf[8] =rp[3];


            write_add(7,0,rp[3]);
            delaya(50);
#if DEBUG_EN == 1
            com_data_tx(0x52);
            com_data_tx(read_add(7, 8));
#endif

        }
        yzbl=rp[4];
        jsll=rp[5];
        gm_tt=rp[6]&0x80;
        gzdu=rp[6]&0x7f;
        if(xf_fig!=rp[7])
        {
            xf_fig=rp[7];
            read_ee_data(7,0);
            IapErase(7,0);
            buf[15] =xf_fig;

            write_add(7,0,xf_fig);
            delaya(50);
#if DEBUG_EN == 1
            com_data_tx(0x53);
            com_data_tx(read_add(7, 15));
#endif

        }
        if(botelv_fig!=rp[8])
        {
            botelv_fig=rp[8];
            read_ee_data(7,0);
            IapErase(7,0);
            buf[16] =botelv_fig;

            write_add(7,0,botelv_fig);
            delaya(50);
#if DEBUG_EN == 1
            com_data_tx(0x54);
            com_data_tx(read_add(7, 16));
#endif

        }
    }
    break;
    default:
        break;
    }
}

void rlrc_shuc(void)
{
    xzsfm=tchr_int(shi,fen);
    rlsfk=tchr_int(rils,rilf);
    rcsfg=tchr_int(rics,ricf);
    if(xzsfm==rlsfk)
    {
        if(rlk_bz==0)
        {
            if(rl_la)scha=1;
            if(rl_lb)schb=1;
            if(rl_lc)schc=1;
            if(rl_ld)schd=1;
            if(rl_le)sche=1;
            if(rl_lf)schf=1;
            if(rl_lg)schg=1;
            if(rl_lh)schh=1;
            if(rl_ha)scla=1;
            if(rl_hb)sclb=1;
            if(rl_hc)sclc=1;
            if(rl_hd)scld=1;
            if(rl_he)scle=1;
            if(rl_hf)sclf=1;
            if(rl_hg)sclg=1;
            if(rl_hh)sclh=1;
            rlk_bz=1;
            sc_bz=1;
        }
    }
    else rlk_bz=0;
    if(xzsfm==rcsfg)
    {
        if(rcg_bz==0)
        {
            if(rc_la)scha=0;
            if(rc_lb)schb=0;
            if(rc_lc)schc=0;
            if(rc_ld)schd=0;
            if(rc_le)sche=0;
            if(rc_lf)schf=0;
            if(rc_lg)schg=0;
            if(rc_lh)schh=0;
            if(rc_ha)scla=0;
            if(rc_hb)sclb=0;
            if(rc_hc)sclc=0;
            if(rc_hd)scld=0;
            if(rc_he)scle=0;
            if(rc_hf)sclf=0;
            if(rc_hg)sclg=0;
            if(rc_hh)sclh=0;
            rcg_bz=1;
            sc_bz=1;
        }
    }
    else rcg_bz=0;
}
//ls=0-15,djd=0-3

/*
unsigned char  TestD[8]=
{
	0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,
};
*/

void kgsc_cl(uchar ls,djd)
{
//	uchar i;

    shifk=tchr_int(dsf[(ls<<4)+djd*4+0],dsf[(ls<<4)+djd*4+1]);
    shifg=tchr_int(dsf[(ls<<4)+djd*4+2],dsf[(ls<<4)+djd*4+3]);

    if(shifk!=shifg)
    {
        if(xzsfm==shifk)
        {
            if(miao<=6)
            {
                EA=0;
                //for(i=0;i<3;i++)
                {
                    SBUF=0x08;
                    while(TI==0) {;}
                    TI=0;
                }
                EA=1;


                txcx=0;
                tx_tt=0;
                if(ls==0)
                {
                    if(dshka==0)
                    {
                        dshka=1;
                        if(scha==0)
                        {
                            scha=1;
                            sc_bz=1;
                            if(dingshi_fig)
                            {
                                switch(xhao)
                                {
                                case 1:
                                    yzbl=0xF0;
                                    break;
                                case 2:
                                    yzbl=0xFF;
                                    break;
                                case 3:
                                    yzbl=0xFF;
                                    jsll=0xf0;
                                    break;
                                case 4:
                                    yzbl=0xFF;
                                    jsll=0xff;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
                else if(ls==1)
                {
                    if(dshkb==0)
                    {
                        dshkb=1;
                        if(schb==0)
                        {
                            schb=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==2)
                {
                    if(dshkc==0)
                    {
                        dshkc=1;
                        if(schc==0)
                        {
                            schc=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==3)
                {
                    if(dshkd==0)
                    {
                        dshkd=1;
                        if(schd==0)
                        {
                            schd=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==4)
                {
                    if(dshke==0)
                    {
                        dshke=1;
                        if(sche==0)
                        {
                            sche=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==5)
                {
                    if(dshkf==0)
                    {
                        dshkf=1;
                        if(schf==0)
                        {
                            schf=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==6)
                {
                    if(dshkg==0)
                    {
                        dshkg=1;
                        if(schg==0)
                        {
                            schg=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==7)
                {
                    if(dshkh==0)
                    {
                        dshkh=1;
                        if(schh==0)
                        {
                            schh=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==8)
                {
                    if(dslka==0)
                    {
                        dslka=1;
                        if(scla==0)
                        {
                            scla=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==9)
                {
                    if(dslkb==0)
                    {
                        dslkb=1;
                        if(sclb==0)
                        {
                            sclb=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==10)
                {
                    if(dslkc==0)
                    {
                        dslkc=1;
                        if(sclc==0)
                        {
                            sclc=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==11)
                {
                    if(dslkd==0)
                    {
                        dslkd=1;
                        if(scld==0)
                        {
                            scld=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==12)
                {
                    if(dslke==0)
                    {
                        dslke=1;
                        if(scle==0)
                        {
                            scle=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==13)
                {
                    if(dslkf==0)
                    {
                        dslkf=1;
                        if(sclf==0)
                        {
                            sclf=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==14)
                {
                    if(dslkg==0)
                    {
                        dslkg=1;
                        if(sclg==0)
                        {
                            sclg=1;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==15)
                {
                    if(dslkh==0)
                    {
                        dslkh=1;
                        if(sclh==0)
                        {
                            sclh=1;
                            sc_bz=1;
                        }
                    }
                }
            }
        }
        else
        {
            dskh=0;
            dskl=0;
        }

        if(xzsfm==shifg)
        {
            if(miao<=6)
            {
                EA=0;
                //for(i=0;i<3;i++)
                {
                    SBUF=0x08;
                    while(TI==0) {;}
                    TI=0;
                }
                EA=1;


                txcx=0;
                tx_tt=0;
                if(ls==0)
                {
                    if((dsgh&0x01)==0)
                    {
                        dsgh=dsgh+0x01;
                        if(scha)
                        {
                            scha=0;
                            sc_bz=1;
                            if(dingshi_fig)
                            {
                                yzbl=0x00;
                                jsll=0x00;
                            }
                        }
                    }
                }
                else if(ls==1)
                {
                    if((dsgh&0x02)==0)
                    {
                        dsgh=dsgh+0x02;
                        if(schb)
                        {
                            schb=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==2)
                {
                    if((dsgh&0x04)==0)
                    {
                        dsgh=dsgh+0x04;
                        if(schc)
                        {
                            schc=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==3)
                {
                    if((dsgh&0x08)==0)
                    {
                        dsgh=dsgh+0x08;
                        if(schd)
                        {
                            schd=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==4)
                {
                    if((dsgh&0x10)==0)
                    {
                        dsgh=dsgh+0x10;
                        if(sche)
                        {
                            sche=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==5)
                {
                    if((dsgh&0x20)==0)
                    {
                        dsgh=dsgh+0x20;
                        if(schf)
                        {
                            schf=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==6)
                {
                    if((dsgh&0x40)==0)
                    {
                        dsgh=dsgh+0x40;
                        if(schg)
                        {
                            schg=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==7)
                {
                    if((dsgh&0x80)==0)
                    {
                        dsgh=dsgh+0x80;
                        if(schh)
                        {
                            schh=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==8)
                {
                    if((dsgl&0x01)==0)
                    {
                        dsgl=dsgl+0x01;
                        if(scla)
                        {
                            scla=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==9)
                {
                    if((dsgl&0x02)==0)
                    {
                        dsgl=dsgl+0x02;
                        if(sclb)
                        {
                            sclb=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==10)
                {
                    if((dsgl&0x04)==0)
                    {
                        dsgl=dsgl+0x04;
                        if(sclc)
                        {
                            sclc=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==11)
                {
                    if((dsgl&0x08)==0)
                    {
                        dsgl=dsgl+0x08;
                        if(scld)
                        {
                            scld=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==12)
                {
                    if((dsgl&0x10)==0)
                    {
                        dsgl=dsgl+0x10;
                        if(scle)
                        {
                            scle=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==13)
                {
                    if((dsgl&0x20)==0)
                    {
                        dsgl=dsgl+0x20;
                        if(sclf)
                        {
                            sclf=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==14)
                {
                    if((dsgl&0x40)==0)
                    {
                        dsgl=dsgl+0x40;
                        if(sclg)
                        {
                            sclg=0;
                            sc_bz=1;
                        }
                    }
                }
                else if(ls==15)
                {
                    if((dsgl&0x80)==0)
                    {
                        dsgl=dsgl+0x80;
                        if(sclh)
                        {
                            sclh=0;
                            sc_bz=1;
                        }
                    }
                }
            }
        }
        else
        {
            dsgh=0;
            dsgl=0;
        }
    }
}
//ls=0-15路,djd=0-3 4段
void dshikg(uchar ls,djd)
{

    xqbz=jiar[ls];

    switch(zhou)
    {
    case 0x00://星期日
    {
        if(xq_tg)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x01://星期1
    {
        if(xq_ta)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x02://星期2
    {
        if(xq_tb)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x03://星期3
    {
        if(xq_tc)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x04://星期4
    {
        if(xq_td)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x05://星期5
    {
        if(xq_te)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    case 0x06://星期6
    {
        if(xq_tf)
        {
            kgsc_cl(ls,djd);
            if(djd==0)rlrc_shuc();		//日落开,日出关
        }
    }
    break;
    default:
        break;
    }
}
static unsigned char result;


//主函数开始
main()
{
    // pwr_init();
    //P_SW2 |= 0x80;	//扩展寄存器(XFR)访问使能

    //write_add(7 , 13,0x05);
    RTC_config();
   // RTC_INT_CONFIG();
init2:
    result = 0;


    CMP_config();
	INTCLKO=EX2;
    init_dat();
    init();
    time1_init();
    in_lcd();	//初始化LCD
    led_init();//LED1-8初始化关

    RTC_read();

   // com_data_tx(nian);

//    nian=YEAR;
//	yue=MONTH;
//	ri=DAY;
//	shi = HOUR;
//	fen = MIN;
//	miao = SEC;
    if(nian<0x22||yue<=0||yue>0x12||shi>=0x24||fen>=0x60||ri>0x31||ri<=0)
    {
        nian = 0x24;
		yue = 0x10;
		ri=0x31;
		shi = 0x10;
		fen = 0x27;
		miao = 0x00;
        SetRTC();
    }



    EA=1;
    tx_tt=1;
    mbdz=0;
    tpi=0;
    while(1)
    {
        //if(B_1s)
        {
            B_1s=0;
            //RTC_read();
           // com_data_tx(miao);
            Ext_Vcc_Det();

        }

        if(result == 1)
        {
            result = 0;
            goto init2;
        }



        // else
        {
            //com_data_tx(0x78);
            if(rlrc_p==0)
            {
                rcrljs_date();
                bany_deng();
            }
            lcd_disp(); 		//LCD显示
            if(tx_tt)	//查询状态
            {
                if(cs_tt==0)chax_com();
                tx_tt=0;
            }
            key_in();	//按键输入
            key_kP2();

            if(cs_tt==0)
            {
                for(dsls=0; dsls<(xhao*4); dsls++)	//定时处理
                {
                    for(dssd=0; dssd<4; dssd++)
                    {
                        dshikg(dsls,dssd);
                    }
                }
            }
            if(sc_bz)
            {
                kzhi_com();
                sc_bz=0;
                cf_bz=1;
                cfjs=0;
            }

        }


//		WDT_CONTR=0x3c;		//看门狗
    }
}
//串口中断
void ser() interrupt 4
{
    uchar i,crc;
    rp[tpi]=SBUF;
    RI=0;
    if(tpi>=9)
    {
        crc=0;
        for(i=0; i<9; i++)crc=crc+rp[i];
        if(crc==rp[9])
        {
            txbz=rp[2];
            jies_data();		//接收处理
        }
        tpi=0;
    }
    else
    {
        if(rp[0]==0xfa)tpi++;
        else
        {
            if((rp[0]&0xfc)==0xfc)mbdz=rp[0]&0x03;
            tpi=0;
        }
    }
}
void  time1_isr() interrupt 3   //1ms
{
    Led_display();//Led1-8  9-16

}
//定时器0
void surt()interrupt 1
{
    wsms++;		//50MS
    if(wsms>9)
    {
        wsms=0;
        led_beiguang++;
        if(led_beiguang>=120)//60S后关 显示屏背光
        {
            led_beiguang=0;
            led=0;
        }
        if(cs_tt)key_time--;
        if(key_time==0)
        {
            key_time=100;
            cs_tt=0;//退出设置
            yiwei=0;
            mima=0;
            cans=0;
        }
        ss_tt=~ss_tt;
        if(cs_tt==0)RTC_read();
        if(ss_tt==0)
        {
            if(cf_bz)
            {
                if(cfjs>9)
                {
                    yzbl=yzblb;
                    jsll=jsllb;
                    kzhi_com();
                    cfjs=0;
                    cf_bz=0;
                }
                else
                {
                    cfjs++;
                    if(cfjs==5)
                    {
                        yzbl=yzblb;
                        jsll=jsllb;
                        kzhi_com();
                    }
                }
            }
            else
            {
                txcx++;
                if(txcx>=7)
                {
                    txcx=0;
                    if(sc_bz==0)tx_tt=1;
                }
                xsqh++;
                if(xsqh>=10)
                {
                    xsqh=0;
                    xian++;
                    if(xian>xhao)xian=0;
                    if(cs_tt==0)yiwei=0;
                }
            }
        }
        if(txbz==1)
        {
            sdfw++;
            if(sdfw>3)
            {
                sdfw=0;
                txbz=0;
            }
        }
        else sdfw=0;
    }
}




























