/*
 * hw_def.h
 *
 *  Created on: Sep 1, 2022
 *      Author: KSK
 */


#ifndef INC_HW_DEF_H_
#define INC_HW_DEF_H_


#include "def.h"

#define HEATING_MAT                   0
#define MAGNETIC_FIELD                1
#define ALL_COIL                      2
#define _USE_HW_BOARD                 MAGNETIC_FIELD

#define _USE_HW_USB
#define _USE_HW_CDC

#define _USE_HW_FLASH
#define _USE_SW_EEPROM
#define      SW_EEPROM_PAGE_SIZE      (uint32_t)0x20000 // Page size = 128KByte //-> eeprom.h
#define      SW_EEPROM_VOLT_RANGE     (uint8_t)VOLTAGE_RANGE_3 //2.7V to 3.6V //-> eeprom.h
#define      SW_EEPROM_START_ADDR     ((uint32_t)0x080A0000) //FLASH_SECTOR_9 address //-> eeprom.h
#define      SW_EEPROM_PAGE0_ID       FLASH_SECTOR_9 //-> eeprom.h
#define      SW_EEPROM_PAGE1_ID       FLASH_SECTOR_10 //-> eeprom.h
#define      SW_EEPROM_VAR_BIT_SIZE   8 //bit

#define _USE_HW_COIL
#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
#define      HW_COIL_MAT_PWM_DUTY         60
#define      HW_COIL_MAT_CURR_MIN         302
#define      HW_COIL_MAT_UNIT_NUM         1
#define      HW_COIL_MAT_CURR_GAP         1
#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
#define      HW_COIL_SOLENOID_PWM_DUTY    40
//#define      HW_COIL_SOLENOID_CURR_MIN    91 //40도
//#define      HW_COIL_SOLENOID_CURR_MIN    88 //50도
//#define      HW_COIL_SOLENOID_CURR_MIN    86 //60도
#define      HW_COIL_SOLENOID_CURR_MIN    125
#define      HW_COIL_SOLENOID_UNIT_NUM    2
#define      HW_COIL_SOLENOID_CURR_GAP    1
#define      HW_COIL_SOLENOID_UNIT_TIME   1000*60*60  //seconds
#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)

#define _USE_HW_TIM
#define      HW_TIM_APB1_CLOCK            60000000  //60MHz
#define      HW_TIM_APB2_CLOCK            120000000 //120MHz
#define      HW_TIM_TASK_FREQ             10000 //10kHz
#define      HW_TIM_COIL_FREQ             7.8 //7.8Hz
#define      HW_TIM_CH_MAX                3
#define      HW_TIM_PERIOD                1000
#define      HW_TIM_PERIOD_MAX            2000

#define _USE_HW_PWM

#define _USE_HW_ADC1
#define      HW_ADC1_CH_MAX               3
#define      HW_ADC1_BUF_SIZE             5
#define      HW_ADC1_OFFSET               100

#define _USE_HW_CLI
#define      HW_CLI_CMD_NAME_MAX          16
#define      HW_CLI_CMD_LIST_MAX          16
#define      HW_CLI_LINE_HIS_MAX          16
#define      HW_CLI_LINE_BUF_MAX          64

#define _USE_HW_GPIO
#define      HW_GPIO_CH_MAX               2

#define _USE_HW_BUTTON
#define      HW_BUTTON_CH_MAX             2
#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
#define      HW_BUTTON1_CNT_MAX           10
#endif //#if (_USE_HW_BOARD == HEATING_MAT || _USE_HW_BOARD == ALL_COIL)
#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
#define      HW_BUTTON1_CNT_MAX           2
#endif //#if (_USE_HW_BOARD == MAGNETIC_FIELD || _USE_HW_BOARD == ALL_COIL)
#define      HW_BUTTON2_CNT_MAX           6

#define _USE_HW_LED
#define      HW_LED_CH_MAX                3

#define _USE_HW_UART
#define      HW_UART_CH_MAX               2
#define      HW_UART_BUF_SIZE             512

//#define _USE_HW_SPI
#define      HW_SPI_CH_MAX                1

#define _USE_HW_ST7735
//#define _USE_HW_ILI9341

#define _USE_HW_LCD
#define      HW_LCD_FSMC                  1
#define      HW_LCD_SPI                   0
#define      HW_LCD_WIDTH                 128
#define      HW_LCD_HEIGHT                160
#define      HW_LCD_ROTAT_MODE            4 //if(st7735){3} else if(ILI9341){1}
#define      HW_LCD_COL_OFFSET            0 //if(1.8"||2.4"){col_offset=0}else if(0.96"){col_offset=1}
#define      HW_LCD_ROW_OFFSET            0 //if(1.8"||2.4"){row_offset=0}else if(0.96"){row_offset=26}
#define      HW_LCD_REG                   ((uint32_t) 0x60000000)
#define      HW_LCD_RAM                   ((uint32_t) 0x60020000)//RS:A16->RAM Address: 0b0000 0000 0000 0001 0000 0000 0000 0000 << 1
//                                                                                = 0b0000 0000 0000 0010 0000 0000 0000 0000 = 0x20000

//#define _USE_HW_AT
#define      HW_AT_CH_MAX                 2
#define      HW_AT_BUF_MAX                24  // AT 수신 버퍼 라인 수
#define      HW_AT_BUF_LENGTH             256 // AT 수신 버퍼 라인 길이
#define      HW_AT_CMD_MAX                16
#define      HW_AT_CMD_LENGTH             32
#define      HW_AT_ARGS_MAX               16  // AT Command의 매개변수 수
#define      HW_AT_ARGS_LENGTH            64  // AT Command의 매개변수 길이
#define      HW_AT_RETRY_NUM              5


#endif /* INC_HW_DEF_H_ */
