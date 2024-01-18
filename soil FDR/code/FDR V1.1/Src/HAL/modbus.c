#include "modbus.h"
#include "main.h"
#include "crc.h"
#include "bsp.h"
#include "adc.h"
#include "eeprom.h"
#include "filter.h"
#include "app.h"
#include "FdrAlgorithm.h"
#include "calibration.h"
void Modbus_01_Solve(void);
void Modbus_02_Solve(void);
void Modbus_03_Solve(void);
void Modbus_05_Solve(void);
void Modbus_06_Solve(void);
void Modbus_07_Solve(void);
void Modbus_08_Solve(void);
void Modbus_09_Solve(void);
void Modbus_10_Solve(void);
void Modbus_11_Solve(void);
void Modbus_16_Solve(void);
void Modbus_12_Solve(void);



extern factor_stru factor_usr;
u16 startRegAddr;//~{<D4fFw5XV7~}
u16 RegNum;//~{<D4fFwJ}A?~}
u16 calCRC;//crc
unsigned char TesetFlag;
extern sensor_stru sensor_usr;
modbus_stru modbus_usr;
extern TIM_HandleTypeDef htim2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef huart1;
//~{3uJ<;/~}USART2

unsigned char res;
/********************************************
					485~{4.?Z3uJ<;/2YWw~}
********************************************/
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
        break;//~{N^P#Qi~}
    case 1:
        huart1.Init.Parity=UART_PARITY_ODD;
        break;//~{FfP#Qi~}
    case 2:
        huart1.Init.Parity=UART_PARITY_EVEN;
        break;//~{E<P#Qi~}
    }
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//~{D,HON*=SJUD#J=~}
    HAL_UART_Receive_IT(&huart1, &res, 1);
}


//////////////////////////////////////////////////////////////////////////////
//buff:~{7"KMGxJW5XV7~}
//len~{#:7"KM5DWV=ZJ}~}
void RS485_SendData(u8 *buff,u8 len)
{
    HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);//~{GP;;N*7"KMD#J=~}
    modbus_usr.RS485_TX_EN=1;
    {
        HAL_UART_Transmit(&huart1,buff,len,1000);
    }
	#if ROLE == 0
	HAL_Delay(50);
	#else
	HAL_Delay(3000);
	#endif
	 HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
	 modbus_usr.RS485_TX_EN=0;
	MX_USART1_UART_Init();
  RS485_Init();
}

unsigned char count;
uint32_t Tick4ms;
/*************************************************************
				485~{4.?Z=SJUVP6O;X5w:/J}~}
*************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 err;
    if(huart->Instance == USART1)
    {
        count++;
        if(HAL_UART_GetError(&huart1)) err=1;//~{<l2b5=TkRt!"V!4mNs;rP#Qi4mNs~}
        else err=0;
        if((modbus_usr.RS485_RX_CNT<2047)&&(err==0))
        {
            modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT]=res;
            modbus_usr.RS485_RX_CNT++;
            modbus_usr.RS485_FrameFlag=1;//~{VCN;V!=aJx1j<G~}
            Tick4ms =HAL_GetTick();			 //~{51=SJU5=R;8vPB5DWV=Z#,=+6(J1Fw~}7~{84N;N*~}0~{#,VXPB<FJ1#(O`51SZN997#)~}
            //~{?*J<<FJ1~}
        }
        HAL_UART_Receive_IT(&huart1, &res, 1); 	  // ~{VXPBW"2aR;4N#,R*2;H;OB4NJU2;5=AK~}
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//~{SC6(J1FwEP6O=SJU?UOPJ1<d#,51?UOPJ1<d4sSZV86(J1<d#,HON*R;V!=aJx~}
//~{6(J1FwVP6O7~Nq3LPr~}
void timCallback()
{

    //~{1`P4;X5wB_<-#,<46(J1Fw~}1~{6(J1~}1MS~{:s5DB_<-~}
    HAL_TIM_Base_Stop_IT(&htim2);  //~{M#V96(J1Fw5DJ1:r5wSCUb8v:/J}9X1U~}
    modbus_usr.RS485_TX_EN=1;//~{M#V9=SJU#,GP;;N*7"KMW4L,~}
    if(modbus_usr.RS485_FrameFlag==1)
    {
        if((HAL_GetTick()-Tick4ms)>=10)
            modbus_usr.RS485_FrameFlag=2;//~{VCN;V!=aJx1j<G~}

    }
    else if(modbus_usr.RS485_FrameFlag!=2)
    {
        Tick4ms =HAL_GetTick();
        modbus_usr.RS485_FrameFlag=3;//~{VCN;V!=aJx1j<G~}
    }


}

/////////////////////////////////////////////////////////////////////////////////////
//RS485~{7~Nq3LPr#,SCSZ4&@m=SJU5=5DJ}>]~}(~{GkTZVw:/J}VPQ-;75wSC~})

void RS485_Service(void)
{
    static uint32_t tickTime_adc;
    static unsigned char tick_1s;
    u16 recCRC;
    if((HAL_GetTick()-tickTime_adc)>=SAMPLE_FRIOD)//~{V\FZPT2I</D#Db5gQ9#,~}50ms~{R;4N~}
    {
        if(getRatio()->calibrationFlag == 0 )
            Get_Adc_Average(N);
        tickTime_adc=HAL_GetTick();
        tick_1s++;
#if DEBUG == 1
        if(TesetFlag==1&&tick_1s>=20)
        {
            tick_1s = 0;
            Modbus_07_Solve();
        }
#endif
    }
    else
        tickTime_adc=tickTime_adc;
    if(modbus_usr.RS485_FrameFlag==2)//~{JU5=R;MjU{J}>]0|~}
    {
        if(modbus_usr.RS485_RX_BUFF[0]==modbus_usr.RS485_Addr||modbus_usr.RS485_RX_BUFF[0]==CALB_ADDR)//~{5XV7U}H7~}
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06)||(modbus_usr.RS485_RX_BUFF[1]==07)
                    ||(modbus_usr.RS485_RX_BUFF[1]==8)||(modbus_usr.RS485_RX_BUFF[1]==9)||(modbus_usr.RS485_RX_BUFF[1]==10))//~{9&D\BkU}H7~}
            {
                startRegAddr=(((u16)modbus_usr.RS485_RX_BUFF[2])<<8)|modbus_usr.RS485_RX_BUFF[3];//~{;qH!<D4fFwFpJ<5XV7~}
                if(modbus_usr.RS485_RX_BUFF[2] == 0x04)
                {
                    if(modbus_usr.RS485_RX_BUFF[3] == 0x01)
                    startRegAddr = 0x04;
				if(modbus_usr.RS485_RX_BUFF[3] == 0x02)
				startRegAddr = 0x05;

                }
                if(startRegAddr<1000)//~{<D4fFw5XV7TZ76N'DZ~}
                {
                    calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//~{<FKcKy=SJUJ}>]5D~}CRC
									  if(modbus_usr.RS485_RX_CNT>=2)
                    recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2])<<8);//~{=SJU5=5D~}CRC(~{5MWV=ZTZG0#,8_WV=ZTZ:s~})
                    if(calCRC==recCRC)//CRC~{P#QiU}H7~}
                    {
                       #if ROLE == 0

                        if(modbus_usr.RS485_RX_BUFF[3] == 0x01)
                        {
                            modbus_usr.RS485_RX_BUFF[1] = 11;
                        }
						#endif
						#if ROLE == 1
                        if(modbus_usr.RS485_RX_BUFF[3] == 0x02)
                        {
                            modbus_usr.RS485_RX_BUFF[1] = 12;
                        }
						#endif 
                        #if ROLE == 1
                        if(modbus_usr.RS485_RX_BUFF[1]!=10&&modbus_usr.RS485_RX_BUFF[1]!=11)
                            getRatio()->calibrationFlag = 0;//~{P#W<1jV>~}
                        #endif
                        TesetFlag = 0;
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        switch(modbus_usr.RS485_RX_BUFF[1])//~{8y>]2;M,5D9&D\Bk=xPP4&@m~}
                        {
                        case 3: //~{6A<D4fFw~}
                        {
                            Modbus_03_Solve();
                            break;
                        }

                        case 6: //~{P45%8v<D4fFw~}
                        {
                            Modbus_06_Solve();
                            break;
                        }
                        case 7: //~{2bJTD#J=#,A,PxJd3vJ}>]~} 0x01	0x07	0x0002	0x0002	0x940b
                        {
                            TesetFlag = 1;
                            break;
                        }
                        case 8: //~{J*6HP#W<O5J}~}
                        {
                            Modbus_08_Solve();
                            break;
                        }
                        case 9: //~{2N?<5gQ9!"NB6H293%!"J*6H5gQ9293%~}
                        {
                            Modbus_09_Solve();
                            break;
                        }
                        case 10: //~{Ft6/P#W<~}
                        {
                            Modbus_10_Solve();
                            break;
                        }
                        case 11: //~{P#W<~}
                        {
                            Modbus_11_Solve();
                            break;
                        }
                        case 12: //~{P#W<~}
                        {
                            Modbus_12_Solve();
                            break;
                        }

                        default:
                            TesetFlag = 0;
                            ;
                        }
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else//CRC~{P#Qi4mNs~}
                    {

                        modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                        modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                        modbus_usr.RS485_TX_BUFF[2]=0x04; //~{Rl3#Bk~}
#if DEBUG
                        RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
#endif
                    }
                }
                else//~{<D4fFw5XV73,3v76N'~}
                {
                    modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                    modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                    modbus_usr.RS485_TX_BUFF[2]=0x02; //~{Rl3#Bk~}
#if DEBUG
                   // RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
#endif
                }
            }
            else//~{9&D\Bk4mNs~}
            {
                modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
                modbus_usr.RS485_TX_BUFF[1]=modbus_usr.RS485_RX_BUFF[1]|0x80;
                modbus_usr.RS485_TX_BUFF[2]=0x01; //~{Rl3#Bk~}
#if DEBUG
                //RS485_SendData(modbus_usr.RS485_TX_BUFF,3);
#endif
            }
        }

        modbus_usr.RS485_FrameFlag=0;//~{84N;V!=aJx1jV>~}
        modbus_usr.RS485_RX_CNT=0;//~{=SJU<FJ}FwGeAc~}
        modbus_usr.RS485_TX_EN=0;//~{?*Ft=SJUD#J=~}
    }
    if( modbus_usr.RS485_FrameFlag==3)
    {
        modbus_usr.RS485_FrameFlag=0;//~{84N;V!=aJx1jV>~}
        modbus_usr.RS485_RX_CNT=0;//~{=SJU<FJ}FwGeAc~}
        modbus_usr.RS485_TX_EN=0;//~{?*Ft=SJUD#J=~}
    }
	 //HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
   // HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
}






//Modbus~{9&D\Bk~}03~{4&@m3LPr~}///////////////////////////////////////////////////////////////////////////////////////~{RQQiV$3LPr~}OK
//~{6A1#3V<D4fFw~}

void Modbus_03_Solve(void)
{
    u8 i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x02||startRegAddr==0x03||startRegAddr==0x0100||startRegAddr==0x0101))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
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
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
                else if(i<2)
                {
                    tmp = sensor_usr.temperature;
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((tmp>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == TE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((sensor_usr.temperature>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(sensor_usr.temperature);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == DEV_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Addr>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)(modbus_usr.RS485_Addr);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            else if(startRegAddr == RATE_ADDR)
            {
                if(i<1)
                {
                    modbus_usr.RS485_TX_BUFF[3+i*2]=(unsigned char)((modbus_usr.RS485_Baudrate>>8)&&0xff);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    modbus_usr.RS485_TX_BUFF[4+i*2]=(unsigned char)modbus_usr.RS485_Baudrate;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,RegNum*2+3);
        modbus_usr.RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC~{8_5XN;2;6TBp#?~}  // ~{OH8_:s5M~}
        modbus_usr.RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,RegNum*2+5);
    }
}


//Modbus~{9&D\Bk~}06~{4&@m3LPr~}   //////////////////////////////////////////////////////////////////////////////////~{RQQiV$3LPr~}OK
//~{P45%8v1#3V<D4fFw~}

void Modbus_06_Solve(void)
{

    unsigned char i;
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|modbus_usr.RS485_RX_BUFF[5];//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0100||startRegAddr==0x0101))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
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

                    //modbus_usr.RS485_TX_BUFF[3+i*2]=(modbus_usr.RS485_Addr>>8)&&0xff;;//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//           /////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
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
                    RS485_Init();															/////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
                    //modbus_usr.RS485_TX_BUFF[4+i*2]=(sensor_usr.temperature)&&0xff;//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
                }
            }
            unsigned char addr;
            addr = 0;
            flash_init();
            flash_write(addr++,0x5a,1);
            flash_write(addr++,modbus_usr.RS485_Addr,1);
            flash_write(addr++,modbus_usr.RS485_Baudrate,1);
            flash_write(addr++,modbus_usr.RS485_Parity,1);
            fator_save_proc(addr);
        }
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
        modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;         //CRC~{8_5XN;2;6TBp#?~}  // ~{OH8_:s5M~}
        modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
    }
}


//Modbus~{9&D\Bk~}03~{4&@m3LPr~}///////////////////////////////////////////////////////////////////////////////////////~{RQQiV$3LPr~}OK sensor_usr
//~{6A1#3V<D4fFw~}
extern factor_stru factor_usr;
void Modbus_07_Solve(void)
{




    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0103))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
    {
        HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
        printf("   %10f   %10f   %10f   %10f    %d\n",sensor_usr.sensor[0],sensor_usr.sensor[1],sensor_usr.sensor[2],factor_usr.humid,sensor_usr.last_Temp);
        HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
    }


}
//Modbus~{9&D\Bk~}06~{4&@m3LPr~}   //////////////////////////////////////////////////////////////////////////////////~{RQQiV$3LPr~}OK
//~{P45%8v1#3V<D4fFw~}

void Modbus_08_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr+RegNum)>=12&&(startRegAddr==0x0102)
            &&(RegNum==0x000c))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
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

        factor_usr.a0 = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[6]),4,10);
        factor_usr.a1 = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[10]),4,10);
        factor_usr.a2 = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[14]),4,10);

        unsigned char addr;
        addr = 0;
        flash_init();
        flash_write(addr++,0x5a,1);
        flash_write(addr++,modbus_usr.RS485_Addr,1);
        flash_write(addr++,modbus_usr.RS485_Baudrate,1);
        flash_write(addr++,modbus_usr.RS485_Parity,1);
        //if(sensor_usr.CalibrationVref!=0)
        {
            fator_save_proc(addr);
            flash_write(addr+12+4+4,sensor_usr.CalibrationT,1);
        }
        //else
        // flash_write_bytes(addr++,&(modbus_usr.RS485_RX_BUFF[6]),12);
        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,18);

        modbus_usr.RS485_TX_BUFF[18]=(calCRC>>8)&0xFF;         //CRC~{8_5XN;2;6TBp#?~}  // ~{OH8_:s5M~}
        modbus_usr.RS485_TX_BUFF[19]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,20);
    }

}
void Modbus_09_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0104)&&(RegNum==0x0009))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
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

        sensor_usr.CalibrationProbeVref =  DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[10]),4,1000);	//~{L=Uk?U2b5gQ97E4s~}1000~{16~}
        sensor_usr.CalibrationT = modbus_usr.RS485_RX_BUFF[14];//~{NB6H293%~}
        if(modbus_usr.RS485_RX_BUFF[6]!=0||modbus_usr.RS485_RX_BUFF[7]!=0||modbus_usr.RS485_RX_BUFF[8]!=0||modbus_usr.RS485_RX_BUFF[9]!=0)
        {
            sensor_usr.CalibrationVref = DataMinusProc2(&(modbus_usr.RS485_RX_BUFF[6]),4,1000);//~{2N?<5gQ9~}
        }
        else
        {

            params_save();
        }

        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,15);
        modbus_usr.RS485_TX_BUFF[15]=(calCRC>>8)&0xFF;         //CRC~{8_5XN;2;6TBp#?~}  // ~{OH8_:s5M~}
        modbus_usr.RS485_TX_BUFF[16]=(calCRC)&0xFF;
        RS485_SendData(modbus_usr.RS485_TX_BUFF,17);
    }
}
void Modbus_10_Solve(void)
{
    RegNum= (((u16)modbus_usr.RS485_RX_BUFF[4])<<8)|((modbus_usr.RS485_RX_BUFF[5]));//~{;qH!<D4fFwJ}A?~}
    if((startRegAddr+RegNum)<1000&&(startRegAddr==0x0105)&&(RegNum==0x0002))//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
    {
        getRatio()->calibrationFlag = 1;//~{P#W<?*J<1jV>~}
        getRatio()->samplecount = 0;

        getRatio()->aF = 0;//F
        getRatio()->aG =0;//G
        getRatio()->aH = 0;//H
        getRatio()->aI = 0;//I
        getRatio()->aB =0;//B
        getRatio()->aC = 0;//C
        getRatio()->aD = 0;//D

    }

}
void Modbus_11_Solve(void)
{
    RegNum= ((modbus_usr.RS485_RX_BUFF[2]));//~{;qH!<D4fFwJ}A?~}
    if(RegNum==0x04)//~{<D4fFw5XV7~}+~{J}A?TZ76N'DZ~}
    {
        float tmp1,tmp2,tmp4;
        uint32_t tmp3;
        tmp3 = modbus_usr.RS485_RX_BUFF[4] << 8;  //~{;qH!2N?<Qy;z5DJ*6H5gQ9#,J}>]7E4s~}1000~{16~}
        tmp3 = tmp3 + modbus_usr.RS485_RX_BUFF[5];
        tmp2 = tmp3 / 10000.0;
        if(getRatio()->calibrationFlag == 1 )
        {
            Get_Adc_Average(N);

            tmp1 = getSensor()->sensor[3];


            if(CalibrationRatio(tmp1,tmp2)->status == 0)//~{=xPPP#W<#,P#W<Mj3I~}
            {
                getFactor()->a0 = getRatio()->a0; //~{8|PBP#W<O5J}~}
                getFactor()->a1 = getRatio()->a1;
                getFactor()->a2 = getRatio()->a2;
                getFactor()->status = 1;//~{P#W<Mj3I1jV>N;~}
                params_save();
							#if DEBUG == 1
                HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
                printf("   %10f	 %10f	%10f\n",getFactor()->a0,getFactor()->a1,getFactor()->a2);//output calibration ratio
                HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
                #endif
            }
            else
            { 
							#if DEBUG == 1
                 HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
							 
                 printf("   %10f	 %10f\n",sensor_usr.sensor[0],tmp2);//~{Jd3v2I</J*6H5gQ9#,2N?<Qy;zJ*6H5gQ9~}
                 HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
							  #endif
                getFactor()->a0 = 82.93;
                getFactor()->a1 = -27.688;
                getFactor()->a2 = 12.822;
                getFactor()->status = 0;

            }

        }
        else
        {
            //Get_Adc_Average(N);
            // HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_SET);
            ; //printf("   %10f  %10f	%10f\n",sensor_usr.sensor[0],tmp2,factor_usr.humid);
            //HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);

        }

    }

}
unsigned int tmp,tmp2;
void Modbus_12_Solve()
{
		   modbus_usr.RS485_TX_BUFF[0]=modbus_usr.RS485_RX_BUFF[0];
		   modbus_usr.RS485_TX_BUFF[1]=0x0a;
		   modbus_usr.RS485_TX_BUFF[2]=0x04;
		   modbus_usr.RS485_TX_BUFF[3]=0x01;

			
			tmp = (sensor_usr.sensor[3]*10000);
	    tmp2 = tmp >>8;
			modbus_usr.RS485_TX_BUFF[4]=(unsigned char)(tmp2);//(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//			/////////~{OH7"KM8_WV=Z~}--~{TZ7"KM5MWV=Z~}
   	modbus_usr.RS485_TX_BUFF[5]=(unsigned char)(tmp);//(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
		   calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,6);
		   modbus_usr.RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;		  //CRC~{8_5XN;2;6TBp#?~}  // ~{OH8_:s5M~}
		   modbus_usr.RS485_TX_BUFF[7]=(calCRC)&0xFF;
		   RS485_SendData(modbus_usr.RS485_TX_BUFF,8);
	 HAL_GPIO_WritePin(GPIOA, RS485_EN1_Pin, GPIO_PIN_RESET);
}


