/*
 * eeprom_mid.c
 *
 *  Created on: 2023. 2. 28.
 *      Author: KSK
 *
 */

/*
 * description  : flash eeprom emulator에 있는 EE_ReadVariable()와 EE_WriteVariable() 함수는
 *                변수 1개씩만 read/write 할 수 있다.
 *                배열 및 문자열을 read/write 하기위해 이 함수를 만들었다.
 *
 *              : flash eeprom emulator는 write 할 때 가상의주소(VirtAddress)를 data와 함께 묶어서 저장하고,
 *                Read할 때 이 VirtAddress를 검색해서 마지막에 저장된 data를 읽어온다.
 *                VirtAddress영역과 data영역은 각각 16bit로 변수 하나에 32bit가 사용된다.
 *
 *              : 배열(문자열)을 저장 할 때에도 배열의 원소 마다 VirtAddress가 하나씩 부여된다.
 *                배열의 고유번호(또는 배열이름(var))를 이용한 VirtAddress는 다음과 같이 생성되며,
 *                이것을 base_addr라고 한다.
 *                -> base_addr = (고유번호 << SHIFT_SIZE)
 *                -> ex) 배열의 고유번호:3, SHIRT_SIZE=(16 - SW_EEPROM_VAR_BIT_SIZE)
 *                       base_addr = (uint16_t)(3 << 8) = 0x0300
 *
 *              : 16bit로 배열의 고유번호와 배열원소를 구분할 수 있도록 다음과 같이 VirtAddress를 생성한다.
 *                16bit 중 MSB는 배열의 고유번호 LSB는 배열원소번호를 위치시킨다.
 *                MSB 영역의 크기를 SW_EEPROM_VAR_BIT_SIZE로 설정한다.
 *                고유번호를 MSB에 위치시키기 위해 왼쪽으로 SHIFT_SIZE만큼 bit shift한다.
 *                고유번호가 위치한 영역 외에 나머지 영역이 배열원소영역이 된다.
 *                (SHIFT_SIZE는 SW_EEPROM_VAR_BIT_SIZE에 따라 바뀐다. 고유번호 영역(MSB)의 비트를 늘리면
 *                 사용가능한 배열변수의 수가 증가하지만 배열의 최대 크기는 작아진다.)
 *                 -> ex)  SW_EEPROM_VAR_BIT_SIZE = 10
 *                         사용가능한 배열 변수의 수(MSB 10bit) = 2^10 = 1024개
 *                         사용가능한 배열 최대 크기(LSB 6bit)  = 2^6  = 64개
 *
 *                         SW_EEPROM_VAR_BIT_SIZE = 7
 *                         사용가능한 배열 변수의 수(MSB 7bit)  = 2^7  = 128개
 *                         사용가능한 배열 최대 크기(LSB 9bit)  = 2^9  = 512개
 *
 *              : 이렇게 생성된 base_addr로 원소의 VirtAddress를 생성하면 다음과 같다.
 *                (배열의 첫번째 주소에는 이 배열의 자료형(Data Type)이 저장되고, 두번째 주소에는
 *                 배열의 길이(Length)가 저장된다. 따라서 배열의 데이터는 세번째 주소부터 저장된다.)
 *                -> (0x0300 | 0x00) = 0x0300 : var의 Data Type
 *                   (0x0300 | 0x01) = 0x0301 : var의 Length
 *                -> base_addr | (원소번호+2)
 *                   (0x0300 | (0+2)) = 0x0302 : var[0]의 VirtAddress
 *                   (0x0300 | (1+2)) = 0x0303 : var[1]의 VirtAddress
 */


#include "user.h"


#ifdef _USE_SW_EEPROM


#define SHIFT_SIZE         (16 - SW_EEPROM_VAR_BIT_SIZE)


bool eepromWrite_char(uint8_t dataType, uint16_t var, uint8_t arrNum, char *data);
bool eepromWrite_int8_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int8_t *data);
bool eepromWrite_uint8_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint8_t *data);
bool eepromWrite_int16_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int16_t *data);
bool eepromWrite_uint16_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint16_t *data);
bool eepromWrite_int32_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int32_t *data);
bool eepromWrite_uint32_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint32_t *data);
bool eepromWrite_float(uint8_t dataType, uint16_t var, uint8_t arrNum, float *data);

bool eepromRead_char(uint16_t var, char *data);
bool eepromRead_int8_t(uint16_t var, int8_t *data);
bool eepromRead_uint8_t(uint16_t var, uint8_t *data);
bool eepromRead_int16_t(uint16_t var, int16_t *data);
bool eepromRead_uint16_t(uint16_t var, uint16_t *data);
bool eepromRead_int32_t(uint16_t var, int32_t *data);
bool eepromRead_uint32_t(uint16_t var, uint32_t *data);
bool eepromRead_float(uint16_t var, float *data);

uint16_t eepromGetArrSize(uint16_t var);
uint16_t eepromGetType(uint16_t var);

//uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
//uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VirtAddVarTab[(NB_OF_VAR*4)]; //by skkim
uint16_t VarDataTab[(NB_OF_VAR*4)]; //by skkim


bool eepromInit(void)
{
  bool ret = true;

  eepromCreatFirstVariables();

  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();

  /* EEPROM Init */
  if( EE_Init() != EE_OK)
  {
    Error_Handler();
  }

  // First access check
  if(EE_CheckFirstAccess() == true)
  {
    char inputchar[4] = "NON";

    for(int i=0; i<var_num; i++)
    {
      eepromWrite(CHAR, i, strlen(inputchar), inputchar);
    }
  }

  return ret;
}

bool eepromCreatFirstVariables(void)
{
  bool ret = true;

  uint16_t var, data;

  for(int i=0; i<NB_OF_VAR; i++)
  {
    for(int j=0; j<4; j++)
    {
      var = ((uint16_t)i << SHIFT_SIZE) | j;

      if(j==0)
        data = UINT8_T;
      else if(j==1)
        data = 2;
      else
        data = 0x0100;

      VirtAddVarTab[(i*3)+j]  = var;
      VarDataTab[(i*3)+j]     = data;
    }
  }

  return ret;
}


/*
 * dataType : 입력 데이터의 자료형
 * var      : 변수 고유번호(eeprom_mid.h -> eeprom_var_e)
 * arrNum   : 변수가 배열일 경우 배열의 크기, 배열이 아니면 1
 * ...      : 입력 데이터(pointer)
 */
bool eepromWrite(uint8_t dataType, uint16_t var, uint8_t arrNum, ...)
{
  bool ret = true;

  va_list ap;
  va_start (ap, arrNum);

  switch (dataType)
  {
      case 0:   eepromWrite_char      (dataType, var, arrNum, va_arg(ap, char *));      break;
      case 1:   eepromWrite_int8_t    (dataType, var, arrNum, va_arg(ap, int8_t *));    break;
      case 2:   eepromWrite_uint8_t   (dataType, var, arrNum, va_arg(ap, uint8_t *));   break;
      case 3:   eepromWrite_int16_t   (dataType, var, arrNum, va_arg(ap, int16_t *));   break;
      case 4:   eepromWrite_uint16_t  (dataType, var, arrNum, va_arg(ap, uint16_t *));  break;
      case 5:   eepromWrite_int32_t   (dataType, var, arrNum, va_arg(ap, int32_t *));   break;
      case 6:   eepromWrite_uint32_t  (dataType, var, arrNum, va_arg(ap, uint32_t *));  break;
      case 7:   eepromWrite_float     (dataType, var, arrNum, va_arg(ap, float *));     break;
      default:  uartPrintf(UART_CLI, "eepromWrite() Error");   break;
  }

  return ret;
}

/*
 * var : 변수 고유번호(eeprom_mid.h -> eeprom_var_e)
 */
bool eepromRead(uint16_t var, ...)
{
  bool ret = true;
  uint16_t data_type;
  uint16_t base_addr;

  va_list ap;
  va_start(ap, var);

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr, &data_type) != HAL_OK)
  {
    Error_Handler();
  }

  switch(data_type)
  {
      case 0:   eepromRead_char      (var, va_arg(ap, char *));      break;
      case 1:   eepromRead_int8_t    (var, va_arg(ap, int8_t *));    break;
      case 2:   eepromRead_uint8_t   (var, va_arg(ap, uint8_t *));   break;
      case 3:   eepromRead_int16_t   (var, va_arg(ap, int16_t *));   break;
      case 4:   eepromRead_uint16_t  (var, va_arg(ap, uint16_t *));  break;
      case 5:   eepromRead_int32_t   (var, va_arg(ap, int32_t *));   break;
      case 6:   eepromRead_uint32_t  (var, va_arg(ap, uint32_t *));  break;
      case 7:   eepromRead_float     (var, va_arg(ap, float *));     break;
      default:  uartPrintf(UART_CLI, "eepromRead() Error");   break;
  }

  return ret;
}



//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
bool eepromWrite_char(uint8_t dataType, uint16_t var, uint8_t arrNum, char *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(i%2 == 0)
      data_temp = ((uint16_t)data[i] << 8) & 0xff00;
    else if(i%2 == 1)
      data_temp |= ((uint16_t)data[i] << 0) & 0x00ff;

    if(i%2 == 1 || i == (arrNum-1))
    {
      if((EE_WriteVariable(base_addr+(i/2), data_temp)) != HAL_OK)
      {
        Error_Handler();
      }
    }
  }

  return ret;
}

bool eepromWrite_int8_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int8_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(i%2 == 0)
      data_temp = ((uint16_t)data[i] << 8) & 0xff00;
    else if(i%2 == 1)
      data_temp |= ((uint16_t)data[i] << 0) & 0x00ff;

    if(i%2 == 1 || i == (arrNum-1))
    {
      if((EE_WriteVariable(base_addr+(i/2), data_temp)) != HAL_OK)
      {
        Error_Handler();
      }
    }
  }

  return ret;
}

bool eepromWrite_uint8_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint8_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(i%2 == 0)
      data_temp = ((uint16_t)data[i] << 8) & 0xff00;
    else if(i%2 == 1)
      data_temp |= ((uint16_t)data[i] << 0) & 0x00ff;

    if(i%2 == 1 || i == (arrNum-1))
    {
      if((EE_WriteVariable(base_addr+(i/2), data_temp)) != HAL_OK)
      {
        Error_Handler();
      }
    }
  }

  return ret;
}

bool eepromWrite_int16_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int16_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    data_temp = (uint16_t)data[i];

    if((EE_WriteVariable(base_addr+i, data_temp)) != HAL_OK)
    {
      Error_Handler();
    }
  }

  return ret;
}

bool eepromWrite_uint16_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint16_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    data_temp = (uint16_t)data[i];

    if((EE_WriteVariable(base_addr+i, data_temp)) != HAL_OK)
    {
      Error_Handler();
    }
  }

  return ret;
}

bool eepromWrite_int32_t(uint8_t dataType, uint16_t var, uint8_t arrNum, int32_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp[2];

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    data_temp[0] = (uint16_t)(((uint32_t)data[i] & 0xffff0000) >> 16);
    data_temp[1] = (uint16_t)(((uint32_t)data[i] & 0x0000ffff) >> 0);

    if((EE_WriteVariable(base_addr+(i*2), data_temp[0])) != HAL_OK)
    {
      Error_Handler();
    }

    if((EE_WriteVariable(base_addr+(i*2)+1, data_temp[1])) != HAL_OK)
    {
      Error_Handler();
    }
  }

  return ret;
}

bool eepromWrite_uint32_t(uint8_t dataType, uint16_t var, uint8_t arrNum, uint32_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp[2];

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    data_temp[0] = (uint16_t)(((uint32_t)data[i] & 0xffff0000) >> 16);
    data_temp[1] = (uint16_t)(((uint32_t)data[i] & 0x0000ffff) >> 0);

    if((EE_WriteVariable(base_addr+(i*2), data_temp[0])) != HAL_OK)
    {
      Error_Handler();
    }

    if((EE_WriteVariable(base_addr+(i*2)+1, data_temp[1])) != HAL_OK)
    {
      Error_Handler();
    }
  }

  return ret;
}

bool eepromWrite_float(uint8_t dataType, uint16_t var, uint8_t arrNum, float *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t data_temp[2];
  uint32_t float2hex_data;

  if(arrNum == 0) return false;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if((EE_WriteVariable(base_addr++, dataType)) != HAL_OK)
  {
    Error_Handler();
  }

  if((EE_WriteVariable(base_addr++, arrNum)) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    float2hex_data = Float2Hex(data[i]);

    data_temp[0] = (uint16_t)(((uint32_t)float2hex_data & 0xffff0000) >> 16);
    data_temp[1] = (uint16_t)(((uint32_t)float2hex_data & 0x0000ffff) >> 0);

    if((EE_WriteVariable(base_addr+(i*2), data_temp[0])) != HAL_OK)
    {
      Error_Handler();
    }

    if((EE_WriteVariable(base_addr+(i*2)+1, data_temp[1])) != HAL_OK)
    {
      Error_Handler();
    }
  }

  return ret;
}


bool eepromRead_char(uint16_t var, char *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint16_t data_temp;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i/2), &data_temp) != HAL_OK)
    {
      Error_Handler();
    }

    if(i%2 == 0)
      data[i] = (char)((data_temp & 0xff00) >> 8);
    else if(i%2 == 1)
      data[i] = (char)((data_temp & 0x00ff) >> 0);
  }

  data[arrNum] = '\0';

  return ret;
}

bool eepromRead_int8_t(uint16_t var, int8_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint16_t data_temp;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i/2), &data_temp) != HAL_OK)
    {
      Error_Handler();
    }

    if(i%2 == 0)
      data[i] = (int8_t)((data_temp & 0xff00) >> 8);
    else if(i%2 == 1)
      data[i] = (int8_t)((data_temp & 0x00ff) >> 0);
  }

  return ret;
}

bool eepromRead_uint8_t(uint16_t var, uint8_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint16_t data_temp;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i/2), &data_temp) != HAL_OK)
    {
      Error_Handler();
    }

    if(i%2 == 0)
      data[i] = (uint8_t)((data_temp & 0xff00) >> 8);
    else if(i%2 == 1)
      data[i] = (uint8_t)((data_temp & 0x00ff) >> 0);
  }

  return ret;
}

bool eepromRead_int16_t(uint16_t var, int16_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint16_t data_temp;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+i, &data_temp) != HAL_OK)
    {
      Error_Handler();
    }

    data[i] = (int16_t)data_temp;
  }

  return ret;
}

bool eepromRead_uint16_t(uint16_t var, uint16_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint16_t data_temp;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+i, &data_temp) != HAL_OK)
    {
      Error_Handler();
    }

    data[i] = (uint16_t)data_temp;
  }

  return ret;
}

bool eepromRead_int32_t(uint16_t var, int32_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint32_t read_data;
  uint16_t data_temp[2];

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i*2), &data_temp[0]) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(base_addr+(i*2)+1, &data_temp[1]) != HAL_OK)
    {
      Error_Handler();
    }

    read_data  = (uint32_t)data_temp[0] << 16;
    read_data |= (uint32_t)data_temp[1] << 0;

    data[i] = (int32_t)read_data;
  }

  return ret;
}

bool eepromRead_uint32_t(uint16_t var, uint32_t *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint32_t read_data;
  uint16_t data_temp[2];

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i*2), &data_temp[0]) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(base_addr+(i*2)+1, &data_temp[1]) != HAL_OK)
    {
      Error_Handler();
    }

    read_data  = (uint32_t)data_temp[0] << 16;
    read_data |= (uint32_t)data_temp[1] << 0;

    data[i] = (uint32_t)read_data;
  }

  return ret;
}

bool eepromRead_float(uint16_t var, float *data)
{
  bool ret = true;
  uint16_t base_addr;
  uint16_t dataType;
  uint16_t arrNum;
  uint32_t read_data;
  uint16_t data_temp[2];

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr++, &dataType) != HAL_OK)
  {
    Error_Handler();
  }

  if(EE_ReadVariable(base_addr++, &arrNum) != HAL_OK)
  {
    Error_Handler();
  }

  for(int i=0; i<arrNum; i++)
  {
    if(EE_ReadVariable(base_addr+(i*2), &data_temp[0]) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(base_addr+(i*2)+1, &data_temp[1]) != HAL_OK)
    {
      Error_Handler();
    }

    read_data  = (uint32_t)data_temp[0] << 16;
    read_data |= (uint32_t)data_temp[1] << 0;

    data[i] = Hex2Float(read_data);
  }

  return ret;
}


uint16_t eepromGetArrSize(uint16_t var)
{
  uint16_t arr_size = 0;
  uint16_t base_addr;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr+1, &arr_size) != HAL_OK)
  {
    Error_Handler();
  }

  return arr_size;
}

uint16_t eepromGetType(uint16_t var)
{
  uint16_t type = 0;
  uint16_t base_addr;

  base_addr = ((uint16_t)var << SHIFT_SIZE);

  if(EE_ReadVariable(base_addr, &type) != HAL_OK)
  {
    Error_Handler();
  }

  return type;
}


#endif //#ifdef _USE_SW_EEPROM
