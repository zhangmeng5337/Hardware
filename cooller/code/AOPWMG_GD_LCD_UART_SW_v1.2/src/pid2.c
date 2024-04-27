#include <reg52.h>

typedef unsigned char      uChar8;     

typedef unsigned int       uInt16;

typedef unsigned long int   uInt32;

 

sbit ConOut = P1^1;     //����˿�ӵ�P1.1��

 

typedef struct  PID_Value

{

    uInt32 liEkVal[3];          //��ֵ���棬�����ͷ����Ĳ�ֵ

    uChar8 uEkFlag[3];          //���ţ�1���Ӧ��Ϊ������0Ϊ��Ӧ��Ϊ����   

    uChar8 uKP_Coe;             //����ϵ��

    uChar8 uKI_Coe;             //���ֳ���

    uChar8 uKD_Coe;             //΢�ֳ���

    uInt16 iPriVal;             //��һʱ��ֵ

    uInt16 iSetVal;             //�趨ֵ

    uInt16 iCurVal;             //ʵ��ֵ

}PID_ValueStr;

 

PID_ValueStr PID;               //����һ���ṹ�壬����ṹ���������㷨��Ҫ�õ��ĸ�������

bit g_bPIDRunFlag = 0;          //PID���б�־λ��PID�㷨����һֱ�����㡣����ÿ��һ��ʱ�䣬��һ�Ρ�

/* ********************************************************

/* �������ƣ�PID_Operation()                                 

/* �������ܣ�PID����                   

/* ��ڲ������ޣ��������룬ϵ�����趨ֵ�ȣ�                     

/* ���ڲ������ޣ����������U(k)��

/* ����˵����U(k)+KP*[E(k)-E(k-1)]+KI*E(k)+KD*[E(k)-2E(k-1)+E(k-2)]                                     

******************************************************** */

void PID_Operation(void)

{

    uInt32 Temp[3] = {0};   //�м���ʱ����

    uInt32 PostSum = 0;     //������

    uInt32 NegSum = 0;      //������

    if(PID.iSetVal > PID.iCurVal)                //�趨ֵ����ʵ��ֵ��

    {

        if(PID.iSetVal - PID.iCurVal > 10)      //ƫ�����10��

            PID.iPriVal = 100;                  //ƫ�����10Ϊ���޷�ֵ���(ȫ�ټ���)

        else                                    //����������

        {

            Temp[0] = PID.iSetVal - PID.iCurVal;    //ƫ��<=10,����E(k)

            PID.uEkFlag[1] = 0;                     //E(k)Ϊ����,��Ϊ�趨ֵ����ʵ��ֵ

            /* ��ֵ������λ��ע��˳�򣬷���Ḳ�ǵ�ǰ�����ֵ */

            PID.liEkVal[2] = PID.liEkVal[1];

            PID.liEkVal[1] = PID.liEkVal[0];

            PID.liEkVal[0] = Temp[0];

            /* =================================================================== */

            if(PID.liEkVal[0] > PID.liEkVal[1])              //E(k)>E(k-1)��

            {

                Temp[0] = PID.liEkVal[0] - PID.liEkVal[1];  //E(k)>E(k-1)

                PID.uEkFlag[0] = 0;                         //E(k)-E(k-1)Ϊ����

            }                                      

            else

            {

                Temp[0] = PID.liEkVal[1] - PID.liEkVal[0];  //E(k)<E(k-1)

                PID.uEkFlag[0] = 1;                         //E(k)-E(k-1)Ϊ����

            }                       

            /* =================================================================== */

            Temp[2] = PID.liEkVal[1] * 2;                   //2E(k-1)

            if((PID.liEkVal[0] + PID.liEkVal[2]) > Temp[2]) //E(k-2)+E(k)>2E(k-1)��

            {

                Temp[2] = (PID.liEkVal[0] + PID.liEkVal[2]) - Temp[2];

                PID.uEkFlag[2]=0;                           //E(k-2)+E(k)-2E(k-1)Ϊ����

            }                                              

            else                                            //E(k-2)+E(k)<2E(k-1)

            {

                Temp[2] = Temp[2] - (PID.liEkVal[0] + PID.liEkVal[2]);

                PID.uEkFlag[2] = 1;                         //E(k-2)+E(k)-2E(k-1)Ϊ����

            }                                  

            /* =================================================================== */

            Temp[0] = (uInt32)PID.uKP_Coe * Temp[0];        //KP*[E(k)-E(k-1)]

            Temp[1] = (uInt32)PID.uKI_Coe * PID.liEkVal[0]; //KI*E(k)

            Temp[2] = (uInt32)PID.uKD_Coe * Temp[2];        //KD*[E(k-2)+E(k)-2E(k-1)]

            /* ���²��ִ����ǽ����е���������ӣ���������� */

            /* ========= ����KP*[E(k)-E(k-1)]��ֵ ========= */

            if(PID.uEkFlag[0] == 0)

                PostSum += Temp[0];                         //������

            else                                            

                NegSum += Temp[0];                          //������

            /* ========= ����KI*E(k)��ֵ ========= */

            if(PID.uEkFlag[1] == 0)    

                PostSum += Temp[1];                         //������

            else

                ;   /* �ղ�����������ΪPID.iSetVal > PID.iCurVal����E(K)>0���Ž���if�ģ�

                    ��ô��û����Ϊ�������Դ��ת��ȥ������ */

            /* ========= ����KD*[E(k-2)+E(k)-2E(k-1)]��ֵ ========= */

            if(PID.uEkFlag[2]==0)

                PostSum += Temp[2];             //������

            else

                NegSum += Temp[2];              //������

            /* ========= ����U(k) ========= */                        

            PostSum += (uInt32)PID.iPriVal;        

            if(PostSum > NegSum)                 //�Ƿ������Ϊ����

            {

                Temp[0] = PostSum - NegSum;

                if(Temp[0] < 100 )               //С�����޷�ֵ��Ϊ����ֵ���

                    PID.iPriVal = (uInt16)Temp[0];

                else PID.iPriVal = 100;         //����Ϊ���޷�ֵ���

            }

            else                                //���������Ϊ�����������0(���޷�ֵ���)

                PID.iPriVal = 0;

        }

    }

    else PID.iPriVal = 0;                       //ͬ�ϣ��ٺ�

}

/* ********************************************************

/* �������ƣ�PID_Output()                                    

/* �������ܣ�PID�������                 

/* ��ڲ������ޣ��������룬U(k)��                        

/* ���ڲ������ޣ����ƶˣ�                                     

******************************************************** */

void PID_Output(void)

{

    static uInt16 iTemp;

    static uChar8 uCounter;

    iTemp = PID.iPriVal;

    if(iTemp == 0)

        ConOut = 1;     //������

    else ConOut = 0;    //����

    if(g_bPIDRunFlag)   //��ʱ�ж�Ϊ100ms(0.1S)����������10S(100��*0.1S)

    {

        g_bPIDRunFlag = 0;

        if(iTemp) iTemp--;      //ֻ��iTemp>0�����б�Ҫ����1��

        uCounter++;

        if(100 == uCounter)

        {

            PID_Operation();    //ÿ��0.1*100S����һ��PID���㡣

            uCounter = 0;  

        }

    }

}

/* ********************************************************

/* �������ƣ�PID_Output()                                    

/* �������ܣ�PID�������                 

/* ��ڲ������ޣ��������룬U(k)��                        

/* ���ڲ������ޣ����ƶˣ�                                     

******************************************************** */

void Timer0Init(void)

{

    TMOD |= 0x01;   // ���ö�ʱ��0������ģʽ1��

    TH0 = 0xDC;

    TL0 = 0x00;     // ����ʼֵ

    TR0 = 1;        // ����ʱ��0

    EA = 1;         // �����ж�

    ET0 = 1;        // ����ʱ���ж�

}

 

void main(void)

{

    Timer0Init();

    while(1)

    {

        PID_Output();

    }

}

 

void Timer0_ISR(void) interrupt 1

{

    static uInt16 uiCounter = 0;

    TH0 = 0xDC;

    TL0 = 0x00;

    uiCounter++;

    if(100 == uiCounter)

    {

        g_bPIDRunFlag = 1;

    }

}
��������������������������������
��Ȩ����������ΪCSDN������СС��ɫ�ܡ���ԭ�����£���ѭCC 4.0 BY-SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
ԭ�����ӣ�https://blog.csdn.net/xiaobaixiongxiong/article/details/83792580

