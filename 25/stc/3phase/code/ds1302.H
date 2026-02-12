//DS1302接口
sbit DS13CS =P1^1;
sbit DS13IO =P1^2;
sbit DS13CLK=P1^3;

//功能说明：向DS1302写一个字节
void wds13byte(uchar date)
{
	uchar i;
	DS13CLK=0;
	for(i=0;i<8;i++)
	{
		if(date&0x01)DS13IO=1;
		else DS13IO=0;
		DS13CLK=1;
		DS13CLK=0;
		date=date>>1;
	}
}
//功能说明:向DS1302读一个字节
uchar rds13byte(void)
{
	uchar i,date,ii;
	DS13CLK=0;
	for(i=0;i<8;i++)
	{
		if(DS13IO)ii=0x80;
		else ii=0x00;
		date=(date>>1)|ii;
		DS13CLK=1;
		DS13CLK=0;
	}
	return date;
}
//功能说明：读功能
uchar readds1302(uchar add)
{
	uchar i;
	DS13CS=0;			//关闭DS1302
	DS13CLK=0;
	DS13CS=1;			//使能DS1302
	wds13byte(add);		//读代码
	i=rds13byte();		//返回读取数字
	DS13CS=0;			//关闭DS1302
	return i;
}
//功能说明:在指定的地址，写的内容
void writeds1302(uchar fp,uchar dcode)
{
	DS13CS=0;        //关闭DS1302
	DS13CLK=0;
	DS13CS=1;        //使能DS1302
	wds13byte(fp);     //写控制命令
	wds13byte(dcode);  //写入数据
	DS13CS=0;         //关闭DS1302
}
//功能说明:
void ds13_charg(uchar z1,uchar z2,uchar z3,uchar z4,uchar z5,uchar z6)
{
	writeds1302(0x8e,0x00);		//解除写保护
	writeds1302(0x80,0x00);		//
	writeds1302(0x8c,z1);		//年
	writeds1302(0x88,z2);		//月
	writeds1302(0x86,z3);		//日
	writeds1302(0x84,z4);		//时
	writeds1302(0x82,z5);		//分
	writeds1302(0x80,z6);		//秒
	writeds1302(0x90,0xa5);		//单二极管2K电阻充电
	writeds1302(0x8e,0x80);		//置位写保护
}
//读时间
void shijian()
{
	miao=readds1302(0x81);
	if(miao>0x59)miao=0x00;
	fen=readds1302(0x83);
	if(fen>0x59)fen=0x00;	
	shi=readds1302(0x85);
	if(shi>0x59)shi=0x00;
	ri=readds1302(0x87);
	if(ri>0x31)ri=0x01;
	yue=readds1302(0x89);
	if(yue>0x12)yue=0x01;
	nian=readds1302(0x8d);
	if(nian>0x99)nian=0x14;
}
//读时间
void shijian_c()
{
	tp[0]=err;
	tp[1]=nian;
	tp[2]=yue;
	tp[3]=ri;
	tp[4]=shi;
	tp[5]=fen;
	tp[6]=miao;
}
