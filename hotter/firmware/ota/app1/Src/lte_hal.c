/*************笔记****************
1、本lte模块采用huart3(串口3)，然后huart1(串口1)作为调试输出。
2、CudeMX配置huart3：
   ------------------------------------------
   Mode        --> Asynchronous(异步)
   Baud Rate   --> 9600 Bit/s
   Word Length --> 8 Bit
   Parity      --> None
   Stop Bits   --> 1
   ------------------------------------------
   NVIC        --> 串口中断使能
   ------------------------------------------
   DMA         --> Add 增加RX TX
               --> Data Width --> Byte
   ------------------------------------------
3、需要FreeRTOS系统支持，需要"uartext.c"、"uartext.h"
4、本代码末尾有FreeRTOS任务模板。StartGSMTask()
5、
***********************************/
#include "lte_hal.h"
#include <stdlib.h>
#include "uart.h"
#include "sys.h"
#include <string.h>
//#include "cat1.h"
#include "mqtt_analy.h"
#include "update.h"
#include "config.h"
#include "stmflash.h"

//rs485_stru *lte_t;
tsATCmds     at_cmds;
teATCmdNum   at_cmd_num;
teATStatus   at_status;
tsLpuart1type *lte_recv;
//char Imei_buffer[16];
//char CSQ_buffer[3];
char Version_buffer[20];
unsigned char  Bin_buffer[1026] = {0};
char Bin_len[10];
unsigned char Msg_Len[10];
uint32_t addr_count = 0;
/* AT指令接收处理逻辑 */
unsigned char ATRec(char *s)
{

    if (lte_recv->Lpuart1RecFlag)
    {
        if (strstr((const char *)lte_recv->Lpuart1RecBuff, s) != NULL)
        {
            at_cmds.ATStatus = SUCCESS_REC;
        }
        else
        {
            ;// at_cmds.ATStatus = ERROR_STATUS;;
        }
        //printf("收到数据：%s", Lpuart1type.Lpuart1RecBuff);
        lte_recv->Lpuart1RecFlag = 0;
        // lte_recv->Lpuart1RecLen = 0;
    }
    else
        at_cmds.ATStatus = NO_REC;
    return at_cmds.ATStatus;
}

/*********************************************************
函数名：lte_Check_Cmd
功  能：发送命令后,检测接收到的应答
形  参：str--期待的应答结果
返回值：0--没有得到期待的应答结果 *?*--期待应答结果的位置(str的位置)
备  注：
**********************************************************/
uint8_t *lte_Check_Cmd(uint8_t *str)
{
    uint8_t *strx ;



    strx = strstr((const char *)(lte_recv->Lpuart1RecBuff), (const char *)str); //寻找文本(被寻找，欲寻找)
    return (uint8_t *)strx;

}


/********************************************
函数名：lte_Send_Cmd
功  能：向GSM发送命令
形  参：cmd:发送的命令字符串(不需要添加回车了)
        ack:期待的应答结果,如果为空,则表示不需要等待应答
        waittime:等待时间(单位:100ms)
返回值：0--发送成功(得到了期待的应答结果)
        1--发送失败
备  注：
*********************************************/
uint8_t lte_Send_Cmd(uint8_t *cmd, uint8_t *ack, unsigned int WaitTime)
{
    uint8_t res = 0;
    uint8_t TxBuffer[1024];
    uint8_t len;


    // tsLpuart1type *lte_recv;
    // lte_recv = get_lte_recv();

    lte_recv->timeout = WaitTime;
    memset(TxBuffer, sizeof(TxBuffer), 0);
    sprintf((char *)TxBuffer, "%s\r\n", cmd);
    // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//发给串口3
    uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));

    if (ack && lte_recv->timeout)    //需要等待应答
    {
        while (--lte_recv->timeout)   //等待倒计时
        {
            if (lte_recv->Lpuart1RecFlag)
            {
                // lte_recv->Lpuart1RecLen = 0;
                lte_recv->Lpuart1RecFlag = 0;
                // len = lte_recv->Lpuart1RecLen; //从串口3读取一次数据
                // if (len > 1) //接收期待的应答结果
                {
                    if (lte_Check_Cmd(ack))
                    {
                        //memset(lte_recv->Lpuart1RecBuff,0,sizeof(lte_recv->Lpuart1RecBuff));
                        break;//得到有效数据
                    }
                    //else

                    // memset(lte_recv->Lpuart1RecBuff,0,sizeof(lte_recv->Lpuart1RecBuff));

                }



            }

        }
        memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));


        if (lte_recv->timeout == 0)
        {
            res = 1;
        }
    }
    return res;
}
/*4G开机*/
void Start_4G(void)
{
    lte_init();

}

/* 初始化CAT1 */
void CAT1_Init(void)
{
    Start_4G();
    HAL_Delay(5000);
    at_cmds.RtyNum = 0;
    at_cmd_num = AT;
    at_cmds.net_status = NO_REC;
    lte_recv = get_lte_recv();

}

/*********************************************************
函数名：lte_Info_Show
功  能：GSM检测(SIM卡准备和是否注册成功)
形  参：无
返回值：2--正常  其他--错误代码
备  注：
**********************************************************/
uint8_t lte_Info_Show(void)
{
    unsigned char buf[256];
    switch (at_cmd_num)
    {
        case AT:
            if (lte_Send_Cmd("AT", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = ATE0;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case ATE0:
            if (lte_Send_Cmd("ATE0", "OK", LTE_SHORT_DELAY)) //关闭回显
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CPIN;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_CPIN:
            if (lte_Send_Cmd("AT+CPIN?", "READY", LTE_SHORT_DELAY)) //查询sim卡
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CGSN;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_CGSN:
            if (lte_Send_Cmd("AT+CGSN", "OK", LTE_SHORT_DELAY)) //查询imei
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {

                Find_string((char *)lte_recv->Lpuart1RecBuff, "\r\n", "\r\n", get_config()->user_id);
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CCID;
            }
            break;
        case AT_CCID:
            if (lte_Send_Cmd("AT+CCID", "OK", LTE_SHORT_DELAY)) //
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CGATT;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;

        case AT_CGATT:
            if (lte_Send_Cmd("AT+CGATT?", "+CGATT: 1", LTE_LONG_DELAY)) //查询网络附着
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                // at_cmd_num = AT_SAPBR_1;
                at_cmd_num = AT_IDLE;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

                //at_cmds.net_status = NET_CONNECT;
                // http_info_show();

            }
            break;
        //***************************************************http
        case AT_SAPBR_1:
            if (lte_Send_Cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_SAPBR_2;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_SAPBR_2:
            if (lte_Send_Cmd("AT+SAPBR=3,1,\"APN\",\"\"", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_SAPBR_2;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_SAPBR_3:
            if (lte_Send_Cmd("AT+SAPBR=1,1", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPINIT_1;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPINIT_1:
            if (lte_Send_Cmd("AT+HTTPINIT", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_1;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_1:
            if (lte_Send_Cmd("AT+HTTPPARA=\"CID\",1", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_2;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_2:
            sprintf(buf, "AT+HTTPPARA=\"URL\",%s", get_config()->http_ip);
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPACTION_1;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPACTION_1:
            if (lte_Send_Cmd("AT+HTTPACTION=0", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPREAD_1;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPREAD_1:
            if (lte_Send_Cmd("AT+HTTPREAD", "+HTTPREAD:", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                Find_string((char *)lte_recv->Lpuart1RecBuff, "{", "}", Version_buffer);
                if (strcmp(Version_buffer, get_config()->version) == 0)
                {
                    printf("硬件版本和云端版本一致，无需升级！\r\n");
                    at_cmds.RtyNum = 0;
                    at_cmd_num = AT_IDLE;
                    memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

                }
                else
                {
                    at_cmds.RtyNum = 0;
                    at_cmd_num = AT_HTTPINIT_2;
                    memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

                }

            }
            break;
        //***********************get bin len*************************
        case AT_HTTPINIT_2:
            if (lte_Send_Cmd("AT+HTTPINIT", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_3;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_3:
            if (lte_Send_Cmd("AT+HTTPPARA=\"CID\",1", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_4;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_4:
            sprintf(buf, "AT+HTTPPARA=\"URL\",%s", get_config()->http_ip);
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPACTION_2;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPACTION_2:
            if (lte_Send_Cmd("AT+HTTPACTION=0", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                memset(Bin_len, 0x00, sizeof(Bin_len));
                Find_string((char *)lte_recv->Lpuart1RecBuff, "200,", "\r\n", Bin_len);

                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPREAD_2;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPREAD_2:
            if (lte_Send_Cmd("AT+HTTPREAD", "+HTTPREAD:", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                unsigned char *len_t ;
                long compare_len = 0;
                unsigned int crc_re, crc_cal;
                memset(Msg_Len, 0x00, sizeof(Msg_Len));
                Find_string((char *)lte_recv->Lpuart1RecBuff, "DATA,", "\r\n", Msg_Len);
                compare_len = atoi(Msg_Len) - 2;
				len_t = strstr((lte_recv->Lpuart1RecBuff), Msg_Len);
				len_t =*len_t - (lte_recv->Lpuart1RecBuff);
				
							
                crc_re = lte_recv->Lpuart1RecBuff[*len_t+compare_len - 2] << 8;
                crc_re = crc_re | lte_recv->Lpuart1RecBuff[*len_t+compare_len - 1];
                crc_cal = CRC_Compute(lte_recv->Lpuart1RecBuff, compare_len - 2);
                if (crc_cal != crc_re)
                {
                    at_cmd_num = AT_HTTPINIT_3;
                    at_cmds.RtyNum = 0;

                    return at_cmds.net_status;
                }

                printf("Find_Buf:%lu\r\n", compare_len);
                if (get_config()->Erase_flag == 1)	  //仅仅开始是擦除flash一次
                {
                    get_config()->Erase_flag = 0;
                    Erase_page(Application_2_Addr, 2); //擦除2扇区
                }

                len_t = strstr((char *)(lte_recv->Lpuart1RecBuff), Msg_Len);
                printf("offset address is: %d\r\n", *len_t);

                if (compare_len == 1024)
                {
                    memset(Bin_len, 0x00, sizeof(Bin_len));
                    for (long b = 0; b < 1024; b++)
                    {
                        Bin_buffer[b] = lte_recv->Lpuart1RecBuff[*len_t + b];
                    }
                    /* 接下来将固件写进flash内 */
                    printf("烧录第%d包...................\r\n", addr_count);
                    WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), 1024);
                    addr_count++;
                    at_cmds.RtyNum = 0;
                    at_cmd_num = at_cmd_num;
                }
                else if (compare_len < 1024)
                {
                    memset(Bin_len, 0x00, sizeof(Bin_len));
                    for (int b = 0; b < 1024; b++)
                    {
                        Bin_buffer[b] = lte_recv->Lpuart1RecBuff[*len_t + b];
                    }
                    /* 接下来将固件写进flash内  */

                    printf("烧录第%d包...................\r\n", addr_count);
                    WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), compare_len);
                    addr_count = 0;
                    get_config()->Erase_flag = 1;
                    get_config()->reboot_flag = 1;
                    at_cmds.RtyNum = 0;
                    at_cmd_num = AT_IDLE;
                    // get_config()->update_setting = 1;
                    OTA_Task();
                    //sprintf(get_config()->version, "%s,", Version_buffer);//user id
                }

            }
            break;

        //***********************post data error****************************
        case AT_HTTPINIT_3:
            if (lte_Send_Cmd("AT+HTTPINIT", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_5;
            }
            break;
        case AT_HTTPPARA_5:
            if (lte_Send_Cmd("AT+HTTPPARA=\"CID\",1", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_6;
            }
            break;
        case AT_HTTPPARA_6:
            sprintf(buf, "AT+HTTPPARA=\"URL\",%s", get_config()->http_ip);
            if (lte_Send_Cmd(buf, "OK", 20)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPDATA_1;
            }
            break;
        case AT_HTTPDATA_1:
            if (lte_Send_Cmd("AT+HTTPDATA=16,10000", "DOWNLOAD", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPDATA_2;
            }
            break;
        case AT_HTTPDATA_2:
            if (lte_Send_Cmd("data crc error", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPACTION_2;
            }
            break;

        //***********************************************************
        case AT_IDLE:
            at_cmds.RtyNum = 0;
            at_cmds.net_status = NET_CONNECT;
            break;

defautl:
            {
                CAT1_Init();
            }
            break;


    }
    if (at_cmds.RtyNum >= 3)
    {
        CAT1_Init();
        at_cmds.net_status = TIME_OUT;

    }

    return at_cmds.net_status;
}
//**********************************************************************





/********************************************
函数名：lte_CallNum
功  能：拨打指定号码
形  参：*num--手机号码("18977011111")
返回值：无
备  注：无
*********************************************/
void lte_CallNum(uint8_t *Num)
{
    uint8_t TxBuffer[20];
    sprintf((char *)TxBuffer, "ATD%s;\r\n", Num);
    uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));
    // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//发给串口3
}

/********************************************
函数名：lte_CmdShowOff
功  能：指令不回显
形  参：无
返回值：无
备  注：无
*********************************************/
void lte_CmdShowOff(void)
{
    uint8_t TxBuffer[10];
    sprintf((char *)TxBuffer, "ATE0\r\n");
    uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));
    //UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//发给串口3
}


/********************************************
函数名：lte_SendEN_SMS
功  能：设置TEXT文本模式发送英文短信
形  参：*phone--接收短信的号码  *text--短信内容
返回值：无
备  注：lte_SendEN_SMS(“10086”,“123”)
*********************************************/
void lte_SendEN_SMS(uint8_t *phone, uint8_t *text)
{
    uint8_t TxBuffer[32];

    lte_Send_Cmd("AT+CMGF=1", "OK", 10);       //设置文本模式
    lte_Send_Cmd("AT+CSCS=\"GSM\"", "OK", 10); //设置TE字符集为GSM

    sprintf((char *)TxBuffer, "AT+CMGS=\"%s\"\r\n", phone);
    lte_Send_Cmd(TxBuffer, ">", 10);                     //发送短信命令+电话号码
    uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));

    //UartPutStr(&huart3, text, strlen((char *)text)); //发给串口3,发送短信内容到GSM模块

    delay_us(1000);                                   //必须增加延时,否则接收方接收信息不全
    if (lte_Send_Cmd("\x1a\r\n", "+CMGS:",
                     100) == 0) //发送结束符,等待发送完成(最长等待10秒钟,因为短信长了的话,等待时间会长一些)
    {
        //  sprintf((char *)TxBuffer, "%d,02,05,SMS Send Success!\r\n", Address);
        // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
    }
    else
    {
        //sprintf((char *)TxBuffer, "%d,02,05,SMS Send fail!\r\n", Address);
        //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
    }
}

void lte_init()
{
    HAL_GPIO_WritePin(lte_3_8V_EN_GPIO_Port, lte_3_8V_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lte_rst_GPIO_Port, lte_rst_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    //HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_SET);
    HAL_Delay(3000);
    HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);

}


