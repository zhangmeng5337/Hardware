#include "pt100.h"
#include "pt100_ROM.h"
stru_pt_stru  pt_u;
/**********************************************
 *
 * 		Author	:		Shawn Guo
 *      Mail    :       iseanxp@gmail.com
 * 		Date	:       2013/8/9
 *      Last    :
 * 		Notes	:       PT100�¶ȴ�����
 * 		Tool    :       MSP430FX2XX PT100
 *
 *
 *      pt100   ---> �ݸ�˹����  --->    ��ַŴ���   --->    AD����
 *      ---->   �����¶�ֵ   --->    LCD��ʾ
 **********************************************/

/***************************************************************************//**
 * @brief   ����ADC�Ĳ���ֵ�����¶�.
 * @param   vadc, ADC��������ĵ�ѹֵ, ��λ: mv
 * @return  �¶ȵ�100�����¶ȳ���ƿ����Ϊ0xffff.
 *******************************************************************************/
unsigned int PT100_Temp(unsigned int vadc)
{
    unsigned long pt100 = (((unsigned long)((PT100_U3 + ((vadc) / (PT100_VADC_TIME)))) * PT100_R2 * 100) / (PT100_VDD - (PT100_U3 + ((vadc) / (PT100_VADC_TIME)))));

    //���ַ������¶�ֵ
    unsigned int start = 0, mid = 0, endd = 1023;
    unsigned int dat = 0;

    // 1. ������ݺϷ���
    if ((pt100 > PT100_MAX_TEMP ) || (pt100 < PT100_MIN_TEMP))
    {
        return 0xffff;  //�����־
    }

    // 2. ��ʼ���ַ�����
    while (start <= endd)
    {
        mid = (endd + start) >> 1;
        dat = PT100_ROM[mid];
        if (pt100 > dat)
        {
            start = mid + 1;
        }
        else if(pt100 < dat)
        {
            if(mid)
            {
                endd = mid - 1;
            }
            else    //mid = 0���������
            {
                break;
            }
        }
        else
        {
            break;  //�ҵ�Ϊ�м�ֵmid��
        }
    }//�������õ�ֵmid��


    //�����±�����¶�ֵ

    if (dat == pt100) // �������
    {
        return mid * 10;
    }
    else
    {
        if (dat > pt100)
        {
            return (10*(mid - 1)  + 10*(pt100 - PT100_ROM[mid - 1]) / (PT100_ROM[mid] - PT100_ROM[mid - 1]));
        }
        else
        {
            return (10 * mid + 10*(pt100 - PT100_ROM[mid]) / (PT100_ROM[mid + 1]-PT100_ROM[mid]));
        }
    }

}

//r=b^2(2a-b)
stru_pt_stru cal_pt_temp(unsigned char pt_NO)
{
	float *adc_vol;
	float pt_a,pt_b;
	//adc_vol = get_ads_volt();

   pt_u.pt_temp[pt_NO]= PT100_Temp(adc_vol[0]);
   return pt_u;
}

