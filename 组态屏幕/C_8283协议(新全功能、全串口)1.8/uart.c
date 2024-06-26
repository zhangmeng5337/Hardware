/******************************************************************************

         ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾

 ******************************************************************************
	�� �� ��  : uart.c
	�� �� ��  : V2.0
	��  ��  	: LYN
	��������  : 2021��6��29��
	��������  : ���ں���
	�޸���ʷ  :
	1.��  ��  : 
		��  ��  : 
		�޸�����  : 
******************************************************************************/

#include "Uart.h"

/********************************/

//u8 Uart2_Rx_Buffer[Uart2_Rx_Lenth];	//���ջ���
//u8 Uart2_Tx_Buffer[Uart2_Tx_Lenth];	//���ͻ���
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
bit                 Response_flog=0;  //Ӧ����
bit                 Auto_data_upload=0;//�����Զ��ϴ���
bit                 Crc_check_flog=0;//CrcУ����
/*****************************************************************************
	�� �� �� : void Uart2_Init(u32 baud_rate)
	�������� : ����2��ʼ��
	������� :	baud_rate ������
	������� : 
	�޸���ʷ :
	1.��  ��  : 2021��9��30��
		��  ��  : LYN
		��  ��  : cuijia
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
	MUX_SEL |= 0x40;	//����uart2
	P0MDOUT &= 0xCF;	//P0 0001 0000
	P0MDOUT |= 0x10;
	ADCON = 0x80;			//UART2 8N1 115200
	SCON0 = 0x50;
	SREL0H = (u8)(i>>8);		//������ = FCLK/64*(1024-SREL)
	SREL0L = (u8)i;    // S_2/115200=224=0X03E4 1024-206438400/(64*115200)=0X03E4  
	ES0 = 1;
	EA=1;	
}
#endif
/*****************************************************************************
	�� �� �� : void Uart3_Init(u32 baud_rate)
	�������� : ����2��ʼ��
	������� :	baud_rate ������
	������� : 
	�޸���ʷ :
	1.��  ��  : 2023��12��1��
		��  ��  : LYN
		��  ��  : cuijia
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
	    SREL1H = (u8)(i>>8);		//������ = FCLK/64*(1024-SREL)
	    SREL1L = (u8)i;    // S_2/115200=224=0X03C8 1024-206438400/(32*115200)=0X03C8
			MUX_SEL|=0x20;    //UART3����
			P0MDOUT|=0x40;	  //P0.6 TXD ����				 
			SCON1 = 0x90;     //����ʹ�ܺ�ģʽ����		 
			IEN2 |= 0x01;	  //�ж�ʹ��		
     	EA  = 1;	
}
#endif
/*****************************************************************************
	�� �� �� : void Uart4_Init(u32 baud_rate)
	�������� : ����4��ʼ��
	������� :	baud_rate ������
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : cuijia
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
	SCON2T= 0x80	;//����ʹ�ܺ�ģʽ����
	SCON2R= 0x80;//����ʹ�ܺ�ģʽ���� 
	ES2R = 1;//�жϽ���ʹ��
  ES2T = 1;//�жϷ���ʹ��
	BODE2_DIV_H = (u8)(i>>8);		//������ = CPU ��Ƶ/(8*������) 
	BODE2_DIV_L = (u8)i;    // 
		    P0MDOUT|=(1<<0); //p0^0 ǿ��
			TR4=0;
   EA=1;			
}
#endif
/*****************************************************************************
	�� �� �� : void Uart5_Init(u32 baud_rate)
	�������� : ����5��ʼ��
	������� :	baud_rate ������
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : cuijia
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
	SCON3T= 0x80	;//����ʹ�ܺ�ģʽ����
	SCON3R= 0x80;//����ʹ�ܺ�ģʽ���� 
	ES3R = 1;//�жϽ���ʹ��
  ES3T = 1;//�жϷ���ʹ��
	BODE3_DIV_H = (u8)(i>>8);		//������ = CPU ��Ƶ/(8*������) 
	BODE3_DIV_L = (u8)i;    // 
		    P0MDOUT|=(1<<1); //p0^1 ǿ��
			TR5=0;
  EA=1;	
}
#endif
/******************************************************************************
���ڳ�ʼ��
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
��������void  uart_send_byte(u8 Uart_number,u8 Dat)
������Uart_number=���ں�=2��3��4��5��DatΪ��Ҫ���͵�byte
���ߣ�cuijia
���ڣ�20220526
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
��������void  uart_send_str(u8 U_number,u8* Str,u8 Len)
������U_number=���ں�=2��3��4��5��Str��Ҫ���͵��ַ��������ͳ���Len
���ߣ�cuijia
���ڣ�20230331
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
	�� �� �� : void Uart2_Tx_write2buff(u8 dat)
	�������� : װ�ش���2���ͻ���
	������� :	dat:Ԥ���͵�����
	������� :
	�޸���ʷ :
	1.��  ��  : 2020��6��29��
		��  ��  : LYN
		�޸�����  : ����
 *****************************************************************************/
void Uart2_Tx_write2buff(u8 dat){
	Uart2.Tx_Buffer[Uart2.Tx_Write] = dat;					//װ���ͻ���

	if(Uart2.Tx_Read > Uart2.Tx_Write){							//�������дָ���ͷ����ָ����ǰ
		while((Uart2.Tx_Write) + 1 == Uart2.Tx_Read);	//�������������ȴ���ָ��ǰ��
	}

	++Uart2.Tx_Write;													//дָ��ǰ��
	if(Uart2.Tx_Write >= Uart2_Tx_Lenth){			//дָ�뼴����ͷ
		while(Uart2.Tx_Read == 0);							//����ָ����ͷ���ȴ���ָ��ǰ��
		Uart2.Tx_Write = 0;											//дָ���ͷ
	}

	if(Uart2.Tx_Busy == 0){				//����
		Uart2.Tx_Busy = 1;					//��־æ
		TI0 = 1;										//���������ж�
	}
}
/*****************************************************************************
	�� �� �� : void Uart3_Tx_write3buff(u8 dat)
	�������� : װ�ش���3���ͻ���
	������� :	dat:Ԥ���͵�����
	������� :
	�޸���ʷ :
	1.��  ��  : 2023��12��1��
		��  ��  : LYN
		�޸�����  : ����
 *****************************************************************************/
void Uart3_Tx_write3buff(u8 dat){
	Uart3.Tx_Buffer[Uart3.Tx_Write] = dat;					//װ���ͻ���

	if(Uart3.Tx_Read > Uart3.Tx_Write){							//�������дָ���ͷ����ָ����ǰ
		while((Uart3.Tx_Write) + 1 == Uart3.Tx_Read);	//�������������ȴ���ָ��ǰ��
	}

	++Uart3.Tx_Write;													//дָ��ǰ��
	if(Uart3.Tx_Write >= Uart3_Tx_Lenth){			//дָ�뼴����ͷ
		while(Uart3.Tx_Read == 0);							//����ָ����ͷ���ȴ���ָ��ǰ��
		Uart3.Tx_Write = 0;											//дָ���ͷ
	}

	if(Uart3.Tx_Busy == 0){				//����
		Uart3.Tx_Busy = 1;					//��־æ
		SCON1|=0x02 ;										//���������ж�
	}
}
/*****************************************************************************
	�� �� �� : void Uart4_Tx_write4buff(u8 dat)
	�������� : װ�ش���4���ͻ���
	������� :	dat:Ԥ���͵�����
	������� :
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		�޸�����  : ����
 *****************************************************************************/
void Uart4_Tx_write4buff(u8 dat){
	Uart4.Tx_Buffer[Uart4.Tx_Write] = dat;					//װ���ͻ���

	if(Uart4.Tx_Read > Uart4.Tx_Write){							//�������дָ���ͷ����ָ����ǰ
		while((Uart4.Tx_Write) + 1 == Uart4.Tx_Read);	//�������������ȴ���ָ��ǰ��
	}

	++Uart4.Tx_Write;													//дָ��ǰ��
	if(Uart4.Tx_Write >= Uart4_Tx_Lenth){			//дָ�뼴����ͷ
		while(Uart4.Tx_Read == 0);							//����ָ����ͷ���ȴ���ָ��ǰ��
		Uart4.Tx_Write = 0;											//дָ���ͷ
	}
    
	if(Uart4.Tx_Busy == 0){				//����
		Uart4.Tx_Busy = 1;					//��־æ
		TR4=1;          //   485��������
		SCON2T|=0x1 ; 										//���������ж�
	}
}
/*****************************************************************************
	�� �� �� : void Uart5_Tx_write5buff(u8 dat)
	�������� : װ�ش���4���ͻ���
	������� :	dat:Ԥ���͵�����
	������� :
	�޸���ʷ :
	1.��  ��  : 2023��11��30��
		��  ��  : LYN
		�޸�����  : ����
 *****************************************************************************/
void Uart5_Tx_write5buff(u8 dat){
	Uart5.Tx_Buffer[Uart5.Tx_Write] = dat;					//װ���ͻ���

	if(Uart5.Tx_Read > Uart5.Tx_Write){							//�������дָ���ͷ����ָ����ǰ
		while((Uart5.Tx_Write) + 1 == Uart5.Tx_Read);	//�������������ȴ���ָ��ǰ��
	}

	++Uart5.Tx_Write;													//дָ��ǰ��
	if(Uart5.Tx_Write >= Uart5_Tx_Lenth){			//дָ�뼴����ͷ
		while(Uart5.Tx_Read == 0);							//����ָ����ͷ���ȴ���ָ��ǰ��
		Uart5.Tx_Write = 0;											//дָ���ͷ
	}
    
	if(Uart5.Tx_Busy == 0){				//����
		Uart5.Tx_Busy = 1;					//��־æ
		TR5=1;          //   485��������
		SCON3T|=0x1 ; 										//���������ж�
	}
}
/****************************
�����Զ��ϴ�
************************************************/
void uart_data_send(u8* Arr,u8 Uartnum,bit Outo_send,bit Crc_ck )
{
	        u8   i=0; 
	        u16  V1=Read_Dgus(0x0f01);	        
          Auto_data_upload=Outo_send;
					if(Auto_data_upload) 
					{
						 Crc_check_flog=Crc_ck;						 
						 if(Crc_check_flog) //��CrcУ��
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
	�� �� �� : Read_0xF00()
	�������� : ����2�����Զ��ϴ�
	������� :
	������� :
	�޸���ʷ :
	1.��  ��  : 2021��10��13��
		��  ��  : 
		��  ��  : cuijia
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
     if(Crc_check_flog==0) //����CRC����
     {		 
			      if(arr[4]==0&&arr[5]==6) //����c����
						{
							 arr[7]=0xA5;
						}
            Write_Dgusii_Vp_byChar((arr[4]<<8)+arr[5],arr+6,arr[2]-3);												 
            if(Response_flog)   
            {
                 u8 Temp_arr[]={DTHD1,DTHD2,0X03,0X82,0X4F,0X4B}; 	
                 uart_send_str(Uart,Temp_arr,6);					                    	 
            }
      }else //��CRCУ��
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
       if(Crc_check_flog==0) //����CRC����
		 	{		
              for(i=0;i<7;i++)
									 arr[i]=arr1[i];
						 Read_Dgusii_Vp((arr[4]<<8)+arr[5],(u8*)&arr[7],arr[6]);
						 arr[2]=(2*arr[6])+4;
             uart_send_str(Uart,arr,arr[2]+3);									 
		   }else   //��Crc����
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
��������deal_uart_data(u8* Arr_rece,u16* Data_len,u8 Uart_num,bit Response,bit Crc_ck)
������Arr_receΪ���ڽ������飬Data_len�������ݳ��ȵ�ַ��Uart_num���ںţ�Response�Ƿ��Ӧ��Crc_ck�Ƿ��У��
���ߣ�cuijia
���ڣ�20220526
*************************************************************************************************************/
void  deal_uart_data(u8* Arr_rece,u8 Uart_num,bit Response,bit Crc_ck)
{
            u16 N=0;	bit Flog=1;

					if((Arr_rece[0]==DTHD1)&&(Arr_rece[1]==DTHD2))   //5A A5 07 82 1000 0001 0002
					{	     if(Arr_rece[3]==0x82)     
						     {	   
									     Response_flog=Response;//Ӧ����
									     Crc_check_flog=Crc_ck;//Crc���
                       deal_82_cmd(Uart_num,Arr_rece);//����82ָ��

						     }								   // 0	 1	2  3  4	5  6  7	8  9 10
						     else if(Arr_rece[3]==0x83) 		   //5A A5 08 83 1000 02 0001 0002
						     {	   
									    
								   		 u8 Val[Uartx_Frame_Data_Lenth]={0}; 
										   Crc_check_flog=Crc_ck;//Crc���
                       deal_83_cmd(Uart_num,Val,Arr_rece);//����83ָ��
						     }
					}

}
/*****************************************************************************
	�� �� �� : void Uart_Handle_Frame(struct Uartx_Define Uart_number,struct Uartx_Frame_Data Uart_data,void (*pSendFuc)(u8),u16 Uart_buff_lenth)
	�������� : �������ݴ���
	������� : Uart_number ���ڶ���ṹ�壬Uart_data ���ڽ�������֡����ṹ�� ��void (*pSendFuc)(u8) ���ڷ��ͺ�����Uart_buff_lenth ���ڻ���������
	������� :
	�޸���ʷ :
	1.��  ��  : 2022��7��11��
		��  ��  : cuijia
 *****************************************************************************/
void Uart_Handle_Frame(P_S_UART Uart_number,P_D_UART Uart_data,u8 Uart_num,u16 Uart_buff_lenth,bit Response,bit Crc_ck) {
	u8  c;
	u16 i=0,Crc=0;

	while( ((*Uart_number).Rx_Write - (*Uart_number).Rx_Read + Uart_buff_lenth) % Uart_buff_lenth){
		EA = 0;
		c = (*Uart_number).Rx_Buffer[(*Uart_number).Rx_Read++];			//��ȡ����
		(*Uart_number).Rx_Read %= Uart_buff_lenth;
		(*Uart_number).Rx_Busy = 0;
		EA = 1;
		if((*Uart_number).Rx_Flag == Uart_Rev_Pre){						//��ʼ����֡����
			(*Uart_data).dataLen = 0;
			(*Uart_data).dataCode = 0x00;
			(*Uart_data).varAddr = 0x0000;
			(*Uart_data).varIndex = 0;
			if(c == DTHD1) 
			{
			    (*Uart_number).Rx_Flag = Uart_Rev_GetFH1;
		    	(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetFH1){		//�Ѽ�⵽֡ͷ1
			if(c == DTHD2){
				(*Uart_number).Rx_Flag = Uart_Rev_GetFH2;
				(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			}else{
				(*Uart_number).Rx_Flag = Uart_Rev_Pre;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetFH2){		//�Ѽ�⵽֡ͷ2
			if(c < 3 || c > Uartx_Frame_Data_Lenth){
				(*Uart_number).Rx_Flag = Uart_Rev_Pre;
			}else{
				(*Uart_data).dataLen = c;
				(*Uart_data).varData[(*Uart_data).varIndex++] = c;
				(*Uart_number).Rx_Flag = Uart_Rev_GetLen;
			}
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetLen){		//�Ѽ�⵽����
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
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetCode){	//�Ѽ�⵽����ָ��
			(*Uart_data).varAddr = ((u16)c) << 8;
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			(*Uart_number).Rx_Flag = Uart_Rev_GetAddr1;
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetAddr1){	//�Ѽ�⵽������ַ��λ
			(*Uart_data).varAddr |= c;
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			(*Uart_number).Rx_Flag = Uart_Rev_GetAddr2;
		}else if((*Uart_number).Rx_Flag == Uart_Rev_GetAddr2){	//�Ѽ�⵽������ַ��λ
			(*Uart_data).varData[(*Uart_data).varIndex++] = c;
			if((*Uart_data).varIndex == (*Uart_data).dataLen + 3){
				(*Uart_number).Rx_Flag = Uart_Rev_Done;
				break;
			}
		}
	}
	
	if((*Uart_number).Rx_Flag == Uart_Rev_Done){				//����һ֡����
		(*Uart_number).Rx_Flag = Uart_Rev_Pre;
		deal_uart_data((*Uart_data).varData,Uart_num,Response,Crc_ck);
	}
}

/************************************************************************************************
��������void  Uart_Handle_all() ���ڴ�����
������
���ڣ�2022��07-11
���ߣ�cuijia
**/
void Uart_Handle_all()
{
    Uart_Handle_Frame(&Uart2,&Uart2_Frame,2,Uart2_Rx_Lenth,RESPONSE_UART2,CRC_CHECK_UART2);
	  Uart_Handle_Frame(&Uart3,&Uart3_Frame,3,Uart3_Rx_Lenth,RESPONSE_UART3,CRC_CHECK_UART3);
    Uart_Handle_Frame(&Uart4,&Uart4_Frame,4,Uart4_Rx_Lenth,RESPONSE_UART4,CRC_CHECK_UART4);
	  Uart_Handle_Frame(&Uart5,&Uart5_Frame,5,Uart5_Rx_Lenth,RESPONSE_UART5,CRC_CHECK_UART5);
}


/*****************************************************************************
	�� �� �� : void Uart2_RX_ISR_PC(void) interrupt 4
	�������� : ����2�жϽ��պ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2021��6��29��
		��  ��  : LYN
		��  ��  : ����
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
	�� �� �� : void Uart3_RX_ISR_PC(void) interrupt 16
	�������� : ����3�жϽ��պ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2023��12��1��
		��  ��  : cuijia
		��  ��  : ����
 *****************************************************************************/
void Uart3_RX_ISR_PC(void)	    interrupt 16   //����3�շ��ж�
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
	�� �� �� : void Uart4_RX_ISR_PC(void) interrupt 11
	�������� : ���� 4�жϽ��պ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : ����
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
	�� �� �� : void Uart4_TX_ISR_PC(void) interrupt 10
	�������� : ���� 4�жϷ��ͺ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : cuijia
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
			TR4=0;        //485�رշ���
			Uart4.Tx_Busy = 0;
		}
	
	EA = 1;
}

/*****************************************************************************
	�� �� �� : void Uart5_RX_ISR_PC(void) interrupt 13
	�������� : ���� 5�жϽ��պ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : ����
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
	�� �� �� : void Uart5_TX_ISR_PC(void) interrupt 12
	�������� : ���� 5�жϷ��ͺ���
	������� :	
	������� : 
	�޸���ʷ :
	1.��  ��  : 2022��3��9��
		��  ��  : LYN
		��  ��  : cuijia
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
			TR5=0;        //485�رշ���
			Uart5.Tx_Busy = 0;
		}
	
	EA = 1;
}