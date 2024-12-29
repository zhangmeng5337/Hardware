#include "intrins.h"
#include "math.h"



#define ADCTIM  (*(unsigned char volatile xdata *)0xfea8)
#define SYS_CLK 24000000
#define BUBBLE_SORT 1
#define SUM_LENGTH 16
#define AdcVref 2.5
#define UA_CHN   0
#define UB_CHN   1
#define UC_CHN   4
#define IA_CHN   5
#define IB_CHN   7
#define IC_CHN   2


unsigned int  zero;
unsigned int square_ok;

unsigned int ua_e, ub_e, uc_e;
unsigned ia_e, ib_e, ic_e;

unsigned int ua, ub, uc;
unsigned ia, ib, ic;
unsigned int ADC_Buffer[SUM_LENGTH];
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
    EA = 1;
    //ADC_CONTR |= 0x40;                          //启动AD转换

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
/*****************************

******************************/
void vol_assign(unsigned int chn, long unsigned int voleSquare)
{
   if(square_ok == 1)
   	{
   square_ok = 0;
    switch (chn)
    {
        case UA_CHN:
            ua = voleSquare;
            break;
        case UB_CHN:
            ub = voleSquare;
            break;

        case UC_CHN:
            uc = voleSquare;
            break;

        case IA_CHN:
            ia = voleSquare;
            break;
        case IB_CHN:
            ib = voleSquare;
            break;
        case IC_CHN:
            ic = voleSquare;
			square_ok = 1;
            break;

        default:
            break;
    }

   }

	 square_ok;
}
void cal_VolCurr(unsigned char chn)
{
   long unsigned int result;
	if(square_ok == 1)
	{
    switch (chn)
    {
        case UA_CHN:
            ua_e = ua/SUM_LENGTH;
			ua_e = sqrt(ua_e);
            break;
        case UB_CHN:
            ub_e = ub/SUM_LENGTH;
			ub_e = sqrt(ub_e);

            break;

        case UC_CHN:
            uc_e = uc/SUM_LENGTH;
			uc_e = sqrt(uc_e);

            break;

        case IA_CHN:
            ic_e = ia/SUM_LENGTH;
			ic_e = sqrt(ic_e);

            break;
        case IB_CHN:
            ib_e = ib/SUM_LENGTH;
			ib_e = sqrt(ib_e);

            break;
        case IC_CHN:
            ic_e = ic/SUM_LENGTH;
			ic_e = sqrt(ic_e);

			square_ok = 1;
            break;

        default:
            break;
    }

	}
}
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
    NOP(10);            //
    while ((ADC_CONTR & ADC_FLAG) == 0)  ;  //等待ADC结束
    ADC_CONTR &= ~ADC_FLAG;
    return ((unsigned int)ADC_RES * 256 + (unsigned int)ADC_RESL);
}
/***********************************
查询方式做一次ADC, chn为通道号, chn=0~7对应P1.0~P1.7, chn=8~14对应P0.0~P0.6, chn=15对应BandGap电压.
***********************************/
void    ADC_convert(unsigned char chn)
{
    long unsigned int   j;
    unsigned char   k;      //平均值滤波时使用
    long  unsigned int   result;
 long  unsigned tmp;
    Get_ADC12bitResult(
        chn);        //参数i=0~15,查询方式做一次ADC, 切换通道后第一次转换结果丢弃. 避免采样电容的残存电压影响.
    //Get_ADC12bitResult(chn);      //参数i=0~15,查询方式做一次ADC, 切换通道后第二次转换结果丢弃. 避免采样电容的残存电压影响.

//#ifdef BUBBLE_SORT  //使用冒泡排序，去掉最高值、最低值，求中间平均值
//
//    for (k = 0; k < 16; k++)
//    {
//
//	}
//        ADC_Buffer[k] = Get_ADC12bitResult(chn);
//    BubbleSort(ADC_Buffer, 16); //冒泡排序
//    for (k = 4, j = 0; k < 12; k++)
//        j += ADC_Buffer[k];  //取中间8个数据
//    j = j * 10;
//    j = j / 8;      // 求平均
//
//#else   //采样累加，求平均值（不需要的话可将 SUM_LENGTH 定义值改为 1 ）
  tmp = 0;
    for (k = 0, j = 0; k < SUM_LENGTH; k++)
    {
        j += Get_ADC12bitResult(
                 chn);   // 采样累加和 参数0~15,查询方式做一次ADC, 返回值就是结果

    if(j>=zero)
    {
		j = j -zero;
	}
	else
	{
		j = zero-j;	
	}
	tmp = tmp + j * j;

	}

    result = tmp;
             vol_assign(chn, result);
}


