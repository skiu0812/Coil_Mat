/*
 * led_mid.h
 *
 *  Created on: 2022. 9. 14.
 *      Author: KSK
 */

#ifndef INC_LED_MID_H_
#define INC_LED_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_LED


#define LED_CH_MAX      HW_LED_CH_MAX


typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;

typedef struct
{
  bool      curr_state;
  uint8_t   set_state;
  uint32_t  delay;
}led_state_t;

typedef enum
{
  LED_OFF = 0,
  LED_ON,
  LED_TOGGLE,
}led_state_e;


bool ledInit(void);
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);
void ledCtrl(uint8_t ch, uint8_t state);


#endif


#endif /* INC_LED_MID_H_ */
