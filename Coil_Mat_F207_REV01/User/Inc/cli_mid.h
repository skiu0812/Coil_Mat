/*
 * cli_mid.h
 *
 *  Created on: Sep 13, 2022
 *      Author: KSK
 */

#ifndef INC_CLI_MID_H_
#define INC_CLI_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_CLI

#define CLI_CMD_LIST_MAX      HW_CLI_CMD_LIST_MAX
#define CLI_CMD_NAME_MAX      HW_CLI_CMD_NAME_MAX

#define CLI_LINE_HIS_MAX      HW_CLI_LINE_HIS_MAX
#define CLI_LINE_BUF_MAX      HW_CLI_LINE_BUF_MAX



typedef struct
{
  uint16_t   argc;
  char     **argv;

  int32_t  (*getData)(uint8_t index);
  float    (*getFloat)(uint8_t index);
  char    *(*getStr)(uint8_t index);
  bool     (*isStr)(uint8_t index, char *p_str);
} cli_args_t;


bool      cliInit(void);
bool      cliOpen(uint8_t ch, uint32_t baud);
bool      cliOpenLog(uint8_t ch, uint32_t baud);
bool      cliMain(uint32_t taskPeriod);
void      cliPrintf(const char *fmt, ...);
bool      cliAdd(const char *cmd_str, void (*p_func)(cli_args_t *));
bool      cliKeepLoop(void);


#endif


#endif /* INC_CLI_MID_H_ */
