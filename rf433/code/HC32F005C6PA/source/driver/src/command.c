//#include "command.h"
#include "base_types.h"
#include "key.h"
#include "uartParse.h"
uint16_t CommandStatusToggleFlag=0;
#define KEY_UP_BIT               	0x8000
#define KEY_DOWN_BIT         	0x4000
#define KEY_EAST_BIT           	0x2000
#define KEY_WEST_BIT          	0x1000
#define KEY_SOUTH_BIT        	0x0800
#define KEY_NORTH_BIT        	0x0400
#define KEY_START_BIT         	0x0200
#define KEY_STOP_BIT           	0x0100

#define KEY_OUT1_BIT         	0x80
#define KEY_OUT2_BIT         	0x40
#define KEY_OUT3_BIT         	0x20
#define KEY_OUT4_BIT         	0x10
#define KEY_BREAK_BIT        	0x08
void  CommandTx(uint16_t key_num,boolean_t toggle_flag)
{

}
/********************************************************************
å‡½æ•°åï¼šboolean_t InhibitonStatus(uint16_t keyInhibitonNum)
å‡½æ•°åŠŸèƒ½ï¼šæ‹Ÿåˆ¶å…³ç³»åˆ¤æ–­
å‚æ•°ï¼škeyInhibitonNum
KEY_UP_DOWN_INCH_BITï¼šupï¼Œdownæ‹Ÿåˆ¶ç¼–å·
è¿”å›å€¼ï¼š
1:éæ‹Ÿåˆ¶å…³ç³»
0ï¼šæ‹Ÿåˆ¶å…³ç³»

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
    if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//·ÇÄâÖÆ×´Ì¬
    {
      if(key_setting_status&KEY_UP_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_UP_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_UP_BIT)==0)
          {
            CommandTx(KEY_UP_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_UP_BIT;
          }
          else
          {
            CommandTx(KEY_UP_BIT,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_UP_BIT);					
          }   
          *tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);	
        }
      }//end if(key_setting_status&KEY_UP_BIT)//×ÔËø×´Ì¬
      if(key_setting_status&KEY_DOWN_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_DOWN_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_DOWN_BIT)==0)
          {
            CommandTx(KEY_DOWN_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_DOWN_BIT;
          }
          else
          {
            CommandTx(KEY_DOWN_BIT,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_DOWN_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_DOWN_BIT);		
        }
      }
      
    }//end  if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))//·ÇÄâÖÆ×´Ì¬
    else //ÄâÖÆ×´Ì¬
    {
      if((*tKeyStatus&KEY_UP_BIT)&&(*tKeyStatus&KEY_DOWN_BIT))
      {
        CommandTx(KEY_UP_BIT);
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
      if(key_setting_status&KEY_EAST_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_EAST_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_EAST_BIT)==0)
          {
            CommandTx(KEY_EAST_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_EAST_BIT;
          }
          else
          {
            CommandTx(KEY_EAST_BIT,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_EAST_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_EAST_BIT);		
        }
      }	
      if(key_setting_status&KEY_WEST_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_WEST_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_WEST_BIT)==0)
          {
            CommandTx(KEY_WEST_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_WEST_BIT;
          }
          else
          {
            CommandTx(KEY_WEST_BIT,1);
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
        CommandTx(KEY_WEST_BIT);
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
      if(key_setting_status&KEY_SOUTH_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_SOUTH_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT)==0)
          {
            CommandTx(KEY_SOUTH_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_SOUTH_BIT;
          }
          else
          {
            CommandTx(KEY_SOUTH_BIT,1);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_SOUTH_BIT);					
          }
          
          *tKeyStatus=*tKeyStatus&(~KEY_SOUTH_BIT);		
        }
      }
      if(key_setting_status&KEY_NORTH_BIT)//×ÔËø×´Ì¬
      {
        if(*tKeyStatus&KEY_NORTH_BIT)//°´¼ü°´ÏÂ
        {	   
          if((tmp->CommandStatusToggleFlag|KEY_NORTH_BIT)==0)
          {
            CommandTx(KEY_NORTH_BIT,0);
            tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_NORTH_BIT;
          }
          else
          {
            CommandTx(KEY_NORTH_BIT,1);
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
        CommandTx(KEY_NORTH_BIT);
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
      if(*tKeyStatus&KEY_OUT1_BIT)//°´¼ü°´ÏÂ
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT1_BIT)==0)
        {
          CommandTx(KEY_OUT1_BIT,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT1_BIT;
        }
        else
        {
          CommandTx(KEY_OUT1_BIT,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT1_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);		
      }
      if(*tKeyStatus&KEY_OUT2_BIT)//°´¼ü°´ÏÂ
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT2_BIT)==0)
        {
          CommandTx(KEY_OUT2_BIT,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT2_BIT;
        }
        else
        {
          CommandTx(KEY_OUT2_BIT,1);
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
        CommandTx(KEY_OUT1_BIT);
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
      if(*tKeyStatus&KEY_OUT3_BIT)//°´¼ü°´ÏÂ
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT3_BIT)==0)
        {
          CommandTx(KEY_OUT3_BIT,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT3_BIT;
        }
        else
        {
          CommandTx(KEY_OUT3_BIT,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT3_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);		
      }
      if(*tKeyStatus&KEY_OUT4_BIT)//°´¼ü°´ÏÂ
      {	   
        if((tmp->CommandStatusToggleFlag|KEY_OUT4_BIT)==0)
        {
          CommandTx(KEY_OUT4_BIT,0);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag|KEY_OUT4_BIT;
        }
        else
        {
          CommandTx(KEY_OUT4_BIT,1);
          tmp->CommandStatusToggleFlag = tmp->CommandStatusToggleFlag&&(~KEY_OUT4_BIT);					
        }
        
        *tKeyStatus=*tKeyStatus&(~KEY_OUT4_BIT);		
      }			
      
    }
    else 
    {
      if((*tKeyStatus&KEY_OUT3_BIT)&&(*tKeyStatus&KEY_OUT4_BIT))
      {
        CommandTx(KEY_OUT3_BIT);
        *tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);
      }
      else
        ;
    }	
  }
}
  /********************************************************************
  å‡½æ•°åï¼šcommand_process
  å‡½æ•°åŠŸèƒ½ï¼šå‘½ä»¤å¤„ç†
  å‚æ•°ï¼škeyInhibitonNum
  
  *******************************************************************/
  boolean_t command_process()
  {
    command_key_anlyz();
    
    ;
  }