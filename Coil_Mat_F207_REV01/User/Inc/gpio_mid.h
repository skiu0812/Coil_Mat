/*
 * gpio_mid.h
 *
 *  Created on: Sep 1, 2022
 *      Author: KSK
 */

#ifndef INC_GPIO_MID_H_
#define INC_GPIO_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_GPIO


#define GPIO_CH_MAX     HW_GPIO_CH_MAX

typedef enum
{
  GPIO_OFF = 0,
  GPIO_ON,
}gpio_state_e;


bool gpioInit(void);
bool gpioPortRcc(uint8_t ch, GPIO_TypeDef *port); // skkim 220915
bool gpioPinMode(uint8_t ch, uint8_t mode);
void gpioPinWrite(uint8_t ch, bool value);
bool gpioPinRead(uint8_t ch);
void gpioPinToggle(uint8_t ch);


#endif


#endif /* INC_GPIO_MID_H_ */
