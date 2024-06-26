#ifndef __PARAMETER_CONFIG_H__
#define __PARAMETER_CONFIG_H__


#define	      SELECT_RTC_TYPE			0		            //RX8130=1;SD2058=2 ;��RTC=0

#define	      UART2_ENABLE          1               //T5L  ����2�����ر����ã�1������0�ر�
#define	      UART3_ENABLE          1               //T5L  ����3�����ر����ã�1������0�ر�
#define	      UART4_ENABLE          1               //T5L  ����4�����ر����ã�1������0�ر�
#define	      UART5_ENABLE          1               //T5L  ����5�����ر����ã�1������0�ر�



#define       BAUD_UART2	        115200          //T5L  ����2����������
#define       BAUD_UART3	        6300          //T5L  ����3����������
#define       BAUD_UART4	        115200          //T5L  ����4����������
#define       BAUD_UART5	        115200         //T5L  ����5����������

#define       RESPONSE_UART2          1               //����2Ӧ��4F4B�����ر����ã�RESPONSE_UART2=1������RESPONSE_UART2=0�ر�
#define       RESPONSE_UART3          0               //����3Ӧ��4F4B�����ر����ã�RESPONSE_UART3=1������RESPONSE_UART3=0�ر�
#define       RESPONSE_UART4          0               //����4Ӧ��4F4B�����ر����ã�RESPONSE_UART4=1������RESPONSE_UART4=0�ر�
#define       RESPONSE_UART5          1               //����5Ӧ��4F4B�����ر����ã�RESPONSE_UART5=1������RESPONSE_UART5=0�ر�

#define       DATA_UPLOAD_UART2          1           //����2�����Զ��ϴ����ã�1�ϴ���0���ϴ�
#define       DATA_UPLOAD_UART3          1           //����3�����Զ��ϴ����ã�1�ϴ���0���ϴ�
#define       DATA_UPLOAD_UART4          1           //����4�����Զ��ϴ����ã�1�ϴ���0���ϴ�
#define       DATA_UPLOAD_UART5          1           //����5�����Զ��ϴ����ã�1�ϴ���0���ϴ�

#define       CRC_CHECK_UART2            0           //����2����CRC�������ã�1����CRC��0�ر�CRC
#define       CRC_CHECK_UART3            0           //����3����CRC�������ã�1����CRC��0�ر�CRC
#define       CRC_CHECK_UART4            0           //����4����CRC�������ã�1����CRC��0�ر�CRC
#define       CRC_CHECK_UART5            0           //����5����CRC�������ã�1����CRC��0�ر�CRC

#define       DTHD1         0X5A			  //֡ͷ1
#define       DTHD2         0XA5			  //֡ͷ2  
#endif