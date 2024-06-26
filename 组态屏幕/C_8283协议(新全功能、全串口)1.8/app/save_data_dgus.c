#include "save_data_dgus.h"
u16  xdata  Power_Down_Save_Address[]={0x6000,0x6001,0x10DA,0x10DA,0x10DA,0x10DA,0x10DA,0x10DA,0x10DA,0x10DA,0x10D6,0x10D7,0x10D8,0x10D9,
                                       0x10DA,0x10DB,0x10DC,0x10DD,0x10DE};//掉电保存地址
u16  xdata  Power_Down_Save_Data[]={0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//第一次上电数据初始化
#define	    SAVE_NUMBER      2  //需要保存的地址数量



#define     SEND_FINISH 3
#define     INITIALIZATION_MARK 	  0X5AAA
#define     START_ADD(a)            (0xE270+a)
#define     READ_MOD                 0x5A
#define     WRITE_MOD                0xA5


/*****************************************************************************
函数功能：初始化
第一次使用保存默认数据，后续从数据库读取数据
*/	
void data_save_init()   
{    
	    u16 Ladd=0,Va1=0,Va2=0,i=0; 
			T5L_Flash(READ_MOD,START_ADD(10),START_ADD(10),2);
			if(Read_Dgus(START_ADD(10))==INITIALIZATION_MARK)
			{                    
            	
                   for(i=0;i<SAVE_NUMBER;i++)//掉电保存参数设置
				   {
						T5L_Flash(READ_MOD,Power_Down_Save_Address[i],Power_Down_Save_Address[i],2);
						Power_Down_Save_Data[i]= Read_Dgus(Power_Down_Save_Address[i]);
				   }
			}
			else   //第一次使用				
			{	
				   	Write_Dgus(START_ADD(10),INITIALIZATION_MARK);
				   T5L_Flash(WRITE_MOD,START_ADD(10),START_ADD(10),2);
                   for(i=0;i<SAVE_NUMBER;i++)//掉电保存参数设置
				   {
						Write_Dgus(Power_Down_Save_Address[i],Power_Down_Save_Data[i]);//初始化变量
                        T5L_Flash(WRITE_MOD,Power_Down_Save_Address[i],Power_Down_Save_Address[i],2);
				   }        					
			}                         
		
}
/*****************************************************************************
函数功能：数据掉电保存
数据改变保存框架
*/
void data_change_sava()
{
	  u16 Val=0;
      u16 i=0,Va2=0;			   
	  for(i=0;i<SAVE_NUMBER;i++)
	  {
			Va2= Read_Dgus(Power_Down_Save_Address[i]);//获取需要掉电保存得地址的值
			if(Power_Down_Save_Data[i]!=Va2)          //掉电保存地址的值与原来值对比数值变化保存
			{
				Power_Down_Save_Data[i]=Va2;            //记录改变后的值
				T5L_Flash(WRITE_MOD,Power_Down_Save_Address[i],Power_Down_Save_Address[i],2);//保存数据
			}
	  }		
}