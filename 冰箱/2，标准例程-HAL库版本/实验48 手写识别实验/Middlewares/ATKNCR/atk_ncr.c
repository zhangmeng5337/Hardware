/**
 ****************************************************************************************************
 * @file        atk_ncr.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-22
 * @brief       ������ĸʶ�� ����
 *   @note      ������ĸʶ�����������ԭ��(ALIENTEK)�ṩ, �����ṩ2��LIB, �����ʹ��
 *              ATKNCR_M_Vx.x.lib��ATKNCR_N_Vx.x.lib��Ψһ�������Ƿ�ʹ�ö�̬�ڴ����
 *              ����:
 *              M,������Ҫ�õ�malloc�İ汾,����ʵ��alientek_ncr_malloc��alientek_ncr_free��������
 *              N,������ͨ�汾,����Ҫʵ��alientek_ncr_malloc��alientek_ncr_free��������
 *              Vx.x,����ǰʶ�����İ汾
 *
 *              ����: ֧������/Сд��ĸ/��д��ĸ/�������ʶ��ģʽ
 *              ��ʶ�����ʹ�������൱��, ��������:
 *              ��һ��:����alientek_ncr_init����,��ʼ��ʶ�����
 *              �ڶ���:��ȡ����ĵ�������(������2�������ϵĲ�ͬ������������)
 *              ������:����alientek_ncr����,�õ�ʶ����
 *              ���Ĳ�:�������Ҫ��ʶ��,�����alientek_ncr_stop����,��ֹʶ��.�������Ҫ����,���ظ�2,3������
 *
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
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

#include "./MALLOC/malloc.h"
#include "./ATKNCR/atk_ncr.h"


/**
 * @brief       �ڴ����ú���
 * @param       *p    : �ڴ��׵�ַ
 * @param       c     : Ҫ���õ�ֵ
 * @param       len   : ��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
 * @retval      ��
 */
void alientek_ncr_memset(char *p, char c, unsigned long len)
{
    my_mem_set((uint8_t*)p, (uint8_t)c, (uint32_t)len);
}

/**
 * @brief       �����ڴ�
 * @param       size : Ҫ������ڴ��С(�ֽ�)
 * @retval      ���䵽���ڴ��׵�ַ.
 */
void *alientek_ncr_malloc(unsigned int size) 
{
    return mymalloc(SRAMIN, size);
}

/**
 * @brief       �ͷ��ڴ�
 * @param       ptr  : �ڴ��׵�ַ
 * @retval      ��
 */
void alientek_ncr_free(void *ptr) 
{
    myfree(SRAMIN, ptr);
}




















