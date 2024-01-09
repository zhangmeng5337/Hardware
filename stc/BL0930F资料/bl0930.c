#include "define_all.h"

METER_IC meter_ic;

U8 Calstep;
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
typedef __packed struct
{
    U16 igain;     
    U16 chipmode;     
    U16 mode;     

    U16 cfdiv;
    U16 kvrms;
    U16 kiarms;
    U16 kwatta;
    
    U16 ia_gain;
    U16 v_gain;
    U16 ia_phs;
    U16 watta_ofs;
    U16 wa_creep;
    
    U32 checksum;

#if 0
    U16 v_ofs;
    U16 ia_ofs;

    U16 ib_gain;
    U16 ib_phs;
    U16 ib_ofs;

    U16 watta_gain;
    U16 wattb_gain;
    U16 wattb_ofs;
    U16 va_gain;
    U16 va_offs;

    U16 vrms_gain;
    U16 iarms_gain;
    U16 ibrms_gain;
    
    U16 vrms_ofs;
    U16 iarms_ofs;
    U16 ibrms_ofs;
#endif    

    U16 check;

}MIC_CAL;

MIC_CAL mic_cal;
MIC mic;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////////只读区域/////////////////////////////////////////////
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


#define BL_PF           0x08//功率因子
#define BL_VA           0x0B//平均视在功率
#define BL_COUNTER_CF   0x0C//CF脉冲个数





#define BL_WRITECMD    0XA8
#define BL_READCMD     0X58
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
U8 bl_buf[8];

/***************************************************************************************
函数名称:bl0930_reset(void)
函数作用:复位计量芯片
变量描述: 
返回值：  
***************************************************************************************/
void bl0930_reset(void)
{   
	U8 buf[35];
	U8 i;
	for(i=0;i<32;i++)
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
函数名称:bl0930_read(U8 reg, U32 *data)
函数作用:读取寄存器的值（通过uart）
变量描述: reg 寄存器 *data读取的32值
返回值：  
***************************************************************************************/
int bl0930_read(U8 reg, U32 *data)
{
    U8 check = 0;
	  U8 i;
	  for(i=0;i<8;i++)
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

    *data = (U32)bl_buf[2];
    *data |= (U32)bl_buf[3]<<8;
    *data |= (U32)bl_buf[4]<<16;

    if(bl_buf[4] & 0x80)
    {
        *data |= 0xFF000000;
    }
    
    return 0;//读取成功
}
/***************************************************************************************
函数名称:bl0930_write(U8 reg, U32 data)
函数作用:写寄存器的值（通过uart）
变量描述: reg 寄存器 *data读取的32值
返回值：  
***************************************************************************************/
int bl0930_write(U8 reg, U32 data)
{
    U8 check = 0;
    
    bl_buf[0] = BL_WRITECMD;
    bl_buf[1] = reg;
    bl_buf[2] = (U8)(data & 0xFF);
    bl_buf[3] = (U8)((data>>8) & 0xFF);
    bl_buf[4] = (U8)((data>>16) & 0xFF);

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
函数名称:mic_read(U8 reg, U32 *data)
函数作用:比较读取读取的数据
变量描述: reg 寄存器 *data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_read(U8 reg, U32 *data)
{
	  U8 i;
	  for(i=0;i<5;i++)
	  {
		   if(bl0930_read(reg, data) < 0)//读取寄存器的值
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
		   	Sysstate.bit.SysStat_RnReadErr=1;
        return -1;	
		}
		else
		{
		    Sysstate.bit.SysStat_RnReadErr=0;
		    return 0;
		}
/*
    if(bl0930_read(reg, data) < 0)//读取寄存器的值
    {
			  Sysstate.bit.SysStat_RnReadErr=1;
        return -1;
    }
		else
		{
		   Sysstate.bit.SysStat_RnReadErr=0;	
		}
    return 0;
	*/
}
/***************************************************************************************
函数名称:mic_write(U8 reg, U32 data)
函数作用:比较写寄存器的值
变量描述: reg 寄存器 data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_write(U8 reg, U32 data)
{
    if(bl0930_write(reg, data) < 0)
    {
			  Sysstate.bit.SysStat_RnWriteErr=1;
        return -1;
    }	
		else
		{
		    Sysstate.bit.SysStat_RnWriteErr=0;		
		}

    return 0;
}
/***************************************************************************************
函数名称:mic_write_Cmp(U8 reg, U32 data)
函数作用:比较写寄存器的值
变量描述: reg 寄存器 data读取的32值
返回值：  0正确
***************************************************************************************/
int mic_write_Cmp(U8 reg, U32 data)
{
	  U32 value=0;
	  if(mic_write(reg, data)<0)
		{
		   return -1;	
		}
		delay_ms(5);
	 	if(mic_read(reg,&value)<0)
		{
//		   	Sysstate.bit.SysStat_RnReadErr=1;
        return -2;	
		}
		if(value!=data)
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
//		if(mic_write(OTP_GAIN_CR, 0x00) < 0) goto err; 	
		if(mic_write_Cmp(BL_CHIP_MODE,0x07) < 0) goto err;
    ret = 0;    
err:
    mic_write(BL_USR_WRPROT, 0xAA);
    return ret;
}

void mic_reset1(void)
{
    U32 value;

	
//	  if(mic_write(BL_OTP_WRPROT, 0x42) < 0) goto err;    //允许通讯
//	  if(mic_write(BL_OTP_SUMERR, 0x0c) < 0) goto err;    //允许通讯
//    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;    //允许写操作 
//	  if(mic_write(BL_OT_FUNX, 0x14) < 0) goto err;    

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
    
    mic.error |= MIC_ERR_RESET;
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
	  if(Sysstate.bit.SysStat_RnReadErr==1)
		{
		    mic.error |= MIC_ERR_READ;
        mic_reset();
        //led_alarm_on();
        return -1; 	
		}
		else if(Sysstate.bit.SysStat_RnWriteErr==1)
		{
	//	    mic.error |= MIC_ERR_CHECK;
        mic_reset();
        //led_alarm_on();
        return -2; 		
		}
    return 0;
}

// read data per second
/***************************************************************************************
函数名称:mic_read_param(void)
函数作用:读取内容参数
变量描述:
返回值：  
***************************************************************************************/
void mic_read_param(void)
{
    U32 value = 0;
    
    static U8 step = 0;

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
        mic.watta = ((S32)value*10)/mic_cal.kwatta; //实际功率值
        //if(abs(mic.watta) < 33)// 3.3W，0.3% * 220V * 5A
        //{
        //    mic.watta = 0;
        //}
    }
		else
		{
		   mic.watta=(S32)value/186;
		}
    
#if 0  
    if(mic_read(BL_VA, &value) < 0)//视在功率
    {
        return;
    }
    if(mic_cal.kwatta != 0)
    {
        mic.vaa = ((S32)value<<8)/mic_cal.kwatta;
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
        mic.factor = (S32)value*125 >> 20;//??
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

int mic_cal_reset(U16 igain,U16 cfdiv, U16 chipmode, U16 mode)
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


int micpara_init(U16 igain,U16 cfdiv, U16 chipmode, U16 mode)
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
函数名称:mic_cal_init(U16 cfdiv, U16 un, U16 ib, U16 im)
函数作用:初始化校表参数
变量描述:
返回值：  正确显示1000 错误显示1099
***************************************************************************************/
void mic_cal_init(U16 igain,U16 cfdiv, U16 chipmode, U16 mode)
{
    mic.calc_result = 1000;
    display_page(DP_CAL_BUSY);
    
    memset(&mic_cal, 0, sizeof(mic_cal));
    
    mic_cal.cfdiv = cfdiv; //默认参数
    mic_cal.igain = igain;
    mic_cal.chipmode = chipmode;
    mic_cal.mode = mode;

    mic.calc = 0x5A3C; //编程校表
	  if(micpara_init(igain,cfdiv,chipmode,mode)<0) goto err;

    Calstep=1;		
    mic.calc_result = 1000;
    display_one_page(DP_CAL_END, 10);
    meter_write_cal();//存储
    return;    
err:
    mic_write(BL_USR_WRPROT, 0xAA);  //禁止校验
    mic.calc_result = 1099;
    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_gain(S32 l0)
函数作用:增益值校验
变量描述:
返回值：  正确显示2000 错误显示0000
***************************************************************************************/
void mic_cal_gain(S32 l0)
{
 //   U32 l1; 
    S32  l1;  
	  if(Calstep!=1) return;
    mic.calc_result = 2000;
    display_page(DP_CAL_BUSY);
    
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
    mic_cal.v_gain = (U16)l1;
   
    if(mic_write(BL_V_CHGN, l1) < 0) goto err;//计量芯片写入校表值

err:
    mic_write(BL_USR_WRPROT, 0xAA);    
    Calstep=2;
    mic.calc_result = 2000;
    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_phase(S32 l0)
函数作用:相位值校验
变量描述:
返回值：  正确显示3000 错误显示0000
***************************************************************************************/
void mic_cal_phase(S32 l0)
{
    S32 l1;
    if(Calstep!=2) return;
    mic.calc_result = 3000;
    display_page(DP_CAL_BUSY);
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
	  mic_cal.ia_phs = (U16)l1;
	  if(mic_write(BL_PHCAL, l1&0xFF) < 0) goto err;   
err:
    mic_write(BL_USR_WRPROT, 0xAA);    
    Calstep=3;
    mic.calc_result = 3000;
    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_offset(S32 l0)
函数作用:偏移量值校验
变量描述:
返回值：  正确显示4000 错误显示0000
***************************************************************************************/
void mic_cal_offset(S32 l0)
{
    S32 l1,lh;
	  U32 value;
	  if(Calstep!=3) return;
    mic.calc_result = 4000;
    display_page(DP_CAL_BUSY);
    
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
			l1=(S32)(l1/lh)&0x000000ff;
    }
    mic_cal.watta_ofs = (U16)l1;
    if(mic_write(BL_WATTOS, l1) < 0) goto err;
err:
    mic_write(BL_USR_WRPROT, 0xAA);    
    Calstep=4;
    mic.calc_result = 4000;
    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_cal_finish(U32 vrms, U32 irms, U32 watt)
函数作用:校表完成
变量描述:
返回值：  正确显示5000 错误显示0000
***************************************************************************************/
void mic_cal_finish(U32 vrms, U32 irms, U32 watt)
{
    U32 value,creep;
    int i;
    if(Calstep!=4) return;
    mic.calc_result = 5000;
    display_page(DP_CAL_BUSY);
    
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
            mic.vrms += (S32)value;
        }
        if(mic_read(BL_I_RMS, &value) < 0) goto err;//获取电流值
        {
            mic.iarms += (S32)value;
        }
        if(mic_read(BL_WATT, &value) < 0) goto err;//获取功率值
        {
            mic.watta += (S32)value;
        }
    }
		WDT_Restart();
    
    mic.vrms /= 10;//求平均值
    mic.iarms /= 10;
    mic.watta /= 10;

    value = mic.vrms;
    mic_cal.kvrms = (U16)(value/2200);//电压系数

    value = mic.iarms;
    mic_cal.kiarms = (U16)(value/500);//电流系数
    
    value = mic.watta;
    mic_cal.kwatta = (U16)(value/110);//功率系数
    
    if(mic_write(BL_USR_WRPROT, 0x55) < 0) goto err;
		creep=(mic.watta*2)/1000;
    mic_cal.wa_creep = (U16)(creep*256/3125); // 0.2%
    if(mic_write(BL_WA_CREEP, mic_cal.wa_creep) < 0) goto err;	 //防潜动阈值
    
//    if(mic_read(BL_CHKSUM, &value) < 0) goto err;
//    mic_cal.checksum = (value & 0x00FFFFFF); 
    meter_write_cal();//存储校验值

err:
    mic_write(BL_USR_WRPROT, 0xAA);  
    Calstep=0;
    mic.calc = 0;
    mic.calc_result = 5000;
    display_one_page(DP_CAL_END, 10);
}
/***************************************************************************************
函数名称:mic_read_reg(U8 reg, U32 *value)
函数作用:读取寄存器参数
变量描述: red 寄存器 value读取值
返回值：  
***************************************************************************************/
int mic_read_reg(U8 reg, U32 *value)
{
    return mic_read(reg, value);
}
/***************************************************************************************
函数名称:mic_write_reg(U8 reg, U32 value)
函数作用:写寄存器参数
变量描述: red 寄存器 value写入值
返回值：  
***************************************************************************************/
int mic_write_reg(U8 reg, U32 value)
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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
