/*
 * adc_api.h
 *
 *  Created on: Dec 12, 2022
 *      Author: skiu0
 */

#ifndef INC_ADC_API_H_
#define INC_ADC_API_H_


#include "hw_def.h"


#ifdef _USE_HW_ADC1


void Task_ADC(uint32_t taskPeriod);


#endif //#ifdef _USE_HW_ADC1


#endif /* INC_ADC_API_H_ */
