/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M_ADC_VOUT_Pin GPIO_PIN_0
#define M_ADC_VOUT_GPIO_Port GPIOC
#define M_ADC_IIN_Pin GPIO_PIN_1
#define M_ADC_IIN_GPIO_Port GPIOC
#define M_ADC_RESERVED_1_Pin GPIO_PIN_3
#define M_ADC_RESERVED_1_GPIO_Port GPIOA
#define M_ADC_VIN_Pin GPIO_PIN_4
#define M_ADC_VIN_GPIO_Port GPIOC
#define M_GPIO_LED_3_Pin GPIO_PIN_2
#define M_GPIO_LED_3_GPIO_Port GPIOB
#define M_FSMC_LCD_DB4_Pin GPIO_PIN_7
#define M_FSMC_LCD_DB4_GPIO_Port GPIOE
#define M_FSMC_LCD_DB5_Pin GPIO_PIN_8
#define M_FSMC_LCD_DB5_GPIO_Port GPIOE
#define M_FSMC_LCD_DB6_Pin GPIO_PIN_9
#define M_FSMC_LCD_DB6_GPIO_Port GPIOE
#define M_FSMC_LCD_DB7_Pin GPIO_PIN_10
#define M_FSMC_LCD_DB7_GPIO_Port GPIOE
#define M_FSMC_LCD_DB8_Pin GPIO_PIN_11
#define M_FSMC_LCD_DB8_GPIO_Port GPIOE
#define M_FSMC_LCD_DB9_Pin GPIO_PIN_12
#define M_FSMC_LCD_DB9_GPIO_Port GPIOE
#define M_FSMC_LCD_DB10_Pin GPIO_PIN_13
#define M_FSMC_LCD_DB10_GPIO_Port GPIOE
#define M_FSMC_LCD_DB11_Pin GPIO_PIN_14
#define M_FSMC_LCD_DB11_GPIO_Port GPIOE
#define M_FSMC_LCD_DB12_Pin GPIO_PIN_15
#define M_FSMC_LCD_DB12_GPIO_Port GPIOE
#define M_GPIO_LCD_BL_Pin GPIO_PIN_13
#define M_GPIO_LCD_BL_GPIO_Port GPIOB
#define M_FSMC_LCD_DB13_Pin GPIO_PIN_8
#define M_FSMC_LCD_DB13_GPIO_Port GPIOD
#define M_FSMC_LCD_DB14_Pin GPIO_PIN_9
#define M_FSMC_LCD_DB14_GPIO_Port GPIOD
#define M_FSMC_LCD_DB15_Pin GPIO_PIN_10
#define M_FSMC_LCD_DB15_GPIO_Port GPIOD
#define M_FSMC_LCD_RS_Pin GPIO_PIN_11
#define M_FSMC_LCD_RS_GPIO_Port GPIOD
#define M_GPIO_LCD_NRESET_Pin GPIO_PIN_13
#define M_GPIO_LCD_NRESET_GPIO_Port GPIOD
#define M_FSMC_LCD_DB0_Pin GPIO_PIN_14
#define M_FSMC_LCD_DB0_GPIO_Port GPIOD
#define M_FSMC_LCD_DB1_Pin GPIO_PIN_15
#define M_FSMC_LCD_DB1_GPIO_Port GPIOD
#define M_USB_DM_Pin GPIO_PIN_11
#define M_USB_DM_GPIO_Port GPIOA
#define M_USB_DP_Pin GPIO_PIN_12
#define M_USB_DP_GPIO_Port GPIOA
#define M_SWDIO_Pin GPIO_PIN_13
#define M_SWDIO_GPIO_Port GPIOA
#define M_SWCLK_Pin GPIO_PIN_14
#define M_SWCLK_GPIO_Port GPIOA
#define M_GPIO_KEY_2_Pin GPIO_PIN_15
#define M_GPIO_KEY_2_GPIO_Port GPIOA
#define M_GPIO_KEY_2_EXTI_IRQn EXTI15_10_IRQn
#define M_UART_DBG_TX_Pin GPIO_PIN_10
#define M_UART_DBG_TX_GPIO_Port GPIOC
#define M_UART_DBG_RX_Pin GPIO_PIN_11
#define M_UART_DBG_RX_GPIO_Port GPIOC
#define M_GPIO_LED_2_Pin GPIO_PIN_12
#define M_GPIO_LED_2_GPIO_Port GPIOC
#define M_FSMC_LCD_DB2_Pin GPIO_PIN_0
#define M_FSMC_LCD_DB2_GPIO_Port GPIOD
#define M_FSMC_LCD_DB3_Pin GPIO_PIN_1
#define M_FSMC_LCD_DB3_GPIO_Port GPIOD
#define M_FSMC_LCD_NRD_Pin GPIO_PIN_4
#define M_FSMC_LCD_NRD_GPIO_Port GPIOD
#define M_FSMC_LCD_NWR_Pin GPIO_PIN_5
#define M_FSMC_LCD_NWR_GPIO_Port GPIOD
#define M_FSMC_LCD_NCS_Pin GPIO_PIN_7
#define M_FSMC_LCD_NCS_GPIO_Port GPIOD
#define M_TIM_MOSFET_CTRL_Pin GPIO_PIN_5
#define M_TIM_MOSFET_CTRL_GPIO_Port GPIOB
#define M_GPIO_LED_1_Pin GPIO_PIN_8
#define M_GPIO_LED_1_GPIO_Port GPIOB
#define M_GPIO_KEY_1_Pin GPIO_PIN_9
#define M_GPIO_KEY_1_GPIO_Port GPIOB
#define M_GPIO_KEY_1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
