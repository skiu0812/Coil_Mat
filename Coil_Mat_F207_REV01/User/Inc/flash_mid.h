/*
 * flsah_mid.h
 *
 *  Created on: 2022. 9. 9.
 *      Author: KSK
 */

#ifndef INC_FLASH_MID_H_
#define INC_FLASH_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_FLASH


typedef struct
{
  uint32_t addr;
  uint32_t length;
} flash_tbl_t;


bool flashInit(void);
bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length);


#endif


#endif /* INC_FLASH_MID_H_ */
