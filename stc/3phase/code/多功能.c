/***********************************
*包括4个按键,LED显示
*3路电压,3路电流输入
*4路开关量输入 4路开关量输出
*创建:xielungui
*日期:2018-04-02 09:28
************************************/
#include <STC8F.h>
#include <24c02.h>
#include <12864.h>
#include <ht7038.h>
#include <canshu.h>
#include <stdio.h>
#include <math.h>
#include <STCPOE.h>
#include <adc.h>

void com_txsj(uint zjzs)
{
	uint i;	
	uchar var;
	EA=0;
	for(i=0;i<zjzs;i++)
	{
		switch(POEFlag)
						{
							case 0:   SBUF=tp[i];
												break;
							case 1:	  
												var=tp[i];
												TB8=ParityCheckRecieve(var,POEFlag);
							          //TB8=parity;
												SBUF=var;
												break;
							case 2:		var=tp[i];
												TB8=ParityCheckRecieve(var,POEFlag);
												SBUF=var;							
												break;	
							default:
												SBUF=tp[i];
							          break;			
						}
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

//增加功能15处理
void yc1_shuju(void)
{
	uint k;
	tp[0]=ipdz;
	tp[1]=txgn;
	if(txgn==15)
	{
  
			if((jcqa==0x00)&&(zjcd==0x04))
								{
							     switch(pvale)
													{
															case 0: 
																      k1=0;
															        k2=0;
															        k3=0;
															        k4=0;
																			break;
															case 1: 
																      k1=1;
															        k2=0;
															        k3=0;
															        k4=0;
																			break;
															case 2: 
																      k2=1;
															        k1=0;
															        k3=0;
															        k4=0;															
																			break;	
															case 3: 
																      k1=1;
															        k2=1;
															        k3=0;
															        k4=0;															
																			break;	
															case 4: 
																      k1=0;
															        k2=0;
															        k3=1;
															        k4=0;															
																			break;		
															case 5: 
																      k1=1;
															        k2=0;
															        k3=1;
															        k4=0;															
																			break;	
															case 6: 
																      k1=0;
															        k2=1;
															        k3=1;
															        k4=0;															
																			break;
															case 7: 
																      k1=1;
															        k2=1;
															        k3=1;
															        k4=0;															
																			break;
															case 8: 
																      k1=0;
															        k2=0;
															        k3=0;
															        k4=1;															
																			break;
															case 9: 
																      k1=1;
															        k2=0;
															        k3=0;
															        k4=1;															
																			break;	
															case 10: 
																      k1=0;
															        k2=1;
															        k3=0;
															        k4=1;															
																			break;	
															case 11: 
																      k1=1;
															        k2=1;
															        k3=0;
															        k4=1;															
																			break;
															case 12: 
																      k1=0;
															        k2=0;
															        k3=1;
															        k4=1;															
																			break;	
															case 13: 
																      k1=1;
															        k2=0;
															        k3=1;
															        k4=1;															
																			break;
															case 14: 
																      k1=0;
															        k2=1;
															        k3=1;
															        k4=1;															
																			break;
															case 15: 
																      k1=1;
															        k2=1;
															        k3=1;
															        k4=1;															
																			break;															
										 
													}
								}			 
				
     k=6;
     tp[2]=0;
     tp[3]=jcqa;
     tp[4]=0;
		 tp[5]=zjcd;				
	}
	else
	{
		k=2;
		tp[1]=0x8F;
		tp[2]=2;	
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
				abc=read_ht7038(0x0d);	//读电压A
				if(abc>0x7fffff)abc=0;
				ax_av=abc/819.2;
				if(ax_av<100)ax_av=0;
				if(us_ys)ax_av=(float)ax_av/2.2;
				acc=ax_av*dybb;
				Float_to_IEEE754((float)acc/10);
				yb[114]=IEEE[0];
				yb[115]=IEEE[1];
				yb[116]=IEEE[2];
				yb[117]=IEEE[3];
				abc=read_ht7038(0x0e);	//读电压B
				if(abc>0x7fffff)abc=0;
				bx_av=abc/819.2;
				if(bx_av<100)bx_av=0;
				if(us_ys)bx_av=(float)bx_av/2.2;
				acc=bx_av*dybb;
				Float_to_IEEE754((float)acc/10);
				yb[118]=IEEE[0];
				yb[119]=IEEE[1];
				yb[120]=IEEE[2];
				yb[121]=IEEE[3];
				abc=read_ht7038(0x0f);	//读电压C
				if(abc>0x7fffff)abc=0;
				cx_av=abc/819.2;
				if(cx_av<100)cx_av=0;
				if(us_ys)cx_av=(float)cx_av/2.2;
				acc=cx_av*dybb;
				Float_to_IEEE754((float)acc/10);
				yb[122]=IEEE[0];
				yb[123]=IEEE[1];
				yb[124]=IEEE[2];
				yb[125]=IEEE[3];

				acc=(ax_av*dybb+bx_av*dybb)*0.8636;
				Float_to_IEEE754((float)acc/10);
				yb[126]=IEEE[0];
				yb[127]=IEEE[1];
				yb[128]=IEEE[2];
				yb[129]=IEEE[3];

				acc=(bx_av*dybb+cx_av*dybb)*0.8636;
				Float_to_IEEE754((float)acc/10);
				yb[130]=IEEE[0];
				yb[131]=IEEE[1];
				yb[132]=IEEE[2];
				yb[133]=IEEE[3];

				acc=(cx_av*dybb+ax_av*dybb)*0.8636;
				Float_to_IEEE754((float)acc/10);
				yb[134]=IEEE[0];
				yb[135]=IEEE[1];
				yb[136]=IEEE[2];
				yb[137]=IEEE[3];

				abc=read_ht7038(0x1c);	//读频率
				if(abc>0x7fffff)abc=0;
				plhz=abc/81.92+plbc-100;
				if(ax_av==0&bx_av==0&cx_av==0)plhz=0;
				Float_to_IEEE754((float)plhz/100);
				yb[214]=IEEE[0];
				yb[215]=IEEE[1];
				yb[216]=IEEE[2];
				yb[217]=IEEE[3];
			}break;
			case 1:
			{
				abc=read_ht7038(0x10);	//读电流A
				if(abc>0x7fffff)abc=0;
				ax_aa=abc/81.92;
				if(ax_aa<8)ax_aa=0;
				if(is_yw)ax_aa=(float)ax_aa/5;
				acc=ax_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[138]=IEEE[0];
				yb[139]=IEEE[1];
				yb[140]=IEEE[2];
				yb[141]=IEEE[3];
				abc=read_ht7038(0x11);	//读电流B
				if(abc>0x7fffff)abc=0;
				bx_aa=abc/81.92;
				if(bx_aa<8)bx_aa=0;
				if(is_yw)bx_aa=(float)bx_aa/5;
				acc=bx_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[142]=IEEE[0];
				yb[143]=IEEE[1];
				yb[144]=IEEE[2];
				yb[145]=IEEE[3];
				abc=read_ht7038(0x12);	//读电流C
				if(abc>0x7fffff)abc=0;
				cx_aa=abc/81.92;
				if(cx_aa<8)cx_aa=0;
				if(is_yw)cx_aa=(float)cx_aa/5;
				acc=cx_aa*dlbb;
				Float_to_IEEE754((float)acc/1000);
				yb[146]=IEEE[0];
				yb[147]=IEEE[1];
				yb[148]=IEEE[2];
				yb[149]=IEEE[3];
			}break;
			case 2:
			{
				abc=read_ht7038(0x01);	//读有功功率A
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					yg_pa=1;
				}else{acc=abc;yg_pa=0;}
				yggla=acc>>7;
				if(us_ys)yggla=(float)yggla/2.2;
				if(is_yw)yggla=(float)yggla/5;
				acc=yggla*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[150]=IEEE[0];
				if(yg_pa)yb[150]=yb[150]+0x80;
				yb[151]=IEEE[1];
				yb[152]=IEEE[2];
				yb[153]=IEEE[3];
				abc=read_ht7038(0x02);	//读有功功率B
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					yg_pb=1;
				}else{acc=abc;yg_pb=0;}
				ygglb=acc>>7;
				if(us_ys)ygglb=(float)ygglb/2.2;
				if(is_yw)ygglb=(float)ygglb/5;
				acc=ygglb*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[154]=IEEE[0];
				if(yg_pb)yb[154]=yb[154]+0x80;
				yb[155]=IEEE[1];
				yb[156]=IEEE[2];
				yb[157]=IEEE[3];
				abc=read_ht7038(0x03);	//读有功功率C
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					yg_pc=1;
				}else{acc=abc;yg_pc=0;}
				ygglc=acc>>7;
				if(us_ys)ygglc=(float)ygglc/2.2;
				if(is_yw)ygglc=(float)ygglc/5;
				acc=ygglc*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[158]=IEEE[0];
				if(yg_pc)yb[158]=yb[158]+0x80;
				yb[159]=IEEE[1];
				yb[160]=IEEE[2];
				yb[161]=IEEE[3];
				abc=read_ht7038(0x04);	//总有功功率
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					yg_pt=1;
				}else{acc=abc;yg_pt=0;}
				ygglt=acc>>6;
				if(us_ys)ygglt=(float)ygglt/2.2;
				if(is_yw)ygglt=(float)ygglt/5;
				if(ygglt==0)yg_pt=0;
				acc=ygglt*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[162]=IEEE[0];
				if(yg_pt)yb[162]=yb[162]+0x80;
				yb[163]=IEEE[1];
				yb[164]=IEEE[2];
				yb[165]=IEEE[3];
			}break;
			case 3:
			{
				abc=read_ht7038(0x05);	//读无功功率A
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					wg_qa=1;
				}else{acc=abc;wg_qa=0;}
				wggla=acc>>7;
				if(us_ys)wggla=(float)wggla/2.2;
				if(is_yw)wggla=(float)wggla/5;
				acc=wggla*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[166]=IEEE[0];
				if(wg_qa)yb[166]=yb[166]+0x80;
				yb[167]=IEEE[1];
				yb[168]=IEEE[2];
				yb[169]=IEEE[3];
				abc=read_ht7038(0x06);	//读无功功率B
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					wg_qb=1;
				}else{acc=abc;wg_qb=0;}
				wgglb=acc>>7;
				if(us_ys)wgglb=(float)wgglb/2.2;
				if(is_yw)wgglb=(float)wgglb/5;
				acc=wgglb*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[170]=IEEE[0];
				if(wg_qb)yb[170]=yb[170]+0x80;
				yb[171]=IEEE[1];
				yb[172]=IEEE[2];
				yb[173]=IEEE[3];
				abc=read_ht7038(0x07);	//读无功功率C
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					wg_qc=1;
				}else{acc=abc;wg_qc=0;}
				wgglc=acc>>7;
				if(us_ys)wgglc=(float)wgglc/2.2;
				if(is_yw)wgglc=(float)wgglc/5;
				acc=wgglc*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[174]=IEEE[0];
				if(wg_qc)yb[174]=yb[174]+0x80;
				yb[175]=IEEE[1];
				yb[176]=IEEE[2];
				yb[177]=IEEE[3];
				abc=read_ht7038(0x08);	//总无功功率
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					wg_qt=1;
				}else{acc=abc;wg_qt=0;}
				wgglt=acc>>6;
				if(us_ys)wgglt=(float)wgglt/2.2;
				if(is_yw)wgglt=(float)wgglt/5;
				if(wgglt==0)wg_qt=0;
				acc=wgglt*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[178]=IEEE[0];
				if(wg_qt)yb[178]=yb[178]+0x80;
				yb[179]=IEEE[1];
				yb[180]=IEEE[2];
				yb[181]=IEEE[3];
			}break;
			case 4:
			{
				abc=read_ht7038(0x09);	//读视在功率A
				szgla=abc>>7;
				if(us_ys)szgla=(float)szgla/2.2;
				if(is_yw)szgla=(float)szgla/5;
				acc=szgla*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[182]=IEEE[0];
				yb[183]=IEEE[1];
				yb[184]=IEEE[2];
				yb[185]=IEEE[3];
				abc=read_ht7038(0x0a);	//读视在功率B
				szglb=abc>>7;
				if(us_ys)szglb=(float)szglb/2.2;
				if(is_yw)szglb=(float)szglb/5;
				acc=szglb*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[186]=IEEE[0];
				yb[187]=IEEE[1];
				yb[188]=IEEE[2];
				yb[189]=IEEE[3];
				abc=read_ht7038(0x0b);	//读视在功率C
				szglc=abc>>7;
				if(us_ys)szglc=(float)szglc/2.2;
				if(is_yw)szglc=(float)szglc/5;
				acc=szglc*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[190]=IEEE[0];
				yb[191]=IEEE[1];
				yb[192]=IEEE[2];
				yb[193]=IEEE[3];
				abc=read_ht7038(0x0c);	//总视在功率
				szglt=abc>>6;
				if(us_ys)szglt=(float)szglt/2.2;
				if(is_yw)szglt=(float)szglt/5;
				acc=szglt*dlbb*dybb;
				Float_to_IEEE754((float)acc/1000);
				yb[194]=IEEE[0];
				yb[195]=IEEE[1];
				yb[196]=IEEE[2];
				yb[197]=IEEE[3];
			}break;
			case 5:
			{
				abc=read_ht7038(0x14);	//功率因数A
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					gl_ya=1;
				}else {acc=abc;gl_ya=0;}
				glysa=acc/838.86;
				Float_to_IEEE754((float)glysa/10000);
				yb[198]=IEEE[0];
				if(gl_ya)yb[198]=yb[198]+0x80;
				yb[199]=IEEE[1];
				yb[200]=IEEE[2];
				yb[201]=IEEE[3];
				abc=read_ht7038(0x15);	//功率因数B
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					gl_yb=1;
				}else {acc=abc;gl_yb=0;}
				glysb=acc/838.86;
				Float_to_IEEE754((float)glysb/10000);
				yb[202]=IEEE[0];
				if(gl_yb)yb[202]=yb[202]+0x80;
				yb[203]=IEEE[1];
				yb[204]=IEEE[2];
				yb[205]=IEEE[3];
				abc=read_ht7038(0x16);	//功率因数C
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					gl_yc=1;
				}else {acc=abc;gl_yc=0;}
				glysc=acc/838.86;
				Float_to_IEEE754((float)glysc/10000);
				yb[206]=IEEE[0];
				if(gl_yc)yb[206]=yb[206]+0x80;
				yb[207]=IEEE[1];
				yb[208]=IEEE[2];
				yb[209]=IEEE[3];
				abc=read_ht7038(0x17);	//总功率因数
				if(abc>0x7fffff)
				{
					acc=0xffffff-abc;
					gl_yt=1;
				}else {acc=abc;gl_yt=0;}
				glyst=acc/838.86;
				Float_to_IEEE754((float)glyst/10000);
				yb[210]=IEEE[0];
				if(gl_yt)yb[210]=yb[210]+0x80;
				yb[211]=IEEE[1];
				yb[212]=IEEE[2];
				yb[213]=IEEE[3];
			}break;
			default:break;
		}
	}
}

void bc_data(void)
{
	uchar i;
	uint date;
	float datbz,datcz;
	EA=0;
	if(ax_av>1600&ax_av<2800)
	{
		datbz=(float)2200/ax_av;
		if(datbz>=1)date=(float)0x7fff*(datbz-1);
		else date=0xffff-(float)0x7fff*(1-datbz);
		bcxz_data(0x17,date);
		ep[0]=(date>>8)&0xff;ep[1]=date&0xff;
	}
	if(bx_av>1600&bx_av<2800)
	{
		datbz=(float)2200/bx_av;
		if(datbz>=1)date=(float)0x7fff*(datbz-1);
		else date=0xffff-(float)0x7fff*(1-datbz);
		bcxz_data(0x18,date);
		ep[2]=(date>>8)&0xff;ep[3]=date&0xff;
	}
	if(cx_av>1600&cx_av<2800)
	{
		datbz=(float)2200/cx_av;
		if(datbz>=1)date=(float)0x7fff*(datbz-1);
		else date=0xffff-(float)0x7fff*(1-datbz);
		bcxz_data(0x19,date);
		ep[4]=(date>>8)&0xff;ep[5]=date&0xff;
	}
	if(ax_aa>4000&ax_aa<6000)
	{
		datbz=(float)5000/ax_aa;
		if(datbz>=1)date=(float)0x7fff*(datbz-1);
		else date=0xffff-(float)0x7fff*(1-datbz);
		bcxz_data(0x1a,date);
		ep[6]=(date>>8)&0xff;ep[7]=date&0xff;
	}
	if(bx_aa>4000&bx_aa<6000)
	{
		datbz=(float)5000/bx_aa;
		if(datbz>=1)date=0x7fff*(float)(datbz-1);
		else date=0xffffff-0x7fff*(float)(1-datbz);
		bcxz_data(0x1b,date);
		ep[8]=(date>>8)&0xff;ep[9]=date&0xff;
	}
	if(cx_aa>4000&cx_aa<6000)
	{
		datbz=(float)5000/cx_aa;
		if(datbz>=1)date=0x7fff*(float)(datbz-1);
		else date=0xffff-0x7fff*(float)(1-datbz);
		bcxz_data(0x1c,date);
		ep[10]=(date>>8)&0xff;ep[11]=date&0xff;
	}
	if(yggla>600&yggla<1700)
	{
		if(yggla>=1100)
		{
			datbz=(float)(yggla-1100)/1100;
			datcz=(float)(datbz)/(1+datbz);
			date=0xffff-(float)0x7fff*datcz;
		}
		else 
		{
			datbz=(float)yggla/1100;
			datcz=(float)(1-datbz)/datbz;
			date=(float)0x7fff*datcz;
		}
		bcxz_data(0x04,date);
		bcxz_data(0x07,date);
		bcxz_data(0x0a,date);
		ep[12]=(date>>8)&0xff;ep[13]=date&0xff;
	}
	if(ygglb>600&ygglb<1700)
	{
		if(ygglb>=1100)
		{
			datbz=(float)(ygglb-1100)/1100;
			datcz=(float)(datbz)/(1+datbz);
			date=0xffff-(float)0x7fff*datcz;
		}
		else 
		{
			datbz=(float)ygglb/1100;
			datcz=(float)(1-datbz)/datbz;
			date=(float)0x7fff*datcz;
		}
		bcxz_data(0x05,date);
		bcxz_data(0x08,date);
		bcxz_data(0x0b,date);
		ep[14]=(date>>8)&0xff;ep[15]=date&0xff;
	}
	if(ygglc>600&ygglc<1700)
	{
		if(ygglc>=1100)
		{
			datbz=(float)(ygglc-1100)/1100;
			datcz=(float)(datbz)/(1+datbz);
			date=0xffff-(float)0x7fff*datcz;
		}
		else 
		{
			datbz=(float)ygglc/1100;
			datcz=(float)(1-datbz)/datbz;
			date=(float)0x7fff*datcz;
		}
		bcxz_data(0x06,date);
		bcxz_data(0x09,date);
		bcxz_data(0x0c,date);
		ep[16]=(date>>8)&0xff;ep[17]=date&0xff;
	}

	for(i=0;i<18;i++){write_add(7,i,ep[i]);delay1(100);}
	EA=1;
	if(plhz>5000)plbc=100-(plhz-5000);
	else plbc=(5000-plhz)+100;
	write_add(0,9,plbc);
	delay1(100);
}

void xjmz_data(void)
{
	uchar i;
	float datbz;
	EA=0;
	if(yggla>300&yggla<800)
	{
		if(yggla>550)
		{
			datbz=(float)(yggla-550)/550;
			shuju=0xffff-(float)0x7fff*(datbz/1.732);
		}
		else 
		{
			datbz=(float)(550-yggla)/550;
			shuju=(float)0x7fff*(datbz/1.732);
		}
		bcxz_data(0x0d,shuju);
		bcxz_data(0x10,shuju);
		ep[18]=(shuju>>8)&0xff;ep[19]=shuju&0xff;
	}
	if(ygglb>300&ygglb<800)
	{
		if(ygglb>550)
		{
			datbz=(float)(ygglb-550)/550;
			shuju=0xffff-(float)0x7fff*(datbz/1.732);
		}
		else 
		{
			datbz=(float)(550-ygglb)/550;
			shuju=(float)0x7fff*(datbz/1.732);
		}
		bcxz_data(0x0e,shuju);
		bcxz_data(0x11,shuju);
		ep[20]=(shuju>>8)&0xff;ep[21]=shuju&0xff;
	}
	if(ygglc>300&ygglc<800)
	{
		if(ygglc>550)
		{
			datbz=(float)(ygglc-550)/550;
			shuju=0xffff-(float)0x7fff*(datbz/1.732);
		}
		else 
		{
			datbz=(float)(550-ygglc)/550;
			shuju=(float)0x7fff*(datbz/1.732);
		}
		bcxz_data(0x0f,shuju);
		bcxz_data(0x11,shuju);
		ep[22]=(shuju>>8)&0xff;ep[23]=shuju&0xff;
	}
	for(i=18;i<24;i++){write_add(7,i,ep[i]);delay1(100);}
	EA=1;
}

void xjbs_data(void)
{
	uchar i;
	float datbz;
	EA=0;
	if(yggla>55)
	{
		datbz=(float)(yggla-55)/55;
		shuju=0xffff-(float)0x7fff*(datbz/1.732);
	}
	else 
	{
		datbz=(float)(55-yggla)/55;
		shuju=(float)0x7fff*(datbz/1.732);
	}
	bcxz_data(0x61,shuju);
	ep[24]=(shuju>>8)&0xff;ep[25]=shuju&0xff;
	if(ygglb>55)
	{
		datbz=(float)(ygglb-55)/55;
		shuju=0xffff-(float)0x7fff*(datbz/1.732);
	}
	else 
	{
		datbz=(float)(55-ygglb)/55;
		shuju=(float)0x7fff*(datbz/1.732);
	}
	bcxz_data(0x62,shuju);
	ep[26]=(shuju>>8)&0xff;ep[27]=shuju&0xff;
	if(ygglc>55)
	{
		datbz=(float)(ygglc-55)/55;
		shuju=0xffff-(float)0x7fff*(datbz/1.732);
	}
	else 
	{
		datbz=(float)(55-ygglc)/55;
		shuju=(float)0x7fff*(datbz/1.732);
	}
	bcxz_data(0x63,shuju);
	ep[28]=(shuju>>8)&0xff;ep[29]=shuju&0xff;
	for(i=24;i<30;i++){write_add(7,i,ep[i]);delay1(100);}
	EA=1;
}
//读取合相电能
void read_dnsj(void)
{
	uint nssj,ldly;
	if(us_ys==0&is_yw==0)ldly=32;
	else 
	{
		if(us_ys&is_yw)ldly=352;
		else if(us_ys)ldly=70;
		else if(is_yw)ldly=160;
	}
	abc=read_ht7038(0x00);
	if((abc==0x7126a0)||(abc==0x7122A0))
	{	
		gnfx=read_ht7038(0x3d);		//读方向
		nssj=read_ht7038(0x21);		//有功电能
		lsygdn=lsygdn+nssj;
		if(lsygdn>=ldly)
		{
			ygdna=ygdna+(long)dybb*dlbb;
			if(ygdna>99)
			{
				ygdnb=ygdnb+(ygdna/100);
				ygdna=ygdna%100;
				if(ygdna>99)ygdna=0;
				write_add(1,0x01,ygdnb>>24);delay1(100);
				write_add(1,0x02,ygdnb>>16&0xff);delay1(100);
				write_add(1,0x03,ygdnb>>8&0xff);delay1(100);
				write_add(1,0x04,ygdnb&0xff);delay1(100);
				Float_to_IEEE754(ygdnb);
				yb[258]=IEEE[0];
				yb[259]=IEEE[1];
				yb[260]=IEEE[2];
				yb[261]=IEEE[3];
				Float_to_IEEE754((float)ygdnb/dybb/dlbb);
				yb[274]=IEEE[0];
				yb[275]=IEEE[1];
				yb[276]=IEEE[2];
				yb[277]=IEEE[3];
			}
			write_add(1,0x00,ygdna);delay1(100);
			lsygdn=lsygdn-ldly;
		}else delay1(250);
		nssj=read_ht7038(0x25);//反向有功电能
		lswgdn=lswgdn+nssj;
		if(lswgdn>=ldly)
		{
			wgdna=wgdna+(long)dybb*dlbb;
			write_add(1,0x05,wgdna>>24);delay1(100);
			write_add(1,0x06,wgdna>>16&0xff);delay1(100);
			write_add(1,0x07,wgdna>>8&0xff);delay1(100);
			write_add(1,0x08,wgdna&0xff);delay1(100);
			lswgdn=lswgdn-ldly;
			Float_to_IEEE754((float)wgdna/100);
			yb[266]=IEEE[0];
			yb[267]=IEEE[1];
			yb[268]=IEEE[2];
			yb[269]=IEEE[3];
			Float_to_IEEE754((float)wgdna/100/dybb/dlbb);
			yb[282]=IEEE[0];
			yb[283]=IEEE[1];
			yb[284]=IEEE[2];
			yb[285]=IEEE[3];
		}else delay1(250);
	}
	else
	{
		delay1(250);
		delay1(250);
		delay1(250);
	}
}
/*
void slsr_io()
{
	uchar i;
	if(dia==0)
	{
		delay1(5);
		if(dia==0)k1=1;
	}else k1=0;
	if(dib==0)
	{
		delay1(5);
		if(dib==0)k2=1;
	}else k2=0;
	if(dic==0)
	{
		delay1(5);
		if(dic==0)k3=1;
	}else k3=0;
	if(did==0)
	{
		delay1(5);
		if(did==0)k4=1;
	}else k4=0;
	yb[110]=0x04;
	i=P2&0x0f;
	yb[111]=0x0f-i;
	yb[112]=0x04;
	i=P2&0xf0;
	yb[113]=i>>4;
}*/
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
		case 1:	 // 过压模式
		{
			if((ax_av*dybb/10>kout_shu_V1)||(bx_av*dybb/10>kout_shu_V1)||(cx_av*dybb/10>kout_shu_V1))
			fig_out_k1=1;
			else fig_out_k1=0;				
		}
		case 2:	 //过载报警输出
		{
			if((ax_aa*dlbb/1000>kout_shu_A1)||(bx_aa*dlbb/1000>kout_shu_A1)||(cx_aa*dlbb/1000>kout_shu_A1)) 
			 	 fig_out_k1=1;
			else fig_out_k1=0;			
		}break;
		default:break;//为3时远程遥控
	 }
	yb[110]=0x04;
	i=P2&0x0f;
	yb[111]=0x0f-i;
	yb[112]=0x04;
	i=P2&0xf0;
	yb[113]=i>>4;
}
/*********K2输出******/
void k2_out()
{
	 uchar i;
	 switch(Do2_moshi)
	 {
	 	case 0:	  //开关量模式
		{
			if(dib==0)
			fig_out_k2=1;				
			else fig_out_k2=0;
		}break;
		case 1:	 // 过压模式
		{
			if((ax_av*dybb/10>kout_shu_V2)||(bx_av*dybb/10>kout_shu_V2)||(cx_av*dybb/10>kout_shu_V2))
			fig_out_k2=1;
			else fig_out_k2=0;				
		}
		case 2:	 //过载报警输出
		{
			if((ax_aa*dlbb/1000>kout_shu_A2)||(bx_aa*dlbb/1000>kout_shu_A2)||(cx_aa*dlbb/1000>kout_shu_A2)) 
			   fig_out_k2=1;
			else fig_out_k2=0;			
		}break;
		default:break;//为3时远程遥控
	 }
	yb[110]=0x04;
	i=P2&0x0f;
	yb[111]=0x0f-i;
	yb[112]=0x04;
	i=P2&0xf0;
	yb[113]=i>>4;
}
/*********K3输出******/
void k3_out()
{
	 uchar i;
	 switch(Do3_moshi)
	 {
	 	case 0:	  //开关量模式
		{
			if(dic==0)
			fig_out_k3=1;				
			else fig_out_k3=0;
		}break;
		case 1:	 // 过压模式
		{
			if((ax_av*dybb/10>kout_shu_V3)||(bx_av*dybb/10>kout_shu_V3)||(cx_av*dybb/10>kout_shu_V3))
			fig_out_k3=1;
			else fig_out_k3=0;				
		}
		case 2:	 //过载报警输出
		{
			if((ax_aa*dlbb/1000>kout_shu_A3)||(bx_aa*dlbb/1000>kout_shu_A3)||(cx_aa*dlbb/1000>kout_shu_A3))  
			   fig_out_k3=1;
			else fig_out_k3=0;			
		}break;
		default:break;//为3时远程遥控
	 }
	yb[110]=0x04;
	i=P2&0x0f;
	yb[111]=0x0f-i;
	yb[112]=0x04;
	i=P2&0xf0;
	yb[113]=i>>4;
}
/*********K4输出******/
void k4_out()
{
	 uchar i;
	 switch(Do4_moshi)
	 {
	 	case 0:	  //开关量模式
		{
			if(did==0)
			fig_out_k4=1;				
			else fig_out_k4=0;
		}break;
		case 1:	 // 过压模式
		{
			if((ax_av*dybb/10>kout_shu_V4)||(bx_av*dybb/10>kout_shu_V4)||(cx_av*dybb/10>kout_shu_V4))
			fig_out_k4=1;
			else fig_out_k4=0;				
		}
		case 2:	 //过载报警输出
		{
			if((ax_aa*dlbb/1000>kout_shu_A4)||(bx_aa*dlbb/1000>kout_shu_A4)||(cx_aa*dlbb/1000>kout_shu_A4))  
			   fig_out_k4=1;
			else fig_out_k4=0;			
		}break;
		default:break;	//为3时远程遥控
	 }
	yb[110]=0x04;
	i=P2&0x0f;
	yb[111]=0x0f-i;
	yb[112]=0x04;
	i=P2&0xf0;
	yb[113]=i>>4;
}
//主函数开始
main() 
{
	uchar i;
	adc_init();	
	init();			//初始化
	time1_init();
	 Timer3_Init();
	in_ht7038();	//初始化ht7038
	k4=0;
  
	while(1)
	{
//		WDT_CONTR=0x3c;	//看门狗
		while(zijian)
		{
			out_disp_zijian();
			cp[12]=0xFF;
			cp[13]=0xFF;
			cp[14]=0xFF;
			zijian_display(0,0);
			zijian_display(0,1);
			zijian_display(0,2);
			zijian_display(0,3);
		
			zijian_display(1,0);
			zijian_display(1,1);
			zijian_display(1,2);
			zijian_display(1,3);

			zijian_display(2,0);
			zijian_display(2,1);
			zijian_display(2,2);
			zijian_display(2,3);
		}
		if(sx_bz==0)
		{
			led_disp();	//LCD显示
			out_disp();
			sx_bz=1;
		}
		VolCurrCal();
		key_in();		//按键输入
		du_dldy();		//读电流电压
		read_dnsj();	//读 电能
		if(xb_bz&xbjs==5)
		{
			if(bc_tt==0)
			{
				bc_data();
				bc_tt=1;
			}
		}
		if(xj_mz&xbjs==5)
		{
			if(bc_tt==0)
			{
				xjmz_data();
				bc_tt=1;
			}
		}
		if(xj_bs&xbjs==5)
		{
			if(bc_tt==0)
			{
				xjbs_data();
				bc_tt=1;
			}
		}
		if(eep>=7)		//接收数据
		{					
				if(rp[1]==0x0F)
				{
					jcrc=Crc_date(1,8);
					ycrc=tchr_int(rp[9],rp[8]);
				}	
				else
				{
					jcrc=Crc_date(1,6);
					ycrc=tchr_int(rp[7],rp[6]);
				}	
			if(jcrc==ycrc)
			{
				if(ipdz==rp[0])
				{
					fig_tongxun=1;
					txgn=rp[1];
					jcqa=rp[3];
					zjcd=rp[5];							
					if(txgn==15)
					{
						pvale=rp[7];	
						yc1_shuju();	//发送数据
					}	
					else	
					{
							yc_shuju();	//发送数据
					}			
					if(txgn==0x50)
					{
						cs_tt=1;xb_bz=1;bc_tt=0;
						mima=0;sj_tt=0;
					}
					else
					if(txgn==0x51)
					{
						cs_tt=1;xj_mz=1;bc_tt=0;
						mima=0;sj_tt=0;
					}
					else
					if(txgn==0x52)
					{
						cs_tt=1;xj_bs=1;bc_tt=0;
						mima=0;sj_tt=0;
					}
					else
					if(txgn==0x55)
					{
						cs_tt=1;
						in_tt=1;
						for(i=0;i<10;i++)
						{
							write_add(0,i,sd[i]);
							delay1(150);
						}
						for(i=0;i<9;i++)
						{
							write_add(1,i,0x00);
							delay1(100);
							ygdna=0;
							ygdnb=0;
							wgdna=0;
						}
						for(i=0;i<30;i++)
						{
							write_add(7,i,0x00);
							delay1(100);
						}
						init();
						in_ht7038();
						mima=0;
						sj_tt=0;
					}
				}
			}			
			rx_tt=0;
			eep=0;
		}
//		slsr_io();
		k1_out();//k1输出
		k2_out();//k1输出
		k3_out();//k1输出
		k4_out();//k1输出	
	}
}

//定时器
void surt()interrupt 1
{
	static	unsigned char cnst=0;
	wsms++;		//50MS
	if(wsms>10)
	{
		wsms=0;
		cnst++;
		if(zijian!=0)
		zijian--;
		if(cnst>10)
		{
			cnst=0;
			fig_tongxun=0;
		}
		ss_tt=~ss_tt;
		wd_cj++;
		if(wd_cj>5)wd_cj=0;
		wd_tt=0;sx_bz=0;
		if(in_tt)
		{
			in_js++;
			if(in_js>4)in_js=0;
		}else in_js=0;
		if(ss_tt==0)
		{
			if(xb_bz|xj_mz|xj_bs|in_tt)
			{
				xbjs++;
				if(xbjs>6)
				{
					xbjs=0;in_tt=0;
					xb_bz=0;xj_mz=0;
					xj_bs=0;cs_tt=0;
				}
			}
			if(xsfs==0)
			{
				xsmz++;
				if(xsmz>9)
				{
					xsmz=0;
					xian++;
					if(xian>5)xian=0;
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
//定时期器1
void time1_isr() interrupt 3		 //50ms
{
	static	unsigned int cnst1=0;
	static	unsigned int cnst2=0;
	static	unsigned int cnst3=0;
	static	unsigned int cnst4=0;
	if(Do1_moshi!=3)
	{
		if(fig_out_k1)				 //k1 输出延时
		{
			cnst1++;
			if(cnst1>=Do_timer_S)
			{
				k1=1; cnst1=0;
			}		  
		}else  k1=0;
	}
	if(Do2_moshi!=3)
	{
			if(fig_out_k2)				 //k2 输出延时
			{	cnst2++;			   	 	
				if(cnst2>=Do_timer_S)
				{
					k2=1; cnst2=0; 
				}		
			}else	 k2=0;
	}
	if(Do3_moshi!=3)
	{
			if(fig_out_k3)				 //k3 输出延时
			{
				cnst3++;	 	
				if(cnst3>=Do_timer_S*2)
				{
						k3=1; cnst3=0;
				}		
			}else 	 k3=0;
	}
	if(Do4_moshi!=3)
	{
			if(fig_out_k4)				 //k4 输出延时
			{  	
				cnst4++;
				if(cnst4>=Do_timer_S*2)
				{
					k4=1; cnst4=0;
				}	 
			}else k4=0; 
	}	
}
//串口1中断
void uart1()interrupt 4
{
	rp[eep]=SBUF;
	RI=0;
	RB8=0;
	eep++;
	if(rp[0]!=ipdz)eep=0;
}
