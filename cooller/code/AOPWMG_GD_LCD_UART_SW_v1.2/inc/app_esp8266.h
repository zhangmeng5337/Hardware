/*
*********************************************************************************************************
*
*	ģ������ : APP_ESP8266������Ӧ��
*	�ļ����� : app_esp8266.c
*	��    �� : V1.0
*	˵    �� : ESP8266������Ӧ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 Liujingtao  ��ʽ����
*
*	Copyright (C), 2013-2014, �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd.
*
*********************************************************************************************************
*/
#ifndef __APP_ESP8266_H_
#define __APP_ESP8266_H_
#define ESP_BUF_SIZE  64
typedef struct 
{
	 char User_ESP8266_Apssid[ESP_BUF_SIZE];			   //Ҫ���ӵ��ȵ������
	 char User_ESP8266_ApPwd[ESP_BUF_SIZE] ;	   //Ҫ���ӵ��ȵ����Կ
	 char User_Server_IP[ESP_BUF_SIZE];		//Ҫ���ӵķ������� IP
	 char User_Server_Port[ESP_BUF_SIZE];			  //Ҫ���ӵķ������Ķ˿�
     unsigned char update;
	 unsigned char status;
}esp8266_params_stru;

void Demo_esp8266(void);
void esp8266_process(void);
esp8266_params_stru *GetEspStatus(void);

#endif


