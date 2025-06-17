#ifndef REG_H_
#define REG_H_
#include "main.h"
#define DAT_HEAD 0x5a
#define REG_SIZE 16

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
} reg_index;			  // cs1237 通道选择

typedef struct
{
 unsigned int reg;
 unsigned char reg_count;
 unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
 unsigned char val_c;
 unsigned int val_u;
 int val_i;
 float val_f;
 unsigned char record_flag;//0:not record 1:record
}reg_dat_pack;

typedef struct
{
 reg_dat_pack *pb;

}reg_stru;
void reg_init(void);

#endif

