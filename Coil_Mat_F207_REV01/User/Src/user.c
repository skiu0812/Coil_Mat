/*
 * user.c
 *
 *  Created on: Sep 13, 2022
 *      Author: KSK
 */


#include "user.h"


bool userInit(void)
{
  bool ret = true;

#ifdef _USE_HW_FLASH
  ret &= flashInit();
#endif
#ifdef _USE_SW_EEPROM
  ret &= eepromInit();
#endif
#ifdef _USE_HW_USB
  ret &= usbInit();
#endif
#ifdef _USE_HW_CLI
  ret &= cliInit();
#endif
#ifdef _USE_HW_TIM
  ret &= timInit();
#endif
#ifdef _USE_HW_GPIO
  ret &= gpioInit();
#endif
#ifdef _USE_HW_LED
  ret &= ledInit();
#endif
#ifdef _USE_HW_BUTTON
  ret &= buttonInit();
#endif
#ifdef _USE_HW_UART
  ret &= uartInit();
#endif
#ifdef _USE_HW_SPI
  ret &= spiInit();
#endif
#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif
#ifdef _USE_HW_CAN
  ret &= canInit();
#endif
#ifdef _USE_HW_ADC1
  ret &= adcInit();
#endif
#ifdef _USE_HW_LCD
  ret &= lcdInit();
#endif
#ifdef _USE_HW_COIL
  ret &= coilInit();
#endif

  return ret;
}
