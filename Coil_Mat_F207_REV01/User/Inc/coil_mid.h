/*
 * coil_mid.h
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */

#ifndef INC_COIL_MID_H_
#define INC_COIL_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_COIL


bool      coilInit(void);
bool      coilSolenoidOpen(void);
bool      coilPwmSet(float dr);
uint32_t  coilCurr(uint32_t btn_lv, uint16_t curr_min, uint8_t unit_num, uint8_t curr_gap);
uint32_t  coilMaxCurr(uint32_t vin);


#endif //#ifdef _USE_HW_COIL


#endif /* INC_COIL_MID_H_ */
