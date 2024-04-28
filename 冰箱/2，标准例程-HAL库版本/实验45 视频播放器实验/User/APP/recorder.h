#ifndef __RECORDER_H
#define __RECORDER_H
#include "./SYSTEM/SYS/sys.h"
#include "./FATFS/source/ff.h"
#include "./AUDIOCODEC/wav/wavplay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//录音机 应用代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	
  

#define I2S_RX_DMA_BUF_SIZE     4096        /* 定义RX DMA 数组大小 */
#define REC_SAMPLERATE          16000       /* 采样率,44.1Khz */

void rec_i2s_dma_rx_callback(void);
void recoder_enter_rec_mode(void);
void recoder_wav_init(__WaveHeader* wavhead);
void recoder_msg_show(uint32_t tsec,uint32_t kbps);
void recoder_remindmsg_show(uint8_t mode);
void recoder_new_pathname(uint8_t *pname); 
void wav_recorder(void);

#endif












