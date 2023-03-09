/*
 * main_mid.h
 *
 *  Created on: Aug 16, 2022
 *      Author: KSK
 */

#ifndef INC_MAIN_MID_H_
#define INC_MAIN_MID_H_


#include "hw_def.h"


#define _USE_LOG_PRINT    1


#if _USE_LOG_PRINT
#define logPrintf(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#else
#define logPrintf(fmt, ...)
#endif


typedef struct
{
  char      name[32];
  bool      active;
  uint32_t  start_time;
  uint32_t  wait_time;
  uint8_t   end_cnt;
}ms_wait_t;

// 공용체(union)를 사용한 float -> hex (https://kirkim.github.io/42seoul/2021/03/20/ft_printf7.html)
typedef union u_float2hex
{
    float      storage;    // 직접 값을 받아오는 변수
    struct
    {
        unsigned long int  mant :23 ;
        unsigned short  expt :8 ;
        unsigned char  sign :1 ;
    } bit;

}t_float2hex;

//공용체(union)를 사용한 hex -> float (https://zingry.tistory.com/3)
typedef union u_hex2float
{
  uint32_t hex;
  float flt;
}t_hex2float;


extern ms_wait_t eWaitList[];


void      delay(uint32_t ms);
uint32_t  millis(void);
uint8_t   msWait_ExistListScan(char *name);
uint8_t   msWait_EmptyListScan(void);
bool      msWait(char *name, int ch, uint32_t wait_ms);
void      msWait_Delete(char *name, int ch);
void      msWait_EndedListCleaning(void);
uint32_t  Float2Hex(float flt);
float     Hex2Float(uint32_t hex);


#endif /* INC_MAIN_MID_H_ */
