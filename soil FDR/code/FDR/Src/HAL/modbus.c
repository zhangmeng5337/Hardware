#include "modbus.h"
#include "main.h"
#include "crc.h"
#include "bsp.h"
#include "adc.h"
#include "eeprom.h"
#include "filter.h"
void Modbus_01_Solve(void);
void Modbus_02_Solve(void);
void Modbus_03_Solve(void);
void Modbus_05_Solve(void);
void Modbus_06_Solve(void);
void Modbus_07_Solve(void);
void Modbus_15_Solve(void);
void Modbus_16_Solve(void);

u16 startRegAddr;
u16 RegNum;
u16 calCRC;
unsigned char TesetFlag;
extern sensor_stru sensor_usr;

///////////////////////////////////////////////////////////
//u32 RS485_Baudrate=9600;//ͨѶ������
//u8 modbus_usr.RS485_Parity=0;//0��У�飻1��У�飻2żУ��
//u8 modbus_usr.RS485_Addr=1;//�ӻ���ַ
//u16 modbus_usr.RS485_Frame_Distance=4;//����֡��С�����ms),������ʱ������Ϊ����һ֡

//u8 modbus_usr.RS485_RX_BUFF[2048];//���ջ�����2048�ֽ�
//u16 modbus_usr.RS485_RX_CNT=0;//���ռ�����
//u8 modbus_usr.RS485_FrameFlag=0;//֡�������
//u8 modbus_usr.RS485_TX_BUFF[2048];//���ͻ�����
//u16 modbus_usr.RS485_TX_CNT=0;//���ͼ�����
modbus_stru modbus_usr;
extern TIM_HandleTypeDef htim2;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus�Ĵ����͵�Ƭ���Ĵ�����ӳ���ϵ
//vu32 *Modbus_InputIO[100];//���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
//vu32 *Modbus_OutputIO[100];//����������Ĵ���ָ��(����ʹ�õ���λ������)
//u16 *Modbus_HoldReg[1000];//���ּĴ���ָ��
//u32 testData1=1201,testData2=1002,testData3=2303,testData4=8204;
//void Modbus_RegMap(void)
//{


//         //���뿪�����Ĵ���ָ��ָ��
//        Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)��ȡPE4�ĵ�ַ��(vu32*)&PEin(4)��PE4��ַǿ��ת��Ϊuw32���͵ĵ�ַ��Modbus_InputIO[0]=(vu32*)&PEin(4); ��ת���õĵ�ַ�͸���ַָ��Modbus_InputIO[0]��
//        Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*Modbus_InputIO[0] ȡ����ַ�е����ݡ�
//        Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
//        Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3

//        //����������Ĵ���ָ��ָ��
//        Modbus_OutputIO[0]=(vu32*)&PBout(5);//LED0
//        Modbus_OutputIO[1]=(vu32*)&PEout(5);//LED1

//        //���ּĴ���ָ��ָ��
//        Modbus_HoldReg[0]=(u16*)&testData1;//��������1
//        Modbus_HoldReg[1]=(u16*)&testData2;//((u16*)&testData1)+1;//��������1
//        Modbus_HoldReg[2]=(u16*)&testData3;//(u16*)&testData2;//��������2
//        Modbus_HoldReg[3]=(u16*)&testData4;//((u16*)&testData2)+1;//��������2
//        Modbus_HoldReg[4]=(u16*)&testData1;
//        Modbus_HoldReg[5]=(u16*)&testData2;
//        Modbus_HoldReg[6]=(u16*)&testData3;
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef huart1;
//��ʼ��USART2

unsigned char res;
void RS485_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = modbus_usr.RS485_Baudrate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    switch(modbus_usr.RS485_Parity)
    {
    case 0:
        huart1.Init.Parity=UART_PARITY_NONE;
        break;//��У��
    case 1:
        huart1.Init.Parity=UART_PARITY_ODD;
        break;//��У��
    case 2:
        huart1.Init.Parity=UART_PARITY_EVEN;
        break;//żУ��
    }
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//Ĭ��Ϊ����ģʽ
    HAL_UART_Receive_IT(&huart1, &res, 1);
  //  modbus_usr.RS485_Baudrate = 9600;


    // Timer7_Init();//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��
    // Modbus_RegMap();//Modbus�Ĵ���ӳ��
}

//��ʱ��7��ʼ��




//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff,u8 len)
{
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
    modbus_usr.RS485_TX_EN=1;//�л�Ϊ����ģʽ
    //  while(len--)
    {
        HAL_UART_Transmit(&huart1,buff,len,100);
        //while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
        // USART_SendData(USART2,*(buff++));
    }
    //while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//�ȴ��������
}

unsigned char count;
uint32_t Tick4ms;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 err;
    if(huart->Instance == USART1)
    {
			 count++;
        if(HAL_UART_GetError(&huart1)) err=1;//��⵽������֡�����У�����
        else err=0;
        if((modbus_usr.RS485_RX_CNT<2047)&&(err==0))
        {
            modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT]=res;
            modbus_usr.RS485_RX_CNT++;
						modbus_usr.RS485_FrameFlag=1;//��λ֡�������
            Tick4ms =HAL_GetTick();			 //�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
						//��ʼ��ʱ
        }            
        HAL_UART_Receive_IT(&huart1, &res, 1); 	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
//        //led_ctrl(2);
    }


}


///////////////////////////////////////////////////////////////////////////////////////
//�ö�ʱ��2�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��2�жϷ������

void timCallback()
{

        //��д�ص��߼�������ʱ��1��ʱ1MS����߼�
        HAL_TIM_Base_Stop_IT(&htim2);  //ֹͣ��ʱ����ʱ�������������ر�
        modbus_usr.RS485_TX_EN=1;//ֹͣ���գ��л�Ϊ����״̬
		if(modbus_usr.RS485_FrameFlag==1)
		{
			if((HAL_GetTick()-Tick4ms)>=4)
			modbus_usr.RS485_FrameFlag=2;//��λ֡�������

		}
		else if(modbus_usr.RS485_FrameFlag!=2)
		{
			Tick4ms =HAL_GetTick();
			modbus_usr.RS485_FrameFlag=3;//��λ֡�������		
		}


}
/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)

void RS485_Service(void)
{
	static uint32_t tickTime_adc;
	static unsigned char tick_1s;
    u16 recCRC;
		if((HAL_GetTick()-tickTime_adc)>=200)
		{
			 Get_Adc_Average(N);
			tickTime_adc=HAL_GetTick();
			tick_1s++;			
			if(TesetFlag==1&&tick_1s>=15)
			{
				  tick_1s = 0;
					Modbus_07_Solve();
			}	
		}
		else
			tickTime_adc=tickTime_adc;
	  

		
    if(modbus_usr.RS485_FrameFlag==2)
    {
        if(modbus_usr.RS485_RX_BUFF[0]==modbus_usr.RS485_Addr)//��ַ��ȷ
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06)||(modbus_usr.RS485_RX_BUFF[1]==07))//��������ȷ
            {
                startRegAddr=(((u16)modbus_usr.RS485_RX_BUFF[2])<<8)|modbus_usr.RS485_RX_BUFF[3];//��ȡ�Ĵ�����ʼ��ַ
                if(startRegAddr<1000)//�Ĵ�����ַ�ڷ�Χ��
                {
                    calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//�������������ݵ�CRC
                    recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
                    if(calCRC==recCRC)//CRCУ����ȷ
                    {
                        ///////////��ʾ��

                        //LCD_ShowxNum(10,230,modbus_usr.RS485_RX_BUFF[0],3,16,0X80);//��ʾ����
                        //  LCD_ShowxNum(42,230,modbus_usr.RS485_RX_BUFF[1],3,16,0X80);//��ʾ����
                        // LCD_ShowxNum(74,230,modbus_usr.RS485_RX_BUFF[2],3,16,0X80);//��ʾ����
                        // LCD_ShowxNum(106,230,modbus_usr.RS485_RX_BUFF[3],3,16,0X80);//��ʾ����
                        // LCD_ShowxNum(138,230,modbus_usr.RS485_RX_BUFF[4],3,16,0X80);//��ʾ����
                        // LCD_ShowxNum(170,230,modbus_usr.RS485_RX_BUFF[5],3,16,0X80);//��ʾ����
///////////////////////
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        switch(modbus_usr.RS485_RX_BUFF[1])//���ݲ�ͬ�Ĺ�������д���
                        {
                        case 03: //���Ĵ���
                        {
													  
													TesetFlag = 0;
                            Modbus_03_Solve();
                            break;
                        }

                        case 06: //д�����Ĵ���
                        {
													  TesetFlag = 0;
                            Modbus_06_Solve();
                            break;
                        }
                        case 07: //����ģʽ������������� 0x01	0x07	0x0002	0x0002	0x940b
                        {
													  TesetFlag = 1;
                           // Modbus_07_Solve();
                            break;
                        }
                        default:TesetFlag = 0;
                            ;


                        }
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else//CRCУ�����
                    {

                        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                        modbus_usr.RS485_TX_BUFF[2]=0x04; //�쳣��
                        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                    }
                }
                else//�Ĵ�����ַ������Χ
                {
                    modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                    modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                    modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
                    RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                }
            }
            else//���������
            {
                modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                modbus_usr.RS485_TX_BUFF[2]=0x01; //�쳣��
                RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
            }
        }
       
        modbus_usr.RS485_FrameFlag=0;//��λ֡������־
        modbus_usr.RS485_RX_CNT=0;//���ռ���������
        modbus_usr.RS485_TX_EN=0;//��������ģʽ
    }
		if( modbus_usr.RS485_FrameFlag==3)
		{
        modbus_usr.RS485_FrameFlag=0;//��λ֡������־
        modbus_usr.RS485_RX_CNT=0;//���ռ���������
        modbus_usr.RS485_TX_EN=0;//��������ģʽ
		}
		HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
}

//Modbus������02�������/////////////////////////////////////////////////////��������֤OK -----����������PE4 PE3 PE2 PA0 ��ʼ�������ſ���OK    KEY_Init();
//�����뿪����
void Modbus_02_Solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        ByteNum=RegNum/8;//�ֽ���
        if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
        modbus_usr.RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
        for(i=0; i<RegNum; i++)
        {
            if(i%8==0) modbus_usr.RS485_TX_BUFF[3+i/8]=0x00;
            modbus_usr.RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
            //  modbus_usr.RS485_TX_BUFF[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
            if(i==RegNum-1)//���͵����һ��λ��
            {
                if(RegNum%8) modbus_usr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,ByteNum+3);
        modbus_usr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,ByteNum+5);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus������01������� ///////////////////////////////////////////////////////////��������֤OK
//�����������
void Modbus_01_Solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        ByteNum=RegNum/8;//�ֽ���
        if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
        modbus_usr.RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
        for(i=0; i<RegNum; i++)
        {
            if(i%8==0) modbus_usr.RS485_TX_BUFF[3+i/8]=0x00;
            // modbus_usr.RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
            // modbus_usr.RS485_TX_BUFF[3+i/8]|=((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
            if(i==RegNum-1)//���͵����һ��λ��
            {
                if(RegNum%8) modbus_usr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,ByteNum+3);
        modbus_usr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,ByteNum+5);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus������05�������   ///////////////////////////////////////////////////////��������֤OK
//д�������������
void Modbus_05_Solve(void)
{
    if(startRegAddr<100)//�Ĵ�����ַ�ڷ�Χ��
    {
        if((modbus_usr.RS485_RX_BUFF[4]==0xFF)||(modbus_usr.RS485_RX_BUFF[5]==0xFF))
            ;//									*Modbus_OutputIO[startRegAddr]=0x01;
        else
            ;
//									*Modbus_OutputIO[startRegAddr]=0x00;

        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=modbus_usr.RS485_RX_BUFF[2];
        modbus_usr.RS485_TX_BUFF[3]=modbus_usr.RS485_RX_BUFF[3];
        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];
        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];

        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
    else//�Ĵ�����ַ������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus������15�������   //////////////////////////////////////////////////////��������֤OK
//д������������
void Modbus_15_Solve(void)
{
    u16 i;
    RegNum=(((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        for(i=0; i<RegNum; i++)
        {
            if(modbus_usr.RS485_RX_BUFF[7+i/8]&0x01)
                ;//*Modbus_OutputIO[startRegAddr+i]=0x01;
            else
                ;//*Modbus_OutputIO[startRegAddr+i]=0x00;
            modbus_usr.RS485_RX_BUFF[7+i/8]>>=1;//�ӵ�λ��ʼ
        }

        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=modbus_usr.RS485_RX_BUFF[2];
        modbus_usr.RS485_TX_BUFF[3]=modbus_usr.RS485_RX_BUFF[3];
        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];
        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���

void Modbus_03_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=RegNum*2;
        for(i=0; i<RegNum; i++)
        {
            unsigned int tmp;
            if(startRegAddr == RH_ADDR)
            {
                if(i<1)
                {
									  tmp = sensor_usr.rh;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                else if(i<2)
                {
									  tmp = sensor_usr.temperature;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == TE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == DEV_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Addr>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(modbus_usr.RS485_Addr);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == RATE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Baudrate>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)modbus_usr.RS485_Baudrate;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,RegNum*2+3);
        modbus_usr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
        modbus_usr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,RegNum*2+5);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}


//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////����֤����OK
//д�������ּĴ���

void Modbus_06_Solve(void)
{
    ;//*Modbus_HoldReg[startRegAddr]=modbus_usr.RS485_RX_BUFF[4]<<8;//���ֽ���ǰ                    ////////�޸�Ϊ���ֽ���ǰ�����ֽ��ں�
    ;//*Modbus_HoldReg[startRegAddr]|=((u16)modbus_usr.RS485_RX_BUFF[5]);//���ֽ��ں�

//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
//        modbus_usr.RS485_TX_BUFF[2]=modbus_usr.RS485_RX_BUFF[2];
//        modbus_usr.RS485_TX_BUFF[3]=modbus_usr.RS485_RX_BUFF[3];
//        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];
//        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];
    unsigned char i;
	RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=startRegAddr>>8;
        modbus_usr.RS485_TX_BUFF[3]=startRegAddr;			
        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];	
        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];			
        for(i=0; i<RegNum; i++)
        {

            if(startRegAddr == DEV_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_Addr = modbus_usr.RS485_TX_BUFF[5];

                    //modbus_usr.RS485_TX_BUFF[3+i*2]=(modbus_usr.RS485_Addr>>8)&&0xff;;//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    //modbus_usr.RS485_TX_BUFF[4+i*2]=(modbus_usr.RS485_Addr)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == RATE_ADDR)
            {
                if(i<1)
                {
                    switch(modbus_usr.RS485_TX_BUFF[5])
                    {
                    case 0x00:
                        modbus_usr.RS485_Baudrate=2400;
                        break;
                    case 0x01:
                        modbus_usr.RS485_Baudrate=4800;
                        break;
                    case 0x02:
                        modbus_usr.RS485_Baudrate=9600;
                        break;
                    case 0x03:
                        modbus_usr.RS485_Baudrate=115200;
                        break;
                    default:
                        modbus_usr.RS485_Baudrate=9600;
                        break;
                    }
                    //(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//
                    RS485_Init();															/////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    //modbus_usr.RS485_TX_BUFF[4+i*2]=(sensor_usr.temperature)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            flash_write(0,0x5a,1);
            flash_write(1,modbus_usr.RS485_Addr,1);
            flash_write(2,modbus_usr.RS485_Baudrate,1);
            flash_write(3,modbus_usr.RS485_Parity,1);
        }
          calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
         RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }



    //calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
    // modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    // modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
    // RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
}

//Modbus������16������� /////////////////////////////////////////////////////////////////////////////////////////////////����֤����OK
//д������ּĴ���
void Modbus_16_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
    {
        for(i=0; i<RegNum; i++)
        {
            // *Modbus_HoldReg[startRegAddr+i]=modbus_usr.RS485_RX_BUFF[7+i*2]; //���ֽ���ǰ                 /////// ���ֽ���ǰ�����ֽ��ں�����
            // *Modbus_HoldReg[startRegAddr+i]|=((u16)modbus_usr.RS485_RX_BUFF[8+i*2])<<8; //���ֽ��ں�
        }

        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=modbus_usr.RS485_RX_BUFF[2];
        modbus_usr.RS485_TX_BUFF[3]=modbus_usr.RS485_RX_BUFF[3];
        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];
        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];

        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
    else//�Ĵ�����ַ+����������Χ
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}
//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���

void Modbus_07_Solve(void)
{
    u8 i;
	   HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
	   printf("RH  TE1  TE2 Voltage:   %f   %f   %f\n",sensor_usr.sensor[0],sensor_usr.sensor[1],sensor_usr.sensor[2]);
	   HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
//    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
//    if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
//    {
//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
//        modbus_usr.RS485_TX_BUFF[2]=RegNum*2;
//        for(i=0; i<RegNum; i++)
//        {
//            unsigned int tmp;
//            if(startRegAddr == RH_ADDR)
//            {
//                if(i<1)
//                {
//									  tmp = sensor_usr.rh;
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//                else if(i<2)
//                {
//									  tmp = sensor_usr.temperature;
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == TE_ADDR)
//            {
//                if(i<1)
//                {
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == DEV_ADDR)
//            {
//                if(i<1)
//                {
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Addr>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(modbus_usr.RS485_Addr);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == RATE_ADDR)
//            {
//                if(i<1)
//                {
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Baudrate>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)modbus_usr.RS485_Baudrate;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//        }
//        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,RegNum*2+3);
//        modbus_usr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
//        modbus_usr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
//        RS485_SendData(modbus_usr.RS485_TX_BUFF,RegNum*2+5);
//    }
//    else//�Ĵ�����ַ+����������Χ
//    {
//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
//        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
//        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
//    }
}
