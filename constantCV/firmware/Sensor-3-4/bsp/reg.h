#ifndef REG_H_
#define REG_H_
#include "main.h"
#define DAT_HEAD 0x5a
#define REG_SIZE 50
#define CHANGE_REG   0x00
#define CHANGE_DAT 0x01
#define CHANGE_DAT_DEGAULT 0x02

#define REG_X100 			0x0000
#define REG_X10 			0x0001
#define REG_HF16 			0x0002
#define REG_LF16 			0x0003
#define REG_RANGZ_HF16 		0x0104
#define REG_RANGZ_LF16 		0x0105
#define REG_RANGF_HF16 		0x0106
#define REG_RANGF_LF16 		0x0107
#define REG_OFFSET_HF16 	0x0108
#define REG_OFFSET_LF16 	0x0109
#define REG_DEV_ADDR 		0x012c
#define REG_RATE			0x012D
#define REG_CHECK 			0x012E
#define REG_DECM_BIT 		0x012F
#define REG_UNIT			0x0130
#define REG_ADC_RATE 		0x0131  
#define REG_CLR_ZEROE 		0x0023  
#define REG_HF16_4 			0x0045  
#define REG_LF16_4 			0x0046 



#define DEV_ADDR_MAX   247

typedef enum 
{
 epa=0, 			
 eKpa ,			
 eMpa ,			
 emmH2O ,			
 emH2O ,		
 ebar ,		
 epsi, 		
 eatm ,		
 ekg ,	
 emm ,	
 em ,		
 eT,			
 eF 			
} unit_index;			  // cs1237 閫氶亾閫夋嫨


typedef enum 
{
 eREG_X100=0, 			
 eREG_X10 ,			
 eREG_HF16 ,			
 eREG_LF16 ,			
 eREG_RANGZ_HF16 ,		
 eREG_RANGZ_LF16 ,		
 eREG_RANGF_HF16, 		
 eREG_RANGF_LF16 ,		
 eREG_OFFSET_HF16 ,	
 eREG_OFFSET_LF16 ,	
 eREG_DEV_ADDR ,		
 eREG_RATE,			
 eREG_CHECK ,			
 eREG_DECM_BIT ,		
 eREG_UNIT,			
 eREG_ADC_RATE,
 eREG_CLR_ZEROE,
 eREG_HF16_4,
 eREG_LF16_4
} reg_index;			  // cs1237 閫氶亾閫夋嫨

typedef struct
{
 unsigned int reg;
 unsigned int reg_remap;
 unsigned char reg_count;
// unsigned char val_c;
// unsigned int val_u;
// int val_i;
// float val_f;
 uint32_t val_u32ToFloat;
 uint32_t val_u32ToFloat_defaut;
 unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
 unsigned char dat_pos;//0:float hight 16bit 1:float low 16bit
 unsigned char record_flag;//0:not record 1:record
 unsigned char enable;
 
}reg_dat_pack;

typedef struct
{
 reg_dat_pack *pb;
 float zero_val;
 unsigned char update;

}reg_stru;
typedef struct
{
  float offset;//offset
  float zero_value;//before zero value  
  float full_rang;
 float coe;
 float cur_set;//set current 0.4  0.8
 float vol_set;//1:1V
 unsigned char typ;//1:temperature 0:pressure
 unsigned char pga;
 //unsigned char sei;//0:1ma
 unsigned char unit;//0
 //unsigned char factory;
 unsigned char mode;//0;1:factory cali 2:recovery factory
 unsigned char zero_cmd;//0:minus 1:no minus
 unsigned char save;
 float coe1;
 float coe2;
 float coe3;
 float coe4;
 float cal1ADC;
 float cal2ADC;
 float cal3ADC;
 float cal4ADC;
 float cal5ADC;
 float cal1val;
 float cal2val;
 float cal3val;
 float cal4val;
 float cal5val;
 float maskzero;//%

}params_private_stru;

void reg_init(void);
reg_stru*GetReg(void);
void FlshDatToRegDat(reg_dat_pack *pb, uint8_t *dat, unsigned char type);
void FlshDatToRegDat(reg_dat_pack *pb, uint8_t *dat, unsigned char type);
void ModbusDatToRegDat(uint16_t RegNum, uint8_t *dat, unsigned char dir);
void RegDatToModbus(uint16_t RegNum, uint8_t *dat);
void REGDatConvToPC(uint16_t RegNum, uint8_t *dat);
void reg_proc(void);
void ModbusDatToRegFloatDat(reg_dat_pack *pb, uint8_t *dat,
                           unsigned char dat_pos);
void ModbusDatToMulRegDat(uint16_t RegNum, uint8_t *dat, unsigned char dir);
void FlshDatToRegDatNoDefault(reg_dat_pack *pb, uint8_t *dat, unsigned char type);

params_private_stru *GetRegPrivate(void);
void RegWrite(void);

#endif

