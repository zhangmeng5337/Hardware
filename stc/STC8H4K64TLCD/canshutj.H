uchar key_i;	//按键值
uchar j,k;
bit ky_lock;

sbit en_cp2=P3^1;	//按键选通
sbit clk2  =P3^0;	//按键时钟
sbit data_i=P2^1;	//按键数据输入

//两字节转为INT并返回
tchr_int(uchar ch1,ch2)
{
	uint ch;
	ch=ch1;ch<<=8;ch=ch|ch2;
	return(ch);
}
//读入HC165里面的数据
void read74hc165m(void)
{
	uchar i;
	en_cp2=0;
	for(i=0;i<8;i++)
	{
		clk2=0;
		key_i=key_i<<1;
		clk2=1;
		en_cp2=1;
		if(data_i==1)key_i++;
	}
}
//确定键处理程序
void sec_value(void)
{
	uchar buf_h,buf_l,aa,i;
	switch(cans)
	{
		case 0:		//密码	
		{
			if(mima==0x04d2)
			{
				for(i=0;i<34;i++)
				{
					write_add(i,sd[i]);
					delay1(100);
				}
			}
			sdsz_t=0;
		}break;
		case 1:		//第1路漏电报警设定		
		{
			buf_h=ld_1>>8;
			buf_l=ld_1&0xff;
			aa=read_add(0x00);
			delay1(100);
			if(buf_h!=aa)write_add(0x00,buf_h);
			delay1(100);
			aa=read_add(0x01);
			if(buf_l!=aa)write_add(0x01,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 2:		//第2路漏电报警设定		
		{
			buf_h=ld_2>>8;
			buf_l=ld_2&0xff;
			aa=read_add(0x02);
			delay1(100);
			if(buf_h!=aa)write_add(0x02,buf_h);
			delay1(100);
			aa=read_add(0x03);
			if(buf_l!=aa)write_add(0x03,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 3:		//第3路漏电报警设定		
		{
			buf_h=ld_3>>8;
			buf_l=ld_3&0xff;
			aa=read_add(0x04);
			delay1(100);
			if(buf_h!=aa)write_add(0x04,buf_h);
			delay1(100);
			aa=read_add(0x05);
			if(buf_l!=aa)write_add(0x05,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 4:		//第4路漏电报警设定		
		{
			buf_h=ld_4>>8;
			buf_l=ld_4&0xff;
			aa=read_add(0x06);
			delay1(100);
			if(buf_h!=aa)write_add(0x06,buf_h);
			delay1(100);
			aa=read_add(0x07);
			if(buf_l!=aa)write_add(0x07,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 5:		//第5路漏电报警设定		
		{
			buf_h=ld_5>>8;
			buf_l=ld_5&0xff;
			aa=read_add(0x08);
			delay1(100);
			if(buf_h!=aa)write_add(0x08,buf_h);
			delay1(100);
			aa=read_add(0x09);
			if(buf_l!=aa)write_add(0x09,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 6:		//第6路漏电报警设定		
		{
			buf_h=ld_6>>8;
			buf_l=ld_6&0xff;
			aa=read_add(0x0a);
			delay1(100);
			if(buf_h!=aa)write_add(0x0a,buf_h);
			delay1(100);
			aa=read_add(0x0b);
			if(buf_l!=aa)write_add(0x0b,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 7:		//第7路漏电报警设定		
		{
			buf_h=ld_7>>8;
			buf_l=ld_7&0xff;
			aa=read_add(0x0c);
			delay1(100);
			if(buf_h!=aa)write_add(0x0c,buf_h);
			delay1(100);
			aa=read_add(0x0d);
			if(buf_l!=aa)write_add(0x0d,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 8:		//第8路漏电报警设定		
		{
			buf_h=ld_8>>8;
			buf_l=ld_8&0xff;
			aa=read_add(0x0e);
			delay1(100);
			if(buf_h!=aa)write_add(0x0e,buf_h);
			delay1(100);
			aa=read_add(0x0f);
			if(buf_l!=aa)write_add(0x0f,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 9:		//8路漏电流%比		
		{
			aa=read_add(0x10);
			delay1(100);
			if(ldyj!=aa)write_add(0x10,ldyj);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 10:		//1路温度		
		{
			aa=read_add(0x11);
			delay1(100);
			if(wd_1!=aa)write_add(0x11,wd_1);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 11:		//2路温度		
		{
			aa=read_add(0x12);
			delay1(100);
			if(wd_2!=aa)write_add(0x12,wd_2);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 12:		//3路温度		
		{
			aa=read_add(0x13);
			delay1(100);
			if(wd_3!=aa)write_add(0x13,wd_3);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 13:		//4路温度		
		{
			aa=read_add(0x14);
			delay1(100);
			if(wd_4!=aa)write_add(0x14,wd_4);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 14:		//5路温度		
		{
			aa=read_add(0x15);
			delay1(100);
			if(wd_5!=aa)write_add(0x15,wd_5);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 15:		//6路温度		
		{
			aa=read_add(0x16);
			delay1(100);
			if(wd_6!=aa)write_add(0x16,wd_6);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 16:		//7路温度		
		{
			aa=read_add(0x17);
			delay1(100);
			if(wd_7!=aa)write_add(0x17,wd_7);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 17:		//8路温度		
		{
			aa=read_add(0x18);
			delay1(100);
			if(wd_8!=aa)write_add(0x18,wd_8);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 18:		//8路温度%比	
		{
			aa=read_add(0x19);
			delay1(100);
			if(wdyj!=aa)write_add(0x19,wdyj);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 19:		//设备地址		
		{
			buf_h=id>>8;
			buf_l=id&0xff;
			aa=read_add(0x1a);
			delay1(100);
			if(buf_h!=aa)write_add(0x1a,buf_h);
			delay1(100);
			aa=read_add(0x1b);
			if(buf_l!=aa)write_add(0x1b,buf_l);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 20:		//保护输出	
		{
			aa=read_add(0x1c);
			delay1(100);
			if(tk_t!=aa)write_add(0x1c,tk_t);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 21:		//报警输出	
		{
			aa=read_add(0x1d);
			delay1(100);
			if(bj_t!=aa)write_add(0x1d,bj_t);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 22:	//漏电校0		
		{
			aa=read_add(0x1e);
			delay1(100);
			if(tldl!=aa)write_add(0x1e,tldl);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 23:	//漏电校正		
		{
			aa=read_add(0x1f);
			delay1(100);
			if(syxz!=aa)write_add(0x1f,syxz);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 24:	//注册温度1		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd1[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0x80+i,wd1[i]);
				delay1(100);
			}
			for(i=0;i<255;i++){disp_end();delay1(5);}
		}break;
		case 25:	//注册温度2		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd2[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0x88+i,wd2[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 26:	//注册温度3		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd3[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0x90+i,wd3[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 27:	//注册温度4		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd4[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0x98+i,wd4[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 28:	//注册温度5		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd5[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0xa0+i,wd5[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 29:	//注册温度6		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd6[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0xa8+i,wd6[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 30:	//注册温度7		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd7[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0xb0+i,wd7[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 31:	//注册温度8		
		{
			Init_DS18B20();
			W_DS18B20(0x33); //读序号列号的操作
			for(i=0;i<8;i++)
			{
				wd8[i]=R_DS18B20();
			}
			for(i=0;i<8;i++)
			{
				write_add(0xb8+i,wd8[i]);
				delay1(100);
			}
			for(i=0;i<250;i++){disp_end();delay1(10);}
		}break;
		case 32:	//漏电路数		
		{
			aa=read_add(0x20);
			delay1(100);
			if(ldls!=aa)write_add(0x20,ldls);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		case 33:	//温度路数		
		{
			aa=read_add(0x21);
			delay1(100);
			if(wdls!=aa)write_add(0x21,wdls);
			delay1(100);
			for(i=0;i<250;i++){disp_end();delay1(10);}
			sdsz_t=0;
		}break;
		default:break;			
	}
}
//加键处理程序
void up_value(void)
{
	if(cans_t==0)
	{
		j++;
		if(j>60)
		{
			if(xszh_t==0)
			{

				ld_xs++;
				if(ld_xs>ldls)ld_xs=1;
			}
			else
			{
				wd_xs++;
				if(wd_xs>wdls)wd_xs=1;
				j=0;
			}
			j=0;
		}
	}
	else
	{
		if(sdsz_t==0)
		{
			if(mima==9876)
			{
				j++;
				if(j>80)
				{
					if(k<10){cans--;}
					j=0;
				}
				if(cans>33)cans=0;
			}
			else
			{
				if(mima==9999)
				{
					j++;
					if(j>80)
					{
						if(k<10){cans++;}
						j=0;
					}
					if(cans>31)cans=0;
				}
			}	
		}
		else
		{
			switch(cans)
			{
				case 0:		//密码
				{
					j++;
					if(j>80)
					{
						if(k<10){mima++;k++;}
						else mima=mima+10;
						j=0;
					}
					if(mima>9999)mima=0;
				}break;
				case 1:		//第1路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_1<999)
						{
							if(k<10){ld_1++;k++;}
							else ld_1=ld_1+10;
						}
						if(ld_1>999)ld_1=999;
						j=0;
					}
				}break;
				case 2:		//第2路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_2<999)
						{
							if(k<10){ld_2++;k++;}
							else ld_2=ld_2+10;
						}
						if(ld_2>999)ld_2=999;
						j=0;
					}
				}break;	
				case 3:		//第3路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_3<999)
						{
							if(k<10){ld_3++;k++;}
							else ld_3=ld_3+10;
						}
						if(ld_3>999)ld_3=999;
						j=0;
					}
				}break;
				case 4:		//第4路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_4<999)
						{
							if(k<10){ld_4++;k++;}
							else ld_4=ld_4+10;
						}
						if(ld_4>999)ld_4=999;
						j=0;
					}
				}break;
				case 5:		//第5路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_5<999)
						{
							if(k<10){ld_5++;k++;}
							else ld_5=ld_5+10;
						}
						if(ld_5>999)ld_5=999;
						j=0;
					}
				}break;
				case 6:		//第6路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_6<999)
						{
							if(k<10){ld_6++;k++;}
							else ld_6=ld_6+10;
						}
						if(ld_6>999)ld_6=999;
						j=0;
					}
				}break;
				case 7:		//第7路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_7<999)
						{
							if(k<10){ld_7++;k++;}
							else ld_7=ld_7+10;
						}
						if(ld_7>999)ld_7=999;
						j=0;
					}
				}break;
				case 8:		//第8路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_8<999)
						{
							if(k<10){ld_8++;k++;}
							else ld_8=ld_8+10;
						}
						if(ld_8>999)ld_8=999;
						j=0;
					}
				}break;
				case 9:		//8路漏电预警设定
				{
					j++;
					if(j>80)
					{
						if(ldyj<90)
						{
							if(k<10){ldyj++;k++;}
							else ldyj=ldyj+10;
						}
						if(ldyj>90)ldyj=90;
						j=0;
					}
				}break;
				case 10:	//第1路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_1<125)
						{
							if(k<10){wd_1++;k++;}
							else wd_1=wd_1+10;
						}
						if(wd_1>125)wd_1=125;
						j=0;
					}
				}break;
				case 11:	//第2路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_2<125)
						{
							if(k<10){wd_2++;k++;}
							else wd_2=wd_2+10;
						}
						if(wd_2>125)wd_2=125;
						j=0;
					}
				}break;	
				case 12:	//第3路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_3<125)
						{
							if(k<10){wd_3++;k++;}
							else wd_3=wd_3+10;
						}
						if(wd_3>125)wd_3=125;
						j=0;
					}
				}break;
				case 13:	//第4路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_4<125)
						{
							if(k<10){wd_4++;k++;}
							else wd_4=wd_4+10;
						}
						if(wd_4>125)wd_4=125;
						j=0;
					}
				}break;
				case 14:	//第5路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_5<125)
						{
							if(k<10){wd_5++;k++;}
							else wd_5=wd_5+10;
						}
						if(wd_5>125)wd_5=125;
						j=0;
					}
				}break;
				case 15:	//第6路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_6<125)
						{
							if(k<10){wd_6++;k++;}
							else wd_6=wd_6+10;
						}
						if(wd_6>125)wd_6=125;
						j=0;
					}
				}break;
				case 16:	//第7路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_7<125)
						{
							if(k<10){wd_7++;k++;}
							else wd_7=wd_7+10;
						}
						if(wd_7>125)wd_7=125;
						j=0;
					}
				}break;
				case 17:		//第8路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_8<125)
						{
							if(k<10){wd_8++;k++;}
							else wd_8=wd_8+10;
						}
						if(wd_8>125)wd_8=125;
						j=0;
					}
				}break;
				case 18:	//8路温度预警设定
				{
					j++;
					if(j>80)
					{
						if(wdyj<90)
						{
							if(k<10){wdyj++;k++;}
							else wdyj=wdyj+10;
						}
						if(wd_1>90)wd_1=90;
						j=0;
					}
				}break;
				case 19:	//设备地址
				{
					j++;
					if(j>80)
					{
						if(id<999)
						{
							if(k<10){id++;k++;}
							else id=id+10;
						}
						if(id>999)id=999;
						j=0;
					}
				}break;
				case 20:	//保护输出
				{
					if(tk_t==0)tk_t=1;
				}break;
				case 21:	//报警输出
				{
					if(bj_t==0)bj_t=1;
				}break;
				case 22:	//漏电调0
				{
					j++;
					if(j>80)
					{
						if(tldl<100)
						{
							if(k<10){tldl++;k++;}
							else tldl=tldl+10;
						}
						if(tldl>100)tldl=100;
						j=0;
					}
				}break;
				case 23:	//漏电修正	50-150
				{
					j++;
					if(j>80)
					{
						if(syxz<150)
						{
							if(k<10){syxz++;k++;}
							else syxz=syxz+10;
						}
						if(syxz>150)syxz=150;
						j=0;
					}
				}break;
				case 32:	//漏电路数	1-8
				{
					j++;
					if(j>80)
					{
						ldls++;
						if(ldls>8)ldls=8;
						j=0;
					}
				}break;
				case 33:	//温度路数	0-8
				{
					j++;
					if(j>80)
					{
						wdls++;
						if(wdls>8)wdls=8;
						j=0;
					}
				}break;
				default:break;
			}
		}
	}
}
//减键处理程序
void down_value(void)
{
	if(cans_t==0)
	{
		j++;
		if(j>60)
		{
			if(xszh_t==0)
			{

				ld_xs--;
				if(ld_xs<1)ld_xs=ldls;
			}
			else
			{
				wd_xs--;
				if(wd_xs<1)wd_xs=wdls;
				j=0;
			}
			j=0;
		}
	}
	else
	{
		if(sdsz_t==0)
		{
			if(mima==9876)
			{
				j++;
				if(j>80)
				{
					if(k<10){cans--;}
					j=0;
				}
				if(cans>33)cans=33;
			}
			else
			{
				if(mima==9999)
				{
					j++;
					if(j>80)
					{
						if(k<10){cans--;}
						j=0;
					}
					if(cans>31)cans=31;
				}
			}	
		}
		else
		{
			switch(cans)
			{
				case 0:		//密码
				{
					j++;
					if(j>80)
					{
						if(k<10){mima--;k++;}
						else mima=mima-10;
						j=0;
					}
					if(mima>9999)mima=9999;
				}break;
				case 1:		//第1路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_1>20)
						{
							if(k<10){ld_1--;k++;}
							else ld_1=ld_1-10;
						}
						if(ld_1<20)ld_1=20;
						j=0;
					}
				}break;
				case 2:		//第2路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_2>20)
						{
							if(k<10){ld_2--;k++;}
							else ld_2=ld_2-10;
						}
						if(ld_2<20)ld_2=20;
						j=0;
					}
				}break;	
				case 3:		//第3路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_3>20)
						{
							if(k<10){ld_3--;k++;}
							else ld_3=ld_3-10;
						}
						if(ld_3<20)ld_3=20;
						j=0;
					}
				}break;
				case 4:		//第4路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_4>20)
						{
							if(k<10){ld_4--;k++;}
							else ld_4=ld_4-10;
						}
						if(ld_4<20)ld_4=20;
						j=0;
					}
				}break;
				case 5:		//第5路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_5>20)
						{
							if(k<10){ld_5--;k++;}
							else ld_5=ld_5-10;
						}
						if(ld_5<20)ld_5=20;
						j=0;
					}
				}break;
				case 6:		//第6路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_6>20)
						{
							if(k<10){ld_6--;k++;}
							else ld_6=ld_6-10;
						}
						if(ld_6<20)ld_6=20;
						j=0;
					}
				}break;
				case 7:		//第7路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_7>20)
						{
							if(k<10){ld_7--;k++;}
							else ld_7=ld_7-10;
						}
						if(ld_7<20)ld_7=20;
						j=0;
					}
				}break;
				case 8:		//第8路漏电报警设定
				{
					j++;
					if(j>80)
					{
						if(ld_8>20)
						{
							if(k<10){ld_8--;k++;}
							else ld_8=ld_8-10;
						}
						if(ld_8<20)ld_8=20;
						j=0;
					}
				}break;
				case 9:		//8路漏电预警设定
				{
					j++;
					if(j>80)
					{
						if(ldyj>50)
						{
							if(k<10){ldyj--;k++;}
							else ldyj=ldyj-10;
						}
						if(ldyj<50)ldyj=50;
						j=0;
					}
				}break;
				case 10:	//第1路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_1>20)
						{
							if(k<10){wd_1--;k++;}
							else wd_1=wd_1-10;
						}
						if(wd_1<20)wd_1=20;
						j=0;
					}
				}break;
				case 11:	//第2路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_2>20)
						{
							if(k<10){wd_2--;k++;}
							else wd_2=wd_2-10;
						}
						if(wd_2<20)wd_2=20;
						j=0;
					}
				}break;	
				case 12:	//第3路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_3>20)
						{
							if(k<10){wd_3--;k++;}
							else wd_3=wd_3-10;
						}
						if(wd_3<20)wd_3=20;
						j=0;
					}
				}break;
				case 13:	//第4路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_4>20)
						{
							if(k<10){wd_4--;k++;}
							else wd_4=wd_4-10;
						}
						if(wd_4<20)wd_4=20;
						j=0;
					}
				}break;
				case 14:	//第5路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_5>20)
						{
							if(k<10){wd_5--;k++;}
							else wd_5=wd_5-10;
						}
						if(wd_5<20)wd_5=20;
						j=0;
					}
				}break;
				case 15:	//第6路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_6>20)
						{
							if(k<10){wd_6--;k++;}
							else wd_6=wd_6-10;
						}
						if(wd_6<20)wd_6=20;
						j=0;
					}
				}break;
				case 16:	//第7路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_7>20)
						{
							if(k<10){wd_7--;k++;}
							else wd_7=wd_7-10;
						}
						if(wd_7<20)wd_7=20;
						j=0;
					}
				}break;
				case 17:		//第8路温度报警设定
				{
					j++;
					if(j>80)
					{
						if(wd_8>20)
						{
							if(k<10){wd_8--;k++;}
							else wd_8=wd_8-10;
						}
						if(wd_8<20)wd_8=20;
						j=0;
					}
				}break;
				case 18:	//8路温度预警设定
				{
					j++;
					if(j>80)
					{
						if(wdyj>50)
						{
							if(k<10){wdyj--;k++;}
							else wdyj=wdyj-10;
						}
						if(wdyj<50)wdyj=50;
						j=0;
					}
				}break;
				case 19:	//设备地址
				{
					j++;
					if(j>80)
					{
						if(id>0)
						{
							if(k<10){id--;k++;}
							else id=id-10;
						}
						if(id>999)id=0;
						j=0;
					}
				}break;
				case 20:	//保护输出
				{
					if(tk_t==1)tk_t=0;
				}break;
				case 21:	//报警输出
				{
					if(bj_t==1)bj_t=0;
				}break;
				case 22:	//漏电调0
				{
					j++;
					if(j>80)
					{
						if(tldl>0)
						{
							if(k<10){tldl--;k++;}
							else tldl=tldl-10;
						}
						if(tldl>100)tldl=0;
						j=0;
					}
				}break;
				case 23:	//漏电修正	50-150
				{
					j++;
					if(j>80)
					{
						if(syxz>50)
						{
							if(k<10){syxz--;k++;}
							else syxz=syxz-10;
						}
						if(syxz<50)syxz=50;
						j=0;
					}
				}break;
				case 32:	//漏电路数	1-8
				{
					j++;
					if(j>80)
					{
						ldls--;
						if(ldls<1)ldls=1;
						j=0;
					}
				}break;
				case 33:	//温度路数	0-8
				{
					j++;
					if(j>80)
					{
						wdls--;
						if(wdls>8)wdls=0;
						j=0;
					}
				}break;
				default:break;
			}
		}
	}
}
//键盘处理程序,入口K作为按键的值
void key_pro(uchar k)
{
	uchar yy;
	switch(k)	
	{
		case 0xfe:		//返回
		if(ky_lock==0)
		{
			ky_lock=1;
			if(sdsz_t==1)sdsz_t=0;
			else if(cans_t==1){cans_t=0;cans=0;mima=0;}
		}break;
		case 0xfd:		//切换与设置键
		if(ky_lock==0)
		{
			ky_lock=1;
			if(wdls!=0)xszh_t=~xszh_t;
			else xszh_t=0;
		}break;
		case 0xfb:		//确定键
		if(ky_lock==0)
		{
			ky_lock=1;
			if(cans_t==1)
			{
				if(sdsz_t==0)sdsz_t=1;
				else sec_value();
			}
		}break;
		case 0xf7:		//加键
		up_value();
		break;
		case 0xef:		//减键
		down_value();
		break;
		case 0xdf:		//试验键
		if(ky_lock==0)
		{
			ky_lock=1;	
			ISP_CONTR=0x20;//软件复位
		}break;
		case 0xbf:		//消音键
		if(ky_lock==0)
		{
			xs_tt=~xs_tt;
			ky_lock=1;
		}break;
		case 0x7f:		//复位键
		if(ky_lock==0)
		{
			ky_lock=1;
			fuwei();
			yy=50;
			while(yy!=0){diap_xs();yy--;}
		}break;
		default:break;
	}
}
//按键子
void key_in(void)
{
	uchar a;
	read74hc165m();
	if((key_i!=0xff)&(key_i!=0))
	{
		a=key_i;
		delay1(10);
		read74hc165m();
		if((key_i!=0xff)&(key_i==a)){key_pro(key_i);}
	}
	else {ky_lock=0;j=0;k=0;}
}
