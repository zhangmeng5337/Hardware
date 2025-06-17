/*****************************************
*        CS1237 designed by Robert Lee           *
*                                        *
*****************************************/
#include "cs1237.h"
#include "stdio.h"
#include "main.h"


//#ifdef USER_CS1237  
#define ADC_Bit  24 //ADC��Чλ����������λ ���24λ
#define CS1237_SCL_H  SetGpio(10, 0)
#define CS1237_SCL_L  ClearGpio(10, 0)
#define CS1237_SDA_H  SetGpio(10, 1)
#define CS1237_SDA_L  ClearGpio(10, 1)
#define        NOP_5()                __asm {nop};__asm {nop};__asm {nop}
#define        NOP30()                NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define        NOP40()                NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define One_CLK  CS1237_SCL_H;delay_us(1);CS1237_SCL_L;delay_us(1);
#define CS_CON  0X00   //оƬ������ �ڲ�REF ���10HZ PGA=1 ͨ��A 0X00  
#define Lv_Bo 0.01  //�˲�ϵ�� С��1

//sbit DOUT = P3^7;//���ݶ�ӦIO��
//sbit SCLK = P3^5;//ʱ�Ӷ�ӦIO��
static long AD_Res_Last=0;//��һ�ֵ�ADC��ֵ����

//��ʱ500US 5.5296MHZ
void delay_500us(volatile unsigned char a)
{        
        volatile unsigned char i,j,b;
        for(b=0;b<a;b++)
        {
                i = 3;
                j = 137;
                do
                {
                        while (--j);
                } while (--i);
        }
}
//CS1237����͹���ģʽ
void CS1237_Power_Down(void)
{
        //SCLK = 1;
        CS1237_SCL_H;
        delay_500us(100);
        //SCLK = 1;
        CS1237_SCL_H;
        //SCLK = 1;
        CS1237_SCL_H;
        delay_500us(100);
}
//����CS1237оƬ
void Con_CS1237(void)
{
        unsigned char i;
        unsigned char dat;
        unsigned short count_i=0;//�����ʱ��

        SetPinMux(10, 0, 0);//SCK
        SetPinMux(10, 1, 0);//SDA

        SetPinDir(10,0,1);
        SetPinDir(10,1,0);
        
        dat = CS_CON;// 0100 1000
        CS1237_SCL_L;//ʱ������
        while(CS1237_SDA_READ ==1)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237ȫ������Ϊ0���㶼׼����
        {
                delay_500us(10);
                count_i++;
                if(count_i > 15000)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("here error\r\n");
                        return;//��ʱ����ֱ���˳�����
                }
        }
        for(i=0;i<29;i++)// 1 - 29
        {
                One_CLK;
        }
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//30
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//31
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//32
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//33
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//34
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//35
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//36
        One_CLK;//37     д����0x65
        for(i=0;i<8;i++)// 38 - 45�������ˣ�д8λ����
        {
                CS1237_SCL_H;NOP40();
                if(dat&0x80)
                        CS1237_SDA_H;
                else
                        CS1237_SDA_L;
                dat <<= 1;
                CS1237_SCL_L;NOP40();
        }
        One_CLK;//46������������������
}
//��ȡоƬ����������
unsigned char Read_CON(void)
{
        unsigned char i;
        unsigned char dat=0;//��ȡ��������
        unsigned short count_i=0;//�����ʱ��
        unsigned char k=0,j=0;//�м����
        CS1237_SCL_L;//ʱ������
        while(CS1237_SDA_READ ==1)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237ȫ������Ϊ0���㶼׼����
        {
                delay_500us(10);
                count_i++;
                if(count_i > 15000)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("here outtime error\r\n");
                        return 1;//��ʱ����ֱ���˳�����
                }
        }
        for(i=0;i<29;i++)// 1 - 29
        {
                One_CLK;
        }
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//30
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//31
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//32
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//33
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//34
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//35
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//36
        CS1237_SDA_H;
        One_CLK;//37     д����0x56
        dat=0;
        for(i=0;i<8;i++)// 38 - 45�������ˣ���ȡ����
        {
                One_CLK;
                dat <<= 1;
                if(CS1237_SDA_READ ==1)
                        dat++;
        }
        One_CLK;//46������������������
        return dat;
}
//��ȡADC���ݣ����ص���һ���з�������
long Read_CS1237(void)
{
        unsigned char i;
        long dat=0;//��ȡ��������
        unsigned char count_i=0;//�����ʱ��
//        DOUT = 1;//�˿�����1��51�ر�
        CS1237_SCL_L;//ʱ������
        while(CS1237_SDA_READ ==1)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237����Ϊ0���㶼׼����
        {
                delay_500us(10);
                count_i++;
                if(count_i > 300)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("Read_CS1237 outtime error\r\n");
                        return 0;//��ʱ����ֱ���˳�����
                }
        }
//        DOUT = 1;//�˿�����1��51�ر�
        dat=0;
        for(i=0;i<24;i++)//��ȡ24λ��Чת��
        {
                CS1237_SCL_H;
                NOP40();
                dat <<= 1;
                if(CS1237_SDA_READ ==1)
                        dat ++;
                CS1237_SCL_L;
                NOP40();        
        }
        for(i=0;i<3;i++)//һ������27������
        {
                One_CLK;
        }
        //CS1237_SDA_H;
        //�ȸ��ݺ궨���������Чλ������һЩ����
        i = 24 - ADC_Bit;//i��ʾ��Ҫ������λ��
        dat >>= i;//���������λ��
        return dat;
}
//��ʼ��ADC��ز���
void Init_CS1237(void)
{
        Con_CS1237();//����CS1237
        if(Read_CON() != CS_CON)//�����ȡ��ADC���ó���������
        {
                printf("Init_CS1237 error\r\n");
        }
                //IAP_CONTR = 0x20;
        //AD_Res_Last = Read_CS1237();
        //AD_Res_Last = Read_CS1237();
        //AD_Res_Last = Read_CS1237();
        //printf("AD_Res_Last 0x%x\r\n",AD_Res_Last);
}
//����һ���˲��� �˲�ϵ��A��С��1����һ����ֵB��������ֵC out = b*A + C*(1-A)
//��������������ȡ������ADC����
long Read_12Bit_AD(void)
{
        float out,c;
        
        out = AD_Res_Last;
        c = Read_CS1237();
        if(out !=0 && (out<c*0.8 || out>c*1.2))
          c = out;
        if(c!=0) // ������ȷ����
        {
                out = out*Lv_Bo + c*(1-Lv_Bo);
                AD_Res_Last = out;//����εļ������ŵ�ȫ�ֱ������汣��
        }
        //printf("AD_Res_Last 0x%x\r\n",AD_Res_Last);
        return AD_Res_Last;
}
//#endif

