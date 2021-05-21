#include "BMP388.h"

extern I2C_HandleTypeDef hi2c1;

struct bmp3_calib_data  calib_data;
struct bmp3_uncomp_data uncomp_data;
bmp3_data      comp_data;

bmp3_data  *GetPressure(void)
{
    return &comp_data;

}

////初始化IIC
//void IIC_Init()
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13);
//}

////产生IIC起始信号，SCL为高的情况下SDA由高变低
//void IIC_Start()
//{
//    SDA_OUT();
//    IIC_SCL=0;
//    IIC_SDA=1;
//    IIC_SCL=1;
//    delay_us(2);
//    IIC_SDA=0;
//    delay_us(2);
//    IIC_SCL=0;
//}

////产生IIC停止信号，SCL为高的情况下SDA由低变高
//void IIC_Stop(void)
//{
//    SDA_OUT();
//    IIC_SCL=0;
//    IIC_SDA=0;
//    IIC_SCL=1;
//    delay_us(2);
//    IIC_SDA=1;
//    delay_us(2);
//		IIC_SCL=0;
//}

////等待应答信号到来
//u8 IIC_Wait_Ack(void)
//{
//    u16 ucErrTime=0;
//    SDA_OUT();
//    IIC_SCL=0;
//    IIC_SDA=1;
//    SDA_IN();
//    delay_us(2);
//    IIC_SCL=1;
//    while(READ_SDA)
//    {
//        ucErrTime++;
//        if(ucErrTime>2500)
//        {
//            IIC_Stop();
//            return 0;
//        }
//    }
//    IIC_SCL=0;
//    return 1;
//}

////产生ACK应答
//void IIC_Ack(void)
//{
//    IIC_SCL=0;
//    SDA_OUT();
//    IIC_SDA=0;
//    delay_us(2);
//    IIC_SCL=1;
//    delay_us(2);
//    IIC_SCL=0;
//}

////不产生ACK应答
//void IIC_NAck(void)
//{
//    IIC_SCL=0;
//    SDA_OUT();
//    IIC_SDA=1;
//    delay_us(2);
//    IIC_SCL=1;
//    delay_us(2);
//    IIC_SCL=0;
//}

////IIC发送一个字节
//void IIC_Send_Byte(u8 txd)
//{
//    u8 t;
//    SDA_OUT();
//    IIC_SCL=0;
//    for(t=0;t<8;t++)
//    {
//        if((txd&0x80)>>7)
//            IIC_SDA=1;
//        else
//            IIC_SDA=0;
//        txd<<=1;
//				delay_us(2);
//        IIC_SCL=1;
//        delay_us(2);
//        IIC_SCL=0;
//    }
//}

////读1个字节
//u8 IIC_Read_Byte(void)
//{
//    unsigned char i,receive=0;
//    SDA_IN();
//    for(i=0;i<8;i++)
//    {
//        IIC_SCL=0;
//        delay_us(2);
//        IIC_SCL=1;
//        receive<<=1;
//        if(READ_SDA)
//            receive++;
//        delay_us(1);
//    }
//    return receive;
//}

//写一个寄存器
//发送开始信号，设备地址，寄存器地址，数据
void BMP388_Send_Byte(u8 addr,u8 data)
{
    /*IIC_Start();
    IIC_Send_Byte(BMP388_Addr<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    IIC_Wait_Ack();
    IIC_Stop();*/
    HAL_I2C_Mem_Write(&hi2c1, BMP388_Addr, addr,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

//读一个寄存器
//先发送开始信号，设备地址，要读的寄存器地址
//然后重新发送开始信号，读的对应设备地址，开始读数据
u8 BMP388_Read_Byte(u8 addr)
{
    u8 BMP388_Recive;
    /*IIC_Start();
    IIC_Send_Byte(BMP388_Addr<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(BMP388_Addr<<1|0x01);
    IIC_Wait_Ack();
    BMP388_Recive = IIC_Read_Byte();
    	IIC_NAck();
    IIC_Stop();*/
    HAL_I2C_Mem_Read(&hi2c1, BMP388_Addr, addr,
                     I2C_MEMADD_SIZE_8BIT, &BMP388_Recive, 1, 1000);
    return BMP388_Recive;
}

//读取未修正的气压和温度信息
void get_PandT()
{
    u8 reg_data[6]= {0};

    u8 status;
    status = BMP388_Read_Byte(0x03);
    while(!((status&0x40)&&(status&0x20)))  //查询数据是否准备完毕  0 1 1 0 0000
    {
        status = BMP388_Read_Byte(0x03);
        //delay_ms(1);
    }
    /*IIC_Start();
    IIC_Send_Byte(BMP388_Addr<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x04);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(BMP388_Addr<<1|0x01);
    IIC_Wait_Ack();*/
    HAL_I2C_Mem_Read(&hi2c1, BMP388_Addr, 0x04,
                     I2C_MEMADD_SIZE_8BIT, reg_data, 5, 1000);
//  / * for(i=0;i<5;i++)												//连续读6个寄存器，读完最后一个数据发送不应答信号
//    {
//        reg_data[i] = IIC_Read_Byte();
//        IIC_Ack();
//    }
//		reg_data[5] = IIC_Read_Byte();
//		IIC_NAck();
//    IIC_Stop();*/

    //数据合成
    uncomp_data.pressure    =  reg_data[2]<<16 | reg_data[1]<<8 | reg_data[0];
    uncomp_data.temperature =  reg_data[5]<<16 | reg_data[4]<<8 | reg_data[3];
}


//读取修正系数
void get_calib_data()
{
    u8 reg_data[21]= {0};
    u8 i=0;
    /*  IIC_Start();
      IIC_Send_Byte(BMP388_Addr<<1);
      IIC_Wait_Ack();
      IIC_Send_Byte(0x31);
      IIC_Wait_Ack();
      IIC_Start();
      IIC_Send_Byte(BMP388_Addr<<1|0x01);
      IIC_Wait_Ack();*/
    HAL_I2C_Mem_Read(&hi2c1, BMP388_Addr, 0x31,
                     I2C_MEMADD_SIZE_8BIT, reg_data, 21, 1000);



    /*  for(i=0;i<20;i++)                //连续读21个数据
      {
          reg_data[i] = IIC_Read_Byte();
          IIC_Ack();
      }
    	reg_data[20] = IIC_Read_Byte();
    	IIC_NAck();
      IIC_Stop();*/
    //根据数据手册的数据类型转换
    calib_data.par_t1  = Concat_Bytes(reg_data[1], reg_data[0]);
    calib_data.par_t2  = Concat_Bytes(reg_data[3], reg_data[2]);
    calib_data.par_t3  = (int8_t)reg_data[4];
    calib_data.par_p1  = (int16_t)Concat_Bytes(reg_data[6], reg_data[5]);
    calib_data.par_p2  = (int16_t)Concat_Bytes(reg_data[8], reg_data[7]);
    calib_data.par_p3  = (int8_t)reg_data[9];
    calib_data.par_p4  = (int8_t)reg_data[10];
    calib_data.par_p5  = Concat_Bytes(reg_data[12], reg_data[11]);
    calib_data.par_p6  = Concat_Bytes(reg_data[14],  reg_data[13]);
    calib_data.par_p7  = (int8_t)reg_data[15];
    calib_data.par_p8  = (int8_t)reg_data[16];
    calib_data.par_p9  = (int16_t)Concat_Bytes(reg_data[18], reg_data[17]);
    calib_data.par_p10 = (int8_t)reg_data[19];
    calib_data.par_p11 = (int8_t)reg_data[20];
}

//修正温度
void compensate_temperature()
{
    uint64_t partial_data1;
    uint64_t partial_data2;
    uint64_t partial_data3;
    int64_t  partial_data4;
    int64_t  partial_data5;
    int64_t  partial_data6;
    int64_t  comp_temp;

    partial_data1 = uncomp_data.temperature - (256 * calib_data.par_t1);
    partial_data2 = calib_data.par_t2 * partial_data1;
    partial_data3 = partial_data1 * partial_data1;
    partial_data4 = (int64_t)partial_data3 * calib_data.par_t3;
    partial_data5 = ((int64_t)(partial_data2 * 262144) + partial_data4);
    partial_data6 = partial_data5 / 4294967296;
    calib_data.t_lin = partial_data6;               /* 存储这个data6为t_lin因为计算气压要用到 */
    comp_temp = (int64_t)((partial_data6 * 25)  / 16384);
    //comp_data.temperature = comp_temp;
    comp_data.temperature = comp_temp;
}

//修正气压
void compensate_pressure()
{
    int64_t partial_data1;
    int64_t partial_data2;
    int64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t offset;
    int64_t sensitivity;
    uint64_t comp_press;

    partial_data1 = calib_data.t_lin * calib_data.t_lin;
    partial_data2 = partial_data1 / 64;
    partial_data3 = (partial_data2 * calib_data.t_lin) / 256;
    partial_data4 = (calib_data.par_p8 * partial_data3) / 32;
    partial_data5 = (calib_data.par_p7 * partial_data1) * 16;
    partial_data6 = (calib_data.par_p6 * calib_data.t_lin) * 4194304;
    offset = (calib_data.par_p5 * 140737488355328) + partial_data4 + partial_data5 + partial_data6;

    partial_data2 = (calib_data.par_p4 * partial_data3) / 32;
    partial_data4 = (calib_data.par_p3 * partial_data1) * 4;
    partial_data5 = (calib_data.par_p2 - 16384) * calib_data.t_lin * 2097152;
    sensitivity = ((calib_data.par_p1 - 16384) * 70368744177664) + partial_data2 + partial_data4 + partial_data5;

    partial_data1 = (sensitivity / 16777216) * uncomp_data.pressure;
    partial_data2 = calib_data.par_p10 * calib_data.t_lin;
    partial_data3 = partial_data2 + (65536 * calib_data.par_p9);
    partial_data4 = (partial_data3 * uncomp_data.pressure) / 8192;
    partial_data5 = (partial_data4 * uncomp_data.pressure) / 512;
    partial_data6 = (int64_t)((uint64_t)uncomp_data.pressure * (uint64_t)uncomp_data.pressure);
    partial_data2 = (calib_data.par_p11 * partial_data6) / 65536;
    partial_data3 = (partial_data2 * uncomp_data.pressure) / 128;
    partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
    comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776);
    //comp_data.pressure = comp_press;
    comp_data.pressure = comp_press/1000.0;

}


//BMP388初始化
u8 BMP388_Init()
{
    // IIC_Init();
    //delay_ms(10);
    if(!(BMP388_Read_Byte(0x00)==0x50))   //读取芯片ID
        return 1;													//读取失败返回1
    get_calib_data();											//读取补偿系数
    while(!(BMP388_Read_Byte(0x03)&0x10)) //查询能否写指令   0 0 0 1 0000
        HAL_Delay(3);
    BMP388_Send_Byte(0x7e,0xb6);					//写重置指令，重置全部寄存器。
    HAL_Delay(5);
    if(BMP388_Read_Byte(0x02)&0x07)				//查看错误指示寄存器，有错误返回1
        return 1;
    BMP388_Send_Byte(0x1c,0x00);    			//设置设置温度过采样*1 气压过采样*1  00 000 000
    BMP388_Send_Byte(0x1f,0x04);    			//设置滤波系数2  0000 010 0
    BMP388_Send_Byte(0x1D,0x01);					//设置输出分频系数，请按照相关公式计算得出要写的值
    BMP388_Send_Byte(0x1b,0x03);    			//使能气压和温度采样但是先不启动采样 00 00 00 1 1
    return 0;
}

//获得正确的值
void BMP388_Get_PaT()
{
    get_PandT();
    compensate_temperature();
    compensate_pressure();
}

void BMP388_ON()
{
    BMP388_Send_Byte(0x1b,0x33);    //启动传输 00 11 00 11
}

void BMP388_OFF()
{
    BMP388_Send_Byte(0x1b,0x03);    //进入睡眠 00 00 00 11
}
