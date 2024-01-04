#include "stc8h1k.h"
#include "intrins.h"

void IapIdle()
{
    IAP_CONTR = 0;                              //关闭IAP功能
    IAP_CMD = 0;                                //清除命令寄存器
    IAP_TRIG = 0;                               //清除触发寄存器
    IAP_ADDRH = 0x80;                           //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

char IapRead(int addr)
{
    char dat;

    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 12;                               //设置等待参数12MHz
    IAP_CMD = 1;                                //设置IAP读命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
    dat = IAP_DATA;                             //读IAP数据
    IapIdle();                                  //关闭IAP功能

    return dat;
}

void IapProgram(int addr, char dat)
{
    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 12;                               //设置等待参数12MHz
    IAP_CMD = 2;                                //设置IAP写命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_DATA = dat;                             //写IAP数据
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
    IapIdle();                                  //关闭IAP功能
}

void IapErase(int addr)
{
    IAP_CONTR = 0x80;                           //使能IAP
    IAP_TPS = 12;                               //设置等待参数12MHz
    IAP_CMD = 3;                                //设置IAP擦除命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();                                    //
    IapIdle();                                  //关闭IAP功能
}

//void eeprom_proc()
//{


//    IapErase(0x0400);
//    P0 = IapRead(0x0400);                       //P0=0xff
//    IapProgram(0x0400, 0x12);
//    P1 = IapRead(0x0400);                       //P1=0x12


//}
unsigned char EEPROM_Read_Byte(unsigned int addr)
{
		return IapRead(addr); 
}
unsigned char EEPROM_Read_Word(u16 addr)
{
   u32 dat;
		dat = IapRead(addr);
		dat = dat <<8;
		dat = dat|IapRead(addr+1);
		dat = dat <<8;
		dat = dat|IapRead(addr+2);
		dat = dat <<8;
		dat = dat|IapRead(addr+3);
	    return dat;
}
unsigned char EEPROM_Read_Inte(u16 addr)
{
   u32 dat;
		dat = IapRead(addr);
		dat = dat <<8;
		dat = dat|IapRead(addr+1);
	    return dat;
}
void EEPROM_Write_Word(u16 addr ,u32 dat)
{
    //u8 i;
	u8 dat_tmp;
	
	IapErase(addr);
	dat_tmp = dat >>24;
    IapProgram(addr, dat_tmp);
	dat_tmp = dat >>16;
	IapProgram(addr+1, dat_tmp);
	dat_tmp = dat >>8;
	IapProgram(addr+2, dat_tmp);
	dat_tmp = dat ;
	IapProgram(addr+3, dat_tmp);

}
void EEPROM_Write_Inte(u16 addr ,u16 dat)
{
//    u8 i;
	u8 dat_tmp;
	
	IapErase(addr);
	dat_tmp = dat >>8;
    IapProgram(addr, dat_tmp);
	dat_tmp = dat ;
	IapProgram(addr+1, dat_tmp);
}
void EEPROM_Write_Byte(u16 addr ,u8 dat)
{
	
	IapErase(addr);
    IapProgram(addr, dat);

}




//	EEPROM_Write_Word(ADDR_REF_P_PLUSEWIDTH_TIME,U32_P_CURRENT_PLUSEWIDTH_TIME);
//	EEPROM_Write_Word(ADDR_REF_V_PLUSEWIDTH_TIME,U32_V_CURRENT_PLUSEWIDTH_TIME); 
//	EEPROM_Write_Word(ADDR_REF_I_PLUSEWIDTH_TIME,U32_I_CURRENT_PLUSEWIDTH_TIME); 
//	EEPROM_Write_Inte(ADDR_REF_001_E,U16_REF_001_E_Pluse_CNT);
//	EEPROM_Write_Word(ADDR_AC_BACKUP_E,0x00); 


