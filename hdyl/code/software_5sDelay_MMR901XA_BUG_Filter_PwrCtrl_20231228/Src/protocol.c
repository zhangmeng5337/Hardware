#include "protocol.h"
#include "ecg.h"
#include "stdio.h"
#include "string.h"
#include "crc16.h"
#include "nibp_if.h"

extern UART_HandleTypeDef huart1;
extern struct __kfifo ecgfifo;

static uint8_t buffer_rx[16];
static uint8_t buffer_tx[2048];
static uint32_t lastcounter;

static struct __kfifo rxfifo;
static uint8_t rxfifobuf[2048];

void protocol_init(void)
{
  __kfifo_init(&rxfifo, rxfifobuf, sizeof(rxfifobuf), sizeof(uint8_t));
  HAL_UART_Receive_DMA(&huart1, buffer_rx, sizeof(buffer_rx));
  lastcounter = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
}

static uint16_t protocol_packet(uint8_t head, uint8_t sn, uint8_t *inpdata, uint16_t len, uint8_t *pdataout)
{
  if(pdataout == 0)
  {
    return 0;
  }
  pdataout[0] = head;
  pdataout[1] = sn;
  pdataout[2] = len >> 8;
  pdataout[3] = len & 0xFF;
  memmove(&pdataout[4], inpdata, len);
  uint16_t crc = CRC16(pdataout, 4 + len);
  pdataout[4 + len] = crc >> 8;
  pdataout[5 + len] = crc & 0xFF;
  return (len + 6);
}

static void protocol_recv_process(void)
{
  uint32_t counter = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
  if((counter != 0) && (lastcounter != counter))
  {
    if(lastcounter > counter)
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

static uint8_t packet_tx_buffer[2048];
static uint8_t packet_tx_sn = 0;
static uint16_t packet_tx_len = 0;
static uint8_t packet_retry = 0;
__IO static uint32_t packet_ticktmp = 0;

static uint8_t packet_ack_buf[128];
static uint16_t packet_ack_len = 0;

static void protocol_ack_send(uint8_t sn, uint8_t *pdata, uint16_t len)
{
  packet_ack_len = protocol_packet(0x5A, sn, pdata, len, packet_ack_buf);
}

static void protocol_parse_process(void)
{
  uint8_t acksn = 0;
  static uint8_t buffer[1024];
  uint16_t len = __kfifo_len(&rxfifo);
  if(len < 6)
  {
    return;
  }
  __kfifo_out_peek(&rxfifo, buffer, 4);
  if(buffer[0] != 0x5A && buffer[0] != 0x55)
  {
    __kfifo_out(&rxfifo, buffer, 1);
    return;
  }
  uint16_t packetlen = ((buffer[2] << 8) | buffer[3]) + 6;
  if(len < packetlen)
  {
    return;
  }
  __kfifo_out_peek(&rxfifo, buffer, packetlen);
  uint16_t localcrc = CRC16(buffer, packetlen - 2);
  uint16_t crc = (buffer[packetlen-2] << 8) | buffer[packetlen-1];
  if(localcrc != crc)
  {
    return;
  }
  if(buffer[0] == 0x55)
  {
    acksn = buffer[1];
    switch(buffer[4])
    {
    case 0x02://start nibp
      protocol_ack_send(acksn, 0, 0);
	  while (get_bp_uart_status() == -1) //add by zm 2023.1226
      ;
      bp_pwr_ctrl(OFF);//bp power OFF
      nibp_if_cmd(0x01);
      break;
    case 0x03:
      protocol_ack_send(acksn, 0, 0);
      nibp_if_stop();//stop nibp
      break;
    case 0x05:
      {
        uint8_t buffer[2];
        buffer[0] = 0x05;
        buffer[1] = MMR_get_pressure() / 100;
        protocol_ack_send(acksn, buffer, 2);
      }
      break;
    case 0x06:
      {
        uint8_t buffer[2];
        buffer[0] = 0x06;
        buffer[1] = nibp_if_getpressure();
        protocol_ack_send(acksn, buffer, 2);
      }
      break;
    case 0x07:
      protocol_ack_send(acksn, 0, 0);
      ecg_enable(1);
      break;
    case 0x08:
      protocol_ack_send(acksn, 0, 0);
      ecg_enable(0);
      break;
    case 0x09:
      protocol_ack_send(acksn, 0, 0);
      nibp_tst_start(buffer[5]);
      break;
    case 0x0A:
      protocol_ack_send(acksn, 0, 0);
      nibp_tst_stop();
      break;
    case 0x0B:
      protocol_ack_send(acksn, 0, 0);
      nibp_tst_valve_set(buffer[5]);
      break;
    case 0x0C:
      protocol_ack_send(acksn, 0, 0);
      nibp_tst_motor_set(buffer[5] << 8);
      break;
    }
  }
  else//ack
  {
    //printf("ack:%d\n", buffer[1]);
    if(buffer[1] == packet_tx_sn)
    {
      packet_retry = 0;
      packet_ticktmp = 0;
    }
  }
  __kfifo_out(&rxfifo, buffer, packetlen);
  memmove(buffer, &buffer[4], packetlen - 6);
  
}

#define WAVE_PACKET_SIZE  (200)

static uint8_t protocol_packet_num(void)
{
  static u8 sn;
  return (sn++);
}

uint8_t protocol_data_send(uint8_t *pdata, uint16_t len, uint8_t retry)
{
  if(packet_retry > 0)
  {
    return -1;
  }
  uint8_t sn = protocol_packet_num();
  len = protocol_packet(0x55, sn, pdata, len, packet_tx_buffer);
  packet_tx_sn = sn;
  packet_tx_len = len;
  packet_retry = retry;
  return 0;
}

uint8_t protocol_is_datasend_busy(void)
{
  return (packet_retry > 0 ? 1 : 0);
}

#define PROTOCOL_RETRY_PEROID   (80)

__IO static uint8_t uart_flag = 0;



static void protocol_send_process(void)
{
  if(packet_ack_len != 0 && HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX_RX)
  {
    HAL_UART_Transmit_DMA(&huart1, packet_ack_buf, packet_ack_len);
    packet_ack_len = 0;
    return;
  }
  uint32_t current = HAL_GetTick();
  if(current > (packet_ticktmp + PROTOCOL_RETRY_PEROID))
  {
    if(packet_retry > 0)
    {
      if(HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX_RX)
      {
        HAL_UART_Transmit_DMA(&huart1, packet_tx_buffer, packet_tx_len);
        uart_flag = 1;
        //printf("%d\n", packet_tx_buffer[1]);
        packet_ticktmp = current;
        packet_retry--;
        if(packet_retry == 0)
        {
          packet_ticktmp = 0;
        }
      }
    }
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart1 && uart_flag == 1)
  {
    uart_flag = 0;
    packet_ticktmp = HAL_GetTick();
  }
}

static void protocol_wave_send_process(void)
{
  if(protocol_is_datasend_busy())
  {
    return;
  }
  uint32_t len;
  static uint8_t buffer[WAVE_PACKET_SIZE * 7 + 2];
  MISCDATA miscdata;
  uint8_t *pdata = &buffer[0];
	uint8_t i = 0;
  *pdata++ = 0x01;
  *pdata++ = WAVE_PACKET_SIZE;
  len = __kfifo_len(&ecgfifo);
  if(len >= WAVE_PACKET_SIZE)
  {
    for( i=0; i<WAVE_PACKET_SIZE; i++)
    {
      __kfifo_out(&ecgfifo, &miscdata, 1);
      *pdata++ = miscdata.data[0];
      *pdata++ = miscdata.data[1];
      *pdata++ = miscdata.data[2];
      *pdata++ = miscdata.data[3];
      *pdata++ = miscdata.data[4];
      *pdata++ = miscdata.data[5];
      *pdata++ = miscdata.data[6];
    }
    protocol_data_send(buffer, pdata - buffer, 2);
  }
}

void protocol_process(void)
{
  protocol_recv_process();
  protocol_parse_process();
  protocol_send_process();
  protocol_wave_send_process();
}















