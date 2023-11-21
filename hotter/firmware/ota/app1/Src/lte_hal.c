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
#include "lte.h"
#include <stdlib.h>
#include "uart.h"
#include "sys.h"
#include <string.h>
#include "cat1.h"
#include "mqtt_analy.h"


rs485_stru *lte_t;

/*********************************************************
函数名：lte_Check_Cmd
功  能：发送命令后,检测接收到的应答
形  参：str--期待的应答结果
返回值：0--没有得到期待的应答结果 *?*--期待应答结果的位置(str的位置)
备  注：
**********************************************************/
uint8_t* lte_Check_Cmd(uint8_t *str)
{
    char *strx ;
    strx = strstr((const char*)(lte_t->recv_buf), (const char*)str); //寻找文本(被寻找，欲寻找)
    return (uint8_t*)strx;

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
    uint8_t TxBuffer[32];
    uint8_t len;

	lte_t = get_uart_recv(LTE_No);

    sprintf((char *)TxBuffer, "%s\r\n", cmd);
   // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//发给串口3
    uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));

    if(ack && WaitTime)     //需要等待应答
    {
        while(--WaitTime)    //等待倒计时
        {
            delay_us(100);
            len = lte_t->recv_len; //从串口3读取一次数据
			
            if(len > 1) //接收期待的应答结果
            {
                if(lte_Check_Cmd(ack))
                {
                    break;//得到有效数据
                }
            }
        }
        if(WaitTime == 0)
        {
            res = 1;
        }
    }
    return res;
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
    static uint8_t sim_flag = 0;
    uint8_t TxBuffer[40];

    switch(sim_flag)
    {
        case 0:
            if(lte_Send_Cmd("AT+CPIN?", "OK", 20)) //查询SIM卡是否在位
            {
               // sprintf((char *)TxBuffer, "%d,02,05,Not SIM Crad\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
            }
            else
            {
                sim_flag = sim_flag++;
                //sprintf((char *)TxBuffer, "%d,02,05,SIM Crad Yes\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
            }
            break;
        case 1:
            if(lte_Send_Cmd("AT+CGATT?", "+CGATT: 1", 20)) //查询SIM卡网络是否已注册
            {
               // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
               // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
            }
            else
            {
                sim_flag = sim_flag++;
                //sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
            }
            break;
	case 2:
		if(lte_Send_Cmd("AT+MCONFIG=\"866289037465624\",\"user\",\"password\"", "OK", 20)) //查询SIM卡网络是否已注册
		{
		   // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
		   // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		else
		{
			sim_flag = sim_flag++;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		break;

	case 3:
		if(lte_Send_Cmd("AT+MIPSTART=\"lbsmqtt.airm2m.com\",\"1884\"", "CONNECT OK", 20)) //查询SIM卡网络是否已注册
		{
		   // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
		   // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		else
		{
			sim_flag = sim_flag++;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		break;

	case 4:
		if(lte_Send_Cmd(NULL, "CLOSED", 1)) //查询SIM卡网络是否已注册
		{
		   sim_flag = 5;

		}
		else
		{
			sim_flag = 2;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		break;
	case 5:
		if(lte_Send_Cmd(NULL, "+CGEV: NW PDN DEACT 1", 1)) //查询SIM卡网络是否已注册
		{
		   sim_flag = 4;

		}
		else
		{
			sim_flag = 2;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//发给串口1方便调试
		}
		break;


			
    }
    return sim_flag;
}

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
	  uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));
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
	  uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));
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
	uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));

    //UartPutStr(&huart3, text, strlen((char *)text)); //发给串口3,发送短信内容到GSM模块

    delay_us(1000);                                   //必须增加延时,否则接收方接收信息不全
    if(lte_Send_Cmd("\x1a\r\n", "+CMGS:", 100) == 0) //发送结束符,等待发送完成(最长等待10秒钟,因为短信长了的话,等待时间会长一些)
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
void lte_proc()
{
	CAT1_Task();
	OTA_Task();
	    registerTick(MQTT_TX_TICK_NO,10000);
    if(GetTickResult(MQTT_TX_TICK_NO)==1)//2s 
    {
        reset_registerTick(MQTT_TX_TICK_NO);
		upload();
			
    }

	

}

