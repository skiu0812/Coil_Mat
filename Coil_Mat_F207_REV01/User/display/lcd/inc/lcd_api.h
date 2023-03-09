/*
 * lcd_api.h
 *
 *  Created on: 2023. 1. 30.
 *      Author: skiu0
 */

#ifndef DISPLAY_LCD_INC_LCD_API_H_
#define DISPLAY_LCD_INC_LCD_API_H_


#include "hw_def.h"


#ifdef _USE_HW_LCD

void Task_LCD(uint32_t taskPeriod);

#endif //#ifdef _USE_HW_LCD


#endif /* DISPLAY_LCD_INC_LCD_API_H_ */
