#include "BL0930F.h"
#include <stdlib.h>
unsigned char  Calstep;
typedef struct __packed
{
    unsigned int igain;
    unsigned int chipmode;
    unsigned int mode;

    unsigned int cfdiv;
    unsigned int kvrms;
    unsigned int kiarms;
    unsigned int kwatta;

    unsigned int ia_gain;
    unsigned int v_gain;
    unsigned int ia_phs;
    unsigned int watta_ofs;
    unsigned int wa_creep;

    long checksum;

#if 0
    unsigned int v_ofs;
    unsigned int ia_ofs;

    unsigned int ib_gain;
    unsigned int ib_phs;
    unsigned int ib_ofs;

    unsigned int watta_gain;
    unsigned int wattb_gain;
    unsigned int wattb_ofs;
    unsigned int va_gain;
    unsigned int va_offs;

    unsigned int vrms_gain;
    unsigned int iarms_gain;
    unsigned int ibrms_gain;

    unsigned int vrms_ofs;
    unsigned int iarms_ofs;
    unsigned int ibrms_ofs;
#endif

    unsigned int check;

} MIC_CAL;


typedef struct
{
    unsigned char SysStat_RnReadErr;
    unsigned char SysStat_RnWriteErr;
} Sysstate_stru;
Sysstate_stru Sysstate;
MIC mic;
MIC_CAL mic_cal;


xlong get_mic(unsigned char num)
{
    xlong result = 0;
	if(num == 0x06)
	{
		result = mic.iarms;
	}
	else if(num == 0x07)
	{
	result = mic.vrms;

	}
	else if(num == 0x08)
	{
	result = mic.iarms;

	}	
	return result;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



unsigned char  bl_buf[8];
//写入数据到ATT7053c的指定地址
void spi_write(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		dat=dat<<1;
		SCLK=1;delay();
		DIN=CY;
		SCLK=0;delay();
	}
}
//从ATT7053c的指定地址读出数据
unsigned char spi_read(uchar dat1)
{
	uchar i;
	unsigned char dat=0;
	for(i=0;i<8;i++)
	{
		dat=dat<<1;
		SCLK=1;delay();
		if(DOUT==1)dat++;
		SCLK=0;delay();
	}

	return(dat);
}

unsigned char uart1_send_receive(unsigned char *wr_buf, unsigned char tx_len, unsigned char *read_buf,unsigned char read_len)
{	
   unsigned char i;
   
   SCLK=0;
	CS=0;delay();
	for(i=0;i<tx_len;i++)
	{
    	spi_write(wr_buf[i]);

	}
	for(i=0;i<read_len;i++)
	{
    	read_buf[i]=spi_read(0);

	}

	CS=1;
   return 0;
}
void delay_ms(long ms)
{

}
/***************************************************************************************
函数名称:bl0930_reset(void)
函数作用:复位计量芯片
变量描述:
返回值：
***************************************************************************************/
void bl0930_reset(void)
{
    unsigned char  buf[35];
    unsigned char  i;
    for(i=0; i<32; i++)
    {
        buf[i]=0;

    }
    /*
        PT_MIC_RX_RST_1;
        PD_MIC_RX_RST;

        PT_MIC_RX_RST_0;
        delay_ms(50); // >20ms
        PT_MIC_RX_RST_1;
        delay_ms(1); // >300us
    */
    uart1_send_receive(buf, 32, 0, 0);
}
/***************************************************************************************
函数名称:bl0930_read(unsigned char  reg, long *data)
函数作用:读取寄存器的值（通过uart）
变量描述: reg 寄存器 *data读取的32值
返回值：
***************************************************************************************/
int bl0930_read(unsigned char  reg, long *datap)
{
    unsigned char  check = 0;
    unsigned char  i;
    for(i=0; i<8; i++)
    {
        bl_buf[i]=0;

    }
    bl_buf[0] = BL_READCMD;
    bl_buf[1] = reg; //寄存器


    if(uart1_send_receive(bl_buf, 2, bl_buf+2, 4) < 0)//收到数据不对
    {
        return -1;
    }
    check += bl_buf[0];
    check += bl_buf[1];
    check += bl_buf[2];//校验值
    check += bl_buf[3];
    check += bl_buf[4];
    check = ~check;

    if(check != bl_buf[5])//接收的校验值不对
    {
        return -3;
    }

    *datap = (long)bl_buf[2];
    *datap |= (long)bl_buf[3]<<8;
    *datap |= (long)bl_buf[4]<<16;

    if(bl_buf[4] & 0x80)
    {
        *datap |= 0xFF000000;
    }

    return 0;//读取成功
}
/***************************************************************************************
函数名称:bl0930_write(unsigned char  reg, long data)
函数作用:写寄存器的值（通过uart）
变量描述: reg 寄存器 *data读取的32值
返回值：
***************************************************************************************/
int bl0930_write(unsigned char  reg, long datap)
{
    unsigned char  check = 0;

    bl_buf[0] = BL_WRITECMD;
    bl_buf[1] = reg;
    bl_buf[2] = (unsigned char )(datap & 0xFF);
    bl_buf[3] = (unsigned char )((datap>>8) & 0xFF);
    bl_buf[4] = (unsigned char )((datap>>16) & 0xFF);

    check += bl_buf[0];
    check += bl_buf[1];
    check += bl_buf[2];
    check += bl_buf[3];
    check += bl_buf[4];
    check = ~check;
    bl_buf[5] = check;


    if(uart1_send_receive(bl_buf, 6, 0, 0) < 0)
    {
        return -1;
    }

    return 0;
}
/***************************************************************************************
函数名称:mic_read(unsigned char  reg, long *data)
函数作用:比较读取读取的数据
变量描述: reg 寄存器 *data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_read(unsigned char  reg, long *datap)
{
    unsigned char  i;
    for(i=0; i<5; i++)
    {
        if(bl0930_read(reg, datap) < 0)//读取寄存器的值
        {

        }
        else
        {
            break;
        }
        delay_ms(20);
    }
    if(i==5)
    {
        //	Sysstate.SysStat_RnReadErr=1;
        return -1;
    }
    else
    {
        //Sysstate.SysStat_RnReadErr=0;
        return 0;
    }
    /*
        if(bl0930_read(reg, data) < 0)//读取寄存器的值
        {
    			  Sysstate.SysStat_RnReadErr=1;
            return -1;
        }
    		else
    		{
    		   Sysstate.SysStat_RnReadErr=0;
    		}
        return 0;
    	*/
}
/***************************************************************************************
函数名称:mic_write(unsigned char  reg, long data)
函数作用:比较写寄存器的值
变量描述: reg 寄存器 data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_write(unsigned char  reg, long datap)
{
    if(bl0930_write(reg, datap) < 0)
    {
        Sysstate.SysStat_RnWriteErr=1;
        return -1;
    }
    else
    {
        //Sysstate.SysStat_RnWriteErr=0;
    }

    return 0;
}
/***************************************************************************************
函数名称:mic_write_Cmp(unsigned char  reg, long data)
函数作用:比较写寄存器的值
变量描述: reg 寄存器 data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_write_Cmp(unsigned char  reg, long datap)
{
    long value=0;
    if(mic_write(reg, datap)<0)
    {
        return -1;
    }
    delay_ms(5);
    if(mic_read(reg,&value)<0)
    {
        Sysstate.SysStat_RnReadErr=1;
        return -2;
    }
    if(value!=datap)
    {
        return -2;
    }

    return 0;
}


/***************************************************************************************
函数名称:mic_reset(void)
函数作用:计量复位（存储校验值载入寄存器里）
变量描述:
返回值：
***************************************************************************************/
int mic_reset(void)
{
    int ret = -1;
    WDT_Restart();
    uart1_init(); //串口复位

    delay_ms(500);
    WDT_Restart();
    bl0930_reset();//计量复位

    delay_ms(500);
    WDT_Restart();

    if(mic_write(BL_OTP_WRPROT, 0x42) < 0) goto err;    //允许通讯
    if(mic_write(BL_OTP_SUMERR, 0x0c) < 0) goto err;    //允许通讯
    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;    //允许写操作
    if(mic_write(BL_OT_FUNX, 0x14) < 0) goto err;

    if(meter_read_cal())
    {

        if(mic_cal.cfdiv==0)
        {
            if(mic_write_Cmp(BL_CFDIV, 0X04) < 0) goto err; //CF倍频默认
        }
        else
        {
            if(mic_write_Cmp(BL_CFDIV, mic_cal.cfdiv) < 0) goto err; //CF倍频默认
        }
        if(mic_cal.igain==0)
        {
            if(mic_write_Cmp(BL_GAIN_CR, 0X02) < 0) goto err;
        }
        else
        {
            if(mic_write_Cmp(BL_GAIN_CR, mic_cal.igain) < 0) goto err; //电流增益
        }
        if(mic_write_Cmp(BL_V_CHGN, mic_cal.v_gain) < 0) goto err; //电压增益
        if(mic_write_Cmp(BL_PHCAL, mic_cal.ia_phs) < 0) goto err; //相位
        if(mic_write_Cmp(BL_WATTOS, mic_cal.watta_ofs) < 0) goto err;//偏移量

        if(mic_cal.wa_creep==0)
        {
            if(mic_write_Cmp(BL_WA_CREEP, 0x0b) < 0) goto err;
        }
        else
        {
            if(mic_write_Cmp(BL_WA_CREEP, mic_cal.wa_creep) < 0) goto err;
        }
    }

    if(mic_cal.mode==0)
    {
        if(mic_write_Cmp(BL_MODE, 0xc7) < 0) goto err;         //读后清

    }
    else
    {
        if(mic_write_Cmp(BL_MODE, mic_cal.mode) < 0) goto err;
    }
    if(mic_write_Cmp(BL_I_CHGN, 0x00) < 0) goto err;
    if(mic_write(OTP_GAIN_CR, 0x00) < 0) goto err;
    if(mic_write_Cmp(BL_CHIP_MODE,0x07) < 0) goto err;
    ret = 0;
err:
    mic_write(BL_USR_WRPROT, 0xAA);
    return ret;
}

void mic_reset1(void)
{
    long value;


    if(mic_write(BL_OTP_WRPROT, 0x42) < 0) goto err;    //允许通讯
    if(mic_write(BL_OTP_SUMERR, 0x0c) < 0) goto err;    //允许通讯
    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;    //允许写操作
    if(mic_write(BL_OT_FUNX, 0x14) < 0) goto err;

    if(mic_read(BL_OTP_WRPROT, &value) < 0) goto err; //CF倍频默认
    if(mic_read(BL_CFDIV, &value) < 0) goto err; //CF倍频默认

    if(mic_read(BL_GAIN_CR, &value) < 0) goto err;
    if(mic_read(BL_V_CHGN, &value) < 0) goto err; //电压增益
    if(mic_read(BL_PHCAL, &value) < 0) goto err; //相位
    if(mic_read(BL_WATTOS, &value) < 0) goto err;//偏移量
    if(mic_read(BL_WA_CREEP, &value) < 0) goto err;
    if(mic_read(BL_MODE, &value) < 0) goto err;         //读后清
    if(mic_read(BL_I_CHGN, &value) < 0) goto err;
    if(mic_read(BL_CHIP_MODE,&value) < 0) goto err;
err:
    mic_write(BL_USR_WRPROT, 0xAA);

}



/*******************************************************************************
Function    : mic_init
Description : 计算芯片初始化，复位并加载校表数据
Input       :
Output      :
Return      : void
History     :
*******************************************************************************/
void mic_init(void)
{
    int i;

    for(i=0; i<10; i++)
    {
        if(0 == mic_reset())
        {
            return;
        }
        delay_ms(500);
    }

    //mic.error |= MIC_ERR_RESET;
    //led_alarm_on();
}

/*******************************************************************************
Function    : mic_check
Description : 校验计量数据
Input       :
Output      :
Return      : void
History     :
*******************************************************************************/
int mic_check(void)
{
    if(Sysstate.SysStat_RnReadErr==1)
    {
        //mic.error |= MIC_ERR_READ;
        mic_reset();
        //led_alarm_on();
        return -1;
    }
    else if(Sysstate.SysStat_RnWriteErr==1)
    {
        //	    mic.error |= MIC_ERR_CHECK;
        mic_reset();
        //led_alarm_on();
        return -2;
    }
    return 0;
}


/***************************************************************************************
函数名称:mic_read_param(void)
函数作用:读取内容参数
变量描述:
返回值：
***************************************************************************************/
void mic_read_param(void)
{
    long value = 0;

    static unsigned char  step = 0;

    if(mic.calc == 0x5A3C) // 校表过程不读寄存器
    {
        return;
    }

    if(mic_check() < 0)
    {
        return;
    }

#if 0
    if(mic_read(BL_COUNTER_CF, &value) < 0)//脉冲个数
    {
        return;
    }
    mic.pulse = value;
#endif

    if(mic_read(BL_WATT, &value) < 0)//平均有功功率
    {
        return;
    }
    if((value&0x800000)!=0)
    {
        value|=0xff000000;
    }

    if(mic_cal.kwatta != 0)
    {
        mic.watta = (( signed long)value*10)/mic_cal.kwatta; //实际功率值
        if(abs(mic.watta) < 33)// 3.3W，0.3% * 220V * 5A
        {
            mic.watta = 0;
        }
    }
    else
    {
        mic.watta=( signed long)value/186;
    }

#if 0
    if(mic_read(BL_VA, &value) < 0)//视在功率
    {
        return;
    }
    if(mic_cal.kwatta != 0)
    {
        mic.vaa = (( signed long)value<<8)/mic_cal.kwatta;
        if(abs(mic.vaa) < 33)// 0.3% * 220V * 5A
        {
            mic.vaa = 0;
        }
    }
#endif

    if(0==step && mic_read(BL_V_RMS, &value) == 0)//电压有效值
    {

        if(mic_cal.kvrms != 0)
        {
             mic.vrms = value/mic_cal.kvrms;
        }
        else
        {

            mic.vrms = value/120/10;
        }
    }
    if(1==step && mic_read(BL_I_RMS, &value) == 0)//电流有效值
    {
        if(mic_cal.kiarms != 0)
        {
             mic.iarms = value*10/mic_cal.kiarms;
            // 按50mA限定显示的最小电流
             if(mic.iarms < 50) // 50mA, 1%  * 5A
            {
                   mic.iarms = 0;
            }
        }
        else
        {
            mic.iarms=value/100;
        }
    }
#if 0
    if(2==step && mic_read(BL_PF, &value) == 0)//功率因数
    {
        mic.factor = ( signed long)value*125 >> 20;//??
    }
#endif
#if 0
    if(3==step && mic_read(BL_FREQ, &value) == 0)
    {
        //87.3906*100*3579545/32/value;
        mic.freq = 977558079/value;
    }
#endif


    if(++step > 2)//有效值寄存器刷新频率2.5HZ
    {
        step = 0;
    }

#if 0
    // only for test
    mic.wattahr = 123;

    mic.vrms = 2201;
    mic.iarms = 10123;
    mic.factor = 123;
    mic.freq = 4912;

    mic.watta = 3000;
    mic.vaa = 4000;
#endif

}

int mic_cal_reset(unsigned int igain,unsigned int cfdiv, unsigned int chipmode, unsigned int mode)
{

    int ret = -1;
    WDT_Restart();
    bl0930_reset();//计量复位

    WDT_Restart();

    delay_ms(1000);

    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;   //允许校验

    if(mic_write_Cmp(BL_CFDIV, cfdiv) < 0) goto err;    //CF倍频默认
    if(mic_write_Cmp(BL_GAIN_CR, igain) < 0) goto err; //电流增益
    if(mic_write_Cmp(BL_MODE, mode) < 0) goto err;    //读后清
    if(mic_write_Cmp(BL_PHCAL, 0x00) < 0) goto err;
    if(mic_write_Cmp(BL_I_CHGN, 0x00) < 0) goto err;
    if(mic_write_Cmp(BL_V_CHGN, 0x00) < 0) goto err;
    if(mic_write_Cmp(BL_WATTOS, 0x00) < 0) goto err;
    ret=0;

err:
    mic_write(BL_USR_WRPROT, 0xAA);
    return ret;

}


int micpara_init(unsigned int igain,unsigned int cfdiv, unsigned int chipmode, unsigned int mode)
{
    int i;
    int ret = -1;

    for(i=0; i<5; i++)
    {
        if(0 == mic_cal_reset(igain,cfdiv,chipmode,mode))
        {
            ret=0;
            return  ret;
        }
        delay_ms(500);
    }
    return  ret;
}

/***************************************************************************************
函数名称:mic_cal_init(unsigned int cfdiv, unsigned int un, unsigned int ib, unsigned int im)
函数作用:初始化校表参数
变量描述:
返回值：  正确显示1000 错误显示1099
***************************************************************************************/
void mic_cal_init(unsigned int igain,unsigned int cfdiv, unsigned int chipmode, unsigned int mode)
{
    unsigned char i;
    mic.calc_result = 1000;
    // display_page(DP_CAL_BUSY);
    // for(i = 0;i< sizeof(mic_cal);i++)
    // memset(&mic_cal, 0, sizeof(mic_cal));

    mic_cal.cfdiv = cfdiv; //默认参数
    mic_cal.igain = igain;
    mic_cal.chipmode = chipmode;
    mic_cal.mode = mode;

    mic.calc = 0x5A3C; //编程校表
    if(micpara_init(igain,cfdiv,chipmode,mode)<0) goto err;

    Calstep=1;
    mic.calc_result = 1000;
    //  display_one_page(DP_CAL_END, 10);
    meter_write_cal();//存储
    return;
err:
    mic_write(BL_USR_WRPROT, 0xAA);  //禁止校验
    mic.calc_result = 1099;
    //display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_gain( signed long l0)
函数作用:增益值校验
变量描述:
返回值：  正确显示2000 错误显示0000
***************************************************************************************/
void mic_cal_gain( signed long l0)
{
//   long l1;
    signed long  l1;
    if(Calstep!=1) return;
    mic.calc_result = 2000;
    //display_page(DP_CAL_BUSY);

    WDT_Restart();
    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;
    delay_ms(1200);
    WDT_Restart();
    if(l0<0)
    {
        l1=-(l0*10000);
        l1=l1/(10000+l0);
        l1=((4096*l1)/10000+4096)&0x00000fff;

    }
    else
    {
        l1=l0*10000;
        l1=l1/(10000+l0);
        l1=(4096-((4096*l1)/10000))&0x00000fff;
    }
    mic_cal.v_gain = (unsigned int)l1;

    if(mic_write(BL_V_CHGN, l1) < 0) goto err;//计量芯片写入校表值

err:
    mic_write(BL_USR_WRPROT, 0xAA);
    Calstep=2;
    mic.calc_result = 2000;
//    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_phase( signed long l0)
函数作用:相位值校验
变量描述:
返回值：  正确显示3000 错误显示0000
***************************************************************************************/
void mic_cal_phase( signed long l0)
{
    signed long l1;
    if(Calstep!=2) return;
    mic.calc_result = 3000;
//    display_page(DP_CAL_BUSY);
    WDT_Restart();
    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;
    delay_ms(1200);
    WDT_Restart();
    if(l0<0)
    {
        l1=-(l0*100);
        l1=l1/54;
        l1=(256-(l1/10))&0x000000ff;

    }
    else
    {
        l1=l0*100;
        l1=l1/54;
        l1=(256+(l1/10))&0x000000ff;
    }
    mic_cal.ia_phs = (unsigned int)l1;
    if(mic_write(BL_PHCAL, l1&0xFF) < 0) goto err;
err:
    mic_write(BL_USR_WRPROT, 0xAA);
    Calstep=3;
    mic.calc_result = 3000;
//    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_offset( signed long l0)
函数作用:偏移量值校验
变量描述:
返回值：  正确显示4000 错误显示0000
***************************************************************************************/
void mic_cal_offset( signed long l0)
{
    signed long l1,lh;
    long value;
    if(Calstep!=3) return;
    mic.calc_result = 4000;
//    display_page(DP_CAL_BUSY);

    WDT_Restart();

    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;
    if(mic_read(BL_WATT, &value) < 0) goto err;
    delay_ms(1200);
    WDT_Restart();

    if(l0<0)
    {
        l1=-(value*l0*256);
        lh=(10000+l0)*3125;
        l1=(l1/lh)&0x000000ff;

    }
    else
    {
        l1=-(value*l0*256);
        lh=(10000+l0)*3125;
        l1=( signed long)(l1/lh)&0x000000ff;
    }
    mic_cal.watta_ofs = (unsigned int)l1;
    if(mic_write(BL_WATTOS, l1) < 0) goto err;
err:
    mic_write(BL_USR_WRPROT, 0xAA);
    Calstep=4;
    mic.calc_result = 4000;
//    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_finish(long vrms, long irms, long watt)
函数作用:校表完成
变量描述:
返回值：  正确显示5000 错误显示0000
***************************************************************************************/
void mic_cal_finish(long vrms, long irms, long watt)
{
    long value,creep;
    int i;
    if(Calstep!=4) return;
    mic.calc_result = 5000;
//    display_page(DP_CAL_BUSY);

    WDT_Restart();
    mic.vrms = 0;
    mic.iarms = 0;
    mic.watta = 0;

    for(i=0; i<10; i++)
    {
        WDT_Restart();
        delay_ms(100);
        if(mic_read(BL_V_RMS, &value) < 0) goto err;//获取电压值
        {
            mic.vrms += ( signed long)value;
        }
        if(mic_read(BL_I_RMS, &value) < 0) goto err;//获取电流值
        {
            mic.iarms += ( signed long)value;
        }
        if(mic_read(BL_WATT, &value) < 0) goto err;//获取功率值
        {
            mic.watta += ( signed long)value;
        }
    }
    WDT_Restart();

    mic.vrms /= 10;//求平均值
    mic.iarms /= 10;
    mic.watta /= 10;

    value = mic.vrms;
    mic_cal.kvrms = (unsigned int)(value/2200);//电压系数

    value = mic.iarms;
    mic_cal.kiarms = (unsigned int)(value/500);//电流系数

    value = mic.watta;
    mic_cal.kwatta = (unsigned int)(value/110);//功率系数

    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;
    creep=(mic.watta*2)/1000;
    mic_cal.wa_creep = (unsigned int)(creep*256/3125); // 0.2%
    if(mic_write(BL_WA_CREEP, mic_cal.wa_creep) < 0) goto err;	 //防潜动阈值

    if(mic_read(BL_CHKSUM, &value) < 0)
        goto err;
    mic_cal.checksum = (value & 0x00FFFFFF);
    meter_write_cal();//存储校验值

err:
    mic_write(BL_USR_WRPROT, 0xAA);
    Calstep=0;
    mic.calc = 0;
    mic.calc_result = 5000;
//    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_read_reg(unsigned char  reg, long *value)
函数作用:读取寄存器参数
变量描述: red 寄存器 value读取值
返回值：
***************************************************************************************/
int mic_read_reg(unsigned char  reg, long *value)
{
    return mic_read(reg, value);
}
/***************************************************************************************
函数名称:mic_write_reg(unsigned char  reg, long value)
函数作用:写寄存器参数
变量描述: red 寄存器 value写入值
返回值：
***************************************************************************************/
int mic_write_reg(unsigned char  reg, long value)
{
    int ret = 0;

    if(mic_write(BL_USR_WRPROT, 0x55) < 0)//允许
    {
        return -11;
    }

    ret = mic_write(reg, value);

    if(mic_write(BL_USR_WRPROT, 0x55) < 0)//禁止
    {
        return -12;
    }

    return ret;
}
void mic_cal_proc(void)
{
	//int micpara_init(unsigned int igain,unsigned int cfdiv, unsigned int chipmode, unsigned int mode)

	 mic_cal_gain(10);
	 mic_cal_phase( 10);
	 mic_cal_offset( 10);
	// mic_cal_finish(long vrms, long irms, long watt)

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
