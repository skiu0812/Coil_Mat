/*
 * coil_mat_api.h
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */

#ifndef INC_COIL_MAT_API_H_
#define INC_COIL_MAT_API_H_


#include "hw_def.h"


#ifdef _USE_HW_COIL

#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

typedef enum
{
  MAT_STOP = 0,
  MAT_START,
  MAT_WAIT,
  MAT_CURR,
  MAT_CURR_WAIT,
}matTaskState_e;

typedef enum
{
  MAT_OFF = 0,
  MAT_ON,
}mat_on_off_e;

void Task_Heat(uint32_t taskPeriod);

#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#endif //#ifdef _USE_HW_COIL

#endif /* INC_COIL_MAT_API_H_ */
