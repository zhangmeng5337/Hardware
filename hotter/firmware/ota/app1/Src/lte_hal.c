/*************笔记****************
1、本lte模块采用huart3(串口3)，然后huart1(串口1)作为调试输出�?
2、CudeMX配置huart3�?
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
3、需要FreeRTOS系统支持，需�?uartext.c"�?uartext.h"
4、本代码末尾有FreeRTOS任务模板。StartGSMTask()
5�?
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
#include "string.h"
#include <stdio.h>
//rs485_stru *lte_t;
tsATCmds     at_cmds;
tsATCmds     at_cmds_ota;

teATCmdNum   at_cmd_num;
teATCmdNum   at_cmd_ota_num;

teATStatus   at_status;
tsLpuart1type *lte_recv;
//char Imei_buffer[16];
//char CSQ_buffer[3];
char Version_buffer[20];
unsigned char  Bin_buffer[1026] = {0};
char Bin_len[10];
unsigned char Msg_Len[10];
uint32_t addr_count = 0;
void Start_4G(void);
/* AT指令接收处理逻辑 */
unsigned char ATRec(char *s)
{
    //CLOSED  +CME ERROR: 767  +CGEV: NW PDN DEACT 1

    if (lte_recv->Lpuart1RecFlag)
    {
        if (strstr((const char *)lte_recv->Lpuart1DMARecBuff, s) != NULL)
        {
            at_cmds.ATStatus = SUCCESS_REC;
        }
        else if (strstr((const char *)lte_recv->Lpuart1DMARecBuff, "CLOSED") != NULL)
        {
            at_cmds.ATStatus = ERROR_STATUS;
        }
        else if (strstr((const char *)lte_recv->Lpuart1DMARecBuff, "+CME ERROR: 767") != NULL)
        {
            at_cmds.ATStatus = ERROR_STATUS;
        }
        else if (strstr((const char *)lte_recv->Lpuart1DMARecBuff, "+CGEV: NW PDN DEACT 1") != NULL)
        {
            at_cmds.ATStatus = ERROR_STATUS;
        }
        else



            //printf("收到数据�?s", Lpuart1type.Lpuart1RecBuff);
            lte_recv->Lpuart1RecFlag = 0;
        // lte_recv->Lpuart1RecLen = 0;
    }
    else
        at_cmds.ATStatus = NO_REC;
    return at_cmds.ATStatus;
}

/*********************************************************
函数名：lte_Check_Cmd
�? 能：发送命令后,检测接收到的应�?
�? 参：str--期待的应答结�?
返回值：0--没有得到期待的应答结�?*?*--期待应答结果的位�?str的位�?
�? 注：
**********************************************************/
uint8_t *lte_Check_Cmd(uint8_t *str)
{
    uint8_t *strx ;
    strx = strstr((const char *)(lte_recv->Lpuart1DMARecBuff), (const char *)str); //寻找文本(被寻找，欲寻�?
    return (uint8_t *)strx;

}


/********************************************
函数名：lte_Send_Cmd
�? 能：向GSM发送命�?
�? 参：cmd:发送的命令字符�?不需要添加回车了)
        ack:期待的应答结�?如果为空,则表示不需要等待应�?
        waittime:等待时间(单位:100ms)
返回值：0--发送成�?得到了期待的应答结果)
        1--发送失�?
�? 注：
*********************************************/
uint8_t lte_Send_Cmd(uint8_t *cmd, uint8_t *ack, unsigned int WaitTime)
{
    uint8_t res = 0;
    uint8_t TxBuffer[1024];
    uint8_t len;


    // tsLpuart1type *lte_recv;
    // lte_recv = get_lte_recv();

    lte_recv->timeout = WaitTime;
    memset(TxBuffer, 0, 1024);
    sprintf((char *)TxBuffer, "%s", cmd);
    // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//发给串口3
    uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));
    //  printf("%s",TxBuffer);
    if (ack && lte_recv->timeout)    //需要等待应�?
    {
        while (--lte_recv->timeout)   //等待倒计�?
        {
            if (lte_recv->Lpuart1RecFlag)
            {
                lte_recv->Lpuart1RecFlag = 0;

                if (lte_Check_Cmd(ack))
                {
                    memcpy(lte_recv->Lpuart1RecBuff, lte_recv->Lpuart1DMARecBuff, lte_recv->Lpuart1DMARecLen);
                    memset(lte_recv->Lpuart1DMARecBuff, 0, LPUART1_DMA_REC_SIZE);
                    break;//得到有效数据
                }
            }

        }

    }
    //  memset(lte_recv->Lpuart1DMARecBuff, 0, LPUART1_DMA_REC_SIZE);
    if (lte_recv->timeout == 0)
    {
        res = 1;
    }
    return res;
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
    at_cmds.RtyNum = 0;
    at_cmd_num = AT;
    at_cmds.net_status = NO_REC;
    lte_recv = get_lte_recv();

}
uint8_t lte_info_ota_show()
{
unsigned char buf[256];
    switch (at_cmd_ota_num)
    {
//***************************************************http
    case AT_HTTPTERM_21:
        //sprintf(buf, "AT+HTTPTERM\\r\n", get_config()->http_download, get_config()->seq_count);//url set
        if (lte_Send_Cmd("AT+CFUN\r\n", "OK", LTE_LONG_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;


        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPTERM_22;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;

    case AT_HTTPTERM_22:
        //sprintf(buf, "AT+HTTPTERM\\r\n", get_config()->http_download, get_config()->seq_count);//url set
        if (lte_Send_Cmd("AT+CIPSHUT\r\n", "OK", LTE_LONG_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;


        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_SAPBR_1;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;

    case AT_SAPBR_1:
        if (lte_Send_Cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", LTE_SHORT_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_SAPBR_2;


        }
        break;
    case AT_SAPBR_2:
        if (lte_Send_Cmd("AT+SAPBR=3,1,\"APN\",\"\"\r\n", "OK", LTE_SHORT_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;
            //  memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_SAPBR_3;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;
    case AT_SAPBR_3:
        if (lte_Send_Cmd("AT+SAPBR=1,1\r\n", "OK", LTE_LONG_DELAY)) //get active PDP quest
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPINIT_1;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;
    case AT_HTTPINIT_1:
        if (lte_Send_Cmd("AT+HTTPINIT\r\n", "OK", LTE_LONG_DELAY)) //init
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;


        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPPARA_1;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;
    case AT_HTTPPARA_1:
        if (lte_Send_Cmd("AT+HTTPPARA=\"CID\",1\r\n", "OK", LTE_LONG_DELAY)) //set http params
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPPARA_2;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;
    case AT_HTTPPARA_2:
        sprintf(buf, "AT+HTTPPARA=\"URL\",%s\r\n", get_config()->http_download);//url set
        if (lte_Send_Cmd(buf, "OK", LTE_LONG_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;


        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPACTION_1;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        break;

    case AT_HTTPACTION_1:
        if (lte_Send_Cmd("AT+HTTPACTION=0\r\n", "+HTTPACTION: 0,200,", LTE_LONG_DELAY)) //start get
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;
            //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

        }
        else
        {
            at_cmds_ota.RtyNum = 0;
            at_cmd_ota_num = AT_HTTPREAD_2;


        }
        break;
    case AT_HTTPREAD_2:
        if (lte_Send_Cmd("AT+HTTPREAD", "+HTTPREAD:", LTE_LONG_DELAY)) //查询AT
        {
            at_cmds_ota.RtyNum = at_cmds_ota.RtyNum++;


        }
        else
        {
            int payload_head_index ;
            uint8_t *tmp;
            long compare_len = 0;
            unsigned int crc_re, crc_cal;




            memset(Msg_Len, 0x00, sizeof(Msg_Len));
            /*
            数据格式�?
            DATA,1024
            1024byte+2(crc)
            */
            Find_string((char *)lte_recv->Lpuart1RecBuff, "DATA,", "\r\n", Msg_Len);
            compare_len = atoi(Msg_Len);
            payload_head_index= strrindex(lte_recv->Lpuart1RecBuff, Msg_Len);
            payload_head_index = payload_head_index + 2;


            crc_re = lte_recv->Lpuart1RecBuff[payload_head_index+compare_len] << 8;
            crc_re = crc_re | lte_recv->Lpuart1RecBuff[payload_head_index+compare_len - 1];

            crc_cal = crc16(lte_recv->Lpuart1RecBuff[payload_head_index], compare_len);

            if (crc_cal != crc_re)
            {
                at_cmd_ota_num = AT_HTTPREAD_2;
                at_cmds_ota.RtyNum = 0;

                return at_cmds_ota.net_status;
            }
            get_config()->seq_count ++;
            // printf("Find_Buf:%lu\r\n", compare_len);
            if (get_config()->Erase_flag == 1)	  //begin erase仅仅开始是擦除flash一�?
            {
                get_config()->Erase_flag = 0;
                Erase_page(Application_2_Addr, 2); //erase 2 sector擦除2扇区
            }

            //  len_t = strstr((char *)(lte_recv->Lpuart1RecBuff), Msg_Len);
            //  printf("offset address is: %d\r\n", *len_t);

            if (compare_len == 1024)
            {
                memset(Bin_len, 0x00, sizeof(Bin_len));
                for (long b = 0; b < 1024; b++)
                {
                    Bin_buffer[b] = lte_recv->Lpuart1RecBuff[payload_head_index + b];
                }
                /* 接下来将固件写进flash�?*/
                //  printf("烧录�?d�?..................\r\n", addr_count);
                WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), 1024);
                addr_count++;
                at_cmds_ota.RtyNum = 0;
                at_cmd_ota_num = AT_HTTPTERM_21;
            }
            else if (compare_len < 1024)
            {
                memset(Bin_len, 0x00, sizeof(Bin_len));
                for (int b = 0; b < 1024; b++)
                {
                    Bin_buffer[b] = lte_recv->Lpuart1RecBuff[payload_head_index + b];
                }
                /* 接下来将固件写进flash�? */

                //printf("烧录�?d�?..................\r\n", addr_count);
                WriteFlash((Application_2_Addr + (addr_count) * 1024), (uint8_t *)(&Bin_buffer[0]), compare_len);
                addr_count = 0;
                get_config()->Erase_flag = 1;
                get_config()->reboot_flag = 1;
                at_cmds_ota.RtyNum = 0;
                at_cmd_ota_num = AT_IDLE;
                at_cmd_num = AT_IDLE;
                // get_config()->update_setting = 1;
                OTA_Task();
                //sprintf(get_config()->version, "%s,", Version_buffer);//user id
            }

        }
        break;



    case AT_IDLE:
        at_cmds_ota.net_status = NET_CONNECT;

        break;
    	}
	        if (at_cmds_ota.RtyNum >= 5)
        {
            CAT1_Init();
            at_cmds_ota.net_status = TIME_OUT;

        }
        return at_cmds_ota.net_status;
    }
    /*********************************************************
    函数名：lte_Info_Show
    �? 能：GSM检�?SIM卡准备和是否注册成功)
    �? 参：�?
    返回值：2--正常  其他--错误代码
    �? 注：
    **********************************************************/
    uint8_t lte_Info_Show(void)
    {
        unsigned char buf[256];
        switch (at_cmd_num)
        {
        case AT:
            if (lte_Send_Cmd("AT\r\n", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = ATE0;


            }
            break;
        case ATE0:
            if (lte_Send_Cmd("ATE0\r\n", "OK", LTE_SHORT_DELAY)) //关闭回显
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;


            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CPIN;


            }
            break;
        case AT_CPIN:
            if (lte_Send_Cmd("AT+CPIN?\r\n", "READY", LTE_SHORT_DELAY)) //查询sim�?
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CGSN;


            }
            break;
        case AT_CGSN:
            if (lte_Send_Cmd("AT+CGSN\r\n", "OK", LTE_SHORT_DELAY)) //查询imei
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {

                Find_string((char *)lte_recv->Lpuart1RecBuff, "\r\n", "\r\n", get_config()->user_id);
                sprintf(get_config()->mqtt_mpubtopic,"%s%s", "mqtt_mub_",get_config()->user_id);
                sprintf(get_config()->mqtt_subtopic,"%s%s", "mqtt_sub_",get_config()->user_id);
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CCID;
            }
            break;
        case AT_CCID:
            if (lte_Send_Cmd("AT+CCID\r\n", "OK", LTE_SHORT_DELAY)) //
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_CGATT;


            }
            break;

        case AT_CGATT:
            if (lte_Send_Cmd("AT+CGATT?\r\n", "+CGATT: 1", LTE_LONG_DELAY)) //查询网络附着
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                // at_cmd_num = AT_SAPBR_1;
                at_cmd_num = AT_SAPBR_1;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

                //at_cmds.net_status = NET_CONNECT;
                // http_info_show();

            }
            break;
        //***************************************************http
        case AT_SAPBR_1:
            if (lte_Send_Cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_SAPBR_2;


            }
            break;
        case AT_SAPBR_2:
            if (lte_Send_Cmd("AT+SAPBR=3,1,\"APN\",\"\"\r\n", "OK", LTE_SHORT_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //  memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_SAPBR_3;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_SAPBR_3:
            if (lte_Send_Cmd("AT+SAPBR=1,1\r\n", "OK", LTE_LONG_DELAY)) //get active PDP quest
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPINIT_1;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPINIT_1:
            if (lte_Send_Cmd("AT+HTTPINIT\r\n", "OK", LTE_LONG_DELAY)) //init
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;


            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_1;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_1:
            if (lte_Send_Cmd("AT+HTTPPARA=\"CID\",1\r\n", "OK", LTE_LONG_DELAY)) //set http params
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPPARA_2;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;
        case AT_HTTPPARA_2:
            sprintf(buf, "AT+HTTPPARA=\"URL\",%s\r\n", get_config()->http_ip);//url set
            if (lte_Send_Cmd(buf, "OK", LTE_LONG_DELAY)) //查询AT
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;


            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPACTION_1;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            break;

        case AT_HTTPACTION_1:
            if (lte_Send_Cmd("AT+HTTPACTION=0\r\n", "+HTTPACTION: 0,200,", LTE_LONG_DELAY)) //start get
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                at_cmds.RtyNum = 0;
                at_cmd_num = AT_HTTPREAD_1;


            }
            break;
        case AT_HTTPREAD_1:
            if (lte_Send_Cmd("AT+HTTPREAD\r\n", "+HTTPREAD:", LTE_LONG_DELAY)) //start get
            {
                at_cmds.RtyNum = at_cmds.RtyNum++;
                //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

            }
            else
            {
                Find_string((char *)lte_recv->Lpuart1RecBuff, "{", ",", Version_buffer);
                if (strcmp(Version_buffer, get_config()->version) == 0)
                {
                    // printf("硬件版本和云端版本一致，无需升级！\r\n");
                    at_cmds.RtyNum = 0;
                    at_cmd_num = AT_IDLE;
                    get_config()->seq_count =1;
                    //memset(lte_recv->Lpuart1RecBuff, 0, sizeof(lte_recv->Lpuart1RecBuff));

                }
                else
                {

                    memcpy(get_config()->version,Version_buffer,strlen(Version_buffer));
                    memset(Version_buffer,0,20);
                    Find_string((char *)lte_recv->Lpuart1RecBuff, ",", ",", Version_buffer);
                    get_config()->seq = atoi(Version_buffer);

                    memset(get_config()->http_download,0,sizeof(get_config()->http_download));
                    Find_string((char *)lte_recv->Lpuart1RecBuff, ",", "}", get_config()->http_download);
                    at_cmds.RtyNum = 0;
                    get_config()->seq_count = 1;
                    at_cmd_num = AT_HTTPTERM_21;//get firmware data
                    at_cmd_ota_num = AT_HTTPTERM_21;
                    at_cmds_ota.net_status =NOT_CONNECT;

                    //get_config()->seq_count ++;

                }

            }
            break;

        case AT_HTTPTERM_21:
            if(lte_info_ota_show() !=NET_CONNECT)
            {

            }
            else
            {
                //at_cmds. =NOT_CONNECT;
                at_cmd_num = AT_IDLE;
                at_cmd_ota_num = AT_HTTPTERM_21;
                //at_cmds.net_status =NOT_CONNECT;
                at_cmds_ota.net_status = NET_CONNECT;

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
        if (at_cmds.RtyNum >= 8)
        {
            CAT1_Init();
            at_cmds.net_status = TIME_OUT;

        }

        return at_cmds.net_status;
    }
//**********************************************************************





    /********************************************
    函数名：lte_CallNum
    �? 能：拨打指定号码
    �? 参：*num--手机号码("18977011111")
    返回值：�?
    �? 注：�?
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
    �? 能：指令不回�?
    �? 参：�?
    返回值：�?
    �? 注：�?
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
    �? 能：设置TEXT文本模式发送英文短�?
    �? 参：*phone--接收短信的号�? *text--短信内容
    返回值：�?
    �? 注：lte_SendEN_SMS(�?0086�?�?23�?
    *********************************************/
    void lte_SendEN_SMS(uint8_t *phone, uint8_t *text)
    {
        uint8_t TxBuffer[32];

        lte_Send_Cmd("AT+CMGF=1", "OK", 10);       //设置文本模式
        lte_Send_Cmd("AT+CSCS=\"GSM\"", "OK", 10); //设置TE字符集为GSM

        sprintf((char *)TxBuffer, "AT+CMGS=\"%s\"\r\n", phone);
        lte_Send_Cmd(TxBuffer, ">", 10);                     //发送短信命�?电话号码
        uart_transmit(LTE_No, TxBuffer, strlen((char *)TxBuffer));

        //UartPutStr(&huart3, text, strlen((char *)text)); //发给串口3,发送短信内容到GSM模块

        delay_us(1000);                                   //必须增加延时,否则接收方接收信息不�?
        if (lte_Send_Cmd("\x1a\r\n", "+CMGS:",
                         100) == 0) //发送结束符,等待发送完�?最长等�?0秒钟,因为短信长了的话,等待时间会长一�?
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

    void Start_4G()
    {
        HAL_GPIO_WritePin(lte_3_8V_EN_GPIO_Port, lte_3_8V_EN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(lte_rst_GPIO_Port, lte_rst_Pin, GPIO_PIN_RESET);
        HAL_Delay(500);
        //HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_SET);
        HAL_Delay(3000);
        HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);

    }


