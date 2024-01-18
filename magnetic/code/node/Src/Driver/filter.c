#include "filter.h"
#include "usystem.h"
#include "math.h"
#include "mag3D3100.h"
#include "Node_Protocol.h"
//#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal.h"
 magnetic_str magnetic;
uint32_t MIN_PERIOD=1500;
float MAX_THRES=45;//�����ֵ
float MIN_THRES=5;
float MAX_ERROR = 20;
 extern  short int ManeticBuffer[3];
 extern MagData_t dataMd;
extern DataPack_stru DataPack;
unsigned char tx_start_flag;
extern uart_stru uart2;
//uint32_t stime,etime;
uint32_t sample_time_start,tx_time_start,tx_time_e,th;
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

	if(magnetic.base_line_update )//x direction trance start
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
float error_tmp22,error_tmp33;
uint32_t stime_tmp,etime_tmp;
float base_original;
unsigned char vehicle_detect()
{
  unsigned char res;
 
  if(magnetic.VehicleVari> magnetic.BaseLineVari)
		error_tmp =  magnetic.VehicleVari- magnetic.BaseLineVari;
	else
		error_tmp =   magnetic.BaseLineVari-magnetic.VehicleVari;		

   if( error_tmp>= MAX_THRES)
	{
		magnetic.detect_flag = 1;//������ֵ�������г�
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
				if(magnetic.elapseTime>= MIN_PERIOD)//����г�ʱ�䳬����С���ڣ�����ͬһ����δ�뿪���
				{
				  
					if(magnetic.noupdate )   //�����û��
					{   	
						magnetic.noupdate = 0;
					    magnetic.Car_Flag = 0;
						magnetic.base_line_update =0;
						magnetic.run_step = 1;
						magnetic.detect_flag = 0;
						magnetic.run_step = 6;
						#if DEBUG_LOG
							printf("********run_step:    %d\n",magnetic.run_step);
						#endif
					}
					else
					{   magnetic.detect_flag = 0;
						magnetic.Car_Flag = 1;
						magnetic.run_step = 2;
						#if DEBUG_LOG
							printf("********run_step:    %d\n",magnetic.run_step);
						#endif

						if(magnetic.noupdate == 0) //������1
						{
							magnetic.count = magnetic.count + 1;
							magnetic.run_step = 3;
							magnetic.detect_flag = 0;
							#if DEBUG_LOG
								printf("********run_step:    %d\n",magnetic.run_step);
							#endif


						}
							
					}
					
					magnetic.sTime = HAL_GetTick();
					th = magnetic.sTime;
					res = 1;
					
			    }
				else if(magnetic.elapseTime>= 1000)
				{ 
				    magnetic.detect_flag = 0;
					if(magnetic.noupdate == 0)
                    {
						  magnetic.noupdate = 1;
					      magnetic.Car_Flag = 1;
						  magnetic.run_step = 4;
							#if DEBUG_LOG
							  printf("********run_step:	 %d\n",magnetic.run_step);
							#endif

					}						
					magnetic.sTime = HAL_GetTick();
					  res = 1;
				}
			}			
		}
	  else
			res = 0;
	}
    if(magnetic.detect_flag == 1)
    {
		
			if(magnetic.base_line_update==0)
				magnetic.base_line_update = 3;	

	}

    if(magnetic.Car_Flag == 1)
    {
		
			if( magnetic.noupdate == 1)
				magnetic.base_line_update = 1;	
			else if(magnetic.noupdate == 0)
			{
					if(magnetic.base_line_update !=2)
					{	
						stime_tmp = HAL_GetTick();
						magnetic.base_line_update = 2;
					}						
			}

	}

	if(magnetic.base_line_update == 2)//��ͣ�ڵش��Ϸ�
	{	
		etime_tmp=HAL_GetTick() - stime_tmp;
		if((etime_tmp<= MIN_PERIOD))//�������������
		{
		  
			error_tmp22 = magnetic.M[Z_DIR][magnetic.index]-magnetic.B[Z_DIR][magnetic.index];
			error_tmp33 = fabs(error_tmp22);
			if(error_tmp33>MAX_ERROR)
			{
			        magnetic.run_step = 5;
					#if DEBUG_LOG
						printf("********run_step:    %d\n",magnetic.run_step);
					#endif

				    magnetic.Car_Flag =1;
				    magnetic.noupdate = 2;//ͬһ����δ�뿪
					magnetic.base_line_update =1;						
			}

		}
		if(etime_tmp>= 2*MIN_PERIOD||magnetic.noupdate == 2)//��ⳬʱ����ʱ��λ
		{	
			if(magnetic.base_line_update==2&&magnetic.noupdate == 0)
			magnetic.base_line_update =0;
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

		if(magnetic.detect_flag  == 1||magnetic.Car_Flag == 1||(magnetic.Car_Flag == 1&&magnetic.noupdate == 1))
		{
			#if DEBUG_LOG

						
//					printf("  MX:           %f",magnetic.M[X_DIR][magnetic.index]);
//					printf("  MY:           %f",magnetic.M[Y_DIR][magnetic.index]);
//					printf("  MZ:           %f",magnetic.M[Z_DIR][magnetic.index]);
//					
//					printf("  FX:           %f",magnetic.F[X_DIR][magnetic.index]);
//					printf("  FY:           %f",magnetic.F[Y_DIR][magnetic.index]);
//					printf("  FZ:           %f",magnetic.F[Z_DIR][magnetic.index]);
//			
//					printf("  BX:           %f",magnetic.B[X_DIR][magnetic.index]);
//					printf("  BY:           %f",magnetic.B[Y_DIR][magnetic.index]);
//					printf("  BZ:           %f",magnetic.B[Z_DIR][magnetic.index]);
//					
//					printf("  BaseLineVari: %f",magnetic.BaseLineVari);
//					printf("  VehicleVari:	%f",magnetic.VehicleVari);
//					printf("  err:	        %f",error_tmp);
			
					printf("  Car_Flag:    %d",magnetic.Car_Flag);
					printf("  elapseTime:  %d",magnetic.elapseTime);
					printf("  noupdate:    %d",magnetic.noupdate); 
					printf("  update:      %d",magnetic.base_line_update);
					printf("  run_step:    %d",magnetic.run_step);
					printf("  count:       %d\n",magnetic.count);
			#endif
		}

       

}
 static unsigned char func;
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
	  //magnetic.Car_Flag = 0;	
      tx_start_flag = 1;
	  tx_time_start = HAL_GetTick();
	  tx_time_e = TIME_OUT;
	// magnetic.noupdate = 0;
	}


		if(tx_start_flag == 1)
		{
		   if(tx_time_e<TIME_OUT)
				tx_time_e = HAL_GetTick()-tx_time_start;
			
			if(tx_time_e>=TIME_OUT)
			{
				tx_time_e = 0;
				tx_time_start = HAL_GetTick();

        
				if(DataPack.register_status == 0)//connect station
				{	
					Transmmit(REGISTER_CODE);
					func = REGISTER_CODE;
				}
				else  //tx car info
				{
					Transmmit(DYNAMIC_MODE);
					func = DYNAMIC_MODE;
				}
				DataPack.seq_num = DataPack.seq_num + 1;	
				if(DataPack.seq_num >TRANSMIT_COUNT)  //max tx times
				{
					magnetic.Car_Flag = 0;
				}
				if(DataPack.seq_num >=(TRANSMIT_COUNT+2))
				{
					DataPack.seq_num = 0;
					tx_start_flag = 0;

				}				
			}
				if(uartparase(2,func))
				{
          printf("  received data from station\n");
					func = 0;
					DataPack.seq_num = 0;
					DataPack.register_status = 1;
					tx_start_flag = 0;	
					magnetic.Car_Flag = 0;
          			 memset(uart2.receive_buffer,0,UARTBUFFERSIZE);					
				}	

		}	
		else
			ReceiverAnalysis();			
}
