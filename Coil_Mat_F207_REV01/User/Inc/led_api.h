/*
 * led_api.h
 *
 *  Created on: 2023. 2. 28.
 *      Author: skiu0
 */

#ifndef INC_LED_API_H_
#define INC_LED_API_H_


#include "hw_def.h"


#ifdef _USE_HW_LED


void Task_LED(uint32_t taskPeriod);


#endif //#ifdef _USE_HW_LED



#endif /* INC_LED_API_H_ */
