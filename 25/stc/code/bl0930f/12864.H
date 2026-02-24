/*定义控制端口*/
sbit Din=P1^3;	//定义DIN数据端口
sbit CLK=P1^2;	//定义CLK数据端口
	
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
// do1
void do1_do4(uchar x)  //输出功能
{
	cp[x*4+0]=0x5E;	 //d
	cp[x*4+1]=0x5c;	 //o
	cp[x*4+2]=0x40;	 //-	
	cp[x*4+3]=0x06;	 //1	
}
//ou1
void ou1_ou4(uchar x)
{
	cp[x*4+0]=0x5c;	 //0
	cp[x*4+1]=0x58;	 //u
	cp[x*4+2]=0x40;	 //-
	cp[x*4+3]=0x06;	 //1
} 
//de-L
void dealy_disp(uchar x)
{
	cp[x*4+0]=0x5e;
	cp[x*4+1]=0x79;
	cp[x*4+2]=0x40;
	cp[x*4+3]=0x38;
}

/********读取DI******/
void dout_di (uchar x)
{ 
	if(dia)
	cp[x*4+0]=shu[0];
	else
	cp[x*4+0]=shu[1];
	if(dib)
	cp[x*4+1]=shu[0];
	else
	cp[x*4+1]=shu[1];
	cp[x*4+2]=0x00;
	cp[x*4+3]=0x00;
}
/********读取DO******/
void dout_do (uchar x)
{ 
	if(k1)
	cp[x*4+0]=shu[1];
	else
	cp[x*4+0]=shu[0];
	cp[x*4+1]=0x00;
	cp[x*4+2]=0x00;
	cp[x*4+3]=0x00;
}
//iA-dp	 ib-dp ib-dp
void uA_dp_disp(uchar x,uchar y)
{
	cp[x*4+0]=0x30;//i
	switch(y)
	{
		case 0:	cp[x*4+1]=0x77;break; //A
		case 1: cp[x*4+1]=0x7c;break;//b
		case 2: cp[x*4+1]=0x58;break;//c
		default: cp[x*4+1]=0x30;break;//i
	}	
	cp[x*4+2]=0x5e;
	cp[x*4+3]=0x73;
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
//自检
void zijian_A(uchar x,uchar dat)
{
    cp[x*4+0]=shu[dat]+0x80;
	cp[x*4+1]=shu[dat]+0x80;
	cp[x*4+2]=shu[dat]+0x80;
	cp[x*4+3]=shu[dat]+0x80;
}
//0.000-9.999
void swxs_disp(uchar x,ulong d_v)
{
	if(d_v>10000000)
	{
		d_v=d_v/1000;
		kk_tt=1;
	}else kk_tt=0;
	if(d_v<10000)
	{
		cp[x*4]=shu[d_v/1000]+0x80;
		cp[x*4+1]=shu[d_v%1000/100];
		cp[x*4+2]=shu[d_v%100/10];
		cp[x*4+3]=shu[d_v%10];
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
void bxs_disp(uchar x)
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
	cp[x*4+1]=0xd8;
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
	cp[x*4+2]=0x79;
	cp[x*4+3]=0x5e;
}
//data
void data_disp(uchar x)
{
	cp[x*4+0]=0x5e;
	cp[x*4+1]=0x77;
	cp[x*4+2]=0x3e;
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
				if(cser==4)	  //DO输出模式
				{
					 do1_do4(1);
					 if(da_tt)swzs_disp(2,Do1_moshi);
					 else bxs_disp(2);
				}else 
				if(cser==5)	 //OU
				{
					ou1_ou4(1);
					if(da_tt)swzs_disp(2,kout_shu_V1);
					else bxs_disp(2);
				}else
				if(cser==6)//DL
				{
					dealy_disp(1);
					if(da_tt)ywxs_disp(2,Do_timer_S);
					else bxs_disp(2);	
				}else
				if(cser==7)
				{
					ct_disp(1);
					if(da_tt)swzs_disp(2,ct_shuliang);
					else bxs_disp(2);

				}else
				if(cser==8)	  //CODE
				{
					code_disp(1);
					if(da_tt)swzs_disp(2,xzmima);
					else bxs_disp(2);
				}else
				if(cser==9)
				{
					swzs_disp(0,dljza);
					uA_dp_disp(1,0);					
					if(da_tt)swxs_disp(2,ax_aa);
					else bxs_disp(2);
				}else
				if(cser==10)
				{
					swzs_disp(0,dljzb);
				    uA_dp_disp(1,1);
					if(da_tt)swxs_disp(2,bx_aa);
					else bxs_disp(2);
				}else
				if(cser==11)
				{	swzs_disp(0,dljzc);
					uA_dp_disp(1,2);
					if(da_tt)swxs_disp(2,cx_aa);
					else bxs_disp(2);
				}else
				if(cser==12)
				{
					set_disp(0);
					uA_dp_disp(1,3);
					if(da_tt)swxs_disp(2,pingbidianliu);
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
void led_dd()
{
	if(cs_tt)
	{
		cp[12]=0x00;
		cp[13]=0x00;
		cp[14]=0x00;		
	}
	else
	{
		if(xian==0)	//电流
		{
			cp[12]=0x29;
			cp[13]=0x12;
			if(kk_tt)cp[14]=0x14;
			else cp[14]=0x10;
		}
		else
		{
			cp[12]=0x00;
			cp[13]=0x00;
			cp[14]=0x00;
		}

	}
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
				swxs_disp(0,5000);
				swxs_disp(1,5000);
				swxs_disp(2,5000);
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
//		date=ax_av*dybb;
//		ywxs_disp(0,date);	//电压A
//		date=bx_av*dybb;
//		ywxs_disp(1,date);	//电压B
//		date=cx_av*dybb;
//		ywxs_disp(0,date);	//电压C
		if(xian==0)
		{
			date=ax_aa*dlbb;
			swxs_disp(0,date);	//电流A
			date=bx_aa*dlbb;
			swxs_disp(1,date);	//电流B
			date=cx_aa*dlbb;
			swxs_disp(2,date);	//电流C		 
		}
		else
		{
			dido_disp(0);
			dout_di(1);
			dout_do(2);
		}
	}
}
