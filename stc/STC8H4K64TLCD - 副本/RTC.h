bit B_1s;

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
   // P_SW2 |= 0x80;
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
	RTCCFG	 = 0x01 ;	//设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.
	while(RTCCFG & 0x01);	//等待初始化完成. 设置RTC时间需要32768Hz的1个周期时间,大约30.5us. 由于同步, 所以实际等待时间是0~30.5us.
	// P_SW2 &= ~0x80;  

    //如果不等待设置完成就睡眠, 则RTC会由于设置没完成, 停止计数, 唤醒后才继续完成设置并继续计数.
   
}
void RTC_config()
{
    P_SW2  |= 0x80;     //SFR enable
//    P1n_pure_input(0xc0);       //P1.6 P1.7设置为高阻输入
   // P1IE = ~0xc0;               //P1.6 P1.7关闭数字输入功能

	X32KCR = 0x80+0x40;
	while(!(X32KCR & 1));

	
    
    //SetRTC();//设置RTC时间
   RTCCFG	= 0x01 ;  //设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.
   RTCIF = 0;
   RTCIEN = 0x00;
   RTCCR = 0x01;
	while(RTCCFG & 0x01);   //等待初始化完成. 设置RTC时间需要32768Hz的1个周期时间,大约30.5us. 由于同步, 所以实际等待时间是0~30.5us.
	//P_SW2 &= ~0x80;  

}
void RTC_INT_CONFIG()
{
	 //SetRTC();//设置RTC时间
	RTCCFG	 = 0x01 ;  //设置RTC时间, |0x00:选择外部32K时钟, |0x02:选择内部32K时钟.
	RTCIF = 0;
	RTCIEN = 0x08;
	RTCCR = 0x01;
	 while(RTCCFG & 0x01);	 //等待初始化完成. 设置RTC时间需要32768Hz的1个周期时间,大约30.5us. 由于同步, 所以实际等待时间是0~30.5us.
	 //P_SW2 &= ~0x80; 
	P_SW2  |= 0x80;


}
/******************** RTC 中断函数 *********************/
void RTC_Isr() interrupt 13
{
char store;
store=P_SW2; 

//P_SW2 = P_SW2|0x80;


if(RTCIF & 0x08) //秒中断
{
RTCIF &= ~0x08;
//com_data_tx(0x1a);
//IAP_CONTR =IAP_CONTR|0x20;

B_1s = 1;

		//Ext_Vcc_Det();
		//com_data_tx(0x77);

}
//	P_SW2 = store;

}

/********************** 读取RTC时间信息 *****************************/
void    RTC_read(void)
{
    uchar seccond;
    unsigned int result;
    uchar tmp;

    P_SW2  |= 0x80;     //SFR enable
    nian    = YEAR;     // RTC的年计数值
    yue   = MONTH;  // RTC的月计数值
    ri     = DAY;       // RTC的日计数值
    shi    = HOUR;      // RTC的时计数值
    fen  = MIN;     // RTC的分计数值
    seccond  = SEC;     // RTC的秒计数值
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
  //  P_SW2 &= ~0x80;                             //??XFR??
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
