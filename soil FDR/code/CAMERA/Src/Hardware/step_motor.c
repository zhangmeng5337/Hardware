#include "step_motor.h"
extern UART_HandleTypeDef huart1;
static uint32_t motor_state_pitch,motor_state_yaw;
uint32_t motor_tick;
unsigned char step_driver(unsigned char motorNo)
{
  
  
  
  if((HAL_GetTick()-motor_tick)>=MOTOR_PHASE_DELAY)
  {
    if(motorNo == YAW)
    {
      motor_tick = HAL_GetTick();
      switch(motor_state_yaw)
      {
      case 0:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_SET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_SET);	//A	
        break;
      case 1:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_SET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);	//A	
        break;
      case 2:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_SET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_SET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);	//A			
        break;
      case 3:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_SET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);	//A			
        break;
      case 4:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_SET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_SET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);	//A			
        break;
      case 5:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_SET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);	//A		
        break;
      case 6:
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_SET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_SET);	//A					
        break;
      case 7:
        //motor_state_yaw ++ ;
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);	//D		
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);	//C	
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);	//B
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_SET);	//A			
        break;
      case 8 :
        //motor_state_yaw =0 ;
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);
        break;	
      default :
        //	motor_state_yaw =0 ;
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_A4_Pin, GPIO_PIN_RESET);
        break;					
      }			
    }
    else
    {
      motor_tick = HAL_GetTick();
      switch(motor_state_pitch)
      {
      case 0:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_SET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_RESET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_RESET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_SET);//D			
        break;
      case 1:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_SET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_RESET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_RESET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_RESET);//D		
        break;
      case 2:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_SET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_SET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_RESET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_RESET);//D		
        break;
      case 3:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_RESET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_SET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_RESET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_RESET);//D			
        break;
      case 4:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_RESET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_SET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_SET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_RESET);//D		
        break;
      case 5:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_RESET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_RESET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_SET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_RESET);//D	
        break;
      case 6:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_RESET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_RESET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_SET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_SET);//D	
        break;
      case 7:
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin, GPIO_PIN_RESET);//A
        HAL_GPIO_WritePin(GPIOA, MOTOR_B2_Pin, GPIO_PIN_RESET);//B
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin, GPIO_PIN_RESET);//C
        HAL_GPIO_WritePin(GPIOB, MOTOR_B4_Pin, GPIO_PIN_SET);//D	
        break;
      case 8 :
        //motor_state_pitch =0 ;
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
        break;	
      default :
        //	motor_state_pitch =0 ;
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
        break;					
      }			
    }
    
    return 1;
  }
  else 
    return 0;
  
}

void step_motor_step_cal(float angle,unsigned char motor_No,unsigned char direction)
{
  uint32_t step;
  static uint32_t i;
  i=0;
  step = (uint32_t)(angle/5.625*64);//steps to be run
  for(i=0;i<step;i++)
  {
    
    if(i>=MAX_STEP_YAW)
    {
      if(motor_No==YAW)
      {
        //motor_state_yaw=0;
        HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin|MOTOR_A2_Pin|MOTOR_A3_Pin|MOTOR_A4_Pin, GPIO_PIN_RESET);
        i = 0;
        break;
      }	
      //			else if(motor_No==PITCH)
      //			{
      //				//motor_state_pitch=0;
      //				HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
      //				HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
      //			}
      
    }
    else if(i>=MAX_STEP_PITCH)
    {
      if(motor_No==PITCH)
      {
        //motor_state_pitch=0;
        HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
        i = 0;
        break;
      }
      
    }
    //else
    {
      while(step_driver(motor_No)==0)
        ;
      {
        if(direction == FORWARD)
        {
          if(motor_No==YAW)
            motor_state_yaw++;
          if(motor_state_yaw>8)
          {
            
            motor_state_yaw = 0;						
          }
          
          if(motor_No==PITCH)
            motor_state_pitch++;	
          if(motor_state_pitch>8)
            motor_state_pitch = 0;
        }
        else
        {
          if(motor_No==YAW)
          {
            if(motor_state_yaw>0)
              motor_state_yaw--;	
            else
              motor_state_yaw = 8;
          }		
          if(motor_No==PITCH)
          {
            if(motor_state_pitch>0)
              motor_state_pitch--;	
            else
              motor_state_pitch = 8;
          }		
        }
      }
    }
  }
  if(motor_No==YAW)
  {
    //motor_state_yaw=0;
    HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin|MOTOR_A2_Pin|MOTOR_A3_Pin|MOTOR_A4_Pin, GPIO_PIN_RESET);
  }	
  else if(motor_No==PITCH)
  {
    motor_state_pitch=0;
    HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
  }
}
void step_motor(float angle,unsigned char motor_No,unsigned char direction)
{
  
  /*state varialbe init*/
  
  if(direction == FORWARD)//forward direction
  {/************************************motor select*******************************/
    if(motor_No==YAW)
    {
      motor_state_yaw=0;	
      HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin|MOTOR_A2_Pin|MOTOR_A3_Pin|MOTOR_A4_Pin, GPIO_PIN_RESET);	
    }			
    if(motor_No==PITCH)
    {
      motor_state_pitch=0;	
      HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);		
    }			
  }
  else
  {
    if(motor_No==YAW)
    {	
      motor_state_yaw=8;
      HAL_GPIO_WritePin(GPIOA, MOTOR_A1_Pin|MOTOR_A2_Pin|MOTOR_A3_Pin|MOTOR_A4_Pin, GPIO_PIN_RESET);	
    }		
    if(motor_No==PITCH)
    {
      motor_state_pitch=8;
      HAL_GPIO_WritePin(GPIOA, MOTOR_B1_Pin|MOTOR_B2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, MOTOR_B3_Pin|MOTOR_B4_Pin, GPIO_PIN_RESET);
    }						
  }
  
  step_motor_step_cal(angle,motor_No,direction);
}
extern UART_HandleTypeDef huart1;

unsigned char Uart1_Receive_flag;
unsigned char Receive_data[UART1_BUFFER_SIZE];
unsigned char TxBuffer[UART1_BUFFER_SIZE];
void uart_test()
{
  TxBuffer[0]= HEADER;
  TxBuffer[TAG_INDEX]= TAG;
  TxBuffer[RGB_INDEX]= RGB_ALL_OFF;
  TxBuffer[DIRECTION_INDEX]= FORWARD;
  TxBuffer[MOTOR_NUM_INDEX]= YAW;
  TxBuffer[TAG_INDEX-1] =  TxBuffer[RGB_INDEX]+TxBuffer[RGB_INDEX]+TxBuffer[DIRECTION_INDEX]+TxBuffer[MOTOR_NUM_INDEX];
  HAL_UART_Transmit(&huart1, TxBuffer, TAG_INDEX, 10);
}
unsigned char Receive_data_temp[UART1_BUFFER_SIZE];
void Receive_command(unsigned char revdat)
{
  
  unsigned char inByte,temp;
  
  static unsigned char Receive_item;
  inByte = revdat;
  
  if(inByte==HEADER)
  {
    Receive_item=0;
  }
  Receive_data_temp[Receive_item++]=inByte;
  if(Receive_item>=UART1_BUFFER_SIZE)
  {
    Receive_item=0;
  }
  if(Receive_data_temp[TAG_INDEX]==TAG)
  {
    temp=0; //对接受的数据进行累计校验
    for(int i=0; i<(TAG_INDEX-1); i++)
    {
      temp += Receive_data_temp[i];
    }
    if(temp==Receive_data_temp[TAG_INDEX-1])
    {
      for(int j=0; j<=TAG_INDEX; j++)
      {
        Receive_data[j]=Receive_data_temp[j];
      } //如果接受到的数据正确则 copy 到 Receive_data 数组中
    }
    Uart1_Receive_flag=1;
    //Receive_item=0;	
    //memset(Receive_data_temp,0,10);
  }
  
}
void rgb_control(unsigned char color)
{
  switch(color)
  {
  case RGB_ALL_OFF:HAL_GPIO_WritePin(GPIOA, R_Pin|G_Pin|B_Pin, GPIO_PIN_RESET);	
  break;
  case RGB_RED_ON:HAL_GPIO_WritePin(GPIOA, R_Pin|B_Pin, GPIO_PIN_RESET);	
  HAL_GPIO_WritePin(GPIOA, G_Pin, GPIO_PIN_SET);
  break;
  case RGB_GREEN_ON:
    HAL_GPIO_WritePin(GPIOA, R_Pin|G_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, B_Pin, GPIO_PIN_SET);		
    break;
  case RGB_BLUE_ON:
    HAL_GPIO_WritePin(GPIOA, G_Pin|B_Pin, GPIO_PIN_RESET);	
    HAL_GPIO_WritePin(GPIOA, R_Pin, GPIO_PIN_SET);
    
    break;
    //default:HAL_GPIO_WritePin(GPIOA, R_Pin|G_Pin|B_Pin, GPIO_PIN_RESET);	
    //		break;
  }
}
/****************************************************
data format: Header    MotorNo             Diretion      Angle_H  Angle_L  rgbled   Checksum   tag
0xaa    YAW/PITCH     FORWARD/BACKWARD

*****************************************************/
void uart_analysis()
{
  if(Uart1_Receive_flag == 1)
  {	
    /*******************************************************************
    led control
    *******************************************************************/
    
    rgb_control(Receive_data[RGB_INDEX]);
    
    /**********************************************************************
    step motro control
    **********************************************************************/
    if(Receive_data[DIRECTION_INDEX]==FORWARD)  
    {
      if(Receive_data[MOTOR_NUM_INDEX]==YAW)	
      {
        step_motor_step_cal((Receive_data[ANGLE_H_INDEX]<<8)+Receive_data[ANGLE_L_INDEX],YAW,FORWARD);
      }	
      else
      {
        step_motor_step_cal((Receive_data[ANGLE_H_INDEX]<<8)+Receive_data[ANGLE_L_INDEX],PITCH,FORWARD);
      }		 
    }
    else
    {
      if(Receive_data[MOTOR_NUM_INDEX]==YAW)	
      {
        step_motor_step_cal((Receive_data[ANGLE_H_INDEX]<<8)+Receive_data[ANGLE_L_INDEX],YAW,BACKWARD);
      }	
      else
      {
        step_motor_step_cal((Receive_data[ANGLE_H_INDEX]<<8)+Receive_data[ANGLE_L_INDEX],PITCH,BACKWARD);
      }	
    }
    
    Uart1_Receive_flag = 0;
  }
}
