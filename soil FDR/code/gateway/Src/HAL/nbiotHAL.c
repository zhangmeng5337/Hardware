#include <string.h>
#include "AT_CMD.h"
#include "nbiotHW.h"
#include "string.h"
#include "nbiotHAL.h"
#include "stdlib.h"



#define Network_Thres    30
unsigned char one_net_key[]="*296832#838905307#json*";//296832����Ʒ��ţ�571498701����Ȩ�룻json���ű�
unsigned char Establish_TCP_Connection[]="AT+CAOPEN=0,0,\"TCP\",\"dtu.heclouds.com\",1811\r";

//unsigned char Establish_TCP_Connection[100];//="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
//unsigned char Establish_TCP_Connection2[100]="AT+CIPSTART=\"TCP\",\"192.168.1.3\",3\r";
unsigned char	NET_STAUS=SIMCOM_NET_NOT;//��������״̬��Ϣ��־
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Get_Network_status(void)
{
    return NET_STAUS;
}

unsigned char uchar2ascii(unsigned char *p,unsigned char len,unsigned char *result,unsigned char *result_len)
{
    unsigned char i,j;
    unsigned  tmp;
    unsigned char q[20];
    j = 0;
    for(i= 0; i<len; i++)
    {
        tmp = p[i];
        tmp = tmp << 8;
        tmp = tmp +p[i+1];
        if(tmp<10)  //0-9
        {
            result[j++] = tmp+0x30;
        }
        else if(tmp<100)//10-99
        {
            result[j++] = tmp/10+0x30;
            result[j++] = tmp%10+0x30;
        }
        else if(tmp<1000)//100-999
        {
            result[j++] = tmp/100+0x30;
            result[j++] = tmp%100/10+0x30;
            result[j++] = tmp%10+0x30;

        }
        else if(tmp<10000)//1000-9999 2221
        {
            result[j++] = tmp/1000+0x30;
            result[j++] = tmp%1000/100+0x30;
            result[j++] = tmp%100/10+0x30;
            result[j++] = tmp%10+0x30;

        }
        else if(tmp<100000)//10000-99999 2221
        {
            result[j++] = tmp/10000+0x30;
            result[j++] = tmp%10000/1000+0x30;
            result[j++] = tmp%1000/100+0x30;
            result[j++] = tmp%100/10+0x30;
            result[j++] = tmp%10+0x30;

        }
    }
    *result_len = j;
    result = q;
}
void ServerIP_Pack(unsigned char *q)
{

    unsigned char *p;

    p = malloc(6);
//    result_len = malloc(1);
//    i = 0;
//    memcpy(Establish_TCP_Connection,TCP_ConnectCMD,sizeof(TCP_ConnectCMD)-1);
//    i = sizeof(TCP_ConnectCMD)-2;
//    uchar2ascii(q,1,p,result_len);
//    for(k= 0; k<*result_len; k++)
//    {
//        Establish_TCP_Connection[i++]=p[k];
//    }
//    Establish_TCP_Connection[i++] = '.';

//    uchar2ascii(q+2,1,p,result_len);
//    for(k= 0; k<*result_len; k++)
//    {
//        Establish_TCP_Connection[i++]=p[k];
//    }
//    Establish_TCP_Connection[i++] = '.';

//    uchar2ascii(q+4,1,p,result_len);
//    for(k= 0; k<*result_len; k++)
//    {
//        Establish_TCP_Connection[i++]=p[k];
//    }
//    Establish_TCP_Connection[i++] = '.';

//    uchar2ascii(q+6,1,p,result_len);
//    for(k= 0; k<*result_len; k++)
//    {
//        Establish_TCP_Connection[i++]=p[k];
//    }
//    //Establish_TCP_Connection[i++] = '.';
//    Establish_TCP_Connection[i++] = '\"';
//    Establish_TCP_Connection[i++] = ',';

//    uchar2ascii(q+8,1,p,result_len);
//    for(k= 0; k<*result_len; k++)
//    {
//        Establish_TCP_Connection[i++]=p[k];
//    }
//    Establish_TCP_Connection[i++] = '\r';


	


}
void SIMCOM_ReConnect()
{
    if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //����ϵͳ��ʱ���ƣ���֤ϵͳ�ܹ��ڳ�ʱ��������������
        SIMCOM_TimeOut_Count++;
    else if(NET_STAUS==SIMCOM_NET_OK)
        SIMCOM_TimeOut_Count=0;
    if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
    {
        SIMCOM_TimeOut_Count=0;
        NET_STAUS=SIMCOM_NET_NOT;
    }

}
#define debug  1
//static uint32_t count = 0;
void set_NetStatus(unsigned char flag)
{
    NET_STAUS = flag;
    SIMCOM_TimeOut_Count = 0;
}
void equipCtrlNbiot(void)
{
    static unsigned char flag;
	if(ROLE !=GATEWAY)
	{
		sendCommand("AT+CPOWD=1\r\n", "OK\r\n", 500, 1);
	}
 

}
void SIMCOM_Register_Network()
{
   // unsigned char *p;
   // p=malloc(sizeof(unsigned char) * 64);

    switch(NET_STAUS)
    {


    case SIMCOM_NET_NOT:
    {
        SIMCOM_TimeOut_Count = 0;
        NET_STAUS = SIMCOM_NET_NOT;
        nbiot_HardwareInit(ON);                   //��λ����
        NET_STAUS=SIMCOM_POWER_ON;       //״̬����λ
    }
    break;
    case SIMCOM_POWER_ON://SIMCOM_READY_YES:
    {

        // if (sendCommand("AT+CGMR\r\n", "OK\r\n", 1000000, 1) == Success)
        if (sendCommand("AT\r\n", "OK\r\n", 50000, 1) == Success)
            NET_STAUS = SIMCOM_READY_YES;
        else
        {
            ;//NET_STAUS = SIMCOM_NET_NOT;

        }

    }
    break;
    case SIMCOM_READY_YES:
    {
        if (sendCommand("ATE0\r\n", "OK\r\n", 10000, 1) == Success)
            NET_STAUS = SIMCOM_CLOSE_ECHO;
    }
    break;


    case SIMCOM_CLOSE_ECHO:
    {
        if (sendCommand("AT+CPIN?\r\n", "READY", 10000, 1) == Success)
            NET_STAUS = SIMCOM_CARD_DET;
    }
    break;
    case SIMCOM_CARD_DET:
    {
    
		//if (sendCommand("AT+CSQ\r\n", "+CSQ: 2", 13000000, 1) == Success)
        if (sendCommand("AT+CGATT?\r\n", "+CGATT: 1", 130000, 1) == Success)
        {

            NET_STAUS=SIMCOM_GPRS_READY;
        }
    }
    break;

    case SIMCOM_GPRS_READY:
    {
        if (sendCommand("AT+CASSLCFG=0,\"SSL\",0\r\n", "OK", 10000, 1) == Success)
            NET_STAUS = SIMCOM_NET_CLOSE;
    }
    break;
    case SIMCOM_NET_CLOSE:
    {
        if (sendCommand("AT+CNACT=0,1\r\n", "ACTIVE", 20000, 1) == Success)
            NET_STAUS = SIMCOM_NET_CONFIG;
        SIMCOM_TimeOut_Count = 0;
    }
    break;


    case SIMCOM_NET_CONFIG://SIMCOM_NET_PASS_THROUGH:
    {
        if (sendCommand(Establish_TCP_Connection, "OK", 500000, 1) == Success)
        {

            NET_STAUS=SIMCOM_NET_OK;
            NET_STAUS=SIMCOM_Connect_Platform;
        }

    }
    break;
    case SIMCOM_Connect_Platform:
    {
        if (sendCommand("AT+CASWITCH =0,1\r\n", "OK", 10000, 1) == Success)
            NET_STAUS = SIMCOM_NET_PASS_THROUGH;
        else
            NET_STAUS = SIMCOM_NET_PASS_THROUGH;
        SIMCOM_TimeOut_Count = 0;
    }
    break;
    case SIMCOM_NET_PASS_THROUGH:
    {
        static unsigned char tx_count;

        if (sendCommand(one_net_key, "received", 340000, 1) == Success)
        {
            NET_STAUS=SIMCOM_NET_OK;
        }
        else
        {
            tx_count++;
            if(tx_count>=100)
            {
                // NET_STAUS=SIMCOM_NET_OK;
                tx_count = 0;
            }

        }
    }
    break;


    case SIMCOM_NET_OK:
    {
        if (sendCommand(one_net_key, "CLOSED", 20, 0) == Success||
                sendCommand(one_net_key, "+CIPOPEN: 0,4", 20, 0) == Success||
                sendCommand(one_net_key, "+CASTATE: 0", 20, 0) == Success)
        {
            NET_STAUS=SIMCOM_NET_ERROR;
        }
    }
    break;
    case SIMCOM_NET_ERROR:
    {
        nbiot_HardwareInit(ON);                   //��λ����
        NET_STAUS=SIMCOM_POWER_ON;       //״̬����λ
    }
    break;
    default :
        break;
    }
    SIMCOM_ReConnect();
 
}
