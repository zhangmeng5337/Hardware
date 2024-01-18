#define GLOBALS
#include "global.h"
#include "adc.h"
#include "other.h"
#include "CC1110.h"
#include "spi.h"
#include "flash.h"
#include "infrared.h"
#include "oled.h"
#include "rtc.h"
#include "M25p64.h"
#define Select_OLED       GPIO_ResetBits(GPIOB, GPIO_Pin_2);
/***************************Ԥ����ע��*****************************
�ڱ���ѡ�񴫸�������ʱ���ڱ�����Options For Target��ĺ궨�����޸�
/***************************�������������****************************/
extern uint32_t CV_Value[4];
//***************************�������������**************************/
#ifdef  Liceng
uint8_t Senor_Type=0xE1;
#elif   Maogan
uint8_t Senor_Type=0xE2;
#elif   Maosuo
uint8_t Senor_Type=0xE3;
#elif   Weiyan
uint8_t Senor_Type=0xE4;
#elif   Yali
uint8_t Senor_Type=0xE5;
#endif
extern void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);
/*******************************************************************/
 RTC_SetTypeDef RTC_SetStructure;
RTC_TimeDateTypeDef RTC_TimeDateStructure;
unsigned char txbuffer[16];
unsigned char rxbuffer[16];
u8 *SpiFlashReadBuffer;
extern unsigned char M25PXX_BUFFER[1024];
extern uint32_t WriteAddressPostion;
/**********************���������ݴ�ȡ���**************************/
uint32_t Flash_Read_Buff[4],Flash_Write_Buff[4];
uint32_t ID_Read_Buff[4],ID_Write_Buff[4];
/**************************��ʱ�������****************************/
extern uint8_t Timer;
/**************************�������*************************/
uint8_t  SET_START,curse_ok;
uint8_t curse=0x28,curse_base=0x04,curse_tmp=0x00;
uint32_t k_code=0x00000000;
uint8_t Senor_IDNUM_Temp=0;
SENOR *pObj;
typedef void MENU_CALLBK(SENOR *pObj);
void Infrared_Fun(SENOR *pObj,MENU_CALLBK *_cbm,MENU_CALLBK *_cbo);
extern unsigned char Infrared_Status;
/**************************���ܺ���*************************/
extern WPStatus CC1100_WAKEUP_Flag;
extern int xinxi;
extern WPStatus PD_WAKEUP_Flag;
void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj);
void AD_Smapling_Function(SENOR *pObj);
void PD_WAKEUP_Int(void);
void AD_Display_Function(SENOR *pObj);
void SPI_ConfigurationOLED(void);
SENOR SenMsg;
extern unsigned char fd0[];
extern unsigned char fd1[];
extern unsigned char fd2[];
extern unsigned char BMP[];
char r[5];
char CV1[5];
char CV11[5];
char CV2[5];
char CV22[5];
char CV3[5];
char CV33[5];
uint32_t VVV1;
uint32_t VVV11;
uint32_t VVV2;
uint32_t VVV22;
uint32_t VVV3;
uint32_t VVV33;
void CC1110_Sendd();
void CC1110_Senddata();
void CC1110_Sendnum();
void CC1110_Senddd();
void CC1110_Sendbianhao();
void peizhiwuxian();
extern uint8_t vid1,vid2,vid3,vid4,oldid,newid;
//������ʾת������
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */
/**************************************************************
������void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj)
���ܣ����߷��ͳ�ʼ��Ϣ
**************************************************************/
void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj)
{
    int j;
    memset(pMsg->Invalid,0xFE,5);
    pMsg->Start_Flag=0xAA;
    pMsg->ID=pObj->Senor_ID;
    pMsg->Type=Senor_Type;
    pMsg->Channel=pObj->Senor_Num;
    pMsg->Battery_Power =Battery_Monitor();
    pMsg->End_Flag=0x55;
    FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
    for(j=0; j<4; j++)
    {
    //    pMsg->T_Data_H[j]=(uint8_t)CV_Value[j]>>8;
    //    pMsg->T_Data_L[j]=(uint8_t)CV_Value[j]&0x00000ff;
			pMsg->T_Data_H[j]=0x00;//55
			pMsg->T_Data_L[j]=0x04;//99
    }
}
extern unsigned char rtc_wakeup_flag;
int main(void)
{  
 unsigned char i;
    CC1110Tx_Msg TxMessage;
	  PWR_FastWakeUpCmd(ENABLE);//����Ƭ������������Դ
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//Ƭ�ڵ�Դ�����ʱ��---ʹ�� 
    PWR_UltraLowPowerCmd(ENABLE);//���õ͵�Դʹ��
    CC1100_WAKEUP_Flag=Low;
    PD_WAKEUP_Flag=Low;
	
    NVIC_Config();
	  RCC_DeInit();
	/********************rtc����ʱ������2019��1��12������11ʱ23��4��************************/
	  RTC_SetStructure.am_pm = RTC_H12_AM;
	RTC_SetStructure.year = 0x19;
	RTC_SetStructure.month = 0x01;
	RTC_SetStructure.date = 0x12;
	RTC_SetStructure.date_format = 0;
	RTC_SetStructure.hour = 0x11;
	RTC_SetStructure.minute = 0x23;
	RTC_SetStructure.second = 0x04;
	RTC_SetStructure.wakeup_time = 0x0a; //����ʱ��10s
  RTC_Config(RTC_SetStructure);	
	
	  //EnterLowPower();
    SenMsg.Senor_ID=1;
    SenMsg.Senor_Num=4;
	  ControlGPIO_Initt();
		GPIO_SetBits(GPIOC,GPIO_Pin_8);//����
    GPIO_ResetBits(GPIOC,GPIO_Pin_7);//���� 

	  SPI_Configuration();	
	  Select_OLED
		for(i=1;i<17;i++)
	  txbuffer[i-1]=i;	
	  //M25PXX_Init();	//add by zm
    ID_Write_Buff[0]=0x01;
    ID_Write_Buff[1]=0x00;
    ID_Write_Buff[2]=0x40;
    ID_Write_Buff[3]=0x0B;
    FLASH_Write(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Write_Buff);
		
		Flash_Write_Buff[0]=0x000000AA;
		Flash_Write_Buff[1]=0x0B;
		Flash_Write_Buff[2]=pObj->Senor_Num;
		Flash_Write_Buff[3]=0x00000055;
		FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
//	IWDG_INIT();

		#if DEBUG
			RTC_WakeUpCmd(ENABLE);	//���߻��Ѵ�rtc�ж�,add by zm
		#endif
    while(1)
    {		

    

			 //IWDG_ReloadCounter();//ι��,add by zm
	     
		
#if DEBUG
	  Bsp_Config();
		M25PXX_Erase_Sector(0);	
#else
		RTC_WakeUpCmd(ENABLE);	//���߻��Ѵ�rtc�ж�,add by zm
		EnterLowPower();	
		RTC_WakeUpCmd(DISABLE);	//���߻��ѹر�rtc�ж�	,add by zm
#endif
		Bsp_Config();
		M25PXX_Init();//���߻������³�ʼ��spi flash,add by zm
		//IWDG_INIT();//���߻������³�ʼ�����Ź�,add by zm
		
		if(rtc_wakeup_flag)//rtc���Ѻ�洢���ݵ�spi flash,add by zm
		{ 
			// rtc_wakeup_flag = 0;
			AD_Smapling_Function(&SenMsg);			
			RTC_TimeDateStructure= RTC_GetTimeDate();
			i = 0;
			txbuffer[i++] = RTC_TimeDateStructure.year;
			txbuffer[i++] = RTC_TimeDateStructure.month;
			txbuffer[i++] = RTC_TimeDateStructure.date;
			txbuffer[i++] = RTC_TimeDateStructure.hour;
			txbuffer[i++] = RTC_TimeDateStructure.minute;
			txbuffer[i++] = RTC_TimeDateStructure.second;

			//CV_Value[0]
			txbuffer[i++] = (unsigned char)CV_Value[0]>>24;
			txbuffer[i++] = (unsigned char)CV_Value[0]>>16;
			txbuffer[i++] = (unsigned char)CV_Value[0]>>8;
			txbuffer[i++] = (unsigned char)CV_Value[0];

			//CV_Value[1]			
			txbuffer[i++] = (unsigned char)CV_Value[1]>>24;
			txbuffer[i++] = (unsigned char)CV_Value[1]>>16;
			txbuffer[i++] = (unsigned char)CV_Value[1]>>8;
			txbuffer[i++] = (unsigned char)CV_Value[1];

			//CV_Value[2]			
			txbuffer[i++] = (unsigned char)CV_Value[2]>>24;
			txbuffer[i++] = (unsigned char)CV_Value[2]>>16;
			txbuffer[i++] = (unsigned char)CV_Value[2]>>8;
			txbuffer[i++] = (unsigned char)CV_Value[2];

			//memcpy(&txbuffer[6],CV_Value,3);	//����adc���ݵ�buffer��	

			SPI_FLASH_Write(txbuffer,0,i);//��ַ���Բ�ָ���������е�ַ�Զ�����
			if( Read_AddressWrite()>=0x400000)//flash��������ȡ��Ƭ��������
			{
				M25PXX_Erase_Chip();	
				WriteAddressPostion	= 1;
				//�����ڲ�flashд��������WriteAddressPostion
				/***********cpu flash�洢˵��*********************
				WRITEADDR���洢��ǰspi flashʣ��ռ���׵�ַ��
				ĿǰWRITEADDRΪ0x08080100������Ҫ�޸������е���
				WriteAddressPostion����ǰspi flashʣ��ռ���׵�ַ
        
				*************************************************/
				FLASH_Write((WRITEADDR),(WRITEADDR),&WriteAddressPostion);	
			}
			//��spi flash���ݲ���ʹ�ã�����ʹ����������ָ��Ҫ��ȡ���ݵ��׵�ַ����������
			//��ȡ�����ݻᰴ˳��洢��M25PXX_BUFFER��������
			//WriteAddressPostion	= 18;
			
		#if DEBUG
			WriteAddressPostion	= i;
		#endif			
			SPI_FLASH_READ(M25PXX_BUFFER,0,WriteAddressPostion);//�����ã���spi flash���ݻ�����M25PXX_BUFFER��
			    						
		}
	  
        /******************��ʼ��ȡflash ������ID��̽ͷ����*********************/
        FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
			     
        if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
        {
            SenMsg.Senor_ID=*(Flash_Read_Buff+1);
            SenMsg.Senor_Num=*(Flash_Read_Buff+2);			  
        }			
        /*********������޹����� ���ͺ�����ģ�������������忪ʼ�ɼ��洢�����*******/
        if((PD_WAKEUP_Flag<<1|CC1100_WAKEUP_Flag)==1)
        { 
				  PD_WAKEUP_Int();	
          CC1110_Config((uint32_t)115200);	
					SendMsg_Int(&TxMessage,&SenMsg);
          CC1110_Send_Flag=1;				 
          if(CC1110_Send_Flag!=Low)
          {
					   if(xinxi==1)
							{ 
								//CC1110_Sendd();
								xinxi=0;
							}
						 CC1110_Send_Flag=Low;
          }

            AD_Smapling_Function(&SenMsg);
            CC1100_WAKEUP_Flag=Low;
            PD_WAKEUP_Flag=Low;
						//ControlGPIO_Init();
						GPIO_SetBits(GPIOC,GPIO_Pin_8);
						//delay_ms(200);delay_ms(200);
        }
        else if((PD_WAKEUP_Flag<<1|CC1100_WAKEUP_Flag)==2||rtc_wakeup_flag)
        {
					

					
				  	rtc_wakeup_flag = 0;
           PD_WAKEUP_Int();
           Timer=0;
					 SPI_Configuration();	
	         Select_OLED
            
					 LCD_Init();
					 LCD_CLS();
					 LCD_P16x16Ch(36,0,2,fd0); 
					 FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
					 FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
          
					 AD_Smapling_Function(&SenMsg);
					 SenMsg.Senor_ID=Flash_Read_Buff[1];
					 pObj->Senor_ID=SenMsg.Senor_ID;
					 Senor_IDNUM_Temp=pObj->Senor_ID;
          *itoa(Senor_IDNUM_Temp,r,10);//			
           LCD_P8x16Str(86,0,r);
		       LCD_P16x16Ch(0,3,8,fd1); 

					 VVV1=CV_Value[0]/10;
					 VVV11=CV_Value[0]%10;
					 *itoa(VVV1,CV1,10);	
				   if(VVV1>9)
					 {
					   	LCD_P8x16Str(18,6,CV1);			
					    LCD_P6x8Str(34,7,".");
					    *itoa(VVV11,CV11,10);	
					    LCD_P8x16Str(39,6,CV11);
					 }
				   else
					 {
					    LCD_P8x16Str(21,6,CV1);
					    LCD_P6x8Str(29,7,".");
					    *itoa(VVV11,CV11,10);	
					    LCD_P8x16Str(34,6,CV11);
				   }
        	
	         VVV2=CV_Value[1]/10;
					 VVV22=CV_Value[1]%10;					
           *itoa(VVV2,CV2,10);//	
     			 if(VVV2>9)
					 {		
              LCD_P8x16Str(81,6,CV2);
					    LCD_P6x8Str(97,7,".");
					    *itoa(VVV22,CV22,10);	
					    LCD_P8x16Str(102,6,CV22);
					 }
					else
					 {
						  LCD_P8x16Str(84,6,CV2);
					    LCD_P6x8Str(92,7,".");
					    *itoa(VVV22,CV22,10);	
					    LCD_P8x16Str(97,6,CV22);
					 }
					
//           VVV3=CV_Value[2]/10;
//					 VVV33=CV_Value[2]%10;						
//           *itoa(VVV3,CV3,10);//	
//    			 if(VVV3>9)
//						{						
//              LCD_P8x16Str(99,6,CV3);	
//					    LCD_P6x8Str(115,7,".");
//					    *itoa(VVV33,CV33,10);	
//					    LCD_P8x16Str(120,6,CV33);
//						}
//					else
//					  {
//						  LCD_P8x16Str(102,6,CV3);	
//					    LCD_P6x8Str(110,7,".");
//					    *itoa(VVV33,CV33,10);	
//					    LCD_P8x16Str(115,6,CV33);
//						}
								
			    delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
			    delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);


          //AD_Display_Function(&SenMsg); 
					PD_WAKEUP_Flag=Low;
          CC1100_WAKEUP_Flag=Low;
					LCD_CLS();
					Power_Control(Power_OFF);        //�رմ�������Դ
           //LED_Display_Clear();             //����
          LED_Control(Power_OFF);          //�ر�����ܵ�Դ
					
        }		
    }
}

/**************************************************************
������void AD_Smapling_Function(CV_VALUE(*fun)(uint16_t, uint8_t),SENSOR *pObj)
���ܣ������߻���ʱAD�������
**************************************************************/
void AD_Smapling_Function(SENOR *pObj)
{
    uint8_t i;
    Power_Control(Power_ON);   //���������������Դ
    for(i=0; i<4; i++)
    {
        delay_us(400);
        Get_ADC(i);
    }
    //Power_Control(Power_OFF);  //�رմ�������Դ
    FLASH_Write(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
}
/**************************************************************
������void PD_WAKEUP_Int(void)
���ܣ������ջ���ʱ��Ҫ��Ӳ��
**************************************************************/
void PD_WAKEUP_Int(void)
{
    Timer_Config(1599);          //��ʱ1��
    Led_Control_GPIO();
    LED_Control(Power_ON);       //����ܹ���
    //SPI_Configuration();         //SPI����
    Power_Control(Power_ON);     //̽ͷ��Դ����
    Infrared_Config(2000,1599);  //���⿪��
    NVIC_Config();
}
/**************************************************************
������static void _cboption(SENOR *pObj)
���ܣ����⹦��
**************************************************************/
static void _cbmenu(SENOR *pObj)
{
    switch(k_code)
    {
    case IR_Up:					
	
    case IR_Down:
        curse_tmp=curse;
        curse=curse_base;
        curse_base=curse_tmp;
        k_code=0;
        break;
    case IR_OK:
        curse_ok=0xff;
        k_code=0;
        break;
    case IR_Cancel:
        curse_ok=0;
        SET_START=0;
        k_code=0;
    default:
        break;
    }
    if(curse==0x28)
    {
        Led_Display_Set(0,pObj->Senor_ID);
        Senor_IDNUM_Temp=pObj->Senor_ID;          //��ʼ��Ϣ
    }
    else if(curse==0x04)
    {
        Led_Display_Set(1,pObj->Senor_Num);
        Senor_IDNUM_Temp=pObj->Senor_Num;
    }
}
/**************************************************************
������static void _cboption(SENOR *pObj)
���ܣ����⹦��
**************************************************************/
static void _cboption(SENOR *pObj)
{
    switch(k_code)
    {
    case IR_Up:
        if(Senor_IDNUM_Temp<1)Senor_IDNUM_Temp=curse;
        Senor_IDNUM_Temp--;
        k_code=0;
        break;
    case IR_Down:
        Senor_IDNUM_Temp++;
        if(Senor_IDNUM_Temp>curse)Senor_IDNUM_Temp=1;
        k_code=0;
        break;
    case IR_OK:
        if(curse==0x28)pObj->Senor_ID=Senor_IDNUM_Temp;
        else if(curse==0x04)pObj->Senor_Num=Senor_IDNUM_Temp;
        Flash_Write_Buff[0]=0x000000AA;
        Flash_Write_Buff[1]=pObj->Senor_ID;
        Flash_Write_Buff[2]=pObj->Senor_Num;
        Flash_Write_Buff[3]=0x00000055;
        FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
        curse_ok=0x00;
        k_code=0;
        break;
    case IR_Cancel:
        curse_ok=0x00;
        k_code=0;
    default:
        break;
    }
    Led_Display_Set(2,Senor_IDNUM_Temp);
}
/**************************************************************
������void Infrared_Fun(SENOR *pObj)
���ܣ����⹦��
**************************************************************/
void Infrared_Fun(SENOR *pObj,MENU_CALLBK *_cbm,MENU_CALLBK *_cbo)
{
    SET_START=1,curse_ok=0;
    while(SET_START==1)
    {
        k_code=IR_KEY_Scan();
        if(curse_ok==0x00)
        {
            _cbm(pObj);
        }
        else if(curse_ok==0xff)
        {
            _cbo(pObj);
					pObj->Senor_ID--;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
        }
				pObj->Senor_ID+2;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
    }
}
/**************************************************************
������void AD_Display_Function(CV_VALUE(*fun)(uint16_t, uint8_t),SENOR *pObj)
���ܣ������ջ���ʱAD�������
**************************************************************/
void AD_Display_Function(SENOR *pObj)
{
     uint32_t KEY_CODE=0;
    Timer=0;
    do {
        if(Timer>=(pObj->Senor_Num))Timer=0;
        //Led_Display_Data(Timer,CV_Value[Timer]);  //��ʾ����

        KEY_CODE=IR_KEY_Scan();

        if(KEY_CODE==IR_Fun1)        //����к����ź�
        {						
					LCD_CLS();					          
			    LCD_P16x16Ch(15,2,6,fd2); 	
					Senor_IDNUM_Temp=pObj->Senor_ID;
          *itoa(Senor_IDNUM_Temp,r,10);//		
          LCD_P8x16Str(56,4,r);		
 				}
          KEY_CODE=0;
          //Infrared_Fun(pObj,&_cbmenu,&_cboption);
					       
					KEY_CODE=IR_KEY_Scan();

        if(KEY_CODE==IR_Up)  
				{
				 pObj->Senor_ID++;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
				}
        else if(KEY_CODE==IR_Down)  
				{
				 pObj->Senor_ID--;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
				}
				else if(KEY_CODE==IR_OK)  
				{
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
         LCD_CLS();			
         LCD_P8x16Str(56,4,r);	
				 LCD_P8x16Str(56,6,"YES"); 
					
				CC1110_Config((uint32_t)9600);		
				ControlGPIO_Initt();	
					//LCD_P8x16Str(6,0,"Y"); 
			  GPIO_SetBits(GPIOC,GPIO_Pin_7);//����
					//LCD_P8x16Str(26,0,"E"); 
		    GPIO_SetBits(GPIOC,GPIO_Pin_8);//����  ��������ģʽ
					//LCD_P8x16Str(46,0,"S"); 
				CC1110_Senddd();delay_ms(200);
					//LCD_P8x16Str(86,0,"P"); 
					
				GPIO_SetBits(GPIOC,GPIO_Pin_7);//����
//					delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
//					GPIO_SetBits(GPIOC,GPIO_Pin_7);//����
//					GPIO_SetBits(GPIOC,GPIO_Pin_8);//����  ��������ģʽ
//					CC1110_Sendddd();
//					GPIO_SetBits(GPIOC,GPIO_Pin_7);//����
	      Flash_Write_Buff[0]=0x000000AA;
        Flash_Write_Buff[1]=pObj->Senor_ID;
        Flash_Write_Buff[2]=pObj->Senor_Num;
        Flash_Write_Buff[3]=0x00000055;
        FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
				}

        
    } while(Timer<2); //�ȴ����ս���
    TIM2->DIER &= (uint16_t)~TIM_IT_Update;
    TIM4->DIER &= (uint16_t)~TIM_IT_CC1|TIM_IT_Update;
    //Power_Control(Power_OFF);        //�رմ�������Դ
    //LED_Display_Clear();             //����
    //LED_Control(Power_OFF);          //�ر�����ܵ�Դ
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif

////////////SPI2�ӿ�����/////////////////////////////////////////////////////
void SPI_ConfigurationOLED(void)
{
	 
	
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);

	  /* SPI2���Ŷ���  PB13--SPI2_SCK  PB15--SPI2_MOSI */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* PB12--OLED RST  ���Ŷ��� */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12  ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
      /* PC6--OLED C/D ���Ŷ��� */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* ����SPI2 OLEDƬѡ  PB2 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			  					 
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;		   //���ģʽ����ٶ�50MHz
   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //ͨ���������ģʽ
	 		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* SPI2 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;   //SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI2, &SPI_InitStructure);
	  /* Enable SPI2 */
	  SPI_Cmd(SPI2, ENABLE);
}
//extern uint32_t CV_Value[4];

int k=0x01;
void CC1110_Sendd(void)
{
    int i,h,m,p,q,v,f,g,l;
	  k++;
      
//			f = 0xff;
//			USART3->DR = (f & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//	
//			g = 0xfe;
//			USART3->DR = (g & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־

//			CV_Value[3]=SenMsg.Senor_ID;
//			q = CV_Value[3];
//			USART3->DR = (q & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
		
//////						 SPI_Configuration();	
//////	         Select_OLED
//////            
//////					 LCD_Init();
//////					 LCD_CLS();
//////					 LCD_P16x16Ch(36,0,2,fd1); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
	
	    FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
			h = *ID_Read_Buff;
			USART3->DR = (h & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־		

			USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
			
			m = *(ID_Read_Buff+2);
			USART3->DR = (m & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
	
			USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			l = SenMsg.Senor_ID;
			USART3->DR = (l & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			USART3->DR = (0xf1 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf2 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf3 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf4 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf5 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf6 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf7 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf8 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xf9 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0xfe & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			USART3->DR = (0xe7 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
}
//int k=0x01;
void CC1110_Senddata(void)
{
      int i,h,m,p,q,v,f,g,l;
	    k++;

//////					 SPI_Configuration();	
//////	         Select_OLED
//////            
//////					 LCD_Init();
//////					 LCD_CLS();
//////					 LCD_P16x16Ch(36,0,2,fd1); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
					 FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
					 FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
          
					 AD_Smapling_Function(&SenMsg);

      FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
			USART3->DR = (*ID_Read_Buff & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־		

			USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
			
			USART3->DR = (*(ID_Read_Buff+2) & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
	
			USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			h = CV_Value[0]>>8;
			USART3->DR = (h & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			

			USART3->DR = (CV_Value[0] & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
			
			m = CV_Value[1]>>8;
			USART3->DR = (m & (uint16_t)0x01FF);		
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			

			USART3->DR = (CV_Value[1] & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			v = 0x00>>8;
			USART3->DR = (v & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			

			USART3->DR = (0x00 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			USART3->DR = (0x00 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			USART3->DR = (0x00 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			CV_Value[4]=Battery_Monitor();
			p = CV_Value[4];
			USART3->DR = (p & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			
			USART3->DR = (0xf0 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־

			USART3->DR = (0xE7 & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
}

//����������Ϣ
void CC1110_Senddd(void)
{
    int co,idh,idl,pz,xd,fc;
	    
			co = 0xC0;
			USART3->DR = (co & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
	
			idh = 0x00;
			USART3->DR = (idh & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
	
			idl = SenMsg.Senor_ID;
			USART3->DR = (idl & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
	      
			pz = 0x18;
			USART3->DR = (pz & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
	
			xd = SenMsg.Senor_ID;
			USART3->DR = (xd & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
	
			fc = 0xFC;
			USART3->DR = (fc & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־		

}
void CC1110_Sendbianhao(void)
{

			Flash_Write_Buff[0]=0x000000AA;
			Flash_Write_Buff[1]=newid;
			Flash_Write_Buff[2]=0x11;
			Flash_Write_Buff[3]=0x00000055;
			FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
	
			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);	
			if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
			{
					SenMsg.Senor_ID=*(Flash_Read_Buff+1);				
			}
	
			USART3->DR = (0x1D & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
			
			USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
			

}

//void CC1110_Sendnum(void)
//{
//      int i,h,m,p,q,v,f,g,l;
//	    k++;
//	
//////      SPI_Configuration();	
//////		 Select_OLED
//////			
//////		 LCD_Init();
//////		 LCD_CLS();
//////		 LCD_P16x16Ch(36,0,2,fd2); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);

////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
//	    Flash_Write_Buff[0]=0x000000AA;
//			Flash_Write_Buff[1]=newid;
//			Flash_Write_Buff[2]=0x11;
//			Flash_Write_Buff[3]=0x00000055;
//			FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
//	
//			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);	
//			if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
//			{
//					SenMsg.Senor_ID=*(Flash_Read_Buff+1);				
//			}
//			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
//			FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
//          
//			//AD_Smapling_Function(&SenMsg);

//      FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
//			USART3->DR = (*ID_Read_Buff & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־		

//			USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
//			
//			USART3->DR = (*(ID_Read_Buff+2) & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־	
//	
//			USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			
//			USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			
//			USART3->DR = (0xD1 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD2 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD3 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD4 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD5 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD6 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD7 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD8 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			USART3->DR = (0xD9 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//			
//			
//			USART3->DR = (0xf9 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־

//			USART3->DR = (0xE7 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //������ɱ�־
//}
