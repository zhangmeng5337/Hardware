/*************�ʼ�****************
1����lteģ�����huart3(����3)��Ȼ��huart1(����1)��Ϊ���������
2��CudeMX����huart3��
   ------------------------------------------
   Mode        --> Asynchronous(�첽)
   Baud Rate   --> 9600 Bit/s
   Word Length --> 8 Bit
   Parity      --> None
   Stop Bits   --> 1
   ------------------------------------------
   NVIC        --> �����ж�ʹ��
   ------------------------------------------
   DMA         --> Add ����RX TX
               --> Data Width --> Byte
   ------------------------------------------
3����ҪFreeRTOSϵͳ֧�֣���Ҫ"uartext.c"��"uartext.h"
4��������ĩβ��FreeRTOS����ģ�塣StartGSMTask()
5��
***********************************/
#include "lte.h"
#include <stdlib.h>
#include "uart.h"
#include "sys.h"
#include <string.h>


rs485_stru *lte_t;

/*********************************************************
��������lte_Check_Cmd
��  �ܣ����������,�����յ���Ӧ��
��  �Σ�str--�ڴ���Ӧ����
����ֵ��0--û�еõ��ڴ���Ӧ���� *?*--�ڴ�Ӧ������λ��(str��λ��)
��  ע��
**********************************************************/
uint8_t* lte_Check_Cmd(uint8_t *str)
{
    char *strx ;
    strx = strstr((const char*)(lte_t->recv_buf), (const char*)str); //Ѱ���ı�(��Ѱ�ң���Ѱ��)
    return (uint8_t*)strx;

}


/********************************************
��������lte_Send_Cmd
��  �ܣ���GSM��������
��  �Σ�cmd:���͵������ַ���(����Ҫ��ӻس���)
        ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
        waittime:�ȴ�ʱ��(��λ:100ms)
����ֵ��0--���ͳɹ�(�õ����ڴ���Ӧ����)
        1--����ʧ��
��  ע��
*********************************************/
uint8_t lte_Send_Cmd(uint8_t *cmd, uint8_t *ack, unsigned int WaitTime)
{
    uint8_t res = 0;
    uint8_t TxBuffer[32];
    uint8_t len;

	lte_t = get_uart_recv(LTE_No);

    sprintf((char *)TxBuffer, "%s\r\n", cmd);
   // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//��������3
    uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));

    if(ack && WaitTime)     //��Ҫ�ȴ�Ӧ��
    {
        while(--WaitTime)    //�ȴ�����ʱ
        {
            delay_us(100);
            len = lte_t->recv_len; //�Ӵ���3��ȡһ������
			
            if(len > 1) //�����ڴ���Ӧ����
            {
                if(lte_Check_Cmd(ack))
                {
                    break;//�õ���Ч����
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
��������lte_Info_Show
��  �ܣ�GSM���(SIM��׼�����Ƿ�ע��ɹ�)
��  �Σ���
����ֵ��2--����  ����--�������
��  ע��
**********************************************************/
uint8_t lte_Info_Show(void)
{
    static uint8_t sim_flag = 0;
    uint8_t TxBuffer[40];

    switch(sim_flag)
    {
        case 0:
            if(lte_Send_Cmd("AT+CPIN?", "OK", 20)) //��ѯSIM���Ƿ���λ
            {
               // sprintf((char *)TxBuffer, "%d,02,05,Not SIM Crad\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
            }
            else
            {
                sim_flag = sim_flag++;
                //sprintf((char *)TxBuffer, "%d,02,05,SIM Crad Yes\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
            }
            break;
        case 1:
            if(lte_Send_Cmd("AT+CGATT?", "+CGATT: 1", 20)) //��ѯSIM�������Ƿ���ע��
            {
               // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
               // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
            }
            else
            {
                sim_flag = sim_flag++;
                //sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
                //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
            }
            break;
	case 2:
		if(lte_Send_Cmd("AT+MCONFIG=\"866289037465624\",\"user\",\"password\"", "OK", 20)) //��ѯSIM�������Ƿ���ע��
		{
		   // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
		   // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		else
		{
			sim_flag = sim_flag++;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		break;

	case 3:
		if(lte_Send_Cmd("AT+MIPSTART=\"lbsmqtt.airm2m.com\",\"1884\"", "CONNECT OK", 20)) //��ѯSIM�������Ƿ���ע��
		{
		   // sprintf((char *)TxBuffer, "%d,02,05,Network Registering!\r\n", Address);
		   // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		else
		{
			sim_flag = sim_flag++;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		break;

	case 4:
		if(lte_Send_Cmd(NULL, "CLOSED", 1)) //��ѯSIM�������Ƿ���ע��
		{
		   sim_flag = 5;

		}
		else
		{
			sim_flag = 2;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		break;
	case 5:
		if(lte_Send_Cmd(NULL, "+CGEV: NW PDN DEACT 1", 1)) //��ѯSIM�������Ƿ���ע��
		{
		   sim_flag = 4;

		}
		else
		{
			sim_flag = 2;
			//sprintf((char *)TxBuffer, "%d,02,05,Network Register Success!\r\n", Address);
			//UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
		}
		break;


			
    }
    return sim_flag;
}

/********************************************
��������lte_CallNum
��  �ܣ�����ָ������
��  �Σ�*num--�ֻ�����("18977011111")
����ֵ����
��  ע����
*********************************************/
void lte_CallNum(uint8_t *Num)
{
    uint8_t TxBuffer[20];
    sprintf((char *)TxBuffer, "ATD%s;\r\n", Num);
	  uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));
   // UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//��������3
}

/********************************************
��������lte_CmdShowOff
��  �ܣ�ָ�����
��  �Σ���
����ֵ����
��  ע����
*********************************************/
void lte_CmdShowOff(void)
{
    uint8_t TxBuffer[10];
    sprintf((char *)TxBuffer, "ATE0\r\n");
	  uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));
    //UartPutStr(&huart3, TxBuffer, strlen((char *)TxBuffer));//��������3
}


/********************************************
��������lte_SendEN_SMS
��  �ܣ�����TEXT�ı�ģʽ����Ӣ�Ķ���
��  �Σ�*phone--���ն��ŵĺ���  *text--��������
����ֵ����
��  ע��lte_SendEN_SMS(��10086��,��123��)
*********************************************/
void lte_SendEN_SMS(uint8_t *phone, uint8_t *text)
{
    uint8_t TxBuffer[32];

    lte_Send_Cmd("AT+CMGF=1", "OK", 10);       //�����ı�ģʽ
    lte_Send_Cmd("AT+CSCS=\"GSM\"", "OK", 10); //����TE�ַ���ΪGSM

    sprintf((char *)TxBuffer, "AT+CMGS=\"%s\"\r\n", phone);
    lte_Send_Cmd(TxBuffer, ">", 10);                     //���Ͷ�������+�绰����
	uart_transmit(LTE_No,TxBuffer ,strlen((char *)TxBuffer));

    //UartPutStr(&huart3, text, strlen((char *)text)); //��������3,���Ͷ������ݵ�GSMģ��

    delay_us(1000);                                   //����������ʱ,������շ�������Ϣ��ȫ
    if(lte_Send_Cmd("\x1a\r\n", "+CMGS:", 100) == 0) //���ͽ�����,�ȴ��������(��ȴ�10����,��Ϊ���ų��˵Ļ�,�ȴ�ʱ��᳤һЩ)
    {
      //  sprintf((char *)TxBuffer, "%d,02,05,SMS Send Success!\r\n", Address);
       // UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
    }
    else
    {
        //sprintf((char *)TxBuffer, "%d,02,05,SMS Send fail!\r\n", Address);
        //UartPutStr(&huart1, TxBuffer, strlen((char *)TxBuffer));//��������1�������
    }
}

void lte_init()
{
	HAL_GPIO_WritePin(lte_3_8V_EN_GPIO_Port, lte_3_8V_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lte_rst_GPIO_Port, lte_rst_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_SET);	
	HAL_Delay(1500);
	HAL_GPIO_WritePin(lte_pwr_ctrl_GPIO_Port, lte_pwr_ctrl_Pin, GPIO_PIN_RESET);

}
void lte_proc()
{

}

