#include "loraHAL.h"
#include "loraHW.h"
#include "Protocol_C.h"
#include "register.h"
#include "nbiotHAL.h"
#include "nbiotHW.h"
#include "modbus.h"
#include "gateway.h"
#include "sensor.h"
unsigned char MODE_STAUS;

loraUart_stru loraUart;
LORAHW_stru lorahw;
LORA_Params_stru loraParams;
loraUart_stru *getloraProcStatus()
{
    return &loraUart;
}

void ParamsInit()
{
    unsigned char *p;
    unsigned int tmp;
    ParamsRead();
    if(ROLE != GATEWAY)
        p = ReadRegister(0x1206);
    else
        p = ReadRegister(0x45f0);
    NbiotUartInit();
    if(ROLE == GATEWAY||getModeStatus()== 0x02)
    {


        ServerIP_Pack(p);
        nbiot_HardwareInit(ON);
    }
    if(ROLE != GATEWAY)
    {
        //  nbiot_HardwareInit(OFF);
        p = ReadRegister(0xf00c);
        tmp = uchar2uint(p);
        sensorModbusRate(tmp,0);
    }

}


unsigned char EquipGateway_Process()
{
    LORAHW_stru loraNo;
    unsigned char * datasrcFlag;
    static unsigned char pcFLag =  0;
    /*******************************************************************
                       lora���ݴ������
    ********************************************************************/
    datasrcFlag = getDataSrc();
    if(loraUart.receivedFlag1 == 1)//lora���ݴ���
    {
        setDataSrc(DAT_FROM_LORA);//��������lora
        if(lorahw.mode ==2)//����ģʽ�յ����ݲ���Э�����
        {
            loraset(6,&(loraUart.lora1RxBuffer[3]),9);
            ParamsSave(0);
            lorahw.mode = 0;
            setDataSrc(0);//��������lora
        }
        else//����ģʽ�յ�������Э�����
        {
            setDataSrc(DAT_FROM_LORA);//��������lora
            if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//У��ɹ�
            {
                if(datasrcFlag[0] == 0||datasrcFlag[0] ==DAT_FROM_LORA)
                {
                    loraNo.loraNo = 0;
                    setDataSrc(DAT_FROM_LORA);//��������lora
                    loraNo.mode =  TransmitMode;
                    if(ROLE != GATEWAY)
                        wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
                    WrRead_equipment(&loraNo);//��д�Ĵ�������
                    Gateway_Process();//����
                    // setDataSrc(0);//��������lora
                }
                if((datasrcFlag[0] == DAT_FROM_RELAY)&&(ROLE == GATEWAY))//����ת�����ݸ��ն˺����Ӧ����
                {
                    unsigned int len;
                    len = ADDR_SIZE + ADDR_SIZE ;
                    len = len + 5 ;
                    len = len + loraUart.lora1RxBuffer[20];
                    if(Get_Network_status()==SIMCOM_NET_OK)
                        SnedToNbiot(loraUart.lora1RxBuffer,len);//�ն�����ͨ������ת����������
                    setDataSrc(0);
                }

            }

        }

        loraUart.receivedFlag1 = 0;

    }
    /*******************************************************************
    				nbiot��pc���Ʒ�ϵͳ���ݴ������
    ********************************************************************/

    MODE_STAUS = getModeStatus();
    unsigned char *nbiot_enable,*data_save;
    if(ROLE == GATEWAY)
    {
        nbiot_enable = ReadRegister(0x45f5);//nbiot 485�Ʒ�ʹ��
        data_save = ReadRegister(0x45f6);//�ڵ����ݴ洢���
    }

    if(MODE_STAUS == 0x02||nbiot_enable[1]==1)//��nbiot�����������ݴ���
    {
        SIMCOM_Register_Network();
        unsigned char *tmp,result;
        tmp = NbiotFrameStatus();//nbiot�������ݱ�־λ

        if(tmp[0] == 1&&Get_Network_status()==SIMCOM_NET_OK)
        {
            setDataSrc(DAT_FROM_NBIOT);//��������nbiot
            result = protocolCAnaly(NbiotFrameBuffer());
            if(result == 0 ||result == 4)//������ȷ��result =0���ݷ��͸��豸����result=4ʱ����Ϊ������Ҫת�����ݣ�
                                            //ת�������� protocolCAnaly(NbiotFrameBuffer());��ʵ��
                setDataSrc(DAT_FROM_NBIOT);//��������nbiot
            else
                setDataSrc(0);//��������nbiot
            if(result	== 0)//У��ɹ�
            {

                pcFLag = 1;
                loraNo.loraNo = 0;
                loraNo.mode =  TransmitMode;


                if(ROLE != GATEWAY)
                    wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
                WrRead_equipment(&loraNo);//��д�Ĵ�������
                Gateway_Process();//������֡�Ĵ�������
                pcFLag =  0;
            }
            else if(result == 4)//��Ϊ���ؽ�������ת�����ն�,�����Ѿ�ת�����
            {
                if(ROLE == GATEWAY)
                    setDataSrc(DAT_FROM_RELAY);
            }
            tmp[0] = 0;
            NbiotUart_CLR_Buf();
        }
    }
    else if(nbiot_enable[1] == 2||ROLE != GATEWAY)//485�Ʒ�ϵͳʹ��
    {
        if(data_save[1]==0&&ROLE == GATEWAY)//ʵʱ����
        {
            setDataSrc(DAT_FROM_PC);
            WrRead_equipment(&loraNo);//��д�Ĵ�������
        }
        unsigned char *fameStatus;
        fameStatus = modbusFrameStatus();
        if(*fameStatus== 2)
        {
            if(protocolCAnaly(modbusBuffer())	== 0)//У��ɹ�,��������pc��
            {

                setDataSrc(DAT_FROM_PC);//��������pc
                loraNo.loraNo = 0;
                loraNo.mode =  TransmitMode;
                if(ROLE != GATEWAY)
                    wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
                WrRead_equipment(&loraNo);//��д�Ĵ�������

            }
            else //���������豸
            {
                if(ROLE != GATEWAY)
                    PCDevice_Modbus_Process();
            }

            Gateway_Process();//������֡�Ĵ�������
            *fameStatus = 0;
        }

    }
    if((getTxmode() ==1)&&ROLE == GATEWAY)//lora���ݶ���ط�����
    {
        if(loraReapte()== 0)
        {
            loraNo.loraNo = 0;
            //setDataSrc(DAT_FROM_LORA);//��������lora
            loraNo.mode =  TransmitMode;
            if(ROLE != GATEWAY)
                wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
            //SendLoraData(&loraNo,p);
            WrRead_equipment(&loraNo);//��д�Ĵ�������
            Gateway_Process();//����

        }

    }

    if(equipmentProcess(pcFLag)==1)
        return 1;//�豸����
    else return 0;
    if(loraUart.receivedFlag2 == 1)
    {

    }
}
