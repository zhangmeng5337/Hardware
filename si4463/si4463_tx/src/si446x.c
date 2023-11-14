/*===========================================================================
Function : Operation for SI446x
http://shop57165217.taobao.com
============================================================================*/
#include "si446x.h"
#include "systemDelay.h"

#include "radio_config_Si446x_1k_26M.h"//26m 晶振
extern SPI_HandleTypeDef hspi1;
const unsigned char config_table_1k[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*read a array of command response*/
void SI446X_READ_RESPONSE(unsigned char *buffer, unsigned char size);

/*wait the device ready to response a command*/
void SI446X_WAIT_CTS(void);

/*write data to TX fifo*/
void SI446X_W_TX_FIFO(unsigned char *txbuffer, unsigned char size);
//void si4463_gpio_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(	SI4463_GPIO_RCC, ENABLE );//A时钟使能 
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_SDN;  	// =SI_SDN   
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(SI4463_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_IRQ;  			// IRQ   
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	
// 	GPIO_Init(SI4463_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = PIN_nSEL;  			// NSEL
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(SI4463_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = PIN_SCK | PIN_MOSI;//MOSI SCK
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(SI4463_PORT, &GPIO_InitStructure);				//初始化GPIOA
//	
//	GPIO_InitStructure.GPIO_Pin=PIN_MISO;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//MISO
//	GPIO_Init(SI4463_PORT,&GPIO_InitStructure);
//}
unsigned char SPI_ExchangeByte(unsigned char data_in)
{
//CPHA=0 CPOL=0 ;注:1.高字节先发送2.要确保片选信号有效时SCK=0
	unsigned char i,ret=0;
	HAL_SPI_TransmitReceive(&hspi1,&data_in,&ret,1,10);
//	for(i=0;i<8;i++)
//	{
//		if(data_in &0x80)
//		{
//			MOSI_HIGH();  
//		}
//		else
//		{
//			MOSI_LOW();
//		}
//		data_in<<=1;
//		ret<<=1;
//		if(READ_SDO)
//		{
//			ret|=1; 
//		}
//		SCK_HIGH(); //上升沿，采样

//		__NOP();
//		SCK_LOW(); //下降沿，锁存
//		__NOP();
//	}
	return ret;
}
/*===========================================================================
SI446X_WAIT_CTS();
Function : wait the device ready to response a command
============================================================================*/
void SI446X_WAIT_CTS(void)
{
    unsigned char cts;
    do
    {
        nSEL_LOW();
        SPI_ExchangeByte(READ_CMD_BUFF);
        cts = SPI_ExchangeByte(0xFF);
        nSEL_HIGH();
    }while(cts != 0xFF);
}

/*===========================================================================
SI446X_CMD();
Function : Send a command to the device
INTPUT   : cmd, the buffer stores the command array
           cmdsize, the size of the command array
============================================================================*/
void SI446X_CMD(unsigned char *cmd, unsigned char cmdsize)
{
    SI446X_WAIT_CTS();
    nSEL_LOW();
    
    while (cmdsize--)   { SPI_ExchangeByte(*cmd++); }
    
    nSEL_HIGH();
}

/*===========================================================================
SI446X_POWER_UP();
Function : Power up the device
INTPUT   : f_xtal, the frequency of the external high-speed crystal
============================================================================*/
void SI446X_POWER_UP(uint32_t f_xtal)
{
    unsigned char cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal>>24;
    cmd[4] = f_xtal>>16;
    cmd[5] = f_xtal>>8;
    cmd[6] = f_xtal;
    SI446X_CMD(cmd, 7);
}
/*===========================================================================
SI446X_READ_RESPONSE();
Function : read a array of command response
INTPUT   : buffer,  a buffer, stores the data responsed
           size,    How many bytes should be read
============================================================================*/
void SI446X_READ_RESPONSE(unsigned char *buffer, unsigned char size)
{
    SI446X_WAIT_CTS();
    nSEL_LOW();
    
	SPI_ExchangeByte(READ_CMD_BUFF);
	while (size--)  { *buffer++ = SPI_ExchangeByte(0xFF); }
    
    nSEL_HIGH();
}

/*===========================================================================
SI446X_NOP();
Function : NO Operation command
============================================================================*/
unsigned char SI446X_NOP(void)
{
    unsigned char cts;
    nSEL_LOW();
    cts = SPI_ExchangeByte(NOP);
    nSEL_HIGH();
	return cts;
}

/*===========================================================================
SI446X_PART_INFO();
Function : Read the PART_INFO of the device, 8 bytes needed
INTPUT   : buffer, the buffer stores the part information
============================================================================*/
void SI446X_PART_INFO(unsigned char *buffer)
{
    unsigned char cmd = PART_INFO;

    SI446X_CMD(&cmd, 1);
    SI446X_READ_RESPONSE(buffer, 8);

}

/*===========================================================================
SI446X_FUNC_INFO();
Function : Read the FUNC_INFO of the device, 7 bytes needed
INTPUT   : buffer, the buffer stores the FUNC information
============================================================================*/
void SI446X_FUNC_INFO(unsigned char *buffer)
{
    unsigned char cmd = FUNC_INFO;

    SI446X_CMD(&cmd, 1);
    SI446X_READ_RESPONSE(buffer, 7);
}

/*===========================================================================
SI446X_INT_STATUS();
Function : Read the INT status of the device, 9 bytes needed
INTPUT   : buffer, the buffer stores the int status
============================================================================*/
void SI446X_INT_STATUS(unsigned char *buffer)
{
    unsigned char cmd[4];
    cmd[0] = GET_INT_STATUS;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;

    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(buffer, 9);

}
void si4463_chip_ID(unsigned char *buffer)
{
    unsigned char cmd[4];
    cmd[0] = PART_INFO;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;

    SI446X_CMD(cmd, 1);
    SI446X_READ_RESPONSE(buffer, 9);
}
/*===========================================================================
SI446X_GET_PROPERTY();
Function : Read the PROPERTY of the device
INTPUT   : buffer, the buffer stores the PROPERTY value
           GROUP_NUM, the group and number of the parameter
           NUM_GROUP, number of the group
============================================================================*/
void SI446X_GET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, 
                           unsigned char *buffer)
{
    unsigned char cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = NUM_PROPS;
    cmd[3] = GROUP_NUM;

    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(buffer, NUM_PROPS + 1);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_X();
Function : Set the PROPERTY of the device
INTPUT   : GROUP_NUM, the group and the number of the parameter
           NUM_GROUP, number of the group
           PAR_BUFF, buffer stores parameters
============================================================================*/
void SI446X_SET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF)
{
    unsigned char cmd[20], i = 0;
    if (NUM_PROPS >= 16)    { return; }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM>>8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while (NUM_PROPS--)     { cmd[i++] = *PAR_BUFF++; }
    SI446X_CMD(cmd, i);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_1();
Function : Set the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
           prioriry,  the value to be set
============================================================================*/
void SI446X_SET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM, unsigned char proirity)
{
    unsigned char cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD(cmd, 5);
}
    
/*===========================================================================
SI446X_GET_PROPERTY_1();
Function : Get the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
OUTPUT   : the PROPERTY value read from device
============================================================================*/
unsigned char SI446X_GET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM)
{
    unsigned char cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(cmd, 2);
    return cmd[1];
}
    
/*===========================================================================
SI446X_RESET();
Function : reset the SI446x device
============================================================================*/
void SI446X_RESET(void)
{
    SDN_HIGH();
    system_delay_ms(10);
    SDN_LOW();
    nSEL_HIGH();
}
    
/*===========================================================================
SI446X_CONFIG_INIT();
Function : configuration the device
============================================================================*/
unsigned char SI446X_CONFIG_INIT(void)
{
    unsigned char i;
    unsigned int j = 0;
    const unsigned char   *ptr = config_table_1k;


    while((i = *(ptr+j)) != 0)
    {
        j += 1;
        SI446X_CMD((unsigned char*)ptr + j, i);
        j += i;
    }
#if SI463_PACKAGE_LEN64 > 0           //fixed packet length
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, SI463_PACKAGE_LEN64);
#else                           //variable packet length
    SI446X_SET_PROPERTY_1(PKT_CONFIG1, 0x00);
    SI446X_SET_PROPERTY_1(PKT_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_LEN_FIELD_SOURCE, 0x01);
    SI446X_SET_PROPERTY_1(PKT_LEN, 0x2A);
    SI446X_SET_PROPERTY_1(PKT_LEN_ADJUST, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, 0x01);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_7_0, 0x10);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CRC_CONFIG, 0x00);
#endif //SI463_PACKAGE_LEN64

    //重要： 4463的GDO2，GDO3控制射频开关，
    //发射时必须： GDO2=1  GDO3=0
    //接收时必须： GDO2=0，GDO3=1
    //所以，以下语句对于4463是必须的，4460或4461不需要射频开关，则可以去掉此语句
    return SI446X_GPIO_CONFIG(35, 24, 33|0x40, 32|0x40, 0, 0, 0);    //重要
}
    
/*===========================================================================
SI446X_W_TX_FIFO();
Function : write data to TX fifo
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
============================================================================*/
void SI446X_W_TX_FIFO(unsigned char *txbuffer, unsigned char size)
{
    nSEL_LOW();
    SPI_ExchangeByte(WRITE_TX_FIFO);
    while (size--)      { SPI_ExchangeByte(*txbuffer++); }
    nSEL_HIGH();
}
    
/*===========================================================================
SI446X_SEND_PACKET();
Function : send a packet
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
           channel, tx channel
           condition, tx condition
============================================================================*/
void SI446X_SEND_PACKET(const unsigned char *txbuffer, unsigned char size, unsigned char channel, unsigned char condition)
{
    unsigned char tx_len = size;

    SI446X_TX_FIFO_RESET();
	SI446X_RX_FIFO_RESET();
    SI446X_CHANGE_STATE(2);
    while(SI446X_GET_DEVICE_STATE() != 2);

	SI446X_WAIT_CTS();
    nSEL_LOW();
    SPI_ExchangeByte(WRITE_TX_FIFO);

    while(size --)    { SPI_ExchangeByte(*txbuffer++); }
    nSEL_HIGH();

	SI446X_START_TX(channel, condition, tx_len);
}
    
/*===========================================================================
SI446X_START_TX();
Function : start TX command
INTPUT   : channel, tx channel
           condition, tx condition
           tx_len, how many bytes to be sent
============================================================================*/
void SI446X_START_TX(unsigned char channel, unsigned char condition, unsigned int tx_len)
{
    unsigned char cmd[6];

    SI446X_CHANGE_STATE(2);
    while (SI446X_GET_DEVICE_STATE() != 2);
    SI446X_CHANGE_STATE(5);
    while (SI446X_GET_DEVICE_STATE() != 5);

    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len>>8;
    cmd[4] = tx_len;
    cmd[5] = 128;
    SI446X_CMD(cmd, 6);
}
    
/*===========================================================================
SI446X_READ_PACKET();
Function : read RX fifo
INTPUT   : buffer, a buffer to store data read
OUTPUT   : received bytes
============================================================================*/
unsigned char SI446X_READ_PACKET(unsigned char *buffer)
{
    unsigned char length, i;
    SI446X_WAIT_CTS();
    nSEL_LOW();

    SPI_ExchangeByte(READ_RX_FIFO);
#if SI463_PACKAGE_LEN64 == 0
    length = SPI_ExchangeByte(0xFF);
#else
    length = SI463_PACKAGE_LEN64;
#endif
    i = length;

    while (length--)    { *buffer++ = SPI_ExchangeByte(0xFF); }
    nSEL_HIGH();
    return i;
}
    
/*===========================================================================
SI446X_START_RX();
Function : start RX state
INTPUT   : channel, receive channel
           condition, receive condition
           rx_len, how many bytes should be read
           n_state1, next state 1
           n_state2, next state 2
           n_state3, next state 3
============================================================================*/
void SI446X_START_RX(unsigned char channel, unsigned char condition, unsigned int rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3)
{
    unsigned char cmd[8];
    SI446X_RX_FIFO_RESET();
    SI446X_TX_FIFO_RESET();

    SI446X_CHANGE_STATE(2);
    while (SI446X_GET_DEVICE_STATE() != 2);
    SI446X_CHANGE_STATE(6);
    while (SI446X_GET_DEVICE_STATE() != 6);
    cmd[0] = START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = rx_len>>8;
    cmd[4] = rx_len;
    cmd[5] = n_state1;
    cmd[6] = n_state2;
    cmd[7] = n_state3;
    SI446X_CMD(cmd, 8);
}
    
/*===========================================================================
SI446X_RX_FIFO_RESET();
Function : reset the RX FIFO of the device
============================================================================*/
void SI446X_RX_FIFO_RESET(void)
{
    unsigned char cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x02;
    SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_TX_FIFO_RESET();
Function : reset the TX FIFO of the device
============================================================================*/
void SI446X_TX_FIFO_RESET(void)
{
    unsigned char cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_PKT_INFO();
Function : read packet information
INTPUT   : buffer, stores the read information
           FIELD, feild mask
           length, the packet length
           diff_len, diffrence packet length
============================================================================*/
void SI446X_PKT_INFO(unsigned char *buffer, unsigned char FIELD, unsigned int length, unsigned int diff_len)
{
    unsigned char cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    SI446X_CMD(cmd, 6);
    SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_FIFO_INFO();
Function : read fifo information
INTPUT   : buffer, stores the read information
============================================================================*/
void SI446X_FIFO_INFO(unsigned char *buffer)
{
    unsigned char cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    SI446X_CMD(cmd, 2);
    SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_GPIO_CONFIG();
Function : config the GPIOs, IRQ, SDO
 Return 1:OK;0:failed;
============================================================================*/
extern unsigned char two_hex_cmp(unsigned char *str1,unsigned char *str2,unsigned char afirstChar,unsigned char bfirstChar,unsigned char length);
unsigned char SI446X_GPIO_CONFIG(unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                        unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG)
{
	unsigned char cmp[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    unsigned char cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = G0;
    cmd[2] = G1;
    cmd[3] = G2;
    cmd[4] = G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    SI446X_CMD(cmd, 8);
    SI446X_READ_RESPONSE(cmd, 8);
//	 	if(two_hex_cmp(cmp,cmd,0,0,8)==1)
// 		{
// 			return 0;
// 		}
	
		
 		return 1;
}
/*===========================================================================
SI446X_CHANGE_STATE();
Function : change a new state
INTPUT   : NewState, input the new state
============================================================================*/
void SI446X_CHANGE_STATE(unsigned char NewState)
{
    unsigned char cmd[2];
    cmd[0] = 0x34;
    cmd[1] = NewState;
    SI446X_CMD(cmd, 2);
}
/*===========================================================================
SI446X_CHIP_STATUS();
Function : Get the chip status
INTPUT   : buffer_8, a buffer to store the read date
============================================================================*/
void SI446X_CHIP_STATUS(unsigned char* buffer_8)
{
    unsigned char cmd[2];
    cmd[0] = GET_CHIP_STATUS;
    cmd[1] = 0xFF;
    SI446X_CMD(cmd, 8);
    SI446X_READ_RESPONSE(buffer_8, 4);
}
/*===========================================================================
SI446X_SET_SYNC();
Function : Set the SYNC bytes of the device
INTPUT   : sync, the SYNC word
============================================================================*/
void SI446X_SET_SYNC(uint32_t sync)
{
    SI446X_SET_PROPERTY_1(SYNC_BITS_31_24, sync>>24);
    SI446X_SET_PROPERTY_1(SYNC_BITS_23_16, sync>>16);
    SI446X_SET_PROPERTY_1(SYNC_BITS_15_8,  sync>>8);
    SI446X_SET_PROPERTY_1(SYNC_BITS_7_0,   sync   );
}
    
/*===========================================================================
SI446X_SET_POWER();
Function : Set the PA Power of the device
INTPUT   : Power_Level,  0- 7F
============================================================================*/
void SI446X_SET_POWER(unsigned char Power_Level)
{
    SI446X_SET_PROPERTY_1(PA_PWR_LVL, Power_Level);
}

unsigned char SI446X_GET_DEVICE_STATE(void)
{
   unsigned char cmd[3];

   cmd[0] = REQUEST_DEVICE_STATE;
   SI446X_CMD(cmd, 1);
   SI446X_READ_RESPONSE(cmd, 3);
   return cmd[1] & 0x0F;
}
//void SI4463_poweron(void)
//{
//	 GPIO_InitTypeDef  GPIO_InitStructure;
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
//	 GPIO_Init(GPIOB, &GPIO_InitStructure);					
//	 GPIO_SetBits(GPIOB,GPIO_Pin_7);						
//}
 /*
 *return:1:初始化成功；0：初始化失败
 */
unsigned char Si4463_init(void)
{
	unsigned char f;
	//si4463_gpio_init();
	SI446X_RESET();         // SI446X 模块复位
	system_delay_ms(20);
	f=SI446X_CONFIG_INIT();   // 寄存器初始化，寄存器来自WDS配置的头文件
	system_delay_ms(10);
	SI446X_SET_POWER(0x7F); // 将输出功率配置为最大
	system_delay_ms(10);
	SI446X_START_RX(0, 0, SI463_PACKAGE_LEN64, 8, 8, 8);  // 进入接收模式
	return f;
}
uint32_t wait;
uint32_t tx_cnt;
void RF_SendPacket(const unsigned char *buf,const unsigned char len)
{
	unsigned char TxBuffer[100];
	__disable_irq();
	SI446X_SEND_PACKET(buf, len, 0, 0);   // 发送数据  
  __enable_irq();
	  wait = 0;
			while(nIRQ)
			wait++;
			
			{
				{
					tx_cnt++;
					__disable_irq();
					SI446X_INT_STATUS(TxBuffer);
					__enable_irq();
				}//while (!(TxBuffer[3] & (1<<5)));               // 等待发射完成
				__disable_irq();
				SI446X_START_RX(0, 0, SI463_PACKAGE_LEN64, 8, 8, 8);  // 进入接收模式  
				__enable_irq();			
			}
	//do

}
////si4463是否已经初始化
//1：已经初始化；0：没有初始化

static unsigned char has_init_si4463=0;
void set_has_init_si4463(unsigned char val)
{
	has_init_si4463=val;
}
unsigned char get_has_init_si4463(void)
{
	return has_init_si4463;
}
/*===========================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
