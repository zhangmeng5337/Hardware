
//#include "sys.h"
#include <stdint.h>
#include "app_fifo.h"


void Fifo_Init(FIFO_Type* fifo, uint8_t* buffer, uint16_t size)
{
	fifo->buffer = buffer;
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = size;
}

uint16_t Fifo_Get(FIFO_Type* fifo, uint8_t* buffer, uint16_t len)
{
	uint16_t lenght;
	uint16_t in = fifo->in;	//防止处理过程中，中断修改fifo->in
	uint16_t i;
	lenght = (in + fifo->size - fifo->out)%fifo->size;
	if(lenght > len)
		lenght = len;
	for(i = 0; i < lenght; i++)
	{
		buffer[i] = fifo->buffer[(fifo->out + i)%fifo->size];
	}
	fifo->out = (fifo->out + lenght)%fifo->size;
	return lenght;
}

uint16_t Fifo_Set(FIFO_Type* fifo, uint8_t* buffer, uint16_t len)
{
	uint16_t lenght;
	uint16_t i;
	lenght = (fifo->out + fifo->size - fifo->in)%fifo->size;
	if(lenght > len)
		lenght = len;
	for(i = 0; i < lenght; i++)
	{
		fifo->buffer[(fifo->in + i)%fifo->size] = buffer[i];
	}
	fifo->in = (fifo->in + lenght)%fifo->size;
	return lenght;
}

uint16_t Fifo_Status(FIFO_Type* fifo)
{
	uint16_t lenght;
	lenght = (fifo->in + fifo->size - fifo->out)%fifo->size;
	return lenght;
}

/**
 ******************************************************************************
 * @文件名 	  fifo.c
 * @作者	  Jing
 * @版本	  V01
 * @日期	  2023-08-25
 * @作用	
 ******************************************************************************
 * 修改记录:
 * 日期           修改者          修改内容
 * 2023-08-25	   Jing    		   1.0版
 ******************************************************************************
 * @注意
 *
 * Copyright (C) 2023-2025
 * All rights reserved Jing
 *
 ******************************************************************************/
 

FifoQueue uart1_rec;
FifoQueue uart2_rec;
FifoQueue uart1_tran;
FifoQueue uart2_tran;
volatile uint8_t work=0;
 
/*
*********************************************************************************
* 函数名: fifo_init
* 功  能: 初始化
* 形  参: 无
* 返回值: 无
*********************************************************************************
*/
void fifo_init(void)
{
    fifo_clear(&uart1_rec);
//	  fifo_clear(&uart2_rec);
//	  fifo_clear(&uart1_tran);
//	  fifo_clear(&uart2_tran);
}
 
/*
*********************************************************************************
* 函数名: fifo_clear
* 功  能: 清空队列
* 形  参: queue
* 返回值: 0,1
*********************************************************************************
*/
void fifo_clear(FifoQueue* queue)
{
    queue->start = 0;
    queue->end = 0;
    queue->count = 0;
}
 
/*
*********************************************************************************
* 函数名: fifo_is_empty
* 功  能: 判断队列是否为空
* 形  参: queue
* 返回值: 结果
*********************************************************************************
*/
bool fifo_is_empty(FifoQueue* queue)
{
    return (queue->count == 0);
}
 
/*
*********************************************************************************
* 函数名: fifo_is_full
* 功  能: 判断队列是否为满
* 形  参: queue
* 返回值: 0,1
*********************************************************************************
*/
bool fifo_is_full(FifoQueue* queue)
{
    return (queue->count == QUEUE_SIZE);
}
 
/*
*********************************************************************************
* 函数名: fifo_enqueue
* 功  能: 单字节入队
* 形  参: queue,item
* 返回值: 无
*********************************************************************************
*/
void fifo_enqueue(FifoQueue* queue, uint8_t item)
{
    if (!fifo_is_full(queue))
    {
        queue->data[queue->end] = item;
        queue->end = (queue->end + 1) % QUEUE_SIZE;
        queue->count++;
    }
}
 
/*
*********************************************************************************
* 函数名: fifo_enqueue_frame
* 功  能: 批量入队
* 形  参: queue,frame,frame_length
* 返回值: 无
*********************************************************************************
*/
void fifo_enqueue_frame(FifoQueue* queue, const uint8_t* frame, uint16_t frame_length)
{
    if ( (frame_length == 0) || fifo_is_full(queue))
    {
        return;
    }
 
    for (uint16_t i = 0; i < frame_length; i++)
    {
        fifo_enqueue(queue, frame[i]);
    }
}
 
/*
*********************************************************************************
* 函数名: fifo_dequeue
* 功  能: 单字节出队
* 形  参: queue
* 返回值: item
*********************************************************************************
*/
uint8_t fifo_dequeue(FifoQueue* queue)
{
    if (!fifo_is_empty(queue))
    {
        uint8_t item = queue->data[queue->start];
        queue->start = (queue->start + 1) % QUEUE_SIZE;
        queue->count--;
        return item;
    }
    return 0;  // 如果队列为空，返回一个默认值
}
 
/*
*********************************************************************************
* 函数名: fifo_dequeue_batch
* 功  能: 批量出队
* 形  参: queue,buffer,count
* 返回值: numDequeued
*********************************************************************************
*/
uint16_t fifo_dequeue_batch(FifoQueue* queue, uint8_t* buffer, uint16_t count)
{
    if (count == 0 || fifo_is_empty(queue))
    {
        return 0;
    }
 
    uint16_t numDequeued = 0;
 
    while (numDequeued < count && !fifo_is_empty(queue))
    {
        buffer[numDequeued] = queue->data[queue->start];
        fifo_dequeue(queue); // 出队一个字节
        numDequeued++;
    }
 
    return numDequeued;
}
 
/*
*********************************************************************************
* 函数名: fifo_find_frame1
* 功  能: 查找不定长数据
* 形  参: queue,buffer,buffer_size
* 返回值: frame_length
*********************************************************************************
*/
uint16_t fifo_find_frame1(FifoQueue* queue, uint8_t* buffer, uint16_t buffer_size,uint8_t* n)
{
    if (buffer_size == 0 || fifo_is_empty(queue))
    {
        return 0;
    }
 
    uint16_t count = queue->count;
    uint16_t start = queue->start;
    uint16_t original_count = count;  // 记录原始的队列元素个数
 
    while (count > miniframe_len && count <= original_count)  // 加入 count <= original_count 条件，保证遍历不会超过原始的队列元素个数
    {
        if (queue->data[start] == FRAME_START_BYTE1 && queue->data[(start + 1) % QUEUE_SIZE] == FRAME_START_BYTE2)
        {
					  queue->start = start;
            uint8_t frame_length = queue->data[(start + misalignment_len) % QUEUE_SIZE]+5;//获取帧长(第三位数据长度+5)
						//uint8_t frame_length = 14;//获取帧长(第三位数据长度+5)
						*n = frame_length;
            // 检查是否能够一次性出队完整的数据帧
            if (count >= frame_length)
            {
                // 确保 buffer 大小足够容纳完整的数据帧
                if (frame_length <= buffer_size)
                {
                    fifo_dequeue_batch(queue, buffer, frame_length);
                    return frame_length; // 返回找到的完整数据帧长度
                }
                else
                {
                    break; // buffer 大小不足以容纳完整的数据帧，结束函数执行
                }
            }
            else
            {
                break; // 数据帧不完整，结束函数执行，等待后续数据接收
            }
        }
        
        // 未找到起始字节，继续搜索下一个位置，并更新队列状态
        start = (start + 1) % QUEUE_SIZE;
        count--;
    }
 
    // 更新队列状态
    queue->start = start;
    queue->count = count;
 
    return 0; // 未找到完整帧
}
 
/*
*********************************************************************************
* 函数名: fifo_find_frame2
* 功  能: 查找定长数据
* 形  参: queue,frame_length,buffer
* 返回值: 0,1
*********************************************************************************
*/
uint8_t fifo_find_frame2(FifoQueue* queue, uint16_t frame_length, uint8_t* buffer)
{
    if (fifo_is_empty(queue))
    {
        return 0;
    }
 
    uint16_t count = queue->count;
    uint16_t start = queue->start;
    uint16_t original_count = count;  // 记录原始的队列元素个数
 
    while (count >= frame_length && count <= original_count)  // 加入 count <= original_count 条件，保证遍历不会超过原始的队列元素个数
    {
        // 检查起始字节
        if (queue->data[start] == FRAME_START_BYTE1 && queue->data[(start + 1) % QUEUE_SIZE] == FRAME_START_BYTE2)
        {
            // 判断是否能够一次性出队完整的数据帧
            if (count >= frame_length)
            {
                fifo_dequeue_batch(queue, buffer, frame_length);
                return 1; // 成功找到完整的数据帧
            }
            else
            {
                break; // 数据帧不完整，结束函数执行，等待后续数据接收
            }
        }
 
        // 未找到起始字节，继续搜索下一个位置，并更新队列状态
        start = (start + 1) % QUEUE_SIZE;
        count--;
    }
 
    // 更新队列状态
    queue->start = start;
    queue->count = count;
 
    return 0; // 未找到完整帧
} 

