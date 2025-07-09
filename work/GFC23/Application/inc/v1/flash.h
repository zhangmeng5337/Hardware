#ifndef FLASH_USR_H_
#define FLASH_USR_H_
#include "main.h"
#define READ  0
#define WRITE 1

#define GAS_SET     0
#define NOZZLE_SET  1
#define PERROR_SET  2

#define GAS_PKT_SIZE  40
#define NOZZ_PKT_SIZE 32

#define GAS_MAX_SIZE PAGESIZE/GAS_PKT_SIZE  

#define GAS_MAX_ADDR GAS_MAX_SIZE*GAS_PKT_SIZE
#define NOPARAMS   0
#define SUCESS    1
#define INVALID_ID    2

typedef struct
{
	uint32_t gas_used_len;//gas params len
	uint32_t press_used_len;//p20 len
	
	uint32_t available_noz_len;//   
	uint32_t available_gas_len;//
	uint32_t available_pressure_len;//
	
	uint32_t total_gas_len;
	uint32_t total_noz_len;
	uint32_t total_pressure_len;
	
	uint32_t addr_gas;//gas params
	uint32_t addr_pressure;//p20
	uint32_t addr_noz;//noz 

     uint32_t total_gas_count;
	 uint32_t nozzle_status;
	uint32_t v_status;
	uint32_t bp_status;
	uint32_t perror_status;


	 
	uint32_t header;
	unsigned char status;
}flash_stru;
void flash_proc(unsigned char wr,unsigned char mode);
unsigned char get_flash_status(void);
unsigned char gas_set_proc(unsigned char oper);
unsigned char nozzle_set_proc(unsigned char oper);
unsigned char boot_params_proc(unsigned char oper);
void flash_init(void);
flash_stru *get_flash_stru(void);

#endif

