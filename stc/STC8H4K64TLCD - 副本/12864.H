//LCD接口
sbit CS1=P1^0;	//芯片选通端,低有效
sbit RST=P1^1;
sbit A0 =P4^7;	//H:数据,L:命令
sbit SCL=P1^4;	//串行时钟端
sbit SI =P1^5;	//串行数据端

sbit led1=P3^4;
sbit led2=P5^0;
sbit led3=P5^1;
sbit led4=P6^6;
sbit led5=P3^3;
sbit led6=P3^2;
sbit led7=P6^7;
sbit led8=P6^5;

sbit led9=P6^3;
sbit led10=P6^2;
sbit led11=P6^1;
sbit led12=P6^0;
sbit led13=P0^7;
sbit led14=P0^6;
sbit led15=P0^5;
sbit led16=P5^3;

void lcd_pwr()
{
    CS1=1;	//芯片选通端,低有效
    RST=0;
    A0 =0;	//H:数据,L:命令
    SCL=0;	//串行时钟端
    SI =0;	//串行数据端

}
void led_init()
{
    led1=0;
    led2=0;
    led3=0;
    led4=0;
    led5=0;
    led6=0;
    led6=0;
    led8=0;

    led9=0;
    led10=0;
    led11=0;
    led12=0;
    led13=0;
    led14=0;
    led15=0;
    led16=0;
}
void sleep()
{
    led9=0;
    led10=0;
    led11=0;
    led12=0;
    led13=0;
    led14=0;
    led15=0;
    led16=0;

}
void Led_display()
{
    led1=~scha;
    led2=~schb;
    led3=~schc;
    led4=~schd;
    led5=~sche;
    led6=~schf;
    led7=~schg;
    led8=~schh;

    led9=scla;
    led10=sclb;
    led11=sclc;
    led12=scld;
    led13=scle;
    led14=sclf;
    led15=sclg;
    led16=sclh;
}

/*************************************/
//串行模式发送数据
void data_send(uchar dat)
{
    uchar i;
    for(i=0; i<8; i++)
    {
        SCL=0;
        delay(2);
        dat=dat<<1;
        if(CY)SI=1;
        else SI=0;
        delay(2);
        SCL=1;
        delay(2);
    }
}
//写命令,串行模式
void w_com(uchar x)
{
    CS1=0;
    A0=0;
    data_send(x);
    A0=1;
    CS1=1;
}

//写数据,串行模式
void w_data(uchar dat)
{
    CS1=0;
    data_send(dat);
    CS1=1;
}

//初始化LCD
void in_lcd(void)
{
    RST=0;			//初始化LCD
    delaya(5);
    RST=1;			//初始化LCD

    w_com(0x2f);	//设置内部电路的电源
    w_com(0x23);	//设置LCD对比度0x00-0x27
    w_com(0x81);	//调节LCD的亮度命令
    w_com(0x2f);	//写入设定值0x00-0x3f
    w_com(0xa2);	//偏压设置(0xa2是1/9,0xa3是1/7)
    w_com(0xa0);	//显示设置(0xa0是正常,0xa1是反向)
    w_com(0xc8);	//显示设置(0xc8是正常,0xc0是反向)
    w_com(0x10);	//列地址,高4位
    w_com(0x00);	//列地址,低4位,两次写入,从第0>>131自动+1
    w_com(0xb0);	//显示页设置(0xb0>>0xb7)
    w_com(0xaf);	//显示开(0xaf开,0xae关)
    w_com(0xa6);	//显示设置(0xa6是正常,0xa7是反白)
}
//在指定的地址显示数字
void lcd_disp_s(uchar ye,lie,zf)
{
    uchar i,j,k,y,z;
    z=zf*16;
    if(ye==0)i=4;
    else if(ye==1)i=6;
    else if(ye==2)i=0;
    else if(ye==3)i=2;
    lie=lie*8;
    j=lie>>4;
    k=lie&0x0f;
    w_com(0x60);
    w_com(0xb0+i);
    w_com(0x10+j);
    w_com(k);
    for(y=0; y<8; y++)
    {
        w_data(shuz[z+y]);
    }
    w_com(0xb0+i+1);
    w_com(0x10+j);
    w_com(k);
    for(y=8; y<16; y++)
    {
        w_data(shuz[z+y]);
    }
}

//在指定的地址显示字符
void lcd_disp_e(uchar ye,lie,zf)
{
    uchar i,j,k,y;
    uint z;
    z=zf;
    z=z*16;
    if(ye==0)i=4;
    else if(ye==1)i=6;
    else if(ye==2)i=0;
    else if(ye==3)i=2;
    lie=lie*8;
    j=lie>>4;
    k=lie&0x0f;
    w_com(0x60);
    w_com(0xb0+i);
    w_com(0x10+j);
    w_com(k);
    for(y=0; y<8; y++)
    {
        w_data(zhif[z+y]);
    }
    w_com(0xb0+i+1);
    w_com(0x10+j);
    w_com(k);
    for(y=8; y<16; y++)
    {
        w_data(zhif[z+y]);
    }
}

//在指定的地址显示一个汉字
void lcd_disp_h(uchar ye,lie,zf)
{
    uchar i,j=0,k=0,y;
    uint z;
    z=zf;
    z=z*32;
    if(ye==0)i=4;
    else if(ye==1)i=6;
    else if(ye==2)i=0;
    else if(ye==3)i=2;
    lie=lie*16;
    j=lie>>4;
    k=lie&0x0f;
    w_com(0x60);
    w_com(0xb0+i);
    w_com(0x10+j);
    w_com(k);
    for(y=0; y<16; y++)
    {
        w_data(hanzi[z+y]);
    }
    w_com(0xb0+i+1);
    w_com(0x10+j);
    w_com(k);
    for(y=16; y<32; y++)
    {
        w_data(hanzi[z+y]);
    }
}

//在指定的地址显示一个汉字
void lcd_disp_d(uchar lie,zf)
{
    uchar j=0,k=0,y,le;
    uint z;
    z=zf;
    z=z*128;
    le=lie*32;
    j=le>>4;
    k=le&0x0f;
    w_com(0x60);
    w_com(0xb6);
    w_com(0x10+j);
    w_com(k);
    for(y=0; y<32; y++)
    {
        w_data(deng[z+y]);
    }
    w_com(0xb7);
    w_com(0x10+j);
    w_com(k);
    for(y=32; y<64; y++)
    {
        w_data(deng[z+y]);
    }
    le=lie*32;
    j=le>>4;
    k=le&0x0f;
    w_com(0x60);
    w_com(0xb0);
    w_com(0x10+j);
    w_com(k);
    for(y=64; y<96; y++)
    {
        w_data(deng[z+y]);
    }
    w_com(0xb1);
    w_com(0x10+j);
    w_com(k);
    for(y=96; y<128; y++)
    {
        w_data(deng[z+y]);
    }
}

void day_convert(uchar add)
{
    lcd_disp_h(add+1, 0,9);	//星
    lcd_disp_h(add+1, 1,10);//期

    switch(zhou)
    {
    case 0x01:
        lcd_disp_e(add+1,0,38);//M
        lcd_disp_e(add+1,0,14);//o
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x02:
        lcd_disp_e(add+1,0,45);//Thesday
        lcd_disp_e(add+1,0,7);//h
        lcd_disp_e(add+1,0,4);//e
        lcd_disp_e(add+1,0,18);//s       
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x03:
        lcd_disp_e(add+1,0,48);//Wednesday
        lcd_disp_e(add+1,0,4);//e
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,4);//e
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x04:
        lcd_disp_e(add+1,0,45);//Thursday
        lcd_disp_e(add+1,0,7);//h
        lcd_disp_e(add+1,0,20);//u
         lcd_disp_e(add+1,0,17);//r
        lcd_disp_e(add+1,0,18);//s
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x05:
        lcd_disp_e(add+1,0,38);//Friday
        lcd_disp_e(add+1,0,14);//o
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x06:
        lcd_disp_e(add+1,0,38);//Saturday
        lcd_disp_e(add+1,0,14);//o
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    case 0x07:
        lcd_disp_e(add+1,0,38);//Sunday
        lcd_disp_e(add+1,0,14);//o
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;
    derault:
        lcd_disp_e(add+1,0,38);//M
        lcd_disp_e(add+1,0,14);//o
        lcd_disp_e(add+1,0,13);//n
        lcd_disp_e(add+1,0,3);//d
        lcd_disp_e(add+1,0,0);//a
        lcd_disp_e(add+1,0,24);//y
        break;



    }
}
void rqsj_disp(uchar add)
{
    uchar i;
    lcd_disp_s(add, 0,2);	//1
    lcd_disp_s(add, 1,0);	//8
    if(yiwei==1&ss_tt)lcd_disp_e(add, 2,54);//
    else lcd_disp_s(add, 2,nian>>4);		//1
    if(yiwei==2&ss_tt)lcd_disp_e(add, 3,54);//
    else lcd_disp_s(add, 3,nian&0x0f);		//8
    lcd_disp_h(add, 2,0);	//年 45
    lcd_disp_e(add, 6,54);//
    lcd_disp_e(add, 7,54);//
    if(yiwei==3&ss_tt)
    {
        lcd_disp_e(add, 8,54);//
        lcd_disp_e(add, 9,54);//
    }
    else
    {
        lcd_disp_s(add, 8,yue>>4);	//0
        lcd_disp_s(add, 9,yue&0x0f);	//7
    }
    lcd_disp_h(add, 5,8);	//月  89
    if(yiwei==4&ss_tt)
    {
        lcd_disp_e(add,12,54);//
        lcd_disp_e(add,13,54);//
    }
    else
    {
        lcd_disp_s(add,12,ri>>4);	//0
        lcd_disp_s(add,13,ri&0x0f);	//2
    }
    lcd_disp_h(add, 7,7);	//日


   day_convert(zhou);

//    lcd_disp_h(add+1, 0,9);	//星
//    lcd_disp_h(add+1, 1,10);//期
//    if(yiwei==5&ss_tt)
//    {
//        lcd_disp_e(add+1, 4,54);//
//        lcd_disp_e(add+1, 5,54);//
//    }
//    else
//    {
//        i=zhou&0x07;
//        if(i==0)i=7;
//        lcd_disp_h(add+1, 2,i); //星期
//    }
    lcd_disp_e(add+1, 6,54);//
    lcd_disp_e(add+1, 7,54);//
    if(yiwei==6&ss_tt)
    {
        lcd_disp_e(add+1, 8,54);//
        lcd_disp_e(add+1, 9,54);//
    }
    else
    {
        lcd_disp_s(add+1, 8,shi>>4);	//1
        lcd_disp_s(add+1, 9,shi&0x0f);	//5
    }
    lcd_disp_e(add+1,10,52);//:
    if(yiwei==7&ss_tt)lcd_disp_e(add+1,11,54);//
    else lcd_disp_s(add+1,11,fen>>4);	//1
    if(yiwei==8&ss_tt)lcd_disp_e(add+1,12,54);//
    else lcd_disp_s(add+1,12,fen&0x0f);	//9
    lcd_disp_e(add+1,13,52);//:
    if(yiwei==9&ss_tt)lcd_disp_e(add+1,14,54);//
    else lcd_disp_s(add+1,14,miao>>4);	//6
    if(yiwei==10&ss_tt)lcd_disp_e(add+1,15,54);//
    else lcd_disp_s(add+1,15,miao&0x0f);	//7
}
void ricril_disp(uchar add)
{
    lcd_disp_h(add, 0, 7);	//日
    lcd_disp_h(add, 1,11);	//出
    lcd_disp_e(add, 4,52);//:
    lcd_disp_e(add, 5,54);//
    lcd_disp_e(add, 6,54);//
    lcd_disp_e(add, 7,54);//
    lcd_disp_s(add, 8,rics>>4);	//
    lcd_disp_s(add, 9,rics&0x0f);	//
    lcd_disp_e(add,10,52);//:
    lcd_disp_s(add,11,ricf>>4);	//
    lcd_disp_s(add,12,ricf&0x0f);	//
    lcd_disp_e(add,13,54);//
    lcd_disp_e(add,14,54);//
    lcd_disp_e(add,15,54);//

    lcd_disp_h(add+1, 0, 7);	//日
    lcd_disp_h(add+1, 1,12);	//落
    lcd_disp_e(add+1, 4,52);//:
    lcd_disp_e(add+1, 5,54);//
    lcd_disp_e(add+1, 6,54);//
    lcd_disp_e(add+1, 7,54);//
    lcd_disp_s(add+1, 8,rils>>4);	//
    lcd_disp_s(add+1, 9,rils&0x0f);	//
    lcd_disp_e(add+1,10,52);//:
    lcd_disp_s(add+1,11,rilf>>4);	//
    lcd_disp_s(add+1,12,rilf&0x0f);	//
    lcd_disp_e(add+1,13,54);//
    lcd_disp_e(add+1,14,54);//
    lcd_disp_e(add+1,15,54);//
}
//经维度
void jwdsq_disp(void)
{
    if(yiwei==1&ss_tt)
    {
        lcd_disp_e(1, 0,54);
        lcd_disp_e(1, 1,54);
        lcd_disp_e(1, 2,54);
        lcd_disp_e(1, 3,54);
    }
    else
    {
        if(jx_tt)lcd_disp_h(1, 0,17);	//西
        else lcd_disp_h(1, 0,16);		//东
        lcd_disp_h(1, 1,13);			//经
    }
    lcd_disp_e(1, 4,54);
    lcd_disp_e(1, 5,54);
    lcd_disp_e(1, 6,54);
    lcd_disp_e(1, 7,54);
    lcd_disp_e(1, 8,54);
    if(yiwei==2&ss_tt)
    {
        lcd_disp_e(1, 9,54);
        lcd_disp_e(1,10,54);
    }
    else
    {
        lcd_disp_s(1, 9,jingd/100);
        lcd_disp_s(1,10,jingd%100/10);
    }
    if(yiwei==3&ss_tt)lcd_disp_e(1,11,54);
    else lcd_disp_s(1,11,jingd%10);
    lcd_disp_e(1,12,55);
    if(yiwei==4&ss_tt)lcd_disp_e(1,13,54);
    else lcd_disp_s(1,13,jingf/10);
    if(yiwei==5&ss_tt)lcd_disp_e(1,14,54);
    else lcd_disp_s(1,14,jingf%10);
    lcd_disp_e(1,15,56);

    if(yiwei==6&ss_tt)
    {
        lcd_disp_e(2, 0,54);
        lcd_disp_e(2, 1,54);
        lcd_disp_e(2, 2,54);
        lcd_disp_e(2, 3,54);
    }
    else
    {
        if(wx_tt)lcd_disp_h(2, 0,19);	//南
        else lcd_disp_h(2, 0,18);		//北
        lcd_disp_h(2, 1,14);			//维
    }
    lcd_disp_e(2, 4,54);
    lcd_disp_e(2, 5,54);
    lcd_disp_e(2, 6,54);
    lcd_disp_e(2, 7,54);
    lcd_disp_e(2, 8,54);
    lcd_disp_e(2, 9,54);
    if(yiwei==7&ss_tt)lcd_disp_e(2,10,54);
    else lcd_disp_s(2,10,weidd/10);
    if(yiwei==8&ss_tt)lcd_disp_e(2,11,54);
    else lcd_disp_s(2,11,weidd%10);
    lcd_disp_e(2,12,55);//:
    if(yiwei==9&ss_tt)lcd_disp_e(2,13,54);
    else lcd_disp_s(2,13,weidf/10);
    if(yiwei==10&ss_tt)lcd_disp_e(2,14,54);
    else lcd_disp_s(2,14,weidf%10);
    lcd_disp_e(2,15,56);
    if(yiwei==11&ss_tt)
    {
        lcd_disp_e(3, 0,54);
        lcd_disp_e(3, 1,54);
        lcd_disp_e(3, 2,54);
        lcd_disp_e(3, 3,54);
        lcd_disp_e(3, 4,54);
        lcd_disp_e(3, 5,54);
    }
    else
    {
        if(sq_tt)lcd_disp_h(3, 0,17);	//西
        else lcd_disp_h(3, 0,16);		//东
        lcd_disp_h(3, 1,20);			//时
        lcd_disp_h(3, 2,21);			//区
    }
    lcd_disp_e(3, 6,54);
    lcd_disp_e(3, 7,54);
    lcd_disp_e(3, 8,54);
    lcd_disp_e(3, 9,54);
    if(yiwei==12&ss_tt)
    {
        lcd_disp_e(3,10,54);
        lcd_disp_e(3,11,54);
    }
    else
    {
        lcd_disp_s(3,10,shiqz/10);
        lcd_disp_s(3,11,shiqz%10);
    }
    lcd_disp_e(3,12,55);
    if(yiwei==13&ss_tt)lcd_disp_e(3,13,54);
    else lcd_disp_s(3,13,shiqf/10);
    if(yiwei==14&ss_tt)lcd_disp_e(3,14,54);
    else lcd_disp_s(3,14,shiqf%10);
    lcd_disp_e(3,15,56);
}
void shida_disp(uchar ls,shik,fenk,shig,feng)
{
    if(ss_tt&yiwei==3)
    {
        lcd_disp_e(3, 0,54);
        lcd_disp_e(3, 1,54);
    }
    else
    {
        lcd_disp_s(3, 0,shik>>4);
        lcd_disp_s(3, 1,shik&0x0f);
    }
    lcd_disp_e(3, 2,52);					//:
    if(ss_tt&yiwei==4)lcd_disp_e(3, 3,54);
    else lcd_disp_s(3, 3,fenk>>4);
    if(ss_tt&yiwei==5)lcd_disp_e(3, 4,54);
    else lcd_disp_s(3, 4,fenk&0x0f);
    if(ss_tt&yiwei==6)lcd_disp_e(3, 5,54);
    else
    {
        if(ls==0x01)
        {
            if(rl_la)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x02)
        {
            if(rl_lb)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x03)
        {
            if(rl_lc)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x04)
        {
            if(rl_ld)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x05)
        {
            if(rl_le)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x06)
        {
            if(rl_lf)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x07)
        {
            if(rl_lg)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x08)
        {
            if(rl_lh)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x09)
        {
            if(rl_ha)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0a)
        {
            if(rl_hb)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0b)
        {
            if(rl_hc)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0c)
        {
            if(rl_hd)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0d)
        {
            if(rl_he)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0e)
        {
            if(rl_hf)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x0f)
        {
            if(rl_hg)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
        else if(ls==0x10)
        {
            if(rl_hh)lcd_disp_e(3,5,58);
            else lcd_disp_e(3,5,57);
        }
    }
    lcd_disp_h(3,3,36);		//开
    if(ss_tt&yiwei==7)
    {
        lcd_disp_e(3, 8,54);
        lcd_disp_e(3, 9,54);
    }
    else
    {
        lcd_disp_s(3, 8,shig>>4);
        lcd_disp_s(3, 9,shig&0x0f);
    }
    lcd_disp_e(3,10,52);					//:
    if(ss_tt&yiwei==8)lcd_disp_e(3,11,54);
    else lcd_disp_s(3,11,feng>>4);
    if(ss_tt&yiwei==9)lcd_disp_e(3,12,54);
    else lcd_disp_s(3,12,feng&0x0f);
    if(ss_tt&yiwei==10)lcd_disp_e(3,13,54);
    else
    {
        if(ls==0x01)
        {
            if(rc_la)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x02)
        {
            if(rc_lb)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x03)
        {
            if(rc_lc)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x04)
        {
            if(rc_ld)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x05)
        {
            if(rc_le)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x06)
        {
            if(rc_lf)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x07)
        {
            if(rc_lg)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x08)
        {
            if(rc_lh)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x09)
        {
            if(rc_ha)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0a)
        {
            if(rc_hb)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0b)
        {
            if(rc_hc)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0c)
        {
            if(rc_hd)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0d)
        {
            if(rc_he)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0e)
        {
            if(rc_hf)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x0f)
        {
            if(rc_hg)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
        else if(ls==0x10)
        {
            if(rc_hh)lcd_disp_e(3,13,58);
            else lcd_disp_e(3,13,57);
        }
    }
    lcd_disp_h(3,7,37);		//关
}

void shidb_disp(uchar shik,fenk,shig,feng)
{
    if(ss_tt&yiwei==3)
    {
        lcd_disp_e(3, 0,54);
        lcd_disp_e(3, 1,54);
    }
    else
    {
        lcd_disp_s(3, 0,shik>>4);
        lcd_disp_s(3, 1,shik&0x0f);
    }
    lcd_disp_e(3, 2,52);					//:
    if(ss_tt&yiwei==4)lcd_disp_e(3, 3,54);
    else lcd_disp_s(3, 3,fenk>>4);
    if(ss_tt&yiwei==5)lcd_disp_e(3, 4,54);
    else lcd_disp_s(3, 4,fenk&0x0f);
    lcd_disp_e(3,5,57);		//=
    lcd_disp_h(3,3,36);		//开
    if(ss_tt&yiwei==6)
    {
        lcd_disp_e(3, 8,54);
        lcd_disp_e(3, 9,54);
    }
    else
    {
        lcd_disp_s(3, 8,shig>>4);
        lcd_disp_s(3, 9,shig&0x0f);
    }
    lcd_disp_e(3,10,52);					//:
    if(ss_tt&yiwei==7)lcd_disp_e(3,11,54);
    else lcd_disp_s(3,11,feng>>4);
    if(ss_tt&yiwei==8)lcd_disp_e(3,12,54);
    else lcd_disp_s(3,12,feng&0x0f);
    lcd_disp_e(3,13,57);	//=
    lcd_disp_h(3,7,37);		//关
}
void jieri_disp()
{
    if(ss_tt&yiwei==2)lcd_disp_h(3,1,41);
    else
    {
        if(xq_ta)lcd_disp_h(3,1,39);//
        else lcd_disp_h(3,1,40);	//
    }
    if(ss_tt&yiwei==3)lcd_disp_h(3,2,41);
    else
    {
        if(xq_tb)lcd_disp_h(3,2,39);//
        else lcd_disp_h(3,2,40);	//
    }
    if(ss_tt&yiwei==4)lcd_disp_h(3,3,41);
    else
    {
        if(xq_tc)lcd_disp_h(3,3,39);//
        else lcd_disp_h(3,3,40);	//
    }
    if(ss_tt&yiwei==5)lcd_disp_h(3,4,41);
    else
    {
        if(xq_td)lcd_disp_h(3,4,39);//
        else lcd_disp_h(3,4,40);	//
    }
    if(ss_tt&yiwei==6)lcd_disp_h(3,5,41);
    else
    {
        if(xq_te)lcd_disp_h(3,5,39);//
        else lcd_disp_h(3,5,40);	//
    }
    if(ss_tt&yiwei==7)lcd_disp_h(3,6,41);
    else
    {
        if(xq_tf)lcd_disp_h(3,6,39);//
        else lcd_disp_h(3,6,40);	//
    }
    if(ss_tt&yiwei==8)lcd_disp_h(3,7,41);
    else
    {
        if(xq_tg)lcd_disp_h(3,7,39);//
        else lcd_disp_h(3,7,40);	//
    }
}

void gmkg_disp(bit kg_zt)
{
    if(ss_tt&yiwei==2)lcd_disp_h(2,3,41);
    else
    {
        if(kg_zt)lcd_disp_h(2,3,39);//
        else lcd_disp_h(2,3,40);	//
    }
}
void gmdt_disp(uchar dea,deb)
{
    lcd_disp_e(3, 0,54);
    lcd_disp_e(3, 1,59);
    if(ss_tt&yiwei==3)
    {
        lcd_disp_e(3, 2,54);
        lcd_disp_e(3, 3,54);
    }
    else
    {
        lcd_disp_s(3, 2,dea/10);
        lcd_disp_s(3, 3,dea%10);
    }
    lcd_disp_h(3, 2,36);
    lcd_disp_e(3, 6,54);
    lcd_disp_e(3, 7,54);
    lcd_disp_e(3, 8,54);
    lcd_disp_e(3, 9,60);
    if(ss_tt&yiwei==4)
    {
        lcd_disp_e(3,10,54);
        lcd_disp_e(3,11,54);
    }
    else
    {
        lcd_disp_s(3,10,deb/10);
        lcd_disp_s(3,11,deb%10);
    }
    lcd_disp_h(3, 6,37);
    lcd_disp_e(3,14,54);
    lcd_disp_e(3,15,54);
}
void cans_dasp(void)
{
    uchar i;
    if(ss_tt&yiwei==0)
    {
        lcd_disp_e(0, 0,54);
        lcd_disp_e(0, 1,54);
    }
    else
    {
        lcd_disp_s(0, 0,cans/10);
        lcd_disp_s(0, 1,cans%10);
    }
    for(i=12; i<16; i++)lcd_disp_e(0,i,54);
    switch(cans)
    {
    case 0x00:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,22);	//输
        lcd_disp_h(0, 3,23);	//入
        lcd_disp_h(0, 4,24);	//密
        lcd_disp_h(0, 5,25);	//码

        for(i=0 ; i<12; i++)lcd_disp_e(1,i,54);

        if(ss_tt&yiwei==4)lcd_disp_e(1,12,54);
        else lcd_disp_s(1,12,mima/1000);

        if(ss_tt&yiwei==3)lcd_disp_e(1,13,54);
        else lcd_disp_s(1,13,mima%1000/100);

        if(ss_tt&yiwei==2)lcd_disp_e(1,14,54);
        else lcd_disp_s(1,14,mima%100/10);

        if(ss_tt&yiwei==1)lcd_disp_e(1,15,54);
        else lcd_disp_s(1,15,mima%10);
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(2,i,54);
            lcd_disp_e(3,i,54);
        }
    }
    break;
    case 0x01:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,20);	//时
        lcd_disp_h(0, 3,26);	//钟
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置
        rqsj_disp(1);			//日期时钟
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(3,i,54);
        }
    }
    break;
    case 0x02:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,13);	//经
        lcd_disp_h(0, 3,14);	//纬
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置
        jwdsq_disp();
    }
    break;
    case 0x03:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,29);	//通
        lcd_disp_h(0, 3,30);	//讯
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置
        lcd_disp_e(1, 0,34);	//I
        lcd_disp_e(1, 1,29);	//D
        lcd_disp_e(1, 2,52);	//:
        lcd_disp_e(1, 3,54);	//
        for(i=4 ; i<13; i++)lcd_disp_e(1,i,54);
        if(ss_tt&yiwei==1)lcd_disp_e(1,13,54);
        else lcd_disp_s(1,13,ipdz/100);

        if(ss_tt&yiwei==2)lcd_disp_e(1,14,54);
        else lcd_disp_s(1,14,ipdz%100/10);

        if(ss_tt&yiwei==3)lcd_disp_e(1,15,54);
        else lcd_disp_s(1,15,ipdz%10);
        lcd_disp_h(2, 0,43);	//智
        lcd_disp_h(2, 1,44);	//能
        lcd_disp_h(2, 2,20);	//时
        lcd_disp_h(2, 3,31);	//控
        lcd_disp_h(2, 4,45);	//器
        lcd_disp_e(2,10,52);	//:
        lcd_disp_e(2,11,54);	//
        if(ss_tt&yiwei==4)
        {
            lcd_disp_e(2,12,54);
            lcd_disp_e(2,13,54);
        }
        else
        {
            lcd_disp_s(2,12,(xhao*4)/10);
            lcd_disp_s(2,13,(xhao*4)%10);
        }
        lcd_disp_h(2, 7,34);	//路
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(3,i,54);
        }

    }
    break;
    case 0x04:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,20);	//时
        lcd_disp_h(0, 3,31);	//控
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置

        lcd_disp_h(1, 0,32);	//对
        lcd_disp_h(1, 1,33);	//象
        lcd_disp_e(1, 4,52);	//:
        lcd_disp_e(1, 5,54);	//

        if(ss_tt&yiwei==1)
        {
            lcd_disp_e(1, 6,54);
            lcd_disp_e(1, 7,54);
        }
        else
        {
            lcd_disp_s(1, 6,kgls/10);
            lcd_disp_s(1, 7,kgls%10);
        }
        lcd_disp_h(1, 4,34);	//路
        for(i=10 ; i<16; i++)lcd_disp_e(1,i,54);

        lcd_disp_h(2, 0,20);	//时
        lcd_disp_h(2, 1,35);	//段
        lcd_disp_e(2, 4,52);	//:
        lcd_disp_e(2, 5,54);	//

        if(ss_tt&yiwei==2)
        {
            lcd_disp_e(2, 6,54);
            lcd_disp_e(2, 7,54);
        }
        else
        {
            lcd_disp_s(2, 6,shid/10);
            lcd_disp_s(2, 7,shid%10);
        }
        lcd_disp_h(2, 4,35);	//段
        for(i=10 ; i<16; i++)lcd_disp_e(2,i,54);
        if(shid==1)shida_disp(kgls,kshia,kfena,gshia,gfena);
        else if(shid==2)shidb_disp(kshib,kfenb,gshib,gfenb);
        else if(shid==3)shidb_disp(kshic,kfenc,gshic,gfenc);
        else if(shid==4)shidb_disp(kshid,kfend,gshid,gfend);
    }
    break;
    case 0x05:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,38);	//假
        lcd_disp_h(0, 3, 7);	//日
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置

        lcd_disp_h(1, 0,32);	//对
        lcd_disp_h(1, 1,33);	//象
        lcd_disp_e(1, 4,52);	//:
        lcd_disp_e(1, 5,54);	//

        if(ss_tt&yiwei==1)
        {
            lcd_disp_e(1, 6,54);
            lcd_disp_e(1, 7,54);
        }
        else
        {
            lcd_disp_s(1, 6,kgls/10);
            lcd_disp_s(1, 7,kgls%10);
        }
        lcd_disp_h(1, 4,34);	//路
        for(i=10 ; i<16; i++)lcd_disp_e(1,i,54);
        lcd_disp_h(2, 0, 9);	//星期
        lcd_disp_h(2, 1, 1);	//1
        lcd_disp_h(2, 2, 2);	//2
        lcd_disp_h(2, 3, 3);	//3
        lcd_disp_h(2, 4, 4);	//4
        lcd_disp_h(2, 5, 5);	//5
        lcd_disp_h(2, 6, 6);	//6
        lcd_disp_h(2, 7, 7);	//7
        lcd_disp_h(3, 0,10);	//期
        jieri_disp();			//
    }
    break;
    case 0x06:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,42);	//光
        lcd_disp_h(0, 3,31);	//控
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置

        lcd_disp_h(1, 0,46);	//第
        if(ss_tt&yiwei==1)
        {
            lcd_disp_e(1, 2,54);
            lcd_disp_e(1, 3,54);
        }
        else
        {
            lcd_disp_s(1, 2,kgls/10);
            lcd_disp_s(1, 3,kgls%10);
        }
        lcd_disp_h(1, 2,34);	//路
        lcd_disp_e(1, 6,54);
        lcd_disp_e(1, 7,54);
        lcd_disp_h(1, 4,47);	//照
        lcd_disp_h(1, 5,15);	//度
        lcd_disp_e(1,12,52);	//:
        if(gm_tt)
        {
            lcd_disp_s(1,13,gzdu/100);
            lcd_disp_s(1,14,gzdu%100/10);
            lcd_disp_s(1,15,gzdu%10);
        }
        else
        {
            lcd_disp_e(1,13,53);
            lcd_disp_e(1,14,53);
            lcd_disp_e(1,15,53);
        }
        lcd_disp_h(2, 0,48);	//状
        lcd_disp_h(2, 1,49);	//态
        lcd_disp_e(2, 4,52);	//:
        lcd_disp_e(2, 5,54);	//
        gmkg_disp(gmkg[(kgls-1)*2]&0x80);
        for(i=8; i<16; i++)lcd_disp_e(2,i,54);
        gmdt_disp(gmkg[(kgls-1)*2]&0x7f,gmkg[(kgls-1)*2+1]);
    }
    break;
    case 0x07:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,50);	//消
        lcd_disp_h(0, 3,51);	//防
        lcd_disp_h(0, 4,27);	//设
        lcd_disp_h(0, 5,28);	//置
        for(i=0 ; i<12; i++)lcd_disp_e(1,i,54);
        if(ss_tt&yiwei==1)lcd_disp_e(1,10,54);
        else lcd_disp_s(1,10,xf_fig);
        lcd_disp_e(1, 11,52);	//:
        lcd_disp_h(1, 6,52);	//强
        if(xf_fig)lcd_disp_h(1, 7,53);	//启
        else lcd_disp_h(1, 7,54);//切
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(2,i,54);
            lcd_disp_e(3,i,54);
        }
        break;
    }
    case 0x08:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,55);	//波
        lcd_disp_h(0, 3,56);	//特
        lcd_disp_h(0, 4,57);	//率
        for(i=0 ; i<12; i++)lcd_disp_e(1,i,54);
        if(ss_tt&yiwei==1)lcd_disp_e(1,10,54);
        else lcd_disp_s(1,10,botelv_fig);
        lcd_disp_e(1, 11,52);	//:
        if(botelv_fig)
        {
            lcd_disp_s(1,12,9);//9
            lcd_disp_s(1,13,6);//6
        }
        else
        {
            lcd_disp_s(1,12,4);//4
            lcd_disp_s(1,13,8);//8
        }
        lcd_disp_s(1,14,0);//0
        lcd_disp_s(1,15,0);//0
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(2,i,54);
            lcd_disp_e(3,i,54);
        }
    }
    break;
    case 0x09:
    {
        lcd_disp_e(0, 2,52);
        lcd_disp_e(0, 3,54);
        lcd_disp_h(0, 2,20);	//时
        lcd_disp_h(0, 3,31);	//控
        lcd_disp_h(0, 4,58);	//方
        lcd_disp_h(0, 5,59);	//式
        for(i=0 ; i<6; i++)lcd_disp_e(1,i,54);
        if(ss_tt&yiwei==1)lcd_disp_e(1,6,54);
        else lcd_disp_s(1,6,dingshi_fig);
        lcd_disp_e(1, 7,52);	//:
        if(dingshi_fig)
        {
            lcd_disp_h(1, 4,60);	//随
            lcd_disp_h(1, 5,61);	//第
            lcd_disp_h(1, 6,62);	//一
            lcd_disp_h(1, 7,34);	//路
        }
        else
        {
            lcd_disp_h(1, 4,63);//自
            lcd_disp_h(1, 5,64);//定
            lcd_disp_h(1, 6,65);//义
            lcd_disp_e(1,14,54);
            lcd_disp_e(1,15,54);
        }
        for(i=0 ; i<16; i++)
        {
            lcd_disp_e(2,i,54);
            lcd_disp_e(3,i,54);
        }
        break;
    }
    default:
        break;
    }
}
void dekg_disp(uchar add,bit kg_bz)
{
    if(kg_bz)lcd_disp_d(add,0);	//开
    else lcd_disp_d(add,1);	//关
}
void deng_disp(uchar ksls)
{
    lcd_disp_h(0, 0,43);	//智
    lcd_disp_h(0, 1,44);	//能
    lcd_disp_h(0, 2,20);	//时
    lcd_disp_h(0, 3,31);	//控
    lcd_disp_h(0, 4,45);	//器
    if(txbz==1&ss_tt)		//通信闪动
    {
        lcd_disp_e(0,10,54);	//
        lcd_disp_e(0,11,54);	//
    }
    else
    {
        lcd_disp_e(0,10,34);	//I
        lcd_disp_e(0,11,29);	//D
    }
    lcd_disp_e(0,12,52);	//:
    lcd_disp_s(0,13,ipdz/100);
    lcd_disp_s(0,14,ipdz%100/10);
    lcd_disp_s(0,15,ipdz%10);
    if(ksls==0)
    {
        dekg_disp(0,scha);
        dekg_disp(1,schb);
        dekg_disp(2,schc);
        dekg_disp(3,schd);
    }
    else if(ksls==1)
    {
        dekg_disp(0,sche);
        dekg_disp(1,schf);
        dekg_disp(2,schg);
        dekg_disp(3,schh);
    }
    else if(ksls==2)
    {
        dekg_disp(0,scla);
        dekg_disp(1,sclb);
        dekg_disp(2,sclc);
        dekg_disp(3,scld);
    }
    else if(ksls==3)
    {
        dekg_disp(0,scle);
        dekg_disp(1,sclf);
        dekg_disp(2,sclg);
        dekg_disp(3,sclh);
    }
    ksls=ksls*4;
    lcd_disp_e(3, 0,36);
    if(yiwei==1&ss_tt)
    {
        lcd_disp_e(3, 1,54);
        lcd_disp_e(3, 2,54);
    }
    else
    {
        lcd_disp_s(3, 1,(ksls+1)/10);
        lcd_disp_s(3, 2,(ksls+1)%10);
    }
    lcd_disp_e(3, 3,54);

    lcd_disp_e(3, 4,36);
    if(yiwei==2&ss_tt)
    {
        lcd_disp_e(3, 5,54);
        lcd_disp_e(3, 6,54);
    }
    else
    {
        lcd_disp_s(3, 5,(ksls+2)/10);
        lcd_disp_s(3, 6,(ksls+2)%10);
    }
    lcd_disp_e(3, 7,54);

    lcd_disp_e(3, 8,36);
    if(yiwei==3&ss_tt)
    {
        lcd_disp_e(3, 9,54);
        lcd_disp_e(3,10,54);
    }
    else
    {
        lcd_disp_s(3, 9,(ksls+3)/10);
        lcd_disp_s(3,10,(ksls+3)%10);
    }
    lcd_disp_e(3,11,54);

    lcd_disp_e(3,12,36);
    if(yiwei==4&ss_tt)
    {
        lcd_disp_e(3,13,54);
        lcd_disp_e(3,14,54);
    }
    else
    {
        lcd_disp_s(3,13,(ksls+4)/10);
        lcd_disp_s(3,14,(ksls+4)%10);
    }
    lcd_disp_e(3,15,54);
}
//整体显示
void lcd_disp(void)
{
    if(cs_tt)cans_dasp();
    else
    {
        if(xian==0)
        {
            rqsj_disp(0);	//日期时钟
            ricril_disp(2);	//日出日落
        }
        else if(xian==1)deng_disp(0);
        else if(xian==2)deng_disp(1);
        else if(xian==3)deng_disp(2);
        else if(xian==4)deng_disp(3);
    }
    if(ss_tt )d1=0;
    else d1=1;
    if(trd_tt)d2=0;
    else d2=1;
    if(xfd_tt)d3=0;
    else d3=1;


}
