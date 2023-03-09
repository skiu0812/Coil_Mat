/*
 * uart_api.h
 *
 *  Created on: Sep 26, 2022
 *      Author: KSK
 */

#ifndef INC_UART_API_H_
#define INC_UART_API_H_


#include "hw_def.h"


#ifdef _USE_HW_UART


void Task_UART(uint32_t taskPeriod);


#endif //#ifdef _USE_HW_UART


#endif /* INC_UART_API_H_ */
