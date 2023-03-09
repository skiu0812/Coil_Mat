/*
 * adc_api.c
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_ADC1


void Task_ADC(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_ADC, taskPeriod) == true)
  {
    //********************* ADC Task Code Begin *********************//
//    adc1DataGet();
//
//    uartPrintf(UART_DEBUG, "ADC1IN1:%d ", eADC_Data.Vin);
//    uartPrintf(UART_DEBUG, "ADC1IN3:%d ", eADC_Data.Vout);
//    uartPrintf(UART_DEBUG, "ADC1IN4:%d ", eADC_Data.Iin);
//    uartPrintf(UART_DEBUG, "ADC1IN4RAW:%d ", eADC1_RawData[2]);
//
//    uartPrintf(UART_DEBUG, "\n");
    //********************* ADC Task Code End *********************//
  }
}


#endif //#ifdef _USE_HW_ADC1
