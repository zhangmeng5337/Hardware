


sfr     IAP_TPS     =   0xF5;

void IapIdle()
{
    IAP_CONTR = 0;                              //�ر�IAP����
    IAP_CMD = 0;                                //�������Ĵ���
    IAP_TRIG = 0;                               //��������Ĵ���
    IAP_ADDRH = 0x80;                           //����ַ���õ���IAP����
    IAP_ADDRL = 0;
}

char IapRead(int addr)
{
    char dat;

    IAP_CONTR = 0x80;                           //ʹ��IAP
    IAP_TPS = 12;                               //���õȴ�����12MHz
    IAP_CMD = 1;                                //����IAP������
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    ;
    dat = IAP_DATA;                             //��IAP����
    IapIdle();                                  //�ر�IAP����

    return dat;
}

void IapProgram(int addr, char dat)
{
    IAP_CONTR = 0x80;                           //ʹ��IAP
    IAP_TPS = 12;                               //���õȴ�����12MHz
    IAP_CMD = 2;                                //����IAPд����
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_DATA = dat;                             //дIAP����
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    ;
    IapIdle();                                  //�ر�IAP����
}

void IapErase(int addr)
{
    IAP_CONTR = 0x80;                           //ʹ��IAP
    IAP_TPS = 12;                               //���õȴ�����12MHz
    IAP_CMD = 3;                                //����IAP��������
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    ;                                    //
    IapIdle();                                  //�ر�IAP����
}
void write_add(unsigned int add,unsigned int address,uchar date)
{
	IapErase( address);
	IapProgram(address, date);

}
uchar read_add(uchar add,uchar address)
{
	uchar date;

	date = IapRead(0x0400+address);
	return date;

}
void test()
{


    IapErase(0x0400+address);
    P0 = IapRead(0x0400);                       //P0=0xff
    IapProgram(0x0400, 0x12);
    P1 = IapRead(0x0400);                       //P1=0x12

    while (1);
}


