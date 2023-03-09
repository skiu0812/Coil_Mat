/*
 * main_mid.c
 *
 *  Created on: Aug 16, 2022
 *      Author: KSK
 */


#include "user.h"


#define WAIT_LIST_MAX     128
#define WATI_TIME_MAX     600000 //(10min)
#define WAIT_TIME_OUT     2000
#define TIME_OUT_CNT_MAX  3


ms_wait_t eWaitList[WAIT_LIST_MAX];


void delay(uint32_t ms)
{
  HAL_Delay(ms);
}


uint32_t millis(void)
{
  return HAL_GetTick();
}


uint8_t msWait_ExistListScan(char *name)
{
  uint8_t ret = WAIT_LIST_MAX;

  for(int i=0; i<WAIT_LIST_MAX; i++)
  {
    if(strcmp(eWaitList[i].name, name) == 0)
    {
      ret = i;
    }
  }

  return ret;
}


uint8_t msWait_EmptyListScan(void)
{
  uint8_t ret = WAIT_LIST_MAX;
  bool    find_empty = false;

  for(int i=0; i<WAIT_LIST_MAX; i++)
  {
    if(strlen(eWaitList[i].name) == 0)
    {
      if(find_empty == false)
      {
        ret = i;
        find_empty = true;
      }
    }
  }

  return ret;
}

/*
 * name     : wait이름 - 문자열 1~99개
 * ch       : 채널 번호(채널 없으면 0)
 * wait_ms  : wait 시간(ms)
 */
bool msWait(char *name, int ch, uint32_t wait_ms)
{
  bool ret = false;
  bool exist = 0;
  bool empty = 0;
  uint8_t idx = 0;

  char c_name[130];
  char c_ch[30];

  strncpy(c_name, name, 100);
  sprintf(c_ch, "%d", ch);

  strcat(c_name, c_ch);

  idx = msWait_ExistListScan(c_name);

  if(idx < WAIT_LIST_MAX) exist = true;
  else if(idx == WAIT_LIST_MAX) exist = false;

  if(exist == true)
  {
    if(millis() - eWaitList[idx].start_time > wait_ms)
    {
      strcpy(eWaitList[idx].name, "");
      eWaitList[idx].start_time = 0;
      eWaitList[idx].wait_time = 0;
      eWaitList[idx].end_cnt = 0;
      eWaitList[idx].active = 0;

      ret = true;
    }
  }
  else if(exist == false)
  {
    idx = msWait_EmptyListScan();

    if(idx < WAIT_LIST_MAX) empty = true;
    else if(idx == WAIT_LIST_MAX) empty = false;

    if(empty == true)
    {
      strcpy(eWaitList[idx].name, c_name);
      eWaitList[idx].start_time = millis();
      eWaitList[idx].wait_time = wait_ms;
      eWaitList[idx].end_cnt = 0;
      eWaitList[idx].active = 1;

      ret = false;
    }
    else if(empty == false)
    {
      uartPrintfNotice(UART_CLI, error, "msWait(\"%s\", %d) -> eWaitList[] overflow!!\n", name, wait_ms);
      ret = false;
    }
  }

  return ret;
}


void msWait_Delete(char *name, int ch)
{
  bool exist = 0;
  uint8_t idx = 0;

  char c_name[130];
  char c_ch[30];

  strncpy(c_name, name, 100);
  sprintf(c_ch, "%d", ch);

  strcat(c_name, c_ch);

  idx = msWait_ExistListScan(c_name);

  if(idx < WAIT_LIST_MAX) exist = true;
  else if(idx == WAIT_LIST_MAX) exist = false;

  if(exist == true)
  {
    strcpy(eWaitList[idx].name, "");
    eWaitList[idx].active = 0;
    eWaitList[idx].start_time = 0;
    eWaitList[idx].wait_time = 0;
    eWaitList[idx].end_cnt = 0;
  }
}


// zombie cleaning
void msWait_EndedListCleaning(void)
{
  uint32_t now = millis();

  for(int i=0; i<WAIT_LIST_MAX; i++)
  {
    if(millis() - eWaitList[i].start_time > WATI_TIME_MAX)
    {
      strcpy(eWaitList[i].name, "");
      eWaitList[i].active = 0;
      eWaitList[i].start_time = 0;
      eWaitList[i].wait_time = 0;
      eWaitList[i].end_cnt = 0;
    }

    if(strlen(eWaitList[i].name) > 0 && eWaitList[i].start_time > 0)
    {
      if(now - eWaitList[i].start_time > (eWaitList[i].wait_time + (WAIT_TIME_OUT * eWaitList[i].end_cnt)))
      {
        if(eWaitList[i].end_cnt > TIME_OUT_CNT_MAX)
        {
         strcpy(eWaitList[i].name, "");
          eWaitList[i].active = 0;
          eWaitList[i].start_time = 0;
          eWaitList[i].wait_time = 0;
          eWaitList[i].end_cnt = 0;
        }
        else
        {
          eWaitList[i].end_cnt++;
        }
      }
    }
  }
}


int __io_putchar(int ch)
{
  uartWrite(UART_CLI, (uint8_t *)&ch, 1);
  return 1;
}


// 공용체(union)를 사용한 부동소수점 변환 (https://docs.microsoft.com/ko-kr/cpp/cpp/data-type-ranges?view=msvc-170)
uint32_t Float2Hex(float flt)
{
  t_float2hex f2h;

    unsigned char result1;
    unsigned short result2;
    unsigned long int result3;
    uint32_t result;

    f2h.storage = flt;     // (float)of.storage
    result1 = f2h.bit.sign;
    result2 = f2h.bit.expt;
    result3 = f2h.bit.mant;

    result = (result1<<31)|(result2<<23)|(result3);

    return result;
}


float Hex2Float(uint32_t hex)
{
  t_hex2float h2f;

  h2f.hex = hex;

  return h2f.flt;
}
