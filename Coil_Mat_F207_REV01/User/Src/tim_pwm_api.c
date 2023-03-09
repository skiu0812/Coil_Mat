/*
 * tim_pwm_api.c
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_PWM


#define VOUT    1500


typedef enum
{
  PWM_ON = 0,
  HEATING,
  COOLDOWN,

}PWM_State_t;

uint32_t eHeating=0;


void Task_TIM_PWM(uint32_t taskPeriod)
{
  static PWM_State_t sPWMState = 0;
  static uint8_t state = 0;
  static uint32_t pretime;

  if(eTmr_Task_TIM_PWM.cnt == 0)
  {
    //***** for Task Timer *****//
    if(!eTmr_Task_TIM_PWM.is_set)
    {
      uSetTaskCntMax(&eTmr_Task_TIM_PWM, taskPeriod);
    }
    else
    {
      eTmr_Task_TIM_PWM.cnt = eTmr_Task_TIM_PWM.cntmax;
    }

    switch(sPWMState)
    {
    case PWM_ON:
      if(timSetDutyRateFixedVolt())
      {
        sPWMState = HEATING;
        pretime = millis();
      }
      break;

    case HEATING:
      if(timSetDutyRateFixedVolt())
      {
        if(millis()-pretime >= 5000)
        {
          if(eADC_Data.Iin <= 680)
          {
            timSetPWMBoostDutyRate(&eTimPWM, 0);
            sPWMState = COOLDOWN;
          }
          pretime = millis();
        }
      }
      break;

    case COOLDOWN:
      if(state == 0 && millis()-pretime >= 20000)
      {
        state = 1;
      }

      if(state > 0 && timSetDutyRateFixedVolt()==true)
      {
        switch(state)
        {
        case 1:
          pretime = millis();
          state = 2;
          break;
        case 2:
          if(millis()-pretime >= 500)
          {
            state = 3;
          }
          break;
        case 3:
          if(eADC_Data.Iin < 690)
          {
            timSetPWMBoostDutyRate(&eTimPWM, 0);
          }
          else
          {
            eHeating++;
            sPWMState = HEATING;
          }
          state = 0;
          pretime = millis();
          break;
        }
      }

      break;
    }
  }
}


#endif //#ifdef _USE_HW_PWM
