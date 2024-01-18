#include "modbus.h"
#include "main.h"
#include "crc.h"
extern UART_HandleTypeDef huart5;
void Modbus_01_Solve(void);
void Modbus_02_Solve(void);
void Modbus_03_Solve(void);
void Modbus_05_Solve(void);
void Modbus_06_Solve(void);
void Modbus_07_Solve(void);
void Modbus_08_Solve(void);
void Modbus_09_Solve(void);
void Modbus_16_Solve(void);
void RS485_SendData(u8 *buff,u8 len,u8 flag);
//extern factor_stru factor_usr;
u16 startRegAddr;
u16 RegNum;
u16 calCRC;
unsigned char TesetFlag;
u8 modbus_role = 0;

//modbus_stru modbus_usr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef huart1;
unsigned char res;
modbus_stru modbus_usr;
void sensorModbusRate(unsigned int p,unsigned char num)
{
        unsigned char addr;
		switch(p)
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
		//(*Modbus_Hold
       for(addr = 1;addr<6;addr++)
       	{
		   modbus_usr.RS485_TX_BUFF[0]=addr;
		   modbus_usr.RS485_TX_BUFF[1]=0x06;
		   modbus_usr.RS485_TX_BUFF[2]=0x01;
		   modbus_usr.RS485_TX_BUFF[3]=0x01;		   
		   modbus_usr.RS485_TX_BUFF[4]=0;  
		   modbus_usr.RS485_TX_BUFF[5]=p;
		   
			calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
		   modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;		 //CRC�ߵ�λ������  // �ȸߺ��
		   modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
		   RS485_SendData(modbus_usr.RS485_TX_BUFF,8,0);

	   }
		RS485_Init();
}

void RS485_Init()
{
	  
    huart5.Instance = UART5;
    huart5.Init.BaudRate = modbus_usr.RS485_Baudrate;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    switch(modbus_usr.RS485_Parity)
    {
    case 0:
        huart5.Init.Parity=UART_PARITY_NONE;
        break;//��У��
    case 1:
        huart5.Init.Parity=UART_PARITY_ODD;
        break;//��У��
    case 2:
        huart5.Init.Parity=UART_PARITY_EVEN;
        break;//żУ��
    }
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//Ĭ��Ϊ����ģʽ
    //HAL_UART_Receive_IT(&huart5, &res, 1);
		
	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	__HAL_UART_DISABLE_IT(&huart5, UART_IT_IDLE);	//ʹ�ܿ����ж�
	HAL_UART_DMAStop(&huart5);
	HAL_UART_DMAResume(&huart5);
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);	//ʹ�ܿ����ж�
	HAL_UART_Receive_DMA(&huart5,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
}


///////////////////////////////////////////////////////////////////////////////////////
//�ö�ʱ��2�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��2�жϷ������
uint32_t Tick4ms;
void timCallback()
{

        //��д�ص��߼�������ʱ��1��ʱ1MS����߼�
        //HAL_TIM_Base_Stop_IT(&htim2);  //ֹͣ��ʱ����ʱ�������������ر�
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
unsigned char *modbusFrameStatus(void)
{
  return &(modbus_usr.RS485_FrameFlag);
}
unsigned char *modbusBuffer()
{
  return modbus_usr.RS485_RX_BUFF;
}

/*************************************************************
				485���ڽ����жϻص�����
*************************************************************/
void RS485_RxCpltCallback()
{
//    u8 err;
	  
//	//  if(huart->Instance == USART1)
//	{
//		if(HAL_UART_GetError(&huart5)) err=1;//��⵽������֡�����У�����
//		else err=0;
//		if((modbus_usr.RS485_RX_CNT<2047)&&(err==0))
//		{
//			modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT]=res;
//			modbus_usr.RS485_RX_CNT++;
//			modbus_usr.RS485_FrameFlag=1;//��λ֡�������
//			Tick4ms =HAL_GetTick();			 //�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
//			//��ʼ��ʱ
//		}            
//		HAL_UART_Receive_IT(&huart5, &res, 1); 	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
			modbus_usr.RS485_FrameFlag=2;//��λ֡�������
			HAL_UART_DMAStop(&huart5);
			HAL_UART_DMAResume(&huart5);
			HAL_UART_Receive_DMA(&huart5,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
}

//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff,u8 len,unsigned char flag)
{
	
    if(flag == 0)
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
    modbus_usr.RS485_TX_EN=1;//�л�Ϊ����ģʽ
    //  while(len--)
    {
        HAL_UART_Transmit(&huart5,buff,len,300);
        //while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
        // USART_SendData(USART2,*(buff++));
    }
    //while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//�ȴ��������
}

//unsigned char count;





/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)

void RS485_Service(void)
{
	static uint32_t tickTime_adc;
	static unsigned char tick_1s;
    u16 recCRC;
		if((HAL_GetTick()-tickTime_adc)>=50)
		{
			 //Get_Adc_Average(N);
			tickTime_adc=HAL_GetTick();
			tick_1s++;			
			if(TesetFlag==1&&tick_1s>=30)
			{
				  tick_1s = 0;
					Modbus_07_Solve();
			}	
		}
		else
			tickTime_adc=tickTime_adc;
	  

		
    if(modbus_usr.RS485_FrameFlag==2)
    {
        if(modbus_usr.RS485_RX_BUFF[0]<=(modbus_usr.RS485_Addr+5))//��ַ��ȷ
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06)||(modbus_usr.RS485_RX_BUFF[1]==07)
							||(modbus_usr.RS485_RX_BUFF[1]==8)||(modbus_usr.RS485_RX_BUFF[1]==9))//��������ȷ
            {
                startRegAddr=(((u16)modbus_usr.RS485_RX_BUFF[2])<<8)|modbus_usr.RS485_RX_BUFF[3];//��ȡ�Ĵ�����ʼ��ַ
                if(startRegAddr<1000)//�Ĵ�����ַ�ڷ�Χ��
                {
                    calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//�������������ݵ�CRC
                    recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
                    if(calCRC==recCRC)//CRCУ����ȷ
                    {
											TesetFlag = 0;
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        switch(modbus_usr.RS485_RX_BUFF[1])//���ݲ�ͬ�Ĺ�������д���
                        {
                        case 3: //���Ĵ���
                        {
                            Modbus_03_Solve();
                            break;
                        }

                        case 6: //д�����Ĵ���
                        {
                            Modbus_06_Solve();
                            break;
                        }
                        case 7: //����ģʽ������������� 0x01	0x07	0x0002	0x0002	0x940b
                        {
													  TesetFlag = 1;
                           // Modbus_07_Solve();
                            break;
                        }
                        case 8: //ʪ��У׼ϵ��
                        {
													 
                            Modbus_08_Solve();
                            break;
                        }
                        case 9: //�ο���ѹ���¶Ȳ�����ʪ�ȵ�ѹ����
                        {
	                            Modbus_09_Solve();
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
                       // RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                    }
                }
                else//�Ĵ�����ַ������Χ
                {
                    modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                    modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                    modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
                    //RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                }
            }
            else//���������
            {
                modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                modbus_usr.RS485_TX_BUFF[2]=0x01; //�쳣��
                //RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
            }
        }
       
       // modbus_usr.RS485_FrameFlag=0;//��λ֡������־
       // modbus_usr.RS485_RX_CNT=0;//���ռ���������
       // modbus_usr.RS485_TX_EN=0;//��������ģʽ
    }
		if( modbus_usr.RS485_FrameFlag==3)
		{
        modbus_usr.RS485_FrameFlag=0;//��λ֡������־
        modbus_usr.RS485_RX_CNT=0;//���ռ���������
        modbus_usr.RS485_TX_EN=0;//��������ģʽ
		}
		HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
}

//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���

void Modbus_03_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x02||startRegAddr==0x03||startRegAddr==0x0100||startRegAddr==0x0101))//�Ĵ�����ַ+�����ڷ�Χ��
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
									  //tmp = sensor_usr.rh;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                else if(i<2)
                {
									//  tmp = sensor_usr.temperature;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == TE_ADDR)
            {
                if(i<1)
                {
                   // modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////�ȷ��͸��ֽ�--�ڷ��͵��ֽ�
                   // modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
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
		if(modbus_role == 0)
		{
		   modbus_role = 3;
			//RS485_SendData(modbus_usr.RS485_TX_BUFF,RegNum*2+5);

		}
		else if(modbus_role == 1)
		{
			modbus_role = 2;
			//RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
		}

    }
}


//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////����֤����OK
//д�������ּĴ���

void Modbus_06_Solve(void)
{
    unsigned char i;
	   RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0100||startRegAddr==0x0101))//�Ĵ�����ַ+�����ڷ�Χ��
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
        }
          calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
         
		if(modbus_role == 0)
		{
		   modbus_role = 2;
		   RS485_SendData(modbus_usr.RS485_TX_BUFF,8,0);
		}
		else if(modbus_role == 1)
		{
			modbus_role = 2;
			//RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
		}

    }
}


//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���

void Modbus_07_Solve(void)
{


	
   RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0103))//�Ĵ�����ַ+�����ڷ�Χ��
    {
	   HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
	  // printf("RH TE1 TE2 Voltage:   %f   %f   %f\n",sensor_usr.sensor[0],sensor_usr.sensor[1],sensor_usr.sensor[2]);
	   HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    }
	
	
}
//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////����֤����OK
//д�������ּĴ���

void Modbus_08_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000&&(startRegAddr+RegNum)>=12&&(startRegAddr==0x0102)
			&&(RegNum==0x000c))//�Ĵ�����ַ+�����ڷ�Χ��
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        modbus_usr.RS485_TX_BUFF[2]=startRegAddr>>8;
        modbus_usr.RS485_TX_BUFF[3]=startRegAddr;			
        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];	
        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];			
        modbus_usr.RS485_TX_BUFF[6]=modbus_usr.RS485_RX_BUFF[6];	
        modbus_usr.RS485_TX_BUFF[7]=modbus_usr.RS485_RX_BUFF[7];	
        modbus_usr.RS485_TX_BUFF[8]=modbus_usr.RS485_RX_BUFF[8];	
        modbus_usr.RS485_TX_BUFF[9]=modbus_usr.RS485_RX_BUFF[9];	
        modbus_usr.RS485_TX_BUFF[10]=modbus_usr.RS485_RX_BUFF[10];	
        modbus_usr.RS485_TX_BUFF[11]=modbus_usr.RS485_RX_BUFF[11];	
        modbus_usr.RS485_TX_BUFF[12]=modbus_usr.RS485_RX_BUFF[12];	
        modbus_usr.RS485_TX_BUFF[13]=modbus_usr.RS485_RX_BUFF[13];	
        modbus_usr.RS485_TX_BUFF[14]=modbus_usr.RS485_RX_BUFF[14];	
        modbus_usr.RS485_TX_BUFF[15]=modbus_usr.RS485_RX_BUFF[15];
        modbus_usr.RS485_TX_BUFF[16]=modbus_usr.RS485_RX_BUFF[16];
        modbus_usr.RS485_TX_BUFF[17]=modbus_usr.RS485_RX_BUFF[17];
			
       calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,18);
			
        modbus_usr.RS485_TX_BUFF[18]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
        modbus_usr.RS485_TX_BUFF[19]=(calCRC)&0xFF;
         
		if(modbus_role == 0)
		{
		   modbus_role = 2;
		   RS485_SendData(modbus_usr.RS485_TX_BUFF,20,0);
		}
		else if(modbus_role == 1)
		{
			modbus_role = 2;
			//RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
		}

    }
//    else//�Ĵ�����ַ+����������Χ
//    {
//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
//        modbus_usr.RS485_TX_BUFF[2]=0x02; //�쳣��
//        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
//    }

}
void Modbus_09_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0104)&&(RegNum==0x0009))//�Ĵ�����ַ+�����ڷ�Χ��
    {
			modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
			modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
			modbus_usr.RS485_TX_BUFF[2]=startRegAddr>>8;
			modbus_usr.RS485_TX_BUFF[3]=startRegAddr;			
			modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];	
			modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];			
			modbus_usr.RS485_TX_BUFF[6]=modbus_usr.RS485_RX_BUFF[6];	
			modbus_usr.RS485_TX_BUFF[7]=modbus_usr.RS485_RX_BUFF[7];
			modbus_usr.RS485_TX_BUFF[8]=modbus_usr.RS485_RX_BUFF[8];
			modbus_usr.RS485_TX_BUFF[9]=modbus_usr.RS485_RX_BUFF[9];
			modbus_usr.RS485_TX_BUFF[10]=modbus_usr.RS485_RX_BUFF[10];
			modbus_usr.RS485_TX_BUFF[11]=modbus_usr.RS485_RX_BUFF[11];
			modbus_usr.RS485_TX_BUFF[12]=modbus_usr.RS485_RX_BUFF[12];
			modbus_usr.RS485_TX_BUFF[13]=modbus_usr.RS485_RX_BUFF[13];
			modbus_usr.RS485_TX_BUFF[14]=modbus_usr.RS485_RX_BUFF[14];
			
			//sensor_usr.CalibrationProbeVref =  DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[10]),4,1000);	//̽��ղ��ѹ�Ŵ�1000��	
			//sensor_usr.CalibrationT = modbus_usr.RS485_RX_BUFF[11];//�¶Ȳ���
      if(modbus_usr.RS485_RX_BUFF[6]!=0||modbus_usr.RS485_RX_BUFF[7]!=0||modbus_usr.RS485_RX_BUFF[8]!=0||modbus_usr.RS485_RX_BUFF[9]!=0)			
			{	
				//sensor_usr.CalibrationVref = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[6]),4,1000);//�ο���ѹ
			}
			else
			{
				
			//	params_save();			
			}
			
			calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,11);
			modbus_usr.RS485_TX_BUFF[15]=(calCRC>>8)&0xFF;         //CRC�ߵ�λ������  // �ȸߺ��
			modbus_usr.RS485_TX_BUFF[16]=(calCRC)&0xFF;
			if(modbus_role == 0)
			{
	            modbus_role = 2;
				//RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
			}
			else if(modbus_role == 1)
			{
	            modbus_role = 2;
				//RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
			}
			
    }


}

void Modbus_Pack(modbus_pack_stru p)
{        
        modbus_usr.RS485_TX_BUFF[0]=p.RS485_Addr;
        modbus_usr.RS485_TX_BUFF[1]=p.func;
        modbus_usr.RS485_TX_BUFF[2]=(u8)(p.startaddr>>8);
        modbus_usr.RS485_TX_BUFF[3]=(u8)(p.startaddr);	
        modbus_usr.RS485_TX_BUFF[4]=(u8)(p.regnum>>8);
        modbus_usr.RS485_TX_BUFF[5]=(u8)(p.regnum);	
		calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
		modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;		   //CRC�ߵ�λ������  // �ȸߺ��
		modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
		
		RS485_SendData(modbus_usr.RS485_TX_BUFF,8,0);
}
unsigned char *modbusRxResult()
{
	return modbus_usr.RS485_RX_BUFF;
}

