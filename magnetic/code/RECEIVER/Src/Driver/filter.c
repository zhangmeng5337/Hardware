#include "filter.h"
#include "usystem.h"
#include "mag3D3100.h"
#include "Node_Protocol.h"
//#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal.h"
magnetic_str magnetic;
uint32_t MIN_PERIOD=500;
float MAX_THRES=120;
float  MIN_THRES=60;
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
            @index: ��ǰ���ݻ����������±�
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
 //�Գ���Ϊn������a����ͳ�ƣ�ͳ����ƽ��ֵaver������vari����׼��stdDev
 int i;
// s32 p[N];
 static float averIns,averBase,sumIns,sumBase,variIns,variBase;
 
 sumBase=0;
 sumIns=0;
 for(i=0;i<n;i++){
  sumIns+=a[i];//�ۼӸ�Ԫ��
  sumBase+=q[i];//�ۼӸ�Ԫ�� 
  //aver2+=(a[i])*(a[i]);//�ۼӸ�Ԫ�ص�ƽ��
 }
 averIns=sumIns/n;//��ƽ��ֵ
 averBase=sumBase/n;//��ƽ��ֵ
  sumBase=0;
 sumIns=0;
  for(i=0;i<n;i++){
  sumIns+=(a[i]-averIns)*(a[i]-averIns);//�ۼӸ�Ԫ��
  sumBase+=(q[i]-averBase)*(q[i]-averBase);//�ۼӸ�Ԫ�� 
  //aver2+=(a[i])*(a[i]);//�ۼӸ�Ԫ�ص�ƽ��
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
//滑膜滤波
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

	if(magnetic.Car_Flag == 1)//x direction trance start
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
  if(magnetic.VehicleVari> magnetic.BaseLineVari)
	error_tmp =  magnetic.VehicleVari- magnetic.BaseLineVari;
	else
	error_tmp =   magnetic.BaseLineVari-magnetic.VehicleVari;		

    if( error_tmp>= MAX_THRES){
	    magnetic.eTime = HAL_GetTick();	
	    magnetic.elapseTime = magnetic.eTime - magnetic.sTime;
	    if(magnetic.elapseTime>= MIN_PERIOD){//����г�ʱ�䳬����С���ڣ�����ͬһ����δ�뿪���
						magnetic.Car_Flag = 1;
            magnetic.count = magnetic.count + 1;
		    magnetic.sTime = HAL_GetTick();
	    	}
			else{ 
				magnetic.noupdate = 1;//ͬһ����δ�뿪
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
			
			for(i=0;i<(N-magnetic.index);i++){
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
		printf("  VehicleVari:  %f",magnetic.VehicleVari);
		printf("  err:  %f\n",error_tmp);
	//	printf("  Car_Flag:  %d",magnetic.Car_Flag);
	//	printf("  elapseTime:  %d",magnetic.elapseTime);
	//	printf("  noupdate:  %d\n",magnetic.noupdate);


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
		magnetic.Car_Flag=0;
		Transmmit(DYNAMIC_MODE);
	}
}
