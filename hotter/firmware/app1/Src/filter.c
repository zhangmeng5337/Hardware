#include "filter.h"
/* 冒泡排序 */
/* 1. 从当前元素起，向后依次比较每一对相邻元素，若逆序则交换 */
/* 2. 对所有元素均重复以上步骤，直至最后一个元素 */
/* elemType arr[]: 排序目标数组; int len: 元素个数 */
void bubbleSort(float arr[], int len)
{
	float temp;
	int i, j;
	for (i = 0; i < len - 1; i++) /* 外循环为排序趟数，len个数进行len-1趟 */
	{
		for (j = 0; j < len - 1 - i; j++)
		{ /* 内循环为每趟比较的次数，第i趟比较len-i次 */
			if (arr[j] > arr[j + 1])
			{ /* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

}
float buf[3];
float *low_temperature_cal(float arr[], int len)
{
	float temp,sum;
	
	int i, j;
	temp = 0;
	sum = 0;
	for (i = 0; i < len - 1; i++) /* 外循环为排序趟数，len个数进行len-1趟 */
	{

			if (arr[i] < LOW_TEMPERATURE)
			{ /* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
				temp = temp + 1;
			}
			sum = sum + arr[i];
	}
	buf[0] = sum/len;
	buf[1] = temp/len;
	buf[2] = arr[len-1] - arr[0];
    return buf;
}


