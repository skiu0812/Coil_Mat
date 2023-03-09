/*
 * uart_mid.c
 *
 *  Created on: Aug 28, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_UART


extern DMA_HandleTypeDef hdma_usart3_rx;


USBD_CDC_LineCodingTypeDef eLineCoding =
{
    115200,
    0x00,
    0x00,
    0x08
};


static bool is_open[UART_CH_MAX];
static uint8_t sRx_buf[UART_CH_MAX][UART_BUF_SIZE];

qbuffer_t eQbuffer[UART_CH_MAX];
uint8_t eRxData[UART_CH_MAX];


bool uartInit(void)
{
  for (int i=0; i<UART_CH_MAX; i++)
  {
    is_open[i] = false;
  }

  return true;
}

/*
 * @ ch - UART Channel
 *      # UART1 - USB CDC - cli
 *      # UART2 - usart3  - Debug
 *
 * @ baud - Baud Rate
 *
 */
bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  switch(ch)
  {
    case _DEF_UART1:

      ret = qbufferCreate(&eQbuffer[ch], &sRx_buf[ch][0], UART_BUF_SIZE);

      if(ret == true)
      {
        qbufferFlush(&eQbuffer[ch]);
        is_open[ch] = true;
      }
      break;

    case _DEF_UART2:

      ret = uartOpen_RxDMA(ch, baud, &huart3, &hdma_usart3_rx, &eQbuffer[ch], &sRx_buf[ch][0], UART_BUF_SIZE);

      if(ret == true)
      {
        is_open[ch] = true;
      }
      break;
  }

  return ret;
}


/* skkim Code Begin */
uint32_t uartOpen_RxDMA(uint8_t ch, uint32_t baud, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_uart_rx, qbuffer_t *qbuffer, uint8_t *rx_buf, uint32_t buf_size)
{
  uint32_t ret;

  huart->Init.BaudRate = baud;

  HAL_UART_DeInit(huart);

  if(HAL_UART_Init(huart) == HAL_OK)
  {
    if(HAL_UART_Receive_DMA(huart, rx_buf, buf_size) == HAL_OK)
    {
      qbufferCreate(qbuffer, rx_buf, buf_size);

      //Flush
      qbufferFlush(&qbuffer[ch]);
      ret = true;
    }
  }
  else
  {
    ret = false;
  }

  return ret;
}
/* skkim Code End */


uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = qbufferAvailable(&eQbuffer[ch]);
      break;

    case _DEF_UART2:
      eQbuffer[ch].in = (eQbuffer[ch].len - hdma_usart3_rx.Instance->NDTR);
      ret = qbufferAvailable(&eQbuffer[ch]);
      break;
  }

  return ret;
}


uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      qbufferPop(&eQbuffer[ch], &ret, 1);
      break;

    case _DEF_UART2:
      qbufferPop(&eQbuffer[ch], &ret, 1);
      break;
  }

  return ret;
}


uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint16_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;

  switch(ch)
  {
    case _DEF_UART1:
      ret = usbCDCWrite(p_data, length);
      break;

    case _DEF_UART2:
      status =  HAL_UART_Transmit(&huart3, p_data, length, 100);
      if(status == HAL_OK)
      {
        ret = length;
      }
      break;
  }

  return ret;
}


uint32_t uartPrintf(uint8_t ch, char *format, ...)
{
  uint32_t  ret;
  char      buf[1024];
  va_list   arg;
  int       len;

  va_start(arg, format);
  len = vsnprintf(buf, 1024, format, arg);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(arg);

  return ret;
}


uint32_t uartPrintfNotice(uint8_t ch, uart_nprintf_t noti, char* format, ...)
{
    char      buf[1024];
    va_list   arg;
    int       len;
    uint32_t  ret;

    if(noti==error)
    {
      strcpy(buf, "[ERROR] ");
    }
    else if(noti==warning)
    {
      strcpy(buf, "[WARNING] ");
    }

    len = strlen(buf);

    va_start(arg, format);
    len += vsprintf(buf+len, format, arg);

    ret = uartWrite(ch, (uint8_t *)buf, len);

    va_end(arg);

    return ret;
}


uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = eLineCoding.bitrate;
      break;

    case _DEF_UART2:
      ret = huart3.Init.BaudRate;
      break;
  }

  return ret;
}


void uartAdjBaud(void)
{
  uint32_t usbBaud = uartGetBaud(_DEF_UART1);;

  if(usbBaud != uartGetBaud(_DEF_UART2))
  {
    uartOpen(_DEF_UART2, usbBaud);
  }

  if(usbBaud != uartGetBaud(_DEF_UART3))
  {
    uartOpen(_DEF_UART3, usbBaud);
  }

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if 0
  if(huart->Instance == USART3)
  {
    qbufferPush(&eQbuffer[_DEF_UART2], &eRxData[_DEF_UART2], 1);

    HAL_UART_Receive_IT(&huart3, (uint8_t *)&eRxData[_DEF_UART2], 1);
  }
#endif
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART3)
  {

  }
}


#endif //#ifdef _USE_HW_UART
