/*
                  ��Ȩ���� (C), 2020, �������ĿƼ����޹�˾

 ******************************************************************************
  ��    ��   : Cui           
  ��������   : 2021��10��14��
  ��������   : 
	��ϵ��ʽ   ��18202831680
******************************************************************************/
#include  "Function_H.h"
u8     Flaga=0;
//bit Compare_Dgus(u16 addr1,u16 addr2,u8 len)//�Ƚ�����Dgus������ֵ����ȷ���1
//{
//			u16  temp1=0,temp2=0;
//	    u8   i=0,j=0;
//	    for(i=0;i<len;i++) 
//			{
//				 temp1=Read_Dgus(addr1+i);			//��ʱ1 
//			   temp2=Read_Dgus(addr2+i);			//��ʱ2
//			   if(temp1==temp2) j++;			   	//�Ƚ�
//			}		    
//      if(j==len)									//��ȡ�ַ�������
//				 return  1;							//���� 1
//			else
//				 return  0; 						//���� 0
//}
// ======================================
void  Norflash_Read(u16 lib_addr,u16 Dgus_addr,u16 len)//���в�������ż��
{
			Write_Dgus(0x000b,len);
			Write_Dgus(0x000a,Dgus_addr);
			Write_Dgus(0x0009,lib_addr);
			Write_Dgus(0x0008,0x5A00);//Norflash 0������
			while(Read_Dgus(0X0008)&0xFF00);		
}
// ======================================
void  Norflash_Write(u16 lib_addr,u16 Dgus_addr,u16 len)//���в�������ż��
{
			Write_Dgus(0x000b,len);
			Write_Dgus(0x000a,Dgus_addr);
			Write_Dgus(0x0009,lib_addr);
			Write_Dgus(0x0008,0xA500);//Norflash 0������
			while(Read_Dgus(0X0008)&0xFF00);		
}
// ======================================
//void  Norflash_Read_2(u16 lib_addr,u16 Dgus_addr,u16 len)//���в�������ż��,
//{
//			Write_Dgus(0x000b,len);
//			Write_Dgus(0x000a,Dgus_addr);
//			Write_Dgus(0x0009,lib_addr);
//			Write_Dgus(0x0008,0x5A02);//Norflash 2������
//			while(Read_Dgus(0X0008)&0xFF00);		
//}
//// ======================================
//void  Norflash_Write_2(u16 lib_addr,u16 Dgus_addr,u16 len)//���в�������ż��
//{
//			Write_Dgus(0x000b,len);
//			Write_Dgus(0x000a,Dgus_addr);
//			Write_Dgus(0x0009,lib_addr);
//			Write_Dgus(0x0008,0xA502);//Norflash 2������
//			while(Read_Dgus(0X0008)&0xFF00);		
//}
// ======================================
void Function_Init()   
{    

//			if(Read_Dgus(Start_ADD+10)==0x5AA5)
//			{ 
////	          Norflash_Read(Start_ADD+18,Start_ADD+18,2);
//			      Norflash_Read(0x1200,0x1200,2);
//				 		Norflash_Read(0x1204,0x1204,12); //�����ʼ�� 
//			}
//			else
//			{
////			     Norflash_Write(Start_ADD+18,Start_ADD+18,2);
//				   Write_Dgus(Start_ADD+10,0x5AA5);
//           Write_Dgus(0x1200,5);					
//			     Norflash_Write(0x1200,0x1200,2);
//				   Norflash_Write(0x1204,0x1204,12);//�����ʼ�� 
//			     Norflash_Write(Start_ADD+10,Start_ADD+10,2);
//			 }
////			Write_Dgus(Start_ADD+17,1);
//			//=====================������ʼ��		
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
void  Function_Screen_Protection()  //��������
{
      
	    u16    V2=Read_Dgus(0x1201);				 //
	     if(V2)
	     {
				    if(number_one)
            {
					     if(T_NUMBER==0)
			         {									
			            number_one--;
					        T_NUMBER=1000;//                ��ʱ��2��ʱ1S									 
			         }
				    }
				    else
					  {
					      Write_Dgus(0x0085,1);
			          if(Read_Dgus(0x131C)&0x4)  {Write_Dgus(0x0084,0x5a01);Write_Dgus(0x1303,1);}  //��ҳ
                if(Read_Dgus(0x131C)&0x10)  Write_Dgus(0x0082,0x1000);	  //Ϣ��
						  	if(Read_Dgus(0x131C)&0x8)  {Write_Dgus(0x3040,20);Data_Send_Light();}        //Ϩ��
                Write_Dgus(0x1201,0);								
						}
				}

			  if(Read_Dgus(0x0016)&0xFF00)
				{
		        number_one=300;				  
					  Write_Dgus(0x1201,1);
					  if(Read_Dgus(0x131C)&0x10)   Write_Dgus(0x0082,0x6400);        //����
					  if(Read_Dgus(0x131C)&0x8)   {Write_Dgus(0x3040,19); T_NUMBER_1=1000;Flaga=1;}           //����
					  Write_Dgus(0x0016,0);
					  T_NUMBER=1000;
				}
				if(T_NUMBER_1==0&&Flaga==1)//�ӳ�1�뷢����ָ��
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
      u16  Va1=Read_Dgus(0x1322);   //��������1
			u16  Va2=Read_Dgus(0x1323);   //��������2
			if((Va1>0)||(Va2>0))
			{
	
						u16  Page=26;
						if((Va1==0x0004)&&(Va2==0x50CC))  Page=20;   //	��ʼ����
						Write_Dgus(0x0085,Page);
			      Write_Dgus(0x0084,0x5a01);
						Write_Dgus(0x1322,0);
			      Write_Dgus(0x1323,0);
			}
}
// ======================================
void  Lock_Light()                   //������������
{			
      u16  Va1=Read_Dgus(0x1320);   //��
			u16  Va2=Read_Dgus(0x1321);   //����
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