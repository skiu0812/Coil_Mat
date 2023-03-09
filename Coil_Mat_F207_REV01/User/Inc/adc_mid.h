/*
 * adc_mid.h
 *
 *  Created on: Nov 28, 2022
 *      Author: skiu0
 */

#ifndef INC_ADC_MID_H_
#define INC_ADC_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_ADC1


typedef struct
{
  uint8_t ADC1_ConvEndFlag;

  uint32_t Vin;
  uint32_t Vout;
  uint32_t Iin;

}ADC_Data_TypeDef;



typedef struct
{
  uint16_t  front;
  uint16_t  rear;
  uint16_t  sum;
  uint16_t  average;
  uint16_t  available;
  uint16_t  length;

  uint16_t  *p_buf;
  uint16_t  offset;
  bool      buf_full;

} adcbuffer_t;

extern uint32_t         eADC1_RawData[HW_ADC1_CH_MAX];
extern adcbuffer_t      eAdcBufNode[HW_ADC1_CH_MAX];
extern ADC_Data_TypeDef eADC_Data;

bool adcInit(void);
void adcStartDma(void);
void adc1DataGet(void);
//void adc2DataGet(void);
uint16_t adcCalcVolt(uint32_t Vadc);
uint16_t adcCalcCurr(uint32_t Iadc);
uint32_t adcAverage(adcbuffer_t *p_node);
bool adcBufferCreate(adcbuffer_t *p_node, uint16_t *p_buf, uint16_t length);
bool adcBuffetWrite(adcbuffer_t *p_node, uint16_t data);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

#endif //#ifdef _USE_HW_ADC


#endif /* INC_ADC_MID_H_ */
