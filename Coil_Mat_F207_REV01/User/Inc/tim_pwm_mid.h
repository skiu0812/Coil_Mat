/*
 * tim_pwm_mid.h
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */

#ifndef INC_TIM_PWM_MID_H_
#define INC_TIM_PWM_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_PWM


typedef struct
{
  uint32_t  prescaler;
  uint32_t  period;
  uint32_t  tim_freq;

  uint32_t  duty_max;
  uint32_t  dead_time;

  float     duty_rate;
  uint32_t  duty_ho_oc;
  uint32_t  duty_lo_oc;
  uint32_t  duty_lo;
  uint32_t  duty_ho;
}tim_pwm_t;

extern tim_pwm_t eTimPWM;

bool timPWMInit(void);
bool timSetPWMFrequency(tim_pwm_t *tp, uint32_t freq);
uint32_t timSetDutyMax(tim_pwm_t *tp, float rate);
bool timSetPWMBoostDutyRate(tim_pwm_t *tp, float dr);
bool timSetDutyRateFixedVolt(void);


#endif //#ifdef _USE_HW_PWM


#endif /* INC_TIM_PWM_MID_H_ */
