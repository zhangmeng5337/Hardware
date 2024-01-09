//sbit an1=P1^2;  
sbit ss1=P3^3;	//-
sbit ss2=P3^2;	//+
sbit ss3=P1^7;	//设置
sbit ss4=P1^6;	//确定
bit key_k;

//baocu
void baoc_data()
{  	
	 xieru();
	cs_tt=0;csyi=0;
	cs_xt=0;sj_tt=0;
	sj_bc=0;da_tt=0;
	mima=9999;cser=0;
	xgma=0;xgmb=0;xgmc=0;
}

//SEC键处理程序
void sec_value(void)
{ 
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
					in_tt=1;		
				}else
				if(cser==3)
				{ 					
					ep[15]=Disp_liangdu;	
				}else
				if(cser==4)
				{
					ep[16]=Do1_moshi;
				}else
				if(cser==5)
				{
					ep[17]=kout_shu_V1>>8;
					ep[18]=kout_shu_V1&0xFF;
				}else
				if(cser==6)
				{
					ep[19]=Do_timer_S;
				}else
				if(cser==7)
				{
					ep[20]=ct_shuliang;
				}else
				if(cser==9)
				{
					ep[9]=dljza>>8;
					ep[10]=dljza&0xFF;
				}else
				if(cser==10)
				{
					ep[11]=dljzb>>8;
					ep[12]=dljzb&0xFF;
				}else
				if(cser==11)
				{
					ep[13]=dljzc>>8;
					ep[14]=dljzc&0xFF;
				}else
				if(cser==12)
					ep[21]=pingbidianliu;
				da_tt=0;
			}
			if(csyi==2)
			{
				if(cser==1)
				{
					ep[8]=shez&0xfc;
				}else
				if(cser==2)
				{
					ep[8]=shez&0xfc;
				} else
				if(cser==3)
				{
					ep[8]=shez&0xfc;
				}else
				if(cser==4)
				{
					ep[0]=voltage_vary_ratio>>8;
					ep[1]=voltage_vary_ratio&0xFF;
				}else
				if(cser==5)
				{
					ep[2]=current_vary_ratio>>8;
					ep[3]=current_vary_ratio&0xFF;
				}
				da_tt=0;
			}else
			if(csyi==3)
			{
			   if(cser==1)
			   {
			   		ep[4]=ipdz;	
			   }else
			   if(cser==2)
			   {
			   		ep[8]=shez&0xfc;
			   }else
			   if(cser==3)
			   {
			   		ep[8]=shez&0x03;;
			   }
			   da_tt=0;
			}
			else
			if(csyi==4)
			{
				if(xgma==yhmm)
				{
					cser++;
					ep[6]=yhmm>>8;
					ep[7]=yhmm&0xFF;
					if(cser>4)cser=1;
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
			if(mima==yhmm){cs_xt=1;csyi=1;}
			else cs_tt=0;			
		}
	}
}

//减键处理程序
void jiandata(void)
{
	uchar das;
	if(cs_tt==0)
	{
		if(Do1_moshi!=2)
		{
			xian--;
			if(xian>1)xian=1;
		}else xian=0;
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
					}else if(cser==2)da_tt=0;
					else if(cser==3)
					{
						Disp_liangdu--;
						if(Disp_liangdu>4)Disp_liangdu=4;
					}else
					if(cser==4)
					{
						Do1_moshi--;
						if(Do1_moshi>2)
						Do1_moshi=2;
					}else
					if(cser==5)	  //
					{
						if(kk<10)
						{
							kout_shu_V1--;
							kk++;
						}else kout_shu_V1=kout_shu_V1-10;
						if(kout_shu_V1==0|kout_shu_V1>1000)kout_shu_V1=1000;
					}else
					if(cser==6)
					{
						if(kk<10)
						{
							Do_timer_S--;kk++;
						}else Do_timer_S=Do_timer_S-10;
						if(Do_timer_S==0|Do_timer_S>255)Do_timer_S=255;	
					}else
					if(cser==7)
					{
						ct_shuliang--;
						if(ct_shuliang>1)ct_shuliang=0;
					}else
					if(cser==8)
					{
						 if(kk<10)
						 {
						 	xzmima--;kk++;
						 }else xzmima=xzmima-10;
						 if(xzmima==0|xzmima>9999)xzmima=9999;
					}else
					if(cser==9)
					{
						if(kk<10)
						 {
						 	dljza--;kk++;
						 }else dljza=dljza-10;
						 if(dljza==0|dljza>9999)dljza=9999;
					}else
					if(cser==10)
					{
						if(kk<10)
						 {
						 	dljzb--;kk++;
						 }else dljzb=dljzb-10;
						 if(dljzb==0|dljzb>9999)dljzb=9999;
					}else
					if(cser==11)
					{
						if(kk<10)
						 {
						 	dljzc--;kk++;
						 }else dljzc=dljzc-10;
						 if(dljzc==0|dljzc>9999)dljzc=9999;
					}	else					
					if(cser==12)
					{
						if(kk<10)
						 {
						 	pingbidianliu--;kk++;
						 }else pingbidianliu=pingbidianliu-10;
						 if(pingbidianliu==0|pingbidianliu>255)dljzc=255;
					}						
				}
				else 
				if(sj_tt)
				{
					cser--;
					if(xzmima==12)
					{
						if(cser==0)cser=12;
					}
					else
					{
						if(cser==0)cser=8;
					}
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
							voltage_vary_ratio--;
							kk++;
						}else voltage_vary_ratio=voltage_vary_ratio-10;
						if(voltage_vary_ratio==0|voltage_vary_ratio>1000)voltage_vary_ratio=1000;
					}
					else
					if(cser==5)
					{
						if(kk<10)
						{
							current_vary_ratio--;
							kk++;
						}else current_vary_ratio=current_vary_ratio-10;
						if(current_vary_ratio==0|current_vary_ratio>5000)current_vary_ratio=5000;
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
		if(Do1_moshi!=2)
		{
			 xian++;
			if(xian>1)xian=0;
		}else xian=0;
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
					}else if(cser==2)da_tt=0;
					else if(cser==3)
					{
						Disp_liangdu++;
						if(Disp_liangdu>4)Disp_liangdu=0;
					}else
					if(cser==4)
					{
						Do1_moshi++;
						if(Do1_moshi>2)
						Do1_moshi=0;
					}else
					if(cser==5)
					{
						if(kk<10)
						{
						  kout_shu_V1++;kk++;
						}else kout_shu_V1=kout_shu_V1+10;
						if(kout_shu_V1>1000)kout_shu_V1=0;
					}else
					if(cser==6)
					{
						if(kk<10)
						{
							Do_timer_S++; kk++;
						} else Do_timer_S=Do_timer_S+10;
						if(Do_timer_S>255)Do_timer_S=0;
					} else
					if(cser==7)
					{
						ct_shuliang++;
						if(ct_shuliang>1)ct_shuliang=0;
					} else
					if(cser==8)
					{
						if(kk<10)
						{
							kk++;xzmima++;
						}else xzmima=xzmima+10;
						if(xzmima>9999)xzmima=0;
					}else
					if(cser==9)
					{
						if(kk<10)
						{
							kk++;dljza++;
						}else dljza=dljza+10;
						if(dljza>9999)dljza=0;
					}else
					if(cser==10)
					{
						if(kk<10)
						{
							kk++;dljzb++;
						}else dljzb=dljzb+10;
						if(dljzb>9999)dljzb=0;
					}else
					if(cser==11)
					{
						if(kk<10)
						{
							kk++;dljzc++;
						}else dljzc=dljzc+10;
						if(dljzc>9999)dljzc=0;
					}else 
					if(cser==12)
					{
						if(kk<10)
						{
							kk++;pingbidianliu++;
						}else pingbidianliu=pingbidianliu+10;
						if(pingbidianliu>255)pingbidianliu=3;
					}
				}
				else 
				if(sj_tt)
				{
					cser++;
					if(xzmima==11)
					{
						if(cser>12)cser=1;
					}else
					{
						if(cser>8)cser=1;
					}
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
							voltage_vary_ratio++;
							kk++;
						}else voltage_vary_ratio=voltage_vary_ratio+10;
						if(voltage_vary_ratio>1000)voltage_vary_ratio=1;
					}
					else
					if(cser==5)
					{
						if(kk<10)
						{
							current_vary_ratio++;
							kk++;
						}else current_vary_ratio=current_vary_ratio+10;
						if(current_vary_ratio>5000)current_vary_ratio=1;
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
	 if(ss1==0)
	 {	
	   tcjs=0;
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
				if(jj>50){key_k=0;jj=0;}
			}
		}
	 }
	if(ss2==0)	   // 加
	{
	   tcjs=0;
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
				if(jj>50){key_k=0;jj=0;}
			}
		}
	}
	   
	if(ss3==0)
	{
	   tcjs=0;
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
	 
	if(ss4==0)
	{
		tcjs=0;
		if(key_k==0)
		{
			key_k=1;sx_bz=0;
			if(cs_tt)sec_value();//确定
		}
	} 
	if(ss1&ss2&ss3&ss4)	
	{
		key_k=0;kk=0;jj=0;
	}
	if((ss2==0)&&(ss3==0))
	{		
		if(mima==0x02)
		{
			xb_bz=1;bc_tt=0;
			mima=0;sj_tt=0;
		}
		else
		if(mima==0x05)
		{
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
