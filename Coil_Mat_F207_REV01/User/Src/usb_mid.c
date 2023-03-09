/*
 * usb_mid.c
 *
 *  Created on: Sep 4, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_USB


bool usbInit(void)
{
  bool ret = true;

  return ret;
}


uint32_t usbCDCWrite(uint8_t *p_data, uint16_t length) // skkim 220904
{
  uint32_t pre_time;
  uint8_t ret;

  pre_time = millis();

  while(1)
  {
    ret = CDC_Transmit_FS(p_data, length);

    if(ret == USBD_OK)
    {
      return length;
    }
    else if(ret == USBD_FAIL)
    {
      return 0;
    }

    // timeout
    if(millis()-pre_time >= 100)
    {
      break;
    }
  }

  return 0;
}


bool eFlagCDCBufFull = false;
uint8_t usbCDCSOF(struct _USBD_HandleTypeDef *pdev) // skkim 220906
{
  uint32_t buf_len;

  if(eFlagCDCBufFull == true)
  {
    // 수신 버퍼에서 비어있는 데이터량
    buf_len = (UART_BUF_SIZE - uartAvailable(_DEF_UART1)) - 1;

    if(buf_len >= USB_FS_MAX_PACKET_SIZE)
    {
      // 다음 데이터 보내줘
      USBD_CDC_ReceivePacket(pdev);
      eFlagCDCBufFull = false;
    }
  }

  return 0;
}


#endif //#ifdef _USE_HW_USB
