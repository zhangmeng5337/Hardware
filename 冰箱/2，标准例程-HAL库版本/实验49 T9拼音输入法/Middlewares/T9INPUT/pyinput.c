/**
 ****************************************************************************************************
 * @file        pyinput.c
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

#include "string.h"
#include "./T9INPUT/pymb.h"
#include "./T9INPUT/pyinput.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"


/* ƴ�����뷨 */
pyinput t9 =
{
    get_pymb,
    0,
};

/**
 * @brief       �Ƚ������ַ�����ƥ�����
 * @param       str1      : �ַ���1
 * @param       str2      : �ַ���2
 * @retval      �ԱȽ��
 *   @arg       0XFF , ��ʾ��ȫƥ��
 *   @arg       ���� , ƥ����ַ���
 */
uint8_t str_match(uint8_t *str1, uint8_t *str2)
{
    uint8_t i = 0;

    while (1)
    {
        if (*str1 != *str2)break;   /* ����ƥ�� */

        if (*str1 == '\0')
        {
            i = 0XFF;   /* ��ȫƥ�� */
            break;
        }

        i++;
        str1++;
        str2++;
    }

    return i;           /* �����ַ������ */
}

/**
 * @brief       ��ȡƥ���ƴ�����
 * @param       strin     : ������ַ���,����:"726"
 * @param       matchlist : �����ƥ���
 * @retval      ƥ��״̬
 *              [7]  , 0,��ʾ��ȫƥ�䣻1����ʾ����ƥ�䣨����û����ȫƥ���ʱ��Ż���֣�
 *              [6:0], ��ȫƥ���ʱ�򣬱�ʾ��ȫƥ���ƴ������
 *                     ����ƥ���ʱ�򣬱�ʾ��Чƥ���λ��
 */
uint8_t get_matched_pymb(uint8_t *strin, py_index **matchlist)
{
    py_index *bestmatch = 0;    /* ���ƥ�� */
    uint16_t pyindex_len = 0;
    uint16_t i = 0;
    uint8_t temp, mcnt = 0, bmcnt = 0;
    
    bestmatch = (py_index *)&py_index3[0];  /* Ĭ��Ϊa��ƥ�� */
    pyindex_len = sizeof(py_index3) / sizeof(py_index3[0]); /* �õ�py������Ĵ�С */

    for (i = 0; i < pyindex_len; i++)
    {
        temp = str_match(strin, (uint8_t *)py_index3[i].py_input);

        if (temp)
        {
            if (temp == 0XFF)
            {
                matchlist[mcnt++] = (py_index *)&py_index3[i];
            }
            else if (temp > bmcnt)  /* �����ƥ�� */
            {
                bmcnt = temp;
                bestmatch = (py_index *)&py_index3[i];  /* ��õ�ƥ�� */
            }
        }
    }

    if (mcnt == 0 && bmcnt)     /* û����ȫƥ��Ľ��,�����в���ƥ��Ľ�� */
    {
        matchlist[0] = bestmatch;
        mcnt = bmcnt | 0X80;    /* ���ز���ƥ�����Чλ�� */
    }

    return mcnt;    /* ����ƥ��ĸ��� */
}

/**
 * @brief       �õ�ƴ�����
 * @param       str       : �����ַ���
 * @retval      ƥ��״̬
 *              [7]  , 0,��ʾ��ȫƥ�䣻1����ʾ����ƥ�䣨����û����ȫƥ���ʱ��Ż���֣�
 *              [6:0], ��ȫƥ���ʱ�򣬱�ʾ��ȫƥ���ƴ������
 *                     ����ƥ���ʱ�򣬱�ʾ��Чƥ���λ��
 */
uint8_t get_pymb(uint8_t *str)
{
    return get_matched_pymb(str, t9.pymb);
}

/**
 * @brief       �õ�ƴ�����(��USMART�õ�)
 * @param       str       : �����ַ���
 * @retval      ��
 */
void test_py(uint8_t *str)
{
    uint8_t t = 0;
    uint8_t i = 0;
    t = t9.getpymb(str);

    if (t & 0X80)
    {
        printf("\r\n��������Ϊ:%s\r\n", str);
        printf("����ƥ��λ��:%d\r\n", t & 0X7F);
        printf("����ƥ����:%s,%s\r\n", t9.pymb[0]->py, t9.pymb[0]->pymb);
    }
    else if (t)
    {
        printf("\r\n��������Ϊ:%s\r\n", str);
        printf("��ȫƥ�����:%d\r\n", t);
        printf("��ȫƥ��Ľ��:\r\n");

        for (i = 0; i < t; i++)
        {
            printf("%s,%s\r\n", t9.pymb[i]->py, t9.pymb[i]->pymb);
        }
    }
    else
    {
        printf("û���κ�ƥ������\r\n");
    }
}
































