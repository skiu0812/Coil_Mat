/*
 * uart_api.c
 *
 *  Created on: Sep 26, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_UART


uint8_t Int1Src;
uint8_t Int2Src;


void Task_UART(uint32_t taskPeriod)
{
  if(timTask_Run(&eTmr_Task_UART, taskPeriod) == true)
  {
    //********************* UART Task Code Begin *********************//

//    //eeprom test
//    char      var_char[5] = "asdf";
//    int8_t    var_int8_t[5] = {10, -20, -30, 40, 50};
//    uint8_t   var_uint8_t[5] = {10, 20, 30, 40, 50};
//    int16_t   var_int16_t[5] = {100, -200, 300, -400, 500};
//    uint16_t  var_uint16_t[5] = {100, 200, 300, 400, 500};
//    int32_t   var_int32_t[5] = {-1000, 2000, -3000, 4000, -5000};
//    uint32_t  var_uint32_t[5] = {1000, 2000, 3000, 4000, 5000};
//    float     var_float[5] = {0.111, 0.222, -0.333, 0.444, 0.555};
//
//    char      var_char_rd[5];
//    int8_t    var_int8_t_rd[5];
//    uint8_t   var_uint8_t_rd[5];
//    int16_t   var_int16_t_rd[5];
//    uint16_t  var_uint16_t_rd[5];
//    int32_t   var_int32_t_rd[5];
//    uint32_t  var_uint32_t_rd[5];
//    float     var_float_rd[5];
//
//    eepromWrite(CHAR, t_char, 5, var_char);
//    eepromWrite(INT8_T, t_int8_t, 5, var_int8_t);
//    eepromWrite(UINT8_T, t_uint8_t, 5, var_uint8_t);
//    eepromWrite(INT16_T,t_int16_t, 5,  var_int16_t);
//    eepromWrite(UINT16_T, t_uint16_t, 5, var_uint16_t);
//    eepromWrite(INT32_T, t_int32_t, 5, var_int32_t);
//    eepromWrite(UINT32_T, t_uint32_t, 5, var_uint32_t);
//    eepromWrite(FLOAT, t_float, 5, var_float);
//
//    eepromRead(t_char, var_char_rd);
//    eepromRead(t_int8_t, var_int8_t_rd);
//    eepromRead(t_uint8_t, var_uint8_t_rd);
//    eepromRead(t_int16_t, var_int16_t_rd);
//    eepromRead(t_uint16_t, var_uint16_t_rd);
//    eepromRead(t_int32_t, var_int32_t_rd);
//    eepromRead(t_uint32_t, var_uint32_t_rd);
//    eepromRead(t_float, var_float_rd);
//
//    uartPrintf(UART_CLI, "char : ");
//    uartPrintf(UART_CLI, "%s", var_char_rd);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "int8_t : ");
//    for(int i=0; i<eepromGetArrSize(t_int8_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_int8_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "uint8_t : ");
//    for(int i=0; i<eepromGetArrSize(t_uint8_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_uint8_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "int16_t : ");
//    for(int i=0; i<eepromGetArrSize(t_int16_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_int16_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "uint16_t : ");
//    for(int i=0; i<eepromGetArrSize(t_uint16_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_uint16_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "int32_t : ");
//    for(int i=0; i<eepromGetArrSize(t_int32_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_int32_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "uint32_t : ");
//    for(int i=0; i<eepromGetArrSize(t_uint32_t); i++)
//      uartPrintf(UART_CLI, "%d ", var_uint32_t_rd[i]);
//    uartPrintf(UART_CLI, "\n");
//
//    uartPrintf(UART_CLI, "float : ");
//    for(int i=0; i<eepromGetArrSize(t_float); i++)
//      uartPrintf(UART_CLI, "%f ", var_float_rd[i]);
//    uartPrintf(UART_CLI, "\n\n");

    //********************* UART Task Code End *********************//
  }
}


#endif //#ifdef _USE_HW_UART
