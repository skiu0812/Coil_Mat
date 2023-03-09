/*
 * button_mid.c
 *
 *  Created on: Sep 15, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_BUTTON


button_tbl_t eButton_tbl[BUTTON_CH_MAX] =
{
    //port                    pin               on_state
    {M_GPIO_KEY_1_GPIO_Port,  M_GPIO_KEY_1_Pin, GPIO_PIN_RESET},
    {M_GPIO_KEY_2_GPIO_Port,  M_GPIO_KEY_2_Pin, GPIO_PIN_RESET},
};

button_obj_t eButton[BUTTON_CH_MAX];


#ifdef _USE_HW_CLI
static void cliButton(cli_args_t *arg);
#endif


bool buttonInit(void)
{
  bool ret = true;

  //              ch            condition   cnt_max t_clk   r_gap_1 r_gap2
  buttonObjCreate(_DEF_BUTTON1, ATTACHED,   10,     20,     1000,   100);
  buttonObjCreate(_DEF_BUTTON2, ATTACHED,   6,      20,     1000,   100);

#ifdef _USE_HW_CLI
  cliAdd("button", cliButton);
#endif

  return ret;
}


void buttonObjCreate(uint8_t ch, bool condition, uint32_t cnt_max, uint32_t t_clk, uint32_t r_gap_1st, uint32_t r_gap_other)
{
  eButton[ch].state             = 0;
  eButton[ch].pressing_time     = 0;
  eButton[ch].pressing_cnt      = 0;
  eButton[ch].cnt               = 0;
  eButton[ch].cnt_max           = cnt_max;
  eButton[ch].cnt_condition     = condition;
  eButton[ch].click_time_min    = t_clk;
  eButton[ch].repeat_gap_1st    = r_gap_1st;
  eButton[ch].repeat_gap_other  = r_gap_other;
}


bool buttonGetPressed(uint8_t ch)
{
  bool ret = false;

  if(ch >= BUTTON_CH_MAX) return false;

  if(HAL_GPIO_ReadPin(eButton_tbl[ch].port, eButton_tbl[ch].pin) == eButton_tbl[ch].on_state)
  {
    ret = true;
  }

  return ret;
}


bool buttonObjGetClicked(uint8_t ch)
{
  bool ret = false;

  switch(eButton[ch].state)
  {
    case 0:
      if(buttonGetPressed(ch) == true)
      {//버튼 눌린상태
        if(msWait("GetClicked_0", ch, eButton[ch].click_time_min) == true)
        {//버튼 눌린 상태로 20ms 이상 지나면
          eButton[ch].state = 1;
          eButton[ch].pressing_cnt = 0;
          msWait_Delete("GetClicked_0", ch);

          if(eButton[ch].cnt_condition == ATTACHED)
          {
            ret = true;
          }
        }
      }
      else
      {
        msWait_Delete("GetClicked_0", ch);
      }
      break;

    case 1:
      if(buttonGetPressed(ch) == true)
      {
        if(msWait("GetClicked_1", ch, eButton[ch].repeat_gap_1st) == true)
        {//버튼 눌린 상태로 repeat_gap_1st 이상 지나면
          eButton[ch].state = 2;
          eButton[ch].pressing_cnt++; //"버튼 누르고있음" 1 증가
          msWait_Delete("GetClicked_1", ch);
          ret = true;
        }
      }
      else
      {
        //repeat_gap_1st 지나기 전에 버튼 떼면
        eButton[ch].state = 0;
        msWait_Delete("GetClicked_1", ch);

        if(eButton[ch].cnt_condition == DETACHED)
        {
          ret = true;
        }
      }

      break;

    case 2:
      if(buttonGetPressed(ch) == true)
      {
        if(msWait("GetClicked_2", ch, eButton[ch].repeat_gap_other) == true)
        {//버튼 눌린 상태로 eButton[ch].repeat_gap_other 이상 지나면
          eButton[ch].pressing_cnt++; //"버튼 누르고있음" 1 증가
          msWait_Delete("GetClicked_2", ch);
          ret = true;
        }
      }
      else//repeat_gap_other 지나기 전에 버튼 떼면
      {
        eButton[ch].state = 0;
        msWait_Delete("GetClicked_2", ch);
      }

      break;
  }

  return ret;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == M_GPIO_KEY_1_Pin)
  {
    if(buttonGetPressed(_DEF_BUTTON1) == true)
      buttonObjGetClicked(_DEF_BUTTON1);
  }
  else if(GPIO_Pin == M_GPIO_KEY_2_Pin)
  {
    if(buttonGetPressed(_DEF_BUTTON2) == true)
      buttonObjGetClicked(_DEF_BUTTON2);
  }
}


#ifdef _USE_HW_CLI
void cliButton(cli_args_t *args)
{
  bool ret = false;

  if(args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for(int i=0; i<BUTTON_CH_MAX; i++)
      {
        cliPrintf("%d", buttonGetPressed(i));
      }
      cliPrintf("\n");
      delay(100);
    }
    ret = true;
  }

  if(ret != true)
  {
    cliPrintf("\n");
    cliPrintf("---------[ Button Commands ]----------\n");
    cliPrintf(">> button show\n");
    cliPrintf("--------------------------------------\n");
  }
}
#endif //#ifdef _USE_HW_CLI


#endif //#ifdef _USE_HW_BUTTON
