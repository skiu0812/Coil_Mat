/*
 * usb_api.h
 *
 *  Created on: Sep 6, 2022
 *      Author: KSK
 */

#ifndef INC_USB_API_H_
#define INC_USB_API_H_


#include "hw_def.h"


#ifdef _USE_HW_USB


void Task_USB(uint32_t taskPeriod);


#endif


#endif /* INC_USB_API_H_ */
