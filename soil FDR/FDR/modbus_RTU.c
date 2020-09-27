#include "modbus_RTU.h"
#include "led.h"
#include "lcd.h"
#include "stm32f10x_tim.h"


///////////////////////////////////////////////////////////
unsigned char RS485_TX_EN;
/*u32 RS485_Baudrate=9600;//通讯波特率
u8 RS485_Parity=0;//0无校验；1奇校验；2偶校验
u8 RS485_Addr=1;//从机地址
u16 RS485_Frame_Distance=4;//数据帧最小间隔（ms),超过此时间则认为是下一帧

u8 RS485_RX_BUFF[2048];//接收缓冲区2048字节
u16 RS485_RX_CNT=0;//接收计数器
u8 RS485_FrameFlag=0;//帧结束标记
u8 RS485_TX_BUFF[2048];//发送缓冲区
u16 RS485_TX_CNT=0;//发送计数器*/
modbus_stru modbusUsr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus寄存器和单片机寄存器的映射关系
vu32 *Modbus_InputIO[100];//输入开关量寄存器指针(这里使用的是位带操作)
vu32 *Modbus_OutputIO[100];//输出开关量寄存器指针(这里使用的是位带操作)
u16 *Modbus_HoldReg[1000];//保持寄存器指针
u32 testData1=1201,testData2=1002,testData3=2303,testData4=8204;
void Modbus_RegMap(void)
{


         //输入开关量寄存器指针指向
        Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)：取PE4的地址，(vu32*)&PEin(4)将PE4地址强制转换为uw32类型的地址，Modbus_InputIO[0]=(vu32*)&PEin(4); 将转换好的地址送给地址指针Modbus_InputIO[0]；
        Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*Modbus_InputIO[0] 取出地址中的内容。
        Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
        Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3

        //输出开关量寄存器指针指向
        Modbus_OutputIO[0]=(vu32*)&PBout(5);//LED0
        Modbus_OutputIO[1]=(vu32*)&PEout(5);//LED1

        //保持寄存器指针指向
        Modbus_HoldReg[0]=(u16*)&testData1;//测试数据1
        Modbus_HoldReg[1]=(u16*)&testData2;//((u16*)&testData1)+1;//测试数据1
        Modbus_HoldReg[2]=(u16*)&testData3;//(u16*)&testData2;//测试数据2
        Modbus_HoldReg[3]=(u16*)&testData4;//((u16*)&testData2)+1;//测试数据2
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

//初始化USART2
void RS485_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//PA2（TX）复用推挽输出
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        GPIO_SetBits(GPIOA,GPIO_Pin_2);//默认高电平

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//PA3（RX）输入上拉
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //修改原GPIO_Mode_IPU（输入上拉）->GPIO_Mode_IN_FLOATING(浮空输入)/////////////////////////////////////////////
        GPIO_Init(GPIOA,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//修改PG9（RE/DE）通用推挽输出->PD7（RE/DE）通用推挽输出//////////////////////////////////////////////////////////////////////
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_Init(GPIOG,&GPIO_InitStructure);
        GPIO_ResetBits(GPIOG,GPIO_Pin_9);//默认接收状态

        USART_DeInit(USART2);//复位串口2
        USART_InitStructure.USART_BaudRate=RS485_Baudrate;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
        switch(RS485_Parity)
        {
                case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//无校验
                case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//奇校验
                case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//偶校验
        }
        USART_Init(USART2,&USART_InitStructure);

        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能串口2接收中断

        NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        USART_Cmd(USART2,ENABLE);//使能串口2
        RS485_TX_EN=0;//默认为接收模式

        Timer7_Init();//定时器7初始化，用于监视空闲时间
        Modbus_RegMap();//Modbus寄存器映射
}

//定时器7初始化
void Timer7_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7时钟使能

        //TIM7初始化设置
        TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance*10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
        TIM_TimeBaseStructure.TIM_Prescaler =7200; //设置用来作为TIMx时钟频率除数的预分频值 设置计数频率为10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

        TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );//TIM7 允许更新中断

        //TIM7中断分组配置
        NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;  //TIM7中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}



//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void RS485_SendData(u8 *buff,u8 len)
{
        RS485_TX_EN=1;//切换为发送模式
        while(len--)
        {
                while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待发送区为空
                USART_SendData(USART2,*(buff++));
        }
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//等待发送完成
}


/////////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)//串口2中断服务程序
{

        u8 res;
        u8 err;

        if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//检测到噪音、帧错误或校验错误
                else err=0;
                LED0=0;
                res=USART_ReceiveData(USART2); //读接收到的字节，同时相关标志自动清除

                if((RS485_RX_CNT<2047)&&(err==0))
                {
                        modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT]=res;
                        modbusUsr.RS485_RX_CNT++;

                        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除定时器溢出中断
                        TIM_SetCounter(TIM7,0);//当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
                        TIM_Cmd(TIM7,ENABLE);//开始计时
                }
        }
}

///////////////////////////////////////////////////////////////////////////////////////
//用定时器7判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
//定时器7中断服务程序
void TIM7_IRQHandler(void)
{
        if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
        {
                TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除中断标志
                TIM_Cmd(TIM7,DISABLE);//停止定时器
                modbusUsr.RS485_TX_EN=1;//停止接收，切换为发送状态
                modbusUsr.RS485_FrameFlag=1;//置位帧结束标记
        }
}

/////////////////////////////////////////////////////////////////////////////////////
//RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)
u16 startRegAddr;
u16 RegNum;
u16 calCRC;
void RS485_Service(void)
{
        u16 recCRC;
        if(modbusUsr.RS485_FrameFlag==1)
        {
                if(modbusUsr.RS485_RX_BUFF[0]==modbusUsr.RS485_Addr)//地址正确
                {
                        if((modbusUsr.RS485_RX_BUFF[1]==01)||(modbusUsr.RS485_RX_BUFF[1]==02)||(modbusUsr.RS485_RX_BUFF[1]==03)||(modbusUsr.RS485_RX_BUFF[1]==05)||(modbusUsr.RS485_RX_BUFF[1]==06)||(modbusUsr.RS485_RX_BUFF[1]==15)||(modbusUsr.RS485_RX_BUFF[1]==16))//功能码正确
                  {
                                startRegAddr=(((u16)modbusUsr.RS485_RX_BUFF[2])<<8)|modbusUsr.RS485_RX_BUFF[3];//获取寄存器起始地址
                                if(startRegAddr<1000)//寄存器地址在范围内
                                {
                                        calCRC=CRC_Compute(modbusUsr.RS485_RX_BUFF,modbusUsr.RS485_RX_CNT-2);//计算所接收数据的CRC
                                        recCRC=modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT-1]|(((u16)modbusUsr.RS485_RX_BUFF[modbusUsr.RS485_RX_CNT-2])<<8);//接收到的CRC(低字节在前，高字节在后)
                                        if(calCRC==recCRC)//CRC校验正确
                                        {
                                                ///////////显示用

        LCD_ShowxNum(10,230,modbusUsr.RS485_RX_BUFF[0],3,16,0X80);//显示数据
         LCD_ShowxNum(42,230,modbusUsr.RS485_RX_BUFF[1],3,16,0X80);//显示数据
        LCD_ShowxNum(74,230,modbusUsr.RS485_RX_BUFF[2],3,16,0X80);//显示数据
        LCD_ShowxNum(106,230,modbusUsr.RS485_RX_BUFF[3],3,16,0X80);//显示数据
        LCD_ShowxNum(138,230,modbusUsr.RS485_RX_BUFF[4],3,16,0X80);//显示数据
        LCD_ShowxNum(170,230,modbusUsr.RS485_RX_BUFF[5],3,16,0X80);//显示数据
///////////////////////
                                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                switch(modbusUsr.RS485_RX_BUFF[1])//根据不同的功能码进行处理
                                                {
                                                        case 2://读输入开关量
                                                        {
                                                                Modbus_02_Solve();
                                                                break;
                                                        }

                                                        case 1://读输出开关量
                                                        {
                                                                Modbus_01_Solve();
                                                                break;
                                                        }

                                                        case 5://写单个输出开关量
                                                        {
                                                                Modbus_05_Solve();
                                                                break;
                                                        }

                                                        case 15://写多个输出开关量
                                                        {
                                                                Modbus_15_Solve();
                                                                break;
                                                        }

                                                        case 03: //读多个寄存器
                                                        {
                                                                Modbus_03_Solve();
                                                                break;
                                                        }

                                                        case 06: //写单个寄存器
                                                        {
                                                                Modbus_06_Solve();
                                                                break;
                                                        }

                                                        case 16: //写多个寄存器
                                                        {
                                                                Modbus_16_Solve();
                                                                break;
                                                        }


                                                }
                                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                        }
                                        else//CRC校验错误
                                        {

                                                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                                modbusUsr.RS485_TX_BUFF[2]=0x04; //异常码
                                                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                                        }
                                }
                                else//寄存器地址超出范围
                                {
                                        modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                        modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                        modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                                        RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                                }
                        }
                        else//功能码错误
                        {
                                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                                RS485_TX_BUFF[2]=0x01; //异常码
                                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
                        }
          }

                modbusUsr.RS485_FrameFlag=0;//复位帧结束标志
                modbusUsr.RS485_RX_CNT=0;//接收计数器清零
                RS485_TX_EN=0;//开启接收模式
        }
}

//Modbus功能码02处理程序/////////////////////////////////////////////////////程序已验证OK -----必须先配置PE4 PE3 PE2 PA0 初始化按键才可以OK    KEY_Init();
//读输入开关量
void Modbus_02_Solve(void)
{
        u16 ByteNum;
        u16 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//获取寄存器数量
        if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                ByteNum=RegNum/8;//字节数
                if(RegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
                modbusUsr.RS485_TX_BUFF[2]=ByteNum;//返回要读取的字节数
                for(i=0;i<RegNum;i++)
                {
                        if(i%8==0) modbusUsr.RS485_TX_BUFF[3+i/8]=0x00;
                        modbusUsr.RS485_TX_BUFF[3+i/8]>>=1;//低位先发送
                        modbusUsr.RS485_TX_BUFF[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
                        if(i==RegNum-1)//发送到最后一个位了
                        {
                                if(RegNum%8) modbusUsr.RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
                        }
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,ByteNum+3);
                modbusUsr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,ByteNum+5);
        }
        else//寄存器地址+数量超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus功能码01处理程序 ///////////////////////////////////////////////////////////程序已验证OK
//读输出开关量
void Modbus_01_Solve(void)
{
        u16 ByteNum;
        u16 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//获取寄存器数量
        if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                ByteNum=RegNum/8;//字节数
                if(RegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
                modbusUsr.RS485_TX_BUFF[2]=ByteNum;//返回要读取的字节数
                for(i=0;i<RegNum;i++)
                {
                        if(i%8==0) modbusUsr.RS485_TX_BUFF[3+i/8]=0x00;
                        modbusUsr.RS485_TX_BUFF[3+i/8]>>=1;//低位先发送
                        modbusUsr.RS485_TX_BUFF[3+i/8]|=((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
                        if(i==RegNum-1)//发送到最后一个位了
                        {
                                if(RegNum%8) RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
                        }
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,ByteNum+3);
                modbusUsr.RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
                modbusUsr.RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,ByteNum+5);
        }
        else//寄存器地址+数量超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus功能码05处理程序   ///////////////////////////////////////////////////////程序已验证OK
//写单个输出开关量
void Modbus_05_Solve(void)
{
        if(startRegAddr<100)//寄存器地址在范围内
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
        else//寄存器地址超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus功能码15处理程序   //////////////////////////////////////////////////////程序已验证OK
//写多个输出开关量
void Modbus_15_Solve(void)
{
        u16 i;
        RegNum=(((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|modbusUsr.RS485_RX_BUFF[5];//获取寄存器数量
        if((startRegAddr+RegNum)<100)//寄存器地址+数量在范围内
        {
                for(i=0;i<RegNum;i++)
                {
                        if(modbusUsr.RS485_RX_BUFF[7+i/8]&0x01) *Modbus_OutputIO[startRegAddr+i]=0x01;
                        else *Modbus_OutputIO[startRegAddr+i]=0x00;
                        modbusUsr.RS485_RX_BUFF[7+i/8]>>=1;//从低位开始
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
        else//寄存器地址+数量超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}

//Modbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//读保持寄存器
void Modbus_03_Solve(void)
{
        u8 i;
        RegNum= (((u16)modbusUsr.RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//获取寄存器数量
        if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1];
                modbusUsr.RS485_TX_BUFF[2]=RegNum*2;
                for(i=0;i<RegNum;i++)
                {
                        modbusUsr.RS485_TX_BUFF[3+i*2]=(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////先发送高字节--在发送低字节
                        modbusUsr.RS485_TX_BUFF[4+i*2]=(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                calCRC=CRC_Compute(modbusUsr.RS485_TX_BUFF,RegNum*2+3);
                modbusUsr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC高地位不对吗？  // 先高后低
                modbusUsr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
                RS485_SendData(modbusUsr.RS485_TX_BUFF,RegNum*2+5);
        }
        else//寄存器地址+数量超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}


//Modbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写单个保持寄存器
void Modbus_06_Solve(void)
{
        *Modbus_HoldReg[startRegAddr]=modbusUsr.RS485_RX_BUFF[4]<<8;//高字节在前                    ////////修改为高字节在前，低字节在后
        *Modbus_HoldReg[startRegAddr]|=((u16)modbusUsr.RS485_RX_BUFF[5]);//低字节在后

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

//Modbus功能码16处理程序 /////////////////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写多个保持寄存器
void Modbus_16_Solve(void)
{
        u8 i;
        RegNum= (((u16)RS485_RX_BUFF[4])<<8)|((RS485_RX_BUFF[5]));//获取寄存器数量
        if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
        {
                for(i=0;i<RegNum;i++)
                {
                        *Modbus_HoldReg[startRegAddr+i]=modbusUsr.RS485_RX_BUFF[7+i*2]; //低字节在前                 /////// 低字节在前，高字节在后正常
                        *Modbus_HoldReg[startRegAddr+i]|=((u16)modbusUsr.RS485_RX_BUFF[8+i*2])<<8; //高字节在后
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
        else//寄存器地址+数量超出范围
        {
                modbusUsr.RS485_TX_BUFF[0]=modbusUsr.RS485_RX_BUFF[0];
                modbusUsr.RS485_TX_BUFF[1]=modbusUsr.RS485_RX_BUFF[1]|0x80;
                modbusUsr.RS485_TX_BUFF[2]=0x02; //异常码
                RS485_SendData(modbusUsr.RS485_TX_BUFF,3);
        }
}



