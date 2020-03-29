#include "machine.h"
extern UART_HandleTypeDef huart1;
//本程序是 Arduino 的示例程序，其他 MCU 请移植。
void Receive_command()
{
    unsigned char inByte,temp;
    if(Serial.available() > 0)
    {
        inByte = Serial.read();
        if(inByte==0xAA)
        {
            Receive_item=0;
        }
        Receive_data_temp[Receive_item]=inByte;
        Receive_item++;
        if(Receive_item>15)
        {
            Receive_item=0;
        }
        if(inByte==0x55)
        {
            temp=0; //对接受的数据进行累计校验
            for(int i=0; i<14; i++)
            {
                temp += Receive_data_temp[i];
            }
            if(temp==Receive_data_temp[14])
            {
                for(int j=0; j<16; j++)
                {
                    Receive_data[j]=Receive_data_temp[j];
                } //如果接受到的数据正确则 copy 到 Receive_data 数组中 }Uart.Receive_flag=1; } } }

                void machine_speed(uint32_t pspeed)
                {

                }
