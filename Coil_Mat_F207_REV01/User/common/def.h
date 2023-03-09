/*
 * sw_def.h
 *
 *  Created on: 2022. 9. 4.
 *      Author: KSK
 */


#ifndef INC_SW_DEF_H_
#define INC_SW_DEF_H_


// C Header
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


#define _DEF_LED1                       0
#define _DEF_LED2                       1
#define _DEF_LED3                       2
#define _DEF_LED4                       3
#define      LED_POWER                  _DEF_LED1
#define      LED_TIME                   _DEF_LED2
#define      LED_COIL                   _DEF_LED3


#define _DEF_LCD1                       0
#define _DEF_LCD2                       1
#define _DEF_LCD3                       2
#define _DEF_LCD4                       3
#define      LCD_ST7735                 _DEF_LCD1
#define      LCD_ILI9341                _DEF_LCD2


#define _DEF_BUTTON1                    0
#define _DEF_BUTTON2                    1
#define _DEF_BUTTON3                    2
#define _DEF_BUTTON4                    3
#define      BUTTON_COIL                _DEF_BUTTON1
#define      BUTTON_TIMER               _DEF_BUTTON2


#define _DEF_GPIO1                      0
#define _DEF_GPIO2                      1
#define _DEF_GPIO3                      2
#define _DEF_GPIO4                      3
#define _DEF_GPIO5                      4
#define _DEF_GPIO6                      5
#define _DEF_GPIO7                      6
#define _DEF_GPIO8                      7
#define _DEF_GPIO9                      8
#define _DEF_GPIO10                     9
#define      GPIO_LCD_NRESET            _DEF_GPIO1
#define      GPIO_LCD_BL                _DEF_GPIO2
#define      GPIO_LCD_CS                _DEF_GPIO3
#define      GPIO_LCD_DC                _DEF_GPIO4


#define _DEF_ADC1                       0
#define _DEF_ADC2                       1
#define _DEF_ADC3                       2
#define _DEF_ADC4                       3
#define      ADC_V_IN                   _DEF_ADC1
#define      ADC_V_OUT                  _DEF_ADC2
#define      ADC_I_IN                   _DEF_ADC3


#define _DEF_UART1                      0 //USB CDC
#define _DEF_UART2                      1 //usart2 - Debug
#define _DEF_UART3                      2 //usart3
#define _DEF_UART4                      3
#define      UART_CLI                   _DEF_UART1
#define      UART_DEBUG                 _DEF_UART2


#define _DEF_TIM1                       0
#define _DEF_TIM2                       1
#define _DEF_TIM3                       2
#define _DEF_TIM4                       3
#define      TIM_TASK                   _DEF_TIM1
#define      TIM_ADC                    _DEF_TIM2
#define      TIM_COIL                   _DEF_TIM3


#define _DEF_TIM_CH1                    0
#define _DEF_TIM_CH2                    1
#define _DEF_TIM_CH3                    2
#define _DEF_TIM_CH4                    3


#define _DEF_AT1                        0
#define _DEF_AT2                        1
#define _DEF_AT3                        2
#define _DEF_AT4                        3


#define _DEF_SPI1                       0
#define _DEF_SPI2                       1
#define _DEF_SPI3                       2
#define _DEF_SPI4                       3
#define      SPI_LCD                    _DEF_SPI1


#define _DEF_I2C1                       0
#define _DEF_I2C2                       1
#define _DEF_I2C3                       2
#define _DEF_I2C4                       3


#define _DEF_INPUT                      0
#define _DEF_INPUT_PULLUP               1
#define _DEF_INPUT_PULLDOWN             2
#define _DEF_OUTPUT                     3
#define _DEF_OUTPUT_PULLUP              4
#define _DEF_OUTPUT_PULLDOWN            5
#define _DEF_OUTPUT_OD                  6
#define _DEF_OUTPUT_OD_PULLUP           7


#define constrain(amt,low,high)         ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


#ifndef max
#define max(a,b)                        (((a) > (b)) ? (a) : (b))
#define min(a,b)                        (((a) < (b)) ? (a) : (b))
#endif


#ifndef map
#define map(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#endif /* INC_SW_DEF_H_ */
