#include "filter.h"
manetic_str manetic;
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
			for(i = indexs;i<=indexe;i++ )
			{
				sum = manetic.M[X_DIR][i] + sum;
			}
			sum = sum /(manetic.index + 1);
		}break;
		case Y_DIR:{
			for(i = indexs;i<=indexe;i++ )
			{
				sum = manetic.M[Y_DIR][i] + sum;
			}
			sum = sum /(manetic.index + 1);
		}break;
		case Z_DIR:{
			for(i = indexs;i<=indexe;i++ )
			{
				sum = manetic.M[Z_DIR][i] + sum;
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
	manetic.M[X_DIR][manetic.index] =      pbuffer[0];
	manetic.M[Y_DIR][manetic.index] =      pbuffer[1];
	manetic.M[Z_DIR][manetic.index] =      pbuffer[2];

}
void void SmoothFilterInit()
{

}
void SmoothFilter()
{
	if(manetic.index >= N)
	{
		manetic.A[X_DIR][manetic.index] = Sum(X_DIR,manetic.index+1-N,manetic.index);
		manetic.A[Y_DIR][manetic.index] = Sum(Y_DIR,manetic.index+1-N,manetic.index);
		manetic.A[Z_DIR][manetic.index] = Sum(Z_DIR,manetic.index+1-N,manetic.index);
	}
	else
	{
		manetic.A[X_DIR][manetic.index] = Sum(X_DIR,0,manetic.index);
		manetic.A[Y_DIR][manetic.index] = Sum(Y_DIR,0,manetic.index);
		manetic.A[Z_DIR][manetic.index] = Sum(Z_DIR,0,manetic.index);		
	}
	manetic.index++;
	if(manetic.index >= BUFFERSIZE)
		manetic.index = 0;
}
void BaseLineTrace()
{

        if(manetic.index == 0)//update Tk start
        {
		manetic.T[X_DIR][manetic.index] = (1+BETA)*manetic.B[X_DIR][BUFFERSIZE-1];
		manetic.T[Y_DIR][manetic.index] = (1+BETA)*manetic.B[Y_DIR][BUFFERSIZE-1];
		manetic.T[Z_DIR][manetic.index] = (1+BETA)*manetic.B[Z_DIR][BUFFERSIZE-1];
	}	
	else
	{
		manetic.T[X_DIR][manetic.index] = (1+BETA)*manetic.B[X_DIR][manetic.index-1];
		manetic.T[Y_DIR][manetic.index] = (1+BETA)*manetic.B[Y_DIR][manetic.index-1];
		manetic.T[Z_DIR][manetic.index] = (1+BETA)*manetic.B[Z_DIR][manetic.index-1];
	}//update Tk end

	if(manetic.A[X_DIR][manetic.index] >=manetic.T[X_DIR][manetic.index])//x direction trance start
	{
	        if(manetic.index == 0)
	        {
			manetic.B[X_DIR][manetic.index] = manetic.B[X_DIR][BUFFERSIZE-1];
		}
			
		else
		{
			manetic.B[X_DIR][manetic.index] = manetic.B[X_DIR][manetic.index-1];
		}
		manetic.F[X_DIR][manetic.index] = 1;

	}
	else
	{
	        if(manetic.index == 0)
	        {
			manetic.B[X_DIR][manetic.index] = manetic.B[X_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                manetic.A[X_DIR][BUFFERSIZE-1]*(ALPHA);
		}
			
		else
		{
			manetic.B[X_DIR][manetic.index] = manetic.B[X_DIR][manetic.index-1]*(1-ALPHA)+
				                                                manetic.A[X_DIR][manetic.index-1]*(ALPHA);
		}
		manetic.F[X_DIR][manetic.index] = 0;
	}//x direction trance END

	if(manetic.A[Y_DIR][manetic.index] >=manetic.T[Y_DIR][manetic.index])//Y direction trance start
	{
	        if(manetic.index == 0)
	        {
			manetic.B[Y_DIR][manetic.index] = manetic.B[Y_DIR][BUFFERSIZE-1];
		}
			
		else
		{
			manetic.B[Y_DIR][manetic.index] = manetic.B[Y_DIR][manetic.index-1];
		}
		manetic.F[Y_DIR][manetic.index] = 1;

	}
	else
	{
	        if(manetic.index == 0)
	        {
			manetic.B[Y_DIR][manetic.index] = manetic.B[Y_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                manetic.A[Y_DIR][BUFFERSIZE-1]*(ALPHA);
		}
			
		else
		{
			manetic.B[Y_DIR][manetic.index] = manetic.B[Y_DIR][manetic.index-1]*(1-ALPHA)+
				                                                manetic.A[Y_DIR][manetic.index-1]*(ALPHA);
		}
		manetic.F[Y_DIR][manetic.index] = 0;
	
	}//Y direction trance END
	if(manetic.A[Z_DIR][manetic.index] >=manetic.T[Z_DIR][manetic.index])//Y direction trance start
	{
	        if(manetic.index == 0)
	        {
			manetic.B[Z_DIR][manetic.index] = manetic.B[Z_DIR][BUFFERSIZE-1];
		}
			
		else
		{
			manetic.B[Z_DIR][manetic.index] = manetic.B[Z_DIR][manetic.index-1];
		}
		manetic.F[Z_DIR][manetic.index] = 1;

	}
	else
	{
	        if(manetic.index == 0)
	        {
			manetic.B[Z_DIR][manetic.index] = manetic.B[Z_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                manetic.A[Z_DIR][BUFFERSIZE-1]*(ALPHA);
		}
			
		else
		{
			manetic.B[Z_DIR][manetic.index] = manetic.B[Z_DIR][manetic.index-1]*(1-ALPHA)+
				                                                manetic.A[Z_DIR][manetic.index-1]*(ALPHA);
		}
		manetic.F[Z_DIR][manetic.index] = 0;
	
	}//Y direction trance END	
	
}
