/*
 * button_mid.h
 *
 *  Created on: Sep 15, 2022
 *      Author: KSK
 */

#ifndef INC_BUTTON_MID_H_
#define INC_BUTTON_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_BUTTON


#define BUTTON_CH_MAX     HW_BUTTON_CH_MAX


typedef struct
{
  GPIO_TypeDef    *port;
  uint32_t        pin;
  GPIO_PinState   on_state;
} button_tbl_t;

typedef struct
{
  uint8_t   state;
  uint32_t  pressing_time;
  uint32_t  pressing_cnt;
  uint32_t  cnt;
  uint32_t  cnt_max;
  bool      click_new; //새로 눌림
  bool      cnt_condition; //버튼 카운트 증가 조건
  uint32_t  click_time_min;
  uint32_t  repeat_gap_1st;
  uint32_t  repeat_gap_other;
}button_obj_t;

typedef enum
{
  ATTACHED = 0, //버튼을 눌렀을(attached) 때 카운트 증가
  DETACHED,    //버튼을 뗐을(detached) 때 카운트 증가
}button_cnt_condition_e;


extern button_tbl_t eButton_tbl[BUTTON_CH_MAX];
extern button_obj_t eButton[BUTTON_CH_MAX];


bool buttonInit(void);
bool buttonGetPressed(uint8_t ch);

void buttonObjCreate(uint8_t ch, bool condition, uint32_t cnt_max, uint32_t t_clk, uint32_t r_gap_1st, uint32_t r_gap_other);
bool buttonObjGetClicked(uint8_t ch);


#endif

#endif /* INC_BUTTON_MID_H_ */
