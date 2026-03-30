#ifndef	_APP_FIFO_H
#define	_APP_FIFO_H

#include <stdint.h>

/**************** 用户任务声明 *******************/

typedef struct {
	uint8_t* buffer;	//FIFO的缓存数组
	uint16_t in;			//缓存数组的下标，in指向要存入数据的地址
	uint16_t out;			//缓存数组的下标，out指向要读出数据的地址
	uint16_t size;		//FIFO缓存数组长度
}FIFO_Type;

void Fifo_Init(FIFO_Type* fifo, uint8_t* buffer, uint16_t size);

uint16_t Fifo_Get(FIFO_Type* fifo, uint8_t* buffer, uint16_t len);

uint16_t Fifo_Set(FIFO_Type* fifo, uint8_t* buffer, uint16_t len);

uint16_t Fifo_Status(FIFO_Type* fifo);
/**
 ******************************************************************************
 * @文件名 	  fifo.h
 * @作者	  Jing
 * @版本	  V01
 * @日期	  2023-08-25
 * @作用	
 ******************************************************************************
 * 修改记录:
 * 日期           修改者          修改内容
 * 2023-08-25	  Jing    		   1.0版
 ******************************************************************************
 * @注意
 *
 * Copyright (C) 2023-2025
 * All rights reserved Jing
 *
 ******************************************************************************/
 
#ifndef FIFO_H
#define FIFO_H
 
#include <stdbool.h>
#include <stdint.h>


#define QUEUE_SIZE        1280
#define FRAME_START_BYTE1 0x7E  // 帧起始字节1
#define FRAME_START_BYTE2 0x01  // 帧起始字节2
#define FRAME_START_BYTE3 0x09  // 帧起始字节3
#define FRAME_START_BYTE4 0x60 // 帧起始字节4
#define FRAME_START_BYTE5 0x01  // 帧起始字节5
#define misalignment_len  2     // 长度字节偏移量
#define miniframe_len     9     // 最小帧字节数
 
typedef struct {
    uint8_t data[QUEUE_SIZE];
    uint16_t start;
    uint16_t end;
    uint16_t count;
} FifoQueue;
 
 
extern FifoQueue uart1_rec;
extern FifoQueue uart2_rec;
extern FifoQueue uart1_tran;
extern FifoQueue uart2_tran;
 
 
void fifo_init(void);
void fifo_clear(FifoQueue* queue);
bool fifo_is_empty(FifoQueue* queue);
bool fifo_is_full(FifoQueue* queue);
void fifo_enqueue(FifoQueue* queue, uint8_t item);
void fifo_enqueue_frame(FifoQueue* queue, const uint8_t* frame, uint16_t frame_length);
uint8_t fifo_dequeue(FifoQueue* queue);
uint16_t fifo_dequeue_batch(FifoQueue* queue, uint8_t* buffer, uint16_t count);
uint16_t fifo_find_frame1(FifoQueue* queue, uint8_t* buffer, uint16_t buffer_size,uint8_t* n);
uint8_t fifo_find_frame2(FifoQueue* queue, uint16_t frame_length, uint8_t* buffer);
 
#endif  // FIFO_H

#endif	//_APP_H_

