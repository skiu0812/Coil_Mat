/*
 * test_mid.c
 *
 *  Created on: 2022. 9. 11.
 *      Author: KSK
 */


#include "user.h"


uint8_t rx_buf[128];
uint32_t rx_len;
void testUART1nUART2SendFile(void)
{
  uartAdjBaud();

  // USB -> USART3
  rx_len = uartAvailable(_DEF_UART1);
  if(rx_len > 128)
  {
    rx_len = 128;
  }
  if(rx_len > 0)
  {
    for(int i=0; i<rx_len; i++)
    {
      rx_buf[i] = uartRead(_DEF_UART1);
    }
    uartWrite(_DEF_UART2, rx_buf, rx_len);
  }

  // USART3 -> USB
  rx_len = uartAvailable(_DEF_UART2);
  if(rx_len > 128)
  {
    rx_len = 128;
  }
  if(rx_len > 0)
  {
    for(int i=0; i<rx_len; i++)
    {
      rx_buf[i] = uartRead(_DEF_UART2);
    }
    uartWrite(_DEF_UART1, rx_buf, rx_len);
  }
}



void testUART2nUART3SendFile(void)
{
  uint8_t rx_buf2[128], rx_buf3[128];
  uint32_t rx_len2, rx_len3;

  rx_len2 = uartAvailable(_DEF_UART2);
  if(rx_len2 > 128)
  {
    rx_len2 = 128;
  }
  // terminal -> usart3
  if(rx_len2 > 0)
  {
    for(int i=0; i<rx_len2; i++)
    {
      rx_buf2[i] = uartRead(_DEF_UART2);
    }
//    uartWrite(_DEF_UART2, rx_buf2, rx_len2);
    uartWrite(_DEF_UART3, rx_buf2, rx_len2);
  }

  rx_len3 = uartAvailable(_DEF_UART3);
  if(rx_len3 > 128)
  {
    rx_len3 = 128;
  }
  // terminal -> usart3
  if(rx_len3 > 0)
  {
    for(int i=0; i<rx_len3; i++)
    {
      rx_buf3[i] = uartRead(_DEF_UART3);
    }
    uartWrite(_DEF_UART2, rx_buf3, rx_len3);
  }
}


void testUART1nUARTxSendFile(uint8_t ch)
{
  uint8_t rx_buf[128], rx_buf_d[128];
  uint32_t rx_len, rx_len_d;

  rx_len = uartAvailable(_DEF_UART1);

  if(rx_len > 128) rx_len = 128;

  // USB CDC -> usart device
  if(rx_len > 0)
  {
    for(int i=0; i<rx_len; i++)
    {
      rx_buf[i] = uartRead(_DEF_UART1);
    }

    uartWrite(ch, rx_buf, rx_len);
  }

  rx_len_d = uartAvailable(ch);

  if(rx_len_d > 128) rx_len_d = 128;

  // usart device -> USB CDC
  if(rx_len_d > 0)
  {
    for(int i=0; i<rx_len_d; i++)
    {
      rx_buf_d[i] = uartRead(ch);
    }
    uartWrite(_DEF_UART1, rx_buf_d, rx_len_d);
  }
}


void testFlashReadWriteErase(void)
{
  if(uartAvailable(_DEF_UART1) > 0)
  {
    uint8_t rx_data;

    rx_data = uartRead(_DEF_UART1);

    if(rx_data == '1')
    {
      uint8_t buf[32];

      logPrintf("Read...\n");

      flashRead(0x8060000, buf, 32);

      for(int i=0; i<32; i++)
      {
        logPrintf("0x%X : 0x%X\n", 0x8060000+i, buf[i]);
      }
    }

    if(rx_data == '2')
    {
      logPrintf("Erase...\n");

      if(flashErase(0x8060000, 32) == true)
      {
        logPrintf("Erase OK\n");
      }
      else
      {
        logPrintf("Erase Fail\n");
      }
    }

    if(rx_data == '3')
    {
      uint8_t buf[32];

      for(int i=0; i<32; i++)
      {
        buf[i] = i;
      }

      logPrintf("Write...\n");

      if(flashWrite(0x8060000, buf, 32) == true)
      {
        logPrintf("Write OK\n");
      }
      else
      {
        logPrintf("Write Fail\n");
      }
    }
  }
}
