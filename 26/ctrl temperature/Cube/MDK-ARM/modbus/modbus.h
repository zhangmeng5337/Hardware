#ifndef _modbus_h_
#define _modbus_h_
//#include "stm32f10x.h"
#include "modbus_crc.h"
#include "main.h"

//设备CSMS483宏定义
#define CSMS483 0x01    //设备id
#define CSMS483_READ 0x03 //读命令
#define CSMS483_WRITE 0x06 //写命令
#define CSMS483_FLOW_ADD 0x0010 //流量地址
#define CSMS483_FLOW_SET_ADD 0x0011 //流量设定值地址
#define CSMS483_ID_ADD 0x0033 //读机器号地址
#define CSMS483_READ_SV_ADD 0x0000 //读开关量首地址
#define CSMS483_WRITE_SV_ADD 0x0000 //写开关量首地址

typedef struct 
{
	//作为从机时使用
  uint8_t  myadd;        //本设备从机地址
	uint8_t  rcbuf[100];   //modbus接受缓冲区
	uint8_t  timout;       //modbus数据持续时间
	uint8_t  recount;      //modbus端口接收到的数据个数
	uint8_t  timrun;       //modbus定时器是否计时标志
	uint8_t  reflag;       //modbus一帧数据接受完成标志位
	uint8_t  sendbuf[100]; //modbus接发送缓冲区
	
	//作为主机添加部分
	uint8_t Host_Txbuf[8];	//modbus发送数组	
	uint8_t slave_add;		//要匹配的从机设备地址（做主机实验时使用）
	uint8_t Host_send_flag;//主机设备发送数据完毕标志位
	int Host_Sendtime;//发送完一帧数据后时间计数
	uint8_t Host_time_flag;//发送时间到标志位，=1表示到发送数据时间了
	uint8_t Host_End;//接收数据后处理完毕
}MODBUS;




extern MODBUS modbus;
extern uint16_t Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Modbus_Event(void);



//void Host_send03(void);
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);
void Host_Read03_slave_4(uint8_t slave,uint16_t StartAddr,uint16_t num);
void Host_RX(void);
//主机接收从机的消息进行处理
void HOST_ModbusRX(void);
//void RS485_Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num);



//从机返回数据
void Host_Func6(void);
//向一个寄存器中写数据的参数设置
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t *num);
void Host_write06_slave_4(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t *num);

#endif

