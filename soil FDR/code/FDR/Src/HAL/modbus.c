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
void Modbus_15_Solve(void);
void Modbus_16_Solve(void);

u16 startRegAddr;
u16 RegNum;
u16 calCRC;

extern sensor_stru sensor_usr;

///////////////////////////////////////////////////////////
//u32 RS485_Baudrate=9600;//通讯波特率
//u8 modbus_usr.RS485_Parity=0;//0无校验；1奇校验；2偶校验
//u8 modbus_usr.RS485_Addr=1;//从机地址
//u16 modbus_usr.RS485_Frame_Distance=4;//数据帧最小间隔（ms),超过此时间则认为是下一帧

//u8 modbus_usr.RS485_RX_BUFF[2048];//接收缓冲区2048字节
//u16 modbus_usr.RS485_RX_CNT=0;//接收计数器
//u8 modbus_usr.RS485_FrameFlag=0;//帧结束标记
//u8 modbus_usr.RS485_TX_BUFF[2048];//发送缓冲区
//u16 modbus_usr.RS485_TX_CNT=0;//发送计数器
modbus_stru modbus_usr;
extern TIM_HandleTypeDef htim2;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus寄存器和单片机寄存器的映射关系
//vu32 *Modbus_InputIO[100];//输入开关量寄存器指针(这里使用的是位带操作)
//vu32 *Modbus_OutputIO[100];//输出开关量寄存器指针(这里使用的是位带操作)
//u16 *Modbus_HoldReg[1000];//保持寄存器指针
//u32 testData1=1201,testData2=1002,testData3=2303,testData4=8204;
//void Modbus_RegMap(void)
//{


//         //输入开关量寄存器指针指向
//        Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)：取PE4的地址，(vu32*)&PEin(4)将PE4地址强制转换为uw32类型的地址，Modbus_InputIO[0]=(vu32*)&PEin(4); 将转换好的地址送给地址指针Modbus_InputIO[0]；
//        Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*Modbus_InputIO[0] 取出地址中的内容。
//        Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
//        Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3

//        //输出开关量寄存器指针指向
//        Modbus_OutputIO[0]=(vu32*)&PBout(5);//LED0
//        Modbus_OutputIO[1]=(vu32*)&PEout(5);//LED1

//        //保持寄存器指针指向
//        Modbus_HoldReg[0]=(u16*)&testData1;//测试数据1
//        Modbus_HoldReg[1]=(u16*)&testData2;//((u16*)&testData1)+1;//测试数据1
//        Modbus_HoldReg[2]=(u16*)&testData3;//(u16*)&testData2;//测试数据2
//        Modbus_HoldReg[3]=(u16*)&testData4;//((u16*)&testData2)+1;//测试数据2
//        Modbus_HoldReg[4]=(u16*)&testData1;
//        Modbus_HoldReg[5]=(u16*)&testData2;
//        Modbus_HoldReg[6]=(u16*)&testData3;
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef huart1;
//初始化USART2

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
        break;//无校验
    case 1:
        huart1.Init.Parity=UART_PARITY_ODD;
        break;//奇校验
    case 2:
        huart1.Init.Parity=UART_PARITY_EVEN;
        break;//偶校验
    }
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//默认为接收模式
    HAL_UART_Receive_IT(&huart1, &res, 1);
    modbus_usr.RS485_Baudrate = 9600;


    // Timer7_Init();//定时器7初始化，用于监视空闲时间
    // Modbus_RegMap();//Modbus寄存器映射
}

//定时器7初始化




//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void RS485_SendData(u8 *buff,u8 len)
{
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
    modbus_usr.RS485_TX_EN=1;//切换为发送模式
    //  while(len--)
    {
        HAL_UART_Transmit(&huart1,buff,len,100);
        //while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待发送区为空
        // USART_SendData(USART2,*(buff++));
    }
    //while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//等待发送完成
}

unsigned char count;
uint32_t Tick4ms;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 err;
    if(huart->Instance == USART1)
    {
			 count++;
        if(HAL_UART_GetError(&huart1)) err=1;//检测到噪音、帧错误或校验错误
        else err=0;
        if((modbus_usr.RS485_RX_CNT<2047)&&(err==0))
        {
            modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT]=res;
            modbus_usr.RS485_RX_CNT++;
						modbus_usr.RS485_FrameFlag=1;//置位帧结束标记
            Tick4ms =HAL_GetTick();			 //当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
						//开始计时
        }            
        HAL_UART_Receive_IT(&huart1, &res, 1); 	  // 重新注册一次，要不然下次收不到了
//        //led_ctrl(2);
    }


}


///////////////////////////////////////////////////////////////////////////////////////
//用定时器2判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
//定时器2中断服务程序

void timCallback()
{

        //编写回调逻辑，即定时器1定时1MS后的逻辑
        HAL_TIM_Base_Stop_IT(&htim2);  //停止定时器的时候调用这个函数关闭
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
/////////////////////////////////////////////////////////////////////////////////////
//RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)

void RS485_Service(void)
{
    u16 recCRC;
	 //Get_Adc_Average(N);
    if(modbus_usr.RS485_FrameFlag==2)
    {
        if(modbus_usr.RS485_RX_BUFF[0]==modbus_usr.RS485_Addr)//地址正确
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06))//功能码正确
            {
                startRegAddr=(((u16)modbus_usr.RS485_RX_BUFF[2])<<8)|modbus_usr.RS485_RX_BUFF[3];//获取寄存器起始地址
                if(startRegAddr<1000)//寄存器地址在范围内
                {
                    calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//计算所接收数据的CRC
                    recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2])<<8);//接收到的CRC(低字节在前，高字节在后)
                    if(calCRC==recCRC)//CRC校验正确
                    {
                        ///////////显示用

                        //LCD_ShowxNum(10,230,modbus_usr.RS485_RX_BUFF[0],3,16,0X80);//显示数据
                        //  LCD_ShowxNum(42,230,modbus_usr.RS485_RX_BUFF[1],3,16,0X80);//显示数据
                        // LCD_ShowxNum(74,230,modbus_usr.RS485_RX_BUFF[2],3,16,0X80);//显示数据
                        // LCD_ShowxNum(106,230,modbus_usr.RS485_RX_BUFF[3],3,16,0X80);//显示数据
                        // LCD_ShowxNum(138,230,modbus_usr.RS485_RX_BUFF[4],3,16,0X80);//显示数据
                        // LCD_ShowxNum(170,230,modbus_usr.RS485_RX_BUFF[5],3,16,0X80);//显示数据
///////////////////////
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        switch(modbus_usr.RS485_RX_BUFF[1])//根据不同的功能码进行处理
                        {
                        case 03: //读多个寄存器
                        {
													  Get_Adc_Average(N);
													
                            Modbus_03_Solve();
                            break;
                        }

                        case 06: //写单个寄存器
                        {
                            Modbus_06_Solve();
                            break;
                        }
                        default:
                            ;


                        }
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else//CRC校验错误
                    {

                        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                        modbus_usr.RS485_TX_BUFF[2]=0x04; //异常码
                        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                    }
                }
                else//寄存器地址超出范围
                {
                    modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                    modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                    modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
                    RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
                }
            }
            else//功能码错误
            {
                modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                modbus_usr.RS485_TX_BUFF[2]=0x01; //异常码
                RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
            }
        }
       
        modbus_usr.RS485_FrameFlag=0;//复位帧结束标志
        modbus_usr.RS485_RX_CNT=0;//接收计数器清零
        modbus_usr.RS485_TX_EN=0;//开启接收模式
    }
		if( modbus_usr.RS485_FrameFlag==3)
		{
        modbus_usr.RS485_FrameFlag=0;//复位帧结束标志
        modbus_usr.RS485_RX_CNT=0;//接收计数器清零
        modbus_usr.RS485_TX_EN=0;//开启接收模式
		}
		HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
}

//Modbus功能码02处理程序/////////////////////////////////////////////////////程序已验证OK -----必须先配置PE4 PE3 PE2 PA0 初始化按键才可以OK    KEY_Init();
//读输入开关量
void Modbus_02_Solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        ByteNum=RegNum/8;//字节数
        if(RegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
        modbus_usr.RS485_TX_BUFF[2]=ByteNum;//返回要读取的字节数
        for(i=0; i<RegNum; i++)
        {
            if(i%8==0) modbus_usr.RS485_TX_BUFF[3+i/8]=0x00;
            modbus_usr.RS485_TX_BUFF[3+i/8]>>=1;//低位先发送
            //  modbus_usr.RS485_TX_BUFF[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
            if(i==RegNum-1)//发送到最后一个位了
            {
                if(RegNum%8) modbus_usr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,ByteNum+3);
        modbus_usr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,ByteNum+5);
    }
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus功能码01处理程序 ///////////////////////////////////////////////////////////程序已验证OK
//读输出开关量
void Modbus_01_Solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
        ByteNum=RegNum/8;//字节数
        if(RegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
        modbus_usr.RS485_TX_BUFF[2]=ByteNum;//返回要读取的字节数
        for(i=0; i<RegNum; i++)
        {
            if(i%8==0) modbus_usr.RS485_TX_BUFF[3+i/8]=0x00;
            // modbus_usr.RS485_TX_BUFF[3+i/8]>>=1;//低位先发送
            // modbus_usr.RS485_TX_BUFF[3+i/8]|=((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
            if(i==RegNum-1)//发送到最后一个位了
            {
                if(RegNum%8) modbus_usr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,ByteNum+3);
        modbus_usr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
        modbus_usr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,ByteNum+5);
    }
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus功能码05处理程序   ///////////////////////////////////////////////////////程序已验证OK
//写单个输出开关量
void Modbus_05_Solve(void)
{
    if(startRegAddr<100)//寄存器地址在范围内
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
    else//寄存器地址超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus功能码15处理程序   //////////////////////////////////////////////////////程序已验证OK
//写多个输出开关量
void Modbus_15_Solve(void)
{
    u16 i;
    RegNum=(((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
    {
        for(i=0; i<RegNum; i++)
        {
            if(modbus_usr.RS485_RX_BUFF[7+i/8]&0x01)
                ;//*Modbus_OutputIO[startRegAddr+i]=0x01;
            else
                ;//*Modbus_OutputIO[startRegAddr+i]=0x00;
            modbus_usr.RS485_RX_BUFF[7+i/8]>>=1;//从低位开始
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
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}

//Modbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//读保持寄存器

void Modbus_03_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//获取寄存器数量
    if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
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
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                else if(i<2)
                {
									  tmp = sensor_usr.temperature;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == TE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == DEV_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Addr>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(modbus_usr.RS485_Addr)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == RATE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Baudrate>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)modbus_usr.RS485_Baudrate&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,RegNum*2+3);
        modbus_usr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
        modbus_usr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,RegNum*2+5);
    }
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}


//Modbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写单个保持寄存器

void Modbus_06_Solve(void)
{
    ;//*Modbus_HoldReg[startRegAddr]=modbus_usr.RS485_RX_BUFF[4]<<8;//高字节在前                    ////////修改为高字节在前，低字节在后
    ;//*Modbus_HoldReg[startRegAddr]|=((u16)modbus_usr.RS485_RX_BUFF[5]);//低字节在后

//        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
//        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1];
//        modbus_usr.RS485_TX_BUFF[2]=modbus_usr.RS485_RX_BUFF[2];
//        modbus_usr.RS485_TX_BUFF[3]=modbus_usr.RS485_RX_BUFF[3];
//        modbus_usr.RS485_TX_BUFF[4]=modbus_usr.RS485_RX_BUFF[4];
//        modbus_usr.RS485_TX_BUFF[5]=modbus_usr.RS485_RX_BUFF[5];
    unsigned char i;
    if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
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
            flash_write(0,0x5a,1);
            flash_write(1,modbus_usr.RS485_Addr,1);
            flash_write(2,modbus_usr.RS485_Baudrate,1);
            flash_write(3,modbus_usr.RS485_Parity,1);
        }
          calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
         RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }



    //calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
    // modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    // modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
    // RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
}

//Modbus功能码16处理程序 /////////////////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写多个保持寄存器
void Modbus_16_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//获取寄存器数量
    if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
    {
        for(i=0; i<RegNum; i++)
        {
            // *Modbus_HoldReg[startRegAddr+i]=modbus_usr.RS485_RX_BUFF[7+i*2]; //低字节在前                 /////// 低字节在前，高字节在后正常
            // *Modbus_HoldReg[startRegAddr+i]|=((u16)modbus_usr.RS485_RX_BUFF[8+i*2])<<8; //高字节在后
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
    else//寄存器地址+数量超出范围
    {
        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
        modbus_usr.RS485_TX_BUFF[2]=0x02; //异常码
        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
    }
}
