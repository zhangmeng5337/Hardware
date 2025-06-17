/*! 
 *  \file cmd_queue.h
 *  \brief ����ָ�����
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include "main.h"

typedef unsigned char qdata;
typedef unsigned short qsize;

#define HEAD  0x07
#define QUEUE_MAX_SIZE 128

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

extern uint8_t CheckSum(uint8_t *buffer,uint8_t n);


#endif

