/*
 * qbuffer.h
 *
 *  Created on: Sep 5, 2022
 *      Author: KSK
 */


#ifndef COMMON_INC_QBUFFER_H_
#define COMMON_INC_QBUFFER_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "def.h"


typedef struct
{
  uint32_t in;
  uint32_t out;
  uint32_t len;

  uint8_t *p_buf;
} qbuffer_t;


void      qbufferInit(void);
bool      qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length);
bool      qbufferPush(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
bool      qbufferPop(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
uint32_t  qbufferAvailable(qbuffer_t *p_node);
void      qbufferFlush(qbuffer_t *p_node);


#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_QBUFFER_H_ */
