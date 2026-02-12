#ifndef ADC_H_
#define ADC_H_
#include "main.h"
//adc 350khz
#define FREQ		1000
//#define RATIO      998400/FREQ-1

#define RATIO      10000000/FREQ-1
#define PERIOD		1.0/FREQ  //959999999
#define ADC_FREQ	357200



#define VREF_VDDA 3.165
#define ADC_BUF_SIZE  30720*3
#define PACKET_SIZE 5
typedef struct
{
	float adc_smple_freq;
	//float uart_txbuffer[ADC_BUF_SIZE];
	unsigned char adc_voltage[ADC_BUF_SIZE];	
	//uint16_t adc_copy[512];

	//unsigned char adc_buf_full;
	//unsigned char sample_buf_full;
	unsigned int sample_point_interval;
	uint16_t adc_sample[3];
	uint32_t index_tx;
	//uint32_t index;
	uint32_t real_index;

	uint32_t count;
	float vref_adc;
	float vol;
	float freq;
	uint32_t len;
	//uint32_t tick_start;
	float tick;
	float sum_count;
	unsigned char update;
	unsigned char conv_flag;
	unsigned char tx_state;
	unsigned char tx_flag;
	uint32_t len_total;
	//uint32_t len_tmp;
}adc_stru;

void adc_copy(void);
adc_stru *get_adc_data(void);
void adc_ctrl(unsigned char ctrl);
void adc_proc(void);
void adc_dma(unsigned char flag);

void adc_dma_start(void);
void timer_ctrl(unsigned char flag);

#endif

