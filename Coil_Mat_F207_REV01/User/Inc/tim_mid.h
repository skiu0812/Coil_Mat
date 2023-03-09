/*
 * tim_mid.h
 *
 *  Created on: Aug 16, 2022
 *      Author: KSK
 */

#ifndef INC_TIM_MID_H_
#define INC_TIM_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_TIM


#define __HAL_TIM_GET_PRESCALER(__HANDLE__)       ((__HANDLE__)->Instance->PSC)


typedef struct
{
  uint32_t  ch;
  float     duty_rate;
  uint32_t  duty;
  uint32_t  dead_time;
}tim_ch_t;

typedef struct
{
  TIM_HandleTypeDef   *htim;

  uint32_t            clock;
  float               freq;
  uint32_t            prescaler;
  uint32_t            period;

  tim_ch_t            ch[4];
}tim_t;

typedef struct
{
  int32_t   cnt;
  uint32_t  period;
  uint32_t  cntmax;
  bool      busy;
  bool      is_set;
}task_cnt_t;


extern tim_t eTim[HW_TIM_CH_MAX];

extern task_cnt_t eTmr_Task_ADC;
extern task_cnt_t eTmr_Task_UART;
extern task_cnt_t eTmr_Task_USB;
extern task_cnt_t eTmr_Task_Button;
extern task_cnt_t eTmr_Task_LED;
extern task_cnt_t eTmr_Task_WDT;
extern task_cnt_t eTmr_Task_CLI;
extern task_cnt_t eTmr_Task_TIM_PWM;
extern task_cnt_t eTmr_Task_Coil;
extern task_cnt_t eTmr_Task_LCD;


bool  timInit(void);
bool  timOpen(void);
bool  timStart(void);
bool  timSet(uint8_t tim, TIM_HandleTypeDef *htim, uint32_t clock, float freq);
bool  timPWMSet(uint8_t tim, uint32_t ch, float dr);
float timPWMGet(uint8_t tim, uint32_t ch);
bool  timTask_Run(task_cnt_t *taskTmr, uint32_t taskPeriod);
void  timTask_Set_CntMax(task_cnt_t *taskTmr, uint32_t taskPeriod);


#endif


#endif /* INC_TIM_MID_H_ */
