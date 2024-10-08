
bit key_k;
sbit  k_set=P3^6;
sbit  k1=P7^0;
sbit  k2=P7^3;
sbit  k3=P2^4;
sbit  k4=P2^3;
sbit  k5=P2^2;
sbit  k6=P2^1;
sbit  k7=P2^0;
sbit  k8=P4^4;
sbit  k9=P4^3;
sbit  k10=P7^7;
sbit  k11=P7^6;
sbit  k12=P7^5;
sbit  k13=P7^4;
sbit 	k14=P2^7;
sbit  k15=P2^6;
sbit  k16=P2^5;
bit kset_fig,canshu_fig;
uchar jj,kk;//长按时间
uint jiajian_data(uint dat)
{
	if(yiwei==1)dat++;
	else if(yiwei==2)dat=dat+10;
	else if(yiwei==3)dat=dat+100;
	else if(yiwei==4)dat=dat+1000;
	return(dat);	
}
uchar dshij(uchar dat)
{
	dat++;
	if((dat&0x0f)>0x09)dat=(dat&0xf0)+0x10;
	if(dat>0x23)dat=0x00;
	return dat;
}

uchar dfensw(uchar dat)
{
	dat=dat+0x10;
	if((dat&0xf0)>=0x59)dat=dat&0x0f;
	return dat;
}

uchar dfengw(uchar dat)
{
	dat++;
	if((dat&0x0f)>0x09)dat=dat&0xf0;
	return dat;
}

void rl_shez(void)
{
	if(kgls==0x01)rl_la=~rl_la;
	else if(kgls==0x02)rl_lb=~rl_lb;
	else if(kgls==0x03)rl_lc=~rl_lc;
	else if(kgls==0x04)rl_ld=~rl_ld;
	else if(kgls==0x05)rl_le=~rl_le;
	else if(kgls==0x06)rl_lf=~rl_lf;
	else if(kgls==0x07)rl_lg=~rl_lg;
	else if(kgls==0x08)rl_lh=~rl_lh;
	else if(kgls==0x09)rl_ha=~rl_ha;
	else if(kgls==0x0a)rl_hb=~rl_hb;
	else if(kgls==0x0b)rl_hc=~rl_hc;
	else if(kgls==0x0c)rl_hd=~rl_hd;
	else if(kgls==0x0d)rl_he=~rl_he;
	else if(kgls==0x0e)rl_hf=~rl_hf;
	else if(kgls==0x0f)rl_hg=~rl_hg;
	else if(kgls==0x10)rl_hh=~rl_hh;
}

void rc_shez(void)
{
	if(kgls==0x01)rc_la=~rc_la;
	else if(kgls==0x02)rc_lb=~rc_lb;
	else if(kgls==0x03)rc_lc=~rc_lc;
	else if(kgls==0x04)rc_ld=~rc_ld;
	else if(kgls==0x05)rc_le=~rc_le;
	else if(kgls==0x06)rc_lf=~rc_lf;
	else if(kgls==0x07)rc_lg=~rc_lg;
	else if(kgls==0x08)rc_lh=~rc_lh;
	else if(kgls==0x09)rc_ha=~rc_ha;
	else if(kgls==0x0a)rc_hb=~rc_hb;
	else if(kgls==0x0b)rc_hc=~rc_hc;
	else if(kgls==0x0c)rc_hd=~rc_hd;
	else if(kgls==0x0d)rc_he=~rc_he;
	else if(kgls==0x0e)rc_hf=~rc_hf;
	else if(kgls==0x0f)rc_hg=~rc_hg;
	else if(kgls==0x10)rc_hh=~rc_hh;
}
void jiaz_date()
{
	kshia=dsf[(kgls-1)*16+ 0];kfena=dsf[(kgls-1)*16+ 1];
	gshia=dsf[(kgls-1)*16+ 2];gfena=dsf[(kgls-1)*16+ 3];
	kshib=dsf[(kgls-1)*16+ 4];kfenb=dsf[(kgls-1)*16+ 5];
	gshib=dsf[(kgls-1)*16+ 6];gfenb=dsf[(kgls-1)*16+ 7];
	kshic=dsf[(kgls-1)*16+ 8];kfenc=dsf[(kgls-1)*16+ 9];
	gshic=dsf[(kgls-1)*16+10];gfenc=dsf[(kgls-1)*16+11];
	kshid=dsf[(kgls-1)*16+12];kfend=dsf[(kgls-1)*16+13];
	gshid=dsf[(kgls-1)*16+14];gfend=dsf[(kgls-1)*16+15];
}
void fuz_date()
{
	dsf[(kgls-1)*16+ 0]=kshia;dsf[(kgls-1)*16+ 1]=kfena;
	dsf[(kgls-1)*16+ 2]=gshia;dsf[(kgls-1)*16+ 3]=gfena;
	dsf[(kgls-1)*16+ 4]=kshib;dsf[(kgls-1)*16+ 5]=kfenb;
	dsf[(kgls-1)*16+ 6]=gshib;dsf[(kgls-1)*16+ 7]=gfenb;
	dsf[(kgls-1)*16+ 8]=kshic;dsf[(kgls-1)*16+ 9]=kfenc;
	dsf[(kgls-1)*16+10]=gshic;dsf[(kgls-1)*16+11]=gfenc;
	dsf[(kgls-1)*16+12]=kshid;dsf[(kgls-1)*16+13]=kfend;
	dsf[(kgls-1)*16+14]=gshid;dsf[(kgls-1)*16+15]=gfend;
}
//SEC键处理程序
void sec_value(void)
{
	uint j;
	if(cs_tt)
	{
		if(yiwei!=0)
		{
			switch(cans)	
			{
				case 0x00:	//密码
				if(mima==7777)
				{
					for(j=0;j<14;j++)
					{
						write_add(7,j,sp[j]);
						delaya(50);
					}
					for(j=0;j<16;j++)
					{
						write_add(2,j,0x7f);
						delaya(50);
					}
					for(j=0;j<32;j++)
					{
						if(j&0x01)write_add(1,j,0x0f);
						else write_add(1,j,0x08);
						delaya(50);
					}
					WDT_CONTR=0x3c;		//看门狗
					for(j=0;j<256;j++)
					{
						write_add(0,j,0x00);
						delaya(50);
						if(j==128)WDT_CONTR=0x3c;//看门狗
					}
					init();
					dnyr_t=0;
					kzhi_com();
				}break;
				case 0x01:	//时钟
				{
//					I2CWriteDate(miao,fen,shi,zhou,ri,yue,nian);
				}break;
				case 0x02:
				{
					write_add(7,0,jwsq&0x07);
					delaya(50);
					write_add(7,1,jingd);
					delaya(50);
					write_add(7,2,jingf);
					delaya(50);
					write_add(7,3,weidd);
					delaya(50);
					write_add(7,4,weidf);
					delaya(50);
					write_add(7,5,shiqz);
					delaya(50);
					write_add(7,6,shiqf);
					delaya(50);
				}break;
				case 0x03:
				{
					write_add(7,7,ipdz>>8);
					delaya(50);
					write_add(7,8,ipdz&0xff);
					delaya(50);
					write_add(7,13,xhao);
					delaya(50);
					kzhi_com();
				}break;
				case 0x04:
				{
					fuz_date();
					for(j=0;j<256;j++)
					{
						write_add(0,j,dsf[j]);
						delaya(50);
					}
					write_add(7,9,rlka);
					delaya(50);
					write_add(7,10,rlkb);
					delaya(50);
					write_add(7,11,rcga);
					delaya(50);
					write_add(7,12,rcgb);
					delaya(50);
					bany_deng();
				}break;
				case 0x05:
				{
					jiar[kgls-1]=xqbz;
					for(j=0;j<16;j++)
					{
						write_add(2,j,jiar[j]);
						delaya(50);
					}
				}break;
				case 0x06:
				{
					for(j=0;j<32;j++)
					{
						write_add(1,j,gmkg[j]);
						delaya(50);
					}
				}break;
				case 0x07:
				{
						write_add(7,15,xf_fig);
						delaya(50);
						kzhi_com();
				}break;
				case 0x08:
				{
						write_add(7,16,botelv_fig);
						delaya(50);
						kzhi_com();
				}break;
				case 0x09:
				{
						write_add(7,17,dingshi_fig);
						delaya(50);						
				}break;
				default:break;
			}
		}
		yiwei=0;
	}
	else
	if(xian!=0)
	{
		if(xian==1)schc=~schc;
		else
		if(xian==2)schg=~schg;
		else
		if(xian==3)sclc=~sclc;
		else 
		if(xian==4)sclg=~sclg;
		kzhi_com();
	}
}
uchar gmsz_dat(uchar date)
{
	if((date&0x80))date=date&0x7f;
	else date=date+0x80;

	return date;
}

uchar gmkj_dat(uchar date)
{
	date++;
	if((date&0x7f)>40)date=date&0x80;
	return date;
}

uchar gmgj_dat(uchar date)
{
	date++;
	if(date>42)date=0x00;
	return date;
}
//UP键处理程序
void up_value(void)
{
	uchar i;
	if(cs_tt)
	{		
		if(yiwei==0)
		{
			if(mima==1)
			{
				cans++;
				if(cans>9)cans=0;
				if(cans==4)
				{
					kgls=1;
					shid=1;
					jiaz_date();//加载时间
				}
				else
				if(cans==5)
				{
					kgls=1;
					xqbz=jiar[kgls-1];
				}
			}
		}
		else
		{
			switch(cans)	
			{
				case 0x00:	//密码
				{
					mima=jiajian_data(mima);
					if(mima>9999)mima=0;
				}break;
				case 0x01:	//时钟
				{
					if(yiwei==1)
					{
						nian=nian+0x10;
						if((nian&0xf0)>0x99)nian=nian&0x0f;
					}
					else
					if(yiwei==2)
					{
					//	nian=nian++;
						nian++;
						if((nian&0x0f)>0x09)nian=nian&0xf0;
					}
					else
					if(yiwei==3)
					{
						yue++;
						if((yue&0x0f)>0x09)yue=(yue&0xf0)+0x10;
						if(yue>0x12)yue=0x01;
					}
					else
					if(yiwei==4)
					{
						ri++;
						if((ri&0x0f)>0x09)ri=(ri&0xf0)+0x10;
						if(yue==2)
						{
							i=((nian&0xf0)>>4)*10+(nian&0x0f);
							if((i%4)!=0){if(ri>0x28)ri=0x01;}
							else {if(ri>0x29)ri=0x01;}
						}
						else 
						if(yue==4|yue==6|yue==9|yue==11)
						{
							if(ri>0x30)ri=0x01;
						}
						else if(ri>0x31)ri=0x01;
					}
					else
					if(yiwei==5)
					{
						zhou++;
						if(zhou>0x06)zhou=0x00;
					}
					else
					if(yiwei==6)
					{
						shi++;
						if((shi&0x0f)>0x09)shi=(shi&0xf0)+0x10;
						if(shi>0x23)shi=0x00;
					}
					else
					if(yiwei==7)
					{
						fen=fen+0x10;
						if((fen&0xf0)>=0x59)fen=fen&0x0f;
					}
					else
					if(yiwei==8)
					{
						fen++;
						if((fen&0x0f)>0x09)fen=fen&0xf0;
					}
					else
					if(yiwei==9)
					{
						miao=miao+0x10;
						if((miao&0xf0)>=0x59)miao=miao&0x0f;
					}
					else
					if(yiwei==10)
					{
						miao++;
						if((miao&0x0f)>0x09)miao=miao&0xf0;
					}
				}break;
				case 0x02:	//经纬度设置
				{
					if(yiwei==1)jx_tt=~jx_tt;
					else
					if(yiwei==2)
					{
						if(jingd==180)jingd=0;
						else jingd=jingd+10;
						if(jingd>180)jingd=jingd-180;
						if(jingd==180)jingf=0;
					}
					else
					if(yiwei==3)
					{
						i=jingd%10;
						jingd=jingd-i;
						i++;
						if(i>0x09)i=0;
						jingd=jingd+i;
					}
					else
					if(yiwei==4)
					{
						if(jingd<180)
						{
							if(jingf==50)jingf=0;
							else jingf=jingf+10;
							if(jingf>59)jingf=jingf-60;
						}
					}
					else
					if(yiwei==5)
					{
						if(jingd<180)
						{
							i=jingf%10;
							jingf=jingf-i;
							i++;
							if(i>0x09)i=0;
							jingf=jingf+i;
						}
					}
					else
					if(yiwei==6)wx_tt=~wx_tt;
					else
					if(yiwei==7)
					{
						if(weidd==90)weidd=0;
						else weidd=weidd+10;
						if(weidd>90)weidd=weidd-90;
						if(weidd==90)weidf=0;
					}
					else
					if(yiwei==8)
					{
						i=weidd%10;
						weidd=weidd-i;
						i++;
						if(i>0x09)i=0;
						weidd=weidd+i;
					}
					else
					if(yiwei==9)
					{
						if(weidd<90)
						{
							if(weidf==50)weidf=0;
							else weidf=weidf+10;
							if(weidf>59)weidf=weidf-60;
						}
					}
					else
					if(yiwei==10)
					{
						if(weidd<90)
						{
							i=weidf%10;
							weidf=weidf-i;
							i++;
							if(i>0x09)i=0;
							weidf=weidf+i;
						}
					}
					else
					if(yiwei==11)sq_tt=~sq_tt;
					else
					if(yiwei==12)
					{
						shiqz++;
						if(shiqz>12)shiqz=0;
						if(shiqz==12)shiqf=0;
					}
					else
					if(yiwei==13)
					{
						if(shiqz<12)
						{
							if(shiqf==50)shiqf=0;
							else shiqf=shiqf+10;
							if(shiqf>59)shiqf=shiqf-60;
						}
					}
					else
					if(yiwei==14)
					{
						if(shiqz<12)
						{
							i=shiqf%10;
							shiqf=shiqf-i;
							i++;
							if(i>0x09)i=0;
							shiqf=shiqf+i;
						}
					}
				}break;
				case 0x03:	//通信设置
				{
					if(yiwei==1)
					{
						if(ipdz>=255)ipdz=1;
						else ipdz=ipdz+100;
						if(ipdz>255)ipdz=ipdz-255;
					}
					else 
					if(yiwei==2)
					{
						i=ipdz%100/10;
						ipdz=ipdz-(i*10);
						i++;
						if(i>9)i=0;
						ipdz=ipdz+(i*10);
					}
					else 
					if(yiwei==3)
					{
						i=ipdz%10;
						ipdz=ipdz-i;
						i++;
						if(i>9)i=0;
						ipdz=ipdz+i;
						if(ipdz==0)ipdz=1;
					}
					else 
					if(yiwei==4)
					{
						xhao++;
						if(xhao>4)xhao=1;
					}
				}break;
				case 0x04:	//时控设置
				{
					if(yiwei==1)
					{
						fuz_date();	//
						kgls++;
						if(kgls>(xhao*4))kgls=1;
						jiaz_date();//加载时间
					}
					else
					if(yiwei==2)
					{
						shid++;
						if(shid>4)shid=1;
					}
					else
					if(yiwei==3)
					{
						if(shid==1)kshia=dshij(kshia);
						else if(shid==2)kshib=dshij(kshib);
						else if(shid==3)kshic=dshij(kshic);
						else if(shid==4)kshid=dshij(kshid);
					}
					else
					if(yiwei==4)
					{
						if(shid==1)     kfena=dfensw(kfena);
						else if(shid==2)kfenb=dfensw(kfenb);
						else if(shid==3)kfenc=dfensw(kfenc);
						else if(shid==4)kfend=dfensw(kfend);
					}
					else
					if(yiwei==5)
					{
						if(shid==1)kfena=dfengw(kfena);
						else if(shid==2)kfenb=dfengw(kfenb);
						else if(shid==3)kfenc=dfengw(kfenc);
						else if(shid==4)kfend=dfengw(kfend);
					}
					else
					if(yiwei==6)
					{
						if(shid==1)rl_shez();
						else if(shid==2)gshib=dshij(gshib);
						else if(shid==3)gshic=dshij(gshic);
						else if(shid==4)gshid=dshij(gshid);
					}
					else
					if(yiwei==7)
					{
						if(shid==1)gshia=dshij(gshia);
						else if(shid==2)gfenb=dfensw(gfenb);
						else if(shid==3)gfenc=dfensw(gfenc);
						else if(shid==4)gfend=dfensw(gfend);
					}
					else
					if(yiwei==8)
					{
						if(shid==1)gfena=dfensw(gfena);
						else if(shid==2)gfenb=dfengw(gfenb);
						else if(shid==3)gfenc=dfengw(gfenc);
						else if(shid==4)gfend=dfengw(gfend);
					}
					else
					if(yiwei==9)
					{
						if(shid==1)gfena=dfengw(gfena);
					}
					else if(yiwei==10)rc_shez();
				}break;
				case 0x05:
				{
					if(yiwei==1)
					{
						jiar[kgls-1]=xqbz;
						kgls++;
						if(kgls>(xhao*4))kgls=1;
						xqbz=jiar[kgls-1];
					}
					else
					if(yiwei==2)xq_ta=~	xq_ta;
					else
					if(yiwei==3)xq_tb=~	xq_tb;
					else
					if(yiwei==4)xq_tc=~	xq_tc;
					else
					if(yiwei==5)xq_td=~	xq_td;
					else
					if(yiwei==6)xq_te=~	xq_te;
					else
					if(yiwei==7)xq_tf=~	xq_tf;
					else
					if(yiwei==8)xq_tg=~	xq_tg;
				}break;
				case 0x06:
				{
					if(yiwei==1)
					{
						kgls++;
						if(kgls>(xhao*4))kgls=1;
					}
					else
					if(yiwei==2)
					{
						gmkg[(kgls-1)*2]=gmsz_dat(gmkg[(kgls-1)*2]);
					}
					else
					if(yiwei==3)
					{
						gmkg[(kgls-1)*2]=gmkj_dat(gmkg[(kgls-1)*2]);
						if(gmkg[(kgls-1)*2+1]<((gmkg[(kgls-1)*2]*0X7F)+3))gmkg[(kgls-1)*2-1]=(gmkg[(kgls-1)*2]&0X7F)+3;
					}
					else
					if(yiwei==4)
					{
						gmkg[(kgls-1)*2+1]=gmgj_dat(gmkg[(kgls-1)*2+1]);
						if(gmkg[(kgls-1)*2+1]<((gmkg[(kgls-1)*2]&0X7F)+3))gmkg[(kgls-1)*2+1]=(gmkg[(kgls-1)*2]&0X7F)+3;
					}
				}break;
				case 0x07:	//消防
				{
					xf_fig=~xf_fig;
				}break;
				case 0x08:	//波特率
				{
					botelv_fig=~botelv_fig;
				}break;
				case 0x09:	//定时方式
				{
					dingshi_fig=~dingshi_fig;
				}break;
				default:break;
			}
		}
	}
	else	
	{
		scha=~scha;
		kzhi_com();
	}
}
//移位
void yw_value(void)
{
	if(cs_tt)
	{
		yiwei++;key_time=100;
		switch(cans)	
		{
			case 0x00:if(yiwei>4)yiwei=0;break;		//密码
			case 0x01:if(yiwei>10)yiwei=1;break;	//时钟
			case 0x02:if(yiwei>14)yiwei=1;break;	//经纬
			case 0x03:if(yiwei>4)yiwei=1;break;		//通信
			case 0x04:
			{
				if(shid==1)
				{
					if(yiwei>10)yiwei=1;
				}
				else 
				{
					if(yiwei>8)yiwei=1;
				}
			}break;
			case 0x05:if(yiwei>8)yiwei=1;break;		//
			case 0x06:if(yiwei>4)yiwei=1;break;		//
			case 0x07:if(yiwei>1)yiwei=0;break;	
			case 0x08:if(yiwei>1)yiwei=0;break;	
			case 0x09:if(yiwei>1)yiwei=0;break;	
			default:break;
		}
	}
	else
	{
		xian++;
		if(xian>xhao)xian=0;
	}
}
//键盘处理程序,入口K作为按键的值
void key_proa(void)
{
			if(k4==0) //k4 
			{
				if(key_k==0)
				{
					key_k=1;if(cs_tt==0)schd=~schd;	kzhi_com();
					led_beiguang=0;led=1;
				}
				else		//全开1-8
				{
					 kk++;
					if(kk>20)
					{
						kk=20;
						switch(xhao)
						{
							case 1: yzbl=0xF0;break;
							case 2: yzbl=0xff;break;
							default:break;
						}
						kzhi_com();
					}
				}
			}//k4 //清0自动关背光时间并打开背光
			if(k3==0)
			{
				if(key_k==0){key_k=1;if(cs_tt==0)schc=~schc;	kzhi_com();led_beiguang=0;led=1;}//k3键 //清0自动关背光时间并打开背光		
			}
			if(k16==0)
			{
				if(key_k==0)
				{
					key_k=1;
					if(cs_tt==0)
						if(xhao>3)
							sclh=~sclh;	
							kzhi_com();led_beiguang=0;led=1;
				}
				else		//全关
				{
						kk++;
						if(kk>=20)
						{
							kk=20; yzbl=0x00;jsll=0x00;	kzhi_com();													
						}
				}
			}//k16 //清0自动关背光时间并打开背光
			if(k15==0)
			{
				if(key_k==0){key_k=1;if(cs_tt==0)if(xhao>3)sclg=~sclg;	kzhi_com();led_beiguang=0;led=1;}//k15		//清0自动关背光时间并打开背光
			}
			if(k14==0) 
			{				
				if(key_k==0){key_k=1;if(cs_tt==0)if(xhao>3)sclf=~sclf; kzhi_com();led_beiguang=0;led=1;}//k14	//清0自动关背光时间并打开背光	
			}
			if(k13==0) //k13	
			{	
					if(key_k==0)
					{
							key_k=1;led_beiguang=0;	led=1;	//清0自动关背光时间并打开背光	
							if(cs_tt==0)
								{
									if(xhao>3) 
									scle=~scle;	
									kzhi_com();
								}
					}	
				}					
					if(k12==0)  //k12
					{
							if(key_k==0)
							{
								key_k=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
								if(cs_tt==0)
								{	
									if(xhao>2)
									scld=~scld;	
									kzhi_com();
								}
							}
					}
				
		if(k11==0)//k11
		{
				if(key_k==0)
				{
					key_k=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
					if(cs_tt==0)
					{	
						if(xhao>2)
						sclc=~sclc;	
						kzhi_com();
					}
				}
		}	
		if(k3&k4&k11&k12&k13&k14&k15&k16){key_k=0;kk=0;canshu_fig=0;}	
}

void key_kP2()//P2按键
{		
		if(k_set==0)//set
		{
				if(kset_fig==0)//短按移位
				{
					kset_fig=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
					yw_value();
				}
				else		//长按设置 
				{
					jj++;
					if(jj>=20)
					{
							jj=20;
							if(canshu_fig==0)
							{
								canshu_fig=1;
								cs_tt=~cs_tt;//设置
								if(cs_tt==0)
								{
									yiwei=0;
									mima=0;
									cans=0;
									key_time=100;//重置自动退出设置时间
								}
							}				
					}
				}
		}
		if(k1==0)//k1
		{
				if(kset_fig==0)//k1
				{
					kset_fig=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光		
						up_value(); key_time=100;//加  重置自动退出设置时间
				}
		}
		if(k2==0)//k2
		{
				if(kset_fig==0)//k2
				{
					kset_fig=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
					if(cs_tt) {sec_value();key_time=100;} //确定  重置自动退出设置时间
					else {schb=~schb;kzhi_com();}//k2
				}
		}
		if(k5==0)
		{
				if(kset_fig==0)//k5
				{
					kset_fig=1;led_beiguang=0;led=1;
					if(cs_tt==0)if(xhao>1){sche=~sche;kzhi_com();}//k5
				}
		}
		if(k6==0)
		{
				if(kset_fig==0)//k6
				{
					kset_fig=1;led_beiguang=0;led=1;
					if(cs_tt==0)if(xhao>1){schf=~schf;kzhi_com();}//k6
				}
		}
		if(k7==0)
		{
				if(kset_fig==0)//k7
				{
					kset_fig=1;led_beiguang=0;led=1;
					if(cs_tt==0)if(xhao>1){schg=~schg;kzhi_com();}//k7
				}
		}	
		if(k8==0)
		{
				if(kset_fig==0)//k8
				{
					kset_fig=1;led_beiguang=0;led=1;
					if(cs_tt==0)if(xhao>1){schh=~schh;kzhi_com();}//k8
				}
				else//长按全关1-8
				{
						jj++;
						if(jj>20)
						{
								jj=20;yzbl=0x00;kzhi_com(); 
						}
				}
		}
		if(k9==0)
		{
				if(kset_fig==0)//k9
				{
					kset_fig=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
					if(cs_tt==0)if(xhao>2){scla=~scla;kzhi_com();}//k9
				}
				else	//长按K9
				{
						jj++;
						if(jj>=20)
						{
							jj=20; yzbl=0xFF;
							switch (xhao) 
							{
									case 3: jsll=0xf0;break;
									case 4: jsll=0xFF;break;
									default: break;
							}
							kzhi_com(); 							
						}
				}
		}
		if(k10==0)
		{
				if(kset_fig==0)//k10
						{
							kset_fig=1;led_beiguang=0;led=1;//清0自动关背光时间并打开背光
							if(cs_tt==0)if(xhao>2){sclb=~sclb;kzhi_com();}//k10
						}
		}			
	if(k_set&k1&k2&k5&k6&k7&k8&k9&k10){kset_fig=0;jj=0;canshu_fig=0;}	
}

//按键
void key_in(void)
{
	/*key=P0;
	if(key!=0xff)
	{
		delaya(10);
		key=P0;
		if(key!=0xff)
		{
			xsqh=0;
			key_proa();
		}
	}
	else
	{
		delaya(10);
		key_k=0;kk=0;
	}*/
	key_proa();
}
