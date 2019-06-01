//#include "command.h"
#include "base_types.h"
#include "key.h"
#include "uartParse.h"
#include "command.h"
#include "main.h"
#include "crc.h"
#include <stdio.h>
#include <stdlib.h>
uint16_t CommandStatusToggleFlag=0;
void ack_response(unsigned char  cmd_type)
{
  uint8_t *pbuffer = malloc(9);

	switch(cmd_type)
	{
		case WAKEUP_CMD:
		{
			pbuffer[0] = WAKE_HEAD;
			pbuffer[1] = WAKE_HEAD;
			pbuffer[2] = WAKE_HEAD;
			pbuffer[3] = WAKE_HEAD;
			pbuffer[4] = WAKEUP_CMD;
			pbuffer[5] = 0x0d;
    			params_typedef *tmp=system_params_get();
			memcpy(&pbuffer[6],tmp,9);
			unsigned int crc=CRC16_Get8(&pbuffer[4],11);
			pbuffer[15] =(unsigned char)( crc>>8);	
			pbuffer[16] = (unsigned char)crc;	
			pbuffer[17] = WAKE_TAG;	

 			UART_Config();
 			 UART1_SendBytes(pbuffer,18);
			   free(pbuffer);
				pbuffer = NULL;
			
		}break;
		case FORMAT_CMD:
		{
			pbuffer[0] = WAKE_HEAD;
			pbuffer[1] = WAKE_HEAD;
			pbuffer[2] = WAKE_HEAD;
			pbuffer[3] = WAKE_HEAD;
			pbuffer[4] = FORMAT_CMD;
			pbuffer[5] = 0x0d;
    			params_typedef *tmp=system_params_get();
			memcpy(&pbuffer[6],tmp,9);
			unsigned int crc=CRC16_Get8(&pbuffer[4],11);
			pbuffer[15] =(unsigned char)( crc>>8);	
			pbuffer[16] = (unsigned char)crc;	
			pbuffer[17] = WAKE_TAG;	

 			UART_Config();
 			 UART1_SendBytes(pbuffer,18);
			   free(pbuffer);
				pbuffer = NULL;
		}break;	
		
	}
}
/******************************************************************
函数名:  CommandTx(uint16_t key_num,uint8_t mode,boolean_t toggle_flag)
参数:
            key_num:被按下的按键
            mode:命令模式
            				0:控制模式
            				1:配置模式
            toggle_flag:继电器动作
          			  	0:继电器吸合
           			 	1:继电器断开
*******************************************************************/
void  CommandTx(uint16_t key_num,uint8_t mode,boolean_t toggle_flag)
{
  uint8_t *pbuffer = malloc(9);
  uint16_t crc_tmp;
  uint8_t len_tmp;
//  *pbuffer++ = WAKE_HEAD;
  *pbuffer++ = mode;
  //控制模式
  if(mode == COMMAND_FUNC_CTRL)
  {
    //按键编号
    *pbuffer++ = (uint8_t)(key_num>>8);	
    *pbuffer++ = (uint8_t)(key_num);
    //继电器动作
    if(toggle_flag == 1)
    {
      *pbuffer++ = (uint8_t)(key_num>>8);	
      *pbuffer++ = (uint8_t)(key_num);		
    }
    else
    {
      *pbuffer++ = (uint8_t)(~key_num>>8);	
      *pbuffer++ = (uint8_t)(~key_num);
    }
	len_tmp = COMMAND_CTRL_LEN;
    crc_tmp = CRC16_Get8(pbuffer, len_tmp);
  }
  //配置模式
  if(mode == COMMAND_FUNC_CONFIG)
  {
    params_typedef *tmp=system_params_get();
    *pbuffer++ = tmp->sn[0];
    *pbuffer++ = tmp->sn[1];
    *pbuffer++ = tmp->sn[2];	
    *pbuffer++ = tmp->sn[3];
    *pbuffer++ =(uint8_t)( tmp->freq>>8);
    *pbuffer++ =(uint8_t)( tmp->freq);	
	len_tmp = COMMAND_CONFIG_LEN;
    crc_tmp = CRC16_Get8(pbuffer, len_tmp);			
  }
  
  
  *pbuffer++ =(uint8_t)(crc_tmp>>8);
  *pbuffer++ =(uint8_t)(crc_tmp);
  *pbuffer++ =WAKE_TAG;
  
   CMT2300_Init();
   setup_Tx();
   unsigned char i;
   i=0;
   while(i<MAX_COUNT)
   {
         i++;
  	  loop_Tx(pbuffer,len_tmp);
   }
   free(pbuffer);
	pbuffer = NULL;

 KEY_Init();
}
/******************************************************************
函数名:  boolean_t InhibitonStatus(uint16_t keyInhibitonNum)
参数:
             keyInhibitonNum:拟制关系状态
            				bit7:up和down按键拟制关系
            				bit6:KEY_EAST_WEST
            				bit5:KEY_SOUTH_NORTH
            				bit4:KEY_OUT1_OUT2 
            				bit4:KEY_OUT3_OUT4
            	bit状态:
            				0:拟制
            				1:不拟制
*******************************************************************/
boolean_t InhibitonStatus(uint16_t keyInhibitonNum)
{
  boolean_t tstatus;
  params_typedef *tmp=system_params_get();	  
  switch(keyInhibitonNum)
  {
  case KEY_UP_DOWN_INCH_BIT:
    {
      if(tmp->inhibition&KEY_UP_DOWN_INCH_BIT)
        tstatus = 1;
      else 
        tstatus = 0;				
    }break;
  case KEY_EAST_WEST_INCH_BIT:
    {
      if(tmp->inhibition&KEY_EAST_WEST_INCH_BIT)
        tstatus = 1;
      else 
        tstatus = 0;				
    }break;	
  case KEY_SOUTH_NORTH_INCH_BIT:
    {
      if(tmp->inhibition&KEY_SOUTH_NORTH_INCH_BIT)
        tstatus = 1;
      else 
        tstatus = 0;				
    }break;	
    
  case KEY_OUT1_OUT2_INCH_BIT:
    {
      if(tmp->inhibition&KEY_OUT1_OUT2_INCH_BIT)
        tstatus = 1;
      else 
        tstatus = 0;				
    }break;	
  case KEY_OUT3_OUT4_INCH_BIT:
    {
      if(tmp->inhibition&KEY_OUT3_OUT4_INCH_BIT)
        tstatus = 1;
      else 
        tstatus = 0;				
    }break;	
  default: tstatus=1;break;
  }	
  return tstatus;
}
/******************************************************************
函数名: command_key_anlyz()
函数功能:判断按键是否被按下，如果被按下根据拟
制关系以及按键状态通过rf发送命令

*******************************************************************/
void command_key_anlyz()
{
  uint16_t *tKeyStatus;
  uint16_t  key_setting_status;
  params_typedef *tmp;
  tmp=system_params_get();
  tKeyStatus=KeyStaus();
  key_setting_status=tmp->KEY_H8<<8+tmp->KEY_L8;
  
  if(*tKeyStatus!=0)
  {
    //up down inch 
    if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//非拟制状态
    {
      if(key_setting_status&KEY_UP_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_UP_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_UP_BIT)==0)
          {
            CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_UP_BIT;
          }
          else
          {
            CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_UP_BIT);					
          }   
          *tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);	
        }
      }//end if(key_setting_status&KEY_UP_BIT)//自锁状态
      if(key_setting_status&KEY_DOWN_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_DOWN_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_DOWN_BIT)==0)
          {
            CommandTx(KEY_DOWN_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_DOWN_BIT;
          }
          else
          {
            CommandTx(KEY_DOWN_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_DOWN_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_DOWN_BIT);		
        }
      }
      
    }//end  if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//非拟制状态
    else //拟制状态
    {
      if((*tKeyStatus&KEY_UP_BIT)&&(*tKeyStatus&KEY_DOWN_BIT))
      {
        CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,1);
        *tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);
      }
      else
        ;
    }
    /*******************************************************************/
    /*******************************************************************/	 
    //EAST WEST inch 
    if(InhibitonStatus(KEY_EAST_WEST_INCH_BIT))
    {
      if(key_setting_status&KEY_EAST_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_EAST_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_EAST_BIT)==0)
          {
            CommandTx(KEY_EAST_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_EAST_BIT;
          }
          else
          {
            CommandTx(KEY_EAST_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_EAST_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_EAST_BIT);		
        }
      }	
      if(key_setting_status&KEY_WEST_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_WEST_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_WEST_BIT)==0)
          {
            CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_WEST_BIT;
          }
          else
          {
            CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_WEST_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_WEST_BIT);		
        }
      }
      //else if(*tKeyStatus&KEY_WEST_BIT)
      //{
      //CommandTx(KEY_WEST_BIT);
      //*tKeyStatus=*tKeyStatus&(~KEY_WEST_BIT);
      //}	
    }
    else 
    {
      if((*tKeyStatus&KEY_EAST_BIT)&&(*tKeyStatus&KEY_WEST_BIT))
      {
        CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,1);
        *tKeyStatus=*tKeyStatus&(~KEY_WEST_BIT);
      }
      else
        ;
    }
    /*******************************************************************/
    /*******************************************************************/	 
    //SOUTH NORTH inch
    if(InhibitonStatus(KEY_SOUTH_NORTH_INCH_BIT)) 
    {
      if(key_setting_status&KEY_SOUTH_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_SOUTH_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT)==0)
          {
            CommandTx(KEY_SOUTH_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT;
          }
          else
          {
            CommandTx(KEY_SOUTH_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_SOUTH_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_SOUTH_BIT);		
        }
      }
      if(key_setting_status&KEY_NORTH_BIT)//自锁状态
      {
        if(*tKeyStatus&KEY_NORTH_BIT)//按键按下
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_NORTH_BIT)==0)
          {
            CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_NORTH_BIT;
          }
          else
          {
            CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_NORTH_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);		
        }
      }		   
      //else if(*tKeyStatus&KEY_NORTH_BIT)
      //{
      //	CommandTx(KEY_NORTH_BIT);
      //	*tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);			
      //}
    }
    else 
    {
      if((*tKeyStatus&KEY_SOUTH_BIT)&&(*tKeyStatus&KEY_NORTH_BIT))
      {
        CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,1);
        *tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);
      }
      else
        ;
    }
    /*******************************************************************/
    /*******************************************************************/	     
    //OUT1 OUT2 NORTH inch 
    if(InhibitonStatus(KEY_OUT1_OUT2_INCH_BIT))
    {
      if(*tKeyStatus&KEY_OUT1_BIT)//按键按下
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT1_BIT)==0)
        {
          CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT1_BIT;
        }
        else
        {
          CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT1_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);		
      }
      if(*tKeyStatus&KEY_OUT2_BIT)//按键按下
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT2_BIT)==0)
        {
          CommandTx(KEY_OUT2_BIT,COMMAND_FUNC_CTRL,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT2_BIT;
        }
        else
        {
          CommandTx(KEY_OUT2_BIT,COMMAND_FUNC_CTRL,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT2_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT2_BIT);		
      }			
      //else if(*tKeyStatus&KEY_OUT2_BIT)
      //{
      //	CommandTx(KEY_OUT2_BIT);
      //	*tKeyStatus=*tKeyStatus&(~KEY_OUT2_BIT);			
      //}		
    }
    else 
    {
      if((*tKeyStatus&KEY_OUT1_BIT)&&(*tKeyStatus&KEY_OUT2_BIT))
      {
        CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,1);
        *tKeyStatus=*tKeyStatus&(~KEY_OUT1_BIT);
      }
      else
        ;
    }
    /*******************************************************************/
    /*******************************************************************/	     
    //OUT3 OUT4 NORTH inch
    if(InhibitonStatus(KEY_OUT3_OUT4_INCH_BIT)) 
    {
      if(*tKeyStatus&KEY_OUT3_BIT)//按键按下
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT3_BIT)==0)
        {
          CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT3_BIT;
        }
        else
        {
          CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT3_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);		
      }
      if(*tKeyStatus&KEY_OUT4_BIT)//按键按下
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT4_BIT)==0)
        {
          CommandTx(KEY_OUT4_BIT,COMMAND_FUNC_CTRL,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT4_BIT;
        }
        else
        {
          CommandTx(KEY_OUT4_BIT,COMMAND_FUNC_CTRL,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT4_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT4_BIT);		
      }			
      
    }
    else 
    {
      if((*tKeyStatus&KEY_OUT3_BIT)&&(*tKeyStatus&KEY_OUT4_BIT))
      {
        CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,1);
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);
      }
      else
        ;
    }	
  }
}
unsigned char WorkModeStatus;
void WorkMode()
{
KEY_Init();
uint16_t *keystatus_flag=KeyStaus();
if(*keystatus_flag&0xffff==KEY_STOP_BIT)
{
	
}
switch(*keystatus_flag&0xffff)
{
	case KEY_STOP_BIT:WorkModeStatus = CONFIGSN;break;
	case KEY_START_BIT:
	{
		if(WorkModeStatus&BREAKWORK)
			WorkModeStatus = WorkModeStatus|STARTWORK;
	}break;	
	case KEY_BREAK_BIT:WorkModeStatus = WorkModeStatus|BREAKWORK;break;	
}
if(ee_CheckOk()==0)//无卡片
  {
    UART_Config();
    uartPrase();  
    command_process();
  }
  else//有卡片
  {
    //uint8_t *pReadBuf= malloc(12);
      uint8_t *pReadBuf ;
    boolean_t readEEPROM_flag=0;
     params_typedef *params=system_params_get();
    if(ee_ReadBytes(pReadBuf, 0, 12)==1&&readEEPROM_flag==0)
    {   
      if(pReadBuf[0]==0x5a)//空卡
      { 
        
        if(readEEPROM_flag == 0)
     	{
          memcpy(&params,&pReadBuf[1],11);
          readEEPROM_flag = 1;
        }
        
        //params->KEY_H8 = pReadBuf[1];
        //params->KEY_L8 =  pReadBuf[2];
        //params->inhibition =  pReadBuf[3];
        //params->sn[0] = pReadBuf[4];
        //params->sn[1] = pReadBuf[5];
        //params->sn[2] = pReadBuf[6];
        //params->sn[3] = pReadBuf[7];
        //params->freq =  pReadBuf[8]<<8 + pReadBuf[9];
      }
      else
      {
        pReadBuf[0] =0x5a;
        memcpy(&pReadBuf[1],&params,11);
        ee_WriteBytes(pReadBuf, 0, 12);
      }
      UART_Config();
      uartPrase();  
      command_process();
    }//有卡片
      
    CMT2300_Init();
    setup_Rx();
    loop_Rx();
}
}
/********************************************************************
函数名command_process


*******************************************************************/
boolean_t command_process()
{
  command_key_anlyz();
  WorkMode();
  ;
  return 1;
}
