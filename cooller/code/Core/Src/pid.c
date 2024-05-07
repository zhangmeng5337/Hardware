#include "pid.h"
#include<string.h>
#include<stdio.h>
#include<math.h>

PID sPID;
PID sPID_h1; // PID Control Structure
PID sPID_h2; // PID Control Structure
PID sPID_h3; // PID Control Structure
PID sPID_h4; // PID Control Structure




float tmp;
double PIDCalc( PID *pp, double NextPoint )
{
    double dError;//,res_tmp;
    float Error;
//    static double last_result;//,integ;
    static unsigned char index;
    Error = NextPoint-pp->SetPoint; // 偏差
    if(pp->LastError==0)
        pp->LastError = Error;
    pp->NextPoint = NextPoint;
    if( pp->SumError>=PID_SI_MAX)
    {
       // if(NextPoint>(pp->SetPoint+PID_O_MAX))    //如果上一次输出变量出现正向的饱和
        {
            tmp = fabs(Error);
            if(tmp>PID_I_MAX)
            {
                index = 0;//偏差大积分不起作用
            }
            else
            {
                index = 1;
                if(Error<0)
                {
                    pp->SumError += Error;  //正饱和只积分负偏差
                }
            }
        }
    }
    else if( pp->SumError<=PID_SI_MIN)
    {
       // if(NextPoint<(pp->SetPoint-PID_O_MAX))  //如果上一次输出变量出现负向的饱和
        {
            tmp = fabs(Error);
            if(tmp>PID_I_MAX)
            {
                index = 0;//偏差大积分不起作用
            }
            else
            {
                index = 1;
                if(Error>0)
                {
                    pp->SumError += Error;	//正饱和只积分负偏差

                }
            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //积分分离的PID优化，可参考以往的文章
        {
            index = 0;//偏差大积分不起作用
        }
        else
        {
            index = 1;
            pp->SumError += Error;	//正饱和只积分负偏差

        }
    }

    dError = pp->LastError - pp->PrevError; //  当前微分
    pp->PrevError = pp->LastError;
    pp->iError = Error;
    pp->LastError = Error;
	pp->result =(pp->Proportion * Error // 比例项
				 +index*pp->Integral * pp->SumError // 积分项
				 + pp->Derivative * dError );




    if( pp->result<=8000)//19900  15000
    {
        pp->result = 8000;
    }

    if( pp->result>=MAX_OUTPUT)
    {
       float fabs_err;
	   fabs_err =fabs(Error);
       if(fabs_err>=5)
        pp->result = MAX_OUTPUT+1;
    }
    return  pp->result;        // 微分项
}



/*====================================================================================================
PID结构体变量初始化函数
=====================================================================================================*/
void PIDInit (PID *pp)
{
    memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
读取输入变量函数（在此设定为固定值100）
======================================================================================================*/




void pid_init(float setvalue,float heat_T)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =380; // Set PID Coefficients  260
    sPID.Integral = 0.95;//0.95;//
    sPID.Derivative = 0.8;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    PIDInit ( &sPID_h1); // Initialize Structure
    sPID_h1.Proportion =380; // Set PID Coefficients  260
    sPID_h1.Integral = 0.95;//0.95;//
    sPID_h1.Derivative = 0.8;
    sPID_h1.result = 0;
    sPID_h1.SetPoint = heat_T; // Set PID Setpoint
    PIDInit ( &sPID_h3); // Initialize Structure
    sPID_h3.Proportion =380; // Set PID Coefficients  260
    sPID_h3.Integral = 0.95;//0.95;//
    sPID_h3.Derivative = 0.8;
    sPID_h3.result = 0;
    sPID_h3.SetPoint = heat_T; // Set PID Setpoint

    PIDInit ( &sPID_h2); // Initialize Structure
    sPID_h2.Proportion =380; // Set PID Coefficients  260
    sPID_h2.Integral = 0.95;//0.95;//
    sPID_h2.Derivative = 0.8;
    sPID_h2.result = 0;
    sPID_h2.SetPoint = heat_T; // Set PID Setpoint

    PIDInit ( &sPID_h4); // Initialize Structure
    sPID_h4.Proportion =380; // Set PID Coefficients  260
    sPID_h4.Integral = 0.95;//0.95;//
    sPID_h4.Derivative = 0.8;
    sPID_h4.result = 0;
    sPID_h4.SetPoint = heat_T; // Set PID Setpoint

}


//主函数

uint32_t pid_proc(double rIn)
{

    sPID.rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    sPID.rOut = PIDCalc ( &sPID,sPID.rIn_PID );   // PID计算函数(Perform PID Interation)
    return (uint32_t)( sPID.rOut );			   // 输出变量控制函数(Effect Needed Changes)
}
uint32_t pid_proc_fan_heat(unsigned char heater_num,double rIn)
{
    if(heater_num == 1)
    	{
    sPID_h1.rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    sPID_h1.rOut = PIDCalc ( &sPID_h1, sPID_h1.rIn_PID_pump );   // PID计算函数(Perform PID Interation)
    return (uint32_t)(  sPID_h1.rOut_pump );			   // 输出变量控制函数(Effect Needed Changes)

	}
    else if(heater_num == 2)
    	{
    sPID_h2.rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    sPID_h2.rOut = PIDCalc ( &sPID_h2, sPID_h2.rIn_PID_pump );   // PID计算函数(Perform PID Interation)
    return (uint32_t)(  sPID_h2.rOut_pump );			   // 输出变量控制函数(Effect Needed Changes)

	}
    else if(heater_num == 3)
    	{
    sPID_h3.rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    sPID_h3.rOut = PIDCalc ( &sPID_h3, sPID_h3.rIn_PID_pump );   // PID计算函数(Perform PID Interation)
    return (uint32_t)(  sPID_h3.rOut_pump );			   // 输出变量控制函数(Effect Needed Changes)

	}
    else if(heater_num == 4)
    	{
    sPID_h4.rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    sPID_h4.rOut = PIDCalc ( &sPID_h4, sPID_h4.rIn_PID_pump );   // PID计算函数(Perform PID Interation)
    return (uint32_t)(  sPID_h4.rOut_pump );			   // 输出变量控制函数(Effect Needed Changes)

	}

}


