/*
 * @Description: cs1237 设备驱动文件
 * @Author: TOTHTOT
 * @Date: 2023-03-23 16:41:59
 * @LastEditTime: 2023-03-28 13:23:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\CS1237\cs1237.c
 */
#include "cs1237.h"
#include "stdio.h"
#include "main.h"
#define setbit(x, y) x |= (1 << y)     // x的y位置1
#define clrbit(x, y) x &= ~(1 << y)    // x的y位置0
#define reversebit(x, y) x ^= (1 << y) // x的y位异�?#define getbit(x, y) ((x) >> (y)&1)    // 获取x的第y�?

int32_t cs1237_read_data(struct cs1237_device *dev);
uint8_t cs1237_read_config(struct cs1237_device *dev);
int32_t calculate_adc_num(struct cs1237_device *dev);
 
void DATAIN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);
}

void DATAOUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);
}
//��ʱ500US 25MHZ
void delay_us(unsigned char a)
{	
	unsigned char i,j,b;
	for(b=0;b<a;b++)
	{
		i = 26;
		j = 37;
		do
		{
			while (--j);
		} while (--i);
	}
}
/**
 * @name: cs1237_nop
 * @msg: cs1237 模块内部使用延时 调用一�?60ns
 * @return {*} �? * @author: TOTHTOT
 * @date: 2023�?�?4�?1:43:50
 */
// static void cs1237_nop(void)
// {
//     unsigned int i = 0;
//     for (i = 0; i < 20; i++)
//         __asm {
//     nop
//         }
// }

/**
 * @name:cs1237_send_bit
 * @msg:发送一位数�? * @param {uint8_t} bit 1位数�? * @param {uint8_t} time_us 时钟高电平时�? 单位:us
 * @return {*} �? * @author: TOTHTOT
 * @date: 2023�?�?4�?0:34:08
 */
void cs1237_send_bit(uint8_t bit, uint8_t time_us)
{
     DATAOUT(); // 切换输出模式
    CS1237_SCL_H;
    delay_us(time_us);
    if (bit == 1)
        CS1237_SDA_H;
    else
        CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(time_us);
}

/**
 * @name: cs1237_read_bit
 * @msg: cs1237 读取1位数�? 使用前确保SDA已经处于输入�? * @param {uint8_t} time_us 电平持续时间
 * @return {*}  读取到的电平
 * @author: TOTHTOT
 * @date: 2023�?�?4�?1:24:52
 */
uint8_t cs1237_read_bit(uint8_t time_us)
{
    uint8_t recv_data = 0;

    CS1237_SCL_H;
    delay_us(time_us);
    recv_data = CS1237_SDA_READ;
    CS1237_SCL_L;
    delay_us(time_us);

    return recv_data;
}

/**
 * @name: cs1237_check
 * @msg: 检测模块是否在�? * @param {cs1237_device} *dev
 * @return {*} == 0 成功
 *             == 1, 失败, 不在�? * @author: TOTHTOT
 * @date:
 */
uint8_t cs1237_check(struct cs1237_device *dev)
{
    uint32_t retry = 0;

    // dev->dev_state_em = dev_online;

    CS1237_SCL_L; // 时钟拉低
   DATAOUT();
    CS1237_SDA_H; // OUT引脚拉高
	DATAIN();
    while (CS1237_SDA_READ == 1) // 等待CS237准备�?    {
        delay_us(10);
        retry++;
        if (retry > 30000000)
        {
             DATAOUT();
            CS1237_SDA_H; // OUT引脚拉高
            CS1237_SCL_H; // CLK引脚拉高
//            ERROR_PRINT("time out\r\n");
            return 1; // 超时，则直接退出程�?        }
    }

    return 0;
}

/**
 * @name: cs1237_send_byte
 * @msg: 发�?字节
 * @param {uint8_t} byte
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void cs1237_send_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) // 38 - 45个脉冲了，写8位数�?    {
        CS1237_SCL_H; // CLK=1;
        delay_us(1);
        if (byte & 0x80)
            CS1237_SDA_H; // OUT = 1
        else
            CS1237_SDA_L;
        byte <<= 1;
        CS1237_SCL_L; // CLK=0;
        delay_us(1);
}

/**
 * @name: cs1237_init
 * @msg: cs1237 初始�? * @param {cs1237_device} *dev 设备
 * @param {enum dev_frequency} frequency 输出频率
 * @param {enum dev_pga} pga
 * @param {enum dev_ch} ch 通道
 * @return {*}  == 0 初始化成�? *              == 1, 初始化失�? * @author: TOTHTOT
 * @date: 2023�?�?3�?7:35:15
 */
uint8_t cs1237_init(struct cs1237_device *dev, enum dev_frequency frequency, enum dev_pga pga, enum dev_ch ch)
{
    uint8_t cs1237_config = 0; // cs1237 的寄存器配置 0100 1000
    uint8_t ret;

    // 初始化频�?    
	switch (frequency)
    {
    case DEV_FREQUENCY_10:
        clrbit(cs1237_config, 4);
        clrbit(cs1237_config, 5);
        break;
    case DEV_FREQUENCY_40:
        setbit(cs1237_config, 4);
        clrbit(cs1237_config, 5);
        break;
    case DEV_FREQUENCY_640:
        setbit(cs1237_config, 5);
        clrbit(cs1237_config, 4);
        break;
    case DEV_FREQUENCY_1280:
        setbit(cs1237_config, 4);
        setbit(cs1237_config, 5);
        break;
    default:
        clrbit(cs1237_config, 4);
        clrbit(cs1237_config, 5);
        break;
    }

    // 初始�?pga
    switch (pga)
    {
    case DEV_PGA_1:
        clrbit(cs1237_config, 2);
        clrbit(cs1237_config, 3);
        break;
    case DEV_PGA_2:
        clrbit(cs1237_config, 3);
        setbit(cs1237_config, 2);
        break;
    case DEV_PGA_64:
        clrbit(cs1237_config, 2);
        setbit(cs1237_config, 3);
        break;
    case DEV_PGA_128:
        setbit(cs1237_config, 2);
        setbit(cs1237_config, 3);
        break;
    default:
        clrbit(cs1237_config, 2);
        clrbit(cs1237_config, 3);
        break;
    }

    // 初始�?通道
    switch (ch)
    {
    case DEV_CH_A:
        clrbit(cs1237_config, 0);
        clrbit(cs1237_config, 1);
        break;
    case DEV_CH_SAVE:
        clrbit(cs1237_config, 1);
        setbit(cs1237_config, 0);
        break;
    case DEV_CH_TEMPERERATURE:
        clrbit(cs1237_config, 0);
        setbit(cs1237_config, 1);
        break;
    case DEV_CH_SHORT:
        setbit(cs1237_config, 1);
        setbit(cs1237_config, 1);
        break;
    default:
        clrbit(cs1237_config, 0);
        clrbit(cs1237_config, 1);
        break;
    }

    // cs1237_config = 0X0C;
    /* 初始化设备结构体 */
    dev->adc_data = 0;
    dev->output_frequency = frequency;
    dev->get_adc_data = calculate_adc_num;
    dev->get_adc_config = cs1237_read_config(dev);
    dev->adc_config = cs1237_config;

    ret = cs1237_check(dev);
    if (ret != 0)
    {
        dev->dev_state_em = DEV_OFFLINE;
       // ERROR_PRINT("cs1237_check failed!\r\n");
        return 1;
    }
    dev->dev_state_em = DEV_ONLINE;

    /* 配置 CS1237 工作模式 */
    // 发�?�?9个脉�?    
		for (uint8_t i = 0; i < 29; i++)
    {
        CS1237_SCL_H;
        delay_us(10);
        CS1237_SCL_L;
        delay_us(10);
    }

     DATAOUT();
    CS1237_SCL_H;
    delay_us(10);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(10); // 30
    CS1237_SCL_H;
         delay_us(10);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(10); // 31
    CS1237_SCL_H;
    delay_us(10);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(10); // 32
    CS1237_SCL_H;
    delay_us(10);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(10); // 33
    CS1237_SCL_H;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(10); // 34
    CS1237_SCL_H;
    delay_us(10);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(10); // 35
    CS1237_SCL_H;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(10); // 36
    // 37     写入�?x65
    CS1237_SCL_H; // CLK=1;
    delay_us(10);
    CS1237_SCL_L; // CLK=0;
    delay_us(10);

    cs1237_send_byte(cs1237_config);

    CS1237_SDA_H; // OUT = 1
    CS1237_SCL_H; // CLK=1;
    delay_us(10);
    CS1237_SCL_L; // CLK=0;
    delay_us(10);

    //INFO_PRINT("adc config = %x\r\n", cs1237_config);
    return 0;
}

/**
 * @name: cs1237_read_config
 * @msg: 读取芯片的配置数�? * @param {cs1237_device} *dev 设备结构�? * @return {*} 读取到的寄存器数�? * @author: TOTHTOT
 * @date:
 */
uint8_t cs1237_read_config(struct cs1237_device *dev)
{
    unsigned char i = 0;
    unsigned char dat = 0; // 读取到的数据
    uint8_t ret = 0;

    ret = cs1237_check(dev);
    if (ret != 0)
    {
        dev->dev_state_em = DEV_OFFLINE;
       // ERROR_PRINT("cs1237_check failed!\r\n");
        return 1;
    }

    for (i = 0; i < 29; i++) // 产生�?�?9个时�? 
    {
        CS1237_SCL_H; // CLK=1;
        delay_us(1);
        CS1237_SCL_L; // CLK=0;
        delay_us(1);
    }

    dev->dev_state_em = DEV_ONLINE;
    DATAOUT();

    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 这是�?0个时�?
    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_L;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 这是�?1个时�?
    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 32

    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_L;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 33

    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 34

    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_H;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 35

    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_L;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 36

    CS1237_SDA_H;
    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // 37     写入0x56 即读命令

    dat = 0;
  DATAIN();
    for (i = 0; i < 8; i++) // �?8 - 45个脉冲了，读取数�?   
		{
        CS1237_SCL_H; // CLK=1;
        delay_us(1);
        CS1237_SCL_L; // CLK=0;
        delay_us(1);
        dat <<= 1;
        if (CS1237_SDA_READ == 1)
            dat++;
    }

    // �?6个脉�?    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SCL_L; // CLK=0;
    delay_us(1);

//    CS1237_SDA_OUT;
    CS1237_SDA_H; // OUT引脚拉高

    dev->adc_config = dat;

    return dat;
}

/**
 * @name: cs1237_read_data
 * @msg: 读取ADC数据，返回的是一个有符号数据
 * @param {cs1237_device} *dev  设备结构�? * @return {*} 读取到的adc数据
 * @author: TOTHTOT
 * @date:
 */
int32_t cs1237_read_data(struct cs1237_device *dev)
{
    unsigned char i = 0;
    uint32_t dat = 0; // 读取到的数据
    uint8_t ret = 0;

    ret = cs1237_check(dev);
    if (ret != 0)
    {
        dev->dev_state_em = DEV_OFFLINE;
        //ERROR_PRINT("cs1237_check failed!\r\n");
        return 1;
    }

    dev->dev_state_em = DEV_ONLINE;
    dat = 0;
	DATAOUT();
	CS1237_SDA_L;
		DATAIN();
    for (i = 0; i < 24; i++) // 获取24位有效转�?    
		{
        CS1237_SCL_H; // CLK=1;
        delay_us(1);
        dat <<= 1;
        if (CS1237_SDA_READ == 1)
            dat++;
        CS1237_SCL_L; // CLK=0;
        delay_us(1);
    }

    for (i = 0; i < 3; i++) // 接着前面的时�?再来3个时�?    
		{
        cs1237_send_bit(1, 1);
    }

	DATAOUT();
    CS1237_SDA_H; // OUT = 1;

    if (dat & 0x00800000) // 判断是负�?最高位24位是符号�?    {
        dev->adc_data = -(((~dat) & 0x007FFFFF) + 1); // 补码变源�?    }
    else
        dev->adc_data = dat; // 正数的补码就是源�?
    return dev->adc_data;
}

/**
 * @name: calculate_adc_num
 * @msg:
 * @param {cs1237_device} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
int32_t calculate_adc_num(struct cs1237_device *dev)
{
    int32_t max = 0, max_2 = 0, max_pos = 0, max_2_pos = 0;
    int32_t min = 0, min_pos = 0;

    // 采集 RAW_DATA_MAX_NUM �?数据去掉最高和最�?   
	for (uint8_t i = 0; i < RAW_DATA_MAX_NUM; i++)
    {
        dev->adc_calculate_raw_data[i] = cs1237_read_data(dev);
        if (dev->adc_calculate_raw_data[i] > max)
        {
            max = dev->adc_calculate_raw_data[i];
            max_pos = i;
            // INFO_PRINT("max = %d, pos = %d\r\n", max, max_pos);
        }
        if (i == 0)
        {
            min = dev->adc_calculate_raw_data[0];
            min_pos = 0;
            // INFO_PRINT("min = %d, pos = %d\r\n", min, min_pos);
        }
        if (dev->adc_calculate_raw_data[i] < min)
        {
            min = dev->adc_calculate_raw_data[i];
            min_pos = i;
            // INFO_PRINT("min = %d, pos = %d\r\n", min, min_pos);
        }
    }

    for (uint8_t i = 0; i < RAW_DATA_MAX_NUM; i++)
    {
        if (i == max_pos || i == min_pos)
        {
            continue;
        }

        dev->adc_calculate_deal_data += dev->adc_calculate_raw_data[i];
    }
    return (int)(dev->adc_calculate_deal_data = dev->adc_calculate_deal_data / (RAW_DATA_MAX_NUM - 2));
}
