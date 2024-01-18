#include "modbus_RTU.h"
#include "led.h"
#include "lcd.h"
#include "stm32f10x_tim.h"


///////////////////////////////////////////////////////////
unsigned char RS485_TX_EN;
/*u32 RS485_Baudrate=9600;//ͨѶ������
u8 RS485_Parity=0;//0��У�飻1��У�飻2żУ��
u8 RS485_Addr=1;//�ӻ���ַ
u16 RS485_Frame_Distance=4;//����֡��С�����ms),������ʱ������Ϊ����һ֡

u8 RS485_RX_BUFF[2048];//���ջ�����2048�ֽ�
u16 RS485_RX_CNT=0;//���ռ�����
u8 RS485_FrameFlag=0;//֡�������
u8 RS485_TX_BUFF[2048];//���ͻ�����
u16 RS485_TX_CNT=0;//���ͼ�����*/
modbus_stru modbusUsr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus�Ĵ����͵�Ƭ���Ĵ�����ӳ���ϵ
vu32 *Modbus_InputIO[100];//���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
vu32 *Modbus_OutputIO[100];//����������Ĵ���ָ��(����ʹ�õ���λ������)
u16 *Modbus_HoldReg[1000];//���ּĴ���ָ��
u32 testData1=1201,testData2=1002,testData3=2303,testData4=8204;
void Modbus_RegMap(void)
{


         //���뿪�����Ĵ���ָ��ָ��
        Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)��ȡPE4�ĵ�ַ��(vu32*)&PEin(4)��PE4��ַǿ��ת��Ϊuw32���͵ĵ�ַ��Modbus_InputIO[0]=(vu32*)&PEin(4); ��ת���õĵ�ַ�͸���ַָ��Modbus_InputIO[0]��
        Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*Modbus_InputIO[0] ȡ����ַ�е����ݡ�
        Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
        Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3

        //����������Ĵ���ָ��ָ��
        Modbus_OutputIO[0]=(vu32*)&PBout(5);//LED0
        Modbus_OutputIO[1]=(vu32*)&PEout(5);//LED1

        //���ּĴ���ָ��ָ��
        Modbus_HoldReg[0]=(u16*)&testData1;//��������1
        Modbus_HoldReg[1]=(u16*)&testData2;//((u16*)&testData1)+1;//��������1
        Modbus_HoldReg[2]=(u16*)&testData3;//(u16*)&testData2;//��������2
        Modbus_HoldReg[3]=(u16*)&testData4;//((u16*)&testData2)+1;//��������2
        Modbus_HoldReg[4]=(u16*)&testData1;
        Modbus_HoldReg[5]=(u16*)&testData2;
        Modbus_HoldReg[6]=(u16*)&testData3;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




u16 CRC_Compute(u8 *puchMsg, u16 usDataLen)
{
    u8 uchCRCHi = 0xFF ;
    u8 uchCRCLo = 0xFF ;
    u32 uIndex ;
    while (usDataLen--)
    {
        uIndex = uchCRCHi ^ *puchMsg++ ;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return ((uchCRCHi<< 8)  | (uchCRCLo)) ;
}//uint16 crc16(uint8 *puchMsg, uint16 usDataLen)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ʼ��USART2
void RS485_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//PA2��TX�������������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        GPIO_SetBits(GPIOA,GPIO_Pin_2);//Ĭ�ϸߵ�ƽ

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//PA3��RX����������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //�޸�ԭGPIO_Mode_IPU������������->GPIO_Mode_IN_FLOATING(��������)/////////////////////////////////////////////
        GPIO_Init(GPIOA,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//�޸�PG9��RE/DE��ͨ���������->PD7��RE/DE��ͨ���������//////////////////////////////////////////////////////////////////////
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_Init(GPIOG,&GPIO_InitStructure);
        GPIO_ResetBits(GPIOG,GPIO_Pin_9);//Ĭ�Ͻ���״̬

        USART_DeInit(USART2);//��λ����2
        USART_InitStructure.USART_BaudRate=RS485_Baudrate;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
        switch(RS485_Parity)
        {
                case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//��У��
                case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//��У��
                case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//żУ��
        }
        USART_Init(USART2,&USART_InitStructure);

        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ʹ�ܴ���2�����ж�

        NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        USART_Cmd(USART2,ENABLE);//ʹ�ܴ���2
        RS485_TX_EN=0;//Ĭ��Ϊ����ģʽ

        Timer7_Init();//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��
        Modbus_RegMap();//Modbus�Ĵ���ӳ��
}

//��ʱ��7��ʼ��
void Timer7_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7ʱ��ʹ��

        //TIM7��ʼ������
        TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance*10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
        TIM_TimeBaseStructure.TIM_Prescaler =7200; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ���ü���Ƶ��Ϊ10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );//TIM7 ��������ж�

        //TIM7�жϷ�������
        NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;  //TIM7�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}



//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff,u8 len)
{
        RS485_TX_EN=1;//�л�Ϊ����ģʽ
        while(len--)
        {
                while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
                USART_SendData(USART2,*(buff++));
        }
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//�ȴ��������
}


/////////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)//����2�жϷ������
{

        u8 res;
        u8 err;

        if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
                else err=0;
                LED0=0;
                res=USART_ReceiveData(USART2); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����

                if((RS485_RX_CNT<2047)&&(err==0))
                {
                        modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT]=res;
                        modbusUsr.RS485_RX_CNT++;

                        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//�����ʱ������ж�
                        TIM_SetCounter(TIM7,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
                        TIM_Cmd(TIM7,ENABLE);//��ʼ��ʱ
                }
        }
}

///////////////////////////////////////////////////////////////////////////////////////
//�ö�ʱ��7�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��7�жϷ������
void TIM7_IRQHandler(void)
{
        if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
        {
                TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����жϱ�־
                TIM_Cmd(TIM7,DISABLE);//ֹͣ��ʱ��
                modbusUsr.RS485_TX_EN=1;//ֹͣ���գ��л�Ϊ����״̬
                modbusUsr.RS485_FrameFlag=1;//��λ֡�������
        }
}

/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)
u16 startRegAddr;
u16 RegNum;
u16 calCRC;
void RS485_Service(void)
{
        u16 recCRC;
        if(modbusUsr.RS485_FrameFlag==1)
        {
                if(modbusUsr.RS485_RX_BUFF[0]==modbusUsr.RS485_Addr)//��ַ��ȷ
                {
                        if((modbusUsr.RS485_RX_BUFF[1]==01)||(modbusUsr.RS485_RX_BUFF[1]==02)||(modbusUsr.RS485_RX_BUFF[1]==03)||(modbusUsr.RS485_RX_BUFF[1]==05)||(modbusUsr.RS485_RX_BUFF[1]==06)||(modbusUsr.RS485_RX_BUFF[1]==15)||(modbusUsr.RS485_RX_BUFF[1]==16))//��������ȷ
                  {
                                startRegAddr=(((u16)modbusUsr.RS485_RX_BUFF[2])<<8)|modbusUsr.RS485_RX_BUFF[3];//��ȡ�Ĵ�����ʼ��ַ
                                if(startRegAddr<1000)//�Ĵ�����ַ�ڷ�Χ��
                                {
                                        calCRC=CRC_Compute(modbusUsr.RS485_RX_BUFF,modbusUsr.RS485_RX_CNT-2);//�������������ݵ�CRC
                                        recCRC=modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT-1]|(((u16)modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT-2])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
                                        if(calCRC==recCRC)//CRCУ����ȷ
                                        {
                                                ///////////��ʾ��

        LCD_ShowxNum(10,230,modbusUsr.RS485_RX_BUFF[0],3,16,0X80);//��ʾ����
         LCD_ShowxNum(42,230,modbusUsr.RS485_RX_BUFF[1],3,16,0X80);//��ʾ����
        LCD_ShowxNum(74,230,modbusUsr.RS485_RX_BUFF[2],3,16,0X80);//��ʾ����
        LCD_ShowxNum(106,230,modbusUsr.RS485_RX_BUFF[3],3,16,0X80);//��ʾ����
        LCD_ShowxNum(138,230,modbusUsr.RS485_RX_BUFF[4],3,16,0X80);//��ʾ����
        LCD_ShowxNum(170,230,modbusUsr.RS485_RX_BUFF[5],3,16,0X80);//��ʾ����
///////////////////////
                                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                switch(modbusUsr.RS485_RX_BUFF[1])//���ݲ�ͬ�Ĺ�������д���
                                                {
                                                        case 2://�����뿪����
                                                        {
                                                                Modbus_02_Solve();
                                                                break;
                                                        }

                                                        case 1://�����������
                                                        {
                                                                Modbus_01_Solve();
                                                                break;
                                                        }

                                                        case 5://д�������������
                                                        {
                                                                Modbus_05_Solve();
                                                                break;
                                                        }

                                                        case 15://д������������
                                                        {
                                                                Modbus_15_Solve();
                                                                break;
                                                        }

                                                        case 03: //������Ĵ���
                                                        {
                                                                Modbus_03_Solve();
                                                                break;
                                                        }

                                                        case 06: //д�����Ĵ���
                                                        {
                                                                Modbus_06_Solve();
                                                                break;
                                                        }

                                                        case 16: //д����Ĵ���
                                                        {
                                                                Modbus_16_Solve();
                                                                break;
                                                        }


                                                }
                                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                        }
                                        else//CRCУ�����
                                        {

                                                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                                modbusUsr.RS485_TX_BUFF[2]=0x04; //�쳣��
                                                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                                        }
                                }
                                else//�Ĵ�����ַ������Χ
                                {
                                        modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                        modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                        modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                                        RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                                }
                        }
                        else//���������
                        {
                                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                RS485_TX_BUFF[2]=0x01; //�쳣��
                                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                        }
          }

                modbusUsr.RS485_FrameFlag=0;//��λ֡������־
                modbusUsr.RS485_RX_CNT=0;//���ռ���������
                RS485_TX_EN=0;//��������ģʽ
        }
}

//Modbus������02�������/////////////////////////////////////////////////////��������֤OK -----����������PE4 PE3 PE2 PA0 ��ʼ�������ſ���OK    KEY_Init();
//�����뿪����
void Modbus_02_Solve(void)
{
        u16 ByteNum;
        u16 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                ByteNum=RegNum/8;//�ֽ���
                if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
                modbusUsr.RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
                for(i=0;i<RegNum;i++)
                {
                        if(i%8==0) modbusUsr.RS485_TX_BUFF[3+i/8]=0x00;
                        modbusUsr.RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
                        modbusUsr.RS485_TX_BUFF[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
                        if(i==RegNum-1)//���͵����һ��λ��
                        {
                                if(RegNum%8) modbusUsr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
                        }
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,ByteNum+3);
                modbusUsr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,ByteNum+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus������01������� ///////////////////////////////////////////////////////////��������֤OK
//�����������
void Modbus_01_Solve(void)
{
        u16 ByteNum;
        u16 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                ByteNum=RegNum/8;//�ֽ���
                if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
                modbusUsr.RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
                for(i=0;i<RegNum;i++)
                {
                        if(i%8==0) modbusUsr.RS485_TX_BUFF[3+i/8]=0x00;
                        modbusUsr.RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
                        modbusUsr.RS485_TX_BUFF[3+i/8]|=((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
                        if(i==RegNum-1)//���͵����һ��λ��
                        {
                                if(RegNum%8) RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
                        }
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,ByteNum+3);
                modbusUsr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,ByteNum+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus������05�������   ///////////////////////////////////////////////////////��������֤OK
//д�������������
void Modbus_05_Solve(void)
{
        if(startRegAddr<100)//�Ĵ�����ַ�ڷ�Χ��
        {
                if((modbusUsr.RS485_RX_BUFF[4]==0xFF)||(modbusUsr.RS485_RX_BUFF[5]==0xFF)) *Modbus_OutputIO[startRegAddr]=0x01;
                else *Modbus_OutputIO[startRegAddr]=0x00;

                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                modbusUsr.RS485_TX_BUFF[2]=modbusUsr.RS485_RX_BUFF[2];
                modbusUsr.RS485_TX_BUFF[3]=modbusUsr.RS485_RX_BUFF[3];
                modbusUsr.RS485_TX_BUFF[4]=modbusUsr.RS485_RX_BUFF[4];
                modbusUsr.RS485_TX_BUFF[5]=modbusUsr.RS485_RX_BUFF[5];

                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,6);
                modbusUsr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,8);
        }
        else//�Ĵ�����ַ������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus������15�������   //////////////////////////////////////////////////////��������֤OK
//д������������
void Modbus_15_Solve(void)
{
        u16 i;
        RegNum=(((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                for(i=0;i<RegNum;i++)
                {
                        if(modbusUsr.RS485_RX_BUFF[7+i/8]&0x01) *Modbus_OutputIO[startRegAddr+i]=0x01;
                        else *Modbus_OutputIO[startRegAddr+i]=0x00;
                        modbusUsr.RS485_RX_BUFF[7+i/8]>>=1;//�ӵ�λ��ʼ
                }

                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                modbusUsr.RS485_TX_BUFF[2]=modbusUsr.RS485_RX_BUFF[2];
                modbusUsr.RS485_TX_BUFF[3]=modbusUsr.RS485_RX_BUFF[3];
                modbusUsr.RS485_TX_BUFF[4]=modbusUsr.RS485_RX_BUFF[4];
                modbusUsr.RS485_TX_BUFF[5]=modbusUsr.RS485_RX_BUFF[5];
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,6);
                modbusUsr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,8);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���
void Modbus_03_Solve(void)
{
        u8 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                modbusUsr.RS485_TX_BUFF[2]=RegNum*2;
                for(i=0;i<RegNum;i++)
                {
                        modbusUsr.RS485_TX_BUFF[3+i*2]=(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                        modbusUsr.RS485_TX_BUFF[4+i*2]=(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,RegNum*2+3);
                modbusUsr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
                modbusUsr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,RegNum*2+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}


//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////����֤����OK
//д�������ּĴ���
void Modbus_06_Solve(void)
{
        *Modbus_HoldReg[startRegAddr]=modbusUsr.RS485_RX_BUFF[4]<<8;//���ֽ���ǰ                    ////////�޸�Ϊ���ֽ���ǰ�����ֽ��ں�
        *Modbus_HoldReg[startRegAddr]|=((u16)modbusUsr.RS485_RX_BUFF[5]);//���ֽ��ں�

        modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
        modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
        modbusUsr.RS485_TX_BUFF[2]=modbusUsr.RS485_RX_BUFF[2];
        modbusUsr.RS485_TX_BUFF[3]=modbusUsr.RS485_RX_BUFF[3];
        modbusUsr.RS485_TX_BUFF[4]=modbusUsr.RS485_RX_BUFF[4];
        modbusUsr.RS485_TX_BUFF[5]=modbusUsr.RS485_RX_BUFF[5];

        calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,6);
        modbusUsr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
        modbusUsr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
        RS485_SendData(modbusUsr.RS485_TX_BUFF,8);
}

//Modbus������16������� /////////////////////////////////////////////////////////////////////////////////////////////////����֤����OK
//д������ּĴ���
void Modbus_16_Solve(void)
{
        u8 i;
        RegNum= (((u16)RS485_RX_BUFF[4])<<8)|((RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                for(i=0;i<RegNum;i++)
                {
                        *Modbus_HoldReg[startRegAddr+i]=modbusUsr.RS485_RX_BUFF[7+i*2]; //���ֽ���ǰ                 /////// ���ֽ���ǰ�����ֽ��ں�����
                        *Modbus_HoldReg[startRegAddr+i]|=((u16)modbusUsr.RS485_RX_BUFF[8+i*2])<<8; //���ֽ��ں�
                }

                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                modbusUsr.RS485_TX_BUFF[2]=modbusUsr.RS485_RX_BUFF[2];
                modbusUsr.RS485_TX_BUFF[3]=modbusUsr.RS485_RX_BUFF[3];
                modbusUsr.RS485_TX_BUFF[4]=modbusUsr.RS485_RX_BUFF[4];
                modbusUsr.RS485_TX_BUFF[5]=modbusUsr.RS485_RX_BUFF[5];

                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,6);
                modbusUsr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,8);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}



