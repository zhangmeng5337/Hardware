#include "filter.h"
#include "usystem.h"
#include "mag3D3100.h"
//#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal.h"
magnetic_str magnetic;
 extern  short int ManeticBuffer[3];
 extern MagData_t dataMd;
unsigned char getIndex()
{
  if(magnetic.index == 0)
  	return BUFFERSIZE - 1;
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
       s32 sum;
	unsigned char i;
	sum = 0;
	switch(dir)
	{
		case X_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[X_DIR][i] + sum;
			}
			sum = sum /(indexe - indexs);
		}break;
		case Y_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[Y_DIR][i] + sum;
			}
			sum = sum /(indexe - indexs);
		}break;
		case Z_DIR:{
			for(i = indexs;i<indexe;i++ )
			{
				sum = magnetic.M[Z_DIR][i] + sum;
			}
			sum = sum /(indexe - indexs);
		}break;
	}
	return sum;
 }
 void Statistic(s32 a[],int n,s32 *aver,s32 *vari,s32 *stdDev){
 //对长度为n的数组a进行统计，统计其平均值aver、方差vari、标准差stdDev
 int i;
 s32 p[N];
 s32 aver2,sum,stdDev_tmp,vari_tmp,aver_tmp;
 sum=0;
 for(i=0;i<n;i++){
  sum+=a[i];//累加各元素
  //aver2+=(a[i])*(a[i]);//累加各元素的平方
 }
 aver_tmp=sum/n;//求平均值
 aver3 =aver_tmp*aver_tmp;
 aver2/=n;//求平方的平均值
 vari_tmp=aver2-(aver3);//计算方差
 stdDev_tmp=sqrt(vari_tmp);//计算标准差
 aver = &aver_tmp;
 vari = &vari_tmp;
 stdDev = &stdDev_tmp;
 
}
void MagneticGet()
{
	 ThreeD3100_magic_GetData(&dataMd);

	magnetic.M[X_DIR][magnetic.index] =    dataMd.MAG_X; 
	magnetic.M[Y_DIR][magnetic.index] =    dataMd.MAG_Y;
	magnetic.M[Z_DIR][magnetic.index] =    dataMd.MAG_Z;
	

}
void MagneticInit()
{ 
   unsigned char i=0;
  for(i=0;i<BUFFERSIZE;i++)
  	{MagneticGet();
    magnetic.index++;}
	  magnetic.index = 0; 
}
void SmoothFilterInit()
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
		magnetic.F[X_DIR][magnetic.index] =( Sum(X_DIR,0,magnetic.index+1)+
			                                                    Sum(X_DIR,BUFFERSIZE-N+magnetic.index+1,BUFFERSIZE))/2;
		magnetic.F[Y_DIR][magnetic.index] = (Sum(Y_DIR,0,magnetic.index+1)+
			                                                    Sum(Y_DIR,BUFFERSIZE-N+magnetic.index+1,BUFFERSIZE))/2;
		magnetic.F[Z_DIR][magnetic.index] = (Sum(Z_DIR,0,magnetic.index+1)+
			                                                    Sum(Z_DIR,BUFFERSIZE-N+magnetic.index+1,BUFFERSIZE))/2;		
	}
	//magnetic.index++;

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
		  magnetic.B[X_DIR][magnetic.index] = (magnetic.B[X_DIR][index_tmp-1])*(1-ALPHA)+
				                                                (magnetic.M[X_DIR][index_tmp-1])*(ALPHA);
		  magnetic.B[Y_DIR][magnetic.index] = magnetic.B[Y_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                magnetic.M[Y_DIR][index_tmp-1]*(ALPHA);
		  magnetic.B[Z_DIR][magnetic.index] = magnetic.B[Z_DIR][index_tmp-1]*(1-ALPHA)+
				                                                magnetic.M[Z_DIR][index_tmp-1]*(ALPHA);


	}//x direction trance END
	MAX_THRES = (magnetic.B[Z_DIR][magnetic.index])*GAMMA;
	MIN_THRES = magnetic.B[Z_DIR][magnetic.index]*(GAMMA-1);	
}
unsigned char vehicle_detect()
{

    if(fabs(magnetic.VehicleVari- magnetic.BaseLineVari) >= MAX_THRES){
	    magnetic.eTime = HAL_GetTick();	
	    magnetic.elapseTime = magnetic.eTime - magnetic.sTime;
	    if(magnetic.elapseTime>= MIN_PERIOD){//检测有车时间超过最小周期，避免同一辆车未离开情况
						magnetic.Car_Flag = 1;
            magnetic.count = magnetic.count + 1;
	    	}
			else{ 
				magnetic.noupdate = 1;//同一辆车未离开
				return 1;
				//magnetic.sTime = HAL_GetTick();
			}
		}
	else 
		return 0;
	
}
 void BaseLineProcess()
{
 		MagneticGet();//get magnetic data
		SmoothFilter();
		s32 p[N];
		s32 *aver;
		s32 *Vari;
		unsigned char i,j;
	  //double *VehicleVari;
	  //double *BaseLineVari;
	  s32 *stdDev;
	j=0;
		if(magnetic.index>=N)
		//memcpy(p,&magnetic.M[Z_DIR][N-magnetic.index],N);
					for(i=(magnetic.index-N);i<magnetic.index;i++)
						p[j++]=magnetic.M[Z_DIR][i];
		else{

			//memcpy(p,&magnetic.M[Z_DIR][0],magnetic.index+1);
			for(i=0;i<magnetic.index+1;i++)
				p[i]=magnetic.M[Z_DIR][i];
			
			for(i=0;i<(N-magnetic.index);i++)
				p[i+magnetic.index+1]=magnetic.M[Z_DIR][BUFFERSIZE+magnetic.index-N+i];			
			//memcpy(p+1+magnetic.index,&magnetic.M[Z_DIR][BUFFERSIZE+magnetic.index-N],N-magnetic.index-1);
		}
		 printf("  M X:  %d",p[0]);
		 printf("  M X:  %d",magnetic.M[Z_DIR][N-magnetic.index]);
		Statistic(p ,N,aver,Vari,stdDev);
		if(vehicle_detect()==1)
			magnetic.VehicleVari = *Vari;
		else 
			magnetic.BaseLineVari = *Vari;
		
		BaseLineTrace();
		
		printf("  M X:  %d",magnetic.M[X_DIR][magnetic.index]);
		printf("  M Y:  %d",magnetic.M[Y_DIR][magnetic.index]);
		printf("  M Z:  %d",magnetic.M[Z_DIR][magnetic.index]);
		
		printf("  Manetic X:  %d",magnetic.F[X_DIR][magnetic.index]);
		printf("  Manetic Y:  %d",magnetic.F[Y_DIR][magnetic.index]);
		printf("  Manetic Z:  %d",magnetic.F[Z_DIR][magnetic.index]);
		
		printf("  BaseLineVari:  %d",magnetic.BaseLineVari);
		printf("  VehicleVari:  %d",magnetic.VehicleVari);
		printf("  Car_Flag:  %d",magnetic.Car_Flag);
		printf("  elapseTime:  %d",magnetic.elapseTime);
		printf("  noupdate:  %d\n",magnetic.noupdate);


}
void vehicle_process()
{
	if(TimingStart(1,SAMPLE_TIME,0,0)==1)
	{

		BaseLineProcess();
		magnetic.index++;
		//vehicle_detect();	
		//BaseLineProcess();
		if(magnetic.index >= BUFFERSIZE)
			magnetic.index = 0;	
	}

	if(magnetic.Car_Flag == 1)
	{
		;
	}
}
