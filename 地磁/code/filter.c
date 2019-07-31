#include "filter.h"
magnetic_str magnetic;

unsigned char getIndex()
{
  if(magnetic.index == 0)
  	return BUFFERSIZE - 1:
  else
  	return magnetic.index;
}

/************************************************************
 name :  short int Sum(unsigned char dir,index)
 params:
            @dir:
                   X_DIR
                   Y_DIR
                   Z_DIR
            @index: 当前数据缓冲区索引下标
return:

*************************************************************/
 short int Sum(unsigned char dir,unsigned char indexs,unsigned char indexe)
 {
       short int sum;
	unsigned char i;
	sum = 0;
	switch(dir)
	{
		case X_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[X_DIR][i] + sum;
			}
			sum = sum /(magnetic.index + 1);
		}break;
		case Y_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[Y_DIR][i] + sum;
			}
			sum = sum /(magnetic.index + 1);
		}break;
		case Z_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[Z_DIR][i] + sum;
			}
			sum = sum /(i);
		}break;
	}
	return sum;
 }
void MagneticGet(short int *pbuffer)
{
        unsigned char i;
	i = 0;
	magnetic.M[X_DIR][magnetic.index] =      pbuffer[0];
	magnetic.M[Y_DIR][magnetic.index] =      pbuffer[1];
	magnetic.M[Z_DIR][magnetic.index] =      pbuffer[2];
	magnetic.index++;

}
void void SmoothFilterInit()
{

}
void SmoothFilter()
{
	if(magnetic.index >= N)
	{
		magnetic.F[X_DIR][magnetic.index] = Sum(X_DIR,magnetic.index+1-N,magnetic.index);
		magnetic.F[Y_DIR][magnetic.index] = Sum(Y_DIR,magnetic.index+1-N,magnetic.index);
		magnetic.F[Z_DIR][magnetic.index] = Sum(Z_DIR,magnetic.index+1-N,magnetic.index);
	}
	else
	{
		magnetic.F[X_DIR][magnetic.index] = Sum(X_DIR,0,magnetic.index);
		magnetic.F[Y_DIR][magnetic.index] = Sum(Y_DIR,0,magnetic.index);
		magnetic.F[Z_DIR][magnetic.index] = Sum(Z_DIR,0,magnetic.index);		
	}
	//magnetic.index++;
	if(magnetic.index >= BUFFERSIZE)
		magnetic.index = 0;
}
void BaseLineTrace()
{

	  unsigned char index_tmp;
	  index_tmp = getIndex();

	if(magnetic.Car_Flag == 1)//x direction trance start
	{

		magnetic.B[X_DIR][magnetic.index] = magnetic.B[X_DIR][index_tmp-1];
		magnetic.B[Y_DIR][magnetic.index] = magnetic.B[Y_DIR][index_tmp-1];
		magnetic.B[Z_DIR][magnetic.index] = magnetic.B[Z_DIR][index_tmp-1];
	}
	else
	{
		  magnetic.B[X_DIR][magnetic.index] = magnetic.B[X_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                magnetic.M[X_DIR][index_tmp-1]*(ALPHA);
		  magnetic.B[Y_DIR][magnetic.index] = magnetic.B[Y_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                magnetic.M[Y_DIR][index_tmp-1]*(ALPHA);
		  magnetic.B[Z_DIR][magnetic.index] = magnetic.B[Z_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                magnetic.M[Z_DIR][index_tmp-1]*(ALPHA);


	}//x direction trance END
}
vehicle_detect()
{
    short int ztmp,ytmp,xtmp,sumtmp;
	unsigned char index_tmp;
	if(magnetic.M[Z_DIR][magnetic.index] > THRES)
		magnetic.Cnt_arr = magnetic.Cnt_arr + 1;
	else
	{
	    index_tmp = getIndex();
	    xtmp = magnetic.M[X_DIR][magnetic.index]-magnetic.M[X_DIR][index_tmp-1];
	    ytmp = magnetic.M[Y_DIR][magnetic.index]-magnetic.M[Y_DIR][index_tmp-1];
	    ztmp = magnetic.M[Z_DIR][magnetic.index]-magnetic.M[Z_DIR][index_tmp-1];
		sumtmp = xtmp + ytmp + ztmp;
		if(sumtmp < THRES)
		{
			if(TimingStart(NO_VEHICLE_TIME) == 1)
			{
				TimingStop();
				magnetic.Car_Flag = 0;
			}

		}

		else
			TimingStop();
		
		magnetic.Cnt_arr = 0;
	}
	if(magnetic.Cnt_arr >= SAMPLE_COUNT)
	{
		magnetic.Car_Flag = 1;
        magnetic.Cnt_arr = 0;
	}

}
void AdaptiveBaseLine()
{
  short int MinY,MinZ,MaxY,MaxZ;
  short int sumtmp;
  static unsigned char adaptive_count;
  if(magnetic.index==SAMPLE_COUNT)
  {
	for(i = magnetic.index +1 -SAMPLE_COUNT;i++;i<(magnetic.index-1)	
	{
		if(magnetic.M[Y_DIR][i]>magnetic.M[Y_DIR][i+1])
			MinY = magnetic.M[Y_DIR][i+1];
		if(magnetic.M[Y_DIR][i]<=magnetic.M[Y_DIR][i+1])
			MaxY = magnetic.M[Y_DIR][i+1];
		
		if(magnetic.M[Z_DIR][i]>magnetic.M[Z_DIR][i+1])
			MinZ = magnetic.M[Z_DIR][i+1];
		if(magnetic.M[Z_DIR][i]<=magnetic.M[Z_DIR][i+1])
			MaxZ = magnetic.M[Z_DIR][i+1];

	}
	sumtmp = MaxZ + MaxY +MinY + MinZ;
	if(sumtmp >4*THRES)
	{
		if(magnetic.Car_Flag == 0)
			magnetic.Car_Flag = 1;
			
	}
	sumtmp = MaxZ + MaxY -MinY - MinZ;

	if(sumtmp >2*THRES)
	{
	    adaptive_count = adaptive_count +1;
		if(adaptive_count >= 3)
		{
			BaseLineTrace();
		}
		if(magnetic.Car_Flag == 0)
			adaptive_count = 0;
			
	}
	else
		adaptive_count = 0;
  }
  else
  {
	BaseLineTrace();

  }
}
void vehicle_process()
{
	MagneticGet();//get magnetic data
	vehicle_detect();	
	AdaptiveBaseLine();

	if(magnetic.Car_Flag == 1)
	{
		;
	}
}
