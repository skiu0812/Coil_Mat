/*
 * coil_solenoid_api.c
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_COIL

#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)


#define PWM_DUTY            HW_COIL_SOLENOID_PWM_DUTY
#define CURR_MIN            HW_COIL_SOLENOID_CURR_MIN * HW_COIL_SOLENOID_UNIT_NUM
#define CURR_GAP            HW_COIL_SOLENOID_CURR_GAP
#define UNIT_TIME           HW_COIL_SOLENOID_UNIT_TIME
#define HOUR_UNIT           (UNIT_TIME)
#define MINUTE_UNIT         ((UNIT_TIME)/60)
#define SECOND_UNIT         (((UNIT_TIME)/60)/60)


solenoidState_t eSolenoid;


void Task_Solenoid(uint32_t taskPeriod)
{
//  static uint8_t  cnt = 0;
  uint32_t ElapsedTime, RemainingTime;

  if(timTask_Run(&eTmr_Task_Coil, taskPeriod) == true)
  {

    //********************* Solenoid Task Code Begin *********************//
    adc1DataGet();

    if(msWait("Task_Solenoid", 0, 100) == true)
    {
      uartPrintf(UART_DEBUG, "Vin:%d, ", eADC_Data.Vin);
      uartPrintf(UART_DEBUG, "Vout:%d, ", eADC_Data.Vout);
      uartPrintf(UART_DEBUG, "Iin:%d, ", eADC_Data.Iin);
      uartPrintf(UART_DEBUG, "Iset:%d, ", coilCurr(eButton[BUTTON_COIL].cnt, eSolenoid.unitCutoffCurr, eSolenoid.unitNumber, CURR_GAP));
      uartPrintf(UART_DEBUG, "BT[FN]:%d, ", eButton[BUTTON_COIL].cnt);
      uartPrintf(UART_DEBUG, "BT[TM]:%d, ", eButton[BUTTON_TIMER].cnt);
      uartPrintf(UART_DEBUG, "Timer:%d, ", eButton[BUTTON_TIMER].cnt);
      uartPrintf(UART_DEBUG, "\n");
    }

    if(eButton[BUTTON_COIL].click_new == true)
    { //전원 버튼이 눌렸을 때
      eButton[BUTTON_COIL].click_new = false;

      if(eButton[BUTTON_COIL].cnt == 0)
      {
        eSolenoid.taskState = SOLENOID_STOP;
        ledCtrl(LED_POWER, LED_OFF);
      }
      else if(eButton[BUTTON_COIL].cnt > 0)
      {
        eSolenoid.taskState = SOLENOID_START;
        ledCtrl(LED_POWER, LED_ON);
      }
    }

    if(eButton[BUTTON_TIMER].click_new == true)
    { //타이머 버튼이 눌렸을 때
      if(eButton[BUTTON_TIMER].cnt > 0 && eButton[BUTTON_COIL].cnt > 0)
      {
        eButton[BUTTON_TIMER].click_new = false;
        eSolenoid.startTime     = millis();
        eSolenoid.runningTime   = eButton[BUTTON_TIMER].cnt * (UNIT_TIME);
        eSolenoid.timerState    = TIMER_ON;
        ledCtrl(LED_TIME, LED_ON);
      }
      else if(eButton[BUTTON_TIMER].cnt == 0)
      {
        eButton[BUTTON_TIMER].click_new = false;
        eSolenoid.startTime     = 0;
        eSolenoid.runningTime   = 0;
        eSolenoid.remainingTime = 0;
        eSolenoid.timerState    = TIMER_OFF;
        ledCtrl(LED_TIME, LED_OFF);
      }
    }
    else
    { //타이머 버튼이 눌리지 않았을 때
      if(eSolenoid.timerState == TIMER_ON && eButton[BUTTON_COIL].cnt > 0)
      {
        ElapsedTime = millis() - eSolenoid.startTime;

        if(ElapsedTime > eSolenoid.runningTime)
        {
          eSolenoid.taskState = SOLENOID_STOP;
        }
        else
        {
          RemainingTime = eSolenoid.runningTime - ElapsedTime;
          eSolenoid.remainingTime = (uint32_t)(RemainingTime / (MINUTE_UNIT));
        }
      }
    }

    switch(eSolenoid.taskState)
    {
    case SOLENOID_STOP:
      coilPwmSet(0);
      eButton[BUTTON_COIL].cnt      = 0;
      eButton[BUTTON_TIMER].cnt     = 0;

      eSolenoid.taskState           = SOLENOID_STOP_WAIT;
      eSolenoid.currcheckState      = CURRENTCHECK_OFF;
      eSolenoid.timerState          = TIMER_OFF;

      eSolenoid.startTime           = 0;
      eSolenoid.runningTime         = 0;
      eSolenoid.remainingTime       = 0;

      ledCtrl(LED_COIL, LED_OFF);
      break;

    case SOLENOID_STOP_WAIT:
      if(msWait("SOLENOID_STOP_WAIT", 0, 1000) == true)
      {
//        ledCtrl(LED_COIL, LED_TOGGLE);
      }
      break;

    case SOLENOID_START:
      coilPwmSet(eSolenoid.pwmDutyRatio);

      if(msWait("SOLENOID_START", 0, 2000) == true)
      {
        eSolenoid.taskState       = SOLENOID_START_WAIT;
        eSolenoid.currcheckState  = CURRENTCHECK_OFF;
      }

      break;

    case SOLENOID_START_WAIT:
      if(eADC_Data.Iin < coilCurr(eButton[BUTTON_COIL].cnt, eSolenoid.unitCutoffCurr, eSolenoid.unitNumber, CURR_GAP))
      {
        coilPwmSet(0);

        eSolenoid.taskState       = SOLENOID_CURRENTCHECK;
        eSolenoid.currcheckState  = CURRENTCHECK_OFF;

        ledCtrl(LED_COIL, LED_OFF);
      }
      else
      {
        if(msWait("SOLENOID_START_WAIT", 0, 100) == true)
        {
          ledCtrl(LED_COIL, LED_TOGGLE);
        }
      }
      break;

    case SOLENOID_CURRENTCHECK:
      if(eSolenoid.currcheckState == CURRENTCHECK_OFF)
      {
        if(msWait("CURRENTCHECK_START", 0, 10000) == true)
        {
          coilPwmSet(eSolenoid.pwmDutyRatio);
          eSolenoid.currcheckState = CURRENTCHECK_ON;
        }
      }
      else if(eSolenoid.currcheckState == CURRENTCHECK_ON)
      {
        if(msWait("CURRCHECK", 0, 500) == true)
        {
          if(eADC_Data.Iin > coilCurr(eButton[BUTTON_COIL].cnt, eSolenoid.unitCutoffCurr, eSolenoid.unitNumber, CURR_GAP) + 2)
          {
            eSolenoid.taskState = SOLENOID_START;
          }
          else
          {
            coilPwmSet(0);
            eSolenoid.currcheckState  = CURRENTCHECK_OFF;
          }
        }
      }
      break;
    }
    //********************* Solenoid Task Code End *********************//

  }
}

#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

#endif //#ifdef _USE_HW_COIL
