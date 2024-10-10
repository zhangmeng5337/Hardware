/********************** ����RTCʱ�� *****************************
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
//  RTCCFG   = 0x01 | 0x02; //����RTCʱ��, |0x00:ѡ���ⲿ32Kʱ��, |0x02:ѡ���ڲ�32Kʱ��.
//  while(RTCCFG & 0x01);   //�ȴ���ʼ�����. ����RTCʱ����Ҫ32768Hz��1������ʱ��,��Լ30.5us. ����ͬ��, ����ʵ�ʵȴ�ʱ����0~30.5us.
                            //������ȴ�������ɾ�˯��, ��RTC����������û���, ֹͣ����, ���Ѻ�ż���������ò���������.
     P_SW2 &= ~0x80;                             //??XFR??

    return 0;
}
��**********************************/
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
    RTCCFG   |= 0x01 ;  //����RTCʱ��, |0x00:ѡ���ⲿ32Kʱ��, |0x02:ѡ���ڲ�32Kʱ��.
    while (RTCCFG &
            0x01);  //�ȴ���ʼ�����. ����RTCʱ����Ҫ32768Hz��1������ʱ��,��Լ30.5us. ����ͬ��, ����ʵ�ʵȴ�ʱ����0~30.5us.
    //������ȴ�������ɾ�˯��, ��RTC����������û���, ֹͣ����, ���Ѻ�ż���������ò���������.
    P_SW2 &= ~0x80;
}
void RTC_config()
{
    P_SW2  |= 0x80;     //SFR enable
    P1n_pure_input(0xc0);       //P1.6 P1.7����Ϊ��������
    P1IE = ~0xc0;               //P1.6 P1.7�ر��������빦��
    RTCCR = 0x01; //RTCʹ��
    IRC32KCR = 0x80; //�����ڲ�32K����
    while (!(IRC32KCR & 0x01)); //�ȵ�ʱ���ȶ�
    RTCCFG |= 0x02; //ѡ���ڲ�32K��ΪRTCʱ��Դ
    SetRTC();//����RTCʱ��
    RTCIEN = 0x00; //ʹ��RTC���ж�
    RTCIF = 0; //���жϱ�־
    P_SW2 &= ~0x80;
}
/********************** RTC���ú��� *****************************
void    RTC_config(void)    //RTC��ʼ������
{
    P_SW2  |= 0x80;     //SFR enable
    P1n_pure_input(0xc0);       //P1.6 P1.7����Ϊ��������
    P1IE = ~0xc0;               //P1.6 P1.7�ر��������빦��
//  X32KCR = 0x80 + 0x40;       //�����ⲿ32K����, ������+0x00, ������+0x40.

//  year    = 24;
//  month   = 3;
//  day     = 31;
//  hour    = 23;
    //minute  = 59;
//  second  = 44;
//  week=7;
        RTCCFG|=0x02; //0x01 | 0x02;    //����RTCʱ��, |0x00:ѡ���ⲿ32Kʱ��, |0x02:ѡ���ڲ�32Kʱ��.  //ѡ���ڲ�32K��ΪRTCʱ��Դ
        while(RTCCFG & 0x01);
        IRC32KCR=0x80;//�����ڲ�32K����
        while(!(IRC32KCR&0x01));//�ȵ�ʱ���ȶ�
        SetRTC();//����RTCʱ��
        RTCIF=0;//���жϱ�־
        RTCIEN=0x08;//ʹ��RTC���ж�
        RTCCR=0x01;//RTCʹ��
        RTCCR   = 0x01; //ʹ��RTC, ����ʼRTC����

        RTCIF   = 0;    //�жϱ�־, 0x80:�����ж�, 0x40:���ж�, 0x20:Сʱ�ж�, 0x10:�����ж�, 0x08:���ж�, 0x04:1/2���ж�, 0x02:1/8���ж�, 0x01:1/32���ж�
        RTCIEN  = 0x04; //�ж�ʹ��, 0x80:�����ж�, 0x40:���ж�, 0x20:Сʱ�ж�, 0x10:�����ж�, 0x08:���ж�, 0x04:1/2���ж�, 0x02:1/8���ж�, 0x01:1/32���ж�
        ALAHOUR = 8;    //����Сʱ
        ALAMIN  = 30;   //���ӷ���
        ALASEC  = 0;    //������
        ALASSEC = 0;    //����1/128��
    P_SW2 &= ~0x80;                             //??XFR??

}
/*
void RTC_config()
{
        IRC32KCR=0x80;//�����ڲ�32K����
//      while(!(IRC32KCR&0x01));//�ȵ�ʱ���ȶ�
        RTCCFG|=0x02;   //ѡ���ڲ�32K��ΪRTCʱ��Դ
        SetRTC();//����RTCʱ��
        RTCIF=0;//���жϱ�־
        RTCIEN=0x08;//ʹ��RTC���ж�
        RTCCR=0x01;//RTCʹ��
}*/
/********************** ��ȡRTCʱ����Ϣ *****************************/
void    RTC_read(void)
{
    uchar seccond;
    unsigned int result;
    uchar tmp;

    P_SW2  |= 0x80;     //SFR enable
    nian    = RTC_YEAR;     // RTC�������ֵ
    yue   = RTC_MONTH;  // RTC���¼���ֵ
    ri     = RTC_DAY;       // RTC���ռ���ֵ
    shi    = RTC_HOUR;      // RTC��ʱ����ֵ
    fen  = RTC_MIN;     // RTC�ķּ���ֵ
    seccond  = RTC_SEC;     // RTC�������ֵ
    miao = seccond;
    //ssecond = RTC_SSEC;       // RTC��1/128�����ֵ



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
