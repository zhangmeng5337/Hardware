//#include "command.h"
#include "base_types.h"
#include "key.h"
#include "uartParse.h"
#include "command.h"
#include "main.h"
#include "crc.h"
#include <stdio.h>
#include <stdlib.h>
#include "bsp_uart.h"
#include "bsp_i2c_ee.h"
#include "led.h"
uint16_t CommandStatusToggleFlag=0;
void uart_ack_response(unsigned char  cmd_type)
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
  case READ_CMD:
    {
      pbuffer[0] = WAKE_HEAD;
      pbuffer[1] = WAKE_HEAD;
      pbuffer[2] = WAKE_HEAD;
      pbuffer[3] = WAKE_HEAD;
      pbuffer[4] = READ_CMD;
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
void rf_ack_response(unsigned char  cmd_type)
{
  uint8_t *pbuffer = malloc(9);
  
  switch(cmd_type)
  {
  case SNACK_CMD:
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
  }
  
}
/******************************************************************
������:  CommandTx(uint16_t key_num,uint8_t mode,boolean_t toggle_flag)
����:
key_num:�����µİ���
mode:����ģʽ
0:����ģʽ
1:����ģʽ
toggle_flag:�̵�������
0:�̵�������
1:�̵����Ͽ�
*******************************************************************/
void  CommandTx(uint16_t key_num,uint8_t mode,boolean_t toggle_flag,uint8_t TxCount)
{
  uint8_t *pbuffer = malloc(9);
  uint16_t crc_tmp;
  uint8_t len_tmp;
  
  if(mode == COMMAND_FUNC_SN_CONFIG)
  {
    *pbuffer++ = WAKE_HEAD;
    *pbuffer++ = mode;
    params_typedef *tmp=system_params_get();
    *pbuffer++ = tmp->sn[0];
    *pbuffer++ = tmp->sn[1];
    *pbuffer++ = tmp->sn[2];	
    *pbuffer++ = tmp->sn[3];
    *pbuffer++ =(uint8_t)( tmp->freq>>8);
    *pbuffer++ =(uint8_t)( tmp->freq);	
    len_tmp = COMMAND_FUNC_SN_CONFIG_LEN;
    crc_tmp = CRC16_Get8(pbuffer, len_tmp);
  }
  else
    *pbuffer++ = mode;	
  
  //����ģʽ
  if(mode == COMMAND_FUNC_CTRL)
  {
    //�������
    *pbuffer++ = (uint8_t)(key_num>>8);	
    *pbuffer++ = (uint8_t)(key_num);
    //�̵�������
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
  //����ģʽ
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
  
  //CMT2300_Init();
 // setup_Tx();
  unsigned char i;
  i=0;
  while(i<TxCount)
  {
    i++;
    //loop_Tx(pbuffer,len_tmp);
  }
  free(pbuffer);
  pbuffer = NULL;
   //setup_Rx();
  KEY_Init();
}
/******************************************************************
������:  boolean_t InhibitonStatus(uint16_t keyInhibitonNum)
����:
keyInhibitonNum:���ƹ�ϵ״̬
bit7:up��down�������ƹ�ϵ
bit6:KEY_EAST_WEST
bit5:KEY_SOUTH_NORTH
bit4:KEY_OUT1_OUT2 
bit4:KEY_OUT3_OUT4
bit״̬:
0:����
1:������
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
������: command_key_anlyz()
��������:�жϰ����Ƿ񱻰��£���������¸�����
�ƹ�ϵ�Լ�����״̬ͨ��rf��������

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
    if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//������״̬
    {
      if(key_setting_status&KEY_UP_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_UP_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_UP_BIT)==0)
          {
            CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_UP_BIT;
          }
          else
          {
            CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_UP_BIT);					
          }   
          *tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);	
        }
      }//end if(key_setting_status&KEY_UP_BIT)//����״̬
      if(key_setting_status&KEY_DOWN_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_DOWN_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_DOWN_BIT)==0)
          {
            CommandTx(KEY_DOWN_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_DOWN_BIT;
          }
          else
          {
            CommandTx(KEY_DOWN_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_DOWN_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_DOWN_BIT);		
        }
      }
      
    }//end  if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//������״̬
    else //����״̬
    {
      if((*tKeyStatus&KEY_UP_BIT)&&(*tKeyStatus&KEY_DOWN_BIT))
      {
        CommandTx(KEY_UP_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
      if(key_setting_status&KEY_EAST_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_EAST_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_EAST_BIT)==0)
          {
            CommandTx(KEY_EAST_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_EAST_BIT;
          }
          else
          {
            CommandTx(KEY_EAST_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_EAST_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_EAST_BIT);		
        }
      }	
      if(key_setting_status&KEY_WEST_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_WEST_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_WEST_BIT)==0)
          {
            CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_WEST_BIT;
          }
          else
          {
            CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
        CommandTx(KEY_WEST_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
      if(key_setting_status&KEY_SOUTH_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_SOUTH_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT)==0)
          {
            CommandTx(KEY_SOUTH_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT;
          }
          else
          {
            CommandTx(KEY_SOUTH_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_SOUTH_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_SOUTH_BIT);		
        }
      }
      if(key_setting_status&KEY_NORTH_BIT)//����״̬
      {
        if(*tKeyStatus&KEY_NORTH_BIT)//��������
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_NORTH_BIT)==0)
          {
            CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_NORTH_BIT;
          }
          else
          {
            CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
        CommandTx(KEY_NORTH_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
      if(*tKeyStatus&KEY_OUT1_BIT)//��������
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT1_BIT)==0)
        {
          CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT1_BIT;
        }
        else
        {
          CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT1_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);		
      }
      if(*tKeyStatus&KEY_OUT2_BIT)//��������
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT2_BIT)==0)
        {
          CommandTx(KEY_OUT2_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT2_BIT;
        }
        else
        {
          CommandTx(KEY_OUT2_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
        CommandTx(KEY_OUT1_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
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
      if(*tKeyStatus&KEY_OUT3_BIT)//��������
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT3_BIT)==0)
        {
          CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT3_BIT;
        }
        else
        {
          CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT3_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);		
      }
      if(*tKeyStatus&KEY_OUT4_BIT)//��������
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT4_BIT)==0)
        {
          CommandTx(KEY_OUT4_BIT,COMMAND_FUNC_CTRL,0,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT4_BIT;
        }
        else
        {
          CommandTx(KEY_OUT4_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT4_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT4_BIT);		
      }			
      
    }
    else 
    {
      if((*tKeyStatus&KEY_OUT3_BIT)&&(*tKeyStatus&KEY_OUT4_BIT))
      {
        CommandTx(KEY_OUT3_BIT,COMMAND_FUNC_CTRL,1,MAX_COUNT);
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);
      }
      else
        ;
    }	
  }
}
uint16_t  WorkModeStatus;
unsigned char  WorkMode()
{
  KEY_Init();
  uint16_t *keystatus_flag=KeyStaus();
  switch(*keystatus_flag&0xffff)
  {
  case KEY_STOP_BIT://ֹͣ����
    {
      if(WorkModeStatus&STARTWORK)
        WorkModeStatus = STOPWORK;//ֹͣģʽ
      else if((WorkModeStatus&BREAKWORK)==0&&(WorkModeStatus&STARTWORK==0))
        WorkModeStatus = CONFIGSN;//����ģʽ
      *keystatus_flag=*keystatus_flag&(~KEY_STOP_BIT);					
    }break;
  case KEY_START_BIT://��������
    {
      if(WorkModeStatus&BREAKWORK)
        WorkModeStatus = STARTWORK;//����ģʽ
      *keystatus_flag=*keystatus_flag&(~KEY_START_BIT);	
    }break;	
  case KEY_BREAK_BIT://�ɻɹ�
    {
      if(WorkModeStatus&BREAKWORK)
        WorkModeStatus = STOPWORK;//ֹͣģʽ
      else if(WorkModeStatus&BREAKWORK==0)
        WorkModeStatus = BREAKWORK;//һ������
      *keystatus_flag=*keystatus_flag&(~KEY_BREAK_BIT);		
    }break;
  }
  
  return WorkModeStatus;
}
boolean_t rfPrase(uint8_t *pbuffer,uint8_t len)
{
  rfparams_typedef *RfparamsInfo=0;
  //tmpuartReceiveriteIndex = uartReceiveLen;
  
  memcpy(RfparamsInfo,pbuffer,len);
  unsigned int crc=CRC16_Get8(&pbuffer[1],len-1);
  if(crc==RfparamsInfo->crc)
    //У����ȷ������ָ��
  {
    params_typedef *tmp=system_params_get();
    if(RfparamsInfo->sn == tmp->sn&&RfparamsInfo->freq==tmp->freq)//���ò���
      return 0;
    else 
      return 1;
  }
  else
    return 1;
}
/********************************************************************
������command_process


*******************************************************************/
boolean_t command_process()
{
  boolean_t rfAck_Flag=0;
  command_key_anlyz();
  if( WorkMode()==STARTWORK)//��������ģʽ
  {
    if(ee_CheckOk()==0)//�޿�Ƭ
    {
      UART_Config();
      uartPrase();  
      //command_process();
    }
    else//�п�Ƭ
    {
      //uint8_t *pReadBuf= malloc(12);
      uint8_t *pReadBuf=0;
      boolean_t readEEPROM_flag=0;
      params_typedef *params=system_params_get();
      if(ee_ReadBytes(pReadBuf, 0, 12)==1&&readEEPROM_flag==0)
      {   
        if(pReadBuf[0]==0x5a)//�ǿտ�
        { 
          
          if(readEEPROM_flag == 0)
          {
            memcpy(&params,&pReadBuf[1],11);
            readEEPROM_flag = 1;
          }
        }//�ǿտ�
        else//�տ�
        {
          pReadBuf[0] =0x5a;
          memcpy(&pReadBuf[1],&params,11);
          ee_WriteBytes(pReadBuf, 0, 12);
        }//�տ�
        UART_Config();
        uartPrase();  
        //command_process();
      }
      
    //  CMT2300_Init();
     // setup_Rx();
      // loop_Rx();
    }//�п�Ƭ
  }//��������ģʽ
  else if(WorkMode()==STOPWORK)//ͣ��ģʽ
  {
    uint16_t *keystatus_flag=KeyStaus();
    keystatus_flag = 0;
  }
  else if(WorkMode()==CONFIGSN)//����ģʽ
  {
    if(rfAck_Flag == 0)
    { 
      CommandTx(0,COMMAND_FUNC_SN_CONFIG,0,1);
      //CMT2300_Init();
     // setup_Rx();
    }
    unsigned char len;
    unsigned char *getstr=0;
   // len = loop_Rx(getstr);
    if(len!=0)
    {
      if(rfPrase(getstr, len)==0)
      {
        rfAck_Flag = 1;
        WorkModeStatus = 0;
      } 
    }
	
  }
  return 1;
}
