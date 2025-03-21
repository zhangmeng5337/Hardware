#ifndef REG_H_
#define REG_H_
#include "main.h"
#define DAT_HEAD 0x5a
#define REG_SIZE 50
#define CHANGE_REG   0x00
#define CHANGE_DAT 0x01

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
 eREG_X100, 			
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
 eREG_ADC_RATE 		
} reg_index;			  // cs1237 閫氶亾閫夋嫨

typedef struct
{
 unsigned int reg;
 unsigned int reg_remap;
 unsigned char reg_count;
 unsigned char val_c;
 unsigned int val_u;
 int val_i;
 float val_f;
 uint32_t val_u32ToFloat;
 unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
 unsigned char dat_pos;//0:float hight 16bit 1:float low 16bit
 unsigned char record_flag;//0:not record 1:record
 
}reg_dat_pack;

typedef struct
{
 reg_dat_pack *pb;
 float zero_val;
 unsigned char update;

}reg_stru;
typedef struct
{
  float offset;//before zero value
  float full_rang;
 float coe;
 float cur_set;//1:1ma
 float vol_set;//1:1V
 unsigned char typ;//0:temperature 1:pressure
 unsigned char pga;
 unsigned char sei;//0:1ma
 unsigned char unit;//0
 float coe1;
 float coe2;
 float coe3;
 float coe4;
}params_private_stru;

void reg_init(void);
reg_stru*GetReg(void);
void DatConvToModbus(uint16_t RegNum, uint8_t *dat);
void ModbusToDatConv(uint16_t RegNum, uint8_t *dat);
void reg_proc(void);
void ModbusSetReg(uint16_t RegNum, uint8_t *dat);
void REGDatConvToPC(uint16_t RegNum, uint8_t *dat);
void RegDatConv(uint16_t RegNum, uint8_t *dat, unsigned char dir);
params_private_stru *GetRegPrivate(void);
void RegWrite(void);

#endif

