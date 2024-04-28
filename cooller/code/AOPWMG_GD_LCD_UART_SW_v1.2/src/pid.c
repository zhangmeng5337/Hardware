#include "pid.h"
#include<string.h>
#include<stdio.h>
#include "sensor.h"
#include<math.h>

PID sPID,sPID_pump; // PID Control Structure
double rOut,rOut_pump; // PID Response (Output)
double rIn_PID,rIn_PID_pump; // PID Feedback (Input)

float tmp;

//Error_stru *sptr;
/*!
 *  @brief      ����ʽPID
 *  @since      v1.0
 *  *sptr ��������
 *  *pid:  PID����
 *  NowPlace��ʵ��ֵ
 *  Point��   ����ֵ
 */
// ����ʽPID�������
#if PID_TYPE == 1
double PIDCalc( PID *pp, double NextPoint )
{
    double dError,result,res_tmp;
    float Error;
    static double last_result,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // ƫ��
    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //�����һ�����������������ı���
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            //index = 1;
            if(Error<0)
            {
                pp->SumError += Error;  //������ֻ���ָ�ƫ��
            }
        }
    }
    else if(NextPoint<(pp->SetPoint-PID_O_MAX))  //�����һ������������ָ���ı���
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            // index = 1;
            if(Error>0)
            {
                pp->SumError += Error;	//������ֻ���ָ�ƫ��

            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //���ַ����PID�Ż����ɲο�����������
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            ;//index = 1;
            pp->SumError += Error;	//������ֻ���ָ�ƫ��

        }
    }

    dError = pp->LastError - pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // ������
             +pp->Integral * pp->SumError // ������
             + pp->Derivative * dError );



    if(result <=5000)
    {
        result = 5000;
    }


    if(result>=MAX_OUTPUT)
    {
        result = MAX_OUTPUT;
        // integ =  0;
    }
    pp->result = result;
    return result;        // ΢����
}
#endif
#if PID_TYPE == 2
double PIDCalc( PID *pp, double NextPoint )
{
    double dError;//,res_tmp;
    float Error;
//    static double last_result;//,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // ƫ��
    if(pp->LastError==0)
        pp->LastError = Error;
    pp->NextPoint = NextPoint;
    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //�����һ�����������������ı���
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
    else if(NextPoint<(pp->SetPoint-PID_O_MAX))  //�����һ������������ָ���ı���
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



    if( Error<0)
    {
        if(pp->LastError>Error)
        {
            pp->dir_cnt_pos++;//ʵ��ֵ����
            pp->dir_cnt_neg=0;

        }

        else
        {
            pp->dir_cnt_neg++;//ʵ��ֵ��С
            pp->dir_cnt_pos=0;

        }
        if(pp->dir_cnt_neg>255)
            pp->dir_cnt_neg=255;
        if(pp->dir_cnt_pos>255)
            pp->dir_cnt_pos=255;


    }
    else
    {
        pp->dir_cnt_pos=0;
        pp->dir_cnt_neg=0;

    }

    dError = pp->LastError - pp->PrevError; //  ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->iError = Error;
    pp->LastError = Error;
    /*      pp->result =(pp->Proportion * Error // ������
                       +pp->Integral * pp->SumError // ������
                       + pp->Derivative * dError );*/
    if(pp->dir_cnt_neg>=3)//�����ͺ󣬽��͵�һ����Χ����������Ҫ����
    {
        if( Error<-150&&Error>=-100)
        {
            pp->result =  pp->result+10;
        }
        else
        {

            pp->result =(pp->Proportion * Error // ������
                         +index*pp->Integral * pp->SumError // ������
                         + pp->Derivative * dError );

        }
    }
    else
    {
        if(Error>=300)
        {
            pp->result = MAX_OUTPUT;

        }
        else
        {

            pp->result =(pp->Proportion * Error // ������
                         +index*pp->Integral * pp->SumError // ������
                         + pp->Derivative * dError );

        }
    }



    if( pp->result<=10000)//19900  15000
    {
//        if(Error<-100)
//            pp->result = 17000;
//				else
//            pp->result = 17000;
        // if( pp->result<17000)
        pp->result = 10000;
        // integ =  0;
    }

    if( pp->result>=MAX_OUTPUT)
    {
        pp->result = MAX_OUTPUT;
        // integ =  0;
    }
    //result = 27000;

//   pp->result = 23000;
    return  pp->result;        // ΢����
}
double PIDCalc1( PID *pp, double NextPoint )
{
    double dError;//,res_tmp;
    float Error;
//    static double last_result;//,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // ƫ��
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
        pp->result = MAX_OUTPUT;
    }
    return  pp->result;        // ΢����
}

#endif
#if PID_TYPE == 0
/*====================================================================================================
PID���㺯��
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
    static double last_result,integ;

    Error = pp->SetPoint - NextPoint; // ƫ��
    pp->SumError += Error; // ����
    dError = pp->LastError - pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // ������
             +integ * pp->SumError // ������
             + pp->Derivative * dError );

    if(result>last_result)
    {
        res_tmp = result-last_result;
        if((res_tmp>0&&res_tmp>=2000))//2000
        {
            last_result=last_result+10;
            result = last_result;
        }
    }
    else
    {
        res_tmp = last_result-result;
        if((res_tmp>=2000))//2000
        {
            last_result=last_result-10;
            result = last_result;
        }
    }

    if(result <0)
    {
        result = 0;
        integ =  0;
    }
    else
        integ =  pp->Integral;

    if(result>=MAX_OUTPUT)
    {
        result = MAX_OUTPUT;
        integ =  0;
    }
    else
        integ =  pp->Integral;
    pp->result = result;
    return result;        // ΢����
}
#endif
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


/*====================================================================================================
����������ƺ���
======================================================================================================*/
void actuator(double rDelta)
{
}
void pid_init_flow(float setvalue)
{


    //PIDInit ( &sPID ); // Initialize Structure
    pid_init( setvalue);
    /* sPID.Proportion =260; // Set PID Coefficients  260
     sPID.Integral = 0.0003;
     sPID.Derivative = 0.0001;
     sPID.SetPoint = setvalue; // Set PID Setpoint
     sPID.LastError = 0;
     sPID.PrevError = 0;
     sPID.SumError = 0;
     sPID.result = 3000;*/
}

#if PID_TYPE == 1
void pid_init(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =380; // Set PID Coefficients  260
    sPID.Integral = 0.95;//0.95;//
    sPID.Derivative = 0.8;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
#endif
#if PID_TYPE == 2
void pid_init(float setvalue)
{

    // PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =185; // Set PID Coefficients  185 260
    sPID.Integral = 0.003;//0.95;//
    sPID.Derivative = 0.004;
    // sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
//    // PIDInit ( &sPID ); // Initialize Structure
//    sPID.Proportion =185; // Set PID Coefficients  185 260
//    sPID.Integral = 0.003;//0.95;//
//    sPID.Derivative = 0.0004;
//    // sPID.result = 0;
//    sPID.SetPoint = setvalue; // Set PID Setpoint
////   sPID.LastError = 0;
////	sPID.PrevError = 0;
////	sPID.SumError = 0;
}
void pid_init2(float setvalue)
{


    // PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =185; // Set PID Coefficients  260
    sPID.Integral = 0.003;//0.95;//
    sPID.Derivative = 0.0004;
    //  sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
    sPID.PrevError = 0;
    sPID.SumError = 0;
}

#endif
#if PID_TYPE == 0
void pid_init(float setvalue)
{

    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;//
    sPID.Derivative = 0.0001;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
#endif
double PIDCalc_pump( PID *pp, double NextPoint )
{
    double dError;//,res_tmp;
    float Error;
//    static double last_result;//,integ;
    // static unsigned char index;
    Error = (pp->SetPoint - NextPoint)*1000; // ƫ��
    if(pp->LastError==0)
        pp->LastError = Error;
    pp->NextPoint = NextPoint;
    if(NextPoint>(pp->SetPoint+PID_O_MAX))	  //�����һ�����������������ı���
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            //index = 1;
            if(Error<0)
            {
                pp->SumError += Error;	//������ֻ���ָ�ƫ��
            }
        }
    }
    else if(NextPoint<(pp->SetPoint-PID_O_MAX))  //�����һ������������ָ���ı���
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            // index = 1;
            if(Error>0)
            {
                pp->SumError += Error;	//������ֻ���ָ�ƫ��

            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)	  //���ַ����PID�Ż����ɲο�����������
        {
            ;//index = 0;//ƫ�����ֲ�������
        }
        else
        {
            // index = 1;
            pp->SumError += Error;	//������ֻ���ָ�ƫ��

        }
    }



    if( Error<0)
    {
        if(pp->LastError>Error)
        {
            pp->dir_cnt_pos++;//ʵ��ֵ����
            pp->dir_cnt_neg=0;

        }

        else
        {
            pp->dir_cnt_neg++;//ʵ��ֵ��С
            pp->dir_cnt_pos=0;

        }
        if(pp->dir_cnt_neg>255)
            pp->dir_cnt_neg=255;
        if(pp->dir_cnt_pos>255)
            pp->dir_cnt_pos=255;


    }
    else
    {
        pp->dir_cnt_pos=0;
        pp->dir_cnt_neg=0;

    }

    dError = pp->LastError - pp->PrevError; //	��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->iError = Error;
    pp->LastError = Error;
    /*	  pp->result =(pp->Proportion * Error // ������
    				 +pp->Integral * pp->SumError // ������
    				 + pp->Derivative * dError );*/
    if(pp->dir_cnt_neg>=3)//�����ͺ󣬽��͵�һ����Χ����������Ҫ����
    {
        if( Error<-150&&Error>=-100)
        {
            pp->result =  pp->result+10;
        }
        else
        {

            pp->result =(pp->Proportion * Error // ������
                         +pp->Integral * pp->SumError // ������
                         + pp->Derivative * dError );

        }
    }
    else
    {
        if(Error>=300)
        {
            pp->result = MAX_OUTPUT;

        }
        else
        {

            pp->result =(pp->Proportion * Error // ������
                         +pp->Integral * pp->SumError // ������
                         + pp->Derivative * dError );

        }
    }



    if( pp->result<=5000)
    {
        //		  if(Error<-100)
        //			  pp->result = 17000;
        //				else
        //			  pp->result = 17000;
        // if( pp->result<17000)
        pp->result = 5000;
        // integ =	0;
    }

    if( pp->result>=MAX_OUTPUT)
    {
        pp->result = MAX_OUTPUT;
        // integ =	0;
    }
    //result = 27000;

    //   pp->result = 23000;
    return	pp->result; 	   // ΢����
}




//������

uint32_t pid_proc(double rIn)
{

    rIn_PID = rIn;				 // ��ȡ�����������(Read Input )
    rOut = PIDCalc1 ( &sPID,rIn_PID );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)( rOut );			   // ����������ƺ���(Effect Needed Changes)


}
uint32_t pid_proc_pump(double rIn)
{

    // PIDInit ( &sPID ); // Initialize Structure
    sPID_pump.Proportion =185; // Set PID Coefficients  260
    sPID_pump.Integral = 0.003;//0.95;//
    sPID_pump.Derivative = 0.0004;
    //  sPID.result = 0;
    sPID_pump.SetPoint = SET_FLOW; // Set PID Setpoint
    //sPID.LastError = 0;
    //sPID.PrevError = 0;
    //sPID.SumError = 0;
    rIn_PID_pump = rIn;				 // ��ȡ�����������(Read Input )
    rOut_pump = PIDCalc_pump ( &sPID_pump,rIn_PID_pump );   // PID���㺯��(Perform PID Interation)
    //rOut_pump = 65535;
    return (uint32_t)( rOut_pump );			   // ����������ƺ���(Effect Needed Changes)


}

