/*
 * uart_mid.h
 *
 *  Created on: Aug 28, 2022
 *      Author: KSK
 */

#ifndef INC_UART_MID_H_
#define INC_UART_MID_H_


#include "hw_def.h"
#include "qbuffer.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"


#ifdef   _USE_HW_UART


#define UART_CH_MAX       HW_UART_CH_MAX
#define UART_BUF_SIZE     HW_UART_BUF_SIZE


typedef enum
{
  error,
  warning,
}uart_nprintf_t;


extern USBD_CDC_LineCodingTypeDef eLineCoding;
extern qbuffer_t eQbuffer[UART_CH_MAX];
extern uint8_t eRxData[UART_CH_MAX];


bool      uartInit(void);
bool      uartOpen(uint8_t ch, uint32_t baud);
uint32_t  uartOpen_RxDMA(uint8_t ch, uint32_t baud, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_uart_rx, qbuffer_t *qbuffer, uint8_t *rx_buf, uint32_t buf_size);
uint32_t  uartAvailable(uint8_t ch);
uint8_t   uartRead(uint8_t ch);
uint32_t  uartWrite(uint8_t ch, uint8_t *p_data, uint16_t length);
uint32_t  uartPrintf(uint8_t ch, char *format, ...);
uint32_t  uartPrintfNotice(uint8_t ch, uart_nprintf_t noti, char* format, ...);
uint32_t  uartGetBaud(uint8_t ch);
void      uartAdjBaud(void);


#endif //#ifdef   _USE_HW_UART


#endif /* INC_UART_MID_H_ */
