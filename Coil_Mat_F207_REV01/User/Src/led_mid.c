/*
 * led_mid.c
 *
 *  Created on: 2022. 9. 14.
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_LED





led_tbl_t led_tbl[LED_CH_MAX] =
{
    {M_GPIO_LED_1_GPIO_Port,  M_GPIO_LED_1_Pin, GPIO_PIN_RESET, GPIO_PIN_SET},
    {M_GPIO_LED_2_GPIO_Port,  M_GPIO_LED_2_Pin, GPIO_PIN_RESET, GPIO_PIN_SET},
    {M_GPIO_LED_3_GPIO_Port,  M_GPIO_LED_3_Pin, GPIO_PIN_RESET, GPIO_PIN_SET},
};


#ifdef _USE_HW_CLI
static void cliLed(cli_args_t *args);
#endif


bool ledInit(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
  cliAdd("led", cliLed);
#endif

  for(int i=0; i<LED_CH_MAX; i++)
  {
    ledCtrl(i, LED_OFF);
  }

  return ret;
}

/*
 * ch : def.h -> _DEF_LEDx
 */
void ledOn(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

/*
 * ch : def.h -> _DEF_LEDx
 */
void ledOff(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

/*
 * ch : def.h -> _DEF_LEDx
 */
void ledToggle(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}


/*
 * ch : def.h -> _DEF_LEDx
 */
void ledCtrl(uint8_t ch, uint8_t state)
{
  if(ch >= LED_CH_MAX) return;

  if(state == LED_OFF)
  {
    HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
  }
  else if(state == LED_ON)
  {
    HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
  }
  else if(state == LED_TOGGLE)
  {
    HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
  }
}

#ifdef _USE_HW_CLI
void cliLed(cli_args_t *args)
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
    cliPrintf("------------[ LED Commands ]-------------\n");
    cliPrintf(">> led toggle ch[1~%d] time(ms)\n", LED_CH_MAX);
    cliPrintf("-----------------------------------------\n");
  }
}
#endif //#ifdef _USE_HW_CLI


#endif //#ifdef _USE_HW_LED
