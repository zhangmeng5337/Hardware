#ifndef PT100_H_
#define PT100_H_
#include "main.h"
#define PT_SIZE  12
typedef struct
{
	float  pt_temp[12];
	float  pt_res[12];
	//unsigned char di_No_buf[DI_SIZE];
} stru_pt_stru;



/**********************************************
 *
 * 		Author	:		Shawn Guo
 *      Mail    :       iseanxp@gmail.com
 * 		Date	:       2013/8/9
 *      Last    :
 * 		Notes	:       PT100�¶ȴ�����
 * 		Tool    :       MSP430FX2XX PT100
 *
 *      pt100   ---> �ݸ�˹����  --->    ��ַŴ���   --->    AD����(A6 P6.6)
 *      ---->   �����¶�ֵ   --->    LCD��ʾ
 *         _____________________________V-
 *         |
 *         |        VDD
 *         |         |
 *         |        / \
 *         |       /   \                    ��ַŴ�    ---> AD
 *         |      /     \
 *         |     R1     R2
 *         |_____/       \ _____________V+
 *               \       /
 *                R3    R4
 *                 \   /
 *                  \ /
 *                   |
 *                  GND
 *
 *      ���㹫ʽ��R1 = R2 = 1K, R3 = 100ŷ, R4 = pt100.
 *
 *      R3�ĵ�λ�̶�ΪU3 = VDD / (R1 + R3) * R3.
 *      ��R4�ĵ�λΪ U3 + UT; UTΪU3��U4�ĵ�λ��.
 *      R4 = (U4) / I4;
 *      I4 = (VDD - U4) / R2;
 **********************************************/

#define PT100_VDD           3300         //�ݸ�˹���ŵĹ����ѹ,�Ŵ�1000��, 3.3V
#define PT100_VADC_TIME     10      //��ַŴ��·�ķŴ���
#define PT100_R1            1000        //R1 = 1K
#define PT100_R2            1000        //R2 = 1K
#define PT100_R3            100         //R3 = 100

#define PT100_U3                (PT100_VDD / (PT100_R1 + PT100_R3) * PT100_R3)
#define PT100_U4(VADC)            (PT100_U3 + ((VADC) / (PT100_VADC_TIME)))

#define PT100_MIN_TEMP      10000
#define PT100_MAX_TEMP      13884   //����PT100�Ĳ��·�ΧΪ0~100��
#define PT100_ROM_NUM       1024     //PT100_ROM��Ĵ�С




/***************************************************************************//**
 * @brief   ����ADC�Ĳ���ֵ�����¶�.
 * @param   vadc, ADC��������ĵ�ѹ��ֵ, ��λ: mv
 * @return  �¶ȵ�100��
 *******************************************************************************/
unsigned int PT100_Temp(unsigned int vadc);


#endif

