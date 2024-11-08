#include "logic/script/scriptinterface.h"

/*************************************************
* Function: 校验函数&和校验
* Parameters: null
* Return: null
*************************************************/
unsigned short sumcheck(unsigned char *pData, int nLen)
{
    unsigned short sum = 0;
    for(int i = 0; i < nLen; i++)
    {
        sum += pData[i];
    }
    return sum;
}

/*************************************************
* Function: init fun
* Parameters: null
* Return: null
*************************************************/
void sendrcv()
{
    /*定义发送数据*/
    QByteArray bytes;

    /*清除缓存区*/
    clearSerialData(1);

    /*发送数据*/
    sendSerialBytes(1, bytes);

    /*延时200ms读取接收缓存,延迟的时间根据项目而定*/
    delayMs(2000);

    /*定义接收缓冲区*/
    bytes.clear();
    bytes = rcvSerialBytes(1);

    /*判断接收长度*/
    if(bytes.size() == 7)
    {
        setRegData(6, bytes[0]);
        setRegData(7, bytes[1]);
        setRegData(8, bytes[2]);
        setRegData(9, bytes[3]);
        setRegData(10, bytes[4]);
        setRegData(11, bytes[5]);
        setRegData(12, bytes[6]);

        ushort nCheck = getSumCheckb(bytes);

        if((bytes[5] == (nCheck & 0xff)) && (bytes[6] == ((nCheck>>8) & 0xff)))
        {
            setRegData(15, 1);
        }
        clearSerialData(1);
    }
}

void led_ctrl(void)
{
    /*定义发送数据*/
    QByteArray bytes;
    unsigned int id,tmp;
    unsigned int sw_status;
    unsigned char sendData[7];
    unsigned char addr_offset,add_end;
    unsigned int i,j;
    id = getRegData(42);
    sendData[0] = id;
    sendData[1] =0x20;
    sendData[4] =0;
    sendData[5] =0;
    sendData[6] =0;

    /*清除缓存区*/
    clearSerialData(1);


    switch(id)
    {
    case 1:
    {
        for(i=1; i<17; i++)
        {
            tmp= getRegData(i);
            tmp = tmp <<(16-i);
            sw_status = sw_status |tmp;

        }
        sendData[2] =sw_status>>8;
        sendData[3] =sw_status;
        sendData[7] =sumcheck(sendData, 7);
        sendSerialData(1, (char*)sendData, 8);	//发送数据
    }
    break;
    default:
    {
        if(id>=2)
        {
            j=(id-2)*16;
            addr_offset = 76+j;
            add_end = addr_offset + 16;
            for(i=addr_offset; i<add_end; i++)
            {
                tmp= getRegData(i);
                tmp = tmp <<(17+addr_offset-i);
                sw_status = sw_status |tmp;
            }
            sendData[2] =sw_status>>8;
            sendData[3] =sw_status;
            sendData[7] =sumcheck(sendData, 7);
            sendSerialData(1, (char*)sendData, 8);//发送数据
        }
    }
    break;

    }

    /*延时200ms读取接收缓存,延迟的时间根据项目而定*/
    delayMs(500);

    /*定义接收缓冲区*/
    bytes.clear();
    bytes = rcvSerialBytes(1);
    ushort nCheck = getSumCheckb(bytes);
    if(bytes.size() != 8||(bytes[7] == (nCheck & 0xff)))
    {
        if(id==1)
        {
            setRegData(39, 1);
        }
        else
        {
            if(id<=29)
            {
                j=(id-2);
                addr_offset = 45+j;
                setRegData(addr_offset, 1);
            }
            else
            {
                j=(id-30);
                addr_offset = 572+j;
                setRegData(addr_offset, 1);
            }
        }
    }


}

