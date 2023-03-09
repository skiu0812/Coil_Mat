/*
 * adc_mid.c
 *
 *  Created on: Nov 28, 2022
 *      Author: skiu0
 */


#include "user.h"


#ifdef _USE_HW_ADC1

#define ADC_RESOLUTION  (float)(4096)
#define VREF            (float)(3.3) //(V)

#define R1              (float)(100000)               //100k[ohm]
#define R2              (float)(5360)                 //5.36k[ohm]
#define V_LSB           (float)(VREF/ADC_RESOLUTION)  //(3.3/4096)
#define R_DIV_RATE      (float)(R2/(R1+R2))           //(5360/(100000+5360))

#define I_GAIN          (float)(200)                  //200
#define R_SHUNT         (float)(0.002)                //2m[ohm]
#define VR              (float)(VREF/I_GAIN)          //(3.3/20) = 0.165
#define I_MAX           (float)(VR/R_SHUNT)           //(0.165/0.02) = 8.25(A)
#define I_LSB           (float)(I_MAX/ADC_RESOLUTION) //(8.25/4096) = 0.00201416


uint32_t          eADC1_RawData[HW_ADC1_CH_MAX];
adcbuffer_t       eAdcBufNode[HW_ADC1_CH_MAX];
ADC_Data_TypeDef  eADC_Data;

static uint16_t   sAdcBuffer[HW_ADC1_CH_MAX][HW_ADC1_BUF_SIZE];

bool adcInit(void)
{
  bool ret = true;

  adcStartDma();

  //create buffer
  adcBufferCreate(&eAdcBufNode[ADC_V_IN],  &sAdcBuffer[ADC_V_IN][0],  HW_ADC1_BUF_SIZE);
  adcBufferCreate(&eAdcBufNode[ADC_V_OUT], &sAdcBuffer[ADC_V_OUT][0], HW_ADC1_BUF_SIZE);
  adcBufferCreate(&eAdcBufNode[ADC_I_IN],  &sAdcBuffer[ADC_I_IN][0],  HW_ADC1_BUF_SIZE);

  return ret;
}

void adcStartDma(void)
{
  HAL_ADC_Start_DMA(&hadc1, eADC1_RawData, HW_ADC1_CH_MAX);
}

void adc1DataGet(void)
{
//  static uint16_t iInBuffer[5] = {0};
//  static uint8_t  iInCnt = 0;
//  static uint8_t  pBuffer = 0;
//  uint16_t sum = 0;
//  uint16_t average = 0;
//
//  iInBuffer[pBuffer] = eAdcBufNode[ADC_I_IN].average;
//
//  for(int i=0; i<iInCnt; i++)
//  {
//    sum += iInBuffer[i];
//  }
//
//  average = (uint32_t)(sum / (iInCnt + 1));
//
//  pBuffer = (pBuffer + 1) % 5;
//  if(iInCnt < 5) iInCnt++;

  if(eADC_Data.ADC1_ConvEndFlag)
  {
    eADC_Data.Vin   = adcCalcVolt(eAdcBufNode[ADC_V_IN].average); //PA4 - ADC1_IN4
    eADC_Data.Vout  = adcCalcVolt(eAdcBufNode[ADC_V_OUT].average); //PA1 - ADC1_IN1
    eADC_Data.Iin   = adcCalcCurr(eAdcBufNode[ADC_I_IN].average); //PA3 - ADC1_IN3

    eADC_Data.ADC1_ConvEndFlag = 0;
  }
}


uint16_t adcCalcVolt(uint32_t Vadc)
{
  uint16_t V;

  V =  (uint16_t)((Vadc * V_LSB * (1/R_DIV_RATE)) * 100);

  return V;
}

uint16_t adcCalcCurr(uint32_t Iadc)
{
  uint16_t I;

//  I =  (uint16_t)(((Iadc * I_LSB) * 100)*0.89);

  float volt, i_temp;
  volt    = Iadc / ADC_RESOLUTION * VREF; // ADC 값을 전압으로 변환
  i_temp  = volt / (R_SHUNT * 176);
  I       = (uint16_t)(i_temp * 100); // 전압을 전류로 변환

  return I;
}


bool adcBufferCreate(adcbuffer_t *p_node, uint16_t *p_buf, uint16_t length)
{
  bool ret = true;

  p_node->front       = 0;
  p_node->available   = 0;
  p_node->average     = 0;
  p_node->length      = HW_ADC1_BUF_SIZE;

  p_node->p_buf       = p_buf;
  p_node->offset      = HW_ADC1_OFFSET;
  p_node->buf_full    = false;

  return ret;
}

bool adcBuffetWrite(adcbuffer_t *p_node, uint16_t data)
{
  bool      ret = true;
  uint16_t  next_in;
  uint16_t  sum = 0;

  if(data > p_node->offset)
  {
    next_in = (p_node->rear + 1) % p_node->length;

    if(next_in == p_node->front)
    {
      p_node->front = (p_node->front + 1) % p_node->length;
    }
    p_node->p_buf[p_node->rear] = data;
    p_node->rear = next_in;

    if(p_node->front == p_node->rear)
      p_node->available = 0;
    else
      p_node->available = ((p_node->length + p_node->rear) - p_node->front) % p_node->length;
  }
  else
  {
//    p_node->front     = 0;
//    p_node->rear      = 0;
//    p_node->average   = 0;
//    p_node->available = 0;
  }

  if(p_node->available > 0)
  {
    for(int i=0; i<p_node->available; i++)
    {
      sum += p_node->p_buf[(p_node->front + i) % p_node->length];
    }
    p_node->average = (uint16_t)(sum / p_node->available);
  }

  return ret;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if (hadc->Instance == ADC1)
  {
    eADC_Data.ADC1_ConvEndFlag = 1;

    adcBuffetWrite(&eAdcBufNode[ADC_V_IN],   eADC1_RawData[0]);
    adcBuffetWrite(&eAdcBufNode[ADC_V_OUT],  eADC1_RawData[1]);
    adcBuffetWrite(&eAdcBufNode[ADC_I_IN],   eADC1_RawData[2]);
  }
//  ledCtrl(LED_POWER, LED_TOGGLE);

}

#endif //#ifdef _USE_HW_ADC
