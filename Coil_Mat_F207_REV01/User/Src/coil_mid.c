/*
 * coil_mid.c
 *
 *  Created on: 2023. 2. 21.
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_COIL


#ifdef _USE_HW_CLI
static void cliCoil(cli_args_t *args);
#endif


bool coilInit(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
  cliAdd("coil", cliCoil);
#endif

#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)

#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
  ret &= coilSolenoidOpen();
#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

  return ret;
}


bool coilSolenoidOpen(void)
{
  bool ret = true;

  coilPwmSet(0);

  eButton[BUTTON_COIL].cnt        = 0;
  eButton[BUTTON_TIMER].cnt       = 0;

  eSolenoid.taskState             = SOLENOID_STOP;
  eSolenoid.currcheckState        = CURRENTCHECK_OFF;

  eSolenoid.pwmDutyRatio          = HW_COIL_SOLENOID_PWM_DUTY;
  eSolenoid.unitCutoffCurr        = HW_COIL_SOLENOID_CURR_MIN;
  eSolenoid.unitNumber            = HW_COIL_SOLENOID_UNIT_NUM;

  eSolenoid.timerState            = TIMER_OFF;
  eSolenoid.startTime             = 0;
  eSolenoid.runningTime           = 0;
  eSolenoid.remainingTime         = 0;

  return ret;
}


/*
 * param
 * dr : Duty Rate (float)(5~100)
 */
bool coilPwmSet(float dr)
{
  bool ret = true;

  if(dr == 100)
  {
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_1, 50);
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_2, 100);
  }
  else if(dr >= 5 && dr < 100)
  {
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_1, dr-1);
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_2, dr);
  }
  else if(dr < 5)
  {
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_1, 0);
    ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_2, 0);
  }

  return ret;
}

uint32_t coilCurr(uint32_t btn_lv, uint16_t curr_min, uint8_t unit_num, uint8_t curr_gap)
{
  uint32_t curr;

  if(btn_lv > 0)
    curr = (curr_min * unit_num) - (btn_lv - curr_gap);
  else
    curr = 1000;

  return curr;
}


uint32_t coilMaxCurr(uint32_t vin)
{
  uint32_t curr = 0;

  curr = (uint32_t)(((float)vin * 0.246) + 0.5);

  return curr;
}


#ifdef _USE_HW_CLI
void cliCoil(cli_args_t *args)
{
  bool ret = true;

  if(args->isStr(0, "sol") == true)
  {
    if(args->isStr(1, "info") == true)
    {
      cliPrintf("\n=====< Solenoid Information >=====\n");
      cliPrintf("- PWM Duty Rate\t\t: %d [%%]\n", eSolenoid.pwmDutyRatio);
      cliPrintf("- Unit Quantity\t\t: %d [pcs]\n", eSolenoid.unitNumber);
      cliPrintf("- Cutoff Current\t: %f [A]/ea\n", (float)eSolenoid.unitCutoffCurr/100);
    }
    else if(args->isStr(1, "duty") == true)
    {
      if(args->getData(2) >= 0 && args->getData(2) <= 100)
      {
        eSolenoid.pwmDutyRatio = (uint8_t)args->getData(2);
        cliPrintf("\nSuccessfully set duty ratio to %d!!\n", eSolenoid.pwmDutyRatio);
      }
      else
      {
        cliPrintf("\nThe entered number out of range[0~100]\n");
      }
    }
    else if(args->isStr(1, "num") == true)
    {
      if(args->getData(2) > 0 && args->getData(2) <= 5)
      {
        eSolenoid.unitNumber = (uint8_t)args->getData(2);
        cliPrintf("\nSuccessfully set units to %d!!\n", eSolenoid.unitNumber);
      }
      else
      {
        cliPrintf("\nThe entered number out of range[1~5]\n");
      }
    }
    else if(args->isStr(1, "curr") == true)
    {
      eSolenoid.unitCutoffCurr = (uint16_t)args->getData(2);
      cliPrintf("\nSuccessfully set the cutoff current to %d!!\n", eSolenoid.unitCutoffCurr);
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf("\n--------------------[ Coil Commands ]---------------------------------\n\n");

    cliPrintf(">> coil sol info\t\t: Solenoid Information\n\n");
    cliPrintf(">> coil sol duty [d]\t\t: Solenoid Duty(0~100)\n");
    cliPrintf(">> coil sol num [n]\t\t: Solenoid Unit Number(1~5)\n");
    cliPrintf(">> coil sol curr [c]\t\t: Solenoid Minimum Current per a Unit\n");

    cliPrintf("\n----------------------------------------------------------------------\n\n");
  }
}

#endif


#endif //#ifdef _USE_HW_COIL
