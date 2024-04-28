/*! 
 *  \file cmd_queue.h
 *  \brief ����ָ�����
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include "hmi_driver.h"
#include "main.h"
typedef unsigned char qdata;
typedef unsigned short qsize;

#define INDEX_CMD 0x02
#define HEAD  0xaa
typedef struct _QUEUE
{
	qsize _head;
	qsize _tail;
	qdata _data[QUEUE_MAX_SIZE];	
}QUEUE;

/*! 
 *  \brief  ���ָ������
 */
extern void queue_reset(void);

/*! 
 * \brief  ���ָ������
 * \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ����� 
 *  \param  _data ָ������
 */
extern void queue_push(qdata _data);

/*! 
 *  \brief  ��ָ�������ȡ��һ��������ָ��
 *  \param  cmd ָ����ջ�����
 *  \param  buf_len ָ����ջ�������С
 *  \return  ָ��ȣ�0��ʾ������������ָ��
 */
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len);
uint8_t *getQueue(void);
void queue_clear(void);
uint8 CheckSum(uint8 *buffer,uint8 n);
#endif
