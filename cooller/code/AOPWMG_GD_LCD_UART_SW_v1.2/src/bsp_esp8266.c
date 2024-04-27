/**
  ******************************************************************************
  * �ļ�����: bsp_esp8266.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: WiFi(ESP8266)�ײ�����ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "bsp_esp8266.h"
#include "string.h"
#if CPU ==ST
extern UART_HandleTypeDef huart2;
#endif
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
static char * itoa( int value, char * string, int radix );

 uint8_t ucTcpClosedFlag=0;

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʼ��ESP8266�õ���GPIO����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */


/**
  * ��������: ���� ESP8266 USART �� NVIC �ж����ȼ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */




/**
  * ��������: ��ʽ�������������C���е�printf��������û���õ�C��
  * �������: USARTx ����ͨ��������ֻ�õ��˴���2����USART2
  *		        Data   Ҫ���͵����ڵ����ݵ�ָ��
  *			      ...    ��������
  * �� �� ֵ: ��
  * ˵    ��������Ӧ�� USART2_printf( USART2, "\r\n this is a demo \r\n" );
  *            		     USART2_printf( USART2, "\r\n %d \r\n", i );
  *            		     USART2_printf( USART2, "\r\n %s \r\n", j );
  */
void USART_printf(UART_HandleTypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	unsigned char tmp;
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':	
					tmp = 0x0d;
					HAL_UART_Transmit(&huart2,&tmp,1,5);//�س���
					Data ++;
				break;
				case 'n':
					tmp = 0x0a;
					HAL_UART_Transmit(&huart2,&tmp,1,5);//���з�	
					Data ++;
				break;
				default:
					Data ++;
				break;
			}			 
		}
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						HAL_UART_Transmit(USARTx,(uint8_t *)s,1,1);
						//while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}				
					Data++;				
				break;
					
				case 'd':			
					//ʮ����
					d = va_arg(ap, int);					
					itoa(d, buf, 10);					
					for (s = buf; *s; s++) 
					{
						//USART_SendData(USARTx,*s);
						HAL_UART_Transmit(USARTx,(uint8_t *)s,1,1);
						//while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}					
					Data++;				
				break;
					
				default:
					Data++;				
				break;				
			}		 
		}		
		else HAL_UART_Transmit(USARTx,(uint8_t *)(Data++),1,1);//USART_SendData(USARTx, *Data++);
		//while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}

/**
  * ��������: ����������ת�����ַ���
  * �������: radix =10 ��ʾ10���ƣ��������Ϊ0
  *           value Ҫת����������
  *           buf ת������ַ���
  *           radix = 10
  * �� �� ֵ: ��
  * ˵    ������USART_printf()����
  */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */




unsigned char res;

/**
  * ��������: ֹͣʹ��ESP8266
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void  ESP8266_stop( void )
{
	ESP8266_RST_LOW_LEVEL();
	#if CPU == ST
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//ʹ�ܿ����ж�
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);	//ʹ�ܿ����ж�    
	//HAL_UART_Receive_IT(&huart2, &res, 1);	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
#endif
  //  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//ʹ�ܿ����ж�
    //HAL_UART_DMAStop(&huart2,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);	
}
void  ESP8266_start( void )
{
	ESP8266_Rst (  );
		#if CPU == ST
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//ʹ�ܿ����ж�
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);	//ʹ�ܿ����ж�    

	HAL_UART_Receive_IT(&huart2, &res, 1);	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
    __HAL_UART_CLEAR_FLAG(&huart2,UART_IT_RXNE);
    __HAL_UART_CLEAR_FLAG(&huart2,UART_IT_IDLE);
 #endif
  //  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//ʹ�ܿ����ж�
    //HAL_UART_DMAStop(&huart2,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);	
}

/**
  * ��������: ����ESP8266ģ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������ESP8266_AT_Test����
  */
void ESP8266_Rst ( void )
{
#if 0
	 ESP8266_Cmd("AT+RST", "OK", "ready", 2500);   	
#else
	#if CPU == ST
	 HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_RESET);
	 HAL_Delay(500); 
	 HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
#endif	
#endif
}

/**
  * ��������: ��ESP8266ģ�鷢��ATָ��
  * �������: cmd�������͵�ָ��
  *           reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
  *           waittime���ȴ���Ӧ��ʱ��
  * �� �� ֵ: 1��ָ��ͳɹ�
  *           0��ָ���ʧ��
  * ˵    ������
  */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	HAL_Delay(waittime);                 //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
	//printf("%s->%s\n",cmd,strEsp8266_Fram_Record .Data_RX_BUF);
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/**
  * ��������: ��ESP8266ģ�����AT��������
  * �������: ��
  * �� �� ֵ: 1��ѡ��ɹ�
  *           0��ѡ��ʧ��
  * ˵    ������
  */
bool ESP8266_AT_Test ( void )
{
	char count=0;
	
	ESP8266_RST_HIGH_LEVEL();	
	//HAL_Delay(1000);
	while(count<3)
	{
		if(ESP8266_Cmd("AT","OK",NULL,5)) return 1;
		ESP8266_Rst();
    HAL_Delay(1);
		++count;
	}
	return 0;
}

/**
  * ��������: ѡ��ESP8266ģ��Ĺ���ģʽ
  * �������: enumMode������ģʽ
  * �� �� ֵ: 1��ѡ��ɹ�
  *           0��ѡ��ʧ��
  * ˵    ������
  */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	bool result=0;
	char count=0;
	while(count<3)
	{
		switch ( enumMode )
		{
			case STA:
				result=ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2 ); 
			break;
			case AP:
				result=ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2 ); 
			break;
			case STA_AP:
				result=ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2 ); 
			break;
			default:
				result=false;
			break;
		}
		if(result) return result;
		++count;
	}
	return 0;
}

/**
  * ��������: ESP8266ģ�������ⲿWiFi
  * �������: pSSID��WiFi�����ַ���
  *           pPassWord��WiFi�����ַ���
  * �� �� ֵ: 1�����ӳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	while(count<3)
	{
		if(ESP8266_Cmd(cCmd,"OK",NULL,5))return 1;
		++count;
	}
	return 0;	
}

/**
  * ��������: ESP8266ģ�鴴��WiFi�ȵ�
  * �������: pSSID��WiFi�����ַ���
  *           pPassWord��WiFi�����ַ���
  *           enunPsdMode��WiFi���ܷ�ʽ�����ַ���
  * �� �� ֵ: 1�������ɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	while(count<3)
	{
		if(ESP8266_Cmd(cCmd,"OK",0,2))return 1;
		++count;
	}
	return 0;	
}

/**
  * ��������: ESP8266ģ������������
  * �������: enumEnUnvarnishTx�������Ƿ������
  * �� �� ֵ: 1�����óɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_Enable_MultipleId ( ControlStatus enumEnUnvarnishTx )
{
	char cStr [20];
	char count=0;
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	while(count<3)
	{
		if(ESP8266_Cmd(cStr,"OK",0,2))return 1;
		++count;
	}
	return 0;		
}

/**
  * ��������: ESP8266ģ�������ⲿ������
  * �������: enumE������Э��
  *           ip��������IP�ַ���
  *           ComNum���������˿��ַ���
  *           id��ģ�����ӷ�������ID
  * �� �� ֵ: 1�����ӳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 3 );
	
}

/**
  * ��������: ESP8266ģ�鿪����رշ�����ģʽ
  * �������: enumMode������/�ر�
  *           pPortNum���������˿ں��ַ���
  *           pTimeOver����������ʱʱ���ַ�������λ����
  * �� �� ֵ: 1�������ɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_StartOrShutServer ( ControlStatus enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 2 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 2 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 2 );
	}
}

/**
  * ��������: ��ȡESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
  * �������: ��
  * �� �� ֵ: 2�����ip
  *           3����������
  *           4��ʧȥ����
  *           0����ȡ״̬ʧ��
  * ˵    ������
  */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 2 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	return 0;
}

/**
  * ��������: ��ȡESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
  * �������: ��
  * �� �� ֵ: �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
  * ˵    ������
  */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 2 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	return ucIdLinkStatus;
}

/**
  * ��������: ��ȡESP8266 �� AP IP
  * �������: pApIp����� AP IP ��������׵�ַ
  *           ucArrayLength����� AP IP ������ĳ���
  * �� �� ֵ: 1����ȡ�ɹ�
  *           0����ȡʧ��
  * ˵    ������
  */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char *pCh;
	
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 2 );
	
	pCh = strstr ( (strEsp8266_Fram_Record .Data_RX_BUF), "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * ��������: ����ESP8266ģ�����͸������
  * �������: ��
  * �� �� ֵ: 1�����óɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_UnvarnishSend ( void )
{
	
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 2 ) )
		return false;
	return  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 2 );
	
}

/**
  * ��������: ����ESP8266ģ���˳�͸��ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void ESP8266_ExitUnvarnishSend ( void )
{
	HAL_Delay(1);
	
	ESP8266_Usart ( "+++" );
	
	HAL_Delay(2);	
}

/**
  * ��������: ESP8266ģ�鷢���ַ���
  * �������: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
  *           pStr��Ҫ���͵��ַ���
  *           ucId���ĸ�ID���͵��ַ���
  *           ulStrLength��Ҫ���͵��ַ������ֽ���
  * �� �� ֵ: 1�����ͳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_SendString ( ControlStatus enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if(enumEnUnvarnishTx )
	{
		ESP8266_Usart ( "%s", pStr );
		
		bRet = true;		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 2 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 2 );
  }
	
	return bRet;

}

/**
  * ��������: ESP8266ģ������ַ���
  * �������: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
  * �� �� ֵ: ���յ����ַ����׵�ַ
  * ˵    ������
  */
char * ESP8266_ReceiveString ( ControlStatus enumEnUnvarnishTx )
{
	char * pRecStr = 0;
		
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	return pRecStr;	
}




void ESP8266_USART_INT_FUN(void)
{
	uint8_t ucCh;
		uint8_t err,result;
	
	#if CPU==ST
	if(HAL_UART_GetError(&huart2)) 
		err=1;//��⵽������֡�����У�����
	else 
		err=0;

	
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)  != RESET )
	{
	   HAL_UART_Receive_IT(&huart2, &res, 1); 	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
		ucCh  = res;
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       	// Ԥ��һ���ֽ�д������
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;

	}
	 	 
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)  == SET)                                         //	����֡�������
	{
        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		HAL_UART_Receive_IT(&huart2, &res, 1); 	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
		ucCh = res;
		ucTcpClosedFlag = strstr(strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  }
	#else
	
	#endif
}


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
