//SPI
sbit CS   =P3^3;		//选通
sbit SCLK =P1^3;		//时钟
sbit DIN  =P1^7;		//输入
sbit DOUT =P1^6;		//输出
sbit RESET=P1^5;		//复位

//写入数据到HT7038的指定地址
void write_ht7038(uchar add,ulong dat)
{
	uchar i;
	SCLK=0;
	if(add<0x7f)add=add+0x80;
	CS=0;
	for(i=0;i<8;i++)
	{
		add=add<<1;
		SCLK=1;delay();
		if(CY==1)DIN=1;
		else DIN=0;
		SCLK=0;delay();
	}
	delay();
	dat=dat<<8;
	for(i=0;i<24;i++)
	{
		dat=dat<<1;
		SCLK=1;delay();
		if(CY==1)DIN=1;
		else DIN=0;
		SCLK=0;delay();
	}
	CS=1;
}
//从HT7038的指定地址读出数据
ulong read_ht7038(uchar add)
{
	uchar i;
	ulong dat=0;
	SCLK=0;
	CS=0;
	for(i=0;i<8;i++)
	{
		add=add<<1;
		SCLK=1;delay();
		if(CY==1)DIN=1;
		else DIN=0;
		SCLK=0;delay();
	}
	delay1(1);
	for(i=0;i<24;i++)
	{
		SCLK=0;delay();
		dat=dat<<1;
		SCLK=1;delay();
		if(DOUT==1)dat=dat+1;
	}
	CS=1;
	return(dat);
}

//初始化HT7038
void in_ht7038(void)
{
	uchar i;
	uint adz;
	RESET=0;
	delay1(200);
	delay1(200);
	for(i=0;i<30;i++){ep[i]=read_add(7,i);delay1(100);}
	adz=tchr_int(ep[0],ep[1]);
	if(adz==0xffff)
	{
		for(i=0;i<30;i++)
		{
			write_add(7,i,0x00);
			delay1(100);
			ep[i]=0x00;
		}
		adz=0;
	}
	RESET=1;
	write_ht7038(0xc6,0x5a);			//校表数据读出
	abc=read_ht7038(0x00);
	while(abc!=0xaaaa)
	{
		write_ht7038(0xc6,0x5a);			//校表数据读出
		delay1(200);
		abc=read_ht7038(0x00);
	}
	write_ht7038(0xc9,0x5a);			//校表数据写使
	write_ht7038(0x01,0xb97e);			//开启6通道
	write_ht7038(0x02,0x00);			//电压电流增益1
	write_ht7038(0x03,0xfc84);			//开启CF1,CF2 读后清0
	write_ht7038(0x1e,0x7b);			//高频脉冲3200
	write_ht7038(0x31,0x3427);			//高通滤波
	if(sx_sx)write_ht7038(0x70,0x01);	//代数相加
	else write_ht7038(0x70,0x00);		//代数相加


	write_ht7038(0x17,adz);				// A相电压(0x17)
	adz=tchr_int(ep[2],ep[3]);
	write_ht7038(0x18,adz);				// B相电压(0x18)
	adz=tchr_int(ep[4],ep[5]);
	write_ht7038(0x19,adz);				// C相电压(0x19)

	adz=tchr_int(ep[6],ep[7]);
	write_ht7038(0x1a,adz);				// A相电流(0x1a)
	adz=tchr_int(ep[8],ep[9]);
	write_ht7038(0x1b,adz);				// B相电流(0x1b)
	adz=tchr_int(ep[10],ep[11]);
	write_ht7038(0x1c,adz);				// C相电流(0x1c)

//	write_ht7038(0x02,0x000c);
	
	
	adz=tchr_int(ep[12],ep[13]);
	write_ht7038(0x04,adz);			// A相功率(0x04)
	write_ht7038(0x07,adz);			// A相功率(0x05)
	write_ht7038(0x0a,adz);			// A相功率(0x0a)
	adz=tchr_int(ep[14],ep[15]);
	write_ht7038(0x05,adz);			// B相功率(0x05)
	write_ht7038(0x08,adz);			// A相功率(0x08)
	write_ht7038(0x0b,adz);			// B相功率(0x0b)
	adz=tchr_int(ep[16],ep[17]);
	write_ht7038(0x06,adz);			// C相功率(0x06)
	write_ht7038(0x09,adz);			// A相功率(0x09)
	write_ht7038(0x0c,adz);			// C相功率(0x0c)

	adz=tchr_int(ep[18],ep[19]);
	write_ht7038(0x0d,adz);			//100%负载(0x0d)
	write_ht7038(0x10,adz);			// A相相角(0x10)
	adz=tchr_int(ep[20],ep[21]);
	write_ht7038(0x0e,adz);			//100%负载(0x0e)
	write_ht7038(0x11,adz);			// B相相角(0x11)
	adz=tchr_int(ep[22],ep[23]);
	write_ht7038(0x0f,adz);			//100%负载(0x0f)
	write_ht7038(0x12,adz);			// C相相角(0x12)

	adz=tchr_int(ep[24],ep[25]);
	write_ht7038(0x61,adz);			//A10%负载(0x61)
	adz=tchr_int(ep[26],ep[27]);
	write_ht7038(0x62,adz);			//B10%负载(0x62)
	adz=tchr_int(ep[28],ep[29]);
	write_ht7038(0x63,adz);			//C10%负载(0x63)

	write_ht7038(0xc9,0xa5);	//校表数据写使
	write_ht7038(0xc6,0xaa);	//校表数据关闭
	delay1(250);
}

//保存校正数据
void bcxz_data(uchar addr,uint dat)
{
	write_ht7038(0xc6,0x5a);	//校表数据读出
	write_ht7038(0xc9,0x5a);	//校表数据写使
	write_ht7038(addr,dat);
	write_ht7038(0xc9,0xa5);	//校表数据关闭
	write_ht7038(0xc6,0xa5);
}
