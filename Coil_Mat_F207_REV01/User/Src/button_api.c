/*
 * button_api.c
 *
 *  Created on: 2022. 9. 25.
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_BUTTON


void Task_Button(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_Button, taskPeriod) == true)
  {
    //********************* Button Task Code Begin *********************//
    if(buttonObjGetClicked(BUTTON_COIL) == true)
    {
      eButton[BUTTON_COIL].click_new = true;
      eButton[BUTTON_COIL].cnt = (eButton[BUTTON_COIL].cnt + 1) % HW_BUTTON1_CNT_MAX;
      uartPrintf(UART_CLI, "BtnClicked1 : %d, BtnPressing_cnt1 : %d\n", (int)eButton[BUTTON_COIL].cnt, (int)eButton[BUTTON_COIL].pressing_cnt);
    }

    if(buttonObjGetClicked(BUTTON_TIMER) == true)
    {
      eButton[BUTTON_TIMER].click_new = true;
      eButton[BUTTON_TIMER].cnt = (eButton[BUTTON_TIMER].cnt + 1) % HW_BUTTON2_CNT_MAX;
      uartPrintf(UART_CLI, "BtnClicked2 : %d, BtnPressing_cnt2 : %d\n", (int)eButton[BUTTON_TIMER].cnt, (int)eButton[BUTTON_TIMER].pressing_cnt);
    }
    //********************* Button Task Code end *********************//
  }
}


#endif //#ifdef _USE_HW_BUTTON
