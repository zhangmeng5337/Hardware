#include "di.h"
static stru_di_stru di_usr;
/*
diNO:di通道号
di_usr.di_status：对应通道有输入，为1
di_usr.di_No_buf：对应通道有输入数组置1
#define WATER_LEVEL				0
#define SEQ_PROTECT				1
#define BASIC_PUMP				2
#define SECONDARY_PUMP			3
#define SUPPLE_PUMP				4

#define BASIC_PUMP_MANUAL		5
#define SECONDARY_PUMP_MANUAL	6
#define SUPPLE_PUMP_MANUAL		7

#define BASIC_PUMP_FAULT		8
#define SECONDARY_PUMP_FAULT	9
#define SUPPLE_PUMP_FAULT		10

*/
/*
0-15----4 3 2 1 8 7 6 5 9 10 11 12 13 14 15 16 
*/
stru_di_stru di_input_read(unsigned int diNO)
{   
		  if(HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin)==0)
		  {
			
			di_usr.di_status  = di_usr.di_status |di_index4 ;
   
		  }
   
		  else 
		  {
		
			di_usr.di_status  = di_usr.di_status &(~di_index4);
   
		  }


		{
		  if(HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin)==0) 
		  {
			 				
			  di_usr.di_status	= di_usr.di_status |di_index3;
		   }
		  else 
		  {
					
			  di_usr.di_status	= di_usr.di_status &(~di_index3);			
		  }
		}

		{
		  if(HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin)==0) 
		  {
						
			  di_usr.di_status	= di_usr.di_status |di_index2;
			}
		  else 
		  {
			
			  di_usr.di_status	= di_usr.di_status &(~di_index2);				
		  }
		}
	
		{
		  if(HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin)==0) 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[3]] = 1; 			
			  di_usr.di_status	= di_usr.di_status |di_index1;
			}
		  else 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[3]] = 0; 			
			  di_usr.di_status	= di_usr.di_status &(~di_index1);			
		  }
		}
	
		{
		  if(HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin)==0) 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[04]] = 1; 			
			  di_usr.di_status	= di_usr.di_status |di_index8;
			}
		  else 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[4]] = 0; 			
			  di_usr.di_status	= di_usr.di_status &(~di_index8);				
		   }
		}
	
		{
		  if(HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin)==0) 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[5]] = 1; 			
			  di_usr.di_status	= di_usr.di_status |di_index7;
			}
		  else 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[5]] = 0; 			
			  di_usr.di_status	= di_usr.di_status &(~di_index7);			
		  }
		}
	
		{
		  if(HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin)==0) 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[6]] = 1; 			
			  di_usr.di_status	= di_usr.di_status |di_index6;
			}
		  else 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[6]] = 0; 			
			  di_usr.di_status	= di_usr.di_status &(~di_index6);				
		  }
		}
	
		{
		  if(HAL_GPIO_ReadPin(DI_IN7_GPIO_Port, DI_IN7_Pin)==0) 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[7]] = 1; 			
			  di_usr.di_status	= di_usr.di_status |di_index5;
			}
		  else 
		  {
//			di_usr.di_No_buf[di_usr.di_index_conv[7]] = 0; 			
			  di_usr.di_status	= di_usr.di_status &(~di_index5);			
		   }
		}
   
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN8_GPIO_Port, DI_IN8_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[8]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index9;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[8]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index9);		   
		  }
	   }
  
	   {
		 if(HAL_GPIO_ReadPin(DI_IN9_GPIO_Port, DI_IN9_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[9]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index10;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[9]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index10);		   
		  }
	   }
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN10_GPIO_Port, DI_IN10_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[10]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index11;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[10]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index11);		   
		  }
	   }
   
  
	   {
		 if(HAL_GPIO_ReadPin(DI_IN11_GPIO_Port, DI_IN11_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[11]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index12;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[11]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index12);		   
		  }
	   }
   
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN12_GPIO_Port, DI_IN12_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[12]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index13;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[12]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index13);		   
		  }
	   }
   
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN13_GPIO_Port, DI_IN13_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[13]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index14;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[13]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index14);		   
		  }
	   }
   
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN14_GPIO_Port, DI_IN14_Pin)==0) 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[14]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index15;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[14]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index15);		   
		  }
	   }
   
   
	   {
		 if(HAL_GPIO_ReadPin(DI_IN15_GPIO_Port, DI_IN15_Pin)==0) 
		 {
//			 di_usr.di_No_buf[di_usr.di_index_conv[15]] = 1;			   
			 di_usr.di_status  = di_usr.di_status |di_index16;
		   }
		 else 
		 {
//		   di_usr.di_No_buf[di_usr.di_index_conv[15]] = 0;			   
			 di_usr.di_status  = di_usr.di_status &(~di_index16);		   
		  }
	   }
   
   
		
  

   // static unsigned char di_status = 0;
//    switch(diNO)
//    {
//		case 0:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin)==0)
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;
//				di_usr.di_status  = di_usr.di_status |0x0001;
//
//			  }
//
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;
//			  	di_usr.di_status  = di_usr.di_status &0x00fe;
//
//			  }
//			  	
//
//			}break;
//		case 1:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin)==0) 
//			  {
//			  	  di_usr.di_No_buf[diNO] = 1;			  	
//				  di_usr.di_status  = di_usr.di_status |0x0002;
//			   }
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//				  di_usr.di_status  = di_usr.di_status &(~0x0002); 			
//		      }
//			}break;
//		case 2:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin)==0)	
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//			  	  di_usr.di_status  = di_usr.di_status |0x0004;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//			  	  di_usr.di_status  = di_usr.di_status &(~0x0004);			  	
//              }
//			}break;
//		case 3:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin)==0) 
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//				  di_usr.di_status  = di_usr.di_status |0x0008;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//				  di_usr.di_status  = di_usr.di_status &(~0x0008); 			
//		      }
//			}break;
//		case 4:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin)==0)	
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//			  	  di_usr.di_status  = di_usr.di_status |0x0010;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//			  	  di_usr.di_status  = di_usr.di_status &(~0x0010);			  	
//               }
//			}break;
//		case 5:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin)==0) 
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//				  di_usr.di_status  = di_usr.di_status |0x0020;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//				  di_usr.di_status  = di_usr.di_status &(~0x0020); 			
//		      }
//			}break;
//		case 6:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin)==0)	
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//			  	  di_usr.di_status  = di_usr.di_status |0x0040;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//			  	  di_usr.di_status  = di_usr.di_status &(~0x0040);			  	
//              }
//			}break;
//		case 7:
//			{
//			  if(HAL_GPIO_ReadPin(DI_IN7_GPIO_Port, DI_IN7_Pin)==0) 
//			  {
//			  	di_usr.di_No_buf[diNO] = 1;			  	
//				  di_usr.di_status  = di_usr.di_status |0x0080;
//			  	}
//			  else 
//			  {
//			  	di_usr.di_No_buf[diNO] = 0;			  	
//				  di_usr.di_status  = di_usr.di_status &(~0x0080); 			
//		       }
//			}break;
//
//	   case 8:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN8_GPIO_Port, DI_IN8_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x0100;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x0100); 		   
//			  }
//		   }break;
//	   case 9:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN9_GPIO_Port, DI_IN9_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x0200;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x0200); 		   
//			  }
//		   }break;
//	   case 10:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN10_GPIO_Port, DI_IN10_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x0400;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x0400); 		   
//			  }
//		   }break;
//
//	   case 11:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN11_GPIO_Port, DI_IN11_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x0800;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x0800); 		   
//			  }
//		   }break;
//
//	   case 12:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN12_GPIO_Port, DI_IN12_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x1000;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x1000); 		   
//			  }
//		   }break;
//
//	   case 13:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN13_GPIO_Port, DI_IN13_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x2000;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x2000); 		   
//			  }
//		   }break;
//
//	   case 14:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN14_GPIO_Port, DI_IN14_Pin)==0) 
//			 {
//			   di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x4000;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x4000); 		   
//			  }
//		   }break;
//
//	   case 15:
//		   {
//			 if(HAL_GPIO_ReadPin(DI_IN15_GPIO_Port, DI_IN15_Pin)==0) 
//			 {
//			     di_usr.di_No_buf[diNO] = 1;			   
//				 di_usr.di_status  = di_usr.di_status |0x8000;
//			   }
//			 else 
//			 {
//			   di_usr.di_No_buf[diNO] = 0;			   
//				 di_usr.di_status  = di_usr.di_status &(~0x8000); 		   
//			  }
//		   }break;
//
//
//			
//	   default:break;
//	
//	}
 	return di_usr;
}
unsigned int di_proc(unsigned char diNO)
{
	di_input_read( diNO);
	return di_usr.di_status ;	
}
void di_init(void)
{
	//0-15----4 3 2 1 8 7 6 5 9 10 11 12 13 14 15 16
	//0-15----4 3 2 1 8 7 6 5 9 10 11 12 13 14 15 16
    unsigned char buf[DI_SIZE]={4,3,2,1,8,7,6,5,9,10,11,12,13,14,15,16};
	memcpy(di_usr.di_index_conv,buf,DI_SIZE);
}
stru_di_stru *get_di_data()
{
   return &di_usr;
}

