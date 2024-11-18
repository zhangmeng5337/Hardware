#include "cat1.h"
#include "config.h"
#include "mqtt_analy.h"
#include "lte_hal.h"
#include "string.h"
extern UART_HandleTypeDef huart1;
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
unsigned char  Bin_buffer[1026] = {0};
char Bin_len[10];
char Msg_Len[10];
char topic_buffer[50];
char message_buffer[150];
char send_buffer[200];

int addr_count = 0;
int reboot_flag = 0;
int Erase_flag = 1;
/*入网AT指令�?/
/*   发送指令内�?   返回内容       超时时间�?   发三次超�?
   {"AT\r\n",       "OK", 1000, NO_REC, 3}*/

/*
{
    "设备ID": "47",
    "运行数据": {
        "出水温度": "value",
        "回水温度": "value",
        "泵前压力": "value",
        "泵后压力": "value",
        "伴热带状�?: "value"
    },
    "设备参数": {
        "版本�?: 255,
        "故障": 255,
        "设置出水温度": "value",
        "设置室内温度": "value",
        "数据上传周期": "value"
    }
}

//设置参数
{
    "设备ID": "47",
    "设备控制": {
        "升级": 1,
        "重启": 1,
        "机组开关机": 0,
        "设置出水温度": "value",
        "设置室内温度": "value",
        "数据上传周期": "value"
    }
}


*/
tsATCmds ATCmds[] =
{
    /* 下面是关于连接MQTT的指令集*/
    {"AT\r\n", "OK", 1000, NO_REC, 3},															//AT指令测试
    {"ATE0\r\n", "OK", 1000, NO_REC, 3},														//关闭回显
    {"AT+CPIN?\r\n", "READY", 1000, NO_REC, 3},
    {"AT+CGSN\r\n", "OK", 2000, NO_REC, 3},													//查询Imei
    {"AT+CCID\r\n", "OK", 1000, NO_REC, 3},													//查询ICCID											//查询当前GPRS注册状�?
    {"AT+CGATT?\r\n", "+CGATT: 1", 2000, NO_REC, 3},//查询当前GPRS附着状�?
    
    //mqtt 订阅订阅主题
    {"AT+MCONFIG=", "OK", 1000, NO_REC, 3},
    {"AT+MIPSTART=", "CONNECT OK", 2000, NO_REC, 3},
    {"AT+AT+MSUBMCONNECT=1,300\r\n", "OK", 1000, NO_REC, 3},//客户端向服务器请求会话连�?
    {"=", "SUBACK", 2000, NO_REC, 3},//订阅消息


    /* 下面是关于查询版本号的AT指令�?/
    {"AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", 2000, NO_REC, 3},
    {"AT+SAPBR=3,1,\"APN\",\"\"", "OK", 2000, NO_REC, 3},
    {"AT+SAPBR=1,1", "OK", 2000, NO_REC, 3},
    {"AT+HTTPINIT\r\n", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"CID\",1", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"URL\",", "OK", 2000, NO_REC, 3},   //设置HTTP参数
    {"AT+HTTPACTION=0\r\n", "OK", 2000, NO_REC, 3},				//操作HTTP方法
    {"AT+HTTPREAD\r\n", "+HTTPREAD:", 2000, NO_REC, 3},			//读取HTTP服务回复

    /* 下面是关于下载BIN文件的AT指令�?/
    {"AT+HTTPINIT\r\n", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"CID\",1", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"URL\",", "OK", 2000, NO_REC, 3},   //设置HTTP参数
    {"AT+HTTPACTION=0\r\n", "+OK", 2000, NO_REC, 3},				//操作HTTP方法
    {"AT+HTTPREAD\r\n", "+HTTPREAD: ", 2000, NO_REC, 3},			//读取HTTP服务回复
    {"AT+HTTPTERM\r\n", "OK", 2000, NO_REC, 3},

    /* 下面是关于post的AT指令�?/
    {"AT+HTTPINIT\r\n", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"CID\",1", "OK", 2000, NO_REC, 3},
    {"AT+HTTPPARA=\"URL\",", "OK", 2000, NO_REC, 3},   //设置HTTP参数
    {"AT+HTTPDATA=16,10000", "DOWNLOAD", 2000, NO_REC, 3},
    {"data crc error\r\n", "OK", 2000, SUCCESS_REC, 3},
    {"AT+HTTPACTION=1\r\n", "OK", 2000, NO_REC, 3},				//操作HTTP方法
    {"AT+HTTPTERM\r\n", "OK", 2000, NO_REC, 3},

    /*mqtt返回信息解析*/
    {"", "+MSUB:", 1000, NO_REC, 3},
    {"AT+MPUB=", "OK", 1000, NO_REC, 3}, //发布消息*/
};

unsigned char *mqtt_send_p;
/* AT指令发送处理逻辑 */
void ATSend(teATCmdNum ATCmdNum, unsigned char mode)
{
    //清空接收缓存�?
    if (mode == 0)
    {
        memset(Lpuart1type.Lpuart1RecBuff, 0, LPUART1_REC_SIZE);
        ATCmds[ATCmdNum].ATStatus = NO_REC;
        ATRecCmdNum = ATCmdNum;
        printf("\r\n当前命令码：%d\r\n", ATCmdNum);
        /* 设置topic的长�?*/
        if (ATCmdNum == AT_MCONFIG)
        {
            memset(get_config()->user_id, 0x00, sizeof(get_config()->user_id));
            memset(get_config()->user, 0x00, sizeof(get_config()->user));
            memset(get_config()->password, 0x00, sizeof(get_config()->password));

            sprintf(get_config()->user_id, "%s,", Imei_buffer);//user id
            //sprintf(get_config()->user_id, "%s,", Imei_buffer);//user id
            sprintf(send_buffer, "%s%s,%s,%s\r\n", ATCmds[ATCmdNum].ATSendStr,
                    get_config()->user_id, get_config()->user, get_config()->password);
            HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
            memset(send_buffer, 0x00, strlen(send_buffer));
        }
        else if (ATCmdNum == AT_MIPSTART)
        {
            sprintf(send_buffer, "%s%s,%s\r\n", ATCmds[ATCmdNum].ATSendStr,
                    get_config()->mqtt_ip, get_config()->mqtt_port);
            HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
            memset(send_buffer, 0x00, strlen(send_buffer));
        }
        else if (ATCmdNum == AT_MSUB)//订阅消息
        {
            sprintf(send_buffer, "%s%s,%d\r\n", ATCmds[ATCmdNum].ATSendStr,
                    get_config()->mqtt_msubtopic, 1);
            HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
            memset(send_buffer, 0x00, strlen(send_buffer));
        }
        else if (ATCmdNum == AT_MPUB)//发布阅消�?
        {
            sprintf(send_buffer, "%s%s,%d,%d,%s\r\n", ATCmds[ATCmdNum].ATSendStr,
                    get_config()->mqtt_subtopic, 1,0,mqtt_send_p);
            HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
            memset(send_buffer, 0x00, strlen(send_buffer));
        }	
        else if (ATCmdNum == AT_HTTPPARA_2||ATCmdNum == AT_HTTPPARA_4||
			     ATCmdNum == AT_HTTPPARA_6)//发布阅消�?AT_HTTPPARA_2
        {
            sprintf(send_buffer, "%s%s\r\n", ATCmds[ATCmdNum].ATSendStr,
                    get_config()->http_ip);
            HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
            memset(send_buffer, 0x00, strlen(send_buffer));
        }			
        else
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)ATCmds[ATCmdNum].ATSendStr, strlen(ATCmds[ATCmdNum].ATSendStr), 0xff);
            printf("send:%s", ATCmds[ATCmdNum].ATSendStr);
        }
        //打开超时定时�?
        SetTime(&TimeCAT1, ATCmds[ATCmdNum].TimeOut);
    }




}

/* AT指令接收处理逻辑 */
void ATRec(void)
{
    if (Lpuart1type.Lpuart1RecFlag)
    {
        if (strstr((const char *)Lpuart1type.Lpuart1RecBuff, ATCmds[ATRecCmdNum].ATRecStr) != NULL)
        {
            ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;
        }
		else 
        {
            ATCmds[ATRecCmdNum].ATStatus = ERROR_STATUS;;
        }
        //printf("收到数据�?s", Lpuart1type.Lpuart1RecBuff);
        Lpuart1type.Lpuart1RecFlag = 0;
        Lpuart1type.Lpuart1RecLen = 0;
    }
}



/*4G开�?/
void Start_4G(void)
{
    lte_init();

}

/* 初始化CAT1 */
void CAT1_Init(void)
{
    Start_4G();
	  HAL_Delay(5000);
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT;
    ATNextCmdNum = ATE0;
}


/* AT指令交互逻辑 */
void CAT1_Task(void)
{
    while (1)
    {
        switch (CAT1_TaskStatus)
        {
            case CAT1_IDIE: //空闲�?
                break;
            case CAT1_SEND://发送命�?
                if (ATCurrentCmdNum != ATNextCmdNum)
                {
                    CurrentRty = ATCmds[ATCurrentCmdNum].RtyNum;
                }
                ATSend(ATCurrentCmdNum, 0);
                CAT1_TaskStatus = CAT1_WAIT;

                break;
            case CAT1_WAIT:	//等待态，等待CAT1返回的信�?
                ATRec();	//调用接收函数
                if (ATCmds[ATRecCmdNum].ATStatus == SUCCESS_REC)
                {
                    printf("当前码：%d\r\n", ATCurrentCmdNum);
                    if (ATCurrentCmdNum == AT_CGSN)	//如果AT指令为查询IMEI
                    {
                        ATCurrentCmdNum += 1;
                        ATNextCmdNum = ATCurrentCmdNum + 1;
                        CAT1_TaskStatus = CAT1_SEND;
                        Find_string((char *)Lpuart1type.Lpuart1RecBuff, "\r\n", "\r\n", Imei_buffer);
                        //printf("Imei_buffer=%s\r\n", Imei_buffer);
                        break;
                    }

                    else if (ATCurrentCmdNum == AT_HTTPREAD_1)	//如果AT指令为查询版本号
                    {
                        ATCurrentCmdNum += 1;
                        ATNextCmdNum = ATCurrentCmdNum + 1;
                        CAT1_TaskStatus = CAT1_SEND;
                        memset(Version_buffer, 0x00, sizeof(Version_buffer));
                        Find_string((char *)Lpuart1type.Lpuart1RecBuff, "{", "}", Version_buffer);
                        printf("Version_buffer=%s\r\n", Version_buffer);
                        if (strcmp(Version_buffer, get_config()->version) == 0)
                        {
                            printf("硬件版本和云端版本一致，无需升级！\r\n");
                            ATCurrentCmdNum = AT_MPUB_RECV;
                            ATNextCmdNum = AT_MPUB;
                            CAT1_TaskStatus = CAT1_MQTT_REC;
                        }
                        else
                        {
                            Update_Task();
                        }
                        break;
                    }
                    else if (ATCurrentCmdNum == AT_HTTPACTION_2)	//如果AT指令为读取BIN文件长度
                    {
                        ATCurrentCmdNum += 1;
                        ATNextCmdNum = ATCurrentCmdNum + 1;
                        CAT1_TaskStatus = CAT1_SEND;
                        memset(Bin_len, 0x00, sizeof(Bin_len));
                        Find_string((char *)Lpuart1type.Lpuart1RecBuff, "200,", "\r\n", Bin_len);
                        printf("Bin_len=%s\r\n", Bin_len);
                        break;
                    }
                    else if (ATCurrentCmdNum == AT_HTTPREAD_2)	//如果AT指令为下载BIN文件
                    {

                        int len = 0;
                        long compare_len = 0;
                        unsigned int crc_re, crc_cal;
                        memset(Msg_Len, 0x00, sizeof(Msg_Len));
                        Find_string((char *)Lpuart1type.Lpuart1RecBuff, "DATA,", "\r\n", Msg_Len);
                        compare_len = atoi(Msg_Len) - 2;
                        crc_re = Lpuart1type.Lpuart1RecBuff[compare_len - 2] << 8;
                        crc_re = crc_re | Lpuart1type.Lpuart1RecBuff[compare_len - 1];
                        crc_cal = CRC_Compute(Lpuart1type.Lpuart1RecBuff, compare_len - 2);
                        if (crc_cal != crc_re)
                        {
                            ATCurrentCmdNum = AT_HTTPINIT_3;
                            ATNextCmdNum = ATCurrentCmdNum + 1;
                            CAT1_TaskStatus = CAT1_SEND;
                            return ;
                        }

                        printf("Find_Buf:%lu\r\n", compare_len);
                        if (Erase_flag == 1)	//仅仅开始是擦除flash一�?
                        {
                            Erase_flag = 0;
													Erase_page(Application_2_Addr, 2); //擦除2扇区
                        }
                        len = strstr((char *)Lpuart1type.Lpuart1RecBuff, Msg_Len) - (char *)Lpuart1type.Lpuart1RecBuff + strlen(Msg_Len) + 2;
                        printf("offset address is: %d\r\n", len);

                        if (compare_len == 1024)
                        {
                            memset(Bin_len, 0x00, sizeof(Bin_len));
                            for (long b = 0; b < 1024; b++)
                            {
                                Bin_buffer[b] = Lpuart1type.Lpuart1RecBuff[len + b];
                            }
                            /* 接下来将固件写进flash�?*/
                            printf("烧录�?d�?..................\r\n", addr_count);
                            WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), 1024);
                            addr_count++;
                            ATNextCmdNum = ATCurrentCmdNum;
                            CAT1_TaskStatus = CAT1_SEND;
                        }
                        else if (compare_len < 1024)
                        {
                            memset(Bin_len, 0x00, sizeof(Bin_len));
                            for (int b = 0; b < 1024; b++)
                            {
                                Bin_buffer[b] = Lpuart1type.Lpuart1RecBuff[len + b];
                            }
                            /* 接下来将固件写进flash�? */

                            printf("烧录�?d�?..................\r\n", addr_count);
                            WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), compare_len);
                            addr_count = 0;
                            Erase_flag = 1;
                            reboot_flag = 1;
                            ATCurrentCmdNum += 1;
                            ATNextCmdNum = ATCurrentCmdNum + 1;
                            CAT1_TaskStatus = CAT1_SEND;
							get_config()->update_setting = 1;
                            sprintf(get_config()->version, "%s,", Version_buffer);//user id
                        }
                        break;
                    }
                    else if (ATCurrentCmdNum == AT_HTTPREAD_POST) //上一包数据错误，重发
                    {
                        ATCurrentCmdNum = AT_HTTPINIT_2;
                        ATNextCmdNum = ATCurrentCmdNum + 1;
                        CAT1_TaskStatus = CAT1_SEND;
                    }
                    else if (ATCurrentCmdNum == AT_HTTPTERM_1)	//表示烧录完成
                    {
                        CAT1_TaskStatus = CAT1_ACCESS;
                        // HAL_NVIC_SystemReset();
                        break;
                    }
                    else if (ATCurrentCmdNum == AT_MPUB)	//表示发送完�?
                    {
						ATCurrentCmdNum = AT_MPUB_RECV;
						ATNextCmdNum = AT_MPUB;
						CAT1_TaskStatus = CAT1_MQTT_REC;

                        break;
                    }
                    else if (ATCurrentCmdNum == AT_MSUB)	//表示订阅完成
                    {
					   	   if(ATCmds[ATRecCmdNum].mqtt_topic_count>0)
					   	   	{
						   ATCurrentCmdNum = ATCurrentCmdNum;
						   //ATNextCmdNum = ATCurrentCmdNum + 1;
						   CAT1_TaskStatus = CAT1_SEND;
						   ATCmds[ATRecCmdNum].mqtt_topic_count--;
						   //get_config()->mqtt_msubtopic = "";
						   break;

						   }
						   else 
						   	{
						   ATCurrentCmdNum = ATCurrentCmdNum+1;
						   ATNextCmdNum = ATCurrentCmdNum + 1;
						   CAT1_TaskStatus = CAT1_SEND;
						   break;

						   }

                    }
                }
                else if (CompareTime(&TimeCAT1)) //表示发送超�?
                {
                    printf("TimeOut:%s\r\n", Lpuart1type.Lpuart1RecBuff);
                    ATCmds[ATRecCmdNum].ATStatus = TIME_OUT;
                    if (CurrentRty > 0)
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
            case CAT1_MQTT_REC:	//不需要升级，循环接受服务器命�?
                ATRec();	//调用接收函数
                if (ATCmds[ATRecCmdNum].ATStatus == SUCCESS_REC)
                {
                    //ATCmds[ATRecCmdNum].ATStatus = NO_REC;
                    if (ATCurrentCmdNum == AT_MPUB_RECV)	//表示收到订阅消息
                    {
                        anlysis_mqtt_recv();
                    }
                }
                else if (ATCmds[ATRecCmdNum].ATStatus == ERROR_STATUS) //异常�?
                {
                     ATCmds[ATRecCmdNum].ATStatus = NO_REC;
                    CAT1_Init();
                    return;
                }
                return;
            case CAT1_ACCESS:	//成功�?
                CAT1_TaskStatus = CAT1_IDIE;
                break;
            default:
                return;
        }
    }
}

/* MQTT发送报�?*/

void MQTTSendData(unsigned char mode,unsigned char *s)
{
    if(mode == 0)
    {
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_MPUB;
    ATNextCmdNum = AT_MPUB_RECV;
	mqtt_send_p = s;
	//ATSend(ATCurrentCmdNum,0);

	}
	else   //订阅消息
	{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_MSUB;
    ATNextCmdNum = AT_MPUB_RECV;
	  //send_buffer = s;
		memcpy(send_buffer,s,strlen(s));
	//ATSend(ATCurrentCmdNum,0);

	}

}

/* 查询版本号任�?*/
void Get_Version(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPREAD_1;
    ATNextCmdNum = AT_HTTPINIT_2;
}

/* 升级任务 */
void Update_Task(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPINIT_2;
    ATNextCmdNum = AT_HTTPPARA_3;
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
