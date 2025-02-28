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
		   modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;		 //CRC高地位不对吗？  // 先高后低
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
        break;//无校验
    case 1:
        huart5.Init.Parity=UART_PARITY_ODD;
        break;//奇校验
    case 2:
        huart5.Init.Parity=UART_PARITY_EVEN;
        break;//偶校验
    }
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//默认为接收模式
    //HAL_UART_Receive_IT(&huart5, &res, 1);
		
	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	__HAL_UART_DISABLE_IT(&huart5, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart5);
	HAL_UART_DMAResume(&huart5);
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_Receive_DMA(&huart5,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
}


///////////////////////////////////////////////////////////////////////////////////////
//用定时器2判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
//定时器2中断服务程序
uint32_t Tick4ms;
void timCallback()
{

        //编写回调逻辑，即定时器1定时1MS后的逻辑
        //HAL_TIM_Base_Stop_IT(&htim2);  //停止定时器的时候调用这个函数关闭
        modbus_usr.RS485_TX_EN=1;//停止接收，切换为发送状态
		if(modbus_usr.RS485_FrameFlag==1)
		{
			if((HAL_GetTick()-Tick4ms)>=4)
			modbus_usr.RS485_FrameFlag=2;//置位帧结束标记

		}
		else if(modbus_usr.RS485_FrameFlag!=2)
		{
			Tick4ms =HAL_GetTick();
			modbus_usr.RS485_FrameFlag=3;//置位帧结束标记		
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
				485串口接收中断回调函数
*************************************************************/
void RS485_RxCpltCallback()
{
//    u8 err;
	  
//	//  if(huart->Instance == USART1)
//	{
//		if(HAL_UART_GetError(&huart5)) err=1;//检测到噪音、帧错误或校验错误
//		else err=0;
//		if((modbus_usr.RS485_RX_CNT<2047)&&(err==0))
//		{
//			modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT]=res;
//			modbus_usr.RS485_RX_CNT++;
//			modbus_usr.RS485_FrameFlag=1;//置位帧结束标记
//			Tick4ms =HAL_GetTick();			 //当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
//			//开始计时
//		}            
//		HAL_UART_Receive_IT(&huart5, &res, 1); 	  // 重新注册一次，要不然下次收不到了
	    
			modbus_usr.RS485_FrameFlag=2;//置位帧结束标记
			HAL_UART_DMAStop(&huart5);
			HAL_UART_DMAResume(&huart5);
			HAL_UART_Receive_DMA(&huart5,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
}

//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void RS485_SendData(u8 *buff,u8 len,unsigned char flag)
{
	
    if(flag == 0)
    HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_SET);
    modbus_usr.RS485_TX_EN=1;//切换为发送模式
    //  while(len--)
    {
        HAL_UART_Transmit(&huart5,buff,len,300);
			HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_RESET);
        //while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待发送区为空
        // USART_SendData(USART2,*(buff++));
    }
    //while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//等待发送完成
}

//unsigned char count;





/////////////////////////////////////////////////////////////////////////////////////
//RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)

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
        if(modbus_usr.RS485_RX_BUFF[0]<=(modbus_usr.RS485_Addr+5))//地址正确
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06)||(modbus_usr.RS485_RX_BUFF[1]==07)
							||(modbus_usr.RS485_RX_BUFF[1]==8)||(modbus_usr.RS485_RX_BUFF[1]==9))//功能码正确
            {
                startRegAddr=(((u16)modbus_usr.RS485_RX_BUFF[2])<<8)|modbus_usr.RS485_RX_BUFF[3];//获取寄存器起始地址
							  startRegAddr=0;//获取寄存器起始地址
							  modbus_usr.RS485_RX_CNT = modbus_usr.RS485_RX_BUFF[2]+5;
                if(startRegAddr<1000)//寄存器地址在范围内
                {
                    calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//计算所接收数据的CRC
                    recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2])<<8);//接收到的CRC(低字节在前，高字节在后)
                    if(calCRC==recCRC)//CRC校验正确
                    {
											TesetFlag = 0;
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        switch(modbus_usr.RS485_RX_BUFF[1])//根据不同的功能码进行处理
                        {
                        case 3: //读寄存器
                        {
                            Modbus_03_Solve();
                            break;
                        }

                        case 6: //写单个寄存器
                        {
                            Modbus_06_Solve();
                            break;
                        }
                        case 7: //测试模式，连续输出数据 0x01	0x07	0x0002	0x0002	0x940b
                        {
													  TesetFlag = 1;
                           // Modbus_07_Solve();
                            break;
                        }
                        case 8: //湿度校准系数
                        {
													 
                            Modbus_08_Solve();
                            break;
                        }
                        case 9: //参考电压、温度补偿、湿度电压补偿
                        {
	                            Modbus_09_Solve();
                            break;
                        }
                        default:TesetFlag = 0;
                            ;
                        }
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else//CRC校验错误
                    {
                        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                        modbus_usr.RS485_TX_BUFF[2]=0x04; //异常码
                       // RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                    }
                }
                else//寄存器地址超出范围
                {
                    modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                    modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                    modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
                    //RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                }
            }
            else//功能码错误
            {
                modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                modbus_usr.RS485_TX_BUFF[2]=0x01; //异常码
                //RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
            }
        }
       
       // modbus_usr.RS485_FrameFlag=0;//复位帧结束标志
       // modbus_usr.RS485_RX_CNT=0;//接收计数器清零
       // modbus_usr.RS485_TX_EN=0;//开启接收模式
    }
		if( modbus_usr.RS485_FrameFlag==3)
		{
        modbus_usr.RS485_FrameFlag=0;//复位帧结束标志
        modbus_usr.RS485_RX_CNT=0;//接收计数器清零
        modbus_usr.RS485_TX_EN=0;//开启接收模式
		}
		HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_RESET);
}

//Modbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//读保持寄存器

void Modbus_03_Solve(void)
{

    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    RegNum= modbus_usr.RS485_RX_BUFF[2];//获取寄存器数量
//    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x02||startRegAddr==0x03||startRegAddr==0x0100||startRegAddr==0x0101))//寄存器地址+数量在范围内
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
//									  //tmp = sensor_usr.rh;
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//                else if(i<2)
//                {
//									//  tmp = sensor_usr.temperature;
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == TE_ADDR)
//            {
//                if(i<1)
//                {
//                   // modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
//                   // modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == DEV_ADDR)
//            {
//                if(i<1)
//                {
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Addr>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(modbus_usr.RS485_Addr);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//            else if(startRegAddr == RATE_ADDR)
//            {
//                if(i<1)
//                {
//                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Baudrate>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
//                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)modbus_usr.RS485_Baudrate;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
//                }
//            }
//        }
//        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,RegNum*2+3);
//        modbus_usr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
//        modbus_usr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
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

    //}
}


//Modbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写单个保持寄存器

void Modbus_06_Solve(void)
{
    unsigned char i;
	   RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0100||startRegAddr==0x0101))//寄存器地址+数量在范围内
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

                    //modbus_usr.RS485_TX_BUFF[3+i*2]=(modbus_usr.RS485_Addr>>8)&&0xff;;//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
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
                    RS485_Init();															/////////先发送高字节--在发送低字节
                    //modbus_usr.RS485_TX_BUFF[4+i*2]=(sensor_usr.temperature)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
        }
          calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
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


//Modbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//读保持寄存器

void Modbus_07_Solve(void)
{


	
   RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//获取寄存器数量
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0103))//寄存器地址+数量在范围内
    {
	   HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_SET);
	  // printf("RH TE1 TE2 Voltage:   %f   %f   %f\n",sensor_usr.sensor[0],sensor_usr.sensor[1],sensor_usr.sensor[2]);
	   HAL_GPIO_WritePin(GPIOB, RS485_EN1_Pin, GPIO_PIN_RESET);
    }
	
	
}
//Modbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写单个保持寄存器

void Modbus_08_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//获取寄存器数量
    if((startRegAddr+RegNum)<1000&&(startRegAddr+RegNum)>=12&&(startRegAddr==0x0102)
			&&(RegNum==0x000c))//寄存器地址+数量在范围内
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
			
        modbus_usr.RS485_TX_BUFF[18]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
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
//    else//寄存器地址+数量超出范围
//    {
//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
//        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
//        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
//    }

}
void Modbus_09_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//获取寄存器数量
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0104)&&(RegNum==0x0009))//寄存器地址+数量在范围内
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
			
			//sensor_usr.CalibrationProbeVref =  DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[10]),4,1000);	//探针空测电压放大1000倍	
			//sensor_usr.CalibrationT = modbus_usr.RS485_RX_BUFF[11];//温度补偿
      if(modbus_usr.RS485_RX_BUFF[6]!=0||modbus_usr.RS485_RX_BUFF[7]!=0||modbus_usr.RS485_RX_BUFF[8]!=0||modbus_usr.RS485_RX_BUFF[9]!=0)			
			{	
				//sensor_usr.CalibrationVref = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[6]),4,1000);//参考电压
			}
			else
			{
				
			//	params_save();			
			}
			
			calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,11);
			modbus_usr.RS485_TX_BUFF[15]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
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
		modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;		   //CRC高地位不对吗？  // 先高后低
		modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
		
		RS485_SendData(modbus_usr.RS485_TX_BUFF,8,0);
}
modbus_stru *modbusRxResult()
{
	return &modbus_usr;
}

