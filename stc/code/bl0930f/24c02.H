#define uchar  unsigned char
#define bchar bdata unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define xchar  xdata unsigned char
#define xint   xdata unsigned int
#define xlong  unsigned long

#define CKSEL (*(unsigned char volatile xdata *)0xfe00)
#define CLKDIV (*(unsigned char volatile xdata *)0xfe01)
#define HIRCCR (*(unsigned char volatile xdata *)0xfe02)
#define XOSCCR (*(unsigned char volatile xdata *)0xfe03)
#define IRC32KCR (*(unsigned char volatile xdata *)0xfe04) 
#define MCLKOCR (*(unsigned char volatile xdata *)0xfe05)

uchar zijian=5;
uchar Disp_liangdu;// 显示亮度调节0-7个等级
uchar Do1_moshi; //	DO1 输出模式 0：开关量 1：过电压 2：无功能 
uchar Do_timer_S;//输出延时 0-30S 
uchar xzmima=0;	//校正密码
signed int kout_shu_V1;	 //过压值 
bit fig_out_k1=0;		   //延时标志
uchar ct_shuliang;  //电压互感器数量 0： 2个 1： 3个

sbit scl=P1^4;
sbit sda=P1^5;	 //IIC

sbit dia=P1^1;//输入
sbit dib=P2^0;//输入 
sbit k1=P1^0;//输出
	 
sbit r485=P4^2;
//位申明
bit ss_tt;	//闪烁标志
bit sx_bz;
bit cs_tt;	//参数1标志
bit cs_xt;	//参数2标志
bit sj_tt;
bit da_tt;
bit sj_bc;
bit xb_bz;
bit in_tt;
bit bc_tt;	//1.0L电压电流功率校正
bit wd_tt;


bit rx_tt;
bit kk_tt;	//k灯
//bit mm_tt;	//M灯
//bit xn_sx;	//0三相4线;1虚拟三相3线

bchar shez;
sbit gzf_a=shez^0;	//0	1偶校验 00,无校验
sbit gzf_b=shez^1;	//1 1奇校验 
sbit gzf_c=shez^2;	//2 
sbit nb1_t=shez^3;	//3 
sbit sx_sx=shez^4;	//4 0三相4线;1三相3线
sbit us_ys=shez^5;	//5 0 400v; 1 100v
sbit is_yw=shez^6;	//6 0  5a   1  1a
sbit btl_t=shez^7;	//7 0 9600  1 4800


uchar kk;
uint jj;

uchar wsms,wd_cj,xian,xbjs,tcjs;

uchar csyi,cser,eep,txgn,jcqa,zjcd;
//uchar deta,detb,detc,detd;
uint dybb,dlbb;				// V A变比
uchar ipdz;					//通信地址
uchar xsfs;					//显示方式
//uchar plbc;					//频率补偿
xint jcrc,ycrc;
xint dljza,dljzb;
xint dljzc;

uint mima,yhmm,xgma,xgmb,xgmc;
xchar in_js; 

//系统初始化数据
uchar code sd[]=
{
	0x00,0x01,		//220:220
	0x00,0x01,		//5:5
	0x01,			//通信地址(1)
	0x00,			//显示方式
	0x00,0x01,		//密码
	0x00,

	0x09,0x00,		//电流系数
	0x09,0x00,		//电压系数
	0x09,0x00,		//功率系数
	0x02,			//亮度
	0x02,			//DO输出模式 
	0x00,0x05,		//过压值
	0x0A,			//延时
	0x01			//电压互感器数量 0:3个 1：2个
};


uchar IEEE[4];
xchar  rp[8];
xint  tp[300];
xint  yb[300];
xchar  ep[22];
xchar  cp[16];

uchar code shu[]=
{
	0x3F,0x06,0x5B,0x4F,0x66,
	0x6D,0x7D,0x07,0x7F,0x6F,
	0x77,0x7c,0x39,0x5e,0x79,0x71,
};
//全局变量
xlong abc;
//xlong shuju;
//xlong ax_av=0,bx_av=0,cx_av=0;	//电压
xlong ax_aa=0,bx_aa=0,cx_aa=0;	//电流
//xint plhz;						//频率


//延时程序
void delay(){;}
void delay1(uchar x)
{
	uchar y,z;
	for(y=x;y>0;y--)
	 for(z=150;z>0;z--);
}

#define WT_30M          0x80
#define WT_24M          0x81
#define WT_20M          0x82
#define WT_12M          0x83
#define WT_6M           0x84
#define WT_3M           0x85
#define WT_2M           0x86
#define WT_1M           0x87


//选择页擦除模式
void chac_add(void)
{
	IAP_CONTR=WT_30M; 	//打开IAP功能与操作时间
	IAP_TPS=12;
	IAP_CMD=0x03;   	//选择页擦除模式
	IAP_ADDRH=0x02;		//填页地址
	IAP_ADDRL=0x00;		//填页地址
	IAP_TRIG=0x5a;		//出发ISP处理器
	IAP_TRIG=0xa5;
	delay();
	IAP_CONTR=0;
	IAP_CMD=0;
	IAP_TRIG=0;
}
//在指定地址(addh=0x00~0xff)写入一个数据
void write_add(uchar add,uchar date)
{
	IAP_CONTR=WT_30M;		//打开IAP功能与操作时间
	IAP_TPS=12;
	IAP_CMD=0x02;  			//选择字节编程模式
	IAP_ADDRH=0x02;			//填页地址
	IAP_ADDRL=add;			//填页地址
	IAP_DATA=date;			//
	IAP_TRIG=0x5a;			//出发ISP处理器
	IAP_TRIG=0xa5;
	delay();
	IAP_CONTR=0;
	IAP_CMD=0;
	IAP_TRIG=0; 
}
//在指定地址(addh=0x00~0xff)读出一个数据
uchar read_add(uchar add)
{
	uchar date;
	IAP_CONTR=WT_30M; 	//打开IAP功能与操作时间
	IAP_TPS=12;
	IAP_CMD=0x01; 		//选择读IAP模式
	IAP_ADDRH=0x02;		//填页地址
	IAP_ADDRL=add;		//填页地址
	IAP_TRIG=0x5a;		//出发ISP处理器
	IAP_TRIG=0xa5;
	delay();
	date=IAP_DATA;		//读出数据
	IAP_CONTR=0;
	IAP_CMD=0;
	IAP_TRIG=0;
	return date;
}
//2字节转为INT并返回
uint tchr_int(uchar ch1,ch2)
{
	uint ch;
	ch=ch1;ch=(ch<<8)|ch2;
	return(ch);
}

union packet
{
	float a;
	char b[4];
};

void Float_to_IEEE754(float x)
{
	char i;
	union packet p;
	
	p.a=x;
	for(i=0;i<4;i++)
	{
		IEEE[i]=p.b[i];
	}
}
uint Crc_date(bit trb,uint das)   
{
	uint i,j;
	uint Crc=0xffff;		//定义16位的CRC
	for(i=0;i<das;i++)		//数组
	{
		if(trb)Crc^=rp[i];
		else Crc^=tp[i];
		for(j=0;j<8;j++)
		{
			if(Crc&0x0001)Crc=(Crc>>1)^0xa001;
			else Crc>>=1;
        }
    }
	return (Crc);
}
//jia zai dian neng
void jzdn_data(void)
{
	Float_to_IEEE754(0x00);
	yb[258]=IEEE[0];
	yb[259]=IEEE[1];
	yb[260]=IEEE[2];
	yb[261]=IEEE[3];
	Float_to_IEEE754(0x00);
	yb[274]=IEEE[0];
	yb[275]=IEEE[1];
	yb[276]=IEEE[2];
	yb[277]=IEEE[3];
	Float_to_IEEE754(0x00);
	yb[266]=IEEE[0];
	yb[267]=IEEE[1];
	yb[268]=IEEE[2];
	yb[269]=IEEE[3];
	Float_to_IEEE754(0x00);
	yb[282]=IEEE[0];
	yb[283]=IEEE[1];
	yb[284]=IEEE[2];
	yb[285]=IEEE[3];
}

void zzai_ybda(void)
{
	uint i;
	yb[0]=yhmm>>8;yb[1]=yhmm&0xff;	//密码
	yb[2]=ipdz;   yb[3]=xsfs;		//通信地址与显示
	yb[4]=dybb>>8;yb[5]=dybb&0xff;	//电压变比
	yb[6]=dlbb>>8;yb[7]=dlbb&0xff;	//电流变比
	yb[0x08]=0x00;yb[0x09]=0x00;	//保留
	yb[0x0a]=0x00;yb[0x0b]=0x00;	//保留
	yb[0x0c]=0x00;yb[0x0d]=0x00;	//保留
	yb[0x0e]=0x00;yb[0x0f]=0x00;	//开关量输出1
	yb[0x10]=0x00;yb[0x11]=0x00;	//开关量输出2
	yb[0x12]=0x00;yb[0x13]=0x00;	//开关量输出3
	yb[0x14]=0x00;yb[0x15]=0x00;	//开关量输出4
	yb[0x16]=0x00;yb[0x17]=0x00;	//模拟量输出1
	yb[0x18]=0x00;yb[0x19]=0x00;	//模拟量输出2
	yb[0x1a]=0x00;yb[0x1b]=0x00;	//模拟量输出3
	yb[0x1c]=0x00;yb[0x1d]=0x00;	//模拟量输出4
	for(i=0x1e;i<114;i++)				//保留
	{
		yb[i]=0x00;
	}
	for(i=218;i<258;i++)				//保留
	{
		yb[i]=0x00;
	}
	yb[262]=0x00;yb[263]=0x00;	//保留
	yb[264]=0x00;yb[265]=0x00;	//保留
	yb[270]=0x00;yb[271]=0x00;	//保留
	yb[272]=0x00;yb[273]=0x00;	//保留
	yb[278]=0x00;yb[279]=0x00;	//保留
	yb[280]=0x00;yb[281]=0x00;	//保留
	yb[286]=0x00;yb[287]=0x00;	//保留
	yb[288]=0x00;yb[289]=0x00;	//保留
}
//初始化
void init()
{
	uchar i,j;
	P1M1=0x00;
	P1M0=0x00;
	P0M0=0x00;
	P0M1=0x00;
	P2M0=0x00;
	P2M1=0x00;
	P3M0=0x00;
	P3M1=0x00;
	P4M0=0x00;
	P4M1=0x00;
	P5M0=0x20;	//P5.5
	P5M1=0x00;
	dia=0;dib=0;k1=0;
	//添加参数加载功能
	j=read_add(0,4);
	delay1(100);
	if(j==0&j==0xff)
	{
		for(i=0;i<21;i++)
		{
			write_add(0,i,sd[i]);
			delay1(150);
		}
	}
	for(i=0;i<21;i++){ep[i]=read_add(0,i);delay1(100);}
	dybb=tchr_int(ep[0],ep[1]);
	dlbb=tchr_int(ep[2],ep[3]);
	ipdz=ep[4];
	xsfs=ep[5];
	yhmm=tchr_int(ep[6],ep[7]);
	shez=ep[8];
//	plbc=ep[9];
	dljza=tchr_int(ep[9],ep[10]);
	dljzb=tchr_int(ep[11],ep[12]);
	dljzc=tchr_int(ep[13],ep[14]);
	Disp_liangdu=ep[15];		   //亮度
	Do1_moshi=ep[16];
	kout_shu_V1=tchr_int(ep[17],ep[18]);
	Do_timer_S=ep[19];
	ct_shuliang=ep[20];
	xian=0;
	jzdn_data();

  /*
	P_SW2=0x80;
	CKSEL=0x20;	//P5^4输出5.5296MHz
	CLKDIV=0x02;
	P_SW2=0x00;	//	*/		//STC8F
	

	P_SW2=0x80;
//	CKSEL=0x20;	//P5^4输出5.5296MHz
	
	CKSEL=0x00;	  //内部IRC时钟
	CLKDIV=0x02;  //时钟2分频
//	HIRCCR=0x80; 
	MCLKOCR=0x02;
	P_SW2=0x00;
 // */	  //STC8C

	TMOD=0x00;
	SCON=0x50;		//串口1方式1
	AUXR |=0x01;	//串口1选择定时器2为波特率发生器
	AUXR |=0x04;	//定时器2时钟为Fosc,即1T
	if(btl_t==0)//9600
	{
		T2L = 0xc7;		//设定定时初值
		T2H = 0xfe;		//设定定时初值
	}
	else		//4800
	{
		TL2 =0x8f;	//设定定时初值
		TH2 =0xfd;	//设定定时初值		
	}
	AUXR |=0x10;	//启动定时器2

	ES=1;			//串口1中断
	PS=1;			//串口1优先

	zzai_ybda();
	TH0=0x3c;TL0=0xb0;
	TR0=1;ET0=1;r485=0;
	EA=1;
}
void time1_init(void)
{  
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x4C;			//设置定时初值
	TH1 = 0x00;			//设置定时初值	50ms
	TF1 = 0;			//清除TF1标志
	TR1 = 1;			//定时器1开始计时
	ET1	=1;
}
