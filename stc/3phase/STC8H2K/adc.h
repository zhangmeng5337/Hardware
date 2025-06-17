#include "intrins.h"
#include "math.h"
#include "stdio.h"

#define FOSC        33000000UL
#define BRT         (65536 - FOSC / 115200 / 4)


#define ADCTIM  (*(unsigned char volatile xdata *)0xfea8)

#define ADC_VZERO    1024    // 0�����
#define ADC_CZERO    1024    // 0�����
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

unsigned int ua, ub, uc;//��ѹ��Чֵ
unsigned ia, ib, ic;//������Чֵ
float pa, pb, pc; //�й�����
float sa, sb, sc; //���ڹ���
float qa, qb, qc; //�޹�����

float total_s; //�����ڹ���
float total_q; //���޹�����
float total_p; //���޹�����

float fi_a;
float fi_b;
float fi_c;
float fi_s;


bit busy;

unsigned int adcVolBuf[SUM_LENGTH];
unsigned int adcCurrBuf[SUM_LENGTH];
void Uart1_Init(void)   //9600bps@33.1776MHz
{
    SCON = 0x50;        //8λ����,�ɱ䲨����
    AUXR |= 0x40;       //��ʱ��ʱ��1Tģʽ
    AUXR &= 0xFE;       //����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F;       //���ö�ʱ��ģʽ
    TL1 = 0xA0;         //���ö�ʱ��ʼֵ
    TH1 = 0xFC;         //���ö�ʱ��ʼֵ
    ET1 = 0;            //��ֹ��ʱ���ж�
    TR1 = 1;            //��ʱ��1��ʼ��ʱ
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



//void Timer0_Init(void)      //1����@33.000MHz
//{
//    AUXR |= 0x80;           //��ʱ��ʱ��1Tģʽ
//    TMOD &= 0xF0;           //���ö�ʱ��ģʽ
//    TL0 = 0x18;             //���ö�ʱ��ʼֵ
//    TH0 = 0x7F;             //���ö�ʱ��ʼֵ
//    TF0 = 0;                //���TF0��־
//    TR0 = 1;                //��ʱ��0��ʼ��ʱ
//}
void Timer3_Init(void)      //1����@33.000MHz
{
    T4T3M |= 0x02;          //��ʱ��ʱ��1Tģʽ
    T3L = 0x18;             //���ö�ʱ��ʼֵ
    T3H = 0x7F;             //���ö�ʱ��ʼֵ
    T4T3M |= 0x08;          //��ʱ��3��ʼ��ʱ
    IE2 = ET3;                                  //ʹ�ܶ�ʱ���ж�
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
    ADC_CONTR &= ~0x20;                         //���жϱ�־
    P2 = ADC_RES;                               //��ȡADC���
    ADC_CONTR |= 0x40;                          //����ADת��
}

void adc_init()
{


    P1M0 = 0x00;                                //����P1.0ΪADC��
    P1M1 = 0x33;
    P5M0 = 0x00;                                //����P1.0ΪADC��
    P5M1 = 0x10;



    P_SW2 |= 0x80;
    ADCTIM = 0x2c;                              //����ADC�ڲ�ʱ��
    P_SW2 &= 0x7f;
    ADCCFG = ~0x0f;                              //����ADCʱ��Ϊϵͳʱ��/2/16
    ADC_CONTR = 0x80;                           //ʹ��ADCģ��
    EADC = 1;                                   //ʹ��ADC�ж�
    Uart1_Init();   //9600bps@33.000MHz
    Timer3_Init();

    EA = 1;
    //ADC_CONTR |= 0x40;                          //����ADת��
    square_ok = 0;
    vzero = ADC_VZERO;
    czero = ADC_CZERO;
    freq_flag = 0;
    // while (1);
}
#ifdef BUBBLE_SORT  //ʹ��ð������
//========================================================================
// ����: void DataSwap(unsigned int* data1, unsigned int* data2)
// ����: ���ݽ���������
// ����: data1,data2 Ҫ����������.
// ����: none.
// �汾: VER1.0
// ����: 2021-9-27
// ��ע:
//========================================================================
void DataSwap(unsigned int *data1, unsigned int *data2)
{
    unsigned int temp;
    temp = *data1;
    *data1 = *data2;
    *data2 = temp;
}

//========================================================================
// ����: void BubbleSort(unsigned int* pDataArry, unsigned char DataNum)
// ����: ð����������
// ����: pDataArry��Ҫ��������飬DataNum��Ҫ��������ݸ���.
// ����: none.
// �汾: VER1.0
// ����: 2021-9-27
// ��ע:
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
        if (!flag) //��һ�ֱȽ��в��������ݽ��������˳�����
        {
            break;
        }
    }
}
#endif


//========================================================================
// ����: unsigned int Get_ADC12bitResult(unsigned char channel))  //channel = 0~15
// ����: ��ѯ����һ��ADC���.
// ����: channel: ѡ��Ҫת����ADC, 0~15.
// ����: 12λADC���.
// �汾: V1.0, 2016-4-28
//========================================================================
unsigned int    Get_ADC12bitResult(unsigned char channel)   //channel = 0~15
{
    ADC_RES = 0;
    ADC_RESL = 0;
    ADC_CONTR = 0x80 | ADC_START | channel;

}
/***********************************
��ѯ��ʽ��һ��ADC, chnΪͨ����, chn=0~7��ӦP1.0~P1.7, chn=8~14��ӦP0.0~P0.6, chn=15��ӦBandGap��ѹ.
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

            tmp1 = tmp1 * ADC_CALI_COFF; //��ѹ
            tmp2 = tmp2 * ADC_CALI_ICOFF; //����
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;

            pa = tmp1 * tmp2 + pa; //�����й�����
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

        uatmp = uatmp * 10; //�Ŵ�10��
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



            tmp1 = tmp1 * ADC_CALI_COFF; //��ѹ
            tmp2 = tmp2 * ADC_CALI_ICOFF; //����
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;
            pb = tmp1 * tmp2 + pa; //�����й�����
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

        uatmp = uatmp * 10; //�Ŵ�10��
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



            tmp1 = tmp1 * ADC_CALI_COFF; //��ѹ
            tmp2 = tmp2 * ADC_CALI_ICOFF; //����
//            tmp2 = tmp2 -1.25;
//          tmp2 = tmp2/62;
//          tmp2 = tmp2*2000;

            pc = tmp1 * tmp2 + pa; //�����й�����
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

        uatmp = uatmp * 10; //�Ŵ�10��
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





