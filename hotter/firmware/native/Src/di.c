#include "di.h"
stru_di_stru di_usr;
/*
diNO:di通道号
di_usr.di_status：对应通道有输入，为1
di_usr.di_No_buf：对应通道有输入数组置1
*/
stru_di_stru di_input_read(unsigned int diNO)
{   
    static unsigned char di_status = 0;
    switch(diNO)
    {
		case 0:
			{
			  if(HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin)==0)
			  {
			  	di_usr.di_No_buf[diNO] = 1;
				di_usr.di_status  = di_usr.di_status |0x01;

			  }

			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;
			  	di_usr.di_status  = di_usr.di_status &0xfe;

			  }
			  	

			}break;
		case 1:
			{
			  if(HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin)==0) 
			  {
			  	  di_usr.di_No_buf[diNO] = 1;			  	
				  di_usr.di_status  = di_usr.di_status |0x02;
			   }
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
				  di_usr.di_status  = di_usr.di_status &(~0x02); 			
		      }
			}break;
		case 2:
			{
			  if(HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin)==0)	
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
			  	  di_usr.di_status  = di_usr.di_status |0x04;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
			  	  di_usr.di_status  = di_usr.di_status &(~0x04);			  	
              }
			}break;
		case 3:
			{
			  if(HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin)==0) 
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
				  di_usr.di_status  = di_usr.di_status |0x08;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
				  di_usr.di_status  = di_usr.di_status &(~0x08); 			
		      }
			}break;
		case 4:
			{
			  if(HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin)==0)	
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
			  	  di_usr.di_status  = di_usr.di_status |0x10;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
			  	  di_usr.di_status  = di_usr.di_status &(~0x10);			  	
               }
			}break;
		case 5:
			{
			  if(HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin)==0) 
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
				  di_usr.di_status  = di_usr.di_status |0x20;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
				  di_usr.di_status  = di_usr.di_status &(~0x20); 			
		      }
			}break;
		case 6:
			{
			  if(HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin)==0)	
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
			  	  di_usr.di_status  = di_usr.di_status |0x40;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
			  	  di_usr.di_status  = di_usr.di_status &(~0x40);			  	
              }
			}break;
		case 7:
			{
			  if(HAL_GPIO_ReadPin(DI_IN7_GPIO_Port, DI_IN7_Pin)==0) 
			  {
			  	di_usr.di_No_buf[diNO] = 1;			  	
				  di_usr.di_status  = di_usr.di_status |0x80;
			  	}
			  else 
			  {
			  	di_usr.di_No_buf[diNO] = 0;			  	
				  di_usr.di_status  = di_usr.di_status &(~0x80); 			
		       }
			}break;
	   default:break;
	
	}
 	return di_usr;
}
unsigned char di_proc(unsigned char diNO)
{
	di_input_read( diNO);
	return di_usr.di_No_buf[diNO] ;	
}

