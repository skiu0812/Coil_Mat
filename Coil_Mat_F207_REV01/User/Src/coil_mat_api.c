/*
 * coil_mat_api.c
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_COIL

#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#define PWM_DUTY      HW_COIL_MAT_PWM_DUTY
#define CURR_MIN      HW_COIL_MAT_CURR_MIN
#define CURR_GAP      HW_COIL_MAT_CURR_GAP


void Task_Heat(uint32_t taskPeriod)
{
  static uint8_t  taskState = 0;
  static bool     matState = false;
//  static uint8_t  cnt = 0;

  if(timTask_Run(&eTmr_Task_Coil, taskPeriod) == true)
  {

    //********************* Heating Task Code Begin *********************//
    adc1DataGet();

    uartPrintf(UART_DEBUG, "ADC1IN1:%d ", eADC_Data.Vin);
    uartPrintf(UART_DEBUG, "ADC1IN3:%d ", eADC_Data.Vout);
    uartPrintf(UART_DEBUG, "ADC1IN4:%d ", eADC_Data.Iin);
    uartPrintf(UART_DEBUG, "Button:%d ", eButton[BUTTON_COIL].cnt);
    uartPrintf(UART_DEBUG, "HeatCurrSet:%d ", coilCurr(eButton[BUTTON_COIL].cnt, CURR_MIN, CURR_GAP));
    uartPrintf(UART_DEBUG, "ADC1IN4RAW:%d ", eADC1_RawData[2]);
    uartPrintf(UART_DEBUG, "\n");

    switch(taskState)
    {
    case MAT_STOP:
      coilPwmSet(0);
      matState = MAT_OFF;
      taskState = MAT_CURR;
      break;

    case MAT_START:
      coilPwmSet(PWM_DUTY);
      matState = MAT_ON;

      taskState = MAT_WAIT;
      break;

    case MAT_WAIT:
      if(matState == MAT_ON)
      {
        if(eADC_Data.Iin < coilCurr(eButton[BUTTON_COIL].cnt, CURR_MIN, CURR_GAP) - 2)
        {
          taskState = MAT_STOP;
          ledCtrl(LED_COIL, LED_OFF);
        }
        else
        {
          ledCtrl(LED_COIL, LED_TOGGLE);
        }
      }
      else if(matState == MAT_OFF)
      {
        if(eADC_Data.Iin > coilCurr(eButton[BUTTON_COIL].cnt, CURR_MIN, CURR_GAP))
          taskState = MAT_START;
      }
      break;

    case MAT_CURR: //Mat off(PWM duty : 0%) 상태에서 5초마다 PWM을 듀티 5%로 활성화.
      if(msWait("MAT_CURR", 0, 5000) == true)
      {
        coilPwmSet(5);
        taskState = MAT_CURR_WAIT;
        msWait_Delete("MAT_CURR_WAIT", 0);
      }
      break;

    case MAT_CURR_WAIT: //
      if(msWait("MAT_CURR_WAIT", 0, 1000) == true)
      {
        if(eADC_Data.Iin < coilCurr(eButton[BUTTON_COIL].cnt, CURR_MIN, CURR_GAP))
        {
          taskState = MAT_STOP;
        }
        else
        {
          taskState = MAT_START;
        }
      }
      break;
    }
    //********************* Heating Task Code End *********************//

  }
}

#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#endif //#ifdef _USE_HW_COIL
