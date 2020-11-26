#include "register.h"
#include "Protocol_C.h"

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
		flag = 1;
		x1 = x1_ADDR_START;
		x12 = x1_ADDR_SIZE;
		x4 = x1_ADDR_SIZE+x12_ADDR_SIZE;
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
功能:写寄存器操作，无只读寄存器判断功能
参数：
    @p:寄存器地址
    @reg：寄存器数值（1字节）
    @len:寄存器数值长度
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
功能:写寄存器操作，带有只读寄存器判断功能
参数：
    @p:寄存器地址
    @regVal：寄存器值（2）字节
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
功能:写寄存器操作，无只读判断功能，强制写入
参数：
    @p:寄存器地址
    @regVal：寄存器值（2）字节
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

REG_STATUS_stru *readRigsterStatus()//读取寄存器状态，如果有写操作，做相应的操作处理
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
				if(array_comp(tmp_id,register_map.value+RegisterAddrCal(0x4010,3)*2+j*8,8) == 0)//判断缓冲区是否为空
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
				uint32_t tt;
				tt = RegisterAddrCal(0x4010,3);
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
	    WriteOneRegisterForce(0xf001,0x0001);//01H：非密集监测周期 02H：密集监测周期
		WriteOneRegisterForce(0xf002,3);//10-1000min设置	
		WriteOneRegisterForce(0xf003,2);//10-120min设置
		WriteOneRegisterForce(0xf004,1);//0-60s设置;设置无线模组激活超时时间
		WriteOneRegisterForce(0xf005,8);//无线模组功耗设置
		//								01H：10%最大功率设置
		//								02H：20%最大功率设置
		//								03H：30%最大功率设置
		//								04H：40%最大功率设置
		//								05H：50%最大功率设置
		//								06H：60%最大功率设置
		//								07H：70%最大功率设置
		//								08H：80%最大功率设置
		//								09H：90%最大功率设置
		//								10H：满功率输出
		WriteOneRegisterForce(0xf006,10);//0-100 对应 0%-100%电量
		WriteOneRegisterForce(0xf007,0);//节点地址高2字节
		WriteOneRegisterForce(0xf008,0);//
		WriteOneRegisterForce(0xf009,0);//
		WriteOneRegisterForce(0xf00a,2);//本地节点地址低2字节LORA物理地址
		q = ReadRegister(0xf00a);
		WriteOneRegisterForce(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA本地物理地址,与0xf00a相同
		WriteOneRegisterForce(0xf00c,2);//农业采集控制终端与传感器通信的Modbus波特率
		//波特率(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200

		WriteOneRegisterForce(0xf00d,0);//农业采集控制终端与电脑连接，配置寄存器的Modbus波特率
		//波特率(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200
		WriteOneRegisterForce(0xf00e,0);//主动上报


		WriteOneRegisterForce(0x1001,30);//第5层温度
		WriteOneRegisterForce(0x1002,40);//第5层湿度
		WriteOneRegisterForce(0x1003,31);//第4层温度
		WriteOneRegisterForce(0x1004,41);//第4层湿度
		WriteOneRegisterForce(0x1005,32);//第3层温度
		WriteOneRegisterForce(0x1006,42);//第3层湿度
		WriteOneRegisterForce(0x1007,33);//第2层温度
		WriteOneRegisterForce(0x1008,43);//第2层湿度
		WriteOneRegisterForce(0x1009,34);//第1层温度
		WriteOneRegisterForce(0x100a,44);//第1层湿度

		WriteOneRegisterForce(0xe000,44);//经度
		WriteOneRegisterForce(0xe003,47);//纬度

		WriteOneRegisterForce(0x1200,0);//目标网关
		WriteOneRegisterForce(0x1201,0);//目标网关
		WriteOneRegisterForce(0x1202,0);//目标网关
		WriteOneRegisterForce(0x1203,3);//目标网关
		WriteOneRegisterForce(0x1205,4);//空速
		WriteOneRegisterForce(0x1206,30);//信道
		
		WriteOneRegisterForce(0x1207,192);//服务器ip H
		WriteOneRegisterForce(0x1208,168);//服务器ip H
		WriteOneRegisterForce(0x1209,1);//服务器ip H
		WriteOneRegisterForce(0x120a,5);//服务器ip H
		WriteOneRegisterForce(0x120b,80);//服务器端口号	

		if(ROLE == GATEWAY)
		{
			WriteOneRegisterForce(0x4006,0);//网关
			WriteOneRegisterForce(0x4007,0);//网关
			WriteOneRegisterForce(0x4008,0);//网关
			WriteOneRegisterForce(0x4009,2);//网关
			WriteOneRegisterForce(0x400a,2);//网关
			WriteOneRegisterForce(0x4409,2);//网关lora地址
			WriteOneRegisterForce(0x44f1,4);//网关空速
			WriteOneRegisterForce(0x44f2,30);//网关信道

			WriteOneRegisterForce(0x45f0,192);//服务器ip
			WriteOneRegisterForce(0x45f1,168);//服务器ip
			WriteOneRegisterForce(0x45f2,1);//服务器ip
			WriteOneRegisterForce(0x45f3,3);//服务器ip		
			WriteOneRegisterForce(0x45f4,3);//服务器端口号	
			WriteOneRegisterForce(0x45f5,1);//nbiot使能
			WriteOneRegisterForce(0x45f6,1);//数据切换

			//WriteOneRegister(0x1006,57);//主动上报

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
			WriteOneRegister(0xf00a,2);//本地节点地址低2字节LORA物理地址
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA本地地址
			WriteOneRegister(0x1202,3);//目标地址
			WriteOneRegister(0x1204,30);//信道		
		}
		else
		{
			WriteOneRegister(0xf00a,1);//本地节点地址低2字节LORA物理地址
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA本地地址
			WriteOneRegister(0x1202,2);//目标地址
			WriteOneRegister(0x1204,30);//信道			
		}

}

