/***********************************
*包括4个按键,LED显示
*3路电压,3路电流输入
*4路开关量输入 4路开关量输出
*创建:xielungui
*日期:2018-04-02 09:28
************************************/

//#include <24c02.h>
#include <eprom.h>
#include <12864.h>
#include <ht7050.h>
#include <canshu.h>


void com_txsj(uint zjzs)
{
	uint i;
	EA=0;
	for(i=0;i<zjzs;i++)
	{
		SBUF=tp[i];
		while(TI==0){;}
		TI=0;
	}
	EA=1;	
}

//通信
void yc_shuju(void)
{
	uint i,j,k;
	tp[0]=ipdz;
	tp[1]=txgn;
	if(txgn==3)
	{
		k=zjcd*2;
		tp[2]=k;
		j=jcqa*2;
		for(i=0;i<k;i++)
		{
			tp[3+i]=yb[j+i];
		}
		k=zjcd*2+3;
	}
	else
	if(txgn==4)
	{
		k=6;
		tp[2]=2;
		abc=read_7053c(jcqa);
		tp[3]=abc>>16&0xff;
		tp[4]=abc>>8&0xff;
		tp[5]=abc&0xff;
	}
	else
	{
		k=4;
		tp[2]=1;
		tp[3]=0;	
	}
	jcrc=Crc_date(0,k);
	tp[k+0]=jcrc&0xff;
	tp[k+1]=jcrc>>8;
	r485=1;
	com_txsj(k+2);	//fasong
	r485=0;
}

//读数据
void du_dldy(void)
{
	xlong acc;
	if(wd_tt==0)
	{
		wd_tt=1;
		switch(wd_cj)
		{
			case 0:
			{
				Float_to_IEEE754(0x00);	//a电压
				yb[114]=IEEE[0];
				yb[115]=IEEE[1];
				yb[116]=IEEE[2];
				yb[117]=IEEE[3];

				Float_to_IEEE754(0x00);//b电压
				yb[118]=IEEE[0];
				yb[119]=IEEE[1];
				yb[120]=IEEE[2];
				yb[121]=IEEE[3];
				Float_to_IEEE754(0x00);//c电压
				yb[122]=IEEE[0];
				yb[123]=IEEE[1];
				yb[124]=IEEE[2];
				yb[125]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[126]=IEEE[0];
				yb[127]=IEEE[1];
				yb[128]=IEEE[2];
				yb[129]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[130]=IEEE[0];
				yb[131]=IEEE[1];
				yb[132]=IEEE[2];
				yb[133]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[134]=IEEE[0];
				yb[135]=IEEE[1];
				yb[136]=IEEE[2];
				yb[137]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[214]=IEEE[0];
				yb[215]=IEEE[1];
				yb[216]=IEEE[2];
				yb[217]=IEEE[3];
	
				abc=read_7053c(0x06);	//读电流
				if(abc>0x7fffff)abc=0;
				ax_aa=abc*10/dljza;
				if(ax_aa<pingbidianliu)ax_aa=0;
				if(is_yw)ax_aa=(float)ax_aa/5;
				acc=ax_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[138]=IEEE[0];
				yb[139]=IEEE[1];
				yb[140]=IEEE[2];
				yb[141]=IEEE[3];
				abc=read_7053c(0x07);	//读电流
				if(abc>0x7fffff)abc=0;
				bx_aa=abc*10/dljzb;
				if(bx_aa<pingbidianliu)bx_aa=0;
				if(is_yw)bx_aa=(float)bx_aa/5;
				acc=bx_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[142]=IEEE[0];
				yb[143]=IEEE[1];
				yb[144]=IEEE[2];
				yb[145]=IEEE[3];
				abc=read_7053c(0x08);	//读电流
				if(abc>0x7fffff)abc=0;
				cx_aa=abc*10/dljzc;
				if(cx_aa<pingbidianliu)cx_aa=0;
				if(is_yw)cx_aa=(float)cx_aa/5;
				acc=cx_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[146]=IEEE[0];
				yb[147]=IEEE[1];
				yb[148]=IEEE[2];
				yb[149]=IEEE[3];
			}break;
			case 1:
			{
				Float_to_IEEE754(0x00);
				yb[150]=IEEE[0];
				yb[151]=IEEE[1];
				yb[152]=IEEE[2];
				yb[153]=IEEE[3];

				yb[162]=IEEE[0];
				yb[163]=IEEE[1];
				yb[164]=IEEE[2];
				yb[165]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[154]=IEEE[0];
				yb[155]=IEEE[1];
				yb[156]=IEEE[2];
				yb[157]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[158]=IEEE[0];
				yb[159]=IEEE[1];
				yb[160]=IEEE[2];
				yb[161]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[166]=IEEE[0];
				yb[167]=IEEE[1];
				yb[168]=IEEE[2];
				yb[169]=IEEE[3];

				yb[178]=IEEE[0];
				yb[179]=IEEE[1];
				yb[180]=IEEE[2];
				yb[181]=IEEE[3];


				Float_to_IEEE754(0x00);
				yb[170]=IEEE[0];
				yb[171]=IEEE[1];
				yb[172]=IEEE[2];
				yb[173]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[174]=IEEE[0];
				yb[175]=IEEE[1];
				yb[176]=IEEE[2];
				yb[177]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[182]=IEEE[0];
				yb[183]=IEEE[1];
				yb[184]=IEEE[2];
				yb[185]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[186]=IEEE[0];
				yb[187]=IEEE[1];
				yb[188]=IEEE[2];
				yb[189]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[190]=IEEE[0];
				yb[191]=IEEE[1];
				yb[192]=IEEE[2];
				yb[193]=IEEE[3];

				Float_to_IEEE754(0x00);
				yb[194]=IEEE[0];
				yb[195]=IEEE[1];
				yb[196]=IEEE[2];
				yb[197]=IEEE[3];
	
				Float_to_IEEE754(0x00);
				yb[198]=IEEE[0];
				yb[199]=IEEE[1];
				yb[200]=IEEE[2];
				yb[201]=IEEE[3];

				yb[210]=IEEE[0];
				yb[211]=IEEE[1];
				yb[212]=IEEE[2];
				yb[213]=IEEE[3];


				Float_to_IEEE754(0x00);
				yb[202]=IEEE[0];
				yb[203]=IEEE[1];
				yb[204]=IEEE[2];
				yb[205]=IEEE[3];
				Float_to_IEEE754(0x00);
				yb[206]=IEEE[0];
				yb[207]=IEEE[1];
				yb[208]=IEEE[2];
				yb[209]=IEEE[3];

			}break;
			default:break;
		}
	}
}

void bc_data(void)
{
	uchar i;
	ulong datbz;
	EA=0;
	datbz=0;
	for(i=0;i<10;i++)
	{
		abc=read_7053c(0x06);	//
		if(abc>0x7fffff)abc=0;
		datbz=datbz+abc;
	}
	if(datbz>0xfffff)
	{
		dljza=datbz/5000;
		ep[9]=dljza>>8;
		ep[10]=dljza&0xff;
	}
	datbz=0;
	for(i=0;i<10;i++)
	{
		abc=read_7053c(0x07);	//
		if(abc>0x7fffff)abc=0;
		datbz=datbz+abc;
	}
	if(datbz>0xfffff)
	{
		dljzb=datbz/5000;
		ep[11]=dljzb>>8;
		ep[12]=dljzb&0xff;
	}
	datbz=0;
	for(i=0;i<10;i++)
	{
		abc=read_7053c(0x08);	//
		if(abc>0x7fffff)abc=0;
		datbz=datbz+abc;
	}
	if(datbz>0xfffff)
	{
		dljzc=datbz/5000;
		ep[13]=dljzc>>8;
		ep[14]=dljzc&0xff;
	}	
	EA=1;
}
/*********K1输出******/
void k1_out()
{
	 uchar i;
	 switch(Do1_moshi)
	 {
	 	case 0:	  //开关量模式
		{
			if(dia==0)
			fig_out_k1=1;				
			else fig_out_k1=0;
		}break;			
		case 1:	 //过压报警输出
		{
			if((ax_aa*dlbb/1000>kout_shu_V1)||(bx_aa*dlbb/1000>kout_shu_V1)||(cx_aa*dlbb/1000>kout_shu_V1)) 
			 	 fig_out_k1=1;
			else fig_out_k1=0;			
		}break;
		default:break;
	 }
	yb[110]=0x03;
	i=P1&0x03;
	yb[111]=0x03-i;
	yb[112]=0x04;
	i=P1&0x10;
	yb[113]=i>>4;
}
//主函数开始
main() 
{
	uchar i;
	init();			//初始化
	in_att7053c();	//初始化7053c
	time1_init();
	while(zijian!=0)
	{
		out_disp();
		zijian_A(0,zijian);
		zijian_A(1,zijian);
		zijian_A(2,zijian);
		cp[12]=0xff;cp[13]=0xff;cp[14]=0xff;
	}
	while(1)
	{
		k1_out();   //DIDO
		if(sx_bz==0)
		{
			led_disp();	//LCD显示
			out_disp();
			sx_bz=1;
		}
		key_in();		//按键输入
		du_dldy();		//读电流电压
		if(xb_bz&xbjs==5)
		{
			if(bc_tt==0)
			{
				bc_data();
				xieru();
				bc_tt=1;
			}
		}
		if(eep>=7)		//接收数据
		{
			jcrc=Crc_date(1,6);
			ycrc=twoByteToint(rp[7],rp[6]);
			if(jcrc==ycrc)
			{
				if(ipdz==rp[0])
				{
					txgn=rp[1];
					jcqa=rp[3];
					zjcd=rp[5];
					yc_shuju();	//发送数据
					if(txgn==0x50)
					{
						cs_tt=1;xb_bz=1;bc_tt=0;
						mima=0;sj_tt=0;
					}
					else
					if(txgn==0x55)
					{
						cs_tt=1;
						in_tt=1;
						eeprom_erase();
						for(i=0;i<23;i++)
						{
							write_add(i,sd[i]);
							delay1(150);
						} 						
						init();
						in_att7053c();
						mima=0;
						sj_tt=0;
					}
				}
			}
			rx_tt=0;
			eep=0;
		}else delay1(100);	
	}
}

//定时器
void surt()interrupt 1
{
	wsms++;		//50MS
	if(wsms>10)
	{
		wsms=0;
		ss_tt=~ss_tt;		 
		  if(zijian!=0)
		  zijian--;		
		wd_cj++;
		if(wd_cj>1)wd_cj=0;
		wd_tt=0;sx_bz=0;
		if(in_tt)
		{
			in_js++;
			if(in_js>4)in_js=0;
		}else in_js=0;
		if(ss_tt==0)
		{
			if(xb_bz|in_tt)
			{
				xbjs++;
				if(xbjs>6)
				{
					xbjs=0;in_tt=0;
					xb_bz=0;
					cs_tt=0;
				}
			}
			if(cs_tt)
			{
				tcjs++;
				if(tcjs>=30)
				{
					mima=0;sj_tt=0;cs_tt=0;
					csyi=0;tcjs=0;
				}
			}
		}
	}
}
//串口1中断
void uart1()interrupt 4
{
	rp[eep]=SBUF;
	RI=0;
	eep++;
	if(rp[0]!=ipdz)eep=0;
}
//定时期器1
void time1_isr() interrupt 3		 //50ms
{
	static	unsigned int cnst1=0;  
	if(fig_out_k1)				 //k1 输出延时
	{
		cnst1++;
		if(cnst1>=Do_timer_S)
		{
			k1=1; cnst1=0;
		}		  
	}else   
	{
		cnst1++;
		if(cnst1>=Do_timer_S)
		{
			k1=0; cnst1=0;
		}
	} 	
}
