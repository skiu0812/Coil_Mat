/*
 * tim_pwm_mid.c
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_PWM


#define TIMER_FREQ            HW_TIM_APB1_CLOCK // (Hz)

#define _SET_PWM_FREQ         480000 //(Hz)

#define _SET_DEAD_TIME_RATE   1  //PERIOD(AUTORELOAD)의 x%
#define _SET_DUTY_MAX_RATE    100 //PERIOD(AUTORELOAD)의 x%

#define _SET_DUTY_RATE        40 //DUTY_MAX의 x%


tim_pwm_t eTimPWM;


bool timPWMInit(void)
{
  bool ret = true;

  eTimPWM.period        = 0;
  eTimPWM.prescaler     = 0;
  eTimPWM.tim_freq      = 0;

  eTimPWM.duty_max      = 0;
  eTimPWM.dead_time     = 0;

  eTimPWM.duty_rate     = 0;
  eTimPWM.duty_lo       = 0;
  eTimPWM.duty_ho       = 0;

  timSetPWMFrequency(&eTimPWM, _SET_PWM_FREQ);
  timSetDutyMax(&eTimPWM, _SET_DUTY_MAX_RATE);
  timSetPWMBoostDutyRate(&eTimPWM, _SET_DUTY_RATE);

  return ret;
}


bool timSetPWMFrequency(tim_pwm_t *tp, uint32_t tim_freq)
{
  bool      ret = false;
  uint32_t  prescaler;
  uint32_t  period;
  uint32_t  dead_time;

  if(tim_freq > 0 && tim_freq < TIMER_FREQ)
  {
    prescaler = __HAL_TIM_GET_PRESCALER(&htim2) + 1;
    period    = (uint32_t)(TIMER_FREQ / (prescaler*tim_freq));
    dead_time = (uint32_t)(((period * _SET_DEAD_TIME_RATE) / 100) + 0.5);

//    __HAL_TIM_SET_PRESCALER()
    __HAL_TIM_SET_AUTORELOAD(&htim2, period-1);
    __HAL_TIM_SET_AUTORELOAD(&htim3, period-1);

    if(period == __HAL_TIM_GET_AUTORELOAD(&htim2)+1 &&
        period == __HAL_TIM_GET_AUTORELOAD(&htim3)+1)
    {
      tp->prescaler = prescaler-1;
      tp->period    = period-1;
      tp->tim_freq  = tim_freq;
      tp->dead_time = dead_time;

      ret = true;
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  return ret;
}

uint32_t timSetDutyMax(tim_pwm_t *tp, float rate)
{
  uint32_t ret = false;
  uint32_t period;

  period = tp->period + 1;

  tp->duty_max = (uint32_t)(((period * rate) / 100) + 0.5);

  ret = tp->duty_max;

  return ret;
}


bool timSetPWMBoostDutyRate(tim_pwm_t *tp, float dr)
{
  bool ret = false;
  uint32_t duty_ho_oc;
  uint32_t duty_lo_oc;
  uint32_t duty_ho;
  uint32_t duty_lo;
  uint32_t duty_max;

  duty_max = tp->duty_max;

  if((dr > 0 && dr < 100))
  {
    duty_lo = (uint32_t)(((duty_max * dr) / 100) + 0.5);
    duty_ho = (uint32_t)(duty_max - duty_lo);

    duty_lo_oc = duty_lo;
    duty_ho_oc = (uint32_t)((duty_ho/3)+0.5);

    duty_lo = duty_lo;
    duty_ho = duty_ho - (tp->dead_time * 6);

    if(duty_lo_oc == 0) duty_lo_oc = 1;
    if(duty_ho_oc == 0) duty_ho_oc = 1;
    if(duty_lo == 0) duty_lo = 1;
    if(duty_ho == 0) duty_ho = 1;

  }
  else if(dr == 0)
  {
    duty_lo_oc = 1;
    duty_ho_oc = 1;
    duty_lo = 1;
    duty_ho = tp->duty_max+1;
  }
  else if(dr == 100)
  {
    duty_lo_oc = 1;
    duty_ho_oc = 1;
    duty_lo = tp->duty_max+1;
    duty_ho = 1;
  }
  else
  {
    return false;
  }

  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty_lo_oc-1);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty_lo-1);

  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty_ho_oc-1);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty_ho-1);

  if( duty_lo == __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2) + 1 &&
      duty_ho == __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2) + 1)
  {
    tp->duty_rate     = dr;
    tp->duty_ho_oc    = duty_ho_oc;
    tp->duty_lo_oc    = duty_lo_oc;
    tp->duty_ho       = duty_ho;
    tp->duty_lo       = duty_lo;

    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool timSetDutyRateFixedVolt(void)
{
  bool ret = false;
  float dr;

  dr = eTimPWM.duty_rate;


  if(eADC_Data.Vin > 1100 && eADC_Data.Vin < 1600)
  {
    if(eADC_Data.Vout < 2370)
    {
      if(dr >= 50) dr = 50;
      else dr = dr + 0.1;

      timSetPWMBoostDutyRate(&eTimPWM, dr);
    }
    else if(eADC_Data.Vout > 2430)
    {
      if(dr <= 0) dr = 0;
      else dr = dr - 0.1;

      timSetPWMBoostDutyRate(&eTimPWM, dr);
    }
    else
    {
      ret = true;
    }
  }
  else
  {
    timSetPWMBoostDutyRate(&eTimPWM, 0);
  }

  return ret;
}

#endif //#ifdef _USE_HW_PWM
