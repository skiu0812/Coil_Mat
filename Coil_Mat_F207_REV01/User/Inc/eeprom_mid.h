/*
 * eeprom_mid.h
 *
 *  Created on: 2023. 2. 28.
 *      Author: KSK
 */

#ifndef INC_EEPROM_MID_H_
#define INC_EEPROM_MID_H_


#include "hw_def.h"


#ifdef _USE_SW_EEPROM


typedef enum
{
  CHAR,
  INT8_T,
  UINT8_T,
  INT16_T,
  UINT16_T,
  INT32_T,
  UINT32_T,
  FLOAT,
}eeprom_type_e;

typedef enum
{
  t_char = 0,
  t_int8_t,
  t_uint8_t,
  t_int16_t,
  t_uint16_t,
  t_int32_t,
  t_uint32_t,
  t_float,

  var_num, //number of var
}eeprom_var_e;


bool      eepromInit(void);
bool      eepromCreatFirstVariables(void);
bool      eepromWrite(uint8_t dataType, uint16_t var, uint8_t arrNum, ...);
bool      eepromRead(uint16_t var, ...);
uint16_t  eepromGetArrSize(uint16_t var);
uint16_t  eepromGetType(uint16_t var);


#endif //#ifdef _USE_SW_EEPROM


#endif /* INC_EEPROM_MID_H_ */
