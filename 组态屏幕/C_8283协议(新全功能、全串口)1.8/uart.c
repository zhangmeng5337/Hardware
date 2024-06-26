/******************************************************************************

         版权所有 (C), 2019, 北京迪文科技有限公司

 ******************************************************************************
	文 件 名  : uart.c
	版 本 号  : V2.0
	作  者  	: LYN
	生成日期  : 2021年6月29日
	功能描述  : 串口函数
	修改历史  :
	1.日  期  : 
		作  者  : 
		修改内容  : 
******************************************************************************/

#include "Uart.h"

/********************************/

//u8 Uart2_Rx_Buffer[Uart2_Rx_Lenth];	//接收缓冲
//u8 Uart2_Tx_Buffer[Uart2_Tx_Lenth];	//发送缓冲
// #if UART2_ENABLE
struct Uartx_Define Uart2;
struct Uartx_Frame_Data Uart2_Frame;
//#endif
// #if UART3_ENABLE
struct Uartx_Define Uart3;
struct Uartx_Frame_Data Uart3_Frame;
//#endif
// #if UART4_ENABLE
struct Uartx_Define Uart4;
struct Uartx_Frame_Data Uart4_Frame;
//#endif
// #if UART5_ENABLE
struct Uartx_Define Uart5;
struct Uartx_Frame_Data Uart5_Frame;
//#endif
/***********************************************************************************************/
bit                 Response_flog=0;  //应答用
bit                 Auto_data_upload=0;//数据自动上传用
bit                 Crc_check_flog=0;//Crc校验标记
/*****************************************************************************
	函 数 名 : void Uart2_Init(u32 baud_rate)
	功能描述 : 串口2初始化
	输入参数 :	baud_rate 波特率
	输出参数 : 
	修改历史 :
	1.日  期  : 2021年9月30日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
 #if UART2_ENABLE
void Uart2_Init(u32 baud_rate){
	u16 i;

	Uart2.id = 2;
	Uart2.Tx_Read = 0;
	Uart2.Tx_Write = 0;
	Uart2.Tx_Busy = 0;
	Uart2.Rx_Read = 0;
	Uart2.Rx_Write = 0;
	Uart2.Rx_Busy = 0;
	Uart2.Rx_Flag = Uart_Rev_Pre;
	
	for(i = 0; i < Uart2_Tx_Lenth; i++) Uart2.Tx_Buffer[i] = 0;
	for(i = 0; i < Uart2_Rx_Lenth; i++) Uart2.Rx_Buffer[i] = 0;
  i=1024-FOSC/64/baud_rate;
	MUX_SEL |= 0x40;	//引出uart2
	P0MDOUT &= 0xCF;	//P0 0001 0000
	P0MDOUT |= 0x10;
	ADCON = 0x80;			//UART2 8N1 115200
	SCON0 = 0x50;
	SREL0H = (u8)(i>>8);		//波特率 = FCLK/64*(1024-SREL)
	SREL0L = (u8)i;    // S_2/115200=224=0X03E4 1024-206438400/(64*115200)=0X03E4  
	ES0 = 1;
	EA=1;	
}
#endif
/*****************************************************************************
	函 数 名 : void Uart3_Init(u32 baud_rate)
	功能描述 : 串口2初始化
	输入参数 :	baud_rate 波特率
	输出参数 : 
	修改历史 :
	1.日  期  : 2023年12月1日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
#if UART3_ENABLE
void Uart3_Init(u32 baud_rate){
    	u16 i=0;
	Uart3.id = 2;
	Uart3.Tx_Read = 0;
	Uart3.Tx_Write = 0;
	Uart3.Tx_Busy = 0;
	Uart3.Rx_Read = 0;
	Uart3.Rx_Write = 0;
	Uart3.Rx_Busy = 0;
	Uart3.Rx_Flag = Uart_Rev_Pre;
	
	for(i = 0; i < Uart3_Tx_Lenth; i++) Uart3.Tx_Buffer[i] = 0;
	for(i = 0; i < Uart3_Rx_Lenth; i++) Uart3.Rx_Buffer[i] = 0;	
      i=1024-FOSC/32/baud_rate;
	    SREL1H = (u8)(i>>8);		//波特率 = FCLK/64*(1024-SREL)
	    SREL1L = (u8)i;    // S_2/115200=224=0X03C8 1024-206438400/(32*115200)=0X03C8
			MUX_SEL|=0x20;    //UART3引出
			P0MDOUT|=0x40;	  //P0.6 TXD 推挽				 
			SCON1 = 0x90;     //接受使能和模式设置		 
			IEN2 |= 0x01;	  //中断使能		
     	EA  = 1;	
}
#endif
/*****************************************************************************
	函 数 名 : void Uart4_Init(u32 baud_rate)
	功能描述 : 串口4初始化
	输入参数 :	baud_rate 波特率
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
#if UART4_ENABLE
void Uart4_Init(u32 baud_rate){
	u16 i;

	Uart4.id = 2;
	Uart4.Tx_Read = 0;
	Uart4.Tx_Write = 0;
	Uart4.Tx_Busy = 0;
	Uart4.Rx_Read = 0;
	Uart4.Rx_Write = 0;
	Uart4.Rx_Busy = 0;
	Uart4.Rx_Flag = Uart_Rev_Pre;
	
	for(i = 0; i < Uart4_Tx_Lenth; i++) Uart4.Tx_Buffer[i] = 0;
	for(i = 0; i < Uart4_Rx_Lenth; i++) Uart4.Rx_Buffer[i] = 0;
	P0MDOUT |=0x03;
  i=FOSC/8/baud_rate;
	SCON2T= 0x80	;//发送使能和模式设置
	SCON2R= 0x80;//接受使能和模式设置 
	ES2R = 1;//中断接受使能
  ES2T = 1;//中断发送使能
	BODE2_DIV_H = (u8)(i>>8);		//波特率 = CPU 主频/(8*波特率) 
	BODE2_DIV_L = (u8)i;    // 
		    P0MDOUT|=(1<<0); //p0^0 强推
			TR4=0;
   EA=1;			
}
#endif
/*****************************************************************************
	函 数 名 : void Uart5_Init(u32 baud_rate)
	功能描述 : 串口5初始化
	输入参数 :	baud_rate 波特率
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
#if UART5_ENABLE
void Uart5_Init(u32 baud_rate){
	u16 i;

	Uart5.id = 2;
	Uart5.Tx_Read = 0;
	Uart5.Tx_Write = 0;
	Uart5.Tx_Busy = 0;
	Uart5.Rx_Read = 0;
	Uart5.Rx_Write = 0;
	Uart5.Rx_Busy = 0;
	Uart5.Rx_Flag = Uart_Rev_Pre;
	
	for(i = 0; i < Uart5_Tx_Lenth; i++) Uart5.Tx_Buffer[i] = 0;
	for(i = 0; i < Uart5_Rx_Lenth; i++) Uart5.Rx_Buffer[i] = 0;
	P0MDOUT |=0x03;
  i=FOSC/8/baud_rate;
	SCON3T= 0x80	;//发送使能和模式设置
	SCON3R= 0x80;//接受使能和模式设置 
	ES3R = 1;//中断接受使能
  ES3T = 1;//中断发送使能
	BODE3_DIV_H = (u8)(i>>8);		//波特率 = CPU 主频/(8*波特率) 
	BODE3_DIV_L = (u8)i;    // 
		    P0MDOUT|=(1<<1); //p0^1 强推
			TR5=0;
  EA=1;	
}
#endif
/******************************************************************************
串口初始化
*******************/
void uart_init(){
#if UART2_ENABLE
	          Uart2_Init(BAUD_UART2);
#endif	
#if UART3_ENABLE
            Uart3_Init(BAUD_UART3);
#endif	
#if UART4_ENABLE	
	          Uart4_Init(BAUD_UART4);
#endif	
#if UART5_ENABLE	
	          Uart5_Init(BAUD_UART5);
#endif	
}

/*************************************************************
函数名：void  uart_send_byte(u8 Uart_number,u8 Dat)
参数：Uart_number=串口号=2，3，4，5；Dat为需要发送的byte
作者：cuijia
日期：20220526
***********************************************************/
void  uart_send_byte(u8 Uart_number,u8 Dat)
{
     if(Uart_number==2){
    	    Uart2_Tx_write2buff(Dat); 		 
		 }else if(Uart_number==3){
		 		Uart3_Tx_write3buff(Dat); 
		 }else if(Uart_number==4){
	 		 Uart4_Tx_write4buff(Dat);
		 }else if(Uart_number==5){
	 		 Uart5_Tx_write5buff(Dat);
		 }
}
#if 1
/*************************************************************
函数名：void  uart_send_str(u8 U_number,u8* Str,u8 Len)
参数：U_number=串口号=2，3，4，5；Str需要发送的字符串，发送长度Len
作者：cuijia
日期：20230331
***********************************************************/
void  uart_send_str(u8 U_number,u8* Str,u8 Len)
{
    u8 i=0;
	       if(U_number==4)       TR4=1;
         else if(U_number==5)  TR5=1; 
	       for(i=0;i<Len;i++)
	      {
	             uart_send_byte(U_number,*Str++);
	      }

}
#endif




/*****************************************************************************
	函 数 名 : void Uart2_Tx_write2buff(u8 dat)
	功能描述 : 装载串口2发送缓冲
	输入参数 :	dat:预发送的数据
	输出参数 :
	修改历史 :
	1.日  期  : 2020年6月29日
		作  者  : LYN
		修改内容  : 创建
 *****************************************************************************/
void Uart2_Tx_write2buff(u8 dat){
	Uart2.Tx_Buffer[Uart2.Tx_Write] = dat;					//装发送缓冲

	if(Uart2.Tx_Read > Uart2.Tx_Write){							//缓冲队列写指针回头，读指针在前
		while((Uart2.Tx_Write) + 1 == Uart2.Tx_Read);	//若缓冲区满，等待读指针前进
	}

	++Uart2.Tx_Write;													//写指针前进
	if(Uart2.Tx_Write >= Uart2_Tx_Lenth){			//写指针即将回头
		while(Uart2.Tx_Read == 0);							//若读指针在头，等待读指针前进
		Uart2.Tx_Write = 0;											//写指针回头
	}

	if(Uart2.Tx_Busy == 0){				//空闲
		Uart2.Tx_Busy = 1;					//标志忙
		TI0 = 1;										//触发发送中断
	}
}
/*****************************************************************************
	函 数 名 : void Uart3_Tx_write3buff(u8 dat)
	功能描述 : 装载串口3发送缓冲
	输入参数 :	dat:预发送的数据
	输出参数 :
	修改历史 :
	1.日  期  : 2023年12月1日
		作  者  : LYN
		修改内容  : 创建
 *****************************************************************************/
void Uart3_Tx_write3buff(u8 dat){
	Uart3.Tx_Buffer[Uart3.Tx_Write] = dat;					//装发送缓冲

	if(Uart3.Tx_Read > Uart3.Tx_Write){							//缓冲队列写指针回头，读指针在前
		while((Uart3.Tx_Write) + 1 == Uart3.Tx_Read);	//若缓冲区满，等待读指针前进
	}

	++Uart3.Tx_Write;													//写指针前进
	if(Uart3.Tx_Write >= Uart3_Tx_Lenth){			//写指针即将回头
		while(Uart3.Tx_Read == 0);							//若读指针在头，等待读指针前进
		Uart3.Tx_Write = 0;											//写指针回头
	}

	if(Uart3.Tx_Busy == 0){				//空闲
		Uart3.Tx_Busy = 1;					//标志忙
		SCON1|=0x02 ;										//触发发送中断
	}
}
/*****************************************************************************
	函 数 名 : void Uart4_Tx_write4buff(u8 dat)
	功能描述 : 装载串口4发送缓冲
	输入参数 :	dat:预发送的数据
	输出参数 :
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修改内容  : 创建
 *****************************************************************************/
void Uart4_Tx_write4buff(u8 dat){
	Uart4.Tx_Buffer[Uart4.Tx_Write] = dat;					//装发送缓冲

	if(Uart4.Tx_Read > Uart4.Tx_Write){							//缓冲队列写指针回头，读指针在前
		while((Uart4.Tx_Write) + 1 == Uart4.Tx_Read);	//若缓冲区满，等待读指针前进
	}

	++Uart4.Tx_Write;													//写指针前进
	if(Uart4.Tx_Write >= Uart4_Tx_Lenth){			//写指针即将回头
		while(Uart4.Tx_Read == 0);							//若读指针在头，等待读指针前进
		Uart4.Tx_Write = 0;											//写指针回头
	}
    
	if(Uart4.Tx_Busy == 0){				//空闲
		Uart4.Tx_Busy = 1;					//标志忙
		TR4=1;          //   485开启发送
		SCON2T|=0x1 ; 										//触发发送中断
	}
}
/*****************************************************************************
	函 数 名 : void Uart5_Tx_write5buff(u8 dat)
	功能描述 : 装载串口4发送缓冲
	输入参数 :	dat:预发送的数据
	输出参数 :
	修改历史 :
	1.日  期  : 2023年11月30日
		作  者  : LYN
		修改内容  : 创建
 *****************************************************************************/
void Uart5_Tx_write5buff(u8 dat){
	Uart5.Tx_Buffer[Uart5.Tx_Write] = dat;					//装发送缓冲

	if(Uart5.Tx_Read > Uart5.Tx_Write){							//缓冲队列写指针回头，读指针在前
		while((Uart5.Tx_Write) + 1 == Uart5.Tx_Read);	//若缓冲区满，等待读指针前进
	}

	++Uart5.Tx_Write;													//写指针前进
	if(Uart5.Tx_Write >= Uart5_Tx_Lenth){			//写指针即将回头
		while(Uart5.Tx_Read == 0);							//若读指针在头，等待读指针前进
		Uart5.Tx_Write = 0;											//写指针回头
	}
    
	if(Uart5.Tx_Busy == 0){				//空闲
		Uart5.Tx_Busy = 1;					//标志忙
		TR5=1;          //   485开启发送
		SCON3T|=0x1 ; 										//触发发送中断
	}
}
/****************************
数据自动上传
************************************************/
void uart_data_send(u8* Arr,u8 Uartnum,bit Outo_send,bit Crc_ck )
{
	        u8   i=0; 
	        u16  V1=Read_Dgus(0x0f01);	        
          Auto_data_upload=Outo_send;
					if(Auto_data_upload) 
					{
						 Crc_check_flog=Crc_ck;						 
						 if(Crc_check_flog) //带Crc校验
						 {
							   u16 Crc=0;	
							   Arr[2] = (((u8)V1)<<1)+6;								   					 						 								   
   							 Crc=crc16table(Arr+3,Arr[2]-2);
		             Arr[Arr[2]+1]=Crc& 0x00FF;
		             Arr[Arr[2]+2]=Crc>> 8;	
                uart_send_str(Uartnum,Arr,Arr[2]+3);						 
							   Arr[2] = (((u8)V1)<<1)+4;
						 }else
             {
                  uart_send_str(Uartnum,Arr,Arr[2]+3);												 
						 }						 						 
					}
}
/*****************************************************************************
	函 数 名 : Read_0xF00()
	功能描述 : 串口2数据自动上传
	输入参数 :
	输出参数 :
	修改历史 :
	1.日  期  : 2021年10月13日
		作  者  : 
		修  改  : cuijia
 *****************************************************************************/
void Read_0xF00()
{
		  u16  Va=Read_Dgus(0x0f00);
			u16  V1=Read_Dgus(0x0f01);
			if(((u8)(Va>>8))==0x5A)
			{	   u8 i=0;u16 Temp=0;
				   u8 Val[100]={0}; 	   //5A A5 06 83 ADDR Len XX XX 
					 Val[0] = DTHD1;
		       Val[1] = DTHD2;
					 Val[2] = (((u8)V1)<<1)+4;
		       Val[3] = 0x83;
				   Val[4]=(u8)Va; Val[5]=(u8)(V1>>8);
					 Val[6]=(u8)V1;
					 for(i=0;i<(u8)V1;i++)
					{
						  Temp=Read_Dgus(((Val[4]<<8)+Val[5]+i));
						  Val[7+2*i]=(u8)(Temp>>8);
						  Val[8+2*i]=(u8)(Temp);
					 }
#if UART2_ENABLE							
	        uart_data_send(Val,2,DATA_UPLOAD_UART2,CRC_CHECK_UART2);
#endif	
#if UART3_ENABLE					 
          uart_data_send(Val,3,DATA_UPLOAD_UART3,CRC_CHECK_UART3);
#endif	
#if UART4_ENABLE					 
					uart_data_send(Val,4,DATA_UPLOAD_UART4,CRC_CHECK_UART4);
#endif	
#if UART5_ENABLE					 
          uart_data_send(Val,5,DATA_UPLOAD_UART5,CRC_CHECK_UART5);	
#endif					
					Write_Dgus(0x0f00,0);Write_Dgus(0x0f01,0);
			}
}



/****************************************************************************/
void  deal_82_cmd(u8 Uart,u8* arr)
{    
	   u8 i=0;
     if(Crc_check_flog==0) //不含CRC检验
     {		 
			      if(arr[4]==0&&arr[5]==6) //更新c代码
						{
							 arr[7]=0xA5;
						}
            Write_Dgusii_Vp_byChar((arr[4]<<8)+arr[5],arr+6,arr[2]-3);												 
            if(Response_flog)   
            {
                 u8 Temp_arr[]={DTHD1,DTHD2,0X03,0X82,0X4F,0X4B}; 	
                 uart_send_str(Uart,Temp_arr,6);					                    	 
            }
      }else //带CRC校验
      {
           u16 Crc=0,Crc_check=0;
           Crc=crc16table((u8*)(&arr[3]),arr[2]-2);
           Crc_check=(u16)(arr[3+arr[2]-1]<<8)+(u16)(arr[3+arr[2]-2]);  
           if(Crc==Crc_check)
           {
                Write_Dgusii_Vp_byChar((arr[4]<<8)+arr[5],arr+6,arr[2]-5);																				
                if(Response_flog)   
                { 
                   u8 Temp_arr[]={DTHD1,DTHD2,0X05,0X82,0X4F,0X4B,0XA5,0XEF}; 	
	                 uart_send_str(Uart,Temp_arr,8);			
                }												 
           }											 
			 }
}
/****************************************************************************/
void  deal_83_cmd(u8 Uart,u8* arr,u8* arr1)
{
	     u8 i=0;
       if(Crc_check_flog==0) //不含CRC检验
		 	{		
              for(i=0;i<7;i++)
									 arr[i]=arr1[i];
						 Read_Dgusii_Vp((arr[4]<<8)+arr[5],(u8*)&arr[7],arr[6]);
						 arr[2]=(2*arr[6])+4;
             uart_send_str(Uart,arr,arr[2]+3);									 
		   }else   //带Crc检验
		  {
						 u16 Crc=0,Crc_check=0;
						 for(i=0;i<9;i++)
									 arr[i]=arr1[i];
						 Crc=crc16table((u8*)(&arr[3]),arr[2]-2);
						 Crc_check=(u16)(arr[3+arr[2]-1]<<8)+(u16)(arr[3+arr[2]-2]);  
					  if(Crc==Crc_check)
						{
								  Read_Dgusii_Vp((arr[4]<<8)+arr[5],(u8*)&arr[7],arr[6]);
								  arr[2]=(2*arr[6])+4+2;
								  Crc=crc16table(arr+3,arr[2]-2);
								  arr[arr[2]+1]=Crc& 0x00FF;
		              arr[arr[2]+2]=Crc>> 8;
	                uart_send_str(Uart,arr,arr[2]+3);	
						}									     
			 }
}
/*************************************************************************************************************
函数名：deal_uart_data(u8* Arr_rece,u16* Data_len,u8 Uart_num,bit Response,bit Crc_ck)
参数：Arr_rece为串口接收数组，Data_len接收数据长度地址，Uart_num串口号，Response是否带应答，Crc_ck是否带校验
作者：cuijia
日期：20220526
*************************************************************************************************************/
void  deal_uart_data(u8* Arr_rece,u8 Uart_num,bit Response,bit Crc_ck)
{
            u16 N=0;	bit Flog=1;

					if((Arr_rece[0]==DTHD1)&&(Arr_rece[1]==DTHD2))   //5A A5 07 82 1000 0001 0002
					{	     if(Arr_rece[3]==0x82)     
						     {	   
									     Response_flog=Response;//应答标记
									     Crc_check_flog=Crc_ck;//Crc标记
                       deal_82_cmd(Uart_num,Arr_rece);//处理82指令

						     }								   // 0	 1	2  3  4	5  6  7	8  9 10
						     else if(Arr_rece[3]==0x83) 		   //5A A5 08 83 1000 02 0001 0002
						     {	   
									    
								   		 u8 Val[Uartx_Frame_Data_Lenth]={0}; 
										   Crc_check_flog=Crc_ck;//Crc标记
                       deal_83_cmd(Uart_num,Val,Arr_rece);//处理83指令
						     }
					}

}
/*****************************************************************************
	函 数 名 : void Uart_Handle_Frame(struct Uartx_Define Uart_number,struct Uartx_Frame_Data Uart_data,void (*pSendFuc)(u8),u16 Uart_buff_lenth)
	功能描述 : 串口数据处理
	输入参数 : Uart_number 串口定义结构体，Uart_data 串口接收数据帧处理结构体 ，void (*pSendFuc)(u8) 串口发送函数，Uart_buff_lenth 串口缓冲区长度
	输出参数 :
	修改历史 :
	1.日  期  : 2022年7月11日
		修  改  : cuijia
 *****************************************************************************/
void Uart_Handle_Frame(P_S_UART Uart_number,P_D_UART Uart_data,u8 Uart_num,u16 Uart_buff_lenth,bit Response,bit Crc_ck) {
	u8  c;
	u16 i=0,Crc=0;

	while( ((*Uart_number).Rx_Write - (*Uart_number).Rx_Read + Uart_buff_lenth) % Uart_buff_lenth){
		EA = 0;
		c = (*Uart_number).Rx_Buffer[(*Uart_number).Rx_Read++];			//读取数据
		(*Uart_number).Rx_Read %= Uart_buff_lenth;
		(*Uart_number).Rx_Busy = 0;
		EA = 1;
		if((*Uart_number).Rx_Flag == Uart_Rev_Pre){						//开始接收帧数据
			(*Uart_data).dataLen = 0;
			(*Uart_data).dataCode = 0x00;
			(*Uart_data).varAddr = 0x0000;
			(*Uart_data).varIndex = 0;
			if(c == DTHD1) 
			{
			    (*Uart_number).Rx_Flag = Uart_Rev_GetFH1;
		    	(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetFH1){		//已检测到帧头1
			if(c == DTHD2){
				(*Uart_number).Rx_Flag = Uart_Rev_GetFH2;
				(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			}else{
				(*Uart_number).Rx_Flag = Uart_Rev_Pre;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetFH2){		//已检测到帧头2
			if(c < 3 || c > Uartx_Frame_Data_Lenth){
				(*Uart_number).Rx_Flag = Uart_Rev_Pre;
			}else{
				(*Uart_data).dataLen = c;
				(*Uart_data).varData[(*Uart_data).varIndex++] = c;
				(*Uart_number).Rx_Flag = Uart_Rev_GetLen;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetLen){		//已检测到长度
			if(c != 0x82 && c != 0x83){
				if(c==0x80||c==0x81){
					(*Uart_data).dataCode = c;
					(*Uart_data).varData[(*Uart_data).varIndex++] = c;
					(*Uart_number).Rx_Flag=Uart_Rev_GetAddr2;
					(*Uart_data).dataLen+=2;
				}else
				  (*Uart_number).Rx_Flag = Uart_Rev_Pre;
			}else{
				(*Uart_data).dataCode = c;
				(*Uart_data).varData[(*Uart_data).varIndex++] = c;
				(*Uart_number).Rx_Flag = Uart_Rev_GetCode;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetCode){	//已检测到操作指令
			(*Uart_data).varAddr = ((u16)c) << 8;
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			(*Uart_number).Rx_Flag = Uart_Rev_GetAddr1;
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetAddr1){	//已检测到变量地址高位
			(*Uart_data).varAddr |= c;
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			(*Uart_number).Rx_Flag = Uart_Rev_GetAddr2;
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetAddr2){	//已检测到变量地址低位
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			if((*Uart_data).varIndex == (*Uart_data).dataLen + 3){
				(*Uart_number).Rx_Flag = Uart_Rev_Done;
				break;
			}
		}
	}
	
	if((*Uart_number).Rx_Flag == Uart_Rev_Done){				//处理一帧数据
		(*Uart_number).Rx_Flag = Uart_Rev_Pre;
		deal_uart_data((*Uart_data).varData,Uart_num,Response,Crc_ck);
	}
}

/************************************************************************************************
函数名：void  Uart_Handle_all() 串口处理函数
参数：
日期：2022—07-11
作者：cuijia
**/
void Uart_Handle_all()
{
    Uart_Handle_Frame(&Uart2,&Uart2_Frame,2,Uart2_Rx_Lenth,RESPONSE_UART2,CRC_CHECK_UART2);
	  Uart_Handle_Frame(&Uart3,&Uart3_Frame,3,Uart3_Rx_Lenth,RESPONSE_UART3,CRC_CHECK_UART3);
    Uart_Handle_Frame(&Uart4,&Uart4_Frame,4,Uart4_Rx_Lenth,RESPONSE_UART4,CRC_CHECK_UART4);
	  Uart_Handle_Frame(&Uart5,&Uart5_Frame,5,Uart5_Rx_Lenth,RESPONSE_UART5,CRC_CHECK_UART5);
}


/*****************************************************************************
	函 数 名 : void Uart2_RX_ISR_PC(void) interrupt 4
	功能描述 : 串口2中断接收函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2021年6月29日
		作  者  : LYN
		修  改  : 创建
 *****************************************************************************/
void Uart2_TX_RX_ISR_PC(void) interrupt 4
{
	u8 res = 0;
	EA = 0;
	if(RI0 == 1){
		RI0 = 0;
			Uart2.Rx_Buffer[Uart2.Rx_Write] = SBUF0;
			Uart2.Rx_Write++;
			Uart2.Rx_Write %= Uart2_Rx_Lenth;

	}else if(TI0 == 1){
		TI0 = 0;
		if(Uart2.Tx_Read != Uart2.Tx_Write){
			SBUF0 = Uart2.Tx_Buffer[Uart2.Tx_Read];
			Uart2.Tx_Read++;
			Uart2.Tx_Read %= Uart2_Tx_Lenth;
		}else{
			Uart2.Tx_Busy = 0;
		}
	}
	EA = 1;
}

/*****************************************************************************
	函 数 名 : void Uart3_RX_ISR_PC(void) interrupt 16
	功能描述 : 串口3中断接收函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2023年12月1日
		作  者  : cuijia
		修  改  : 创建
 *****************************************************************************/
void Uart3_RX_ISR_PC(void)	    interrupt 16   //串口3收发中断
{           
       if(SCON1&0x01)
		   {        
		            
		            SCON1&=0xFE;
//				        SCON1&=0xFE;
		    	Uart3.Rx_Buffer[Uart3.Rx_Write] = SBUF1;
		     	Uart3.Rx_Write++;
		    	Uart3.Rx_Write %= Uart3_Rx_Lenth;   
		   }else if(SCON1&0x02)
		   { 
		            SCON1&=0xFD ; 
				        SCON1&=0xFD ; 
			 		  if(Uart3.Tx_Read != Uart3.Tx_Write){
		    	  SBUF1 = Uart3.Tx_Buffer[Uart3.Tx_Read];
			      Uart3.Tx_Read++;
		     	  Uart3.Tx_Read %= Uart3_Tx_Lenth;
		        }else{
			           Uart3.Tx_Busy = 0;
		        }         
		   }
} 
/*****************************************************************************
	函 数 名 : void Uart4_RX_ISR_PC(void) interrupt 11
	功能描述 : 串口 4中断接收函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : 创建
 *****************************************************************************/
void Uart4_RX_ISR_PC(void) interrupt 11
{
	u8 res = 0;
	EA = 0;
    SCON2R&=0xFE;
			Uart4.Rx_Buffer[Uart4.Rx_Write] = SBUF2_RX;
			Uart4.Rx_Write++;
			Uart4.Rx_Write %= Uart4_Rx_Lenth;
	
	
	EA = 1;
}

/*****************************************************************************
	函 数 名 : void Uart4_TX_ISR_PC(void) interrupt 10
	功能描述 : 串口 4中断发送函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
void Uart4_TX_ISR_PC(void) interrupt 10
{
	u8 res = 0;
	EA = 0;
		SCON2T&=0xFE ; 
		if(Uart4.Tx_Read != Uart4.Tx_Write){
			SBUF2_TX = Uart4.Tx_Buffer[Uart4.Tx_Read];
			Uart4.Tx_Read++;
			Uart4.Tx_Read %= Uart4_Tx_Lenth;
		}else{
			TR4=0;        //485关闭发送
			Uart4.Tx_Busy = 0;
		}
	
	EA = 1;
}

/*****************************************************************************
	函 数 名 : void Uart5_RX_ISR_PC(void) interrupt 13
	功能描述 : 串口 5中断接收函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : 创建
 *****************************************************************************/
void Uart5_RX_ISR_PC(void) interrupt 13
{
	u8 res = 0;
	EA = 0;
    SCON3R&=0xFE;
			Uart5.Rx_Buffer[Uart5.Rx_Write] = SBUF3_RX;
			Uart5.Rx_Write++;
			Uart5.Rx_Write %= Uart5_Rx_Lenth;

	
	EA = 1;
}

/*****************************************************************************
	函 数 名 : void Uart5_TX_ISR_PC(void) interrupt 12
	功能描述 : 串口 5中断发送函数
	输入参数 :	
	输出参数 : 
	修改历史 :
	1.日  期  : 2022年3月9日
		作  者  : LYN
		修  改  : cuijia
 *****************************************************************************/
void Uart5_TX_ISR_PC(void) interrupt 12
{
	u8 res = 0;
	EA = 0;
		SCON3T&=0xFE ; 
		if(Uart5.Tx_Read != Uart5.Tx_Write){
			SBUF3_TX = Uart5.Tx_Buffer[Uart5.Tx_Read];
			Uart5.Tx_Read++;
			Uart5.Tx_Read %= Uart5_Tx_Lenth;
		}else{
			TR5=0;        //485关闭发送
			Uart5.Tx_Busy = 0;
		}
	
	EA = 1;
}