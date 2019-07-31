#include "filter.h"
#include "bsp_qmc5883l.h"
#include "math.h"
MPV_Struct MPV;
MANETIC_TYPEDEF  manetic;
unsigned char MP_index,MV_index;
 uint32_t  LastTick;
	signed long int tmp2,tmp3; 
long int FloatAverage(unsigned int * tpbuff,unsigned char SumCount)
{
        unsigned char i;
	signed long int tmp;
	tmp =0;

	for(i = 0;i<SumCount/2;i++)
		tmp = tmp + tpbuff[i];
	       tmp = tmp /SumCount/2;
	for(i = (SumCount/2-1);i<SumCount;i++)
		tmp = tmp + tpbuff[i];
	       tmp = tmp /SumCount/2;
      return tmp;
}

long int IntAverage(short int * tpbuff,unsigned char SumCount)
{
        unsigned char i;
	signed long int tmp;
	tmp =0;
	for(i = 0;i<SumCount/2;i++)
		tmp = tmp + tpbuff[i];
	       tmp = tmp /SumCount/2;
	for(i = (SumCount/2-1);i<SumCount;i++)
		tmp = tmp + tpbuff[i];
	       tmp = tmp /SumCount/2;
      return tmp;
}
long int MaxOrMin(MPV_Struct t,unsigned int count,unsigned char flag)
{
      signed long int result;
	unsigned char i,j;
	if(flag == 0)
	{
		for(j=0;j<3;j++)
		{
				result = t.Mp[j][0];
			for(i=0;i<count;i++)
			{
				if(result<t.Mp[j][i])
					result = t.Mp[j][i];
			}		
		}

	}
	else
	{
		for(j=0;j<3;j++)
		{
			 result =  t.Mv[j][0];
			for(i=0;i<count;i++)
			{
				if(result>t.Mv[j][i])
					result = t.Mv[j][i];	
			}		
		}

		
	}
	return result;
}

void manetic_filter(signed short*data)
{
        unsigned char i;
	// QMC5883L_ReadData();	
	for(i=0;i<3;i++)
	{
		manetic.S[i][manetic.sample_index]=data[i];	
	}
	if(manetic.sample_index < N)//cal A(k) when k<N
	{
		for(i=0;i<3;i++)
		{
			manetic.A[i][manetic.sample_index]=
			IntAverage(&manetic.S[i][0], manetic.sample_index+1);
		}
	}
	else//cal A(k) when k>=N
	{
		for(i=0;i<3;i++)
		{

			manetic.A[i][manetic.sample_index]=
			IntAverage(&manetic.S[i][manetic.sample_index+1-N], N);
		}
	}	
	//manetic.sample_index = manetic.sample_index + 1;


}

void InitBaseLine()
{
	 unsigned char i,j;
	 uint32_t LastTick;
	 LastTick = 0;
	while(manetic.sample_index<50)
	{
		if(HAL_GetTick()-LastTick>=10)
		{
			manetic_filter(QMC5883L_ReadData());
			LastTick =HAL_GetTick();
			for(i=0;i<3;i++)
			{
				manetic.B[i][manetic.sample_index]=FloatAverage(&manetic.A[i][0], manetic.sample_index+1);
			}
			manetic.sample_index++;
		}
	}
	//manetic.sample_index = 49;
}
void CarDetect()
{
	 unsigned int  i,j;
	 signed long int tmp;
	for(i=0;i<3;i++)
	{
	      switch(i)
	      	{
			case 0: tmp = THRES_X;break;
			case 1: tmp = THRES_Y;break;
			case 2: tmp = THRES_Z;break;
			
		  }
		if((manetic.S[i][manetic.sample_index]-
			manetic.B[i][manetic.sample_index])>tmp)//event check start
		{
			
			if(MPV.STimes<UPDATE_MAX_TIME)
				MPV.STimes = MPV.STimes +1;
			else  //event start
			{
				if(MPV.UpdateFlag == 0)
					MPV.Ts = HAL_GetTick() -LastTick;
				MPV.UpdateFlag = 1;
				MPV.Mp[i][MP_index] = manetic.S[i][manetic.sample_index];
				MPV.Tp[i][MP_index++] = HAL_GetTick() -LastTick;
			}
		}//event check end
		else if(fabs(manetic.S[i][manetic.sample_index]-
			manetic.B[i][manetic.sample_index])<=tmp)
		{
			if(MPV.STimes<UPDATE_MAX_TIME)//no event 
			{	
			       MPV.STimes = 0;
			       MP_index = 0;
			       MV_index = 0;
			      MPV.UpdateFlag = 0;
			}
			else  //event sample end
			{
				MPV.Eimes = MPV.Eimes + 1;
				if(MPV.Eimes >= UPDATE_MAX_TIME)
				{
				   	MPV.Te= HAL_GetTick() -LastTick;
					MPV.EventEndFalg = 1;
				}

			      // MPV.STimes = 0;
			      // MP_index = 0;
			      // MV_index = 0;
			      //MPV.UpdateFlag = 0;
			      //MPV.Eimes = 0;
			}
			
		}
		if((manetic.B[i][manetic.sample_index])-
			manetic.S[i][manetic.sample_index]>tmp)
		{
			if(MPV.UpdateFlag == 1)
			{
				MPV.Mv[i][MV_index] = manetic.S[i][manetic.sample_index];
				MPV.Tv[i][MV_index++] = HAL_GetTick() -LastTick;
			}
		}

	      if(MPV.EventEndFalg  ==1)
	      	{
			MPV.Mp_MAX = MaxOrMin(MPV,MP_index-1,0);
			MPV.Mv_MIN = MaxOrMin(MPV,MV_index-1,1);

			if(MP_index<MV_index)
			{
				for(j=0;j<MV_index;j++)
				{
					if(fabs(MPV.Mp[i][j]-MPV.Mv[i][j])<(MPV.Mp_MAX-MPV.Mv_MIN )*0.1)
					{
						
					}
					else 
					{
						if(((MPV.Tv[i][j]-MPV.Tp[i][j])>(MPV.Te-MPV.Ts)*0.5)&&(MPV.Mp_MAX - MPV.Mv_MIN)>TD)
							MPV.CarFLag = 1;
					}
				}
			}
			else
			{
				for(j=0;j<MV_index;j++)
				{
					if(fabs(MPV.Mp[i][j]-MPV.Mv[i][j])<(MPV.Mp_MAX-MPV.Mv_MIN )*0.1)
					{
						
					}
					else 
					{
						if(((MPV.Tv[i][j]-MPV.Tp[i][j])>(MPV.Te-MPV.Ts)*0.5)&&(MPV.Mp_MAX - MPV.Mv_MIN)>TD)
							MPV.CarFLag = 1;
					}
				}
			}

		       MPV.STimes = 0;
		       MP_index = 0;
		       MV_index = 0;
		      MPV.UpdateFlag = 0;
		      MPV.Eimes = 0;
		     MPV.Ts = 0;
		     MPV.Te = 0;
		     MPV.EventEndFalg = 0;
		     MPV.Mp_MAX = 0;	
		     MPV.Mv_MIN = 0;				 
	
			
		 }
		
		if(MP_index>(BUFFERSIZE/2))
		{
				MP_index = 0;
		}
		if(MV_index>(BUFFERSIZE/2))
		{
				MV_index = 0;
		}
	}
}
	signed long int t1,t2,t3;
void TraceBasLine()
{
	 unsigned char i;
	 long int  tmp;

		for(i=0;i<3;i++)//trace base line start
		{    
		      switch(i)
		      	{
				case 0: tmp = THRES_X;break;
				case 1: tmp = THRES_Y;break;
				case 2: tmp = THRES_Z;break;
				
			  }
				 if(manetic.sample_index == 0)
				 {
					 if(abs(manetic.B[i][BUFFERSIZE-1]-
							manetic.S[i][manetic.sample_index]) >tmp)
						 manetic.B[i][manetic.sample_index] =
												manetic.B[i][BUFFERSIZE-1];
					else
					{
							
						 manetic.B[i][manetic.sample_index] =
								manetic.B[i][BUFFERSIZE-1]*(1-ALPHA)+
								manetic.A[i][manetic.sample_index]*(ALPHA);
					}				 
				 }
				 else
				 {
					  t1 = manetic.B[i][manetic.sample_index-1];
					  t2 =manetic.S[i][manetic.sample_index];
					 if(abs(t1-t2)
							 >tmp)
						 manetic.B[i][manetic.sample_index] =
												manetic.B[i][manetic.sample_index-1];
					else
					{
							
						 manetic.B[i][manetic.sample_index] =
												manetic.B[i][manetic.sample_index-1]*(1-ALPHA)+
												manetic.A[i][manetic.sample_index-1]*(ALPHA);
												;
					}				 
				 }

			 }//trace base line end
}
void SupvisorProcess()
{

	// LastTick = 0;

	if(HAL_GetTick()-LastTick>=SAMPLE_CYCLE)
	{ HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		manetic_filter(QMC5883L_ReadData());
		LastTick =HAL_GetTick();
		TraceBasLine();
		CarDetect();
		manetic.sample_index = manetic.sample_index + 1;
	}
	if(manetic.sample_index >= BUFFERSIZE)
		manetic.sample_index = 0;
}
