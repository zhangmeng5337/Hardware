#include "kalman.h"
#include "stdio.h"
#include "string.h"

static  float   uiChannel1Buffer[FILTER_CAPTURE];

// 传感器数据窗口（静态数组，避免动态内存分配）
static int32_t data_window[WINDOW_SIZE] = {0};
static uint8_t data_index = 0;  // 当前数据插入位置

// 快速排序分区函数（仅划分到中值位置）
// 返回值：分区后的中值位置
int partition(int32_t *arr, int low, int high)
{
    int32_t pivot = arr[high];  // 选择最后一个元素为基准
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            // 交换元素
            int32_t temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // 将基准值放到正确位置
    int32_t temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

// 快速选择中值（仅排序到中间位置）
int32_t quick_select_median(int32_t *arr, int size)
{
    int low = 0, high = size - 1;
    int target = size / 2;  // 中值位置

    while (low <= high)
    {
        int pivot_idx = partition(arr, low, high);
        if (pivot_idx == target)
        {
            return arr[pivot_idx];  // 找到中值
        }
        else if (pivot_idx < target)
        {
            low = pivot_idx + 1;    // 向右继续查找
        }
        else
        {
            high = pivot_idx - 1;   // 向左继续查找
        }
    }
    return arr[target];  // 返回中值
}

// 插入新数据并计算中值
int32_t update_median_filter(int32_t new_data)
{
    // 1. 更新数据窗口（循环覆盖旧数据）
    data_window[data_index] = new_data;
    data_index = (data_index + 1) % WINDOW_SIZE;

    // 2. 拷贝窗口数据到临时数组（避免修改原始数据）
    int32_t temp_window[WINDOW_SIZE];
    for (int i = 0; i < WINDOW_SIZE; i++)
    {
        temp_window[i] = data_window[i];
    }

    // 3. 快速选择中值（仅排序到中间位置）
    return quick_select_median(temp_window, WINDOW_SIZE);
}





float  GetMedianNum(float bArray)
{

    static float bTemp;
    bTemp = update_median_filter(bArray);//pb[(ADC_CAPTURE + 1) / 2];
    return bTemp;
}

float medium_aver(float dat)
{
    float sum, change;
    unsigned char i, j;
    static unsigned char index = 0, index_tmp = 0;
    sum = 0;
    uiChannel1Buffer[index_tmp] = dat;

    if (index < FILTER_CAPTURE)
        index ++ ;

    if (index_tmp < FILTER_CAPTURE)
        index_tmp ++ ;
    else
        index_tmp = 0;

    if (index >= FILTER_CAPTURE)
    {
        for (i = 1; i < FILTER_CAPTURE; i++)
            for (j = 0; j < FILTER_CAPTURE - i; j++)
            {
                if (uiChannel1Buffer[j] > uiChannel1Buffer[j + 1])
                {
                    change = uiChannel1Buffer[j];
                    uiChannel1Buffer[j] = uiChannel1Buffer[j + 1];
                    uiChannel1Buffer[j + 1] = change;
                }
            }
        for (i = 5; i < FILTER_CAPTURE - 5; i++)// 5
            sum = sum + uiChannel1Buffer[i];
        return sum / (FILTER_CAPTURE - 10);
    }
    else
    {
        for (i = 0; i < index; i++)
            sum = sum + uiChannel1Buffer[i];
        return sum / (index);

    }

}

float SilderFilter(float _value)
{
    char i;
    static unsigned char count = 0;
    float ulChannel1AdcValue;
    ulChannel1AdcValue = 0;
    if (count < FILTER_CAPTURE)
        count ++;
    for (char i = 0; i < (FILTER_CAPTURE - 1); i++)
    {

        uiChannel1Buffer[i] = uiChannel1Buffer[i + 1];
    }

    uiChannel1Buffer[FILTER_CAPTURE - 1] = _value;

    for (char i = 0; i < FILTER_CAPTURE; i++)
    {
        ulChannel1AdcValue = uiChannel1Buffer[i] + ulChannel1AdcValue;
    }

    if (count >= FILTER_CAPTURE)
    {
        ulChannel1AdcValue = ulChannel1AdcValue / FILTER_CAPTURE;

    }
    else
    {
        ulChannel1AdcValue = ulChannel1AdcValue / count;
    }
    return ulChannel1AdcValue;

}




//kalman *kalman_kfp_get(void)
//{
//   return g_kfp_st;
//}

/**
 * @name: kalman_init
 * @msg: 卡尔曼滤波初始化
 * @param {kalman} *kfp 卡尔曼结构体参数
 * @return {*} �? * @author: TOTHTOT
 * @date:
 */
void kalman_init(kalman *kfp)
{
    kfp->Last_P = 30; // 10
    kfp->Now_P = 0;
    kfp->out = 0;
    kfp->Kg = 0;
    kfp->Q = 0.01;
    kfp->R = 0.5;
}

/**
 * @name: kalman_filter
 * @msg: 卡尔曼滤波器
 * @param {kalman} *kfp 卡尔曼结构体参数
 * @param {float} input 需要滤波的参数的测量值（即传感器的采集值）
 * @return {*} 滤波后的参数（最优值）
 * @author: TOTHTOT
 * @date:
 */
float kalman_filter(kalman *kfp, float input)
{
    // 预测协方差方程：k时刻系统估算协方�?= k-1时刻的系统协方差 + 过程噪声协方�?
    kfp->Now_P = kfp->Last_P + kfp->Q;
    // 卡尔曼增益方程：卡尔曼增�?= k时刻系统估算协方�?/ （k时刻系统估算协方�?+ 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // 更新最优值方程：k时刻状态变量的最优�?= 状态变量的预测�?+ 卡尔曼增�?* （测量�?- 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -
                                     kfp->out); // 因为这一次的预测值就是上一次的输出�?    // 更新协方差方�? 本次的系统协方差付给 kfp->LastP 威下一次运算准备�?
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}




