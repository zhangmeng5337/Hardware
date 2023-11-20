#include "cat1.h"

static uint8_t CurrentRty;
static teATCmdNum ATRecCmdNum;
static teATCmdNum ATCurrentCmdNum;
static teATCmdNum ATNextCmdNum;
static teCAT1_TaskStatus CAT1_TaskStatus;

static tsTimeType TimeCAT1;
//char CAT1_SENDDataBuff[200];

char Imei_buffer[16];
char CSQ_buffer[3];
char Version_buffer[20];
unsigned char  Bin_buffer[1026] ={0};
char Bin_len[10];
char Msg_Len[10];
char topic_buffer[50];
char message_buffer[150];
char send_buffer[200];

int addr_count = 0;
int reboot_flag = 0;
int Erase_flag = 1;
/*入网AT指令集*/
tsATCmds ATCmds[] =
{
    /* 下面是关于连接MQTT的指令集*/
    //{"AT+CRESET\r\n","PB DONE",5000,NO_REC,3},
    {"AT\r\n","OK",1000,NO_REC,3},															//AT指令测试
    {"ATE0\r\n","OK",1000,NO_REC,3},														//关闭回显
    {"AT+CGSN\r\n","OK",1000,NO_REC,3},													//查询Imei
    {"AT+CCID\r\n","OK",1000,NO_REC,3},													//查询ICCID
    {"AT+CEREG?\r\n","OK",1000,NO_REC,100},											//查询当前GPRS注册状态
    {"AT+CGATT?\r\n","OK",1000,NO_REC,3},												//查询当前GPRS附着状态
    {"AT+CSQ\r\n","OK",1000,NO_REC,3},													//查询信号值
    {"AT+CMQTTSTART\r\n","+CMQTTSTART: 0",2000,NO_REC,3},				//开启MQTT连接
    {"AT+CMQTTACCQ=0,\"clientid01\"\r\n","OK",2000,NO_REC,3},		//注册MQTT的ClientID
    {"AT+CMQTTCONNECT=0,\"tcp://47.98.183.97:1884\",120,1,\"admin\",\"public\"\r\n","+CMQTTCONNECT: 0,0",3000,NO_REC,3},	//设置IP port username password keepalive
    {"AT+CSQ\r\n","OK",1000,NO_REC,3},													//查询CSQ
    {"AT+CMQTTTOPIC=0,",">",2000,NO_REC,3},											//设置topic
    {topic_buffer,"OK",3000,NO_REC,3},													//topic内容
    {"AT+CMQTTPAYLOAD=0,",">",2000,NO_REC,3},										//设置发送报文
    {message_buffer,"OK",3000,NO_REC,5},												//msg内容
    {"AT+CMQTTPUB=0,1,60\r\n","+CMQTTPUB: 0,0",3000,NO_REC,5},	//发送报文
    /* 下面是关于查询版本号的AT指令集*/
    {"AT+HTTPINIT\r\n","OK",1000,NO_REC,3},											//开启HTTP服务
    {"AT+HTTPPARA=\"URL\",\"http://47.98.248.24:8888/getVersion?device_id=123456\"\r\n","OK",1000,NO_REC,3},	//设置HTTP参数
    {"AT+HTTPACTION=0\r\n","+HTTP_PEER_CLOSED",1000,NO_REC,3},				//操作HTTP方法
    {"AT+HTTPREAD=0,100\r\n","+HTTPREAD:0",1000,NO_REC,3},			//读取HTTP服务回复
    {"AT+HTTPTERM\r\n","OK",1000,NO_REC,3},											//关闭HTTP服务
    /* 下面是关于下载BIN文件的AT指令集*/
    {"AT+HTTPINIT\r\n","OK",1000,NO_REC,3},											//开启HTTP服务
    {"AT+HTTPPARA=\"URL\",\"http://47.98.248.24:8888/download1?device_id=123456&version=2.0.0\"\r\n","OK",3000,NO_REC,3},	//设置HTTP参数
    {"AT+HTTPACTION=0\r\n","+HTTPACTION:",3000,NO_REC,3},				//操作HTTP方法
    {"AT+HTTPREAD=0,1024\r\n","+HTTPREAD:",3000,NO_REC,3},			//读取HTTP服务回复
    {"AT+HTTPTERM\r\n","OK",1000,NO_REC,3},											//关闭HTTP服务
};

/* AT指令发送处理逻辑 */
void ATSend(teATCmdNum ATCmdNum)
{
    //清空接收缓存区
    memset(Lpuart1type.Lpuart1RecBuff,0,LPUART1_REC_SIZE);
    ATCmds[ATCmdNum].ATStatus = NO_REC;
    ATRecCmdNum = ATCmdNum;
    printf("\r\n当前命令码：%d\r\n",ATCmdNum);
    /* 设置topic的长度 */
    if(ATCmdNum == AT_CMQTTTOPIC)
    {
        memset(topic_buffer,0x00,sizeof(topic_buffer));
        sprintf(topic_buffer,"dev/%s",Imei_buffer);
        sprintf(send_buffer,"%s%d\r\n",ATCmds[ATCmdNum].ATSendStr,strlen(topic_buffer));
        HAL_UART_Transmit(&hlpuart1,(uint8_t*)send_buffer,strlen(send_buffer),0xFF);
        printf("send_buffer:%s\r\n",send_buffer);
        memset(send_buffer,0x00,strlen(send_buffer));
    }
    /* 设置msg的长度 */
    else if(ATCmdNum == AT_CMQTTPAYLOAD)
    {
        memset(message_buffer,0x00,sizeof(message_buffer));
        sprintf(message_buffer,"{\"BSQ\":%s}",CSQ_buffer);
        sprintf(send_buffer,"%s%d\r\n",ATCmds[ATCmdNum].ATSendStr,strlen(message_buffer));
        HAL_UART_Transmit(&hlpuart1,(uint8_t*)send_buffer,strlen(send_buffer),0xFF);
        printf("send_buffer:%s",send_buffer);
        memset(send_buffer,0x00,sizeof(send_buffer));
    }
    else
    {
        HAL_UART_Transmit(&hlpuart1,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),0xff);
        printf("send:%s",ATCmds[ATCmdNum].ATSendStr);
    }
    //打开超时定时器
    SetTime(&TimeCAT1,ATCmds[ATCmdNum].TimeOut);

}

/* AT指令接收处理逻辑 */
void ATRec(void)
{
    if(Lpuart1type.Lpuart1RecFlag)
    {
        if(strstr((const char*)Lpuart1type.Lpuart1RecBuff,ATCmds[ATRecCmdNum].ATRecStr) != NULL)
        {
            ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;
        }
        printf("收到数据：%s",Lpuart1type.Lpuart1RecBuff);
        Lpuart1type.Lpuart1RecFlag = 0;
        Lpuart1type.Lpuart1RecLen = 0;
    }
}
/* 初始化CAT1 */
void CAT1_Init(void)
{
    Start_4G();
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT;
    ATNextCmdNum = ATE0;
}

/* AT指令交互逻辑 */
void CAT1_Task(void)
{
    while(1)
    {
        switch(CAT1_TaskStatus)
        {
        case CAT1_IDIE: //空闲态
            return;
        case CAT1_SEND:
            if(ATCurrentCmdNum !=ATNextCmdNum)
            {
                CurrentRty = ATCmds[ATCurrentCmdNum].RtyNum;
            }
            ATSend(ATCurrentCmdNum);
            CAT1_TaskStatus = CAT1_WAIT;

            return;
        case CAT1_WAIT:	//等待态，等待CAT1返回的信息
            ATRec();	//调用接收函数
            if(ATCmds[ATRecCmdNum].ATStatus == SUCCESS_REC)
            {
                printf("当前码：%d\r\n",ATCurrentCmdNum);
                if(ATCurrentCmdNum == AT_CGSN)	//如果AT指令为查询IMEI
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset (CSQ_buffer,0x00,sizeof(CSQ_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "\r\n", "\r\n", Imei_buffer);
                    printf("Imei_buffer=%s\r\n",Imei_buffer);
                    break;
                }
                else if(ATCurrentCmdNum == AT_CSQ_2)	//如果AT指令为查询CSQ
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset(CSQ_buffer,0x00,sizeof(CSQ_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, " ", ",", CSQ_buffer);
                    printf("CSQ_buffer=%s\r\n",CSQ_buffer);
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPREAD_1)	//如果AT指令为查询版本号
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset(Version_buffer,0x00,sizeof(Version_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "{", "}", Version_buffer);
                    printf("Version_buffer=%s\r\n",Version_buffer);
                    if(strcmp(Version_buffer,"\"version\":\"4.0.0\"") == 0)
                    {
                        printf("硬件版本和云端版本一致，无需升级！\r\n");
                    }
                    else
                    {
                        Update_Task();
                    }
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPACTION_2)	//如果AT指令为读取BIN文件长度
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset (Bin_len,0x00,sizeof(Bin_len));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "200,", "\r\n", Bin_len);
                    printf("Bin_len=%s\r\n",Bin_len);
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPREAD_2)	//如果AT指令为下载BIN文件
                {

                    int len = 0;
                    long compare_len = 0;

                    memset(Msg_Len,0x00,sizeof(Msg_Len));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "DATA,", "\r\n", Msg_Len);
                    compare_len = atoi(Msg_Len);
                    printf("Find_Buf:%lu\r\n",compare_len);
                    if(Erase_flag == 1)	//仅仅开始是擦除flash一次
                    {
                        Erase_flag = 0;
                        Erase_page(Application_2_Addr, 45); //擦除45页  90K
                    }
                    len = strstr((char *)Lpuart1type.Lpuart1RecBuff, Msg_Len) - (char*)Lpuart1type.Lpuart1RecBuff + strlen(Msg_Len) + 2;
                    printf("offset address is: %d\r\n",len);
									if(compare_len == 1024)
									{
											memset (Bin_len,0x00,sizeof(Bin_len));
											for(long b = 0; b < 1024; b++)
											{
													Bin_buffer[b] = Lpuart1type.Lpuart1RecBuff[len+b];
											}
											/* 接下来将固件写进flash内 */
											printf("烧录第%d包...................\r\n",addr_count);
											WriteFlash((Application_2_Addr+(addr_count)*1024), (uint8_t *)(&Bin_buffer[0]), 1024);
											addr_count++;
											ATNextCmdNum = ATCurrentCmdNum;
											CAT1_TaskStatus = CAT1_SEND;
									}
									else if(compare_len < 1024)
									{
											memset (Bin_len,0x00,sizeof(Bin_len));
											for(int b = 0; b < 1024; b++)
											{
													Bin_buffer[b] = Lpuart1type.Lpuart1RecBuff[len+b];
											}
											/* 接下来将固件写进flash内  */

											printf("烧录第%d包...................\r\n",addr_count);
											WriteFlash((Application_2_Addr+(addr_count)*1024), (uint8_t *)(&Bin_buffer[0]), compare_len);
											addr_count = 0;
											Erase_flag =1;
											reboot_flag = 1;
											ATCurrentCmdNum += 1;
											ATNextCmdNum = ATCurrentCmdNum+1;
											CAT1_TaskStatus = CAT1_SEND;
									}
                    break;
                }
                else if(ATCurrentCmdNum == AT_CMQTTPUB)	//表示发送msg成功
                {
                    CAT1_TaskStatus = CAT1_ACCESS;
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPTERM_1)	//表示查询版本号成功
                {
                    CAT1_TaskStatus = CAT1_ACCESS;
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPTERM_2)	//表示下载BIN文件成功
                {
                    CAT1_TaskStatus = CAT1_ACCESS;
                    break;
                }
                else
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    break;
                }
            }
            else if(CompareTime(&TimeCAT1))//表示发送超时
            {
                printf("TimeOut:%s\r\n",Lpuart1type.Lpuart1RecBuff);
                ATCmds[ATRecCmdNum].ATStatus = TIME_OUT;
                if(CurrentRty > 0)
                {
                    CurrentRty--;
                    ATNextCmdNum = ATCurrentCmdNum;
                    CAT1_TaskStatus = CAT1_SEND;
                    break;
                }
                else
                {
                    CAT1_Init();
                    return;
                }
            }
            return;
        case CAT1_ACCESS:	//成功态
            CAT1_TaskStatus = CAT1_IDIE;
            break;
        default:
            return;
        }
    }
}

/* MQTT发送报文 */
void MQTTSendData(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_CSQ_2;
    ATNextCmdNum = AT_CMQTTTOPIC;
}

/* 查询版本号任务 */
void Get_Version(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPINIT_1;
    ATNextCmdNum = AT_HTTPPARA_VERSION;
}

/* 升级任务 */
void Update_Task(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPINIT_2;
    ATNextCmdNum = AT_HTTPPARA_DATA;
}

/* 下载BIN文件 */
void Download_BIN(void)
{
    //char BIN_buffer[1025];
    //char Bin_len[10];
    //Bin_len=12624
    //memcpy(Lpuart1type.Lpuart1RecBuff,Lpuart1type.Lpuart1DMARecBuff,Lpuart1type.Lpuart1DMARecLen);
//	int i =0;
//	int Msg_Len =0;
//	Msg_Len = atoi(Bin_len);
//	i = Msg_Len/1024;
//	if(Msg_Len%1024 == 0)
//	{
//
//	}
//



}
