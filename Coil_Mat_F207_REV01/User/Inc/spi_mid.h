/*
 * spi_mid.h
 *
 *  Created on: Sep 19, 2022
 *      Author: KSK
 */

#ifndef INC_SPI_MID_H_
#define INC_SPI_MID_H_


#include "hw_def.h"

#ifdef _USE_HW_SPI

#define SPI_CH_MAX          HW_SPI_CH_MAX


#define SPI_MODE0           0
#define SPI_MODE1           1
#define SPI_MODE2           2
#define SPI_MODE3           3


bool spiInit(void);
bool spiBegin(uint8_t ch);
void spiSetDataMode(uint8_t ch, uint8_t dataMode);
void spiSetBitWidth(uint8_t ch, uint8_t bit_width);

uint8_t  spiTransfer8(uint8_t ch, uint8_t data);
uint16_t spiTransfer16(uint8_t ch, uint16_t data);

void spiDmaTxStart(uint8_t ch, uint8_t *p_buf, uint16_t length);
void spiDmaTxTransfer(uint8_t ch, void *buf, uint16_t length, uint32_t timeout) ;
bool spiDmaTxIsDone(uint8_t ch);
void spiAttachTxInterrupt(uint8_t ch, void (*func)());


#endif


#endif /* INC_SPI_MID_H_ */
