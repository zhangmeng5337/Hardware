#include "modbus_mfc.h"
#include "usart.h"
#include "string.h"

//extern DMA_HandleTypeDef hdma_usart2_tx;

static uint8_t tx_buf[20]={0};	//mfc dma发送数组
//参数1从机，参数2起始地址，参数3寄存器个数
//MFC读函数
void MFC_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num)
{
	//uint8_t tx_buf[8]={0};
		uint16_t crc;//计算的CRC校验位
		memset(tx_buf,0x00,sizeof(tx_buf));
	 tx_buf[0]=slave;//这是要匹配的从机地址
	 tx_buf[1]=0x03;//功能码
	 tx_buf[2]=StartAddr/256;//起始地址高位
	 tx_buf[3]=StartAddr%256;//起始地址低位
	 tx_buf[4]=num/256;//寄存器个数高位
	 tx_buf[5]=num%256;//寄存器个数低位
	 crc=Modbus_CRC16(tx_buf,6); //获取CRC校验位
	 tx_buf[6]=crc/256;//寄存器个数高位
	 tx_buf[7]=crc%256;//寄存器个数低位
	//等待上一次的数据发送完毕
   //while(HAL_DMA_GetState(&hdma_usart2_tx) == HAL_DMA_STATE_BUSY)  osDelay(1);
	 HAL_UART_Transmit_DMA(&huart2, tx_buf, 8);
		//HAL_UART_Transmit(&huart2, tx_buf, sizeof(tx_buf),1000);
	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) == RESET)
   {
       osDelay(1);
   }
}
//参数1从机，参数2起始地址，写入的数据
//MFC写功能函数
void MFC_write06_slave(uint8_t slave,uint16_t StartAddr,uint16_t *num)
{
	uint16_t crc;//计算的CRC校验位
	//static uint8_t tx_buf[8]={0};
		memset(tx_buf,0x00,sizeof(tx_buf));
	tx_buf[0]=slave;//这是要匹配的从机地址
	tx_buf[1]=0x06;//功能码
	tx_buf[2]=StartAddr/256;//起始地址高位
	tx_buf[3]=StartAddr%256;//起始地址低位
//	modbus.Host_Txbuf[4]=num/256;
//	modbus.Host_Txbuf[5]=num%256;
	tx_buf[4]=(*num)/256;
	tx_buf[5]=(*num)%256;
	crc=Modbus_CRC16(tx_buf,6); //获取CRC校验位
	tx_buf[6]=crc/256;//寄存器个数高位
	tx_buf[7]=crc%256;//寄存器个数低位
	//while(HAL_DMA_GetState(&hdma_usart2_tx) == HAL_DMA_STATE_BUSY)  osDelay(1);
	
	HAL_UART_Transmit_DMA(&huart2, tx_buf,8);
	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) == RESET)
   {
       osDelay(1);
   }

}

//MFC写连续寄存器功能函数 //修改数据内容
//参数1从机，参数2起始地址，参数3写入长度 参数4写入的数据
void MFC_write10_slave(uint8_t slave,uint16_t StartAddr,uint16_t reg_len,uint8_t len,uint8_t *num)
{
	uint16_t crc;//计算的CRC校验位
	//uint8_t tx_buf1[20]={0};
	int i;
//	int n = sizeof(num);//求得写入数据长度
//	if(n<len)//写如数据比要写数据小
//	{
//		printf("错误！要求写入%d个数据，实际写了%d\r\n",len,n);
//		return;
//	}
	memset(tx_buf,0x00,sizeof(tx_buf));
	tx_buf[0]=slave;//这是要匹配的从机地址
	tx_buf[1]=0x10;//功能码
	tx_buf[2]=StartAddr/256;//起始地址高位
	tx_buf[3]=StartAddr%256;//起始地址低位
	tx_buf[4]= reg_len/256;//修改字节长度高位
	tx_buf[5]= reg_len%256;//修改字节长度低位
	tx_buf[6]= len;//修改字节长度低位
	for(i=0;i<len;i++)
	{
		tx_buf[i+7] = num[i];
	}
	crc=Modbus_CRC16(tx_buf,len+7); //获取CRC校验位
	tx_buf[len+7]=crc/256;//寄存器个数高位
	tx_buf[len+8]=crc%256;//寄存器个数低位
	//while(HAL_DMA_GetState(&hdma_usart2_tx) == HAL_DMA_STATE_BUSY)  osDelay(1);
	
	HAL_UART_Transmit_DMA(&huart2, tx_buf, len+9);
		//HAL_UART_Transmit(&huart2, tx_buf, len+9,100);
	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) == RESET)
   {
       osDelay(1);
   }
}

//MFC主机读流量
void MFC_Read_flow(uint8_t slave)
{
	MFC_Read03_slave(slave,0x0016,0x0002);
}

//MFC主机读设定流量
void MFC_Read_setflow(uint8_t slave)
{
	MFC_Read03_slave(slave,0x0022,0x0002);
}

//MFC主机设置流量（手册方式2）
void MFC_SET_flow(uint8_t slave,uint8_t Flow[],int buf_size)
{
	int i;
	uint8_t flow[6]={0};
	for(i=0;i<4;i++)
	{
		flow[i+2] = Flow[i];//取数据
	}
	MFC_write10_slave(slave,MFC_COTR_ADD,0x0003,0x06,flow);
}

//MFC主机读累积流量
void MFC_Read_cum_flow(uint8_t slave)
{
	MFC_Read03_slave(slave,MFC_FLOWCUM_32H_HIGH_ADD,0x0009);
}
//MFC主机读流量和累积流量
void MFC_Read_flowandcumflow(uint8_t slave)
{
	MFC_Read03_slave(slave,MFC_FLOW_16_HIGH_ADD,0x0006);
}

//MFC主机清0累积流量
void MFC_RESET_cum_flow(uint8_t slave)
{
	uint8_t flow[8]={0};
	MFC_write10_slave(slave,MFC_FLOWCUM_32H_HIGH_ADD,0x0004,0x08,flow);
}
//MFC零点校准
void MFC_SET_zeropoint(uint8_t slave)
{	
	uint8_t ZERO[2]={0X00,0X01};
	MFC_write06_slave(slave,MFC_SETZERO_ADD,(uint16_t *)ZERO);
}


//MFC回复数据处理
void MFC_deal_rxdata(uint8_t *rxbuf,uint8_t rx_len,MFC_FLOW *mfc)
{	
	MFC_FLOW *m = mfc;
	int i;
	if(rxbuf[0]==0x01 ||	rxbuf[0]==0x02 ||	rxbuf[0]==0x03)
	{
		m->slave_add = rxbuf[0];									//接收设备地址
	}
	else
	{
			return;															//不是目标设备中的三个直接返回
	}
	if(rxbuf[1]==0x03)
	{
		if(rxbuf[2]==0x12)											//读累积流量数据
		{
				m->data_type = rxbuf[2];						//获取数据类型
				m->cum.cum_flow_data[0] = (rxbuf[3] << 24) | (rxbuf[4] << 16) |	(rxbuf[5]<<8) | rxbuf[6];	//接收累积流量高位
				m->cum.cum_flow_data[1] = (rxbuf[7] << 24) | (rxbuf[8] << 16) |	(rxbuf[9]<<8) | rxbuf[10];//接收累积流量低位
			
				m->cum.cum_data_uint = (rxbuf[11]<<8) | rxbuf[12] ;			//接收单位数据
				
				m->cum.data_time_day = (rxbuf[13]<<8) | rxbuf[14];	//接收天数数据

				m->cum.data_time_hour = (rxbuf[15]<<8) | rxbuf[16];	//接收小时数据
	
				m->cum.data_time_min = (rxbuf[17]<<8) | rxbuf[18];	//接收分钟数据
			
				m->cum.data_time_sec = (rxbuf[19]<<8) | rxbuf[20];	//接收秒钟数据
	
		}
		else if(rxbuf[2]==0x04)									//读流量数据
		{
				m->data_type = rxbuf[2];						//获取数据类型
				m->flow_data =  (rxbuf[3] << 24) | (rxbuf[4] << 16) |	(rxbuf[5]<<8) | rxbuf[6];	//接收流量高位			
		}
		else if(rxbuf[2]==0x0C)								//读累积流量和流量
		{
			m->data_type = rxbuf[2];						//获取数据类型
			m->flow_data =  (rxbuf[3] << 24) | (rxbuf[4] << 16) |	(rxbuf[5]<<8) | rxbuf[6];	//接收流量
			m->cum.cum_flow_data[0] = (rxbuf[7] << 24) | (rxbuf[8] << 16) |	(rxbuf[9]<<8) | rxbuf[10];	//接收累积流量高位
			m->cum.cum_flow_data[1] = (rxbuf[11] << 24) | (rxbuf[12] << 16) |	(rxbuf[13]<<8) | rxbuf[14];//接收累积流量低位			
			
		}
		
		
	
	}
	else if(rxbuf[1]==0x06)
	{
		
	}
	else if(rxbuf[1]==0x10)
	{
		
	}
}
