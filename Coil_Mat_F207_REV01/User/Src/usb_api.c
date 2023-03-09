/*
 * usb_api.c
 *
 *  Created on: Sep 6, 2022
 *      Author: KSK
 */


#include "user.h"
#include "usbd_def.h"
#include "usbd_cdc_if.h"


#ifdef _USE_HW_USB


extern USBD_HandleTypeDef hUsbDeviceFS;


void Task_USB(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_USB, taskPeriod) == true)
  {
    //********************* USB Task Code Begin *********************//
    usbCDCSOF(&hUsbDeviceFS);
    //********************* USB Task Code End *********************//
  } //if(timTask_Run(&eTmr_Task_USB, taskPeriod) == true)
} //void Task_USB(uint32_t taskPeriod)


#endif //#ifdef _USE_HW_USB
