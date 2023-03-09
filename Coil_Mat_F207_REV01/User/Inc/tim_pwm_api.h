/*
 * tim_pwm_api.h
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */

#ifndef INC_TIM_PWM_API_H_
#define INC_TIM_PWM_API_H_


#include "hw_def.h"


#ifdef _USE_HW_PWM


extern uint32_t eHeating;


void Task_TIM_PWM(uint32_t taskPeriod);


#endif //#ifdef _USE_HW_PWM


#endif /* INC_TIM_PWM_API_H_ */
