/**
 ****************************************************************************************************
 * @file        pyinput.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-22
 * @brief       ƴ�����뷨 ����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211122
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __PYINPUT_H
#define __PYINPUT_H


#include "./SYSTEM/sys/sys.h"


/* ƴ�������ƴ���Ķ�Ӧ�� */
typedef struct
{
  char *py_input;    /* ������ַ��� */
  char *py;          /* ��Ӧ��ƴ�� */
  char *pymb;        /* ��� */
}py_index;

#define MAX_MATCH_PYMB      10          /* ���ƥ���� */

/* ƴ�����뷨 */
typedef struct
{
  uint8_t(*getpymb)(uint8_t *instr);    /* �ַ���������ȡ���� */
  py_index *pymb[MAX_MATCH_PYMB];       /* �����λ�� */
}pyinput;

extern pyinput t9;

uint8_t str_match(uint8_t*str1,uint8_t*str2);
uint8_t get_matched_pymb(uint8_t *strin,py_index **matchlist);
uint8_t get_pymb(uint8_t* str);
void test_py(uint8_t *inputstr);


#endif




















