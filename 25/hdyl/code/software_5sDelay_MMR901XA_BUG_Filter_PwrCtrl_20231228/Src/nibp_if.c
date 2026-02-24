#include "nibp_if.h"
#include "stdio.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim4;

static uint8_t buffer_rx[16];
static uint32_t lastcounter;

static struct __kfifo rxfifo;
static uint8_t rxfifobuf[256];

static uint8_t parity(uint8_t in)
{
    return (0xD6 + (in & 0x0F) + (in >> 4));
}

static void nibp_tst_init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}
void bp_pwr_ctrl(unsigned char ctrl)//add by zm 2023.12.26
{
    if (ctrl == ON)
    {
        HAL_GPIO_WritePin(BP_POWER_EN_GPIO_Port, BP_POWER_EN_Pin, GPIO_PIN_SET);
        HAL_Delay(50);
    }
    else
    {
        
        HAL_GPIO_WritePin(BP_POWER_EN_GPIO_Port, BP_POWER_EN_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
    }
}
int8_t get_bp_uart_status()
{
    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX_RX)
    {
        return -1;
    }
    else
        return 0;

}
void nibp_if_init(void)
{
    nibp_tst_init();
    __kfifo_init(&rxfifo, rxfifobuf, sizeof(rxfifobuf), sizeof(uint8_t));
    HAL_UART_Receive_DMA(&huart2, buffer_rx, sizeof(buffer_rx));
    lastcounter = __HAL_DMA_GET_COUNTER(huart2.hdmarx);
}

static void nibp_if_recv_process(void)
{
    uint32_t counter = __HAL_DMA_GET_COUNTER(huart2.hdmarx);
    if ((counter != 0) && (lastcounter != counter))
    {
        if (lastcounter > counter)
        {
            __kfifo_in(&rxfifo, &buffer_rx[sizeof(buffer_rx) - lastcounter], lastcounter - counter);
        }
        else
        {
            __kfifo_in(&rxfifo, &buffer_rx[sizeof(buffer_rx) - lastcounter], lastcounter);
            __kfifo_in(&rxfifo, &buffer_rx[0], sizeof(buffer_rx) - counter);
        }
        lastcounter = counter;
    }
}

int8_t nibp_if_cmd(uint8_t cmd)
{
    static uint8_t buffer[8];
    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX_RX)
    {
        return -1;
    }
    bp_pwr_ctrl(ON);//bp power on //add by zm 2023.1226
    buffer[0] = 0x02;
    sprintf((char *)&buffer[1], "%02X;;%02X", cmd, parity(cmd));
    buffer[7] = 0x03;
    HAL_UART_Transmit_DMA(&huart2, buffer, 8);
    return 0;
}

int8_t nibp_if_stop(void)
{
    static uint8_t cmd = 'x';
    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX_RX)
    {
        return -1;
    }
//    bp_pwr_ctrl(ON);//bp power on
    HAL_UART_Transmit_DMA(&huart2, &cmd, 1);
//    while (get_bp_uart_status() == -1) //add by zm 2023.1226
//        ;
//    bp_pwr_ctrl(OFF);//bp power OFF
    return 0;
}

static uint16_t g_pressure = 0;
static uint16_t g_sdp = 0, g_dbp = 0, g_mbp = 0, g_pr = 0;

uint16_t nibp_if_getpressure(void)
{
    return g_pressure;
}

static void nibp_if_parse_process(void)
{
    uint8_t buffer[128];
    uint16_t len;
    len = __kfifo_len(&rxfifo);
    if (len < 1)
    {
        return;
    }
    __kfifo_out_peek(&rxfifo, buffer, 1);
    if (buffer[0] != 0x02)
    {
        __kfifo_out(&rxfifo, buffer, 1);
        return;
    }
    len = __kfifo_len(&rxfifo);
    if (len >= 6)
    {
        __kfifo_out_peek(&rxfifo, buffer, 6);
        if (buffer[4] == 0x03 && buffer[5] == 0x0D) //测量结束
        {
            __kfifo_out(&rxfifo, buffer, 6);
            nibp_if_cmd(0x18);
//            while (get_bp_uart_status() == -1) //add by zm 2023.1226
//                ;
//            bp_pwr_ctrl(OFF);//bp power OFF

        }
    }
    len = __kfifo_len(&rxfifo);
    if (len >= 10)
    {
        __kfifo_out_peek(&rxfifo, buffer, 10);
        if (buffer[8] == 0x03 && buffer[9] == 0x0D) //袖带压返回
        {
            __kfifo_out(&rxfifo, buffer, 10);
            char buf[4];
            buf[0] = buffer[1];
            buf[1] = buffer[2];
            buf[2] = buffer[3];
            buf[3] = 0;
            g_pressure = atoi(buf);
//            while (get_bp_uart_status() == -1) //add by zm 2023.1226
//                ;
//            bp_pwr_ctrl(OFF);//bp power OFF			
        }
    }
    len = __kfifo_len(&rxfifo);
    if (len >= 42)
    {
        __kfifo_out_peek(&rxfifo, buffer, 42);
        if (buffer[40] == 0x03 && buffer[41] == 0x0D) //系统状态返回
        {
            __kfifo_out(&rxfifo, buffer, 42);
            int sta, mode, mmode, err, sbp, dbp, mbp, pr;
            int count = sscanf((char *)&buffer[1], "S%1d;A%1d;C%02d;M%02d;P%03d%03d%03d;R%03d;", \
                               &sta, &mode, &mmode, &err, &sbp, &dbp, &mbp, &pr);
            //printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", count, sta, mode, mmode, err, sbp, dbp, mbp, pr);
            if (count == 8 && (err == 0 || err == 3))
            {
                g_sdp = sbp;
                g_dbp = dbp;
                g_mbp = mbp;
                g_pr = pr;
            }
        }
    }
}

static void nibp_if_send_process(void)
{
    if (g_pr != 0 && protocol_is_datasend_busy() == 0)
    {
        uint8_t buffer[5];
        buffer[0] = 0x04;
        buffer[1] = g_sdp;
        buffer[2] = g_dbp;
        buffer[3] = g_mbp;
        buffer[4] = g_pr;
        g_pr = 0;
        protocol_data_send(buffer, 5, 2);
    }
}



static int16_t dstpressure = -1;

void nibp_tst_start(uint16_t dst)
{
    dstpressure = dst;
    HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_RESET);
}

void nibp_tst_stop(void)
{
    dstpressure = -1;
    HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_SET);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

static void nibp_tst_process(void)
{
    if (dstpressure == -1)
    {
        return;
    }
    int32_t pwm;
    pwm = (dstpressure - MMR_get_pressure() / 100) * 5000;
    uint16_t pwmmin;
    pwmmin = 0x2000 + dstpressure * 30;
    if (pwm < pwmmin)
    {
        pwm = pwmmin;
    }
    if (pwm > 65535)
    {
        pwm = 65535;
    }
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pwm);
    if (dstpressure - MMR_get_pressure() / 100 < (-dstpressure / 70))
    {
        dstpressure = -1;
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    }
}

void nibp_tst_valve_set(uint8_t stat)
{
    if (stat == 1)
    {
        HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_RESET);
    }
}

void nibp_tst_motor_set(uint16_t data)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, data);
}

void nibp_if_process(void)
{
    nibp_if_recv_process();
    nibp_if_parse_process();
    nibp_if_send_process();
    nibp_tst_process();
}
















