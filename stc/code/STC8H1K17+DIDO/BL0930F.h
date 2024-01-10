#ifndef BL0930F_H_
#define BL0930F_H_
#include "main.h" 
#define BL_I_WAVE      0x01
#define BL_V_WAVE      0x02
#define BL_I_RMS       0x03//电流A有效值
#define BL_V_RMS       0x04//电压有效值
#define BL_I_FAST_RMS  0x05
#define BL_WATT        0x06
#define BL_CF_CNT      0x07
#define BL_FREQ        0x08//线电压频率
#define BL_STATUS      0x09
//////////////////////////读写区域/////////////////////////////////////////////
#define BL_I_RMSOS         0x12   //电流小信号校准
#define BL_WA_CREEP        0x14   //防潜动
#define BL_I_FAST_RMS_TH   0x15   //电流有效阈值
#define BL_I_FAST_RMS_CYC  0x16   //电流刷新周期
#define BL_FREQ_CYC        0x17   //线电压频率刷新周期
#define BL_OT_FUNX         0x18   //输出配置寄存器
#define BL_MODE            0x19
#define BL_GAIN_CR         0x1A   //电流模拟增益控制
#define BL_SOFT_RESET      0x1C   
#define BL_USR_WRPROT      0x1D   
//////////////////////////软件校表OTP/////////////////////////////////////////////
#define BL_OTP_BLOCK      0x20
#define BL_OTP_CHCKSUM    0x2F
#define BL_OTP_WRPROT     0x31
#define BL_OTP_CTRL       0x32
#define BL_OTP_STA        0x33
#define BL_OTP_SUMERR     0x34
//////////////////////////软件校表寄存器/////////////////////////////////////////////
#define BL_PHCAL         0x21
#define BL_I_CHGN        0x22 
#define BL_V_CHGN        0x23
#define BL_WATTOS        0x24
#define BL_OTP_WA_CREEP  0x25 //防潜动
#define BL_CHIP_MODE     0x26
#define BL_CFDIV         0x27

#define OTP_GAIN_CR      0x29
#define  BL_CHKSUM    0x2f


#define BL_PF           0x08//功率因子
#define BL_VA           0x0B//平均视在功率
#define BL_COUNTER_CF   0x0C//CF脉冲个数
#define BL_WRITECMD    0XA8
#define BL_READCMD     0X58
typedef struct
{
    unsigned int calc;
    unsigned int calc_result;

    long pulse;

    long vrms;     // XXX.X V
    signed long iarms;     // XX.XXX A
    signed long ibrms;     // XX.XXX A

    signed long watta;     // XXXX.X W
    signed long wattb;     // XXXX.X W

    long wattahr;
    long pwattahr;
    long nwattahr;

    long vara;
    long varahr;

    signed long vaa;
    long vaahr;

    long iapeak;
    long ibpeak;

    signed long factor;  // 0.XXX
    long freq;    // XX.XX

    int error;
} MIC;


#endif

