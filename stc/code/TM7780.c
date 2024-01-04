#include "tm7780.h"
#include "hardware.h"
#include "eeprom.h"
#include "uart.h"

//#include "stdio.h"

power_stru power_usr;

//--------------------------------------------------------------------------------------------
/*=============================================================================
  * File Name	     : TM7780�ο�����
  * Describe 	     : �������ʡ���ѹ����������������
  * Version	     : V1.0
  * Record	     : 2014/04/10
=============================================================================*/
/* Includes -----------------------------------------------------------------*/

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Time1��ʱ����ʱ,ʱ����� = 1ms
#define D_TIME1_20MS				20
#define D_TIME1_100MS				100
#define D_TIME1_150MS				150
#define D_TIME1_200MS				200
#define D_TIME1_400MS				400
#define D_TIME1_500MS				500
#define D_TIME1_1S				1000		//Time1��ʱ����ʱ1Sʱ�䳣��
#define D_TIME1_2S				2000
#define D_TIME1_3S				2000
#define D_TIME1_4S				4000
#define D_TIME1_6S				6000
#define D_TIME1_8S				8000
#define D_TIME1_9S				9000
#define D_TIME1_10S				10000
#define D_TIME1_20S				20000


#define D_TIME1_V_OVERFLOW                      500        //Time1��ʱ��,��ѹ��������趨Ϊ500mS,���˵���������ڴ���500mS
#define D_TIME1_I_OVERFLOW			8000	   //Time1��ʱ��,������������趨Ϊ10S,���˵���������ڴ���10S
#define D_TIME1_P_OVERFLOW			12000	   //Time1��ʱ��,������������趨Ϊ10S(Լ0.5W��Сֵ),���˵���������ڴ���10S
//#define D_TIME1_P_OVERFLOW			40000	   //Time1��ʱ��,������������趨Ϊ40S(Լ0.2W��Сֵ)
#define D_TIME1_CAL_TIME			36000	   //У��ʱ�䣬��¼�ڴ�ʱ���ڵ���������1000W�������õ�36Sʱ���ںķ�0.01�ȵ�
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------


//����ģʽ
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
#define D_ERR_MODE                	0x00        //������ʾģʽ
#define D_NORMAL_MODE		      	0x10	    //��������ģʽ
#define D_CAL_START_MODE		0x21	    //У��ģʽ������
#define D_CAL_END_MODE		        0x23	    //У��ģʽ�����
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------


//ROM����
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
union IntData
{
    u16  inte;
    u8 byte[2];
};
union LongData
{
    u32  word;
    u16  inte[2];
    u8   byte[4];
};
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
u16	U16_P_TotalTimes;			//��ǰ���� ���ʲ�����ʱ��
u16	U16_V_TotalTimes;			//��ǰ���� ��ѹ������ʱ��
u16	U16_I_TotalTimes;			//��ǰ���� ����������ʱ��

u16	U16_P_OneCycleTime;			//���ʲ���ʱ�����
u16	U16_V_OneCycleTime;			//��ѹ����ʱ�����
u16	U16_I_OneCycleTime;			//��������ʱ�����

u16	U16_P_Last_OneCycleTime;		//���ʲ���ʱ���������һ������ֵ
u16	U16_V_Last_OneCycleTime;		//��ѹ����ʱ���������һ������ֵ
u16	U16_I_Last_OneCycleTime;		//��������ʱ���������һ������ֵ

u16	U16_P_CNT;				//���ʲ�����������
u16	U16_V_CNT;				//��ѹ������������
u16	U16_I_CNT;				//����������������

u16	U16_P_Last_CNT;				//���ʲ���������������һ������ֵ
u16	U16_V_Last_CNT;				//��ѹ����������������һ������ֵ
u16	U16_I_Last_CNT;				//��������������������һ������ֵ

u8	B_P_TestOneCycle_Mode;			//���ʲ���ģʽ 1:�����ڲ�����0:1S��ʱ����
u8	B_V_TestOneCycle_Mode;
u8	B_I_TestOneCycle_Mode;

u8	B_P_Last_TestOneCycle_Mode;
u8	B_V_Last_TestOneCycle_Mode;
u8	B_I_Last_TestOneCycle_Mode;

u8  	B_P_OVERFLOW;       			// ������������ �����־λ
u8  	B_V_OVERFLOW;       			// ��ѹ�������� �����־λ
u8  	B_I_OVERFLOW;       			// ������������ �����־λ

u8	B_P_Last_OVERFLOW;       		// ������������ �����־λ
u8  	B_V_Last_OVERFLOW;       		// ��ѹ�������� �����־λ
u8  	B_I_Last_OVERFLOW;       		// ������������ �����־λ

u8    	B_VI_Test_Mode;				//1:��ѹ����ģʽ;0:��������ģʽ
u16   	U16_VI_Test_Times;
u16   	U16_Cal_Times;

//u16   	power_usr.U16_AC_P;				//����ֵ 1000.0W
//u16   	power_usr.U16_AC_V;				//��ѹֵ 220.0V
//u16   	power_usr.U16_AC_I;				//����ֵ 4.545A
//u32   	U32_AC_E;				//�õ���   0.01��
u32   	U32_AC_BACKUP_E;			//�õ�������
u8    	U8_AC_COS;				//�������� 0.00

u16   	U16_REF_001_E_Pluse_CNT;        	//0.01�ȵ����������ο�ֵ
u16   	U16_E_Pluse_CNT;          	 	//��������Ĵ���

u32   	U32_Cal_Times;                 		//У��ʱ��

u32   	U32_P_REF_PLUSEWIDTH_TIME;      	//�ο����� ��������
u32   	U32_V_REF_PLUSEWIDTH_TIME;      	//�ο���ѹ ��������
u32   	U32_I_REF_PLUSEWIDTH_TIME;      	//�ο����� ��������

u32   	U32_P_CURRENT_PLUSEWIDTH_TIME;      	//��ǰ���� ��������
u32   	U32_V_CURRENT_PLUSEWIDTH_TIME;      	//��ǰ��ѹ ��������
u32   	U32_I_CURRENT_PLUSEWIDTH_TIME;      	//��ǰ���� ��������

u16   	U16_P_REF_Data;				//�ο�����ֵ,����1000WУ����1000.0W
u16   	U16_V_REF_Data;				//�ο���ѹ  220.0V
u16   	U16_I_REF_Data;				//�ο�����  1000W,220V��������4.545A

u8    	U8_CURR_WorkMode;
//--------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
/*=====================================================
 * Function : void Init_Rom(void)
 * Describe : rom�ռ��ʼ��
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void Init_Rom(void)
{
    U8_CURR_WorkMode = D_NORMAL_MODE;
    //U8_DspMode = 0;

    power_usr.U16_AC_P = 0;
    power_usr.U16_AC_V = 0;
    power_usr.U16_AC_I = 0;
    U8_AC_COS = 0;


    U16_P_TotalTimes = 0;
    U16_V_TotalTimes = 0;
    U16_I_TotalTimes = 0;


    U16_P_OneCycleTime = 0;
    U16_V_OneCycleTime = 0;
    U16_I_OneCycleTime = 0;
    U16_P_Last_OneCycleTime = 0;
    U16_V_Last_OneCycleTime = 0;
    U16_I_Last_OneCycleTime = 0;

    U16_P_CNT = 0;
    U16_V_CNT = 0;
    U16_I_CNT = 0;
    U16_P_Last_CNT = 0;
    U16_V_Last_CNT = 0;
    U16_I_Last_CNT = 0;

    //��ʼ�������ڲ���ģʽ
    B_P_TestOneCycle_Mode = 1;
    B_V_TestOneCycle_Mode = 1;
    B_I_TestOneCycle_Mode = 1;
    B_P_Last_TestOneCycle_Mode = 1;
    B_V_Last_TestOneCycle_Mode = 1;
    B_I_Last_TestOneCycle_Mode = 1;

    //��ʼ�������������־λΪ1
    B_P_OVERFLOW = 1;
    B_V_OVERFLOW = 1;
    B_I_OVERFLOW = 1;

    B_P_Last_OVERFLOW = 1;
    B_V_Last_OVERFLOW = 1;
    B_I_Last_OVERFLOW = 1;

    //�ϵ��ʼ��Ϊ��ѹ����ģʽ
    B_VI_Test_Mode = 1;
    IO_TM7780_CF1_S = 1;
    U16_VI_Test_Times = D_TIME1_2S;

    //��ʾ��ʼ��
    //  U8_DspMode = 0;
    //  B_DIS_EN = FALSE;
    //  U16_SendUart_Time = 0;

}
/*=====================================================
 * ��������: PC�ж�,IO---PC5
 * ��������: CF�ж� ���ʲ���
 * �������: ��
 * �������:
 * ��    ��:
=====================================================*/
//  #pragma vector = 7
void P_Interrupt(void)
{
	//printf("LOW4 = %d\r\n", B_V_OVERFLOW);

    //���ʲ���
    U16_P_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
    U16_P_CNT++;
    if (B_P_OVERFLOW == TRUE)
    {
        //�����ģʽת��,��ʼ����
        B_P_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
        U16_P_TotalTimes = 0;       //������Ĵ�������
        U16_P_OneCycleTime = 0;     //������Ĵ���
        U16_P_CNT = 1;
        B_P_OVERFLOW = FALSE;       //�������־λ
    }
    else
    {
        if (B_P_TestOneCycle_Mode == 1)
        {
            if (U16_P_OneCycleTime >= D_TIME1_100MS)
            {
                //�����ڲ���ģʽ
                U16_P_Last_OneCycleTime = U16_P_OneCycleTime;
                B_P_Last_TestOneCycle_Mode = B_P_TestOneCycle_Mode;
                B_P_OVERFLOW = FALSE;       //�����־λ����
                B_P_Last_OVERFLOW = B_P_OVERFLOW;
                //��״̬����,���¿�ʼ����
                B_P_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                U16_P_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                U16_P_OneCycleTime = 0;     //������Ĵ���
                U16_P_CNT = 1;

            }
        }
        else
        {
            if (U16_P_OneCycleTime >= D_TIME1_1S)
            {
                U16_P_Last_OneCycleTime = U16_P_OneCycleTime;
                U16_P_Last_CNT = U16_P_CNT;
                B_P_Last_TestOneCycle_Mode = B_P_TestOneCycle_Mode;
                B_P_OVERFLOW = FALSE;       //�����־λ����
                B_P_Last_OVERFLOW = B_P_OVERFLOW;
                //��״̬����,���¿�ʼ����
                B_P_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                U16_P_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                U16_P_OneCycleTime = 0;     //������Ĵ���
                U16_P_CNT = 1;
            }
        }
    }


    //У��ģʽ
    if (U8_CURR_WorkMode == D_CAL_START_MODE)
    {
        //��¼��λʱ���ڵ��õ���
        U16_E_Pluse_CNT++;
    }

    //�õ���������ÿ0.01�ȵ磬�õ����Ĵ�������0.01��
    if (U8_CURR_WorkMode == D_NORMAL_MODE)
    {
        U16_E_Pluse_CNT++;
        if (U16_E_Pluse_CNT == U16_REF_001_E_Pluse_CNT)
        {
            U16_E_Pluse_CNT = 0;
            power_usr.U32_AC_E++;
        }
    }
}
/*=====================================================
 * ��������: PD�ж�,IO---PD2
 * ��������: CF1�ж� ��ѹ����������
 * �������:
 * �������:
 * ��    ��:
=====================================================*/
//  #pragma vector = 8
void VI_Interrupt(void)
{
	//printf("LOW3 = %d\r\n", B_V_OVERFLOW);
    
    //��ѹ����ģʽ
    if (B_VI_Test_Mode == 1)
    {   
        U16_V_TotalTimes = 0;
        U16_V_CNT++;
		
	  //  printf("CNT = %d\r\n", U16_V_CNT);
        if (B_V_OVERFLOW == TRUE)
        {
         // printf("LOW = %d\r\n", B_V_OVERFLOW);
            //�����ģʽת��,��ʼ����
            B_V_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
            U16_V_TotalTimes = 0;       //������Ĵ�������
            U16_V_OneCycleTime = 0;     //������Ĵ���
            U16_V_CNT = 1;
            B_V_OVERFLOW = FALSE;       //�������־λ
        }
        else
        {   
            if (B_V_TestOneCycle_Mode == 1)
            {
                if (U16_V_OneCycleTime >= D_TIME1_100MS)
                {
                    //�����ڲ���ģʽ
                    U16_V_Last_OneCycleTime = U16_V_OneCycleTime;
                    B_V_Last_TestOneCycle_Mode = B_V_TestOneCycle_Mode;
                    B_V_OVERFLOW = FALSE;       //�����־λ����
                    B_V_Last_OVERFLOW = B_V_OVERFLOW;
                    //��״̬����,���¿�ʼ����
                    B_V_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                    U16_V_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                    U16_V_OneCycleTime = 0;     //������Ĵ���
                    U16_V_CNT = 1;
                }
            }
            else
            {
                if (U16_V_OneCycleTime >= D_TIME1_200MS)
                {
                    U16_V_Last_OneCycleTime = U16_V_OneCycleTime;
                    U16_V_Last_CNT = U16_V_CNT;
                    B_V_Last_TestOneCycle_Mode = B_V_TestOneCycle_Mode;
                    B_V_OVERFLOW = FALSE;       //�����־λ����
                    B_V_Last_OVERFLOW = B_V_OVERFLOW;
                    //��״̬����,���¿�ʼ����
                    B_V_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                    U16_V_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                    U16_V_OneCycleTime = 0;     //������Ĵ���
                    U16_V_CNT = 1;
                    B_V_OVERFLOW = FALSE;       //�����־λ����
                }
            }
        }
    }

    //��������ģʽ
    if (B_VI_Test_Mode == 0)
    {
        U16_I_TotalTimes = 0;
        U16_I_CNT++;
		//printf("CNT2 = %d\r\n", U16_V_CNT);
        if (B_I_OVERFLOW == TRUE)
        {
       // printf("LOW2 = %d\r\n", B_V_OVERFLOW);
            //�����ģʽת��,��ʼ����
            B_I_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
            U16_I_TotalTimes = 0;       //������Ĵ�������
            U16_I_OneCycleTime = 0;     //������Ĵ���
            U16_I_CNT = 1;
            B_I_OVERFLOW = FALSE;       //�������־λ
        }
        else
        {
            if (B_I_TestOneCycle_Mode == 1)
            {
                if (U16_I_OneCycleTime >= D_TIME1_100MS)
                {
                    //�����ڲ���ģʽ
                    U16_I_Last_OneCycleTime = U16_I_OneCycleTime;
                    B_I_Last_TestOneCycle_Mode = B_I_TestOneCycle_Mode;
                    B_I_OVERFLOW = FALSE;       //�����־λ����
                    B_I_Last_OVERFLOW = B_I_OVERFLOW;
                    //��״̬����,���¿�ʼ����
                    B_I_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                    U16_I_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                    U16_I_OneCycleTime = 0;     //������Ĵ���
                    U16_I_CNT = 1;
                }
            }
            else
            {
                if (U16_I_OneCycleTime >= D_TIME1_1S)
                {
                    U16_I_Last_OneCycleTime = U16_I_OneCycleTime;
                    U16_I_Last_CNT = U16_I_CNT;
                    B_I_Last_TestOneCycle_Mode = B_I_TestOneCycle_Mode;
                    B_I_OVERFLOW = FALSE;       //�����־λ����
                    B_I_Last_OVERFLOW = B_I_OVERFLOW;
                    //��״̬����,���¿�ʼ����
                    B_I_TestOneCycle_Mode = 0;  //��ʼ��Ϊ�����������ģʽ
                    U16_I_TotalTimes = 0;       //���һ����Ч�Ĳ���������Ĵ�������
                    U16_I_OneCycleTime = 0;     //������Ĵ���
                    U16_I_CNT = 1;
                }
            }
        }
    }
}
void set_mode(u8 mode)
{
	switch(mode)
	{
		case D_CAL_START_MODE:U8_CURR_WorkMode = D_CAL_START_MODE;break;
		case D_NORMAL_MODE:U8_CURR_WorkMode = D_NORMAL_MODE;break;		
		case D_CAL_END_MODE:U8_CURR_WorkMode = D_CAL_END_MODE;break;
    default:		U8_CURR_WorkMode = D_NORMAL_MODE;break;		
		
	}
}
/*=====================================================
 * ��������: TIM1
 * ��������: �������ʡ���ѹ������
 * �������:
 * �������:
 * ��    ��:
=====================================================*/

void TIM1_UPD_Interrupt(void)
{
    //У��ģʽ
    if (U8_CURR_WorkMode == D_CAL_START_MODE)
    {
        UartSend(0x5b);
        U32_Cal_Times++;//У��ʱ��36S��1000W����36Sʱ������0.01�ȵ�
        if (U32_Cal_Times == D_TIME1_CAL_TIME)
        {
            UartSend(0x5c);
            U8_CURR_WorkMode = D_CAL_END_MODE;
            U16_REF_001_E_Pluse_CNT = U16_E_Pluse_CNT;		//��¼36Sʱ���ڵ�������������������ʾ0.01���õ���
        }
    }

    //���ʲ���
    if (U16_P_CNT != 0)
    {
        U16_P_OneCycleTime++;
        U16_P_TotalTimes++;

    }
    if (U16_P_TotalTimes >= D_TIME1_P_OVERFLOW)
    {
        B_P_OVERFLOW = TRUE; 		//�����
        B_P_Last_OVERFLOW = B_P_OVERFLOW;
        //��״̬����,���¿�ʼ����
        U16_P_TotalTimes = 0;       //������Ĵ���
        U16_P_OneCycleTime = 0;
        U16_P_CNT = 0;              //�ȴ���һ���жϿ�ʼ����
        B_P_TestOneCycle_Mode = 0;   //��ʼ��Ϊ�����������ģʽ
    }
    else if (U16_P_OneCycleTime == D_TIME1_100MS)
    {
        if (U16_P_CNT < 2)
        {
            // 100ms��ֻ��һ���жϣ�˵������>100ms,���õ����ڲ���ģʽ
            B_P_TestOneCycle_Mode = 1;
        }
        else
        {
            // 100ms����2�λ������������壬˵������<100ms�����ü����������ģʽ
            B_P_TestOneCycle_Mode = 0;
        }
    }

    //��ѹ����������
    if (B_VI_Test_Mode == 1)
    {
        //��ѹ����
        if (U16_V_CNT != 0)
        {
            U16_V_OneCycleTime++;
            U16_V_TotalTimes++;
        }
        if (U16_V_TotalTimes >= D_TIME1_V_OVERFLOW)
        {
            B_V_OVERFLOW = TRUE;
            B_V_Last_OVERFLOW = B_V_OVERFLOW;
            //��״̬����,���¿�ʼ����
            U16_V_TotalTimes = 0;       //������Ĵ���
            U16_V_OneCycleTime = 0;
            U16_V_CNT = 0;
            B_V_TestOneCycle_Mode = 0;   //��ʼ��Ϊ�����������ģʽ
        }
        else if (U16_V_OneCycleTime == D_TIME1_100MS)
        {
            if (U16_V_CNT < 2)
            {
                // 100ms��ֻ��һ���жϣ�˵������>100ms,���õ����ڲ���ģʽ
                B_V_TestOneCycle_Mode = 1;
            }
            else
            {
                // 100ms����2�λ������������壬˵������<100ms�����ü����������ģʽ
                B_V_TestOneCycle_Mode = 0;
            }
        }
    }
    else
    {
        //��������
        if (U16_I_CNT != 0)
        {
            U16_I_OneCycleTime++;
            U16_I_TotalTimes++;
        }
        if (U16_I_TotalTimes >= D_TIME1_I_OVERFLOW)
        {
            B_I_OVERFLOW = TRUE;
            B_I_Last_OVERFLOW = B_I_OVERFLOW;
            //��״̬����,���¿�ʼ����
            U16_I_TotalTimes = 0;       //������Ĵ���
            U16_I_OneCycleTime = 0;
            U16_I_CNT = 0;
            B_I_TestOneCycle_Mode = 0;   //��ʼ��Ϊ�����������ģʽ
        }
        else if (U16_I_OneCycleTime == D_TIME1_100MS)
        {
            if (U16_I_CNT < 2)
            {
                // 100ms��ֻ��һ���жϣ�˵������>100ms,���õ����ڲ���ģʽ
                B_I_TestOneCycle_Mode = 1;
            }
            else
            {
                // 100ms����2�λ������������壬˵������<100ms�����ü����������ģʽ
                B_I_TestOneCycle_Mode = 0;
            }
        }
    }


    //��ѹ����������ģʽ�л�  B_VI_Test_Mode:(1:��ѹ����ģʽ) (0:��������ģʽ)
    U16_VI_Test_Times--;
    if (U16_VI_Test_Times == 0)
    {
        if (B_VI_Test_Mode == 1)
        {
            //תΪ��������ģʽ
            B_VI_Test_Mode = 0;
            IO_TM7780_CF1_S = 0;
            U16_VI_Test_Times = D_TIME1_10S;

            //��״̬����
            U16_I_TotalTimes = 0;
            U16_I_OneCycleTime = 0;
            U16_I_CNT = 0;
            B_I_OVERFLOW = FALSE;
        }
        else
        {
            //תΪ��ѹ����ģʽ
            B_VI_Test_Mode = 1;
            IO_TM7780_CF1_S = 1;
            U16_VI_Test_Times = D_TIME1_1S;

            //��״̬����
            U16_V_TotalTimes = 0;
            U16_V_OneCycleTime = 0;
            U16_V_CNT = 0;
            B_V_OVERFLOW = FALSE;
        }
    }

    //����ɨ��,20mSһ��
    //	U8_KeyScanTimes++;
    //  	if (U8_KeyScanTimes > D_TIME1_20MS)
    //  	{
    //      		B_KeyScanEn = TRUE;
    //	  	U8_KeyScanTimes = 0;
    //  	}

    // ÿ0.5S���ڷ���һ������
    //    U16_SendUart_Time++;
    //    if (U16_SendUart_Time == D_TIME1_500MS)
    //    {
    //        B_DIS_EN = TRUE;
    //        U16_SendUart_Time = 0;
    //    }

    // Clear TIM1 INT FLAG
    // TIM1_SR1 = 0x00;
}



/*-------------------------------------------- ���ʡ���ѹ���������� -------------------------------------------*/


/*=====================================================
 * Function : void TM7780_Measure_P(void)
 * Describe :
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure_P(void)
{
    u32 a;
    u32 b;
    u32 u32_P_Period;

    if (B_P_Last_TestOneCycle_Mode == 1)
    {
        //�����ڲ���ģʽ
        b = U16_P_Last_OneCycleTime;
        b = b * 1000; //msת����us
        u32_P_Period = b;
    }
    else
    {
        //�����������ģʽ
        b = U16_P_Last_OneCycleTime;
        b = b * 1000;
        u32_P_Period = b / (U16_P_Last_CNT - 1);
    }

    U32_P_CURRENT_PLUSEWIDTH_TIME = u32_P_Period;      // У��ʱȡU32_P_CURRENT_PLUSEWIDTH_TIME������Ϊ�ο�ֵ
    a = U16_P_REF_Data * U32_P_REF_PLUSEWIDTH_TIME;
    power_usr.U16_AC_P = a / U32_P_CURRENT_PLUSEWIDTH_TIME;
	

	//printf("REF_Data = %d\r\n", U16_P_REF_Data);
	//printf("ac_p = %d\r\n", power_usr.U16_AC_P);
	
    if (power_usr.U16_AC_P == 0xffff)     //����ʱU32_P_CURRENT_PLUSEWIDTH_TIME = 0���������
    {
        power_usr.U16_AC_P = 0;
    }

    if (B_P_Last_OVERFLOW == TRUE)
    {
        power_usr.U16_AC_P = 0;
    }
}
/*=====================================================
 * Function : void TM7780_Measure_V(void)
 * Describe :
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure_V(void)
{
    u32 a;
    u32 b;
    u32 u32_V_Period;

    if (U16_VI_Test_Times < D_TIME1_100MS)
    {
        if (B_V_Last_TestOneCycle_Mode == 1)
        {
            b = U16_V_Last_OneCycleTime;
            u32_V_Period = b * 1000;   //msת����us
            U32_V_CURRENT_PLUSEWIDTH_TIME = u32_V_Period;
        }
        else
        {
            b = U16_V_Last_OneCycleTime;
            b = b * 1000;
            u32_V_Period = b / (U16_V_Last_CNT - 1);
            //u32_V_Period = b/U16_V_Last_CNT;
            U32_V_CURRENT_PLUSEWIDTH_TIME = u32_V_Period;
        }
    }

    a = U16_V_REF_Data * U32_V_REF_PLUSEWIDTH_TIME;
    power_usr.U16_AC_V = a / U32_V_CURRENT_PLUSEWIDTH_TIME;

    if (power_usr.U16_AC_V == 0xffff)     //����ʱU32_V_CURRENT_PLUSEWIDTH_TIME = 0���������
    {
        power_usr.U16_AC_V = 0;
    }

    if (B_V_Last_OVERFLOW == TRUE)
    {
        power_usr.U16_AC_V = 0;
    }

    if (power_usr.U16_AC_P == 0)
    {
        power_usr.U16_AC_V = 0;

    }
}
/*=====================================================
 * Function : void TM7780_Measure_I(void)
 * Describe :
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure_I(void)
{
    u32 a;
    u32 b;
    u32 u32_I_Period;

    if (U16_VI_Test_Times < D_TIME1_6S)
    {
        if (B_I_Last_TestOneCycle_Mode == 1)
        {

            b = U16_I_Last_OneCycleTime;
            u32_I_Period = b * 1000;   //msת����us
            U32_I_CURRENT_PLUSEWIDTH_TIME = u32_I_Period;
        }
        else
        {
            b = U16_I_Last_OneCycleTime;
            b = b * 1000;
            u32_I_Period = b / (U16_I_Last_CNT - 1);
            //u32_I_Period = b/U16_I_Last_CNT;
            U32_I_CURRENT_PLUSEWIDTH_TIME = u32_I_Period;
        }
    }


    a = U16_I_REF_Data * U32_I_REF_PLUSEWIDTH_TIME;
    power_usr.U16_AC_I = a / U32_I_CURRENT_PLUSEWIDTH_TIME;


    if (power_usr.U16_AC_I > 180)
    {
        if (power_usr.U16_AC_I != 0xffff)
        {
            ;//_NOP();
            ;//_NOP();
        }
    }

    if (power_usr.U16_AC_P == 0)
    {
        power_usr.U16_AC_I = 0;

    }

    if (power_usr.U16_AC_I == 0xffff)     //����ʱU32_I_CURRENT_PLUSEWIDTH_TIME = 0���������
    {
        power_usr.U16_AC_I = 0;
    }

    if (B_I_OVERFLOW == TRUE)
    {
        power_usr.U16_AC_I = 0;
    }
}
/*=====================================================
 * Function : void TM7780_Measure_COS(void)
 * Describe :
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure_COS(void)
{
    u32 u32_P_Apparent;
    u32 a;
    u8 b;
    u32_P_Apparent = (u32)power_usr.U16_AC_I * (u32)power_usr.U16_AC_V;
    u32_P_Apparent = u32_P_Apparent / 1000;
    a = power_usr.U16_AC_P;
    a = a * 100;

    b = (u8)(a / u32_P_Apparent);
    if (b > 100)
        U8_AC_COS = 100; 		//������λ��ʱ��ɵĹ�������>1���������������ȡֵ1
    else
        U8_AC_COS = b;
}
/*=====================================================
 * Function : void TM7780_Measure_E(void)
 * Describe : ÿ1�ȵ�洢һ��
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure_E(void)
{
    if (U32_AC_BACKUP_E < power_usr.U32_AC_E)
    {
        U32_AC_BACKUP_E = power_usr.U32_AC_E;
        //ÿ����0.01�ȵ���и��£������õ����ݴ���EEPROM
        //Updata_BackUp_E_EEPROM();
    }
}
/*=====================================================
 * Function : void TM7780_Measure(void)
 * Describe :
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2014/04/14
=====================================================*/
void TM7780_Measure(void)
{
    TM7780_Measure_P();
    TM7780_Measure_V();
    TM7780_Measure_I();
    TM7780_Measure_COS();
    TM7780_Measure_E();
}


/*-------------------------------------------- EEPROM -------------------------------------------*/

/*=====================================================
 * ��������: void Read_CalData_EEPROM(void)
 * ��������:
 * �������:
 * �������:
 * ��    ��:
=====================================================*/
void Read_CalData_EEPROM(void)
{
    u8  u8_temp;

    u8_temp = EEPROM_Read_Byte(ADDR_CAL_FLAG);
    U32_P_REF_PLUSEWIDTH_TIME = EEPROM_Read_Word(ADDR_REF_P_PLUSEWIDTH_TIME);
    U32_V_REF_PLUSEWIDTH_TIME = EEPROM_Read_Word(ADDR_REF_V_PLUSEWIDTH_TIME);
    U32_I_REF_PLUSEWIDTH_TIME = EEPROM_Read_Word(ADDR_REF_I_PLUSEWIDTH_TIME);

    U16_REF_001_E_Pluse_CNT = EEPROM_Read_Inte(ADDR_REF_001_E);
    U32_AC_BACKUP_E = EEPROM_Read_Word(ADDR_AC_BACKUP_E);
    power_usr.U32_AC_E = U32_AC_BACKUP_E;
	//UartSendStr("eeprom ok\r\n");


    if (u8_temp != 0x55)	//�ж�EEPROM���Ƿ����У��ֵ,��û�У��򽫲�������ΪĬ��ֵ
    { // UartSendStr("eeprom read ok\r\n");
        
        //����Ĭ��ֵ
        U16_P_REF_Data = 10000;		// 1000.0W
        U16_V_REF_Data = 2200;		// 220.0V
        U16_I_REF_Data = 4545;		// 4.545A

        //Ĭ������
        U32_P_REF_PLUSEWIDTH_TIME = 4975;     // 4975us
        U32_V_REF_PLUSEWIDTH_TIME = 1666;      // 1666us
        U32_I_REF_PLUSEWIDTH_TIME = 1666;      // 1666us
        U32_AC_BACKUP_E = 0;
        power_usr.U32_AC_E = 0;
    }
}

/*=====================================================
 * ��������: void Write_CalData_EEPROM(void)
 * ��������:
 * �������:
 * �������:
 * ��    ��:
=====================================================*/
void Write_CalData_EEPROM(void)
{
	IapErase(ADDR_CAL_FLAG);

    EEPROM_Write_Word(ADDR_REF_P_PLUSEWIDTH_TIME, U32_P_CURRENT_PLUSEWIDTH_TIME);
    EEPROM_Write_Word(ADDR_REF_V_PLUSEWIDTH_TIME, U32_V_CURRENT_PLUSEWIDTH_TIME);
    EEPROM_Write_Word(ADDR_REF_I_PLUSEWIDTH_TIME, U32_I_CURRENT_PLUSEWIDTH_TIME);
    EEPROM_Write_Inte(ADDR_REF_001_E, U16_REF_001_E_Pluse_CNT);
    EEPROM_Write_Word(ADDR_AC_BACKUP_E, 0x00);

    EEPROM_Write_Byte(ADDR_CAL_FLAG, 0x55);

    U32_AC_BACKUP_E = 0;
    power_usr.U32_AC_E = 0;
}



power_stru *get_power(void)
{
    return &power_usr;
}

void tm7780_init(void)
{
    //初始化MCU
    //初始化ROM
    Init_Rom();
    //读取存放于EEPROM的校正数�?
    Read_CalData_EEPROM(); //读取参考功率、参考电压、参考电流的脉冲周期


}
void tm7780_proc()
{
    if(power_usr.tm_cfv_edge != IO_TM7780_CF1)
    {
		power_usr.tm_cfv_edge = IO_TM7780_CF1;
		VI_Interrupt();
	}
    if(power_usr.tm_cfp_edge != IO_TM7780_CF)
    {
		power_usr.tm_cfv_edge = IO_TM7780_CF;
		 P_Interrupt();
	}	
    if (U8_CURR_WorkMode == D_NORMAL_MODE)
    {
        TM7780_Measure();		//计算功率、电压、电�?
    }
    else if (U8_CURR_WorkMode == D_CAL_END_MODE)	//校正模式
    {
        //记录校正值，将当前功率、电压、电流值存入EEPROM;�?.01度电的脉冲数参值也存入EEPROM
        Write_CalData_EEPROM();
        //读取校正值，将用电量清零
        Read_CalData_EEPROM();
        //返回测量模式
        U8_CURR_WorkMode = D_NORMAL_MODE;
		UartSend(0x5a);
    }
}


