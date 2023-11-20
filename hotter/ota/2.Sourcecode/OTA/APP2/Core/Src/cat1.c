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
/*����ATָ�*/
tsATCmds ATCmds[] =
{
    /* �����ǹ�������MQTT��ָ�*/
    //{"AT+CRESET\r\n","PB DONE",5000,NO_REC,3},
    {"AT\r\n","OK",1000,NO_REC,3},															//ATָ�����
    {"ATE0\r\n","OK",1000,NO_REC,3},														//�رջ���
    {"AT+CGSN\r\n","OK",1000,NO_REC,3},													//��ѯImei
    {"AT+CCID\r\n","OK",1000,NO_REC,3},													//��ѯICCID
    {"AT+CEREG?\r\n","OK",1000,NO_REC,100},											//��ѯ��ǰGPRSע��״̬
    {"AT+CGATT?\r\n","OK",1000,NO_REC,3},												//��ѯ��ǰGPRS����״̬
    {"AT+CSQ\r\n","OK",1000,NO_REC,3},													//��ѯ�ź�ֵ
    {"AT+CMQTTSTART\r\n","+CMQTTSTART: 0",2000,NO_REC,3},				//����MQTT����
    {"AT+CMQTTACCQ=0,\"clientid01\"\r\n","OK",2000,NO_REC,3},		//ע��MQTT��ClientID
    {"AT+CMQTTCONNECT=0,\"tcp://47.98.183.97:1884\",120,1,\"admin\",\"public\"\r\n","+CMQTTCONNECT: 0,0",3000,NO_REC,3},	//����IP port username password keepalive
    {"AT+CSQ\r\n","OK",1000,NO_REC,3},													//��ѯCSQ
    {"AT+CMQTTTOPIC=0,",">",2000,NO_REC,3},											//����topic
    {topic_buffer,"OK",3000,NO_REC,3},													//topic����
    {"AT+CMQTTPAYLOAD=0,",">",2000,NO_REC,3},										//���÷��ͱ���
    {message_buffer,"OK",3000,NO_REC,5},												//msg����
    {"AT+CMQTTPUB=0,1,60\r\n","+CMQTTPUB: 0,0",3000,NO_REC,5},	//���ͱ���
    /* �����ǹ��ڲ�ѯ�汾�ŵ�ATָ�*/
    {"AT+HTTPINIT\r\n","OK",1000,NO_REC,3},											//����HTTP����
    {"AT+HTTPPARA=\"URL\",\"http://47.98.248.24:8888/getVersion?device_id=123456\"\r\n","OK",1000,NO_REC,3},	//����HTTP����
    {"AT+HTTPACTION=0\r\n","+HTTP_PEER_CLOSED",1000,NO_REC,3},				//����HTTP����
    {"AT+HTTPREAD=0,100\r\n","+HTTPREAD:0",1000,NO_REC,3},			//��ȡHTTP����ظ�
    {"AT+HTTPTERM\r\n","OK",1000,NO_REC,3},											//�ر�HTTP����
    /* �����ǹ�������BIN�ļ���ATָ�*/
    {"AT+HTTPINIT\r\n","OK",1000,NO_REC,3},											//����HTTP����
    {"AT+HTTPPARA=\"URL\",\"http://47.98.248.24:8888/download1?device_id=123456&version=2.0.0\"\r\n","OK",3000,NO_REC,3},	//����HTTP����
    {"AT+HTTPACTION=0\r\n","+HTTPACTION:",3000,NO_REC,3},				//����HTTP����
    {"AT+HTTPREAD=0,1024\r\n","+HTTPREAD:",3000,NO_REC,3},			//��ȡHTTP����ظ�
    {"AT+HTTPTERM\r\n","OK",1000,NO_REC,3},											//�ر�HTTP����
};

/* ATָ��ʹ����߼� */
void ATSend(teATCmdNum ATCmdNum)
{
    //��ս��ջ�����
    memset(Lpuart1type.Lpuart1RecBuff,0,LPUART1_REC_SIZE);
    ATCmds[ATCmdNum].ATStatus = NO_REC;
    ATRecCmdNum = ATCmdNum;
    printf("\r\n��ǰ�����룺%d\r\n",ATCmdNum);
    /* ����topic�ĳ��� */
    if(ATCmdNum == AT_CMQTTTOPIC)
    {
        memset(topic_buffer,0x00,sizeof(topic_buffer));
        sprintf(topic_buffer,"dev/%s",Imei_buffer);
        sprintf(send_buffer,"%s%d\r\n",ATCmds[ATCmdNum].ATSendStr,strlen(topic_buffer));
        HAL_UART_Transmit(&hlpuart1,(uint8_t*)send_buffer,strlen(send_buffer),0xFF);
        printf("send_buffer:%s\r\n",send_buffer);
        memset(send_buffer,0x00,strlen(send_buffer));
    }
    /* ����msg�ĳ��� */
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
    //�򿪳�ʱ��ʱ��
    SetTime(&TimeCAT1,ATCmds[ATCmdNum].TimeOut);

}

/* ATָ����մ����߼� */
void ATRec(void)
{
    if(Lpuart1type.Lpuart1RecFlag)
    {
        if(strstr((const char*)Lpuart1type.Lpuart1RecBuff,ATCmds[ATRecCmdNum].ATRecStr) != NULL)
        {
            ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;
        }
        printf("�յ����ݣ�%s",Lpuart1type.Lpuart1RecBuff);
        Lpuart1type.Lpuart1RecFlag = 0;
        Lpuart1type.Lpuart1RecLen = 0;
    }
}
/* ��ʼ��CAT1 */
void CAT1_Init(void)
{
    Start_4G();
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT;
    ATNextCmdNum = ATE0;
}

/* ATָ����߼� */
void CAT1_Task(void)
{
    while(1)
    {
        switch(CAT1_TaskStatus)
        {
        case CAT1_IDIE: //����̬
            return;
        case CAT1_SEND:
            if(ATCurrentCmdNum !=ATNextCmdNum)
            {
                CurrentRty = ATCmds[ATCurrentCmdNum].RtyNum;
            }
            ATSend(ATCurrentCmdNum);
            CAT1_TaskStatus = CAT1_WAIT;

            return;
        case CAT1_WAIT:	//�ȴ�̬���ȴ�CAT1���ص���Ϣ
            ATRec();	//���ý��պ���
            if(ATCmds[ATRecCmdNum].ATStatus == SUCCESS_REC)
            {
                printf("��ǰ�룺%d\r\n",ATCurrentCmdNum);
                if(ATCurrentCmdNum == AT_CGSN)	//���ATָ��Ϊ��ѯIMEI
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset (CSQ_buffer,0x00,sizeof(CSQ_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "\r\n", "\r\n", Imei_buffer);
                    printf("Imei_buffer=%s\r\n",Imei_buffer);
                    break;
                }
                else if(ATCurrentCmdNum == AT_CSQ_2)	//���ATָ��Ϊ��ѯCSQ
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset(CSQ_buffer,0x00,sizeof(CSQ_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, " ", ",", CSQ_buffer);
                    printf("CSQ_buffer=%s\r\n",CSQ_buffer);
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPREAD_1)	//���ATָ��Ϊ��ѯ�汾��
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset(Version_buffer,0x00,sizeof(Version_buffer));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "{", "}", Version_buffer);
                    printf("Version_buffer=%s\r\n",Version_buffer);
                    if(strcmp(Version_buffer,"\"version\":\"4.0.0\"") == 0)
                    {
                        printf("Ӳ���汾���ƶ˰汾һ�£�����������\r\n");
                    }
                    else
                    {
                        Update_Task();
                    }
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPACTION_2)	//���ATָ��Ϊ��ȡBIN�ļ�����
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    CAT1_TaskStatus = CAT1_SEND;
                    memset (Bin_len,0x00,sizeof(Bin_len));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "200,", "\r\n", Bin_len);
                    printf("Bin_len=%s\r\n",Bin_len);
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPREAD_2)	//���ATָ��Ϊ����BIN�ļ�
                {

                    int len = 0;
                    long compare_len = 0;

                    memset(Msg_Len,0x00,sizeof(Msg_Len));
                    Find_string((char*)Lpuart1type.Lpuart1RecBuff, "DATA,", "\r\n", Msg_Len);
                    compare_len = atoi(Msg_Len);
                    printf("Find_Buf:%lu\r\n",compare_len);
                    if(Erase_flag == 1)	//������ʼ�ǲ���flashһ��
                    {
                        Erase_flag = 0;
                        Erase_page(Application_2_Addr, 45); //����45ҳ  90K
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
											/* ���������̼�д��flash�� */
											printf("��¼��%d��...................\r\n",addr_count);
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
											/* ���������̼�д��flash��  */

											printf("��¼��%d��...................\r\n",addr_count);
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
                else if(ATCurrentCmdNum == AT_CMQTTPUB)	//��ʾ����msg�ɹ�
                {
                    CAT1_TaskStatus = CAT1_ACCESS;
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPTERM_1)	//��ʾ��ѯ�汾�ųɹ�
                {
                    CAT1_TaskStatus = CAT1_ACCESS;
                    break;
                }
                else if(ATCurrentCmdNum == AT_HTTPTERM_2)	//��ʾ����BIN�ļ��ɹ�
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
            else if(CompareTime(&TimeCAT1))//��ʾ���ͳ�ʱ
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
        case CAT1_ACCESS:	//�ɹ�̬
            CAT1_TaskStatus = CAT1_IDIE;
            break;
        default:
            return;
        }
    }
}

/* MQTT���ͱ��� */
void MQTTSendData(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_CSQ_2;
    ATNextCmdNum = AT_CMQTTTOPIC;
}

/* ��ѯ�汾������ */
void Get_Version(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPINIT_1;
    ATNextCmdNum = AT_HTTPPARA_VERSION;
}

/* �������� */
void Update_Task(void)
{
    CAT1_TaskStatus = CAT1_SEND;
    ATCurrentCmdNum = AT_HTTPINIT_2;
    ATNextCmdNum = AT_HTTPPARA_DATA;
}

/* ����BIN�ļ� */
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
