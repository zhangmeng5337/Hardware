/*
 * @Description: cs1237 �豸�����ļ�
 * @Author: TOTHTOT
 * @Date: 2023-03-23 16:41:59
 * @LastEditTime: 2023-03-28 13:23:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\CS1237\cs1237.c
 */
#include "math.h"
#include "cs1237.h"
#include "stdio.h"
#include "main.h"
#include "kalman.h"
#include "reg.h"
static long AD_Res_Last = 0; //��һ�ֵ�ADC��ֵ����

#define setbit(x, y) x |= (1 << y)     // x��yλ��1
#define clrbit(x, y) x &= ~(1 << y)    // x��yλ��0
#define reversebit(x, y) x ^= (1 << y) // x��yλ��??#define getbit(x, y) ((x) >> (y)&1)    // ��ȡx�ĵ�y??
#define One_CLK  CS1237_SCL_H;delay_us(1);CS1237_SCL_L;delay_us(1);
#define ADC_Bit  24 //ADC��Чλ����������λ ���24λ
#define Lv_Bo 0.45//0.05  //�˲�ϵ�� С��1

int32_t cs1237_read_data(struct cs1237_device *dev);
uint8_t cs1237_read_config(struct cs1237_device *dev);
int32_t calculate_adc_num(struct cs1237_device *dev);
struct cs1237_device g_cs1237_device_st;
void DATAIN_IRQ(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin : PB9 */
//    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
//    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

}
void DATAIN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
//    GPIO_InitStruct.IT = GPIO_IT_FALLING;
//    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

}

void DATAOUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);
}
//???500US 25MHZ
void get_adc_update(uint32_t dat)
{
    g_cs1237_device_st.update = dat;
}
unsigned char get_adc_flag()
{
    return g_cs1237_device_st.update;
}
/**
 * @name: cs1237_nop
 * @msg: cs1237 ģ���ڲ�ʹ����ʱ ����һ??60ns
 * @return {*} ?? * @author: TOTHTOT
 * @date: 2023????4??1:43:50
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
 * @msg:����һλ��?? * @param {uint8_t} bit 1λ��?? * @param {uint8_t} time_us ʱ�Ӹߵ�ƽʱ?? ��λ:us
 * @return {*} ?? * @author: TOTHTOT
 * @date: 2023????4??0:34:08
 */
void cs1237_send_bit(uint8_t bit, uint8_t time_us)
{
    DATAOUT(); // �л����ģʽ
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
 * @msg: cs1237 ��ȡ1λ��?? ʹ��ǰȷ��SDA�Ѿ���������?? * @param {uint8_t} time_us ��ƽ����ʱ��
 * @return {*}  ��ȡ���ĵ�ƽ
 * @author: TOTHTOT
 * @date: 2023????4??1:24:52
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
 * @msg: ���ģ���Ƿ���?? * @param {cs1237_device} *dev
 * @return {*} == 0 �ɹ�
 *             == 1, ʧ��, ����?? * @author: TOTHTOT
 * @date:
 */
uint8_t cs1237_check(struct cs1237_device *dev)
{
    static uint32_t retry = 0;

    CS1237_SCL_L;
    DATAIN_IRQ();
    while (g_cs1237_device_st.update == 0)
    {
        static uint32_t retry = 0;
        retry++;
        if (retry >= 30000000)
        {
            retry = 0;
            return 1;

        }

    }




    return 0;
}
void cs1237_pwr_pd(void)
{

    CS1237_SDA_H; // OUT��������
    // delay_us(100000);

}
/**
 * @name: cs1237_send_byte
 * @msg: ��??�ֽ�
 * @param {uint8_t} byte
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void cs1237_send_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) // 38 - 45�������ˣ�д8λ��??
    {
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
}
//unsigned int getPga(unsigned char pga_dat)
//{
//    unsigned int result;
//    switch (pga_dat)
//    {
//        case 0:
//            result = 1;
//            break;
//        case 1:
//            result = 2;
//
//            break;
//        case 2:
//            result = 4;
//            break;
//        case 3:
//            result = 8;
//            break;
//        case 4:
//            result = 16;
//            break;
//        case 5:
//            result = 32;
//            break;
//        case 6:
//            result = 64;
//            break;
//        case 7:
//            result = 128;
//            break;
//        case 8:
//            result = 256;
//            break;
//
//        default:
//            result = 1;
//            break;
//    }
//
//    return result;
//}
//
//
//
//unsigned char getPgaToADC(unsigned char pga_dat)
//{
//    unsigned int result;
//    switch (pga_dat)
//    {
//        case 0://1
//            result = 1;
//            break;
//        case 1://2
//            result = 2;
//
//            break;
//        case 2://4
//            result = 1;
//            break;
//        case 3:
//            result = 1;
//            break;
//        case 4:
//            result = 1;
//            break;
//        case 5:
//            result = 1;
//            break;
//        case 6://64
//            result = 3;
//            break;
//        case 7://128
//            result = 4;
//            break;
//        case 8:
//            result = 1;
//            break;
//
//        default:
//            result = 1;
//            break;
//    }
//
//    return result;
//}


/**
 * @name: cs1237_init
 * @msg: cs1237 ��ʼ?? * @param {cs1237_device} *dev �豸
 * @param {enum dev_frequency} frequency ���Ƶ��
 * @param {enum dev_pga} pga
 * @param {enum dev_ch} ch ͨ��
 * @return {*}  == 0 ��ʼ����?? *              == 1, ��ʼ��ʧ?? * @author: TOTHTOT
 * @date: 2023????3??7:35:15
 */
uint8_t cs1237_init(struct cs1237_device *dev, enum dev_frequency frequency,
                    enum dev_pga pga, enum dev_ch ch)
{
    uint8_t cs1237_config = 0; // cs1237 �ļĴ������� 0100 1000
    uint8_t ret;

    clrbit(cs1237_config, 6);


    // ��ʼ��Ƶ??
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

    // ��ʼ??pga
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

    // ��ʼ??ͨ��
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
    /* ��ʼ���豸�ṹ�� */
    dev->adc_data = 0;
    dev->output_frequency = frequency;
    dev->get_adc_data = calculate_adc_num(dev);
    dev->get_adc_config = cs1237_read_config(dev);
    dev->adc_config = cs1237_config;
    SysTickDelay(1000);

    ret = cs1237_check(dev);
    if (ret == 1)
    {
        return 1;
    }
    dev->dev_state_em = DEV_ONLINE;
    DATAIN();

    /* ���� CS1237 ����ģʽ */
    // ��????9����??
    for (uint8_t i = 0; i < 29; i++)
    {
        CS1237_SCL_H;
        delay_us(CONFIG_DELAY);
        CS1237_SCL_L;
        delay_us(CONFIG_DELAY);
    }

    DATAOUT();
    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 30

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 31

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 32

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 33

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 34

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_L;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 35

    CS1237_SCL_H;
    delay_us(CONFIG_DELAY);
    CS1237_SDA_H;
    CS1237_SCL_L;
    delay_us(CONFIG_DELAY); // 36

    // 37     д��??x65
    CS1237_SCL_H; // CLK=1;
    delay_us(CONFIG_DELAY);
    CS1237_SCL_L; // CLK=0;
    delay_us(CONFIG_DELAY);

    cs1237_send_byte(cs1237_config);

    CS1237_SDA_H; // OUT = 1
    CS1237_SCL_H; // CLK=1;
    delay_us(CONFIG_DELAY);
    CS1237_SCL_L; // CLK=0;
    delay_us(CONFIG_DELAY);
    dev->get_adc_config = cs1237_read_config(dev);
    //INFO_PRINT("adc config = %x\r\n", cs1237_config);
    AD_Res_Last = 0;
    DATAIN_IRQ();
    return 0;
}

/**
 * @name: cs1237_read_config
 * @msg: ��ȡоƬ��������?? * @param {cs1237_device} *dev �豸�ṹ?? * @return {*} ��ȡ���ļĴ�����?? * @author: TOTHTOT
 * @date:
 */
uint8_t cs1237_read_config(struct cs1237_device *dev)
{
    unsigned char i = 0;
    unsigned char dat = 0; // ��ȡ��������
    uint8_t ret = 0;

    ret = cs1237_check(dev);
    if (ret == 1)
    {
        return 1;
    }
    DATAIN();


    for (i = 0; i < 29; i++) // ����????9��ʱ??
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
    delay_us(1);  // ����??0��ʱ??
    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SDA_L;
    CS1237_SCL_L; // CLK=0;
    delay_us(1);  // ����??1��ʱ??
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
    delay_us(1);  // 37     д��0x56 ��������

    dat = 0;
    DATAIN();
    for (i = 0; i < 8; i++) // ??8 - 45�������ˣ���ȡ��??
    {
        CS1237_SCL_H; // CLK=1;
        delay_us(1);
        CS1237_SCL_L; // CLK=0;
        delay_us(1);
        dat <<= 1;
        if (CS1237_SDA_READ == 1)
            dat++;
    }

    // ??6����??    CS1237_SCL_H; // CLK=1;
    delay_us(1);
    CS1237_SCL_L; // CLK=0;
    delay_us(1);

    DATAOUT();
    CS1237_SDA_H; // OUT��������

    dev->adc_config = dat;

    return dat;
}
//��ȡADC���ݣ����ص���һ���з�������
long Read_CS1237(void)
{
    unsigned char i;
    long dat = 0; //��ȡ��������
    unsigned char count_i = 0; //�����ʱ��
//        DOUT = 1;//�˿�����1��51�ر�
    CS1237_SCL_L;//ʱ������
    DATAOUT();
    //CS1237_SDA_H; // OUT��������
    DATAIN();

    while (CS1237_SDA_READ ==
            1) //оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237����Ϊ0���㶼׼����
    {
        delay_us(50);
        count_i++;
        if (count_i > 300)
        {
            CS1237_SCL_H;
            CS1237_SDA_H;
            return 0;//��ʱ����ֱ���˳�����
        }
    }
//        DOUT = 1;//�˿�����1��51�ر�
    dat = 0;
    for (i = 0; i < 24; i++) //��ȡ24λ��Чת��
    {
        CS1237_SCL_H;
        delay_us(1);
        dat <<= 1;
        if (CS1237_SDA_READ == 1)
            dat ++;
        CS1237_SCL_L;
        delay_us(1);
    }
    for (i = 0; i < 3; i++) //һ������27������
    {
        One_CLK;
    }
    //CS1237_SDA_H;
    //�ȸ��ݺ궨���������Чλ������һЩ����
    i = 24 - ADC_Bit;//i��ʾ��Ҫ������λ��
    dat >>= i;//���������λ��
    return dat;
}

/**
 * @name: cs1237_read_data
 * @msg: ��ȡADC���ݣ����ص���һ���з�������
 * @param {cs1237_device} *dev  �豸�ṹ?? * @return {*} ��ȡ����adc����
 * @author: TOTHTOT
 * @date:
 */
int32_t cs1237_read_data(struct cs1237_device *dev)
{
    unsigned char i = 0;
    uint32_t dat = 0; // ��ȡ��������
    uint8_t ret = 0;


//     ret = cs1237_check(dev);
//    if (ret != 0)
//    {
//        dev->dev_state_em = DEV_OFFLINE;
//        //ERROR_PRINT("cs1237_check failed!\r\n");
//        return 0;
//    }

    //if( g_cs1237_device_st.update == 1)
    {
        // g_cs1237_device_st.update = 0;
        dev->dev_state_em = DEV_ONLINE;
        dat = 0;
        // DATAOUT();
        CS1237_SDA_L;
        DATAIN();
        for (i = 0; i < 24; i++) // ��ȡ24λ��Чת??
        {
            CS1237_SCL_H; // CLK=1;
            delay_us(1);
            dat <<= 1;
            if (CS1237_SDA_READ == 1)
                dat++;
            CS1237_SCL_L; // CLK=0;
            delay_us(1);
        }

        for (i = 0; i < 3; i++) // ����ǰ���ʱ??����3��ʱ??
        {
            cs1237_send_bit(1, 1);
        }


        DATAOUT();
        CS1237_SDA_H; // OUT = 1;
        DATAIN_IRQ();
        if (dat & 0x00800000) // �ж��Ǹ�??���λ24λ�Ƿ���??    {
            dev->adc_data = -(((~dat) & 0x007FFFFF) + 1); // �����Դ??    }
        else
            dev->adc_data = dat; // �����Ĳ������Դ??
        dev->adc_data = dev->adc_data;
        return dev->adc_data;
    }


}


unsigned int getPga(unsigned char pga_dat)
{
    unsigned int result;
    switch (pga_dat)
    {
        case 0:
            result = 1;
            break;
        case 1:
            result = 2;

            break;
        case 2:
            result = 4;
            break;
        case 3:
            result = 8;
            break;
        case 4:
            result = 16;
            break;
        case 5:
            result = 32;
            break;
        case 6:
            result = 64;
            break;
        case 7:
            result = 128;
            break;
        case 8:
            result = 256;
            break;

        default:
            result = 1;
            break;
    }

    return result;
}



unsigned char getPgaToADC(unsigned char pga_dat)
{
    unsigned int result;
    switch (pga_dat)
    {
        case 0://1
            result = 1;
            break;
        case 1://2
            result = 2;

            break;
        case 2://4
            result = 1;
            break;
        case 3:
            result = 1;
            break;
        case 4:
            result = 1;
            break;
        case 5:
            result = 1;
            break;
        case 6://64
            result = 3;
            break;
        case 7://128
            result = 4;
            break;
        case 8:
            result = 1;
            break;

        default:
            result = 1;
            break;
    }

    return result;
}

extern uint32_t time_cal;
float val;
uint32_t ti;
void read_data()
{
    unsigned char pga;


	
    g_cs1237_device_st.adc_ori_data = cs1237_read_data(&g_cs1237_device_st);
	 pga = getPga(GetRegPrivate()->pga);
//	if(pga <= 2)
//		 g_cs1237_device_st.adc_ori_data  = 
//		  g_cs1237_device_st.adc_ori_data >>5;
//	else if(pga <= 64)
//		g_cs1237_device_st.adc_ori_data  = 
//		 g_cs1237_device_st.adc_ori_data >>6;
//
//	else if(pga <= 128)
//		g_cs1237_device_st.adc_ori_data  = 
//		 g_cs1237_device_st.adc_ori_data >>7;//8
//
//	else
//		g_cs1237_device_st.adc_ori_data  = 
//		 g_cs1237_device_st.adc_ori_data >>5;


}
long Read_12Bit_AD(void)
{
    float out, adc_val, adc_Newval, e;
    float tmp, tmp2;
	unsigned int pga;
    static unsigned char i = 0;
    adc_val = AD_Res_Last;
    //if (g_cs1237_device_st.update == 1)
    {

        time_cal = 0;
        e =  g_cs1237_device_st.adc_ori_data;
		AD_Res_Last = e/20.0 ;

        if (e != 0)
        {
//             pga = getPga(GetRegPrivate()->pga);
//			if(pga <= 1)
//				pga = 5;//10
//			else if(pga <= 64)
//				pga = 5;
//			else if(pga <= 128)
//				pga = 5; //100 80  50 10
//			else
//				pga = 5;   //30        
//			// g_cs1237_device_st.adc_ori_data = e;
//            time_cal = 0;
           // adc_Newval = GetMedianNum(e);

            tmp = adc_Newval - adc_val;
            tmp = fabs(tmp);

            if (adc_val != 0 && tmp >= pga)
                adc_val = adc_Newval;
            else
            {
                if (adc_val == 0)
                    adc_val = adc_Newval;
            }


            if (adc_val == adc_Newval)
            {
                i++;
                if (i > SLID_SIZE)
                {
                    i = 0;
                    adc_val = adc_Newval;
                    if (adc_val != 0) // ������ȷ����
                    {
                        adc_val = adc_val * Lv_Bo + adc_Newval * (1 - Lv_Bo);
                        tmp2 = adc_val;//����εļ������ŵ�ȫ�ֱ������汣��
                        AD_Res_Last = tmp2 ;
                    }
                }
            }
            else
                i = 0;

		return AD_Res_Last;
        }
        else
            return 0;

    }




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
    static uint8_t i = 0;
    // �ɼ� RAW_DATA_MAX_NUM ??����ȥ����ߺ���??
    //for ( i = 0; i < RAW_DATA_MAX_NUM; i++)
    {
        return  Read_12Bit_AD();
    }
}
