#include "intrins.h"
#include "math.h"
#include "stdio.h"

#define FOSC        33000000UL
#define BRT         (65536 - FOSC / 115200 / 4)


#define ADCTIM  (*(unsigned char volatile xdata *)0xfea8)

#define ADC_VZERO    1024    // 0点读数
#define ADC_CZERO    1024    // 0点读数
#define ADC_CALI_COFF 0.196581 //3/483 0.00621  0.001221
#define ADC_CALI_ICOFF 0.039377

#define SUM_LENGTH 300
#define AdcVref 2.5

#define UA_CHN   0
#define UB_CHN   1
#define UC_CHN   4
#define IA_CHN   5
#define IB_CHN   7
#define IC_CHN   2

void    ADC_convert(void);

unsigned char adcPollChn[6] = {UA_CHN, IA_CHN, UB_CHN, IB_CHN, UC_CHN, IC_CHN};
unsigned int  vzero, czero;
unsigned int square_ok;

unsigned int freq_count = 0;
unsigned int freq;
unsigned char freq_flag;

unsigned int ua, ub, uc;//电压有效值
unsigned ia, ib, ic;//电流有效值
float pa, pb, pc; //有功功率
float sa, sb, sc; //视在功率
float qa, qb, qc; //无功功率

float total_s; //总视在功率
float total_q; //总无功功率
float total_p; //总无功功率

float fi_a;
float fi_b;
float fi_c;
float fi_s;


bit busy;

unsigned int adcVolBuf[SUM_LENGTH];
unsigned int adcCurrBuf[SUM_LENGTH];
void Uart1_Init(void)   //9600bps@33.1776MHz
{
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x40;       //定时器时钟1T模式
    AUXR &= 0xFE;       //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F;       //设置定时器模式
    TL1 = 0xA0;         //设置定时初始值
    TH1 = 0xFC;         //设置定时初始值
    ET1 = 0;            //禁止定时器中断
    TR1 = 1;            //定时器1开始计时
}

void UartPutc(unsigned char dat)
{
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}

char putchar(char c)
{
    UartPutc(c);
    return c;
}



//void Timer0_Init(void)      //1毫秒@33.000MHz
//{
//    AUXR |= 0x80;           //定时器时钟1T模式
//    TMOD &= 0xF0;           //设置定时器模式
//    TL0 = 0x18;             //设置定时初始值
//    TH0 = 0x7F;             //设置定时初始值
//    TF0 = 0;                //清除TF0标志
//    TR0 = 1;                //定时器0开始计时
//}
void Timer3_Init(void)      //1毫秒@33.000MHz
{
    T4T3M |= 0x02;          //定时器时钟1T模式
    T3L = 0x18;             //设置定时初始值
    T3H = 0x7F;             //设置定时初始值
    T4T3M |= 0x08;          //定时器3开始计时
    IE2 = ET3;                                  //使能定时器中断
}
void TM3_Isr() interrupt 19
{
    static unsigned int last_vol;
    last_vol ++;
    if (freq_flag == 1)
    {

        freq_count++;
        freq_flag = 0;
        if (last_vol >= 5000)
        {
            freq = freq_count * 10 / last_vol; //freq times 10
            last_vol = 0;
            freq_count = 0;
        }

    }
    //UartSend(4);
    printf("irq\r\n");
    ADC_convert();

}

void ADC_Isr() interrupt 5
{
    ADC_CONTR &= ~0x20;                         //清中断标志
    P2 = ADC_RES;                               //读取ADC结果
    ADC_CONTR |= 0x40;                          //继续AD转换
}

void adc_init()
{


    P1M0 = 0x00;                                //设置P1.0为ADC口
    P1M1 = 0x33;
    P5M0 = 0x00;                                //设置P1.0为ADC口
    P5M1 = 0x10;



    P_SW2 |= 0x80;
    ADCTIM = 0x2c;                              //设置ADC内部时序
    P_SW2 &= 0x7f;
    ADCCFG = ~0x0f;                              //设置ADC时钟为系统时钟/2/16
    ADC_CONTR = 0x80;                           //使能ADC模块
    EADC = 1;                                   //使能ADC中断
    Uart1_Init();   //9600bps@33.000MHz
    Timer3_Init();

    EA = 1;
    //ADC_CONTR |= 0x40;                          //启动AD转换
    square_ok = 0;
    vzero = ADC_VZERO;
    czero = ADC_CZERO;
    freq_flag = 0;
    // while (1);
}
#ifdef BUBBLE_SORT  //使用冒泡排序
//========================================================================
// 函数: void DataSwap(unsigned int* data1, unsigned int* data2)
// 描述: 数据交换函数。
// 参数: data1,data2 要交换的数据.
// 返回: none.
// 版本: VER1.0
// 日期: 2021-9-27
// 备注:
//========================================================================
void DataSwap(unsigned int *data1, unsigned int *data2)
{
    unsigned int temp;
    temp = *data1;
    *data1 = *data2;
    *data2 = temp;
}

//========================================================================
// 函数: void BubbleSort(unsigned int* pDataArry, unsigned char DataNum)
// 描述: 冒泡排序函数。
// 参数: pDataArry需要排序的数组，DataNum需要排序的数据个数.
// 返回: none.
// 版本: VER1.0
// 日期: 2021-9-27
// 备注:
//========================================================================
void BubbleSort(unsigned int *pDataArry, unsigned char DataNum)
{
    bit flag;
    unsigned char i, j;
    for (i = 0; i < DataNum - 1; i++)
    {
        flag = 0;
        for (j = 0; j < DataNum - i - 1; j++)
        {
            if (pDataArry[j] > pDataArry[j + 1])
            {
                flag = 1;
                DataSwap(&pDataArry[j], &pDataArry[j + 1]);
            }
        }
        if (!flag) //上一轮比较中不存在数据交换，则退出排序
        {
            break;
        }
    }
}
#endif


//========================================================================
// 函数: unsigned int Get_ADC12bitResult(unsigned char channel))  //channel = 0~15
// 描述: 查询法读一次ADC结果.
// 参数: channel: 选择要转换的ADC, 0~15.
// 返回: 12位ADC结果.
// 版本: V1.0, 2016-4-28
//========================================================================
unsigned int    Get_ADC12bitResult(unsigned char channel)   //channel = 0~15
{
    ADC_RES = 0;
    ADC_RESL = 0;
    ADC_CONTR = 0x80 | ADC_START | channel;

}
/***********************************
查询方式做一次ADC, chn为通道号, chn=0~7对应P1.0~P1.7, chn=8~14对应P0.0~P0.6, chn=15对应BandGap电压.
***********************************/
void    ADC_convert()
{
    static unsigned char i = 0, j = 0;
    if ((ADC_CONTR & ADC_FLAG) == 1)
    {
        switch (i)
        {
            case 0:
                i = 5; //ua
                adcVolBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;
            case 1:
                i = 3;//ub
                adcVolBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;
            case 4:
                i = 2;//uc
                adcVolBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;
            case 5:
                i = 1;//ia
                adcCurrBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;
            case 3:
                i = 4;
                adcCurrBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;//ib
            case 2:
                i = 0;
                adcCurrBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
                break;//ic
        }
//        if (i % 2 == 0)
//            adcVolBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
//        else
//            adcCurrBuf[j++] = ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
        ADC_CONTR &= ~ADC_FLAG;
        Get_ADC12bitResult(i);
//        if (i == 6)
//        {
//            i = 0;
//        }
        if (j == SUM_LENGTH)
        {
            j = 0;
            square_ok = 1;
        }

    }

}

void VolCurrCal()
{
    float tmp1, tmp2, tmp3;
    float uatmp, ubtmp, uctmp;
    float iatmp, ibtmp, ictmp;

    unsigned char i;
    if (square_ok == 1)
    {
        uatmp = 0;
        ubtmp = 0;
        uctmp = 0;
        iatmp = 0;
        ibtmp = 0;
        ictmp = 0;
        pa = 0;
        pb = 0;
        pc = 0;
        sa = 0;
        sb = 0;
        sc = 0;
        qa = 0;
        qb = 0;
        qc = 0;
        total_s = 0;
        total_q = 0;
        total_p = 0;


        for (i = 0; i < SUM_LENGTH / 3; i++)
        {
            if (adcVolBuf[6 * i] >= vzero)
                tmp1 = adcVolBuf[6 * i] - vzero;
            else
                tmp1 = vzero - adcVolBuf[6 * i];
            if (adcVolBuf[1 + 6 * i] >= czero)
                tmp2 = adcVolBuf[6 * i] - czero;
            else
                tmp2 = czero - adcVolBuf[1 + 6 * i];

            tmp1 = tmp1 * ADC_CALI_COFF; //电压
            tmp2 = tmp2 * ADC_CALI_ICOFF; //电流
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;

            pa = tmp1 * tmp2 + pa; //单件有功功率
            tmp1 = tmp1 * tmp1; // uk^2
            tmp2 = tmp2 * tmp2; //ik^2
            uatmp = uatmp + tmp1;//sum uk^2
            iatmp = iatmp + tmp2;//sum uk^2
        }
        uatmp = uatmp / SUM_LENGTH;
        uatmp = uatmp * 3;
        uatmp = sqrt(uatmp);

        iatmp = iatmp / SUM_LENGTH;
        iatmp = iatmp * 3;
        iatmp = sqrt(iatmp);

        sa = uatmp * iatmp;

        uatmp = uatmp * 10; //放大10倍
        iatmp = iatmp * 10;

        ia = (unsigned int)iatmp;
        ua = (unsigned int)uatmp;

        for (i = 0; i < SUM_LENGTH / 3; i++)
        {
            if (adcVolBuf[2 + 6 * i] >= vzero)
                tmp1 = adcVolBuf[2 + 6 * i] - vzero;
            else
                tmp1 = vzero - adcVolBuf[2 + 6 * i];
            if (adcVolBuf[3 + 6 * i] >= czero)
                tmp2 = adcVolBuf[3 + 6 * i] - czero;
            else
                tmp2 = czero - adcVolBuf[3 + 6 * i];



            tmp1 = tmp1 * ADC_CALI_COFF; //电压
            tmp2 = tmp2 * ADC_CALI_ICOFF; //电流
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;
            pb = tmp1 * tmp2 + pa; //单件有功功率
            tmp1 = tmp1 * tmp1; // uk^2
            tmp2 = tmp2 * tmp2; //ik^2
            uatmp = uatmp + tmp1;//sum uk^2
            iatmp = iatmp + tmp2;//sum uk^2
        }
        uatmp = uatmp / SUM_LENGTH;
        uatmp = uatmp * 3;
        uatmp = sqrt(uatmp);

        iatmp = iatmp / SUM_LENGTH;
        iatmp = iatmp * 3;
        iatmp = sqrt(iatmp);

        sb = uatmp * iatmp;

        uatmp = uatmp * 10; //放大10倍
        iatmp = iatmp * 10;

        ib = (unsigned int)iatmp;
        ub = (unsigned int)uatmp;



        for (i = 0; i < SUM_LENGTH / 3; i++)
        {
            if (adcVolBuf[4 + 6 * i] >= vzero)
                tmp1 = adcVolBuf[4 + 6 * i] - vzero;
            else
                tmp1 = vzero - adcVolBuf[4 + 6 * i];
            if (adcVolBuf[5 + 6 * i] >= czero)
                tmp2 = adcVolBuf[5 + 6 * i] - czero;
            else
                tmp2 = czero - adcVolBuf[5 + 6 * i];



            tmp1 = tmp1 * ADC_CALI_COFF; //电压
            tmp2 = tmp2 * ADC_CALI_ICOFF; //电流
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;

            pc = tmp1 * tmp2 + pa; //单件有功功率
            tmp1 = tmp1 * tmp1; // uk^2
            tmp2 = tmp2 * tmp2; //ik^2
            uatmp = uatmp + tmp1;//sum uk^2
            iatmp = iatmp + tmp2;//sum uk^2
        }
        uatmp = uatmp / SUM_LENGTH;
        uatmp = uatmp * 3;
        uatmp = sqrt(uatmp);

        iatmp = iatmp / SUM_LENGTH;
        iatmp = iatmp * 3;
        iatmp = sqrt(iatmp);

        sc = uatmp * iatmp;//

        uatmp = uatmp * 10; //放大10倍
        iatmp = iatmp * 10;

        ic = (unsigned int)iatmp;
        uc = (unsigned int)uatmp;



        for (i = 0; i < SUM_LENGTH; i++)
        {
            adcVolBuf[i] = 0;
            adcCurrBuf[i] = 0;
        }
        square_ok = 0;
        qa = sa - pa;
        qb = sb - pb;
        qc = sc - pc;
        total_p = total_p + pa;
        total_p = total_p + pb;
        total_p = total_p + pc;

        total_s = total_s + sa;
        total_s = total_s + sb;
        total_s = total_s + sc;
        total_q = total_q + qa;
        total_q = total_q + qb;
        total_q = total_q + qc;
        fi_a = pa / sa;
        fi_b = pb / sb;

        fi_c = pc / sc;
        fi_s = total_p / total_s;
    }
}





