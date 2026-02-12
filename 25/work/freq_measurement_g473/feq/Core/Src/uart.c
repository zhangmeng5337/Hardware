#include "uart.h"
#include "adc.h"
#include "stdio.h"
#include <string.h>
#include "my_string.h"
#include "stdarg.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
uart_stru uart_usr;


// printf函数端口重映射，方便串口输出
struct __FILE
{
    int handle;//
};
FILE __stdout;



int fputc(int ch, FILE *f)
{
    unsigned char tmp;
    tmp = (unsigned char)ch;
    //HAL_UART_Transmit(&huart1, &tmp, 1, 1);
    HAL_UART_Transmit_DMA(&huart1, &tmp, 3);

    //  HAL_UART_Transmit_DMA(&huart1, &tmp, 1);


    return ch;
}
void led_ctrl(uint32_t delay_ms)
{
    static uint32_t tick_ms = 0;

    if (delay_ms < 0xffffffff)
    {
        if (tick_ms >= 200000)
        {
            tick_ms = 0;
            HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
            // tick_ms = HAL_GetTick();
        }
        else
            tick_ms++;

    }
    else
        HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
}
void led_ctrl_irq()
{
    led_ctrl(uart_usr.led_ms);
}
//串口初始化，空闲dma中断
void uart_init()
{

    uart_usr.led_ms = 0xffffffff;
    //uart_usr.status = 0;
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_IDLEF);
    HAL_UART_Receive_DMA(&huart1, (uint8_t *) uart_usr.recv_buf, UART_BUFFER_SIZE);
    //  printf("********************************\n");
    // printf("    Sample frequence:  Hz%f    \n", get_adc_data()->adc_smple_freq);
    //  printf("********************************\n");
}
void uart_rec()
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != 0)
    {
        __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_IDLEF);
        //计算接收到的数据长度3333
        uart_usr.recv_len  = UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        uart_usr.recv_update = 1;
    }
    //停止本次DMA传输pppp
    HAL_UART_DMAStop(&huart1);


    //重启开始DMA传输 每次BUFFER_SIZE字节数据
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart_usr.recv_buf, UART_BUFFER_SIZE);

}
/*********************************************************
函数名：uart_Check_Cmd
�? 能：发送命令后,检测接收到的应�?
�? 参：str--期待的应答结�?
返回值：0--没有得到期待的应答结�?*?*--期待应答结果的位�?str的位�?
�? 注：
**********************************************************/

uint8_t *uart_Check_Cmd(uint8_t *str)
{
    char *strx ;
    strx = strstr((const char *)(uart_usr.recv_buf),
                  (const char *)str); //寻找文本(被寻找，欲寻�?
    return (uint8_t *)strx;

}
extern uint32_t tick;
extern uint32_t timer_cnt;
unsigned int tmp2, tmp3, flag;
unsigned char dest[32];
unsigned char result;
uint32_t tx_count, tx_count_real, head_cnt = 0, tag_cnt = 0;
uint32_t  tx_count_1, rx_count_1, tmp;
//串口收发处理
void uart_tx()
{
    static HAL_StatusTypeDef status = HAL_OK;
    if (get_adc_data()->tx_state == 1)
    {
        if (get_adc_data()->tx_flag == 1)

            //if (get_adc_data()->real_index != get_adc_data()->index_tx)
        {

            // if (get_adc_data()->real_index >= get_adc_data()->index_tx)
            {
                //get_adc_data()->len =  get_adc_data()->real_index - get_adc_data()->index_tx;
                get_adc_data()->tx_state = 0;
                get_adc_data()->len_total = get_adc_data()->len_total + ADC_BUF_SIZE;
                //  adc_ctrl(0);
                //DFT_Calculation(get_adc_data()->adc_voltage,ADC_BUF_SIZE,1,ADC_FREQ);    // 1000个采样点数，需要保存第10个频点
                status = HAL_UART_Transmit_DMA(&huart1,
                                               get_adc_data()->adc_voltage, ADC_BUF_SIZE);
                //adc_ctrl(1);
                get_adc_data()->index_tx =  ADC_BUF_SIZE;
                tx_count_1++;
            }

//            else
//            {
//                tmp = ADC_BUF_SIZE - get_adc_data()->index_tx;
//                get_adc_data()->len = get_adc_data()->real_index;
//                get_adc_data()->index_tx =  0;
//                get_adc_data()->tx_state = 0;
//              get_adc_data()->len_total = get_adc_data()->len_total + tmp;
//                status = HAL_UART_Transmit_DMA(&huart1,
//                                               &get_adc_data()->adc_voltage[get_adc_data()->index_tx], tmp);
//
//            }
            get_adc_data()->sum_count = get_adc_data()->len_total;
            //            get_adc_data()->tick = get_adc_data()->tick + PERIOD;
            if (get_adc_data()->tick > 0)
                get_adc_data()->freq = get_adc_data()->sum_count / get_adc_data()->tick;
        }


        if (get_adc_data()->tick >= 60)
        {
            {
                //adc_usr.index_tx = 0;
                get_adc_data()->tick = 0;

                get_adc_data()->sum_count = 0;
            }
        }
    }
}
void uart_proc()
{
    unsigned  char pb[32];
    float tmp;
    //**********************tx mode*************************************

    static unsigned int i = 0;
    //***********************recv mode**********************************
    if (uart_usr.recv_update == 1)
    {
        uart_usr.recv_update = 0;
        if (uart_Check_Cmd("start\r\n"))
        {
            get_adc_data()->update = 0;
            uart_usr.recv_update = 0;
            //uart_usr.status = 1;
            get_adc_data()->tick = 0;
            //get_adc_data()->tick_start = 1;
            get_adc_data()->sum_count = 0;
            //  get_adc_data()->index = 0;
            get_adc_data()->index_tx = 0;
            get_adc_data()->real_index = 0;
            tmp = -0.225 * get_adc_data()->adc_smple_freq + 1000;
            if (tmp > 1000)
                tmp = 1000;
            else if (tmp < 100)
                tmp = 100;
            uart_usr.led_ms = tmp; //4000--->100     1--->1000     900/3999+999
            adc_ctrl(1);//启动adc
            // printf("sample start\r\n");
        }
        else if (uart_Check_Cmd("sample rate:"))//设置频率
        {

            Find_string((char *)uart_usr.recv_buf, "sample rate:", "hz", (char *)pb);
            get_adc_data()->adc_smple_freq = atof((const char *)pb);
            if (get_adc_data()->adc_smple_freq < 4)
                get_adc_data()->adc_smple_freq = 4;
            else if (get_adc_data()->adc_smple_freq > 4000)
                get_adc_data()->adc_smple_freq = 4000;

            tmp = -0.225 * get_adc_data()->adc_smple_freq + 1000;
            if (tmp > 1000)
                tmp = 1000;
            else if (tmp < 100)
                tmp = 100;

            uart_usr.led_ms = (unsigned int)tmp;
            //printf("sample rate:%fhz\r\n", get_adc_data()->adc_smple_freq);
        }
        else if (uart_Check_Cmd("stop\r\n"))
        {
            //get_adc_data()->tick_start = 0;

            memset(get_adc_data()->adc_voltage, 0, ADC_BUF_SIZE);
            //uart_usr.status = 0;
            uart_usr.led_ms = 0xffffffff;
            // get_adc_data()->sample_buf_full = 0;
            // get_adc_data()->adc_buf_full = 0;
            // printf("sample stop\r\n");
            //get_adc_data()->tick_start = 0;
            adc_ctrl(0);//停止adc

        }

        memset(uart_usr.recv_buf, 0, UART_BUFFER_SIZE);
        uart_usr.recv_len = 0;
        uart_usr.recv_update = 0;

    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        //uwTxState = 1; // 设置标志位表示传输完成
        get_adc_data()->tx_state = 1;
    }

}

void app()
{
    if (uart_usr.recv_update == 1)
    {

        uart_proc();

    }
    else
    {
        adc_copy();
        uart_tx();
        led_ctrl_irq();

    }

}
