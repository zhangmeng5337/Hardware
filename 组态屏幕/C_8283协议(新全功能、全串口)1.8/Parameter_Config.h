#ifndef __PARAMETER_CONFIG_H__
#define __PARAMETER_CONFIG_H__


#define	      SELECT_RTC_TYPE			0		            //RX8130=1;SD2058=2 ;无RTC=0

#define	      UART2_ENABLE          1               //T5L  串口2开启关闭设置，1开启，0关闭
#define	      UART3_ENABLE          1               //T5L  串口3开启关闭设置，1开启，0关闭
#define	      UART4_ENABLE          1               //T5L  串口4开启关闭设置，1开启，0关闭
#define	      UART5_ENABLE          1               //T5L  串口5开启关闭设置，1开启，0关闭



#define       BAUD_UART2	        115200          //T5L  串口2波特率设置
#define       BAUD_UART3	        6300          //T5L  串口3波特率设置
#define       BAUD_UART4	        115200          //T5L  串口4波特率设置
#define       BAUD_UART5	        115200         //T5L  串口5波特率设置

#define       RESPONSE_UART2          1               //串口2应答4F4B开启关闭设置，RESPONSE_UART2=1开启，RESPONSE_UART2=0关闭
#define       RESPONSE_UART3          0               //串口3应答4F4B开启关闭设置，RESPONSE_UART3=1开启，RESPONSE_UART3=0关闭
#define       RESPONSE_UART4          0               //串口4应答4F4B开启关闭设置，RESPONSE_UART4=1开启，RESPONSE_UART4=0关闭
#define       RESPONSE_UART5          1               //串口5应答4F4B开启关闭设置，RESPONSE_UART5=1开启，RESPONSE_UART5=0关闭

#define       DATA_UPLOAD_UART2          1           //串口2数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART3          1           //串口3数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART4          1           //串口4数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART5          1           //串口5数据自动上传设置，1上传，0不上传

#define       CRC_CHECK_UART2            0           //串口2数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART3            0           //串口3数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART4            0           //串口4数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART5            0           //串口5数据CRC检验设置，1开启CRC，0关闭CRC

#define       DTHD1         0X5A			  //帧头1
#define       DTHD2         0XA5			  //帧头2  
#endif