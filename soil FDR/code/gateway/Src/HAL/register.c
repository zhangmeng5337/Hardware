#include "register.h"
#include "Protocol_C.h"
#include "config.h"
static REG_val_stru register_map;
REG_STATUS_stru reg_status;


unsigned char array_comp(unsigned char *p1,unsigned char *p2,unsigned char len)
{
	unsigned char i;
	unsigned char result;
	result = 0;
	for(i=0;i<len;i++)
	{
		if(p1[i]!=p2[i])
		{
			result = 1;
			break;
		}
		else
			result = 0;
	}
	return result;
}
unsigned int RegisterAddrCal(unsigned int p,unsigned char num)
{
	static uint32_t x1,x12,x4,x44,xe,xf;
	static unsigned char flag;
	if(flag == 0)
	{ 
	/*#define x1_ADDR_SIZE      x1_ADDR_END-x1_ADDR_START+1   //0x101
	#define x12_ADDR_SIZE     x12_ADDR_END-x12_ADDR_START+1 //0x0b
	#define x4_ADDR_SIZE      x44_ADDR_END-x44_ADDR_START+1 //0x3fb
	#define x44_ADDR_SIZE     x44_ADDR_END-x44_ADDR_START+1 //0x120
	#define xE_ADDR_SIZE      xE_ADDR_END-xE_ADDR_START+1  //0x0f
	#define xF_ADDR_SIZE      xF_ADDR_END-xF_ADDR_START+1 //0x0d */                 
		flag = 1;
		x1 = x1_ADDR_START;//0x1001
		x12 = x1_ADDR_SIZE;//0x101
		x4 = x1_ADDR_SIZE+x12_ADDR_SIZE;//0x10c
		x44 = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE;
		xe = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE+x44_ADDR_SIZE;
		xf = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE+x44_ADDR_SIZE+xE_ADDR_SIZE;		
	}
	switch(num)
	{
		case 1:return p-x1;//1x1001-0x0x1102
		case 2:return p+x12-x12_ADDR_START;		//0x1200-0x1209
		case 3:return p+x4-x4_ADDR_START;//0x4001-0x43FC
		case 4:return p+x44-x44_ADDR_START;//0x44f1-0x4610
		case 5:return p+xe-xE_ADDR_START;//0xe000-0xe00F
		case 6:return p+xf-xF_ADDR_START;//0xf001-0xf00e
		case 7:return p+x1;
		case 8:return p+x12_ADDR_START-x12;	
		case 9:return p+x4_ADDR_START-x4;
		case 10:return p+x44_ADDR_START-x44;
		case 11:return p+xE_ADDR_START-xe;
		case 12:return p+xF_ADDR_START-xf;		
		default: return NULL;
	}
 return NULL;
}
unsigned int ReadRegisterOffsetAddr(unsigned int p)
{

   static uint32_t x1,x12,x4,x44,xe,xf;
   static unsigned char flag;
   unsigned char num;
 
    num = 6;
	if(p>=x1_ADDR_START&&p<x12_ADDR_START)
	{
	    num = 1;
		
	}
	else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
	{
		num =2;
	}
	else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
	{
		num =3;
	}
	else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
	{
		num = 4;
	}
	else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
	{
		num =5;
	}
	else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
	{
		num = 6;
	}
	else
		num = 0xff;
	   if(flag == 0)
   {
	   flag = 1;
	   x1 = x1_ADDR_START;
	   x12 = x12_ADDR_START;
	   x4 = x4_ADDR_START;
	   x44 = x44_ADDR_START;
	   xe = xE_ADDR_START;
	   xf = xF_ADDR_START; 
   }
   switch(num)
   {
	   case 1:return p-x1;
	   case 2:return p-x12;	   
	   case 3:return p-x4;
	   case 4:return p-x44;
	   case 5:return p-xe;
	   case 6:return p-xf;	
	   default: return NULL;
   }

}

unsigned char *ReadRegister(unsigned int p)
{
   unsigned int tmp;
   tmp = 0xffff;
	if(p>=x1_ADDR_START&&p<x12_ADDR_START)
	{
	    tmp = RegisterAddrCal(p,1);
		
	}
	else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
	{
		tmp = RegisterAddrCal(p,2);
	}
	else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
	{
		tmp =RegisterAddrCal(p,3);
	}
	else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
	{
		tmp = RegisterAddrCal(p,4);
	}
	else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
	{
		tmp = RegisterAddrCal(p,5);
	}
	else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
	{
		tmp = RegisterAddrCal(p,6);
	}
	else
		tmp = 0xffff;
	if(tmp != 0xffff)
		return &(register_map.value[2*tmp]);
	else
		return NULL;

}
unsigned char ReadOnlyAddr(unsigned int p)
{
 
	if(RegisterAddrCal(p,7)<=0x100a||RegisterAddrCal(p,8)==0x1202||RegisterAddrCal(p,9)==0x4001||RegisterAddrCal(p,9)==0x400a||
		(RegisterAddrCal(p,10)>=0x4600&&RegisterAddrCal(p,10)<=0x460f)||RegisterAddrCal(p,12)>=0xf006||RegisterAddrCal(p,12)>=0xf00b||
		(RegisterAddrCal(p,11)>=0xe000&&RegisterAddrCal(p,11)<=0xe00f))
		return 1;
	else 
		return 0;
}
/*************************************************************
����:д�Ĵ�����������ֻ���Ĵ����жϹ���
������
    @p:�Ĵ�����ַ
    @reg���Ĵ�����ֵ��1�ֽڣ�
    @len:�Ĵ�����ֵ����
**************************************************************/

unsigned char WriteRegisteSet(unsigned int p,unsigned char *reg,unsigned char len)
{
   unsigned int tmp;
   unsigned char i,result;
   if(p>=x1_ADDR_START&&p<x12_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,1);
	   
   }
   else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,2);
   }
   else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
   {
	   tmp =RegisterAddrCal(p,3);
   }
   else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,4);
   }
   else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,5);
   }
   else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
   {
	   tmp = RegisterAddrCal(p,6);
   }


	else
		tmp = 0xffff;
	if(tmp != 0xffff)
	{   
    // tmp addr 
		// tmp:
		//   0: 1001   1002

	    //1:   1003 1004
	    //2:   1005  1006
	    //3:   1007  1008
	    //4    1009 100a   
		for(i = 0;i<len;i++)
		{ 
			  if(i%2==0)
				{
//					if(ReadOnlyAddr(i/2+2*tmp)==1)
//					continue;		
					register_map.value[i+2*tmp] = reg[i];
          register_map.value[i+2*tmp+1] = reg[i+1];					
				}

			
			//register_map.value[i+2*tmp+1] = reg[i];
		}	 

		reg_status.wrStatus = 1;
		reg_status.regAddr = p;
		reg_status.len = len;
		result = 0;
	}
	else
	{
		reg_status.wrStatus = 0;
		reg_status.regAddr = p;
		reg_status.len = len;
		result = 1;

	}
		
		return result;

}
/*************************************************************
����:д�Ĵ�������������ֻ���Ĵ����жϹ���
������
    @p:�Ĵ�����ַ
    @regVal���Ĵ���ֵ��2���ֽ�
**************************************************************/
void WriteOneRegister(unsigned int p,unsigned int regVal)
{
   unsigned int tmp;
   tmp = 0xffff;
   
   if(p>=x1_ADDR_START&&p<x12_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,1);
	   
   }
   else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,2);
   }
   else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
   {
	   tmp =RegisterAddrCal(p,3);
   }
   else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,4);
   }
   else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,5);
   }
   else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
   {
	   tmp = RegisterAddrCal(p,6);
   }


	else
		tmp = 0xffff;

	if(tmp != 0xffff)
	{   

			if(ReadOnlyAddr(tmp)==0)
			{
				register_map.value[tmp*2] = (u8)(regVal>>8);	 
				register_map.value[tmp*2+1] = (u8)(regVal); 

			}
		
	}

}
/*************************************************************
����:д�Ĵ�����������ֻ���жϹ��ܣ�ǿ��д��
������
    @p:�Ĵ�����ַ
    @regVal���Ĵ���ֵ��2���ֽ�
**************************************************************/

void WriteOneRegisterForce(unsigned int p,unsigned int regVal)
{
   unsigned int tmp;
   tmp = 0xffff;
   
   if(p>=x1_ADDR_START&&p<x12_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,1);
	   
   }
   else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,2);
   }
   else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
   {
	   tmp =RegisterAddrCal(p,3);
   }
   else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,4);
   }
   else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,5);
   }
   else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
   {
	   tmp = RegisterAddrCal(p,6);
   }


	else
		tmp = 0xffff;

	if(tmp != 0xffff)
	{   

			{
				register_map.value[tmp*2] = (u8)(regVal>>8);	 
				register_map.value[tmp*2+1] = (u8)(regVal); 

			}
		
	}

}
unsigned int uchar2uint(unsigned char *p)//0   0 1   1  2 3   2  4 5
{
    unsigned int tmp;
	tmp = ((u16)(p[0])<<8)+p[1];
	return tmp;
}

REG_STATUS_stru *readRigsterStatus()//��ȡ�Ĵ���״̬�������д����������Ӧ�Ĳ�������
{
	return &reg_status;
}
void equip_bind(unsigned char *p)
{
    unsigned int i,j;
	unsigned int k;
	unsigned char result;
	unsigned char tmp_id[8];
	result = 0;
	memset(tmp_id,0,8);
	k = 0;

	for(i=0;i<200;i++)
	{
		for(j=i;j<8;j++)
		{
			if(array_comp(p,register_map.value+RegisterAddrCal(0x4010,3)*2+j*8,8) == 0)
			{
				result = 0;
				break;
			}
			else
				result = 1;

			if(result == 1)
			{
				if(array_comp(tmp_id,register_map.value+RegisterAddrCal(0x4010,3)*2+j*8,8) == 0)//�жϻ������Ƿ�Ϊ��
				{
					if((k&0x1000)==0)
					{
						k = i;
						k = k|0x1000;
					}

					//break;
				}
			}		
		}
		if(result == 0)
		break;
	}
		if(result == 1)
		{
			if(register_map.bindCount<200)
			{
				register_map.bindCount = register_map.bindCount+1;
				k = k&0x0fff;
			//	tt = RegisterAddrCal(0x4010,3);
				memcpy(&register_map.value[k*8+RegisterAddrCal(0x4010,3)*2],p,8);
			}
	  }
		

}

unsigned char equip_bind_analy(unsigned char *p)
{
    unsigned int i,j;
	unsigned char result;
	result = 1;
	

	for(i=0;i<register_map.bindCount;i++)
	{
		for(j=i;j<8;j++)
		{
			if(array_comp(p,register_map.value+RegisterAddrCal(0x4010,3)*2+j*8,8) == 0)
			{
				result = 0;
				break;
			}
			else
				result = 1;
		}
		if(result == 0)
			break;
		//if(result == 1)
		//	break;
	}
		return result;

}

unsigned char equip_bind_delete(unsigned char *p)
{
    unsigned int i,j;
	unsigned char result;
	result = 1;
	

	for(i=0;i<register_map.bindCount;i++)
	{
		for(j=i;j<8;j++)
		{
			if(array_comp(p,register_map.value+RegisterAddrCal(0x4010,3)*2+j*8,8) == 0)
			{
				result = 0;
				memset(&register_map.value[RegisterAddrCal(0x4010,3)*2+j*8],0,8);
				register_map.bindCount = register_map.bindCount -1;

				break;
			}
			else
				result = 1;
		}
		if(result == 0)
		break;
	}
		return result;

}


void register_init()
{
        unsigned char *q;
	    WriteOneRegisterForce(0xf001,0x0001);//01H�����ܼ�������� 02H���ܼ��������
		WriteOneRegisterForce(0xf002,3);//10-1000min����	
		WriteOneRegisterForce(0xf003,4);//10-120min����
		WriteOneRegisterForce(0xf004,60);//0-60s����;��������ģ�鼤�ʱʱ��
		WriteOneRegisterForce(0xf005,8);//����ģ�鹦������
		//								01H��10%���������
		//								02H��20%���������
		//								03H��30%���������
		//								04H��40%���������
		//								05H��50%���������
		//								06H��60%���������
		//								07H��70%���������
		//								08H��80%���������
		//								09H��90%���������
		//								10H�����������


		WriteOneRegisterForce(0xf006,10);//0-100 ��Ӧ 0%-100%����
		WriteOneRegisterForce(0xf007,((u16)(getConfig()->srcaddr[0])<<8)+getConfig()->srcaddr[1]);//�ڵ��ַ��2�ֽ�
		WriteOneRegisterForce(0xf008,((u16)(getConfig()->srcaddr[2])<<8)+getConfig()->srcaddr[3]);//
		WriteOneRegisterForce(0xf009,((u16)(getConfig()->srcaddr[4])<<8)+getConfig()->srcaddr[5]);//
		WriteOneRegisterForce(0xf00a,((u16)(getConfig()->srcaddr[6])<<8)+getConfig()->srcaddr[7]);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
		q = ReadRegister(0xf00a);
		WriteOneRegisterForce(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���������ַ,��0xf00a��ͬ
		WriteOneRegisterForce(0xf00c,2);//ũҵ�ɼ������ն��봫����ͨ�ŵ�Modbus������
		//������(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200

		WriteOneRegisterForce(0xf00d,0);//ũҵ�ɼ������ն���������ӣ����üĴ�����Modbus������
		//������(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200
		WriteOneRegisterForce(0xf00e,0);//�����ϱ�


		WriteOneRegisterForce(0x1001,30);//��5���¶�
		WriteOneRegisterForce(0x1002,40);//��5��ʪ��
		WriteOneRegisterForce(0x1003,31);//��4���¶�
		WriteOneRegisterForce(0x1004,41);//��4��ʪ��
		WriteOneRegisterForce(0x1005,32);//��3���¶�
		WriteOneRegisterForce(0x1006,42);//��3��ʪ��
		WriteOneRegisterForce(0x1007,33);//��2���¶�
		WriteOneRegisterForce(0x1008,43);//��2��ʪ��
		WriteOneRegisterForce(0x1009,34);//��1���¶�
		WriteOneRegisterForce(0x100a,44);//��1��ʪ��

		WriteOneRegisterForce(0xe000,44);//����
		WriteOneRegisterForce(0xe003,47);//γ��

		WriteOneRegisterForce(0x1200,((u16)(getConfig()->destaddr[0])<<8)+getConfig()->destaddr[1]);//Ŀ������
		WriteOneRegisterForce(0x1201,((u16)(getConfig()->destaddr[2])<<8)+getConfig()->destaddr[3]);//Ŀ������
		WriteOneRegisterForce(0x1202,((u16)(getConfig()->destaddr[4])<<8)+getConfig()->destaddr[5]);//Ŀ������
		WriteOneRegisterForce(0x1203,((u16)(getConfig()->destaddr[6])<<8)+getConfig()->destaddr[7]);//Ŀ������
		WriteOneRegisterForce(0x1205,getConfig()->airrate);//����
		WriteOneRegisterForce(0x1206,getConfig()->channel);//�ŵ�
	
	
		WriteOneRegisterForce(0x1207,getConfig()->TcpServer[0]);//������ip H
		WriteOneRegisterForce(0x1208,getConfig()->TcpServer[1]);//������ip H
		WriteOneRegisterForce(0x1209,getConfig()->TcpServer[2]);//������ip H
		WriteOneRegisterForce(0x120a,getConfig()->TcpServer[3]);//������ip H
		WriteOneRegisterForce(0x120b,getConfig()->TcpServerPort);//�������˿ں�	

		if(ROLE == GATEWAY)
		{
			WriteOneRegisterForce(0x1200,((u16)(getConfig()->srcaddr[0])<<8)+getConfig()->srcaddr[1]);//Ŀ������
			WriteOneRegisterForce(0x1201,((u16)(getConfig()->srcaddr[2])<<8)+getConfig()->srcaddr[3]);//Ŀ������
			WriteOneRegisterForce(0x1202,((u16)(getConfig()->srcaddr[4])<<8)+getConfig()->srcaddr[5]);//Ŀ������
			WriteOneRegisterForce(0x1203,((u16)(getConfig()->srcaddr[6])<<8)+getConfig()->srcaddr[7]);//Ŀ������

			WriteOneRegisterForce(0x4006,((u16)(getConfig()->srcaddr[0])<<8)+getConfig()->destaddr[1]);//����
			WriteOneRegisterForce(0x4007,((u16)(getConfig()->srcaddr[2])<<8)+getConfig()->destaddr[3]);//����
			WriteOneRegisterForce(0x4008,((u16)(getConfig()->srcaddr[4])<<8)+getConfig()->destaddr[5]);//����
			WriteOneRegisterForce(0x4009,((u16)(getConfig()->srcaddr[6])<<8)+getConfig()->destaddr[7]);//����
			WriteOneRegisterForce(0x400a,((u16)(getConfig()->srcaddr[6])<<8)+getConfig()->destaddr[7]);//����
			WriteOneRegisterForce(0x4409,((u16)(getConfig()->srcaddr[6])<<8)+getConfig()->destaddr[7]);//����lora��ַ
			WriteOneRegisterForce(0x44f1,getConfig()->airrate);//���ؿ���
			WriteOneRegisterForce(0x44f2,getConfig()->channel);//�����ŵ�

			WriteOneRegisterForce(0x45f0,getConfig()->TcpServer[0]);//������ip
			WriteOneRegisterForce(0x45f1,getConfig()->TcpServer[1]);//������ip
			WriteOneRegisterForce(0x45f2,getConfig()->TcpServer[2]);//������ip
			WriteOneRegisterForce(0x45f3,getConfig()->TcpServer[3]);//������ip		
			WriteOneRegisterForce(0x45f4,getConfig()->TcpServerPort);//�������˿ں�	
			WriteOneRegisterForce(0x45f5,1);//nbiotʹ��
			WriteOneRegisterForce(0x45f6,1);//�����л�

			//WriteOneRegister(0x1006,57);//�����ϱ�

		}

}
REG_val_stru *getRegAddr()
{
	return &register_map;
}
void loratestInit()
{
	unsigned char *q;
	  if(LORA_TX == 0)
		{
			WriteOneRegister(0xf00a,2);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���ص�ַ
			WriteOneRegister(0x1202,3);//Ŀ���ַ
			WriteOneRegister(0x1204,30);//�ŵ�		
		}
		else
		{
			WriteOneRegister(0xf00a,1);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���ص�ַ
			WriteOneRegister(0x1202,2);//Ŀ���ַ
			WriteOneRegister(0x1204,30);//�ŵ�			
		}

}

