#include "ads1158.h"
#include "sys.h"



/*float ft;
float vcc;
float gain;
float offset;
float temp;
uint32_t adsvolt[20];


uint32_t ADIvcc;
uint32_t ADIref;
uint32_t ADIgain;
uint32_t ADItemp;
uint32_t ADIoffset;*/

static structChan ads1158_adc1_data;
structChan ads1158_adc2_data;

uint8_t sys_ID = 0x00;

//根据DRDY直接读数据时使用的变量
uint8_t read_data[3] = {0x00, 0x00, 0x00};
uint8_t Status_byte = 0x00;
uint8_t HSB_byte = 0x00;
uint8_t MSB_byte = 0x00;
uint8_t LSB_byte = 0x00;
uint8_t channel = 0x00;
uint16_t Data_combine = 0x00;
float Data_result = 0;

uint8_t count = 0;					//计数 行 列
uint8_t ChannelSendData[8] = {0x00};


//ads1158初始化函数 试着读取系统ID
void ads_init(void)
{
    uint8_t cmd_reset = 0xC0;

    uint8_t cmd_readID2[2] = {0x49, 0x00}; //命令读ID寄存器
    uint8_t back_readID2[2] = {0x00, 0x00};

    //SPI写0xc0（复位） 软件复位
    ADS1158_CS1_LOW;
    delay_us(20);
    Ads1158_Transmit(&cmd_reset, 1);
    delay_us(100);
    ADS1158_CS1_HIGH;
    HAL_Delay(10);

    //SPI写0x49（寄存器读命令 一次访问一个寄存器 访问09h寄存器），然后读值，值应该是0x9b（判断设备ID）
    ADS1158_CS1_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(20);
    ADS1158_CS1_HIGH;
    HAL_Delay(10);

    sys_ID = back_readID2[1];
    if (sys_ID == 0x8B)
    {
        //ok
        printf("ADS1158 ok\r\n");
    }
    else
    {
        //error
        printf("ADS1158 failed\r\n");
    }
    printf("ADS1158 id is %d\r\n",sys_ID);


    //SPI写0xc0（复位） 软件复位
    ADS1158_CS2_LOW;
    delay_us(20);
    Ads1158_Transmit(&cmd_reset, 1);
    delay_us(100);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    //SPI写0x49（寄存器读命令 一次访问一个寄存器 访问09h寄存器），然后读值，值应该是0x9b（判断设备ID）
    ADS1158_CS2_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(20);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    sys_ID = back_readID2[1];
    if (sys_ID == 0x8B)
    {
        //ok
        printf("ADS11582 ok\r\n");
    }
    else
    {
        //error
        printf("ADS11582 failed\r\n");
    }
    printf("ADS11582 id is %d\r\n",sys_ID);



}

/*void ads_rdid(void)
{
    //uint8_t cmd_reset=0xC0;
    uint8_t cmd_readID2[2] = {0x49, 0x00}; //命令读ID寄存器
    uint8_t back_readID2[2] = {0x00, 0x00};

    //SPI写0x49（寄存器读命令 一次访问一个寄存器 访问09h寄存器），然后读值，值应该是0x9b（判断设备ID）
    ADS1158_CS1_LOW;
    delay_us(100);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(100);
    ADS1158_CS1_HIGH;
    HAL_Delay(10);

    sys_ID = back_readID2[1];
    printf("id is %d\r\n",sys_ID);
    if (sys_ID == 0x9B)
    {
        back_readID2[1] = 0; //ok
    }
    else
    {
        back_readID2[1] = 1; //error
    }

    ADS1158_CS2_LOW;
    delay_us(100);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(100);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    sys_ID = back_readID2[1];
    printf("id is %d\r\n",sys_ID);
    if (sys_ID == 0x9B)
    {
        back_readID2[1] = 0; //ok
    }
    else
    {
        back_readID2[1] = 1; //error
    }




}
*/





/*配置ADS1158的10个寄存器
unsigned char write_conf[11]={0x70,0x02,0x50,0x00,0x00,0x80,0x1F,0x00,0xFF,0x00,0x8B},read_conf[10];
0x70 :寄存器写命令 从00h 写到09h
0x16 ：1 config0  auto-scan,外部adc输入，带状态字  0
0x01 ：2 config1  standby 转速延迟16period 转换速率最低1
0x00 ：3 MUXSCH fixed设置，默认值2
0x00 ：4 MUXDIF scan设置，没有使用差分输入，设置为0   3
0x80 ：5 MUXSG0 使用单端输入7-12，这个寄存器设置0-7 FF  4
0x1F ：6 MUXSG1 单端输入8-15配置，我们使用8-12      0   3        5
0x00 ：7 SYSRED 读取系统温度，可以用作验证  6
0xFF,0x00 ：8 9 GPIOC GPIOD 不使用，默认值  7 8 
0x8B ：10 ID寄存器 默认是0x9Bh  
*/
 static uint8_t j = 0;
void ads_config(void)
{
    uint8_t i = 0;

#ifdef AUTOSCAN
    //uint8_t txdata[11] = {0x70, 0x06, 0x01, 0x00, 0x00, 0xFF, 0x03, 0x00, 0xFF, 0x00, 0x8B}; //normal-autoscan   neibu
    //uint8_t txdata[11] = {0x70, 0x16, 0x01, 0x00, 0x00, 0xFF, 0x03, 0x00, 0xFF, 0x00, 0x8B}; //normal-autoscan
    //uint8_t txdata[11] = {0x70, 0x16, 0x01, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x8B}; //normal-autoscan
    //uint8_t txdata[11] = {0x70, 0x06, 0x01, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x8B}; //normal-autoscan   内部直连
#ifdef DIFF
    //uint8_t txdata[11] = {0x70, 0x02, 0x01, 0x00, 0x1f, 0x00, 0x00, 0x3d, 0xFF, 0x00, 0x8B}; //normal-autoscan   内部直连  基准计算
    uint8_t txdata[11] = {0x70, 0x12, 0x01, 0x00, 0x1f, 0x00, 0x00, 0x3d, 0xFF, 0x00, 0x8B}; //normal-autoscan   外部直连 差分 基准计算
#else
    //uint8_t txdata[11] = {0x70, 0x02, 0x01, 0x00, 0x00, 0xFF, 0x03, 0x3d, 0xFF, 0x00, 0x8B}; //normal-autoscan   内部直连  基准计算   用在天津与保定项目
    uint8_t txdata[11] = {0x70, 0x02, 0x01, 0x00, 0x00, 0x1C, 0x00, 0X3C, 0xFF, 0x00, 0x8B}; //normal-autoscan   内部直连  基准计算   用三个通道
#endif
#endif
#ifdef SINGLE
	uint8_t txdata[11] = {0x70, 0x06, 0x01, 0x00, 0x00, 0xff, 0xff, 0x3d, 0xFF, 0x00, 0x9B};

                        //       00h   01h    02h   03h   04     05   06    07   08    09
   // uint8_t txdata[11] = {0x70, 0x16, 0x01, 0x00, 0x00, 0xff, 0xff, 0x3d, 0xFF, 0x00, 0x9B}; //normal-fixed-01
    //uint8_t txdata[11] = {0x70, 0x36, 0x01, 0x10, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x00, 0x8B}; //normal-fixed-01
#endif
    //
    //uint8_t txdata[11] = {0x70, 0x16, 0x01, 0x00, 0x00, 0x28, 0x3c, 0x00, 0xFF, 0x00, 0x8B}; //normal
    //uint8_t txdata[11] = {0x70, 0x12, 0x01, 0x00, 0x00, 0x28, 0x3c, 0x3c, 0xFF, 0x00, 0x9B};//config ref channel for test
    uint8_t readcmd[11] = {0}; //从00h到09h 读所有寄存器

    uint8_t rxdata[11] = {0}; //回读10个寄存器的数据，判断设置是否成功

    //配置10个寄存器
    ADS1158_CS1_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(txdata, rxdata, 11);
    delay_us(20);
    ADS1158_CS1_HIGH;
    delay_us(10);

    //回读10个寄存器
    readcmd[0] = 0x50; //从00h到09h 读所有寄存器

    ADS1158_CS1_LOW;
    delay_us(10);
    Ads1158_TransmitReceive(readcmd, rxdata, 11);
    delay_us(10);
    ADS1158_CS1_HIGH;
    delay_us(10);

    printf("tx data is %x %x %x %x %x %x %x %x %x %x\r\n",txdata[1],txdata[2],txdata[3],txdata[4],txdata[5],txdata[6],txdata[7],txdata[8],txdata[9],txdata[10]);
    printf("rx data is %x %x %x %x %x %x %x %x %x %x\r\n",rxdata[1],rxdata[2],rxdata[3],rxdata[4],rxdata[5],rxdata[6],rxdata[7],rxdata[8],rxdata[9],rxdata[10]);
    for (i = 1; i < 9; i++)
    {
        if (txdata[i] == rxdata[i])
            j++;
    }
    if (j == 8)
    {
        //配置ok
        printf("ADS11581 set up ok\r\n");
    }

    //配置10个寄存器
    ADS1158_CS2_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(txdata, rxdata, 11);
    delay_us(20);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    //回读10个寄存器
    readcmd[0] = 0x50; //从00h到09h 读所有寄存器

    ADS1158_CS2_LOW;
    delay_us(10);
    Ads1158_TransmitReceive(readcmd, rxdata, 11);
    delay_us(10);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    printf("tx data is %x %x %x %x %x %x %x %x %x %x\r\n",txdata[1],txdata[2],txdata[3],txdata[4],txdata[5],txdata[6],txdata[7],txdata[8],txdata[9],txdata[10]);
    printf("rx data is %x %x %x %x %x %x %x %x %x %x\r\n",rxdata[1],rxdata[2],rxdata[3],rxdata[4],rxdata[5],rxdata[6],rxdata[7],rxdata[8],rxdata[9],rxdata[10]);
    j=0;
		for (i = 1; i < 9; i++)
    {
        if (txdata[i] == rxdata[i])
            j++;
    }
    if (j == 8)
    {
        //配置ok
        printf("ADS11582 set up ok\r\n");
    }



}

void ads_Pulse(unsigned char adc_NO)
{
    uint8_t cmd_Pulse = 0x80; //转换命令

    if(adc_NO == 1)
    {
        ADS1158_CS1_LOW;
        //delay_us(1);
        START_H
        delay_us(1);
        //Ads1158_Transmit(&cmd_Pulse, 1);
        START_L
        delay_us(1);
        ADS1158_CS1_HIGH;

    }
    else if(adc_NO == 2)
    {
        ADS1158_CS2_LOW;
        //delay_us(1);
        START_H
        delay_us(1);
        //Ads1158_Transmit(&cmd_Pulse, 1);
        START_L
        delay_us(1);
        ADS1158_CS2_HIGH;

    }

}

/*
ads1158通道数据的读取
我们使用的是单端输入7-12：7 gyro_x； 8 gyro_y； 9 gyro_z； 10 acc_x ； 11 acc_y； 12 acc_z
配置中使用的auto_scan模式，
转换命令触发转发，根据DRDY读取转换的结果，一次转化的结束后 通道自动指向下一个选中的通道 再次转换命令
*/

unsigned int dataChannelFlag = 0;
float vTemp;
float wendu;
/*unsigned char REG_readdata(unsigned char adc_chip_sel)
{

    unsigned char ret = 0;

    uint8_t REG_cmddata[5] = {0x30, 0x00, 0x00, 0x00,0x00}; //使用寄存器方式 读取通道数据
    uint8_t REG_recdata[5] = {0x00, 0x00, 0x00, 0x00,0x00}; //使用寄存器方式 读取通道数据

    if(adc_chip_sel == 1 )
    {
        ADS1158_CS1_LOW;
        delay_us(1);

        Ads1158_TransmitReceive(REG_cmddata, REG_recdata,5);

        delay_us(1);
        ADS1158_CS1_HIGH;

    }
    else
    {
        ADS1158_CS2_LOW;
        delay_us(1);

        Ads1158_TransmitReceive(REG_cmddata, REG_recdata,5);

        delay_us(1);
        ADS1158_CS2_HIGH;

    }




    Status_byte = REG_recdata[1];
    channel = Status_byte & 0x1f;

    if(channel<24)
        channel = channel - 0x08;

    HSB_byte = REG_recdata[2];
    MSB_byte = REG_recdata[3];
    LSB_byte = REG_recdata[4];


    switch(channel)
    {
    case 0:
        adsvolt[0] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 1:
        adsvolt[1] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 2:
        adsvolt[2] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 3:
        adsvolt[3] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 4:
        adsvolt[4] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 5:
        adsvolt[5] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 6:
        adsvolt[6] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 7:
        adsvolt[7] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 8:
        adsvolt[8] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 9:
        adsvolt[9] = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 24:
        ADIoffset = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        break;
    case 26:
        ADIvcc = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        vcc = ADIvcc/786432.0;
        vcc = vcc + BASE;
        break;
    case 27:
        ADItemp = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        if(ADItemp & 0x800000)
        {
            wendu = ((float)(0xffffff - ADItemp)/(float)0x780000)*ft;
            wendu = -vTemp;
        }
        else
        {
            wendu = ((float)ADItemp/(float)0x780000)*ft;
        }
        wendu = (wendu * 1000000 - 168000) / 563 + 25;
        break;
    case 28:
        ADIgain = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        gain = ADIgain/7864320.0;
        break;
    case 29:
        ADIref = (HSB_byte<<16) | (MSB_byte<<8) | LSB_byte;
        ft = ADIref/786432.0;
        break;
    default:
        break;
    }
    if(channel < 9)
    {
        if(adsvolt[channel] & 0x800000)
        {
            vTemp = ((float)(0xffffff - adsvolt[channel])/(float)0x780000)*ft/gain;
            vTemp = -vTemp;
        }
        else
        {
            vTemp = ((float)adsvolt[channel]/(float)0x780000)*ft/gain;
        }


        if(4 == channel)
            dataInfoCan.voltCanSend = vTemp * 50;//V*10
        else if(2 == channel)
        {
            //vTemp = vTemp - 0.613;
            if(vTemp<0)
                vTemp = -vTemp;
            dataInfoCan.currentLCanSend = vTemp/0.006;//100MA
            if(dataInfoCan.currentLCanSend<4)
                dataInfoCan.currentLCanSend = 0;
        }
        else if(3 == channel)
        {
            //vTemp = vTemp - 0.613;
            if(vTemp<0)
                vTemp = -vTemp;

            dataInfoCan.currentRCanSend = vTemp/0.006;//100MA
            if(dataInfoCan.currentRCanSend<4)
                dataInfoCan.currentRCanSend = 0;
        }


#ifdef PRINTF
        printf("%d:%f  ",channel,vTemp);
#endif
    }

    //printf("channel %d is %x:%f\r\n",channel,adsvolt[channel],vTemp);
    if(channel ==4)
    {
#ifdef PRINTF
        printf("ref:%f  ",ft);
        printf("temp:%f  ",wendu);
        printf("vcc:%f  ",vcc);
        printf("gain:%f    ",gain);
        //printf("offset:%d  ",ADIoffset);
        printf("totol time is %.2f ms",(10000-timestamp1)*0.1);
        printf("\r\n\r\n");
#endif
        return 1;
    }

}*/

/*void readADS(void)
{
    unsigned char adc_chip_sel;

    {
        int timeout = 10000;
        adc_chip_sel = 1;
continu_conv:
        ads_Pulse(adc_chip_sel);

        while(timeout)
        {
            if(adc_chip_sel == 1)
            {
                if (DATA_READY == 0)
                {
                    if (REG_readdata() == 1)
                    {
                        //	timeout = 0;
                        if(adc_chip_sel == 1)
                            adc_chip_sel = 2;
                        break;
                    }
                    ads_Pulse(adc_chip_sel);
                }

            }
            else if(adc_chip_sel == 2)
            {
                if (DATA_READY2 == 0)
                {
                    if (REG_readdata() == 1)
                    {
                        //	timeout = 0;
                        if(adc_chip_sel == 2)
                            adc_chip_sel = 0;
                        break;
                    }
                    ads_Pulse(adc_chip_sel);
                }
            }
            else
                ;

            timeout--;
        }
        if(adc_chip_sel == 1)
        {
            timeout = 10000;
            adc_chip_sel = 2;
            goto continu_conv;
        }

    }
}

//float *get_ads_volt()
//{
//    return adsvolt;
//}
*/
unsigned char REG_readdata_u(unsigned char adc_NO)
{

    unsigned char ret = 0;
    uint32_t tmp;
	    uint8_t REG_readdata[5] = {0x30, 0x00, 0x00, 0x00,0x00}; //使用寄存器方式 读取通道数据

    uint8_t REG_cmddata[5] = {0x30, 0x00, 0x00, 0x00,0x00}; //使用寄存器方式 读取通道数据
    uint8_t REG_recdata[5] = {0x00, 0x00, 0x00, 0x00,0x00}; //使用寄存器方式 读取通道数据

    if(adc_NO  == 0x01)
    {
        ADS1158_CS1_LOW;
        delay_us(1);

        Ads1158_TransmitReceive(REG_readdata, REG_recdata,5);

        delay_us(1);
        ADS1158_CS1_HIGH;
    }
    else if(adc_NO  == 0x02)
    {
        ADS1158_CS2_LOW;
        delay_us(1);
        Ads1158_TransmitReceive(REG_readdata, REG_recdata,5);
        delay_us(1);
        ADS1158_CS2_HIGH;
    }


    Status_byte = REG_recdata[1];
    channel = Status_byte & 0x1f;
    if((Status_byte&0x80)==0)
		return 0;
    if(channel<24&&channel>7)
        channel = channel - 0x08;

    HSB_byte = REG_recdata[2];
    MSB_byte = REG_recdata[3];
    //LSB_byte = REG_recdata[4];
    //LSB_byte = REG_recdata[4];
	tmp =  HSB_byte<<8;
	tmp =  tmp | MSB_byte;
    if(channel>=0&&channel<=15)
    {
        if(adc_NO  == 1)

            ads1158_adc1_data.adsvolt[channel] = tmp;
        else
            ads1158_adc2_data.adsvolt[channel] = tmp;


    }
    switch(channel)
    {
	    case 24:
	        if(adc_NO  == 1)
	            ads1158_adc1_data.ADIoffset = tmp  ;
	        else
	            ads1158_adc2_data.ADIoffset = tmp  ;

	        break;
	    case 26:

	        if(adc_NO  == 1)
	        {
	            ads1158_adc1_data.ADIvcc = tmp  ;
	            ads1158_adc1_data.vcc = ads1158_adc1_data.ADIvcc/3072.0;
	            ads1158_adc1_data.vcc = ads1158_adc1_data.vcc + BASE;

	        }
	        else
	        {
	            ads1158_adc2_data.ADIvcc = tmp ;
	            ads1158_adc2_data.vcc = ads1158_adc2_data.ADIvcc/3072.0;
	            ads1158_adc2_data.vcc = ads1158_adc2_data.vcc + BASE;

	        }

	        break;
	    case 27:
	        if(adc_NO  == 1)
	        {
	            ads1158_adc1_data.ADItemp = tmp  ;
	            ads1158_adc1_data.vcc = ads1158_adc1_data.ADIvcc/3072.0;
	            ads1158_adc1_data.vcc = ads1158_adc1_data.vcc + BASE;
	            if(ads1158_adc1_data.ADItemp & 0x800000)
	            {
	                ; //ads1158_adc1_data.wendu = ((float)(0xffffff - ads1158_adc1_data.ADItemp)/(float)0x780000)*ads1158_adc1_data.ft;
	                //ads1158_adc1_data.wendu = -ads1158_adc1_data.vTemp;
	            }
	            else
	            {
	                ads1158_adc1_data.wendu = ((float)ads1158_adc1_data.ADItemp/(float)0x780000)*ads1158_adc1_data.ft;
	            }
	            ads1158_adc1_data.wendu = (ads1158_adc1_data.wendu * 1000000 - 168000) / 563 + 25;
	        }
	        else
	        {
	            ads1158_adc2_data.ADItemp = tmp  ;
	            ads1158_adc2_data.vcc = ads1158_adc2_data.ADIvcc/3072.0;
	            ads1158_adc2_data.vcc = ads1158_adc2_data.vcc + BASE;
	            if(ads1158_adc2_data.ADItemp & 0x800000)
	            {
	                // ads1158_adc2_data.wendu = ((float)(0xffffff - ads1158_adc2_data.ADItemp)/(float)0x780000)*ads1158_adc2_data.ft;
	                ;//  ads1158_adc2_data.wendu = -ads1158_adc2_data.vTemp;
	            }
	            else
	            {
	                ads1158_adc2_data.wendu = ((float)ads1158_adc2_data.ADItemp/(float)0x780000)*ads1158_adc2_data.ft;
	            }
	            ads1158_adc1_data.wendu = (ads1158_adc1_data.wendu * 1000000 - 168000) / 563 + 25;
	        }
	        break;
	    case 28:
	        if(adc_NO  == 1)
	        {
	            ads1158_adc1_data.ADIgain = tmp;
	            ads1158_adc1_data.gain = ads1158_adc1_data.ADIgain/3072.0;
	        }
	        else
	        {
	            ads1158_adc2_data.ADIgain = tmp;
	            ads1158_adc2_data.gain = ads1158_adc2_data.ADIgain/3072.0;
	        }

	        break;
	    case 29:
	        if(adc_NO  == 1)
	        {
	            ads1158_adc1_data.ADIref = tmp ;
	            ads1158_adc1_data.ft = ads1158_adc1_data.ADIref/3072.0;
	        }
	        else
	        {
	            ads1158_adc2_data.ADIref = tmp ;
	            ads1158_adc2_data.ft = ads1158_adc2_data.ADIref/3072.0;

	        }
	        return 1;
	        break;
	    default:
	        break;
    }
    /*if(channel < 9)
    {
    	if(adsvolt[channel] & 0x800000)
    	{
    		vTemp = ((float)(0xffffff - adsvolt[channel])/(float)0x780000)*ft/gain;
    		vTemp = -vTemp;
    	}
    	else
    	{
    		vTemp = ((float)adsvolt[channel]/(float)0x780000)*ft/gain;
    	}


    	if(4 == channel)
    		dataInfoCan.voltCanSend = vTemp * 50;//V*10
    	else if(2 == channel)
    	{
    		//vTemp = vTemp - 0.613;
    	  if(vTemp<0)
    			vTemp = -vTemp;
    		dataInfoCan.currentLCanSend = vTemp/0.006;//100MA
    		if(dataInfoCan.currentLCanSend<4)
    			dataInfoCan.currentLCanSend = 0;
    	}
    	else if(3 == channel)
    	{
    		//vTemp = vTemp - 0.613;
    	  if(vTemp<0)
    			vTemp = -vTemp;

    		dataInfoCan.currentRCanSend = vTemp/0.006;//100MA
    		if(dataInfoCan.currentRCanSend<4)
    			dataInfoCan.currentRCanSend = 0;
    	}


    	#ifdef PRINTF
    	printf("%d:%f  ",channel,vTemp);
    	#endif
    }

    //printf("channel %d is %x:%f\r\n",channel,adsvolt[channel],vTemp);
    if(channel ==4)
    {
    	#ifdef PRINTF
    	printf("ref:%f  ",ft);
    	printf("temp:%f  ",wendu);
    	printf("vcc:%f  ",vcc);
    	printf("gain:%f    ",gain);
    	//printf("offset:%d  ",ADIoffset);
    	printf("totol time is %.2f ms",(10000-timestamp1)*0.1);
    	printf("\r\n\r\n");
    	#endif
    	return 1;
    }*/
  return 0;
}



void ads_read_data()
{
    static unsigned char adc_chip_sel;
	//ads1158_adc1_data.chip_No = 1;
	if(ads1158_adc1_data.chip_No == 1)
	{
	    if(ads1158_adc1_data.ads1_start== 0||
			ads1158_adc1_data.ads1_update == 1)
	    {
			ads_Pulse(ads1158_adc1_data.chip_No);
			ads1158_adc1_data.ads1_start=1;//ads1158 1启动标志
			ads1158_adc1_data.ads2_start=0;
			ads1158_adc1_data.ads1_update = 0;

			}


	}
	else if(ads1158_adc1_data.chip_No == 2)
	{
	    if(ads1158_adc1_data.ads2_start== 0||
			ads1158_adc1_data.ads2_update == 1)
	    {
			ads_Pulse(ads1158_adc1_data.chip_No);
			ads1158_adc1_data.ads2_start=1;//ads1158 1启动标志
			ads1158_adc1_data.ads1_start=0;
			ads1158_adc1_data.ads2_update = 0;

			}


	}
    
	
    if(ads1158_adc1_data.chip_No == 1||ads1158_adc1_data.chip_No == 2)
    {
        if (REG_readdata_u(ads1158_adc1_data.chip_No) == 1)
        {
            if(ads1158_adc1_data.chip_No == 1)
            {
               // ads1158_adc1_data.chip_No = 0;
                ads1158_adc1_data.chip_No = 2;//供片选2使用
            }

            else
            {
                ads1158_adc1_data.chip_No = 1;
                //ads1158_adc1_data.chip_No = 0;

            }

        }

    }

}
void ads1158_to_ma()
{
 //  ads1158_adc1_data.ads_ma
}
