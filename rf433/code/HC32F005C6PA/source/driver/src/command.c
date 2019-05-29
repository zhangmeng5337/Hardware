//#include "command.h"
#include "base_types.h"
#include "key.h"

uint16_t CommandStatusToggleFlag=0;
/********************************************************************
函数名：boolean_t InhibitonStatus(uint16_t keyInhibitonNum)
函数功能：拟制关系判断
参数：keyInhibitonNum
KEY_UP_DOWN_INCH_BIT：up，down拟制编号
返回值：
      1:非拟制关系
      0：拟制关系

*******************************************************************/
boolean_t InhibitonStatus(uint16_t keyInhibitonNum)
{
      boolean_t tstatus;
      params_typedef * tmp=system_params_get();	  
      switch(keyInhibitonNum)
	  {
		case KEY_UP_DOWN_INCH_BIT:
		{
			if(tmp.inhibition&KEY_UP_DOWN_INCH_BIT)
				tstatus = 1;
			else 
				tstatus = 0;				
		}break;
		case KEY_EAST_WEST_INCH_BIT:
		{
			if(tmp.inhibition&KEY_EAST_WEST_INCH_BIT)
				tstatus = 1;
			else 
				tstatus = 0;				
		}break;	
		case KEY_SOUTH_NORTH_INCH_BIT:
		{
			if(tmp.inhibition&KEY_SOUTH_NORTH_INCH_BIT)
				tstatus = 1;
			else 
				tstatus = 0;				
		}break;	

		case KEY_OUT1_OUT2_INCH_BIT:
		{
			if(tmp.inhibition&KEY_OUT1_OUT2_INCH_BIT)
				tstatus = 1;
			else 
				tstatus = 0;				
		}break;	
		case KEY_OUT3_OUT4_INCH_BIT:
		{
			if(tmp.inhibition&KEY_OUT3_OUT4_INCH_BIT)
				tstatus = 1;
			else 
				tstatus = 0;				
		}break;	
		default tstatus=1;break;
	  }	
	  return tstatus;
}
/********************************************************************
函数名：command_process
函数功能：命令处理
参数：keyInhibitonNum

*******************************************************************/
boolean_t command_process()
{
  uint16_t *tKeyStatus;
  tKeyStatus=KeyStaus();

  if(*tKeyStatus!=0)
  {
    //up down inch 
	if(InhibitonStatus(KEY_UP_DOWN_INCH_BIT))
	{
		if(*tKeyStatus&KEY_UP_BIT)
		{
		   
			CommandTx(KEY_UP_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);		
		}
		else if(*tKeyStatus&KEY_DOWN_BIT)
		{
			CommandTx(KEY_DOWN_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_DOWN_BIT);			
		}
		else if((*tKeyStatus&KEY_UP_BIT)&&(*tKeyStatus&KEY_DOWN_BIT))
		{
		//CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_DOWN_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);
		}		
	}
	else 
	{
		if((*tKeyStatus&KEY_UP_BIT)&&(*tKeyStatus&KEY_DOWN_BIT))
		{
			CommandTx(KEY_UP_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_UP_BIT);
		}
		else
			;
	}
	
  //EAST WEST inch 
	if(InhibitonStatus(KEY_EAST_WEST_INCH_BIT))
	{
		if(*tKeyStatus&KEY_EAST_BIT)
		{
			CommandTx(KEY_EAST_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_EAST_BIT);
		}
		else if(*tKeyStatus&KEY_WEST_BIT)
		{
			CommandTx(KEY_WEST_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_WEST_BIT);
		}
		else if((*tKeyStatus&KEY_EAST_BIT)&&(*tKeyStatus&KEY_WEST_BIT))
		{
			//CommandTx(KEY_NORTH_BIT);
						//CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_WEST_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_EAST_BIT);
		}		
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
	
//SOUTH NORTH inch
	if(InhibitonStatus(KEY_SOUTH_NORTH_INCH_BIT)) 
	{
		if(*tKeyStatus&KEY_SOUTH_BIT)
		{
			CommandTx(KEY_SOUTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_SOUTH_BIT);
		}
		else if(*tKeyStatus&KEY_NORTH_BIT)
		{
			CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);			
		}
		else if((*tKeyStatus&KEY_SOUTH_BIT)&&(*tKeyStatus&KEY_NORTH_BIT))
		{
			//CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_SOUTH_BIT);			
		}
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
	
	//OUT1 OUT2 NORTH inch 
	if(InhibitonStatus(KEY_OUT1_OUT2_INCH_BIT))
	{
		if(*tKeyStatus&KEY_OUT1_BIT)
		{
			CommandTx(KEY_OUT1_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT1_BIT);
		}
		else if(*tKeyStatus&KEY_OUT2_BIT)
		{
			CommandTx(KEY_OUT2_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT2_BIT);			
		}
		else if((*tKeyStatus&KEY_OUT1_BIT)&&(*tKeyStatus&KEY_OUT2_BIT))
		{
			//CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT1_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT2_BIT);			
		}		
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

	//OUT3 OUT4 NORTH inch
	if(InhibitonStatus(KEY_OUT3_OUT4_INCH_BIT)) 
	{
		if(*tKeyStatus&KEY_OUT3_BIT)
		{
			CommandTx(KEY_OUT3_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);
		}
		else if(*tKeyStatus&KEY_OUT4_BIT)
		{
			CommandTx(KEY_OUT4_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT4_BIT);
		}
		else if((*tKeyStatus&KEY_OUT3_BIT)&&(*tKeyStatus&KEY_OUT4_BIT))
		{
			//CommandTx(KEY_NORTH_BIT);
			*tKeyStatus=*tKeyStatus&(~KEY_OUT3_BIT);
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
  
	if(CommandStatusToggleFlag&KEY_UP_BIT==0)
		params_typedef * tmp=system_params_get();	
}