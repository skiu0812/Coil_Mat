/*
 * test_mid.h
 *
 *  Created on: 2022. 9. 11.
 *      Author: KSK
 */

#ifndef INC_TEST_MID_H_
#define INC_TEST_MID_H_


#include "hw_def.h"

void testUART1nUART2SendFile(void);
void testUART2nUART3SendFile(void);
void testUART1nUARTxSendFile(uint8_t ch);
void testFlashReadWriteErase(void);

#endif /* INC_TEST_MID_H_ */
