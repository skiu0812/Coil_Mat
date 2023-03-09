/*
 * led_api.c
 *
 *  Created on: 2023. 2. 28.
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_LED


void Task_LED(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_LED, taskPeriod) == true)
  {
    //********************* LED Task Code Begin *********************//

    //********************* LED Task Code end *********************//
  }
}


#endif //#ifdef _USE_HW_LED
