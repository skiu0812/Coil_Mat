/*
 * button_api.h
 *
 *  Created on: 2022. 9. 25.
 *      Author: KSK
 */

#ifndef INC_BUTTON_API_H_
#define INC_BUTTON_API_H_


#include "hw_def.h"


#ifdef _USE_HW_BUTTON


void Task_Button(uint32_t taskPeriod);


#endif //#ifdef _USE_HW_BUTTON



#endif /* INC_BUTTON_API_H_ */
