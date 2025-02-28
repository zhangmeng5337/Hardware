#include "sensor.h"
#include "register.h"
#include "modbus.h"
#include "rtc.h"
#include "gps.h"
#include "loraHW.h"
#include "adc.h"
#include "nbiotHAL.h"

extern unsigned char modbus_role;
unsigned char getModeStatus()
{
    unsigned char result;
    result = 0;
    if(HAL_GPIO_ReadPin(GPIOA, MODE2_Pin))
        result = result |0x02;
    else
        result = result &0xfd;

    if(HAL_GPIO_ReadPin(GPIOA, MODE1_Pin))
        result = result |0x01;
    else
        result = result &0xfe;
    return   result;
}
void getRH_T()//获取温湿度传感器数据
{
    modbus_pack_stru p;
    static uint32_t timeout;
    modbus_stru *tmp;
    unsigned char result[4];
    static unsigned char index;
    index = 0;

    p.RS485_Addr = 0;
    while(p.RS485_Addr<=4)
    {

        p.RS485_Addr ++;
        p.func = 0x03;
        p.regnum = 0x0002;
        p.startaddr = 0x0002;
        modbus_role = 1;
        Modbus_Pack(p);
        timeout = HAL_GetTick();


        while(tmp->RS485_FrameFlag !=2)
        {
            tmp=modbusRxResult();
            if((HAL_GetTick()-timeout)>=500)
            {
                result[0] = 0;
                result[1] = 0;
                result[2] = 0;
                result[3] = 0;

                break;
            }

        }
        PCDevice_Modbus_Process();
        if(modbus_role == 2)
        {
            tmp=modbusRxResult();
            unsigned char *fameStatus;
            fameStatus = modbusFrameStatus();
            *fameStatus = 0;
            result[0] =	tmp->RS485_RX_BUFF[5];
            result[1] = tmp->RS485_RX_BUFF[6];
            result[2] = tmp->RS485_RX_BUFF[3];
            result[3] = tmp->RS485_RX_BUFF[4];
            WriteRegisteSet(0x1001+index,result,4);
            index = index + 2;

        }

        // index = index +4;

    }
    modbus_role = 0;

}
void getvbat()
{
    unsigned char *p;

    p = getadc();
    p[1] = p[0];
    p[0] = 0;
    WriteRegisteSet(0xf006,p,2);
}

void getGPS()//获取gps位置信息
{
    //unsigned char *p;
    //p = (unsigned char *)malloc(60);
    //p =parseGpsBuffer();
    //if(p!=NULL)
    //WriteRegisteSet(0xe0000,p,8);
    //else
    //	free(p);
    //free(p);
}
void  actuator(unsigned int p,unsigned char len)
{
    unsigned char *q;
    unsigned char i;
    modbus_pack_stru modbus;

    for(i = 0; i<len/2; i++)
    {
        modbus.RS485_Addr = 0x01+i;
        modbus.func = 0x06;
        modbus.startaddr = p+i;
        q = ReadRegister(p+i);
        modbus.regnum = q[1];
        Modbus_Pack(modbus);

    }
}


void equip_set()//网关写命令后，对相关指令进行具体的执行
{
    REG_STATUS_stru *val;
    unsigned char *p,result;

    result = 1;
    val = readRigsterStatus();
    if(val->wrStatus == 1)//判断是否有寄存器写入

    {
        p = ReadRegister(val->regAddr);
        if(val->regAddr>=0xf001&&val->regAddr<=0xf003)
        {
            result = 0;
            p = ReadRegister(0xf001);//设置休眠唤醒时间
            if(p[1] == 0x01)
            {

                SetWakeUp(ReadRegister(0xf002));
            }
            else
                SetWakeUp(ReadRegister(0xf003));
        }
        else if(val->regAddr==0xf004)//无线模组超时时间时间
        {
            result = 0;
            ;//loraset(0,p,val->len);
        }
        else if(val->regAddr==0xf005)//功率设置
        {
            result = 0;
            loraset(1,p,val->len);
        }
        else if(val->regAddr>=0xf00c)//终端与传感器的modbus速率
        {
            result = 0;
            sensorModbusRate(uchar2uint(p),0);
            //loraSend(LORAHW_stru *p,unsigned char *buffer,unsigned int len);

        }
        else if(val->regAddr>=0xf00d)//终端与电脑的modbus速率
        {
            result = 0;
            sensorModbusRate(uchar2uint(p),1);
            //loraSend(LORAHW_stru *p,unsigned char *buffer,unsigned int len);

        }
        else if(val->regAddr==0x1205)//空中速率
        {
            result = 0;
            loraset(2,p,val->len);
        }

        else if(val->regAddr==0x44f1)//w网关空中速率
        {
            result = 0;
            loraset(2,p,val->len);
        }
        else if(val->regAddr==0x1206)//lora信道0-127
        {
            result = 0;
            loraset(3,p,val->len);
        }
        else if(val->regAddr==0x44f2)//网关lora信道0-127
        {
            result = 0;
            loraset(3,p,val->len);
        }

        else if(val->regAddr>=0x1207&&val->regAddr<=0x120b)//设置ip地址
        {
            result = 0;
            ServerIP_Pack(ReadRegister(val->regAddr));
        }
        else if(val->regAddr>=0x45f0&&val->regAddr<=0x45f4)//网关设置ip地址
        {
            result = 0;
            ServerIP_Pack(ReadRegister(val->regAddr));
        }

        else if(val->regAddr>=0x1010&&val->regAddr<=0x1012)//设置执行机构
        {
            result = 0;
            actuator(val->regAddr,val->len);
        }
        else if(val->regAddr>=0x1100&&val->regAddr<=0x1102)//设置时间
        {
            result = 0;
            setRtc(p, 3);

        }

        val->wrStatus = result;


    }
}

