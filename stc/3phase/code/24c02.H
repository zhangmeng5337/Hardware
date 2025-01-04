#define uchar  unsigned char
#define bchar bdata unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define xchar  xdata unsigned char
#define xint   xdata unsigned int
#define xlong xdata unsigned long

#define CKSEL (*(unsigned char volatile xdata *)0xfe00)
#define CLKDIV (*(unsigned char volatile xdata *)0xfe01)
#define HIRCCR (*(unsigned char volatile xdata *)0xfe02)
#define XOSCCR (*(unsigned char volatile xdata *)0xfe03)
#define IRC32KCR (*(unsigned char volatile xdata *)0xfe04)
#define MCLKOCR (*(unsigned char volatile xdata *)0xfe05)

uchar Disp_liangdu;// 显示亮度调节0-7个等级
uchar Do1_moshi; // DO1 输出模式 0：开关量 1：过电压 2：过流
uchar Do2_moshi; // DO2 输出模式 0：开关量 1：过电压 2：过流
uchar Do3_moshi; // DO3 输出模式 0：开关量 1：过电压 2：过流
uchar Do4_moshi; // DO4 输出模式 0：开关量 1：过电压 2：过流
uchar Do_timer_S;//输出延时 0-30S

signed int kout_shu_A1;
signed int kout_shu_A2;
signed int kout_shu_A3;
signed int kout_shu_A4; // 过流值设定

signed int kout_shu_V1;
signed int kout_shu_V2;
signed int kout_shu_V3;
signed int kout_shu_V4; // 过压值设定

bit fig_out_k1;
bit fig_out_k2;
bit fig_out_k3;
bit fig_out_k4; //输出延时标志
bit fig_tongxun = 0; //通讯点 亮显示标志

sbit scl = P1 ^ 0;
sbit sda = P1 ^ 1; //IIC

sbit dia = P2 ^ 0; //输入
sbit dib = P2 ^ 1; //输入
sbit dic = P2 ^ 2; //输入
sbit did = P2 ^ 3; //输入


sbit k1 = P2 ^ 4; //输出
sbit k2 = P2 ^ 5; //输出
sbit k3 = P2 ^ 6; //输出
sbit k4 = P2 ^ 7; //输出

sbit r485 = P3 ^ 2;
//位申明
bit ss_tt;  //闪烁标志
bit sx_bz;
bit cs_tt;  //参数1标志
bit cs_xt;  //参数2标志
bit sj_tt;
bit da_tt;
bit sj_bc;
bit xb_bz;
bit in_tt;
bit bc_tt;  //1.0L电压电流功率校正
bit xj_mz;  //0.5L满载功率校正
bit xj_bs;  //0.5L10% 功率校正
bit wd_tt;
bit yg_pa, yg_pb, yg_pc, yg_pt; //有功功率标志0+,1-
bit wg_qa, wg_qb, wg_qc, wg_qt; //无功功率标志0+,1-
bit gl_ya, gl_yb, gl_yc, gl_yt; //功率因数标志0+,1-
bchar gnfx;
sbit yg_a = gnfx ^ 0; //1
sbit yg_b = gnfx ^ 1; //2
sbit yg_c = gnfx ^ 2; //3
sbit yg_t = gnfx ^ 3; //4
sbit wg_a = gnfx ^ 4; //5
sbit wg_b = gnfx ^ 5; //6
sbit wg_c = gnfx ^ 6; //7
sbit wg_t = gnfx ^ 7; //8

uchar zijian = 5;
bit rx_tt;
bit kk_tt;  //k灯
bit mm_tt;  //M灯
bit xn_sx;  //0三相4线;1虚拟三相3线
bit fig_pt;
bchar shez;
sbit gzf_a = shez ^ 0; //0 1偶校验 00,无校验
sbit gzf_b = shez ^ 1; //1 1奇校验
sbit gzf_c = shez ^ 2; //2
sbit nb1_t = shez ^ 3; //3
sbit sx_sx = shez ^ 4; //4 0三相4线;1三相3线
sbit us_ys = shez ^ 5; //5 0 400v; 1 100v
sbit is_yw = shez ^ 6; //6 0  5a   1  1a
sbit btl_t = shez ^ 7; //7 0 9600  1 4800


uchar key_i, kk, jj;

uchar wsms, wd_cj, xian, xbjs, tcjs;

uchar csyi, cser, eep, txgn, jcqa, zjcd, pvale;
uchar deta, detb, detc, detd;
uint dybb, dlbb;            // V A变比
uchar ipdz;                 //通信地址
uchar xsfs;                 //显示方式
uchar plbc;                 //频率补偿
xint jcrc, ycrc;

uint mima, yhmm, xgma, xgmb, xgmc;
xchar xsmz, in_js;


//串口奇偶检验标志位
uchar POEFlag = 0;   //0 无  1 偶  2 奇


//系统初始化数据
uchar code sd[] =
{
    0x00, 0x01,     //220:220
    0x00, 0x01,     //5:5
    0x01,           //通信地址(1)
    0x01,           //显示方式
    0x00, 0x01,     //密码
    0x00,
    0x64,

    0x03,          //显示亮度等级 3
    0x00, 0x00, 0x00, 0x00, // k1-k4 开关量模式
    0x0A,          //输出延时1s
    0x00, 0x05, //k1  过流报警值
    0x00, 0x05, //k2  过流报警值
    0x00, 0x05, //k3  过流报警值
    0x00, 0x05, //k4  过流报警值

    0x00, 0xfA, //k1  过压报警值
    0x00, 0xfA, //k2  过压报警值
    0x00, 0xfA, //k3  过压报警值
    0x00, 0xfA, //k4  过压报警值
    0x00
};

uchar IEEE[4];
xchar  rp[8];
xint  tp[300];
xint  yb[300];
xchar  ep[33];
xchar  cp[16];

uchar code shu[] =
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F,
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,
};
//全局变量
xlong abc;
xlong shuju;
xlong ax_av = 0, bx_av = 0, cx_av = 0; //电压
xlong ax_aa = 0, bx_aa = 0, cx_aa = 0; //电流
xlong yggla, ygglb, ygglc;      //有功功率
xlong wggla, wgglb, wgglc;      //无功功率
xlong szgla, szglb, szglc;
xint glysa, glysb, glysc, glyst; //功率因数
xint plhz;                      //频率
xlong ygglt, wgglt, szglt;      //总有功功率,无功功率,视在功率


xlong ygdnb = 0;
xlong ygdna = 0;    //有功电能
xlong zxydnb = 0;
xlong zxydna = 0;   //正向有功电能
xlong fxydnb = 0;
xlong fxydna = 0;   //反向有功电能

xlong wgdna = 0;            //无功电能
//xlong zxwdna=0;           //正向无功电能
//xlong fxwdna=0;           //反向无功电能

xint lsygdn = 0;            //临时有功电能
xint lswgdn = 0;            //临时无功电能

//延时程序
void delay()
{
    uchar i = 5;
    while (i != 0)
    {
        i--;
    }
}
void delay1(uchar x)
{
    uchar y, z;
    for (y = x; y > 0; y--)
        for (z = 110; z > 0; z--);
}
//24C02起始信号
void start()
{
    sda = 1;
    delay();
    scl = 1;
    delay();
    delay();
    sda = 0;
    delay();
    delay();
    scl = 0;
    delay();
}
//24C02终止信号
void stop()
{
    scl = 0;
    delay();
    delay();
    sda = 0;
    delay();
    delay();
    scl = 1;
    delay();
    delay();
    sda = 1;
    delay();
    delay();
}
//24C02应答信号
void respons()
{
    uchar i = 255;
    sda = 1;
    delay();
    delay();
    scl = 1;
    delay();
    delay();
    while (sda)
    {
        i--;
        delay();
        if (i == 0)sda = 0;
    }
    scl = 0;
    delay();
    delay();
}
//向24C02写入一个字节
void write_byte(uchar date)
{
    uchar i, temp;
    temp = date;
    for (i = 0; i < 8; i++)
    {
        temp = temp << 1;
        scl = 0;
        delay();
        delay();
        sda = CY;
        delay();
        delay();
        scl = 1;
        delay();
    }
    scl = 0;
    delay();
    delay();
    sda = 1;
    delay();
    delay();
}
//向24C02读出一个字节
uchar read_byte()
{
    uchar i, k;
    scl = 0;
    delay();
    delay();
    sda = 1;
    delay();
    delay();
    for (i = 0; i < 8; i++)
    {
        scl = 1;
        delay();
        delay();
        k = (k << 1) | sda;
        delay();
        scl = 0;
        delay();
        delay();
    }
    return k;
}
//在24c02指定地址写入数据
void write_add(uchar add, uchar address, uchar date)
{
    start();
    write_byte(0xa0 + (add << 1));
    respons();
    write_byte(address);
    respons();
    write_byte(date);
    respons();
    stop();
}
//在24c02指定地址读出数据
uchar read_add(uchar add, uchar address)
{
    uchar date;
    start();
    write_byte(0xa0 + (add << 1));
    respons();
    write_byte(address);
    respons();
    start();
    write_byte(0xa1 + (add << 1));
    respons();
    date = read_byte();
    stop();
    return date;
}
//2字节转为INT并返回
uint tchr_int(uchar ch1, ch2)
{
    uint ch;
    ch = ch1;
    ch = (ch << 8) | ch2;
    return (ch);
}

//4字节转为long并返回
ulong tchr_long(uchar ch0, ch1, ch2, ch3)
{
    ulong ch;
    ch = ch0;
    ch = (ch << 8) | ch1;
    ch = (ch << 8) | ch2;
    ch = (ch << 8) | ch3;
    return (ch);
}
union packet
{
    float a;
    char b[4];
};
/*
float IEEE754_to_Float(char *p)
{
    char i;
    char tmp;
    union packet q;

    tmp=*p;
    *p=*(p+3);
    *(p+3)=tmp;
    tmp=*(p+1);
    *(p+1)=*(p+2);
    *(p+2)=tmp;

    for(i=0;i<4;i++)
    q.b[i]=p[i];

    return q.a;
}*/

void Float_to_IEEE754(float x)
{
    char i;
    union packet p;

    p.a = x;
    for (i = 0; i < 4; i++)
    {
        IEEE[i] = p.b[i];
    }
}
uint Crc_date(bit trb, uint das)
{
    uint i, j;
    uint Crc = 0xffff;      //定义16位的CRC
    for (i = 0; i < das; i++) //数组
    {
        if (trb)Crc ^= rp[i];
        else Crc ^= tp[i];
        for (j = 0; j < 8; j++)
        {
            if (Crc & 0x0001)Crc = (Crc >> 1) ^ 0xa001;
            else Crc >>= 1;
        }
    }
    return (Crc);
}
//jia zai dian neng
void jzdn_data(void)
{
    uchar i;
    for (i = 0; i < 9; i++)
    {
        ep[i] = read_add(1, i);
        delay1(100);
    }
    ygdna = ep[0x00];
    ygdnb = tchr_long(ep[0x01], ep[0x02], ep[0x03], ep[0x04]); //有功电能
    wgdna = tchr_long(ep[0x05], ep[0x06], ep[0x07], ep[0x08]); //无功电能
    Float_to_IEEE754(ygdnb);
    yb[258] = IEEE[0];
    yb[259] = IEEE[1];
    yb[260] = IEEE[2];
    yb[261] = IEEE[3];
    Float_to_IEEE754(ygdnb / dybb / dlbb);
    yb[274] = IEEE[0];
    yb[275] = IEEE[1];
    yb[276] = IEEE[2];
    yb[277] = IEEE[3];
    Float_to_IEEE754(wgdna / 100);
    yb[266] = IEEE[0];
    yb[267] = IEEE[1];
    yb[268] = IEEE[2];
    yb[269] = IEEE[3];
    Float_to_IEEE754(wgdna / 100 / dybb / dlbb);
    yb[282] = IEEE[0];
    yb[283] = IEEE[1];
    yb[284] = IEEE[2];
    yb[285] = IEEE[3];
}

void zzai_ybda(void)
{
    uint i;
    yb[0] = yhmm >> 8;
    yb[1] = yhmm & 0xff; //密码
    yb[2] = ipdz;
    yb[3] = xsfs;     //通信地址与显示
    yb[4] = dybb >> 8;
    yb[5] = dybb & 0xff; //电压变比
    yb[6] = dlbb >> 8;
    yb[7] = dlbb & 0xff; //电流变比
    yb[0x08] = 0x00;
    yb[0x09] = 0x00;  //保留
    yb[0x0a] = 0x00;
    yb[0x0b] = 0x00;  //保留
    yb[0x0c] = 0x00;
    yb[0x0d] = 0x00;  //保留
    yb[0x0e] = 0x00;
    yb[0x0f] = 0x00;  //开关量输出1
    yb[0x10] = 0x00;
    yb[0x11] = 0x00;  //开关量输出2
    yb[0x12] = 0x00;
    yb[0x13] = 0x00;  //开关量输出3
    yb[0x14] = 0x00;
    yb[0x15] = 0x00;  //开关量输出4
    yb[0x16] = 0x00;
    yb[0x17] = 0x00;  //模拟量输出1
    yb[0x18] = 0x00;
    yb[0x19] = 0x00;  //模拟量输出2
    yb[0x1a] = 0x00;
    yb[0x1b] = 0x00;  //模拟量输出3
    yb[0x1c] = 0x00;
    yb[0x1d] = 0x00;  //模拟量输出4
    for (i = 0x1e; i < 114; i++)        //保留
    {
        yb[i] = 0x00;
    }
    for (i = 218; i < 258; i++)         //保留
    {
        yb[i] = 0x00;
    }
    yb[262] = 0x00;
    yb[263] = 0x00; //保留
    yb[264] = 0x00;
    yb[265] = 0x00; //保留
    yb[270] = 0x00;
    yb[271] = 0x00; //保留
    yb[272] = 0x00;
    yb[273] = 0x00; //保留
    yb[278] = 0x00;
    yb[279] = 0x00; //保留
    yb[280] = 0x00;
    yb[281] = 0x00; //保留
    yb[286] = 0x00;
    yb[287] = 0x00; //保留
    yb[288] = 0x00;
    yb[289] = 0x00; //保留
}
//初始化
void init()
{
    uchar i, j;
    P2 = 0X0F;
    P1M1 = 0x00;
    P1M0 = 0x00;
    P0M0 = 0x00;
    P0M1 = 0x00;
    P2M0 = 0xF0;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;

    //添加参数加载功能



    j = read_add(0, 4);
    delay1(100);
    if (j == 0 | j == 0xff)
    {
        for (i = 0; i < 34; i++)
        {
            write_add(0, i, sd[i]);
            delay1(150);
        }
    }
    for (i = 0; i < 33; i++)
    {
        ep[i] = read_add(0, i);
        delay1(100);
    }
    dybb = tchr_int(ep[0], ep[1]);
    dlbb = tchr_int(ep[2], ep[3]);
    ipdz = ep[4];
    xsfs = ep[5];
    yhmm = tchr_int(ep[6], ep[7]);
    shez = ep[8];
    if ((shez & 0x03) == 0)
    {
        POEFlag = 0;

    }
    else    if ((shez & 0x03) == 1)
    {
        POEFlag = 2;

    }
    else   if ((shez & 0x03) == 2)
    {
        POEFlag = 1;

    }
    plbc = ep[9];
    Disp_liangdu = ep[10];
    Do1_moshi =
        ep[11]; //    DO1 输出模式 0：开关量 1：过电压 2：过流
    Do2_moshi =
        ep[12]; //    DO2 输出模式 0：开关量 1：过电压 2：过流
    Do3_moshi =
        ep[13]; //    DO3 输出模式 0：开关量 1：过电压 2：过流
    Do4_moshi =
        ep[14]; //    DO4 输出模式 0：开关量 1：过电压 2：过流
    Do_timer_S = ep[15]; //    输出延时 0-30S
    kout_shu_A1 = tchr_int(ep[16], ep[17]);
    kout_shu_A2 = tchr_int(ep[18], ep[19]);
    kout_shu_A3 = tchr_int(ep[20], ep[21]);
    kout_shu_A4 = tchr_int(ep[22], ep[23]); // 过流值设定
    kout_shu_V1 = tchr_int(ep[24], ep[25]);
    kout_shu_V2 = tchr_int(ep[26], ep[27]);
    kout_shu_V3 = tchr_int(ep[28], ep[29]);
    kout_shu_V4 = tchr_int(ep[30], ep[31]); // 过压值设定
    fig_pt = ep[32];
    if (xsfs != 0)xian = xsfs - 1;
    else
        xian = 1;

    jzdn_data();
    /*
        P_SW2=0x80;
        CKSEL=0x20; //P5^4输出5.5296MHz
        CLKDIV=0x02;
        P_SW2=0x00;     //STC8F


        P_SW2=0x80;
    //  CKSEL=0x20; //P5^4输出5.5296MHz

        CKSEL=0x00;   //内部IRC时钟
        CLKDIV=0x02;  //时钟2分频
    //  HIRCCR=0x80;
        MCLKOCR=0x02;
        P_SW2=0x00;
     //       //STC8C
    */
    P_SW2 = 0x80;
//  MCLKOCR = 0x01;                             //主时钟输出到P5.4口
    MCLKOCR = 0x02;                             //主时钟2分频输出到P5.4口
//  MCLKOCR = 0x04;                             //主时钟4分频输出到P5.4口
//  MCLKOCR = 0x84;                             //主时钟4分频输出到P1.6口
    P_SW2 = 0x00;

    TMOD = 0x00;
    if (POEFlag == 0)
    {
        SCON = 0x50;    //串口1方式1
    }
    else if (POEFlag == 1)
    {
        SCON = 0xD0;    //串口1方式3
    }
    else if (POEFlag == 2)
    {
        SCON = 0xD0;    //串口1方式3
    }
    //SCON=0x50;        //串口1方式1
    AUXR |= 0x01; //串口1选择定时器2为波特率发生器
    AUXR |= 0x04; //定时器2时钟为Fosc,即1T
    if (btl_t == 0) //9600
    {
        T2L = 0xe0;     //设定定时初值
        T2H = 0xfe;     //设定定时初值
    }
    else        //4800
    {
        TL2 = 0xc0; //设定定时初值
        TH2 = 0xfd; //设定定时初值
    }
    AUXR |= 0x10;   //启动定时器2

    ES = 1;         //串口1中断
    PS = 1;         //串口1优先

    zzai_ybda();
    TH0 = 0x4c;
    TL0 = 0x00;
    TR0 = 1;
    ET0 = 1;
    r485 = 0;
    EA = 1;
}

void time1_init(void)
{
    AUXR &= 0xBF;       //定时器时钟12T模式
    TMOD &= 0x0F;       //设置定时器模式
    TL1 = 0x4C;         //设置定时初值
    TH1 = 0x00;         //设置定时初值    50ms
    TF1 = 0;            //清除TF1标志
    TR1 = 1;            //定时器1开始计时
    ET1 = 1;
}
