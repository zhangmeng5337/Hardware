/*
                  版权所有 (C), 2020, 北京迪文科技有限公司

 ******************************************************************************
  作    者   : Cui           
  生成日期   : 2021年10月14日
  功能描述   : 
	联系方式   ：18202831680
******************************************************************************/
#include  "Function_H.h"
u8     Flaga=0;
//bit Compare_Dgus(u16 addr1,u16 addr2,u8 len)//比较两个Dgus变量的值，相等返回1
//{
//			u16  temp1=0,temp2=0;
//	    u8   i=0,j=0;
//	    for(i=0;i<len;i++) 
//			{
//				 temp1=Read_Dgus(addr1+i);			//临时1 
//			   temp2=Read_Dgus(addr2+i);			//临时2
//			   if(temp1==temp2) j++;			   	//比较
//			}		    
//      if(j==len)									//获取字符串长度
//				 return  1;							//返回 1
//			else
//				 return  0; 						//返回 0
//}
// ======================================
void  Norflash_Read(u16 lib_addr,u16 Dgus_addr,u16 len)//所有参数需是偶数
{
			Write_Dgus(0x000b,len);
			Write_Dgus(0x000a,Dgus_addr);
			Write_Dgus(0x0009,lib_addr);
			Write_Dgus(0x0008,0x5A00);//Norflash 0号区域
			while(Read_Dgus(0X0008)&0xFF00);		
}
// ======================================
void  Norflash_Write(u16 lib_addr,u16 Dgus_addr,u16 len)//所有参数需是偶数
{
			Write_Dgus(0x000b,len);
			Write_Dgus(0x000a,Dgus_addr);
			Write_Dgus(0x0009,lib_addr);
			Write_Dgus(0x0008,0xA500);//Norflash 0号区域
			while(Read_Dgus(0X0008)&0xFF00);		
}
// ======================================
//void  Norflash_Read_2(u16 lib_addr,u16 Dgus_addr,u16 len)//所有参数需是偶数,
//{
//			Write_Dgus(0x000b,len);
//			Write_Dgus(0x000a,Dgus_addr);
//			Write_Dgus(0x0009,lib_addr);
//			Write_Dgus(0x0008,0x5A02);//Norflash 2号区域
//			while(Read_Dgus(0X0008)&0xFF00);		
//}
//// ======================================
//void  Norflash_Write_2(u16 lib_addr,u16 Dgus_addr,u16 len)//所有参数需是偶数
//{
//			Write_Dgus(0x000b,len);
//			Write_Dgus(0x000a,Dgus_addr);
//			Write_Dgus(0x0009,lib_addr);
//			Write_Dgus(0x0008,0xA502);//Norflash 2号区域
//			while(Read_Dgus(0X0008)&0xFF00);		
//}
// ======================================
void Function_Init()   
{    

//			if(Read_Dgus(Start_ADD+10)==0x5AA5)
//			{ 
////	          Norflash_Read(Start_ADD+18,Start_ADD+18,2);
//			      Norflash_Read(0x1200,0x1200,2);
//				 		Norflash_Read(0x1204,0x1204,12); //密码初始化 
//			}
//			else
//			{
////			     Norflash_Write(Start_ADD+18,Start_ADD+18,2);
//				   Write_Dgus(Start_ADD+10,0x5AA5);
//           Write_Dgus(0x1200,5);					
//			     Norflash_Write(0x1200,0x1200,2);
//				   Norflash_Write(0x1204,0x1204,12);//密码初始化 
//			     Norflash_Write(Start_ADD+10,Start_ADD+10,2);
//			 }
////			Write_Dgus(Start_ADD+17,1);
//			//=====================屏保初始化		
//			number_one=Read_Dgus(0x1200)*60;	
//			Write_Dgus(0x1201,1);	
/******************************************************************************/			 
}
/******************************************************************************/
void  Data_Send_Light()
{
			u16  V1=Read_Dgus(0X3040);  
        			u8 i=0;
				   u8 Val[]={0X5A,0XA5,0X06,0X83,0X30,0X40,0X01,0X00,0X00}; 	   //5A A5 06 83 ADDR 01 XX XX 
				   Val[7]=(u8)(V1>>8); Val[8]=(u8)(V1);
				   for(i=0;i<9;i++)Uart2_Tx_write2buff(Val[i]);


}
/******************************************************************************/
void  Function_Screen_Protection()  //屏保程序
{
      
	    u16    V2=Read_Dgus(0x1201);				 //
	     if(V2)
	     {
				    if(number_one)
            {
					     if(T_NUMBER==0)
			         {									
			            number_one--;
					        T_NUMBER=1000;//                定时器2延时1S									 
			         }
				    }
				    else
					  {
					      Write_Dgus(0x0085,1);
			          if(Read_Dgus(0x131C)&0x4)  {Write_Dgus(0x0084,0x5a01);Write_Dgus(0x1303,1);}  //切页
                if(Read_Dgus(0x131C)&0x10)  Write_Dgus(0x0082,0x1000);	  //息屏
						  	if(Read_Dgus(0x131C)&0x8)  {Write_Dgus(0x3040,20);Data_Send_Light();}        //熄灯
                Write_Dgus(0x1201,0);								
						}
				}

			  if(Read_Dgus(0x0016)&0xFF00)
				{
		        number_one=300;				  
					  Write_Dgus(0x1201,1);
					  if(Read_Dgus(0x131C)&0x10)   Write_Dgus(0x0082,0x6400);        //亮屏
					  if(Read_Dgus(0x131C)&0x8)   {Write_Dgus(0x3040,19); T_NUMBER_1=1000;Flaga=1;}           //亮灯
					  Write_Dgus(0x0016,0);
					  T_NUMBER=1000;
				}
				if(T_NUMBER_1==0&&Flaga==1)//延迟1秒发亮灯指令
				{
				    Data_Send_Light();
					  Flaga=0;
				}
//				if(Read_Dgus(0x1202))
//				{
//			    	Norflash_Write(0x1200,0x1200,2);
//			    	Write_Dgus(0x1202,0);	
//				}
}
/****************************************************************************/
void  Pass_WD1()
{			
      u16  Va1=Read_Dgus(0x1322);   //密码输入1
			u16  Va2=Read_Dgus(0x1323);   //密码输入2
			if((Va1>0)||(Va2>0))
			{
	
						u16  Page=26;
						if((Va1==0x0004)&&(Va2==0x50CC))  Page=20;   //	初始密码
						Write_Dgus(0x0085,Page);
			      Write_Dgus(0x0084,0x5a01);
						Write_Dgus(0x1322,0);
			      Write_Dgus(0x1323,0);
			}
}
// ======================================
void  Lock_Light()                   //锁加照明程序
{			
      u16  Va1=Read_Dgus(0x1320);   //锁
			u16  Va2=Read_Dgus(0x1321);   //照明
			if(Va1>0)
			{

           if(Read_Dgus(0x1303)==1)
           {						 
						   Write_Dgus(0x1303,0);
					 		 Write_Dgus(0x0085,0);
			         Write_Dgus(0x0084,0x5a01);
					 }
					 else
           {						 
						   Write_Dgus(0x1303,1);
						   Write_Dgus(0x0085,1);
			         Write_Dgus(0x0084,0x5a01);
					 }
						Write_Dgus(0x1320,0);

			}
				if(Va2>0)
			{

           if(Read_Dgus(0x3040)==19) 
						   Write_Dgus(0x3040,20);
					 else 
						   Write_Dgus(0x3040,19);
					  Data_Send_Light();
			      Write_Dgus(0x1321,0);
			}
}
// ======================================

// ======================================




//===================================	
void    Function_All()
{					
	    Pass_WD1();
      Lock_Light();



	    Function_Screen_Protection();

}
//===================================	