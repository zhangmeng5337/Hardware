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
 * 		Notes	:       PT100温度传感�?
 * 		Tool    :       MSP430FX2XX PT100
 *
 *      pt100   ---> 惠更斯电�? --->    差分放大�?  --->    AD采样(A6 P6.6)
 *      ---->   计算温度�?  --->    LCD显示
 *         _____________________________V-
 *         |
 *         |        VDD
 *         |         |
 *         |        / \
 *         |       /   \                    差分放大    ---> AD
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
 *      计算公式：R1 = R2 = 1K, R3 = 100�? R4 = pt100.
 *
 *      R3的电位固定为U3 = VDD / (R1 + R3) * R3.
 *      则R4的电位为 U3 + UT; UT为U3和U4的电位差.
 *      R4 = (U4) / I4;
 *      I4 = (VDD - U4) / R2;
 **********************************************/

#define PT100_VDD           3300         //惠更斯电桥的供电电压,放大1000�? 3.3V
#define PT100_VADC_TIME     10      //差分放大电路的放大倍数
#define PT100_R1            1000        //R1 = 1K
#define PT100_R2            1000        //R2 = 1K
#define PT100_R3            100         //R3 = 100

#define PT100_U3                (PT100_VDD / (PT100_R1 + PT100_R3) * PT100_R3)
#define PT100_U4(VADC)            (PT100_U3 + ((VADC) / (PT100_VADC_TIME)))

#define PT100_MIN_TEMP      76.33
#define PT100_MAX_TEMP      138.51   //配置PT100的测温范围为0~100�?
#define PT100_ROM_NUM       801     //PT100_ROM表的大小


#define PT100_RATIO 160/321
#define PT100_B     -60

/***************************************************************************//**
 * @brief   根据ADC的采样值计算温�?
 * @param   vadc, ADC采样后换算的电压差�? 单位: mv
 * @return  温度�?00�?
 *******************************************************************************/
float PT100_Temp(float vadc);


#endif

