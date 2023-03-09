/*
 * gpio_mid.c
 *
 *  Created on: Sep 1, 2022
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_GPIO


typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  uint8_t       mode;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
  bool          init_value;
} gpio_tbl_t;


gpio_tbl_t gpio_tbl[GPIO_CH_MAX] =
{
    {M_GPIO_LCD_NRESET_GPIO_Port,  M_GPIO_LCD_NRESET_Pin,   _DEF_OUTPUT,  GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_ON},  //M_GPIO_PB12_LCD_NRESET
    {M_GPIO_LCD_BL_GPIO_Port,      M_GPIO_LCD_BL_Pin,       _DEF_OUTPUT,  GPIO_PIN_SET,   GPIO_PIN_RESET, GPIO_OFF}, //M_GPIO_PB13_LCD_BL
};


#ifdef _USE_HW_CLI
static void cliGpio(cli_args_t *arg);
#endif


bool gpioInit(void)
{
  bool ret = true;

  for(int i=0; i<GPIO_CH_MAX; i++)
  {
    gpioPortRcc(i, gpio_tbl[i].port); // skkim 220915
    gpioPinMode(i, gpio_tbl[i].mode);
    gpioPinWrite(i, gpio_tbl[i].init_value);
  }

#ifdef _USE_HW_CLI
  cliAdd("gpio", cliGpio);
#endif

  return ret;
}


// skkim 220915
bool gpioPortRcc(uint8_t ch, GPIO_TypeDef *port)
{
  bool ret = true;
  uint32_t portaddr=0;

  portaddr = (uint32_t)port; // port address

  switch(portaddr)
  {
    case (uint32_t)GPIOA:
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
    case (uint32_t)GPIOB:
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
    case (uint32_t)GPIOC:
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
    case (uint32_t)GPIOD:
      __HAL_RCC_GPIOD_CLK_ENABLE();
      break;
    case (uint32_t)GPIOE:
      __HAL_RCC_GPIOE_CLK_ENABLE();
      break;
    case (uint32_t)GPIOF:
      __HAL_RCC_GPIOF_CLK_ENABLE();
      break;
    case (uint32_t)GPIOG:
      __HAL_RCC_GPIOG_CLK_ENABLE();
      break;
    case (uint32_t)GPIOH:
      __HAL_RCC_GPIOH_CLK_ENABLE();
      break;
    case (uint32_t)GPIOI:
      __HAL_RCC_GPIOI_CLK_ENABLE();
      break;
  }

  return ret;
}


bool gpioPinMode(uint8_t ch, uint8_t mode)
{
  bool ret = true;
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  switch(mode)
  {
    case _DEF_INPUT:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
    case _DEF_INPUT_PULLUP:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;
    case _DEF_INPUT_PULLDOWN:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      break;
    case _DEF_OUTPUT:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
    case _DEF_OUTPUT_PULLUP:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;
    case _DEF_OUTPUT_PULLDOWN:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      break;
    case _DEF_OUTPUT_OD:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
    case _DEF_OUTPUT_OD_PULLUP:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;
  }

  GPIO_InitStruct.Pin = gpio_tbl[ch].pin;
  HAL_GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);

  return ret;
}


void gpioPinWrite(uint8_t ch, bool value)
{
  if(ch >= GPIO_CH_MAX)
  {
    return;
  }

  if(value == GPIO_ON)
  {
    HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
  }
  else
  {
    HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
  }
}


bool gpioPinRead(uint8_t ch)
{
  bool ret = false;

  if(ch >= GPIO_CH_MAX)
  {
    return false;
  }

  if(HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin) == gpio_tbl[ch].on_state)
  {
    ret = true;
  }

  return ret;
}


void gpioPinToggle(uint8_t ch)
{
  if(ch >= GPIO_CH_MAX)
  {
    return;
  }

  HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
}



#ifdef _USE_HW_CLI
void cliGpio(cli_args_t *args)
{
  bool ret = false;

  if(args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for(int i=0; i<GPIO_CH_MAX; i++)
      {
        cliPrintf("%d", gpioPinRead(i));
      }
      cliPrintf("\n");
      delay(100);
    }

    ret = true;
  }

  if(args->argc == 2 && args->isStr(0, "read") == true)
  {
    uint8_t ch;

    ch = (uint8_t)args->getData(1);

    while(cliKeepLoop())
    {
      cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));
      delay(100);
    }

    ret = true;
  }

  if(args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint8_t ch;
    uint8_t value;

    ch    = (uint8_t)args->getData(1);
    value = (uint8_t)args->getData(2);

    gpioPinWrite(ch, value);
    cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));

    ret = true;
  }

  if(ret != true)
  {
    cliPrintf("\n");
    cliPrintf("------------[ GPIO Commands ]-------------\n");
    cliPrintf(">> gpio show\n");
    cliPrintf(">> gpio read ch[0~%d]\n", GPIO_CH_MAX-1);
    cliPrintf(">> gpio write ch[0~%d] 0:1\n", GPIO_CH_MAX-1);
    cliPrintf("------------------------------------------\n");
  }
}


#endif //#ifdef _USE_HW_CLI


#endif //#ifdef _USE_HW_GPIO
