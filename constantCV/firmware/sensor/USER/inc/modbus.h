/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_H
#define __SENSOR_H
#include "main.h"
#define MODBUS_RECV_SIZE		128
#define REG_COUNT 64
#define SUPPORT_CMD_SIZE 4

/*modbus**************************************************/
#define MODBUS_FUNC_READ  				0x03
#define MODBUS_FUNC_ONLYREAD  			0x04
#define MODBUS_FUNC_ONE_WRITE	  		0x06
#define MODBUS_FUNC_MUL_WRITE	  		0x10
typedef struct
{
unsigned int Reg;
unsigned char SupportCmd[SUPPORT_CMD_SIZE];//analy request
unsigned char CmdSize;
unsigned int val_uint; 
int val_int;
float f;
unsigned char data_type;//0:unint;1:int  2:float

}modbus_reg_stru;
typedef struct
{
unsigned int cmd_sup;
modbus_reg_stru *pb;//analy request
}modbus_reg_cmd_stru;

typedef struct
{
unsigned char DevAddr;
unsigned char Addr;
unsigned char Func;
unsigned int RegStart;
unsigned int RegEnd;
unsigned int RegCount;
unsigned char len;
unsigned int crc;
unsigned char payload[MODBUS_RECV_SIZE];//analy request
unsigned char request[MODBUS_RECV_SIZE];//decto mcu
}modbus_stru;

typedef struct
{
unsigned int reg;
unsigned int val_uint; 
int val_int;
float f;
unsigned char data_type;//0:unint;1:int  2:float
}reg_dat_type_stru;



void modbus_recv_proc(void);

#endif /*__CW32L010_SPI_H */

