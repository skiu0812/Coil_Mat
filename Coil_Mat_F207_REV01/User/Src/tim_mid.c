/*
 * tim_mid.c
 *
 *  Created on: Aug 16, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_TIM


tim_t eTim[HW_TIM_CH_MAX];

volatile uint32_t eTmr_MainClock   =  0;
task_cnt_t eTmr_Task_ADC          = {0};
task_cnt_t eTmr_Task_UART         = {0};
task_cnt_t eTmr_Task_USB          = {0};
task_cnt_t eTmr_Task_Button       = {0};
task_cnt_t eTmr_Task_LED          = {0};
task_cnt_t eTmr_Task_WDT          = {0};
task_cnt_t eTmr_Task_CLI          = {0};
task_cnt_t eTmr_Task_TIM_PWM      = {0};
task_cnt_t eTmr_Task_Coil         = {0};
task_cnt_t eTmr_Task_LCD          = {0};


#ifdef _USE_HW_CLI
static void cliTim(cli_args_t *args);
#endif

bool timInit(void)
{
  bool ret = true;

  ret &= timOpen();
  ret &= timStart();

#ifdef _USE_HW_CLI
  cliAdd("tim", cliTim);
#endif

  return ret;
}

bool timOpen(void)
{
  bool ret = true;

  ret &= timSet(TIM_TASK, &htim14,  HW_TIM_APB1_CLOCK,  HW_TIM_TASK_FREQ);

  ret &= timSet(TIM_COIL, &htim3,   HW_TIM_APB1_CLOCK, HW_TIM_COIL_FREQ);
  ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_1, 50);
  ret &= timPWMSet(TIM_COIL, TIM_CHANNEL_2, 0);

  ret &= timSet(TIM_ADC,  &htim2,   HW_TIM_APB1_CLOCK, HW_TIM_COIL_FREQ);
//  ret &= timPWMSet(TIM_ADC, TIM_CHANNEL_1, 50);
  ret &= timPWMSet(TIM_ADC, TIM_CHANNEL_2, 50);

  return ret;
}

bool timStart(void)
{
  bool ret = true;

  //TIM Start TIM_TASK
  HAL_TIM_Base_Start_IT(&htim14);

  //TIM Start TIM_COIL
  HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  //TIM Start TIM_ADC
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

  return ret;
}


bool timSet(uint8_t tim, TIM_HandleTypeDef *htim, uint32_t clock, float freq)
{
  bool ret = false;
  float peri_temp;

  if(freq > 0 && freq <= clock)
  {
    eTim[tim].htim   = htim;
    eTim[tim].clock  = clock;
    eTim[tim].freq   = freq;

    peri_temp = (float)clock / freq;

    if(peri_temp >= HW_TIM_PERIOD_MAX)
    {
      eTim[tim].prescaler = (uint32_t)((peri_temp / (float)HW_TIM_PERIOD) + 0.5);
      eTim[tim].period = (uint32_t)((peri_temp / (float)eTim[tim].prescaler) + 0.5);
    }
    else if(peri_temp < HW_TIM_PERIOD_MAX)
    {
      eTim[tim].prescaler = 1;
      eTim[tim].period = (uint32_t)peri_temp;
    }

    if(eTim[tim].prescaler > 0 && eTim[tim].period > 0)
    {
      __HAL_TIM_SET_PRESCALER(htim, (eTim[tim].prescaler - 1));
      __HAL_TIM_SET_AUTORELOAD(htim, (eTim[tim].period - 1));
      delay(50);

      ret = true;
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

  return ret;
}

bool timPWMSet(uint8_t tim, uint32_t ch, float dr)
{
  bool ret = false;
  uint32_t peri_temp, duty_temp;

  if(dr >= 0 && dr <= 100)
  {
    eTim[tim].ch[ch/4].ch         = ch;
    eTim[tim].ch[ch/4].duty_rate  = dr;

    peri_temp = __HAL_TIM_GET_AUTORELOAD(eTim[tim].htim) + 1;

    duty_temp = (uint32_t)((((float)peri_temp / 100) * dr) + 0.5);

    eTim[tim].ch[ch/4].duty = duty_temp;

    __HAL_TIM_SET_COMPARE(eTim[tim].htim, ch, eTim[tim].ch[ch/4].duty);
    delay(50);

    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

float timPWMGet(uint8_t tim, uint32_t ch)
{
  float dr;
  uint32_t peri_temp, duty_temp;

  duty_temp = __HAL_TIM_GET_COMPARE(eTim[tim].htim, ch);
  peri_temp = __HAL_TIM_GET_AUTORELOAD(eTim[tim].htim) + 1;

  dr = (float)duty_temp * (100 / (float)peri_temp);

  return dr;
}


bool timTask_Run(task_cnt_t *taskTmr, uint32_t taskPeriod)
{
  bool ret = false;

  if(taskTmr->is_set == true)
  {
    if(taskTmr->cnt <= 0)
    {
      taskTmr->cnt = taskTmr->cntmax;
      ret = true;
    }
  }
  else
  {
    timTask_Set_CntMax(taskTmr, taskPeriod);
  }

  return ret;
}

void timTask_Set_CntMax(task_cnt_t *taskTmr, uint32_t taskPeriod)
{
  taskTmr->cnt    = 0;
  taskTmr->period = taskPeriod;
  taskTmr->cntmax = (uint32_t)(((float)taskPeriod * ((float)eTim[TIM_TASK].freq/1000)));
  taskTmr->is_set = true;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM14)
  {
    if(eTmr_MainClock)          eTmr_MainClock--;
    if(eTmr_Task_ADC.cnt)       eTmr_Task_ADC.cnt--;
    if(eTmr_Task_UART.cnt)      eTmr_Task_UART.cnt--;
    if(eTmr_Task_USB.cnt)       eTmr_Task_USB.cnt--;
    if(eTmr_Task_Button.cnt)    eTmr_Task_Button.cnt--;
    if(eTmr_Task_LED.cnt)       eTmr_Task_LED.cnt--;
    if(eTmr_Task_WDT.cnt)       eTmr_Task_WDT.cnt--;
    if(eTmr_Task_CLI.cnt)       eTmr_Task_CLI.cnt--;
    if(eTmr_Task_TIM_PWM.cnt)   eTmr_Task_TIM_PWM.cnt--;
    if(eTmr_Task_Coil.cnt)      eTmr_Task_Coil.cnt--;
    if(eTmr_Task_LCD.cnt)       eTmr_Task_LCD.cnt--;
  }
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

}


#ifdef _USE_HW_CLI
void cliTim(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 3 && args->isStr(0, "toggle") == true)
  {
    uint8_t  led_ch;
    uint32_t toggle_time;
    uint32_t pre_time;

    led_ch      = args->getData(1);
    toggle_time = args->getData(2);

    if (led_ch > 0)
    {
      led_ch--;
    }

    pre_time = millis();
    while(cliKeepLoop())
    {
      if (millis()-pre_time >= toggle_time)
      {
        pre_time = millis();
        ledCtrl(led_ch, LED_TOGGLE);
      }
    }
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("\n");
    cliPrintf("------------[ Timer Commands ]-------------\n");
    cliPrintf(">> tim toggle ch[1~%d] time(ms)\n", LED_CH_MAX);
    cliPrintf("-------------------------------------------\n");
  }
}
#endif //#ifdef _USE_HW_CLI


#endif //#ifdef _USE_HW_TIM
