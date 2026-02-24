/*定义控制端口*/
sbit Din=P3^6;	//定义DIN数据端口
sbit CLK=P3^7;	//定义CLK数据端口
	
/*Start函数*/
void I2CStart()
{
    Din=1;
    CLK=1;
	delay();delay();
	delay();delay();
	Din=1; 
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
	delay();delay();
	delay();delay();
	CLK=0;
	delay();delay();
	delay();delay();
}
/*Stop函数*/
void I2CStop()
{
	CLK=1;
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
	Din=1;
	delay();delay();
	delay();delay();
	CLK=0;
	Din=0;

}
/*发送8bit数据,从低位开始*/
void I2CWritebyte(uchar oneByte)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(oneByte&0x01)Din=1;
		else Din=0;
		delay();delay();
		delay();delay();
		delay();delay();
		CLK=1;
		oneByte=oneByte>>1;
	}
	CLK=0;
	delay();delay();
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
}
//送出8位串行数据
void out_disp(void)
{
	uchar i;
	I2CStart();
	I2CWritebyte(0x40);	//数据命令设置:普通模式,地址自动加一
	I2CStop();
	
	I2CStart();
	I2CWritebyte(0xc0);	//地址命令设置:初始地址00H
	for(i=0;i<16;i++)	//发送16位显示数据
	{
		I2CWritebyte(cp[i]);
	}
	I2CStop();
	I2CStart();	
	switch(Disp_liangdu)
	{
		case 0:	I2CWritebyte(0x88);break;
		case 1:	I2CWritebyte(0x89);break;
		case 2:	I2CWritebyte(0x8A);break;
		case 3:	I2CWritebyte(0x8b);break;  //显示控制:显示开,脉冲宽度设为12/16
		case 4:	I2CWritebyte(0x8c);break;
		case 5:	I2CWritebyte(0x8d);break;
		case 6:	I2CWritebyte(0x8e);break;
		case 7:	I2CWritebyte(0x8f);break;
		default:I2CWritebyte(0x8b);break;
	}  
	I2CStop();
}
//送出8位串行数据
void out_disp_zijian(void)
{
	uchar i;
	I2CStart();
	I2CWritebyte(0x40);	//数据命令设置:普通模式,地址自动加一
	I2CStop();
	
	I2CStart();
	I2CWritebyte(0xc0);	//地址命令设置:初始地址00H
	for(i=0;i<16;i++)	//发送16位显示数据
	{
		I2CWritebyte(cp[i]);
	}
	I2CStop();
	I2CStart();	
	I2CWritebyte(0x88);  
	I2CStop();
}
//显示0000-9999
void swzs_disp(uchar x,uint dat)
{
	cp[x*4+0]=shu[dat/1000];
	cp[x*4+1]=shu[dat%1000/100];
	cp[x*4+2]=shu[dat%100/10];
	cp[x*4+3]=shu[dat%10];
}
//显示0.0-999.9
void ywxs_disp(uchar x,ulong d_v)
{
	uchar aa,ab;
	if(d_v>100000)
	{
		d_v=d_v/1000;
		kk_tt=1;
	}else kk_tt=0;
	if(d_v<10000)
	{
		aa=d_v/1000;
		if(aa!=0)cp[x*4]=shu[aa];
		else cp[x*4]=0x00;
		ab=d_v%1000/100;
		if(aa!=0|ab!=0)cp[x*4+1]=shu[ab];
		else cp[x*4+1]=0x00;
		cp[x*4+2]=shu[d_v%100/10]+0x80;
		cp[x*4+3]=shu[d_v%10];
	}
	else
	if(d_v<100000)
	{
		cp[x*4]=shu[d_v/10000];
		cp[x*4+1]=shu[d_v%10000/1000];
		cp[x*4+2]=shu[d_v%1000/100];
		cp[x*4+3]=shu[d_v%100/10];
	}
//	cp[x*4]=shu[(d_v>>20)&0x0f];
//	cp[x*4+1]=shu[(d_v>>16)&0x0f];
//	cp[x*4+2]=shu[(d_v>>12)&0x0f];
//	cp[x*4+3]=shu[(d_v>>8)&0x0f];
}
//显示0.00-99.99
void ewxs_disp(uchar x,ulong d_v)
{
	uchar aa;
	if(d_v<10000)
	{
		aa=d_v/1000;
		if(aa!=0)cp[x*4]=shu[aa];
		else cp[x*4]=0x00;
		cp[x*4+1]=shu[d_v%1000/100]+0x80;
		cp[x*4+2]=shu[d_v%100/10];
		cp[x*4+3]=shu[d_v%10];
	}
	else
	if(d_v<100000)
	{
		cp[x*4]=shu[d_v/10000];
		cp[x*4+1]=shu[d_v%10000/1000];
		cp[x*4+2]=shu[d_v%1000/100]+0x80;
		cp[x*4+3]=shu[d_v%100/10];
	}
	else
	if(d_v<1000000)
	{
		d_v=d_v/10;
		cp[x*4]=shu[d_v/10000];
		cp[x*4+1]=shu[d_v%10000/1000];
		cp[x*4+2]=shu[d_v%1000/100];
		cp[x*4+3]=shu[d_v%100/10];
	}
}
//0.000-9.999
void swxs_disp(bit fh,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		kk_tt=1;
	}else kk_tt=0;
	if(d_v<10000)
	{
		if(fh)
		{
			cp[x*4]=0x40;
			cp[x*4+1]=shu[d_v/1000]+0x80;
			cp[x*4+2]=shu[d_v%1000/100];
			cp[x*4+3]=shu[d_v%100/10];
		}
		else
		{
			cp[x*4]=shu[d_v/1000]+0x80;
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10];
		}
	}
	else
	if(d_v<100000)
	{
		cp[x*4]=shu[d_v/10000];
		cp[x*4+1]=shu[d_v%10000/1000]+0x80;
		cp[x*4+2]=shu[d_v%1000/100];
		cp[x*4+3]=shu[d_v%100/10];
	}
	else
	if(d_v<1000000)
	{
		cp[x*4]=shu[d_v/100000];
		cp[x*4+1]=shu[d_v%100000/10000];
		cp[x*4+2]=shu[d_v%10000/1000]+0x80;
		cp[x*4+3]=shu[d_v%1000/100];
	}
	else
	if(d_v<10000000)
	{
		cp[x*4]=shu[d_v/1000000];
		cp[x*4+1]=shu[d_v%1000000/100000];
		cp[x*4+2]=shu[d_v%100000/10000];
		cp[x*4+3]=shu[d_v%10000/1000];
	}
}
//显示功率
void pqs_disp(bit hhw,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		mm_tt=1;
	}else mm_tt=0;
	if(hhw)
	{
		cp[x*4]=0x40;
		if(d_v<10000)
		{
			d_v=d_v/10;
			cp[x*4+1]=shu[d_v/100]+0x80;
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/100;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/1000;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80; 
		}
	}
	else
	{
		if(d_v<10000)
		{
			cp[x*4+0]=shu[d_v/1000]+0x80;
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/10;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100]+0x80;
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/100;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<10000000)
		{
			d_v=d_v/1000;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
		}
	}
}

void bxs_disp(uchar x)	  //不显示
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x00;
	cp[x*4+2]=0x00;
	cp[x*4+3]=0x00;
}
//CodE
void code_disp(uchar x)
{
	cp[x*4+0]=0x39;
	cp[x*4+1]=0x5c;
	cp[x*4+2]=0x5e;
	cp[x*4+3]=0x79;
}
//DP--
void DP_disp(uchar x)
{
	cp[x*4+0]=0x5E;
	cp[x*4+1]=0x73;
	cp[x*4+2]=0x40;
	cp[x*4+3]=0x40;
}
//dido
void dido_disp(uchar x)
{
	cp[x*4+0]=0x5E;
	cp[x*4+1]=0x30;
	cp[x*4+2]=0x5E;
	cp[x*4+3]=0x5C;
}
// do1-do4
void do1_do4(uchar x ,uchar y)
{
	cp[x*4+0]=0x5E;	 //d
	cp[x*4+1]=0x5c;	 //o
	cp[x*4+2]=0x40;	 //-
	switch(y)
	{
	  case 1: cp[x*4+3]=0x06;break; //1
	  case 2: cp[x*4+3]=0x5b;break; //2
	  case 3: cp[x*4+3]=0x4f;break; //3
	  case 4: cp[x*4+3]=0x66;break; //4
	  default:break;
	} 	
}
//Oc1-oc4
void oc1_oc4(uchar x ,uchar y)
{
	cp[x*4+0]=0x5c;	 //o
	cp[x*4+1]=0x58;	 //c
	cp[x*4+2]=0x40;	 //-
	switch(y)
	{
	  case 1: cp[x*4+3]=0x06;break; //1
	  case 2: cp[x*4+3]=0x5b;break; //2
	  case 3: cp[x*4+3]=0x4f;break; //3
	  case 4: cp[x*4+3]=0x66;break; //4
	  default:break;
	} 	
}
//ou1-ou4
void ou1_ou4(uchar x ,uchar y)
{
	cp[x*4+0]=0x5c;	 //0
	cp[x*4+1]=0x1c;	 //u
	cp[x*4+2]=0x40;	 //-
	switch(y)
	{
	  case 1: cp[x*4+3]=0x06;break; //1
	  case 2: cp[x*4+3]=0x5b;break; //2
	  case 3: cp[x*4+3]=0x4f;break; //3
	  case 4: cp[x*4+3]=0x66;break; //4
	  default:break;
	} 	
}
//de-L
void dealy_disp(uchar x)
{
	cp[x*4+0]=0x5e;
	cp[x*4+1]=0x79;
	cp[x*4+2]=0x40;
	cp[x*4+3]=0x38;
}
//set
void set_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x6d;
	cp[x*4+2]=0x79;
	cp[x*4+3]=0x78;
}
//inpt
void inpt_disp(uchar x)
{
	cp[x*4+0]=0x04;
	cp[x*4+1]=0xd4;
	cp[x*4+2]=0x73;
	cp[x*4+3]=0x78;
}
//conn
void conn_disp(uchar x)
{
	cp[x*4+0]=0x39;
	cp[x*4+1]=0x5c;
	cp[x*4+2]=0x54;
	cp[x*4+3]=0x54;
}

//disp
void disp_disp(uchar x)
{
	cp[x*4+0]=0x5e;
	cp[x*4+1]=0x04;
	cp[x*4+2]=0x6d;
	cp[x*4+3]=0x73;
}

//clre
void clre_disp(uchar x)
{
	cp[x*4+0]=0x39;
	cp[x*4+1]=0x38;
	cp[x*4+2]=0xd0;
	cp[x*4+3]=0x79;
}
//net
void net_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x54;
	cp[x*4+2]=0x79;
	cp[x*4+3]=0x78;
}

//uscl
void uscl_disp(uchar x)
{
	cp[x*4+0]=0xbe;
	cp[x*4+1]=0x6d;
	cp[x*4+2]=0x39;
	cp[x*4+3]=0x38;
}
//iscl
void iscl_disp(uchar x)
{
	cp[x*4+0]=0x84;
	cp[x*4+1]=0x6d;
	cp[x*4+2]=0x39;
	cp[x*4+3]=0x38;
}
//pt
void pt_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x00;
	cp[x*4+2]=0x73;
	cp[x*4+3]=0x78;
}
//ct
void ct_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x00;
	cp[x*4+2]=0x39;
	cp[x*4+3]=0x78;
}
//sn
void sn_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x00;
	cp[x*4+2]=0x6d;
	cp[x*4+3]=0x54;
}
//baud
void baud_disp(uchar x)
{
	cp[x*4+0]=0x7c;
	cp[x*4+1]=0x77;
	cp[x*4+2]=0x3e;
	cp[x*4+3]=0x5e;
}
//data
void data_disp(uchar x)
{
	cp[x*4+0]=0x5e;
	cp[x*4+1]=0x77;
	cp[x*4+2]=0x78;
	cp[x*4+3]=0x77;
}
//end
void end_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x79;
	cp[x*4+2]=0x54;
	cp[x*4+3]=0x5e;
}

//
void sxsx_disp(uchar x,ssx)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0xd4;
	cp[x*4+2]=0xcf;
	if(ssx==0)cp[x*4+3]=0x66;
	else cp[x*4+3]=0x4f;
}
//
void usys_disp(uchar x,ssx)
{
	if(ssx==0)cp[x*4+0]=0x66;//400
	else cp[x*4+0]=0x06;	 //100
	cp[x*4+1]=0x3f;
	cp[x*4+2]=0x3f;
	cp[x*4+3]=0x3e;
}
//
void isyw_disp(uchar x,ssx)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x00;
	if(ssx==0)cp[x*4+2]=0x6d;//5A
	else cp[x*4+2]=0x06;	 //1A
	cp[x*4+3]=0x77;
}
void btl_disp(uchar x,ssx)
{
	if(ssx==0){cp[x*4+0]=0x6f;cp[x*4+1]=0x7d;}
	else {cp[x*4+0]=0x66;cp[x*4+1]=0x7f;}
	cp[x*4+2]=0x3f;
	cp[x*4+3]=0x3f;
}
void cms_disp(uchar x,ssx)
{
	cp[x*4+0]=0x00;
	if(ssx==0){cp[x*4+1]=0xd4;}
	else if(ssx==1){cp[x*4+1]=0xf9;}
	else if(ssx==2){cp[x*4+1]=0xdc;}
	cp[x*4+2]=0xff;
	cp[x*4+3]=0x06;
}
void old_disp(uchar x)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x5c;
	cp[x*4+2]=0x38;
	cp[x*4+3]=0x5e;
}
void nye_disp(uchar x,ssx)
{
	cp[x*4+0]=0x00;
	cp[x*4+1]=0x54;
	cp[x*4+2]=0x40;
	if(ssx==0)cp[x*4+3]=0x06;
	else cp[x*4+3]=0x5b;
}
/********自检显示******/
void zijian_display(uchar x,uchar y)
{
	cp[x*4+y]=shu[zijian]|0x80;	
}
/********读取DI******/
void dout_di (uchar x)
{ 
	if(dia)
	cp[x*4+0]=0x3F;
	else
	cp[x*4+0]=0x06;
	if(dib)
	cp[x*4+1]=0x3F;
	else
	cp[x*4+1]=0x06;
	if(dic)
	cp[x*4+2]=0x3F;
	else
	cp[x*4+2]=0x06;
	if(did)
	cp[x*4+3]=0x3F;
	else
	cp[x*4+3]=0x06;	
}
/********读取DO******/
void dout_do (uchar x)
{ 
	if(k1)
	cp[x*4+0]=0x06;
	else
	cp[x*4+0]=0x3F;
	if(k2)
	cp[x*4+1]=0x06;
	else
	cp[x*4+1]=0x3F;
	if(k3)
	cp[x*4+2]=0x06;
	else
	cp[x*4+2]=0x3F;
	if(k4)
	cp[x*4+3]=0x06;
	else
	cp[x*4+3]=0x3F;	
}
/**********显示VA视载功率**********/
//显示功率
void pqs_disp_VA(bit hhw,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		mm_tt=1;
	
	}else {mm_tt=0;}
	if(hhw)
	{
		cp[x*4]=0x40;  // -
		if(d_v<10000)
		{
			d_v=d_v/10;
//			cp[x*4+1]=shu[d_v/100]+0x80;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/100;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/1000;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80; 
//			cp[x*4+3]=shu[d_v%10];
		}
	}
	else
	{
		if(d_v<10000)
		{
			cp[x*4+0]=shu[d_v/1000]+0x80;
//			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/10;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100]+0x80;
//			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/100;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<10000000)
		{
			d_v=d_v/1000;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
	}
}
/*************************************/
/***********************************/
/*********显示A  相功率 **********/
//显示功率
void pqs_disp_A(bit hhw,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		mm_tt=1;
		
	}else {mm_tt=0;;}
	if(hhw)
	{
		cp[x*4]=0x40;  // -
		if(d_v<10000)
		{
			d_v=d_v/10;
			cp[x*4+1]=shu[d_v/100]+0x80;
//			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/100;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/1000;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80; 
//			cp[x*4+3]=shu[d_v%10];
		}
	}
	else
	{
		if(d_v<10000)
		{
			cp[x*4+0]=shu[d_v/1000]+0x80;
//			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{
			d_v=d_v/10;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100]+0x80;
//			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{
			d_v=d_v/100;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<10000000)
		{
			d_v=d_v/1000;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10]+0x80;
//			cp[x*4+3]=shu[d_v%10];
		}
	}
}
/*************************************/
/*********显示B  相功率 **********/
//显示功率
void pqs_disp_B(bit hhw,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		mm_tt=1;
		cp[14]=0x08;
	}else {mm_tt=0;cp[14]=0x04;}
	if(hhw)
	{
		cp[x*4]=0x40;  // -	 		
		if(d_v<10000)
		{  			
			d_v=d_v/10;
			cp[x*4+1]=shu[d_v/100]+0x80;
//			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{	
			d_v=d_v/100;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{	
			d_v=d_v/1000;
			cp[x*4+1]=shu[d_v/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80; 
			cp[x*4+3]=shu[d_v%10];
		}
	}
	else
	{
		if(d_v<10000)
		{	
			cp[x*4+0]=shu[d_v/1000]+0x80;
//			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<100000)
		{	
			d_v=d_v/10;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100]+0x80;
//			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
		else
		if(d_v<1000000)
		{	
			d_v=d_v/100;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10]+0x80;
//			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
		}
		else
		if(d_v<10000000)
		{	
			d_v=d_v/1000;
			cp[x*4+0]=shu[d_v/1000];
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
//			cp[x*4+3]=shu[d_v%10]+0x80;
			cp[x*4+3]=shu[d_v%10];
		}
	}
}
/*************************************/
/**********显示ABC功率因数**************/
void swxs_disp_cos(bit fh,uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		kk_tt=1;
	}else kk_tt=0;
	if(d_v<10000)
	{
		if(fh)
		{
			cp[x*4]=0x40;
			cp[x*4+1]=shu[d_v/1000]+0x80;
			cp[x*4+2]=shu[d_v%1000/100];
			cp[x*4+3]=shu[d_v%100/10];
			cp[11]=0x80;
		}
		else
		{
			cp[x*4]=shu[d_v/1000]+0x80;
			cp[x*4+1]=shu[d_v%1000/100];
			cp[x*4+2]=shu[d_v%100/10];
			cp[x*4+3]=shu[d_v%10];
			cp[11]=0x80;
		}
	}
	else
	if(d_v<100000)
	{
		cp[x*4]=shu[d_v/10000];
		cp[x*4+1]=shu[d_v%10000/1000]+0x80;
		cp[x*4+2]=shu[d_v%1000/100];
		cp[x*4+3]=shu[d_v%100/10];
		cp[11]=0x80;
	}
	else
	if(d_v<1000000)
	{
		cp[x*4]=shu[d_v/100000];
		cp[x*4+1]=shu[d_v%100000/10000];
		cp[x*4+2]=shu[d_v%10000/1000]+0x80;
		cp[x*4+3]=shu[d_v%1000/100];
		cp[11]=0x80;
	}
	else
	if(d_v<10000000)
	{
		cp[x*4]=shu[d_v/1000000];
		cp[x*4+1]=shu[d_v%1000000/100000];
		cp[x*4+2]=shu[d_v%100000/10000];
		cp[x*4+3]=shu[d_v%10000/1000];
		cp[11]=0x80;
	}
}
/***************************************/
void cs_disp(void)
{
	if(sj_bc)
	{
		cp[0]=0x6d;
		cp[1]=0x77;
		cp[2]=0x3e;
		cp[3]=0x79;	

		cp[4]=0x00;
		cp[5]=0x6e;
		cp[6]=0x79;
		cp[7]=0x6d;

		bxs_disp(2);
	}
	else
	{
		switch(csyi)
		{
			case 0:		//密码登录
			{
				code_disp(0);
				bxs_disp(1);
				swzs_disp(2,mima);
			}break;
			case 1:		//
			{
				set_disp(0);
				if(cser==0)
				{
					bxs_disp(1);
					bxs_disp(2);
				}
				else 
				if(cser==1)		//显示方式
				{
					disp_disp(1);
					if(da_tt)swzs_disp(2,xsfs);
					else bxs_disp(2);
				}
				else 
				if(cser==2)		//电能清0
				{
					clre_disp(1);
					if(da_tt)end_disp(2);
					else bxs_disp(2);
				}else 				
				if(cser==3)		//亮度调节
				{
					DP_disp(1);//DP--
					if(da_tt)swzs_disp(2,Disp_liangdu);
					else bxs_disp(2);
				}else
				if(cser==4)		//Do1模式
				{
					do1_do4(1,1);
					if(da_tt)swzs_disp(2,Do1_moshi);
					else bxs_disp(2);
				} else
				if(cser==5)		//Do2模式
				{
					do1_do4(1,2);
					if(da_tt)swzs_disp(2,Do2_moshi);
					else bxs_disp(2);
				}else
				if(cser==6)		//Do3模式
				{
					do1_do4(1,3);
					if(da_tt)swzs_disp(2,Do3_moshi);
					else bxs_disp(2);
				}else
				if(cser==7)		//Do4模式
				{
					do1_do4(1,4);
					if(da_tt)swzs_disp(2,Do4_moshi);
					else bxs_disp(2);
				}else
				if(cser==8)	  //OC1
				{
					oc1_oc4(1 ,1);
					if(da_tt)swzs_disp(2,kout_shu_A1);
					else bxs_disp(2);
				}else
				if(cser==9)	  //OC2
				{
					oc1_oc4(1 ,2);
					if(da_tt)swzs_disp(2,kout_shu_A2);
					else bxs_disp(2);
				}else
				if(cser==10)	  //OC3
				{
					oc1_oc4(1 ,3);
					if(da_tt)swzs_disp(2,kout_shu_A3);
					else bxs_disp(2);
				}else
				if(cser==11)	  //OC4
				{
					oc1_oc4(1 ,4);
					if(da_tt)swzs_disp(2,kout_shu_A4);
					else bxs_disp(2);
				}else

				if(cser==12)	  //OU1
				{
					ou1_ou4(1 ,1);
					if(da_tt)swzs_disp(2,kout_shu_V1);
					else bxs_disp(2);
				}else
				if(cser==13)	  //OU2
				{
					ou1_ou4(1 ,2);
					if(da_tt)swzs_disp(2,kout_shu_V2);
					else bxs_disp(2);
				}else
				if(cser==14)	  //OU3
				{
					ou1_ou4(1 ,3);
					if(da_tt)swzs_disp(2,kout_shu_V3);
					else bxs_disp(2);
				}else
				if(cser==15)	  //OU4
				{
					ou1_ou4(1 ,4);
					if(da_tt)swzs_disp(2,kout_shu_V4);
					else bxs_disp(2);
				}else
				if(cser==16)
				{
				   dealy_disp(1);
				   if(da_tt)ywxs_disp(2,Do_timer_S);
				   else bxs_disp(2);
				}else
				if(cser==17)
				{
					pt_disp(1);
					if(da_tt)swzs_disp(2,fig_pt);
				    else bxs_disp(2);					
				}
			}break;
			case 2:
			{
				inpt_disp(0);
				if(cser==0)
				{
					bxs_disp(1);
					bxs_disp(2);
				}
				else 
				if(cser==1)		//三相4线&三相三线
				{
					net_disp(1);
					if(da_tt)sxsx_disp(2,sx_sx); 
					else bxs_disp(2);
				}
				else 
				if(cser==2)		//电压100V&400V
				{
					uscl_disp(1);
					if(da_tt)usys_disp(2,us_ys); 
					else bxs_disp(2);
				}
				else 
				if(cser==3)		//电流1A&5A
				{
					iscl_disp(1);
					if(da_tt)isyw_disp(2,is_yw); 
					else bxs_disp(2);
				}
				else 
				if(cser==4)		//电压变比
				{
					pt_disp(1);
					if(da_tt)swzs_disp(2,dybb);
					else bxs_disp(2);
				}
				else 
				if(cser==5)		//电流变比
				{
					ct_disp(1);
					if(da_tt)swzs_disp(2,dlbb);
					else bxs_disp(2);
				}
			}break;
			case 3:
			{
				conn_disp(0);
				if(cser==0)
				{
					bxs_disp(1);
					bxs_disp(2);
				}
				else 
				if(cser==1)		//
				{
					sn_disp(1);
					if(da_tt)swzs_disp(2,ipdz);
					else bxs_disp(2);
				}
				else 
				if(cser==2)		//
				{
					baud_disp(1);
					if(da_tt)btl_disp(2,btl_t); 
					else bxs_disp(2);
				}
				else 
				if(cser==3)		//
				{
					data_disp(1);
					if(da_tt)cms_disp(2,shez&0x03);
					else bxs_disp(2);
				}
			}break;
			case 4:		//修改密码
			{
				code_disp(0);
				if(cser==0)
				{
					bxs_disp(1);
					bxs_disp(2);
				}
				else 
				if(cser==1)		//
				{
					if(da_tt)
					{
						old_disp(1);
						swzs_disp(2,xgma);
					}
					else
					{
						code_disp(1);
						bxs_disp(2);
					}
				}
				else 
				if(cser==2)		//
				{
					nye_disp(1,0);
					swzs_disp(2,xgmb);
				}
				else 
				if(cser==3)		//
				{
					nye_disp(1,1);
					swzs_disp(2,xgmc);
				}
			}break;
			default:break;
		}
	}
}

//有功电能
void ygdn_disp(void)
{
	cp[ 0]=shu[ygdnb/1000000000];
	cp[ 1]=shu[ygdnb%1000000000/100000000];
	cp[ 2]=shu[ygdnb%100000000/10000000];
	cp[ 3]=shu[ygdnb%10000000/1000000]+0x80;
	cp[ 4]=shu[ygdnb%1000000/100000];
	cp[ 5]=shu[ygdnb%100000/10000];
	cp[ 6]=shu[ygdnb%10000/1000];
	cp[ 7]=shu[ygdnb%1000/100];
	cp[ 8]=shu[ygdnb%100/10];
	cp[ 9]=shu[ygdnb%10]+0x80;
	cp[10]=shu[ygdna/10];
	cp[11]=shu[ygdna%10];

}
//无功电能
void wgdn_disp(void)
{
	cp[ 0]=0x00;
	cp[ 1]=0x00;
	cp[ 2]=shu[wgdna/1000000000];
	cp[ 3]=shu[wgdna%1000000000/100000000];
	cp[ 4]=shu[wgdna%100000000/10000000];
	cp[ 5]=shu[wgdna%10000000/1000000];
	cp[ 6]=shu[wgdna%1000000/100000];
	cp[ 7]=shu[wgdna%100000/10000]+0x80;
	cp[ 8]=shu[wgdna%10000/1000];
	cp[ 9]=shu[wgdna%1000/100]+0x80;
	cp[10]=shu[wgdna%100/10];
	cp[11]=shu[wgdna%10];
}
void led_dd()
{
	if(cs_tt)
	{
		cp[12]=0x00;
		cp[13]=0x00;
		//cp[14]=0x02;	//DO
		//cp[14]=0x01;	//DI	
		cp[14]=0x00;		
	}
	else
	if(xian==0)	//电压
	{
		if(sx_sx==0)
		{
			if(xn_sx)cp[12]=0x7b;
			else cp[12]=0x29;
		}else cp[12]=0x7b;
		cp[13]=0x21;
		if(kk_tt)cp[14]=0x24;
		else cp[14]=0x20;
	}
	else 
	if(xian==1)//电流
	{
		cp[12]=0x29;
		cp[13]=0x12;
		if(kk_tt)cp[14]=0x14;
		else cp[14]=0x10;
	}
	else 
	if(xian==2)//有功功率,无功功率,功率因数,
	{
		cp[12]=0x00;
		cp[13]=0x00;
		cp[14]=0x04;		
	}
	else 
	if(xian==3)//频率
	{
		cp[12]=0x00;
		cp[13]=0x40;		//Hz
		if(kk_tt)cp[14]=0x38;
		else cp[14]=0x34;
		
	}
	else 
	if(xian==4)//有功电能
	{
		cp[12]=0x00;
		cp[13]=0x00;
		cp[14]=0x84;//kh	
	}
	else 
	if(xian==5)//无功电能
	{
		cp[12]=0x00;
		cp[13]=0x08;
		cp[14]=0x04;//kh		
	}
	else
	if(xian==6)	// A B C相有功功率
	{
	 	cp[12]=0x29;
		cp[13]=0x00;
		if(kk_tt)cp[14]=0x08;
		else cp[14]=0x04;//kw
	}
	else
	if(xian==7)	// A B C相无功功率
	{
	 	cp[12]=0x29;
		cp[13]=0x00;
		if(kk_tt)cp[14]=0x08;
		else cp[14]=0x04;//var
	}
	else
	if(xian==8)	// A B C相功率因数
	{
	 	cp[12]=0x29;
		cp[13]=0x00;
		cp[14]=0x00;//cos
	}
	else
	 if(xian==9)   // ABC相视载功率
	 {
	  	cp[12]=0x29;
		cp[13]=0x33;
		cp[14]=0x34;//VA
	 }
	 else
	 if(xian==10)
	 {
	 	cp[12]=0x00;
		cp[13]=0x00;
		cp[14]=0x00;
	 }
	 if((k1||k2||k3||k4)==1)
		 cp[14]=cp[14]|0x02;
	 if((dia==0)||(dib==0)||(dic==0)||(did==0))
		 cp[14]=cp[14]|0x01;
	 if(fig_tongxun)
		 cp[12]=cp[12]|0x04;
}

//显示
void led_disp(void)
{
	ulong date;
	led_dd();
	if(cs_tt)
	{
		if(xb_bz)
		{
			if(ss_tt)
			{
				bxs_disp(0);
				bxs_disp(1);
				bxs_disp(2);
			}
			else
			{
				ywxs_disp(0,2200);
				swxs_disp(0,1,5000);
				swxs_disp(0,2,1000);
			}
		}
		else
		if(xj_mz)
		{
			if(ss_tt)
			{
				bxs_disp(0);
				bxs_disp(1);
				bxs_disp(2);
			}
			else
			{
				ywxs_disp(0,2200);
				swxs_disp(0,1,5000);
				swxs_disp(0,2,500);
			}
		}
		else
		if(xj_bs)
		{
			if(ss_tt)
			{
				bxs_disp(0);
				bxs_disp(1);
				bxs_disp(2);
			}
			else
			{
				ywxs_disp(0,2200);
				swxs_disp(0,1,500);
				swxs_disp(0,2,500);
			}
		}
		else
		if(in_tt)
		{
			bxs_disp(0);
			if(in_js==0)
			{
				cp[4]=0x00;
				cp[5]=0x00;
				cp[6]=0x00;
				cp[7]=0x00;	
			}
			else
			if(in_js==1)
			{
				cp[4]=0x40;
				cp[5]=0x00;
				cp[6]=0x00;
				cp[7]=0x00;
			}
			else
			if(in_js==2)
			{
				cp[4]=0x40;
				cp[5]=0x40;
				cp[6]=0x00;
				cp[7]=0x00;
			}
			else
			if(in_js==3)
			{
				cp[4]=0x40;
				cp[5]=0x40;
				cp[6]=0x40;
				cp[7]=0x00;
			}
			else
			if(in_js==4)
			{
				cp[4]=0x40;
				cp[5]=0x40;
				cp[6]=0x40;
				cp[7]=0x40;
			}
			bxs_disp(2);	
		}
		else cs_disp();
	}
	else
	{
		switch(xian)
		{
			case 0:	//三相电压
			{
				if(sx_sx)
				{
					if(fig_pt)
					{
						date=(ax_av*dybb+cx_av*dybb)*0.8659;
						ywxs_disp(0,date);
						date=(ax_av*dybb+cx_av*dybb)*0.8659;
						ywxs_disp(1,date);
						date=(cx_av*dybb+ax_av*dybb)*0.8659;
						ywxs_disp(2,date);
					}else
					{
						date=(ax_av*dybb+bx_av*dybb)*0.8659;
						ywxs_disp(0,date);
						date=(bx_av*dybb+cx_av*dybb)*0.8659;
						ywxs_disp(1,date);
						date=(cx_av*dybb+ax_av*dybb)*0.8659;
						ywxs_disp(2,date);
					}
				}
				else
				{
					if(xn_sx==0)
					{
						date=ax_av*dybb;
						ywxs_disp(0,date);
						date=bx_av*dybb;
						ywxs_disp(1,date);
						date=cx_av*dybb;
						ywxs_disp(2,date);
					}
					else
					{
						date=(ax_av*dybb+bx_av*dybb)*0.8659;
						ywxs_disp(0,date);
						date=(bx_av*dybb+cx_av*dybb)*0.8659;
						ywxs_disp(1,date);
						date=(cx_av*dybb+ax_av*dybb)*0.8659;
						ywxs_disp(2,date);
					}
				}
			}break;
			case 1:	//三相电流
			{
				date=ax_aa*dlbb;
				swxs_disp(0,0,date);
				date=bx_aa*dlbb;
			//	if(sx_sx)bxs_disp(1);
			//	else 
				swxs_disp(0,1,date);
				date=cx_aa*dlbb;
				swxs_disp(0,2,date);
			}break;
			case 2://总
			{
				date=ygglt*dybb*dlbb;
				pqs_disp(yg_pt,0,date);	//总有功功率
				date=wgglt*dybb*dlbb;
				pqs_disp(wg_qt,1,date);	//总无功功率
				date=szglt*dybb*dlbb;
				swxs_disp(gl_yt,2,glyst/10);//功率因数
				cp[11]=cp[11]+0x80;
				
			}break;
			case 3: //
			{
				pqs_disp_VA(0,0,szglt*dlbb*dybb);
				bxs_disp(1);
				ewxs_disp(2,plhz);		//频率
			}break;
			case 4:ygdn_disp();break;  //总有功电能
			case 5:wgdn_disp();break;  //总无功电能
			case 6:				   		//ABC相有功功率 
			{
			 	pqs_disp_A(yg_pa,0,yggla*dlbb*dybb);	//A有功功率
				pqs_disp_B(yg_pb,1,ygglb*dlbb*dybb);	//B有功功率	
				pqs_disp_B(yg_pc,2,ygglc*dlbb*dybb);	//C有功功率
			}break;
			case 7:						//ABC相无功功率
			{
			  	pqs_disp_B(wg_qa,0,wggla*dlbb*dybb);	//A无功功率
				pqs_disp(wg_qb,1,wgglb*dlbb*dybb);	//B无功功率
				pqs_disp_B(wg_qc,2,wgglc*dlbb*dybb);	//C无功功率
			}break;
			case 8:					   //ABC相功率因数
			{
			  	swxs_disp_cos(gl_ya,0,glysa/10);//功率因数 					
				swxs_disp_cos(gl_yb,1,glysb/10);
				swxs_disp_cos(gl_yc,2,glysc/10);
			}break;
			case 9:						//ABC相视载功率
			{
		//		cp[11]&=0x7f;
				pqs_disp_VA(0,0,szgla*dlbb*dybb);
				pqs_disp_VA(0,1,szglb*dlbb*dybb);
				pqs_disp_VA(0,2,szglc*dlbb*dybb);			   
			}break;
			case 10:
			{
				dido_disp(0);
				dout_di(1);
				dout_do(2);
			}
			default:break;
		}
	}
}