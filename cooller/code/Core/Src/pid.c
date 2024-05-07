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
    Error = NextPoint-pp->SetPoint; // ƫ��
    if(pp->LastError==0)
        pp->LastError = Error;
    pp->NextPoint = NextPoint;
    if( pp->SumError>=PID_SI_MAX)
    {
       // if(NextPoint>(pp->SetPoint+PID_O_MAX))    //�����һ�����������������ı���
        {
            tmp = fabs(Error);
            if(tmp>PID_I_MAX)
            {
                index = 0;//ƫ�����ֲ�������
            }
            else
            {
                index = 1;
                if(Error<0)
                {
                    pp->SumError += Error;  //������ֻ���ָ�ƫ��
                }
            }
        }
    }
    else if( pp->SumError<=PID_SI_MIN)
    {
       // if(NextPoint<(pp->SetPoint-PID_O_MAX))  //�����һ������������ָ���ı���
        {
            tmp = fabs(Error);
            if(tmp>PID_I_MAX)
            {
                index = 0;//ƫ�����ֲ�������
            }
            else
            {
                index = 1;
                if(Error>0)
                {
                    pp->SumError += Error;	//������ֻ���ָ�ƫ��

                }
            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //���ַ����PID�Ż����ɲο�����������
        {
            index = 0;//ƫ�����ֲ�������
        }
        else
        {
            index = 1;
            pp->SumError += Error;	//������ֻ���ָ�ƫ��

        }
    }

    dError = pp->LastError - pp->PrevError; //  ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->iError = Error;
    pp->LastError = Error;
	pp->result =(pp->Proportion * Error // ������
				 +index*pp->Integral * pp->SumError // ������
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
    return  pp->result;        // ΢����
}



/*====================================================================================================
PID�ṹ�������ʼ������
=====================================================================================================*/
void PIDInit (PID *pp)
{
    memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
��ȡ��������������ڴ��趨Ϊ�̶�ֵ100��
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


//������

uint32_t pid_proc(double rIn)
{

    sPID.rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    sPID.rOut = PIDCalc ( &sPID,sPID.rIn_PID );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)( sPID.rOut );			   // ����������ƺ���(Effect Needed Changes)
}
uint32_t pid_proc_fan_heat(unsigned char heater_num,double rIn)
{
    if(heater_num == 1)
    	{
    sPID_h1.rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    sPID_h1.rOut = PIDCalc ( &sPID_h1, sPID_h1.rIn_PID_pump );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)(  sPID_h1.rOut_pump );			   // ����������ƺ���(Effect Needed Changes)

	}
    else if(heater_num == 2)
    	{
    sPID_h2.rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    sPID_h2.rOut = PIDCalc ( &sPID_h2, sPID_h2.rIn_PID_pump );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)(  sPID_h2.rOut_pump );			   // ����������ƺ���(Effect Needed Changes)

	}
    else if(heater_num == 3)
    	{
    sPID_h3.rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    sPID_h3.rOut = PIDCalc ( &sPID_h3, sPID_h3.rIn_PID_pump );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)(  sPID_h3.rOut_pump );			   // ����������ƺ���(Effect Needed Changes)

	}
    else if(heater_num == 4)
    	{
    sPID_h4.rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    sPID_h4.rOut = PIDCalc ( &sPID_h4, sPID_h4.rIn_PID_pump );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)(  sPID_h4.rOut_pump );			   // ����������ƺ���(Effect Needed Changes)

	}

}


