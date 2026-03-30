#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "temp_pid.h"

PID_UPDATE pid_update1;	//更新PID参数
PID_UPDATE pid_update2;	//更新PID参数
PID_UPDATE pid_update3;	//更新PID参数
PID pid1; //存放PID算法所需要的数据
volatile float out1;

PID pid2; //存放PID算法所需要的数据
volatile float out2;

PID pid3; //存放PID算法所需要的数据
volatile float out3;

void PID1_SET()
{
//  //pid1.Sv=-168;//用户设定温度
//	if(pid_update1.change_flag == 1){
//		pid_update1.change_flag = 0;
//		pid1.Kp=250;
//		pid1.Ki=0;
//		pid1.Kd=2000;
	
}

void PID2_SET()
{
//  pid1.Sv=-168;//用户设定温度
//	pid1.Kp=250;
//	pid1.Ki=0;
//	pid1.Kd=2000;
}

void PID3_SET()
{
//  pid1.Sv=-168;//用户设定温度
//	pid1.Kp=250;
//	pid1.Ki=0;
//	pid1.Kd=2000;
}

void PID1_Init()
{
		pid1.ref = -200;
	  pid1.kp = 500;
    pid1.ki = 20;
    pid1.kd = 0;
    pid1.Integ = 0;
		pid1.err = 0;
    pid1.err_pre = 0;
    pid1.err_pre_pre = 0;
    pid1.max = 40000;
		pid1.min = 0;
//  pid1.Sv=-168;//用户设定温度
//	pid1.Kp=250;
//	pid1.Ki=0;
//	pid1.Kd=2000;
//	//pid1.OUT = 375;
//  //pid1.T=250;//PID计算周期
//  //pid1.Ti=500000;//积分时间
//  //pid1.Td=10000;//微分时间
//	pid1.pwmcycle=500;//pwm周期1000
//	pid1.OUT0=1;
//	pid1.times=0;
//	pid1.speed = 0;
//	pid1.low_cnt = 0;
	
}

void PID2_Init()
{
	  pid2.ref = -200;
		pid2.kp = 500;
    pid2.ki = 20;
    pid2.kd = 0;
    pid2.Integ = 0;
		pid2.err = 0;
    pid2.err_pre = 0;
    pid2.err_pre_pre = 0;
    pid2.max = 40000;
		pid2.min = 0;
//  pid2.Sv=-160;//用户设定温度
//	pid2.Kp=250;
//	pid2.Ki=0;
//	pid2.Kd=2000;
//	pid2.pwmcycle=500;//pwm周期1000
//	pid2.OUT0=1;
//	pid2.speed = 0;
//	pid2.low_cnt = 0;
}

void PID3_Init()
{
//  pid3.Sv=-190;//用户设定温度
//	pid3.Kp=80;
//	pid3.Ki=0;
//	pid3.Kd=150;
//	pid3.pwmcycle=500;//pwm周期1000
//	pid3.OUT0=1;
//	pid3.speed = 0;
//	pid3.low_cnt = 0;
	  pid3.ref = -200;
		pid3.kp = 500;
    pid3.ki = 20;
    pid3.kd = 0;
    pid3.Integ = 0;
		pid3.err = 0;
    pid3.err_pre = 0;
    pid3.err_pre_pre = 0;
    pid3.max = 40000;
		pid3.min = 0;
}


void PID1_Calc()  //pid计算
{
//  float DelEk;
//	float ti,ki;
//  //float Iout;
//  //float Pout;
//  //float Dout;
//	float td;
//	float kd;
//	
//  pid1.Ek=pid1.Sv-pid1.Pv;   //得到当前的偏差值
//  pid1.speed = ((pid1.Pv - pid1.Pv1)/(CAL_TIME/1000.0));//计算速度
//// if(pid1.Ek > 150)
////	pid1.Kp = 20;
//// if(pid1.Ek > 100)
////	pid1.Kp = 13;
//// else if(pid1.Ek > 70)
////	pid1.Kp = 10;
//// else if(pid1.Ek > 50)
////	pid1.Kp = 13;
////  else if(pid1.Ek > 20)
////	pid1.Kp = 10;
//// else if(pid1.Ek > 10)
////	pid1.Kp = 10;
////	 else if(pid1.Ek > 5)
////	pid1.Kp = 20;
////	 else
////		 pid1.Kp = 7;
//// 
// 
// pid1.SEk+=pid1.Ek;        //历史偏差总和
// 
// DelEk=pid1.Ek-pid1.Ek_1;  //最近两次偏差之差
// 
//  pid1.Pout=pid1.Kp*pid1.Ek;      //比例输出
//  pid1.Iout=pid1.Ki*pid1.SEk;
////	if(pid1.Iout<-10)
////		pid1.Iout = -10;
////		if(pid1.Iout>100)
////		pid1.Iout = 1000;
//  pid1.Dout=pid1.Kd*DelEk;    //微分输出
// 
// out1= pid1.Pout+ pid1.Iout+ pid1.Dout;
// 
// //////////////////////////////////////////////////////////
// 
// if(out1>(pid1.pwmcycle))
// {
//  pid1.OUT=pid1.pwmcycle;
// }
// else if(out1<0)
// {
//  //pid1.OUT=pid1.OUT0; 
//	 pid1.OUT=0; 
// }
// else 
// {
//  pid1.OUT=out1;
// }
// //pid.OUT+=; //本次的计算结果
// pid1.Ek_1=pid1.Ek;  //更新偏差
// pid1.Pv1=pid1.Pv;//保存上次温度
// if(pid1.Ek>MAX_EK)
// {
//		pid1.low_cnt++;
// }
// else
// {
//		pid1.low_cnt=0;
// }
 
		pid1.err = pid1.ref - pid1.fd;//ref?????????,fd?????????? 
    pid1.Integ = (pid1.kp + pid1.ki) * pid1.err - (pid1.kp * pid1.err_pre);
//    p->Integ = (p->kp + p->ki + p->kd) * p->err - (p->kp + 2 * p->kd) * p->err_pre + p->kd * p->err_pre_pre; 
		if(pid1.err >15)//修改out初值
		{
			if(pid1.ref <120)
			{
				pid1.max = 40000;
			}
			else
			{	
				pid1.max = 45000;
			}
		}
		else if(pid1.err >10)//后面温度升的快 温差小的时候限幅值小一点
		{
			if(pid1.ref <120)
			{
				pid1.max = 30000;
			}
			else
			{	
				pid1.max = 40000;
			}
			
		}
		else if(pid1.err >5){
			
			if(pid1.ref <120)
			{
				pid1.max = 20000;
			}
			else
			{	
				pid1.max = 30000;
			}
		}
		else if(pid1.err >2){
			
			if(pid1.ref <120)
			{
				pid1.max = 10000;
			}
			else
			{	
				pid1.max = 20000;
			}
		}
		else
		{
			pid1.Integ = 2000	;
			if(pid1.ref <120)
			{
				pid1.max = 10000;
			}
			else
			{	
				pid1.max = 20000;
			}
			
		}
		if(pid1.err < 0) 
		{
			pid1.Integ = 0;
			pid1.out = 0;
		}
		else
		{
			 pid1.out = pid1.Integ+ pid1.out;
		}
   
    if(pid1.out >= pid1.max)
    {
        pid1.out = pid1.max;
    }
    else if(pid1.out<=pid1.min)
    {
        pid1.out = pid1.min;
    }
    pid1.err_pre = pid1.err;
    pid1.err_pre_pre = pid1.err_pre;
}


void PID2_Calc()  //pid计算
{
// float DelEk;
//	float ti,ki;
////	float Iout;
////	float Pout;
////	float Dout;
//	float td;
//	float kd;
//	

// 
// pid2.Ek=pid2.Sv-pid2.Pv;   //得到当前的偏差值
//  pid2.speed = ((pid2.Pv - pid2.Pv1)/(CAL_TIME/1000.0));//计算速度
// pid2.SEk+=pid2.Ek;        //历史偏差总和
// DelEk=pid2.Ek-pid2.Ek_1;  //最近两次偏差之差
//  pid2.Pout=pid2.Kp*pid2.Ek;      //比例输出
//  pid2.Iout=pid2.Ki*pid2.SEk;
//  pid2.Dout=pid2.Kd*DelEk;    //微分输出
// 
// out2= pid2.Pout+ pid2.Iout+ pid2.Dout;

// if(out2>(pid2.pwmcycle))
// {
//  pid2.OUT=pid2.pwmcycle;
// }
// else if(out2<0)
// { 
//	 pid2.OUT=0; 
// }
// else 
// {
//  pid2.OUT=out2;
// }
// pid2.Ek_1=pid2.Ek;  //更新偏差
// pid2.Pv1=pid2.Pv;//保存上次温度
// if(pid2.Ek>MAX_EK)
// {
//		if(pid2.low_cnt<=50)
//	{
//		pid2.low_cnt++;
//	}

//	 
// }
// else
// {
//		pid2.low_cnt = 0;
// }
		pid2.err = pid2.ref - pid2.fd;//ref?????????,fd?????????? 
    pid2.Integ = (pid2.kp + pid2.ki) * pid2.err - (pid2.kp * pid2.err_pre);
//    p->Integ = (p->kp + p->ki + p->kd) * p->err - (p->kp + 2 * p->kd) * p->err_pre + p->kd * p->err_pre_pre; 
		if(pid2.err >15)//修改out初值
		{
			if(pid2.ref <120)
			{
				pid2.max = 40000;
			}
			else
			{	
				pid2.max = 45000;
			}
		}
		else if(pid2.err >10)//后面温度升的快 温差小的时候限幅值小一点
		{
			if(pid2.ref <80)
			{
				pid2.max = 30000;
			}
			if(pid2.ref <120)
			{
				pid2.max = 40000;
			}
			else
			{	
				pid2.max = 45000;
			}
			
		}
		else if(pid2.err >5){
			if(pid2.ref <80)
			{
				pid2.max = 20000;
			}
			else if(pid2.ref <120)
			{
				pid2.max = 40000;
			}
			else
			{	
				pid2.max = 45000;
			}
		}
		else if(pid2.err >2){
			if(pid2.ref <80)
			{
				pid2.max = 10000;
			}
			if(pid2.ref <120)
			{
				pid2.max = 30000;
			}
			else
			{	
				pid2.max = 45000;
			}
		}
		else
		{
			pid2.Integ = 2000	;
			if(pid2.ref <80)
			{
				pid2.max = 10000;
			}
			if(pid2.ref <120)
			{
				pid2.max = 25000;
			}
			else
			{	
				pid2.max = 45000;
			}
		}	
		if(pid2.err < 0) 
		{
			pid2.Integ = 0;
			pid2.out = 0;
		}
		else
		{
			 pid2.out = pid2.Integ+ pid2.out;
		}
   
    if(pid2.out >= pid2.max)
    {
        pid2.out = pid2.max;
    }
    else if(pid2.out<=pid2.min)
    {
        pid2.out = pid2.min;
    }
    pid2.err_pre = pid2.err;
    pid2.err_pre_pre = pid2.err_pre;
}


void PID3_Calc()  //pid计算
{
		pid3.err = pid3.ref - pid3.fd;//ref?????????,fd?????????? 
    pid3.Integ = (pid3.kp + pid3.ki) * pid3.err - (pid3.kp * pid3.err_pre);
//    p->Integ = (p->kp + p->ki + p->kd) * p->err - (p->kp + 2 * p->kd) * p->err_pre + p->kd * p->err_pre_pre; 

//		if(pid3.err >15)//修改out初值  修改错误
//		{
//			if(pid3.ref <120)
//			{
//				pid3.max = 40000;
//			}
//			else
//			{	
//				pid3.max = 45000;
//			}
//		}
//		else if(pid3.err >10)//后面温度升的快 温差小的时候限幅值小一点
//		{
//			if(pid3.ref <80)
//			{
//				pid3.max = 30000;
//			}
//			if(pid3.ref <120)
//			{
//				pid3.max = 40000;
//			}
//			else
//			{	
//				pid3.max = 45000;
//			}
//			
//		}
//		else if(pid3.err >5){
//			if(pid3.ref <80)
//			{
//				pid3.max = 20000;
//			}
//			else if(pid3.ref <120)
//			{
//				pid3.max = 30000;
//			}
//			else
//			{	
//				pid3.max = 40000;
//			}
//		}
//		else if(pid3.err >2){
//			if(pid3.ref <80)
//			{
//				pid3.max = 10000;
//			}
//			if(pid3.ref <120)
//			{
//				pid3.max = 20000;
//			}
//			else
//			{	
//				pid3.max = 30000;
//			}
//		}
//		else
//		{
//			pid3.Integ = 2000	;
//			if(pid3.ref <80)
//			{
//				pid3.max = 10000;
//			}
//			if(pid3.ref <120)
//			{
//				pid3.max = 20000;
//			}
//			else
//			{	
//				pid3.max = 30000;
//			}
//			
//		}
if(pid3.err >15)//修改out初值
		{
			if(pid3.ref <120)
			{
				pid3.max = 40000;
			}
			else
			{	
				pid3.max = 45000;
			}
		}
		else if(pid3.err >10)//后面温度升的快 温差小的时候限幅值小一点
		{
			if(pid3.ref <120)
			{
				pid3.max = 30000;
			}
			else
			{	
				pid3.max = 40000;
			}
			
		}
		else if(pid3.err >5){
			
			if(pid3.ref <120)
			{
				pid3.max = 20000;
			}
			else
			{	
				pid3.max = 30000;
			}
		}
		else if(pid3.err >2){
			
			if(pid3.ref <120)
			{
				pid3.max = 10000;
			}
			else
			{	
				pid3.max = 20000;
			}
		}
		else
		{
			pid3.Integ = 2000	;
			if(pid3.ref <120)
			{
				pid3.max = 10000;
			}
			else
			{	
				pid3.max = 20000;
			}
			
		}
		if(pid3.err < 0) 
		{
			pid3.Integ = 0;
			pid3.out = 0;
		}
		else
		{
			 pid3.out = pid3.Integ+ pid3.out;
		}
   
    if(pid3.out >= pid3.max)
    {
        pid3.out = pid3.max;
    }
    else if(pid3.out<=pid3.min)
    {
        pid3.out = pid3.min;
    }
    pid3.err_pre = pid3.err;
    pid3.err_pre_pre = pid3.err_pre;
}

void PID_out()  //输出PID运算结果到负载---每1ms被调用1次
{
//   static uint16_t pw;
//	 pw++;
//	 if(pw>=pid1.pwmcycle)  //
//	 {
//	   pw=0;
//	 }
//	  //0  ~  pid.pwmcycle-1
//	 
//	 if(pw<pid1.OUT)
//	 {
//	   //pwmout_0;//加热
//	 }
//	 else
//	 {
//	   //pwmout_1;//停止加热
//	 }
	 
}



void PID_Calc_overshoot()  //pid计算
{
// float DelEk;
//	float ti,ki;
////	float Iout;
////	float Pout;
////	float Dout;
//	float td;
//	float kd;
//	float SvSave;
//	
//	
//	
//	
// SvSave=pid1.Sv;
// 
//// if(pid1.Pv<(SvSave*0.55))//温度达到计数开始 66度
//// {
////    pid1.times=0;
//// }
//// 
//  if(pid1.times<=300)//3分钟 72度  100s
// {
//  pid1.Sv=SvSave*0.6;
// }
// 
// if((pid1.times>300)&&(pid1.times<=320))//3分钟 96度  100s
// {
//  pid1.Sv=SvSave*0.8;
// }
// 
// if((pid1.times>320)&&(pid1.times<=340))//2分钟 102度 100s
// {
//  pid1.Sv=SvSave*0.85;
// }
// 
// if((pid1.times>340)&&(pid1.times<=360))//2分钟 108度 100s
// {
//  pid1.Sv=SvSave*0.9;
// }
// 
//  if((pid1.times>360)&&(pid1.times<=380))//2分钟 114度 100s
// {
//  pid1.Sv=SvSave*0.95;
// }
// 
// if(pid1.times>400)
// {
//  pid1.Sv=SvSave;
//	 //pid1.times = 0;
// }
// 
// 
// pid1.times++; //150ms加一次
// 
// 

// pid1.Ek=pid1.Sv-pid1.Pv;   //得到当前的偏差值
// pid1.Pout=pid1.Kp*pid1.Ek;      //比例输出
// 
// pid1.SEk+=pid1.Ek;        //历史偏差总和
// 
// DelEk=pid1.Ek-pid1.Ek_1;  //最近两次偏差之差
// 
// ti=pid1.T/pid1.Ti;
// ki=ti*pid1.Kp;
// 
//  pid1.Iout=ki*pid1.SEk*pid1.Kp;  //积分输出

// td=pid1.Td/pid1.T;
// 
// kd=pid1.Kp*td;
// 
//  pid1.Dout=kd*DelEk;    //微分输出
// 
// out1= pid1.Pout+ pid1.Iout+ pid1.Dout;
// 
// //////////////////////////////////////////////////////////
// 
// if(out1>(pid1.pwmcycle))
// {
//  pid1.OUT=pid1.pwmcycle;
// }
// else if(out1<0)
// {
//  pid1.OUT=pid1.OUT0; 
// }
// else 
// {
//  pid1.OUT=out1;
// }
// //pid.OUT+=; //本次的计算结果
// pid1.Ek_1=pid1.Ek;  //更新偏差
// pid1.Sv=SvSave;
 
}

int cmd_rx(uint8_t *rxbuf,uint8_t rx_len)
{
	if(rxbuf[0]=='P'&&rxbuf[1]=='I'&&rxbuf[2]=='D')
	{
		//用','分割字符串，得到两个内容为浮点数的字符串
		char out[100];
		char* token[3];//存放字符串分割结果
		float para[3];//存放参数
		int id = rxbuf[3] - '0';
		switch(id)
		{
			case 1:
					if(rxbuf[4] == '=')
					{
						token[0]=strtok((char*)(rxbuf+4),",");
						token[1]=strtok(NULL,",");	//继续分割
						token[2]=strtok(NULL,",");	//继续分割
						if(!(strtok(NULL,",")==NULL))	//还没分完，说明字符串有错误
								return 1;
						//字符串转换为浮点数
						para[0]=atof(token[0]);
						para[1]=atof(token[1]);
						para[2]=atof(token[2]);
					}
					else if(rxbuf[4] == '?')
					{
						sprintf(out,
						"/PID1 CONFIG/\r\n"
						"|P:%f I:%f D:%f|\r\n",
						pid1.kp,
						pid1.ki,
						pid1.kd);
//						pid1.Kp,
//						pid1.Ki,
//						pid1.Kd);
					}
					else if(rxbuf[4] == '#')
					{
						//保存PID
					}
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
			

	}
	
	
}