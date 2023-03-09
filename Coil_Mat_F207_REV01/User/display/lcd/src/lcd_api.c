/*
 * lcd_api.c
 *
 *  Created on: 2023. 1. 30.
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_LCD

void Task_LCD(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_LCD, taskPeriod) == true)
  {
    //********************* LCD Task Code Begin *********************//
    /* Background Color */
    lcdClearBuffer(black);

    /* Top Bar */
//    lcdDrawFillRoundRect(0, 0, 128, 15, 0, hyundai_blue);

    /* Temperature Button */
//    lcdDrawFillRoundRect(12, 22, 32, 20, 4, brown);
//    lcdDrawFillRoundRect(10, 20, 32, 20, 4, red);
//
//    lcdDrawFillRoundRect(48, 22, 32, 20, 4, brown);
//    lcdDrawFillRoundRect(46, 20, 32, 20, 4, coral_red);
//
//    lcdDrawFillRoundRect(84, 22, 32, 20, 4, brown);
//    lcdDrawFillRoundRect(82, 20, 32, 20, 4, coral_reef);
//
//    lcdDrawFillRoundRect(120, 22, 32, 20, 4, brown);
//    lcdDrawFillRoundRect(118, 20, 32, 20, 4, tulip);
//
//    /* Timer Button */
//    lcdDrawFillRoundRect(12, 49, 32, 20, 4, darkblue);
//    lcdDrawFillRoundRect(10, 47, 32, 20, 4, lightblue);
//
//    lcdDrawFillRoundRect(48, 49, 32, 20, 4, darkblue);
//    lcdDrawFillRoundRect(46, 47, 32, 20, 4, lightblue);
//
//    lcdDrawFillRoundRect(84, 49, 32, 20, 4, darkblue);
//    lcdDrawFillRoundRect(82, 47, 32, 20, 4, lightblue);
//
//    lcdDrawFillRoundRect(120, 49, 32, 20, 4, darkblue);
//    lcdDrawFillRoundRect(118, 47, 32, 20, 4, lightblue);

    /* LV printf */
//    lcdPrintf(10, 30, black, "Heat LV : %d", eButton[BUTTON_COIL].cnt);
//    lcdPrintf(10, 47, black, "Timer   : -");

#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
    lprintf(&font_tbl[GULIM8B], 5, 2, coral_red, white, 1, "Heating");
    lprintf(&font_tbl[HYTAEBAEK48B], 45, 14, coral_red, white, 1, "%d", eButton[BUTTON_COIL].cnt);
#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
    lprintf(&font_tbl[GULIM8B], 5, 2, coral_red, white, 1, "Magnet");
    if(eButton[BUTTON_COIL].cnt == 0)
      lprintf(&font_tbl[HYTAEBAEK36B], 16, 25, coral_red, white, 1, "OFF");
    else if(eButton[BUTTON_COIL].cnt > 0)
      lprintf(&font_tbl[HYTAEBAEK36B], 24, 25, coral_red, white, 1, "ON");
#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

    lprintf(&font_tbl[GULIM8B], 5, 86, lightblue, white, 1, "Timer");
    lprintf(&font_tbl[HYTAEBAEK48B], 45, 98, lightblue, white, 1, "%d", eButton[BUTTON_TIMER].cnt);
    if(eSolenoid.remainingTime > 0)
    {
      lprintf(&font_tbl[GULIM8B], 100, 145, lightblue, white, 1, "%d", eSolenoid.remainingTime);
    }

    /* Mat Image */
//    lcdDrawImage_GradAtoB(&image_tbl[mat], transp_on, green, white, red, 1, 300);  // skkim-lcd
//    lcdDrawImage_Move(&image_tbl[mat_heating], transp_on, 52, 70, 52, 60, 1, 300);

    /* Update to LCD */
    lcdUpdateDraw();
    //********************* LCD Task Code End *********************//
  }
}

#endif //#ifdef _USE_HW_LCD
