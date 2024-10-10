/********************** 设置RTC时间 *****************************
uchar   SetRTC(void)
{
    P_SW2 |= 0x80;      //SFR enable
    if(nian > 99)                       return 1;
    if((yue == 0) || (yue > 12))    return 2;
    if((ri == 0) || (ri > 31))      return 3;
    if(shi > 23)                    return 4;
    if(fen > 59)                    return 5;
    if(miao > 59)                   return 6;
    INIYEAR  = nian;
    INIMONTH = yue;
    INIDAY   = ri;
    INIHOUR  = shi;
    INIMIN   = fen;
    INISEC   = miao;
    INISSEC  = 0;
//  RTCCFG   = 0x01 | 0x02; //设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.
//  while(RTCCFG & 0x01);   //等待初始化完成. 设置RTC时间需要32768Hz的1个周期时间,大约30.5us. 由于同步, 所以实际等待时间是0~30.5us.
                            //如果不等待设置完成就睡眠, 则RTC会由于设置没完成, 停止计数, 唤醒后才继续完成设置并继续计数.
     P_SW2 &= ~0x80;                             //??XFR??

    return 0;
}
、**********************************/
void SetRTC()
{
    xchar tmp;
    unsigned int result,result2;
	  result2 = nian;
    P_SW2 |= 0x80;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp + result;
    INIYEAR  = tmp;
 result2 = yue;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp + result;
    INIMONTH = tmp;
 result2 = ri;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp + result;
    INIDAY   = tmp;
result2 = shi;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp +result;
    INIHOUR  = tmp;
result2 = fen;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp + result;
    INIMIN   = tmp;
result2 = miao;
    result = result2 / 16;
    tmp = result*10;
    result = result2 % 16;
    tmp = tmp + result;
    INISEC   = tmp;
    INISSEC  = 0;
    RTCCFG   |= 0x01 ;  //设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.
    while (RTCCFG &
            0x01);  //等待初始化完成. 设置RTC时间需要32768Hz的1个周期时间,大约30.5us. 由于同步, 所以实际等待时间是0~30.5us.
    //如果不等待设置完成就睡眠, 则RTC会由于设置没完成, 停止计数, 唤醒后才继续完成设置并继续计数.
    P_SW2 &= ~0x80;
}
void RTC_config()
{
    P_SW2  |= 0x80;     //SFR enable
    P1n_pure_input(0xc0);       //P1.6 P1.7设置为高阻输入
    P1IE = ~0xc0;               //P1.6 P1.7关闭数字输入功能
    RTCCR = 0x01; //RTC使能
    IRC32KCR = 0x80; //启动内部32K振荡器
    while (!(IRC32KCR & 0x01)); //等等时钟稳定
    RTCCFG |= 0x02; //选择内部32K作为RTC时钟源
    SetRTC();//设置RTC时间
    RTCIEN = 0x00; //使能RTC秒中断
    RTCIF = 0; //清中断标志
    P_SW2 &= ~0x80;
}
/********************** RTC配置函数 *****************************
void    RTC_config(void)    //RTC初始化函数
{
    P_SW2  |= 0x80;     //SFR enable
    P1n_pure_input(0xc0);       //P1.6 P1.7设置为高阻输入
    P1IE = ~0xc0;               //P1.6 P1.7关闭数字输入功能
//  X32KCR = 0x80 + 0x40;       //启动外部32K晶振, 低增益+0x00, 高增益+0x40.

//  year    = 24;
//  month   = 3;
//  day     = 31;
//  hour    = 23;
    //minute  = 59;
//  second  = 44;
//  week=7;
        RTCCFG|=0x02; //0x01 | 0x02;    //设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.  //选择内部32K作为RTC时钟源
        while(RTCCFG & 0x01);
        IRC32KCR=0x80;//启动内部32K振荡器
        while(!(IRC32KCR&0x01));//等等时钟稳定
        SetRTC();//设置RTC时间
        RTCIF=0;//清中断标志
        RTCIEN=0x08;//使能RTC秒中断
        RTCCR=0x01;//RTC使能
        RTCCR   = 0x01; //使能RTC, 并开始RTC计数

        RTCIF   = 0;    //中断标志, 0x80:闹钟中断, 0x40:日中断, 0x20:小时中断, 0x10:分钟中断, 0x08:秒中断, 0x04:1/2秒中断, 0x02:1/8秒中断, 0x01:1/32秒中断
        RTCIEN  = 0x04; //中断使能, 0x80:闹钟中断, 0x40:日中断, 0x20:小时中断, 0x10:分钟中断, 0x08:秒中断, 0x04:1/2秒中断, 0x02:1/8秒中断, 0x01:1/32秒中断
        ALAHOUR = 8;    //闹钟小时
        ALAMIN  = 30;   //闹钟分钟
        ALASEC  = 0;    //闹钟秒
        ALASSEC = 0;    //闹钟1/128秒
    P_SW2 &= ~0x80;                             //??XFR??

}
/*
void RTC_config()
{
        IRC32KCR=0x80;//启动内部32K振荡器
//      while(!(IRC32KCR&0x01));//等等时钟稳定
        RTCCFG|=0x02;   //选择内部32K作为RTC时钟源
        SetRTC();//设置RTC时间
        RTCIF=0;//清中断标志
        RTCIEN=0x08;//使能RTC秒中断
        RTCCR=0x01;//RTC使能
}*/
/********************** 读取RTC时间信息 *****************************/
void    RTC_read(void)
{
    uchar seccond;
    unsigned int result;
    uchar tmp;

    P_SW2  |= 0x80;     //SFR enable
    nian    = RTC_YEAR;     // RTC的年计数值
    yue   = RTC_MONTH;  // RTC的月计数值
    ri     = RTC_DAY;       // RTC的日计数值
    shi    = RTC_HOUR;      // RTC的时计数值
    fen  = RTC_MIN;     // RTC的分计数值
    seccond  = RTC_SEC;     // RTC的秒计数值
    miao = seccond;
    //ssecond = RTC_SSEC;       // RTC的1/128秒计数值



    result = nian / 10;
    tmp = result;
    tmp = tmp << 4;
    result = nian % 10;
    tmp = tmp | result;
    nian = tmp;

    result = yue / 10;
    tmp = result;
    tmp = tmp << 4;
    result = yue % 10;
    tmp = tmp | result;
    yue = tmp;


    result = ri / 10;
    tmp = result;
    tmp = tmp << 4;
    result = ri % 10;
    tmp = tmp | result;
    ri = tmp;

    result = shi / 10;
    tmp = result;
    tmp = tmp << 4;
    result = shi % 10;
    tmp = tmp | result;
    shi = tmp;

    result = fen / 10;
    tmp = result;
    tmp = tmp << 4;
    result = fen % 10;
    tmp = tmp | result;
    fen = tmp;

    result = miao / 10;
    tmp = result;
    tmp = tmp << 4;
    result = miao % 10;
    tmp = tmp | result;
    miao = tmp;
    P_SW2 &= ~0x80;                             //??XFR??
    if ((shi == 23) && (fen == 59) && (miao == 59))
    {
        zhou++;
        if (zhou == 8)zhou = 1;
    }
}
void tinit()
{
	nian=0x24;
	yue=0x10;
	ri=0x10;
	shi=0x18;
	fen=0x1;
	miao=0x0;
}
