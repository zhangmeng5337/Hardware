#include "filter.h"
#include "usystem.h"
#include "mag3D3100.h"
#include "Node_Protocol.h"
//#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal.h"
static magnetic_str magnetic;
uint32_t MIN_PERIOD=1000;
float MAX_THRES=35;//最大阈值
float MIN_THRES=15;
float MAX_ERROR = 20;
 extern  short int ManeticBuffer[3];
 extern MagData_t dataMd;
extern DataPack_stru DataPack;
unsigned char tx_start_flag;
extern uart_stru uart2;
//uint32_t stime,etime;
uint32_t sample_time_start,tx_time_start,th;
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
float Sum(unsigned char dir,unsigned char indexs,unsigned char indexe)
 {
       float sum;
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
 void Statistic(float a[],float q[],int n,float *vari1,float *vari2){
 //对长度为n的数组a进行统计，统计其平均值aver、方差vari、标准差stdDev
 int i;
// s32 p[N];
 static float averIns,averBase,sumIns,sumBase,variIns,variBase;
 
 sumBase=0;
 sumIns=0;
 for(i=0;i<n;i++){
  sumIns+=a[i];//累加各元素
  sumBase+=q[i];//累加各元素 
  //aver2+=(a[i])*(a[i]);//累加各元素的平方
 }
 averIns=sumIns/n;//求平均值
 averBase=sumBase/n;//求平均值
  sumBase=0;
 sumIns=0;
  for(i=0;i<n;i++){
  sumIns+=(a[i]-averIns)*(a[i]-averIns);//累加各元素
  sumBase+=(q[i]-averBase)*(q[i]-averBase);//累加各元素 
  //aver2+=(a[i])*(a[i]);//累加各元素的平方
 }
 variIns =sumIns/n;
 variBase =sumBase/n;
 *vari1 = variIns;
  *vari2 = variBase;
// vari = &variIns;
 // printf("  variIns:  %f",variIns);
 //printf("  variBase:  %f",variBase); 
 //printf("  variIns:  %f",*vari1);
 //printf("  variBase:  %f",*vari1); 
 
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
  	magnetic.B[X_DIR][magnetic.index] =    dataMd.MAG_X; 
	magnetic.B[Y_DIR][magnetic.index] =    dataMd.MAG_Y;
	magnetic.B[Z_DIR][magnetic.index] =    dataMd.MAG_Z;
    magnetic.index++;}
	  magnetic.index = 0; 
}
void SmoothFilterInit()
{

}
//婊戣啘婊ゆ尝
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
			                                                    Sum(X_DIR,BUFFERSIZE-N+magnetic.index,BUFFERSIZE))/2;
		magnetic.F[Y_DIR][magnetic.index] = (Sum(Y_DIR,0,magnetic.index+1)+
			                                                    Sum(Y_DIR,BUFFERSIZE-N+magnetic.index,BUFFERSIZE))/2;
		magnetic.F[Z_DIR][magnetic.index] = (Sum(Z_DIR,0,magnetic.index+1)+
			                                                    Sum(Z_DIR,BUFFERSIZE-N+magnetic.index,BUFFERSIZE))/2;		
	}
	//magnetic.index++;

}
void BaseLineTrace()
{

	  unsigned char index_tmp;
	  index_tmp = getIndex();

	if(magnetic.detect_flag == 1)//x direction trance start
	{

		magnetic.B[X_DIR][magnetic.index] = magnetic.B[X_DIR][index_tmp-1];
		magnetic.B[Y_DIR][magnetic.index] = magnetic.B[Y_DIR][index_tmp-1];
		magnetic.B[Z_DIR][magnetic.index] = magnetic.B[Z_DIR][index_tmp-1];
	}
	else
	{
		  magnetic.B[X_DIR][magnetic.index] = (magnetic.B[X_DIR][index_tmp-1])*(1-ALPHA)+
				                                                (magnetic.F[X_DIR][index_tmp-1])*(ALPHA);
		  magnetic.B[Y_DIR][magnetic.index] = magnetic.B[Y_DIR][BUFFERSIZE-1]*(1-ALPHA)+
				                                                magnetic.F[Y_DIR][index_tmp-1]*(ALPHA);
		  magnetic.B[Z_DIR][magnetic.index] = (magnetic.B[Z_DIR][index_tmp-1])*(1-ALPHA)+
				                                                (magnetic.F[Z_DIR][index_tmp-1])*(ALPHA);


	}//x direction trance END
	//MAX_THRES = (magnetic.B[Z_DIR][magnetic.index])*GAMMA;
	//MIN_THRES = magnetic.B[Z_DIR][magnetic.index]*(GAMMA-1);	
}
float error_tmp;

unsigned char vehicle_detect()
{
  unsigned char res;
  if(magnetic.VehicleVari> magnetic.BaseLineVari)
	error_tmp =  magnetic.VehicleVari- magnetic.BaseLineVari;
	else
	error_tmp =   magnetic.BaseLineVari-magnetic.VehicleVari;		

    if( error_tmp>= MAX_THRES){
		magnetic.detect_flag = 1;//大于阈值，疑似有车
		res = 0;
    }
	else
	{
		if(magnetic.detect_flag == 1)
		{
			if(error_tmp<= MIN_THRES)
			{			
				magnetic.eTime = HAL_GetTick(); 
				magnetic.elapseTime = magnetic.eTime - magnetic.sTime;	
				if(magnetic.elapseTime>= MIN_PERIOD)//检测有车时间超过最小周期，避免同一辆车未离开情况
				{
					magnetic.detect_flag = 0;
					magnetic.Car_Flag = 1;
					magnetic.count = magnetic.count + 1;
					magnetic.sTime = HAL_GetTick();
					th = magnetic.sTime;
					magnetic.noupdate = 0; 
					res = 1;
			    }
				else
				{ 
				       	magnetic.detect_flag = 0;
						magnetic.Car_Flag = 1;
						magnetic.noupdate = 1;//同一辆车未离开
						res = 1;
				}
			}			


		}
	    else
			res = 0;

		if(magnetic.Car_Flag == 1)//车停在地磁上方
		{
			if((magnetic.M[Z_DIR][magnetic.index]-magnetic.B[Z_DIR][magnetic.index])>MAX_ERROR)
			{
				magnetic.noupdate = 1;//同一辆车未离开
				magnetic.Car_Flag = 1;

			}
			else
			{
				magnetic.noupdate = 0;//
				magnetic.Car_Flag = 0;

			}

		}
	}


		return res;
	
}
 void BaseLineProcess()
{
 		MagneticGet();//get magnetic data
		SmoothFilter();
		static float p[N],q[N];
//		s32 *aver;
		float VariIns,VariBase;
		unsigned char i,j;
	  //double *VehicleVari;
	  //double *BaseLineVari;
//	    s32 *stdDev;
	    j=0;
		if(magnetic.index>=N){
			for(i=(magnetic.index-N);i<magnetic.index;i++)
			{
			    //if(i==0)
				//	q[j]=magnetic.B[Z_DIR][getIndex()];
                //else
				   q[j]=magnetic.B[Z_DIR][i];
			
				p[j++]=magnetic.M[Z_DIR][i];
			
			}

		}

		else{

			//memcpy(p,&magnetic.M[Z_DIR][0],magnetic.index+1);
			for(i=0;i<magnetic.index+1;i++)
			{	
			  //  if(i==0)
				//	q[i]=magnetic.B[Z_DIR][getIndex()];
				//else 
					q[i]=magnetic.B[Z_DIR][i];					
				p[i]=magnetic.M[Z_DIR][i];

			}
			
			for(i=0;i<(N-magnetic.index-1);i++){
				q[i+magnetic.index+1]=magnetic.B[Z_DIR][BUFFERSIZE+magnetic.index-N+i];
				p[i+magnetic.index+1]=magnetic.M[Z_DIR][BUFFERSIZE+magnetic.index-N+i];

				}

			
			//memcpy(p+1+magnetic.index,&magnetic.M[Z_DIR][BUFFERSIZE+magnetic.index-N],N-magnetic.index-1);
		}

		Statistic(p ,q,N,&VariIns,&VariBase);
		//printf("  M X:  %f",VariIns);
		//printf("  M X:  %f",VariBase);

		vehicle_detect();
		magnetic.VehicleVari = VariIns; 
		magnetic.BaseLineVari = VariBase;
		
		BaseLineTrace();

		//if(DataPack.tx_period == 0xff)
		{
					printf("  MX:  %f",magnetic.M[X_DIR][magnetic.index]);
					printf("  MY:  %f",magnetic.M[Y_DIR][magnetic.index]);
					printf("  MZ:  %f",magnetic.M[Z_DIR][magnetic.index]);
					
					printf("  FX:  %f",magnetic.F[X_DIR][magnetic.index]);
					printf("  FY:  %f",magnetic.F[Y_DIR][magnetic.index]);
					printf("  FZ:  %f",magnetic.F[Z_DIR][magnetic.index]);
			
					printf("  BX:  %f",magnetic.B[X_DIR][magnetic.index]);
					printf("  BY:  %f",magnetic.B[Y_DIR][magnetic.index]);
					printf("  BZ:  %f",magnetic.B[Z_DIR][magnetic.index]);
					
					printf("  BaseLineVari:  %f",magnetic.BaseLineVari);
					printf("  VehicleVari:	%f",magnetic.VehicleVari);
					printf("  err:	%f\n",error_tmp);
					printf("  Car_Flag:  %d",magnetic.Car_Flag);
					printf("  elapseTime:  %d",magnetic.elapseTime);
					printf("  noupdate:  %d",magnetic.noupdate);
					printf("  count:  %d\n",magnetic.count);


		}

       

}
volatile unsigned char func;
void vehicle_process()
{
	
	if((HAL_GetTick()-sample_time_start)>=SAMPLE_TIME)
	{
		sample_time_start = HAL_GetTick();
		BaseLineProcess();
		magnetic.index++;
		if(magnetic.index >= BUFFERSIZE)
			magnetic.index = 0;	
	}

	if(magnetic.Car_Flag == 1)
	{
	  magnetic.Car_Flag = 0;	
    tx_start_flag = 1;
		
	}
	if(DataPack.tx_period != 0xff)
	{

		if(tx_start_flag == 1)
		{
			if((HAL_GetTick()-tx_time_start)>=TIME_OUT)
			{
				tx_time_start = HAL_GetTick();


				if(DataPack.register_status == 0)
				{	
					Transmmit(REGISTER_CODE);
					func = REGISTER_CODE;
				}
				else
				{
					Transmmit(DYNAMIC_MODE);
					func = DYNAMIC_MODE;
				}
				DataPack.seq_num = DataPack.seq_num + 1;	
				if(DataPack.seq_num >1)
				{
					DataPack.seq_num = 0;
					tx_start_flag = 0;
					magnetic.Car_Flag = 0;
				}			
			}
			else
			{
				if(uartparase(2,func))
				{
						DataPack.seq_num = 0;
						DataPack.register_status = 1;
						tx_start_flag = 0;	
						magnetic.Car_Flag = 0;					
				}	
			}

		}			
	}
	
	
	ReceiverAnalysis();
	
}
