#include "LteHal.h"
#include "string.h"
#include "LteHardware.h"


unsigned char one_net_key[]="*284261#580794566#json*";//296832：产品编号；571498701：鉴权码；json：脚本
#if ROLE == LTE_4G
unsigned char Establish_TCP_Connection[]="AT+CIPOPEN=0,\"TCP\",\"thingssaas.jiyikeji.cn\",9501\r";
#else
unsigned char Establish_TCP_Connection[]="AT+CAOPEN=0,0,\"TCP\",\"thingssaas.jiyikeji.cn\",9501\r";
#endif
//unsigned char Establish_TCP_Connection[100];//="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
//unsigned char Establish_TCP_Connection2[100]="AT+CIPSTART=\"TCP\",\"192.168.1.3\",3\r";
lte_stru lte_usr;
//lte_usr.NetStatus=SIMCOM_NET_NOT;//网络链接状态信息标志
uint32_t  SIMCOM_TimeOut_Count;


lte_stru *GetLteStru()
{
    return &lte_usr;
}
unsigned int sendCommand(unsigned char moduleType,char *Command, char *Response, uint32_t Timeout, unsigned char Retry,unsigned char eflag)
{
    unsigned char *USARTX_RX_BUF;


    static unsigned char RetryCount;
    static uint32_t timeout;
    if(moduleType == LTE_4G)
    {

        USARTX_RX_BUF =lte_usr.lterxbuffer;

    }
    else if((moduleType == LTE_NBIOT))
    {

        USARTX_RX_BUF =lte_usr.lterxbuffer;;
    }
    uint32_t tmp;
    tmp = HAL_GetTick()-timeout;
    if((tmp)>=Timeout)
    {
        if(RetryCount<=Retry)
        {
            if(Retry>0)
            {
                LteUart_SendStr( moduleType,Command);
                RetryCount = RetryCount +1;
                timeout = HAL_GetTick();
            }

        }
        else
        {
            RetryCount = 0;
        }

        if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
            if(eflag == 0)
                USART1_CLR_Buf();
            return Success;
        }
        else
        {
					  if(eflag == 1)
            USART1_CLR_Buf();
            return Failure;
        }

    }
    else
    {
        if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
            if(eflag == 0)
                USART1_CLR_Buf();
            return Success;
        }
        else
        {

			// USART1_CLR_Buf();
            return Failure;
        }

    }

}

void SIMCOM_ReConnect()
{
    if( lte_usr.NetStatus!=SIMCOM_NET_NOT&& lte_usr.NetStatus!=SIMCOM_NET_OK) //控制系统超时机制，保证系统能够在超时后重新启动链接
        SIMCOM_TimeOut_Count++;
    else if( lte_usr.NetStatus==SIMCOM_NET_OK)
        SIMCOM_TimeOut_Count=0;
    if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
    {
        SIMCOM_TimeOut_Count=0;
        lte_usr.NetStatus=SIMCOM_NET_NOT;
        lte_usr.RetryConnectCount = 0;
    }

}
#define debug  1
//static uint32_t count = 0;
void set_NetStatus(unsigned char flag)
{
    lte_usr.NetStatus   = flag;
    SIMCOM_TimeOut_Count = 0;
}
unsigned char * GetLteTime()
{
    if (sendCommand(LTE_4G,"AT+CCLK?\r\n", "AT+CCLK=", 50000, 3,1) == Success)
    {
        return lte_usr.lterxbuffer;
    }

}
void SIMCOM_Register_Network()
{
    // unsigned char *p;
    // p=malloc(sizeof(unsigned char) * 64);
    unsigned char moduleType;
#if ROLE == 0
    moduleType = LTE_4G;
#else
    moduleType = LTE_NBIOT;
#endif
    switch( lte_usr.NetStatus)
    {


    case SIMCOM_NET_NOT:
    {
        SIMCOM_TimeOut_Count = 0;
        if(lte_usr.RetryConnectCount <= MAX_CONNECT_COUNT)
            lte_usr.RetryConnectCount ++;
        if(DEBUG_MODE == 0)
            LtePowerManage(LTE_4G,ON);                   //复位重启
        lte_usr.NetStatus=SIMCOM_POWER_ON;       //状态机复位
    }
    break;
    case SIMCOM_POWER_ON://SIMCOM_READY_YES:
    {

        if (sendCommand(moduleType,"AT\r\n", "OK\r\n", 200, 1,0) == Success)
            lte_usr.NetStatus = SIMCOM_READY_YES;
        else
        {
            ;//lte_usr.NetStatus = SIMCOM_NET_NOT;

        }

    }
    break;
    case SIMCOM_READY_YES:
    {
        if (sendCommand(moduleType,"ATE0\r\n", "OK\r\n", 200, 1,0) == Success)
            lte_usr.NetStatus = SIMCOM_CLOSE_ECHO;
    }
    break;


    case SIMCOM_CLOSE_ECHO:
    {
        if (sendCommand(moduleType,"AT+CPIN?\r\n", "READY", 500, 1,0) == Success)
            lte_usr.NetStatus = SIMCOM_CARD_DET;
    }
    break;
    case SIMCOM_CARD_DET:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CGATT?\r\n", "PB DONE", 1000, 0,0) == Success)
            lte_usr.NetStatus=SIMCOM_NET_DONE;
#else
        if (sendCommand(moduleType,"AT+CREG?\r", "PB DONE", 1000, 0,0) == Success)
            lte_usr.NetStatus=SIMCOM_NET_DONE;
#endif
    }
    break;
    case SIMCOM_NET_DONE:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CGATT?\r\n", "+CGATT: 1", 2000, 1,0) == Success)
            lte_usr.NetStatus=SIMCOM_GPRS_READY;
#else
        if (sendCommand(moduleType,"AT+CREG?\r", "OK", 1000, 1,0) == Success)
            lte_usr.NetStatus=SIMCOM_GPRS_READY;
#endif
    }
    break;
    case SIMCOM_GPRS_READY:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CASSLCFG=0,\"SSL\",0\r\n", "OK", 500, 1,0) == Success)
#else
        ;//if (sendCommand(moduleType,"AT+CIPCLOSE=1\r", "OK", 1000, 1,0) == Success)

#endif
			

            lte_usr.NetStatus = SIMCOM_NET_CLOSE;
    }
    break;

		case SIMCOM_NET_CLOSE:
		{
#if ROLE == LTE_NBIOT
			if (sendCommand(moduleType,"AT+CASWITCH =0,1\r\n", "OK", 1000, 1,0) == Success)
						{
						// RTC_Calibration(lte_usr.lterxbuffer);
						lte_usr.NetStatus = SIMCOM_NET_UPDATE;
					}
#else
			if(sendCommand(LTE_4G,"AT+CTZU=1\r\n", "OK", 1000, 3,1) == Success)//鏃堕棿鏍″噯
					{
						// RTC_Calibration(lte_usr.lterxbuffer);
						lte_usr.NetStatus = SIMCOM_NET_UPDATE;
					}
			
#endif
				
		}
		break;


    case SIMCOM_NET_UPDATE:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CASWITCH =0,1\r\n", "OK", 1000, 1,0) == Success)
    				{
				     RTC_Calibration(lte_usr.lterxbuffer);
					lte_usr.NetStatus = SIMCOM_NET_RTC;
				}            
#else
        if(sendCommand(LTE_4G,"AT+CCLK?\r\n", "+CCLK", 1000, 3,1) == Success)//鏃堕棿鏍″噯
				{
				     RTC_Calibration(lte_usr.lterxbuffer);
					lte_usr.NetStatus = SIMCOM_NET_RTC;
				}
        
#endif
            
    }
    break;
    case SIMCOM_NET_RTC:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CASWITCH =0,1\r\n", "OK", 1000, 1,0) == Success)

#else
        if (sendCommand(moduleType,"AT+CIPMODE =1\r\n", "OK", 1000, 1,0) == Success)
#endif

            lte_usr.NetStatus = SIMCOM_NET_PASS_THROUGH;
    }
    break;
    case SIMCOM_NET_PASS_THROUGH:
    {
#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,"AT+CNACT=0,1\r\n", "ACTIVE", 1000, 1,0) == Success)

#else
        if (sendCommand(moduleType,"AT+NETOPEN\r\n", "Network is already", 1000, 1,0) == Success)

#endif
            lte_usr.NetStatus = SIMCOM_NET_CONFIG;
        SIMCOM_TimeOut_Count = 0;
    }
    break;
    case SIMCOM_NET_CONFIG://SIMCOM_NET_PASS_THROUGH:
    {

#if ROLE == LTE_NBIOT
        if (sendCommand(moduleType,Establish_TCP_Connection, "OK", 2000, 1,0) == Success)
            lte_usr.NetStatus=SIMCOM_Connect_Platform;
#else
        if (sendCommand(moduleType,Establish_TCP_Connection, "CONNECT", 2000, 1,0) == Success)
            lte_usr.NetStatus=SIMCOM_Connect_Platform;
#endif


    }
    break;
    case SIMCOM_Connect_Platform:
    {
        static unsigned char tx_count;
lte_usr.NetStatus=SIMCOM_NET_OK;
        if (sendCommand(moduleType,one_net_key, "received", 1400, 1,0) == Success)
        {
            lte_usr.NetStatus=SIMCOM_NET_OK;
            lte_usr.RetryConnectCount = 0;
        }
        else
        {
            tx_count++;
            if(tx_count>=100)
            {
                lte_usr.NetStatus=SIMCOM_NET_OK;
                tx_count = 0;
                lte_usr.RetryConnectCount = 0;
            }

        }
    }
    break;


    case SIMCOM_NET_OK:
    {
        if (sendCommand(moduleType,one_net_key, "CLOSED", 20, 0,0) == Success||
                sendCommand(moduleType,one_net_key, "+CIPOPEN: 0,4", 20, 0,0) == Success||
                sendCommand(moduleType,one_net_key, "+CASTATE: 0", 20, 0,0) == Success)
        {
            lte_usr.NetStatus=SIMCOM_NET_ERROR;
        }
    }
    break;
    case SIMCOM_NET_ERROR:
    {
        LtePowerManage(LTE_4G,ON);                   //复位重启
        lte_usr.NetStatus=SIMCOM_POWER_ON;       //状态机复位
    }
    break;
    default :
        break;
    }
    SIMCOM_ReConnect();

}


