#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
#include "FdrAlgorithm.h"
extern factor_stru factor_usr;//湿度算法系数
extern modbus_stru modbus_usr;//modbus数据
extern sensor_stru sensor_usr;//adc采集转换数据
/************************************************
功能：存储关键参数
*************************************************/
void fator_save_proc(unsigned char addr)
{
    uint32_t tmp;
//	  if(factor_usr.a0<=99&&factor_usr.a0>=-99)
//			tmp = FloatToCharProc(factor_usr.a0,10000);//湿度系数1
//		else
    tmp = FloatToCharProc(factor_usr.a0,1000);//湿度系数1
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

//	  if(factor_usr.a1<=99&&factor_usr.a1>=-99)
//			tmp = FloatToCharProc(factor_usr.a1,10000);//湿度系数1
//		else
    tmp = FloatToCharProc(factor_usr.a1,1000);//湿度系数1
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

//	  if(factor_usr.a2<=99&&factor_usr.a2>=-99)
//			tmp = FloatToCharProc(factor_usr.a2,10000);//湿度系数1
//		else
    tmp = FloatToCharProc(factor_usr.a2,1000);//湿度系数1
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(sensor_usr.ADC_REF,1000);//参考电压
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(sensor_usr.CalibrationProbeVref,1000);//探头空测电压
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    flash_write(addr++,factor_usr.status,1);//校准完成标志位
    flash_write(addr++,factor_usr.dutycycle,1); //pwm

}
/**************************************************************
传感器参数初始化
***************************************************************/
void params_init()
{
    uint32_t p[FLASH_BUFFER_SIZE];

    unsigned char addr;
    addr = 0;
    flash_read(addr,p,FLASH_BUFFER_SIZE-1);
    modbus_usr.RS485_Addr =0x01;
    modbus_usr.RS485_Baudrate = 9600;
    modbus_usr.RS485_Parity = 0;
    sensor_usr.CalibrationT = 0;
    sensor_usr.CalibrationR = 0;
    if(p[0]==0x0000005a)//参数存储过，直接读取参数
    {
        addr = 1;
        modbus_usr.RS485_Addr=p[addr++];
        modbus_usr.RS485_Baudrate=p[addr++];
        modbus_usr.RS485_Parity=p[addr++];


        factor_usr.a0 = DataMinusProc(&p[addr],1,1000);
        addr = addr + 4;
        factor_usr.a1 = DataMinusProc(&p[addr],1,1000);
        addr = addr + 4;
        factor_usr.a2 = DataMinusProc(&p[addr],1,1000);
        addr = addr + 4;
        if(p[addr]!=0xffffffff)
            sensor_usr.ADC_REF = DataMinusProc(&p[addr],1,1000);
        addr = addr + 4;
        if(p[addr]!=0xffffffff)
            sensor_usr.CalibrationProbeVref = DataMinusProc(&p[addr],1,1000);
        addr = addr + 4;
        if(p[addr]!=0xffffffff)
            factor_usr.status = p[addr];
        addr = addr + 1;
        if(p[addr]!=0xffffffff)
            factor_usr.dutycycle = p[addr];
        addr = addr + 1;
        if(p[addr]!=0xffffffff)
            sensor_usr.CalibrationT = p[addr];
    }
    else
    {
        //初始参数
        flash_init();
        factor_usr.a0 = 82.93;
        factor_usr.a1 = -27.688;
        factor_usr.a2 = 12.822;
        factor_usr.status = 0;
        factor_usr.dutycycle = 9;
        flash_write(addr++,0x5a,1);
        flash_write(addr++,modbus_usr.RS485_Addr,1);
        flash_write(addr++,modbus_usr.RS485_Baudrate,1);
        flash_write(addr++,modbus_usr.RS485_Parity,1);
        fator_save_proc(addr);
        flash_write(addr+12+4+4+1+1,sensor_usr.CalibrationT,1);
    }


}
void params_save()
{
    unsigned char addr;
    addr = 0;
    flash_init();
    flash_write(addr++,0x5a,1);
    flash_write(addr++,modbus_usr.RS485_Addr,1);
    flash_write(addr++,modbus_usr.RS485_Baudrate,1);
    flash_write(addr++,modbus_usr.RS485_Parity,1);
    fator_save_proc(addr);
    flash_write(addr+12+4+4+1+1,sensor_usr.CalibrationT,1);		//温度校准
    //flash_write(addr+12+4+4+1,factor_usr.status,1); 	//校准完成标志位

}
void app_loop()
{

    RS485_Service();//主程序
}
