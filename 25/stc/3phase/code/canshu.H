sbit an1=P1^2;
sbit ss1=P0^3;//+
sbit ss2=P0^2;//-
sbit ss3=P0^1;//设置
sbit ss4=P0^0;//确定
bit key_k;

//baocu
void baoc_data()
{
	write_add(0,0,dybb>>8);
	delay1(100);
	write_add(0,1,dybb&0xff);
	delay1(100);
	write_add(0,2,dlbb>>8);
	delay1(100);
	write_add(0,3,dlbb&0xff);
	delay1(100);
	write_add(0,4,ipdz);
	delay1(100);
	write_add(0,5,xsfs);
	delay1(100);
	write_add(0,6,yhmm>>8);
	delay1(100);
	write_add(0,7,yhmm&0xff);
	delay1(100);
	write_add(0,8,shez&0xfb);
	delay1(100);
	write_add(0,10,Disp_liangdu);
 
	delay1(100);
	cs_tt=0;csyi=0;
	cs_xt=0;sj_tt=0;
	sj_bc=0;da_tt=0;
	mima=9999;cser=0;
	xgma=0;xgmb=0;xgmc=0;
}

//SEC键处理程序
void sec_value(void)
{
	uchar i;
	if(cs_tt)
	{
		if(sj_bc)baoc_data();
		else
		if(da_tt)
		{
			if(csyi==1)
			{
				if(cser==2)
				{
					for(i=0;i<9;i++)
					{
						write_add(1,i,0x00);
						delay1(100);
						ygdna=0;
						ygdnb=0;
						wgdna=0;
					}
					in_tt=1;		
				} else
				if(cser==4)
				{write_add(0,11,Do1_moshi);} else
				if(cser==5)
				{write_add(0,12,Do2_moshi);} else
				if(cser==6)
				{write_add(0,13,Do3_moshi);} else
				if(cser==7)
				{write_add(0,14,Do4_moshi);} else
				if(cser==8)
				{
					write_add(0,16,kout_shu_A1>>8);
					delay1(100);
					write_add(0,17,kout_shu_A1&0xff);	
				}else
				if(cser==9)
				{
					write_add(0,18,kout_shu_A2>>8);
					delay1(100);
					write_add(0,19,kout_shu_A2&0xff);	
				}else
				if(cser==10)
				{
					write_add(0,20,kout_shu_A3>>8);
					delay1(100);
					write_add(0,21,kout_shu_A3&0xff);	
				}else 				
				if(cser==11)
				{
					write_add(0,22,kout_shu_A4>>8);
					delay1(100);
					write_add(0,23,kout_shu_A4&0xff);	
				}
				if(cser==12)
				{
					write_add(0,24,kout_shu_V1>>8);
					delay1(100);
					write_add(0,25,kout_shu_V1&0xff);	
				}else
				if(cser==13)
				{
					write_add(0,26,kout_shu_V2>>8);
					delay1(100);
					write_add(0,27,kout_shu_V2&0xff);	
				}else
				if(cser==14)
				{
					write_add(0,28,kout_shu_V3>>8);
					delay1(100);
					write_add(0,29,kout_shu_V3&0xff);	
				}else 				
				if(cser==15)
				{
					write_add(0,30,kout_shu_V4>>8);
					delay1(100);
					write_add(0,31,kout_shu_V4&0xff);	
				}else
				if(cser==16)
				{write_add(0,15,Do_timer_S);} 	
				else
				if(cser==17)
				{
				   write_add(0,32,fig_pt);
				}										
				da_tt=0;
			}
			else
			if(csyi==4)
			{
				if(xgma==yhmm)
				{
					cser++;
					if(cser>3)cser=1;
					if(xgmc==xgmb){da_tt=0;yhmm=xgmc;}
				}else da_tt=0;
			}else da_tt=0;
		}
		else
		if(sj_tt)
		{
			da_tt=1;
		}
		else 
		if(cs_xt)
		{
			sj_tt=1;
			cser=1;
		}
		else
		{
			if(mima==6)
			{
				in_tt=1;
				for(i=0;i<9;i++)
				{
					write_add(0,i,sd[i]);
					delay1(100);
				}
				init();
				sj_tt=0;mima=0;
			}
			else
			{
				if(mima==yhmm){cs_xt=1;csyi=1;}
				else cs_tt=0;
			}
		}
	}
}

//减键处理程序
void jiandata(void)
{
	uchar das;
	if(cs_tt==0)
	{
		xian--;
		if(xian>10)xian=10;
	}
	else
	if(sj_tt==0&cs_xt==0)
	{
		if(csyi==0)
		{
			if(kk<10)
			{
				mima--;
				kk++;
			}else mima=mima-10;
			if(mima>9999)mima=9999;
		}
	}
	else
	if(sj_tt|da_tt)
	{
		switch(csyi)
		{
			case 1:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						xsfs--;
						if(xsfs>6)xsfs=6;
					}
					else if(cser==2)da_tt=0;					
					else if(cser==3)
					{
						Disp_liangdu--;
						if(Disp_liangdu>7)Disp_liangdu=7;
					}
					else if(cser==4)
					{
						Do1_moshi--;if(Do1_moshi>3)Do1_moshi=0;
					}
					else if(cser==5)
					{
						Do2_moshi--;if(Do2_moshi>3)Do2_moshi=0;
					}
					else if(cser==6)
					{
						Do3_moshi--;if(Do3_moshi>3)Do3_moshi=0;
					}
					else if(cser==7)
					{
						Do4_moshi--;if(Do4_moshi>3)Do4_moshi=0;
					}
					else if(cser==8)
					{
						if(kk<10)
						{
							kout_shu_A1--;
							kk++;
						}else kout_shu_A1=kout_shu_A1-10;
						if(kout_shu_A1==0|kout_shu_A1>5000)kout_shu_A1=5000;
					}
					else if(cser==9)
					{
						if(kk<10)
						{
							kout_shu_A2--;
							kk++;
						}else kout_shu_A2=kout_shu_A2-10;
						if(kout_shu_A2==0|kout_shu_A2>5000)kout_shu_A2=5000;
					}
					else if(cser==10)
					{
						if(kk<10)
						{
							kout_shu_A3--;
							kk++;
						}else kout_shu_A3=kout_shu_A3-10;
						if(kout_shu_A3==0|kout_shu_A3>5000)kout_shu_A3=5000;
					}
					else if(cser==11)
					{
						if(kk<10)
						{
							kout_shu_A4--;
							kk++;
						}else kout_shu_A4=kout_shu_A4-10;
						if(kout_shu_A4==0|kout_shu_A4>5000)kout_shu_A4=5000;
					}

					else if(cser==12)
					{
						if(kk<10)
						{
							kout_shu_V1--;
							kk++;
						}else kout_shu_V1=kout_shu_V1-10;
						if(kout_shu_V1==0|kout_shu_V1>5000)kout_shu_V1=5000;
					}
					else if(cser==13)
					{
						if(kk<10)
						{
							kout_shu_V2--;
							kk++;
						}else kout_shu_V2=kout_shu_V2-10;
						if(kout_shu_V2==0|kout_shu_V2>5000)kout_shu_V2=5000;
					}
					else if(cser==14)
					{
						if(kk<10)
						{
							kout_shu_V3--;
							kk++;
						}else kout_shu_V3=kout_shu_V3-10;
						if(kout_shu_V3==0|kout_shu_V3>5000)kout_shu_V3=5000;
					}
					else if(cser==15)
					{
						if(kk<10)
						{
							kout_shu_V4--;
							kk++;
						}else kout_shu_V4=kout_shu_V4-10;
						if(kout_shu_V4==0|kout_shu_V4>5000)kout_shu_V4=5000;
					}
					else if(cser==16)
					{
						if(kk<10)
						{
							Do_timer_S--;
							kk++;
						}else Do_timer_S=Do_timer_S-10;
						if(Do_timer_S==0|Do_timer_S>250)Do_timer_S=250;
					}
					else if(cser==17)
					{ 						
						if(fig_pt==1)fig_pt=0;
					}
				}
				else 
				if(sj_tt)
				{
					cser--;
					if(cser==0)cser=17;
				}
			}break;
			case 2:
			{
				if(da_tt)
				{
					if(cser==1)sx_sx=~sx_sx;
					else if(cser==2)us_ys=~us_ys;
					else if(cser==3)is_yw=~is_yw;
					else 
					if(cser==4)
					{
						if(kk<10)
						{
							dybb--;
							kk++;
						}else dybb=dybb-10;
						if(dybb==0|dybb>1000)dybb=1000;
					}
					else
					if(cser==5)
					{
						if(kk<10)
						{
							dlbb--;
							kk++;
						}else dlbb=dlbb-10;
						if(dlbb==0|dlbb>5000)dlbb=5000;
					}
				}
				else 
				if(sj_tt)
				{
					cser--;
					if(cser==0)cser=5;
				}
			}break;
			case 3:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						if(kk<10)
						{
							ipdz--;
							kk++;
						}else ipdz=ipdz-10;
						if(ipdz==0|ipdz>247)ipdz=247;
					}
					else if(cser==2)btl_t=~btl_t;
					else
					if(cser==3)
					{
						das=shez&0x03;
						das--;
						if(das>2)das=2;
						shez=(shez&0xfc)+das;
					}
				}
				else 
				if(sj_tt)
				{
					cser--;
					if(cser==0)cser=3;
				}
			}break;
			case 4:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						if(kk<10)
						{
							xgma--;
							kk++;
						}else xgma=xgma-10;
						if(xgma>9999)xgma=9999;
					}
					else 
					if(cser==2)
					{
						if(kk<10)
						{
							xgmb--;
							kk++;
						}else xgmb=xgmb-10;
						if(xgmb>9999)xgmb=9999;
					}
					else
					if(cser==3)
					{
						if(kk<10)
						{
							xgmc--;
							kk++;
						}else xgmc=xgmc-10;
						if(xgmc>9999)xgmc=9999;
					}
				}
			}break;
			default:break;
		}
	}
	else
	if(cs_xt)
	{
		if(mima==yhmm)
		{
			csyi--;
			if(csyi==0)csyi=4;
		}
	}
}
//加键处理程序
void jia_data(void)
{
	uchar das;
	if(cs_tt==0)
	{
		xian++;
		if(xian>10)xian=0;
	}
	else
	if(sj_tt==0&cs_xt==0)
	{
		if(csyi==0)
		{
			if(kk<10)
			{
				mima++;
				kk++;
			}else mima=mima+10;
			if(mima>9999)mima=0;
		}		
	}
	else
	if(sj_tt|da_tt)
	{
		switch(csyi)
		{
			case 1:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						xsfs++;
						if(xsfs>6)xsfs=0;
					}
					else if(cser==2)da_tt=0;					
					else if(cser==3)
					{
						Disp_liangdu++;
						if(Disp_liangdu>7)
						   Disp_liangdu=0;
					}
					else if(cser==4)
					{
						Do1_moshi++;if(Do1_moshi>3)Do1_moshi=0;
					}
					else if(cser==5)
					{
						Do2_moshi++;if(Do2_moshi>3)Do2_moshi=0;
					}
					else if(cser==6)
					{
						Do3_moshi++;if(Do3_moshi>3)Do3_moshi=0;
					}
					else if(cser==7)
					{
						Do4_moshi++;if(Do4_moshi>3)Do4_moshi=0;
					}
					else if(cser==8)
					{
						if(kk<10)
						{
							kout_shu_A1++;
							kk++;
						}else kout_shu_A1=kout_shu_A1+10;
						if(kout_shu_A1==0|kout_shu_A1>5000)kout_shu_A1=1;
					}
					else if(cser==9)
					{
						if(kk<10)
						{
							kout_shu_A2++;
							kk++;
						}else kout_shu_A2=kout_shu_A2+10;
						if(kout_shu_A2==0|kout_shu_A2>5000)kout_shu_A2=1;
					}
					else if(cser==10)
					{
						if(kk<10)
						{
							kout_shu_A3++;
							kk++;
						}else kout_shu_A3=kout_shu_A3+10;
						if(kout_shu_A3==0|kout_shu_A3>5000)kout_shu_A3=1;
					}
					else if(cser==11)
					{
						if(kk<10)
						{
							kout_shu_A4++;
							kk++;
						}else kout_shu_A4=kout_shu_A4+10;
						if(kout_shu_A4==0|kout_shu_A4>5000)kout_shu_A4=1;
					}

					else if(cser==12)
					{
						if(kk<10)
						{
							kout_shu_V1++;
							kk++;
						}else kout_shu_V1=kout_shu_V1+10;
						if(kout_shu_V1==0|kout_shu_V1>5000)kout_shu_V1=1;
					}
					else if(cser==13)
					{
						if(kk<10)
						{
							kout_shu_V2++;
							kk++;
						}else kout_shu_V2=kout_shu_V2+10;
						if(kout_shu_V2==0|kout_shu_V2>5000)kout_shu_V2=1;
					}
					else if(cser==14)
					{
						if(kk<10)
						{
							kout_shu_V3++;
							kk++;
						}else kout_shu_V3=kout_shu_V3+10;
						if(kout_shu_V3==0|kout_shu_V3>5000)kout_shu_V3=1;
					}
					else if(cser==15)
					{
						if(kk<10)
						{
							kout_shu_V4++;
							kk++;
						}else kout_shu_V4=kout_shu_V4+10;
						if(kout_shu_V4==0|kout_shu_V4>5000)kout_shu_V4=1;
					}
					else if(cser==16)
					{
						if(kk<10)
						{
							Do_timer_S++;
							kk++;
						}else Do_timer_S=Do_timer_S+10;
						if(Do_timer_S==0|Do_timer_S>250)Do_timer_S=1;
					} else
					if(cser==17)
					{ 					
						if(fig_pt==0)fig_pt=1;
					}
				}
				else 
				if(sj_tt)
				{
					cser++;
					if(cser>17)cser=1;
				}
			}break;
			case 2:
			{
				if(da_tt)
				{
					if(cser==1)sx_sx=~sx_sx;
					else if(cser==2)us_ys=~us_ys;
					else if(cser==3)is_yw=~is_yw;
					else 
					if(cser==4)
					{
						if(kk<10)
						{
							dybb++;
							kk++;
						}else dybb=dybb+10;
						if(dybb>1000)dybb=1;
					}
					else
					if(cser==5)
					{
						if(kk<10)
						{
							dlbb++;
							kk++;
						}else dlbb=dlbb+10;
						if(dlbb>5000)dlbb=1;
					}
				}
				else 
				if(sj_tt)
				{
					cser++;
					if(cser>5)cser=1;
				}
			}break;
			case 3:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						if(kk<10)
						{
							ipdz++;
							kk++;
						}else ipdz=ipdz+10;
						if(ipdz>247)ipdz=1;
					}
					else if(cser==2)btl_t=~btl_t;
					else
					if(cser==3)
					{
						das=shez&0x03;
						das++;
						if(das>2)das=0;
						shez=(shez&0xfc)+das;
					}
				}
				else 
				if(sj_tt)
				{
					cser++;
					if(cser>3)cser=1;
				}
			}break;
			case 4:
			{
				if(da_tt)
				{
					if(cser==1)
					{
						if(kk<10)
						{
							xgma++;
							kk++;
						}else xgma=xgma+10;
						if(xgma>9999)xgma=0;
					}
					else 
					if(cser==2)
					{
						if(kk<10)
						{
							xgmb++;
							kk++;
						}else xgmb=xgmb+10;
						if(xgmb>9999)xgmb=0;
					}
					else
					if(cser==3)
					{
						if(kk<10)
						{
							xgmc++;
							kk++;
						}else xgmc=xgmc+10;
						if(xgmc>9999)xgmc=0;
					}
				}
			}break;
			default:break;
		}
	}
	else
	if(cs_xt)
	{
		if(mima==yhmm)
		{
			csyi++;
			if(csyi>4)csyi=1;
		}
	}
}
//键盘处理程序
void key_in(void)
{
	uchar i;
	key_i=P0;
	if(key_i!=0xff)tcjs=0;	
		if(ss1==0)//AN1键 减
		{
				if(key_k==0)
				{
					key_k=1;sx_bz=0;
					jiandata();
				}
				else
						{
					if(cs_tt)
					{
						jj++;
						if(jj>10){key_k=0;jj=0;}
					}
				}
		}
		if(ss2==0)//AN2键 加
		{
				if(key_k==0)
				{
					key_k=1;sx_bz=0;
					jia_data();
				}
				else
				{
					if(cs_tt)
					{
						jj++;
						if(jj>10){key_k=0;jj=0;}
					}
				}
		}
		if(ss3==0)//AN3 设置
		{
				if(key_k==0)
				{
					key_k=1;
					if(cs_tt==0)
					{
						cs_tt=1;csyi=0;
						cs_xt=0;sj_tt=0;
						sj_bc=0;da_tt=0;
						mima=9999;cser=0;
					}
					else
					{
						if(da_tt)da_tt=0;
						else 
						if(sj_tt){sj_tt=0;cser=0;}
						else
						if(cs_xt){cs_xt=0;sj_bc=1;}
						else 
						if(sj_bc){sj_bc=0;cs_tt=0;}
						else cs_tt=0;
					}
				}
			}
		if(ss4==0)//AN4键 确定
		{
				if(key_k==0)
				{
					key_k=1;sx_bz=0;
					if(cs_tt)sec_value();//确定
					else if(xian==0)xn_sx=~xn_sx;
				}
		}
		if(ss1&ss2&ss3&ss4)	
		{
			key_k=0;kk=0;jj=0;
		}	
	
	if((an1==0)||((ss2==0)&&(ss3==0)))
	{
		delay1(20);		
			if(mima==0x02)
			{
				xb_bz=1;bc_tt=0;
				mima=0;sj_tt=0;
			}
			else
			if(mima==0x03)
			{
				xj_mz=1;bc_tt=0;
				mima=0;sj_tt=0;
			}
			else
			if(mima==0x04)
			{
				xj_bs=1;bc_tt=0;
				mima=0;sj_tt=0;
			}
			else
			if(mima==0x05)
			{
				in_tt=1;
				for(i=0;i<32;i++)
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
