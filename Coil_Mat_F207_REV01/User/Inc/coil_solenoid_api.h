/*
 * coil_solenoid_api.h
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */

#ifndef INC_COIL_SOLENOID_API_H_
#define INC_COIL_SOLENOID_API_H_


#include "hw_def.h"


#ifdef _USE_HW_COIL

#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

typedef struct
{
  uint8_t   taskState;
  uint8_t   currcheckState;

  uint8_t   pwmDutyRatio;
  uint16_t  unitCutoffCurr;   // Single Unit Cutoff Current
  uint8_t   unitNumber;       // Number of Unit

  uint8_t   timerState;
  uint32_t  startTime;
  uint32_t  runningTime;
  uint32_t  remainingTime;
}solenoidState_t;

typedef enum
{
  SOLENOID_STOP = 0,
  SOLENOID_STOP_WAIT,
  SOLENOID_START,
  SOLENOID_START_WAIT,
  SOLENOID_CURRENTCHECK,
  SOLENOID_TIMER,
}solenoidTaskState_e;

typedef enum
{
  BUTTON_OFF = 0,
  BUTTON_ON,
}solenoidButtonkState_e;

typedef enum
{
  TIMER_OFF = 0,
  TIMER_ON,
}solenoidTimerState_e;

typedef enum
{
  SOLENOID_OFF = 0,
  SOLENOID_ON,
}solenoidMagnetState_e;

typedef enum
{
  CURRENTCHECK_OFF = 0,
  CURRENTCHECK_ON,
}solenoidCurrentCheckState_e;


extern solenoidState_t eSolenoid;


void Task_Solenoid(uint32_t taskPeriod);

#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

#endif //#ifdef _USE_HW_COIL


#endif /* INC_COIL_SOLENOID_API_H_ */
