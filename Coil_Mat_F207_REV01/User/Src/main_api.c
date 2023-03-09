/*
 * main_api.c
 *
 *  Created on: Sep 1, 2022
 *      Author: KSK
 */


#include "user.h"


void apInit(void)
{
#ifdef _USE_HW_CLI
  cliOpen(UART_CLI, 115200); // USB CDC
#endif
//  adcStartDma();
}


void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();

  while(1)
  {
    if(millis()-pre_time >= 500)
    {
//      ledCtrl(_DEF_LED1, LED_TOGGLE);

      pre_time = millis();
    }

    Task_ADC(50);
    Task_USB(1);
    Task_UART(100);
    Task_Button(10);
    Task_LED(10);
    Task_LCD(10);

#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
    Task_Heat(130);
#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
    Task_Solenoid(50);
#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

#ifdef _USE_HW_CLI
    cliMain(10);
#endif

  }
}
