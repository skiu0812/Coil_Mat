/*
 * flash_mid.c
 *
 *  Created on: 2022. 9. 9.
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_FLASH


//#define _USE_FLASH_PAGE         0 //flash 영역이 PAGE로 구성된 경우 활성화 ex)stm32f103
#define _USE_FLASH_SECTOR       0 //flash 영역이 SECTOR로 구성된 경우 활성화 ex)stm32f407

#define FLASH_BASE_ADDRESS      0x08000000
#define FLASH_BANK_MAX          1
#define FLASH_SECTOR_MAX        12 // The Maximum Number of Sector in a Bank

#ifdef _USE_FLASH_PAGE
#define FLASH_PAGE_SIZE         1024
static flash_tbl_t flash_tbl[FLASH_SECTOR_MAX];
#endif //#ifdef _USE_FLASH_PAGE

#ifdef _USE_FLASH_SECTOR
const flash_tbl_t flash_tbl[FLASH_SECTOR_MAX] =
{
    {0x8000000,  16*1024},  //Sector 0
    {0x8004000,  16*1024},  //Sector 1
    {0x8008000,  16*1024},  //Sector 2
    {0x800C000,  16*1024},  //Sector 3
    {0x8010000,  64*1024},  //Sector 4
    {0x8020000, 128*1024},  //Sector 5
    {0x8040000, 128*1024},  //Sector 6
    {0x8060000, 128*1024},  //Sector 7
    {0x8080000, 128*1024},  //Sector 8
    {0x80A0000, 128*1024},  //Sector 9
    {0x80C0000, 128*1024},  //Sector 10
    {0x80E0000, 128*1024},  //Sector 11
};
#endif //#ifdef _USE_FLASH_SECTOR


static bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length);


#ifdef _USE_HW_CLI
static void cliFlash(cli_args_t *args);
#endif


bool flashInit(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
  cliAdd("flash", cliFlash);
#endif

#ifdef _USE_FLASH_PAGE
  for(int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    flash_tbl[i].addr   = FLASH_BASE_ADDRESS + (i * FLASH_PAGE_SIZE);
    flash_tbl[i].length = FLASH_PAGE_SIZE;
  }
#endif //#ifdef _USE_FLASH_PAGE

  return ret;
}


bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = false;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef init;
  uint32_t page_error;

  int16_t start_sector_num = -1;
  uint32_t sector_count = 0;

  for(int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    if(flashInSector(i, addr, length) == true)
    {
      if(start_sector_num < 0)
      {
        start_sector_num = i;
      }
      sector_count++;
    }
  }

  if(sector_count > 0)
  {
    HAL_FLASH_Unlock();

#ifdef _USE_FLASH_PAGE
    init.TypeErase    = FLASH_TYPEERASE_PAGES;
    init.Banks        = FLASH_BANK_1;
    init.PageAddress  = flash_tbl[start_sector_num].addr;
    init.NbSectors    = sector_count;
    init.VoltageRange = FLASH_VOLTAGE_RANGE_3;
#endif //#ifdef _USE_FLASH_PAGE

#ifdef _USE_FLASH_SECTOR
    init.TypeErase    = FLASH_TYPEERASE_SECTORS;
    init.Banks        = FLASH_BANK_1;
    init.Sector       = start_sector_num;
    init.NbSectors    = sector_count;
    init.VoltageRange = FLASH_VOLTAGE_RANGE_3;
#endif //#ifdef _USE_FLASH_SECTOR

    status = HAL_FLASHEx_Erase(&init, &page_error);

    if(status == HAL_OK)
    {
      ret = true;
    }

    HAL_FLASH_Lock();
  }

  return ret;
}


bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
 bool ret = true;
 HAL_StatusTypeDef status;

#ifdef _USE_FLASH_PAGE
 if(addr%2 != 0)
 {
   return false;
 }

 HAL_FLASH_Unlock();

 for(int i=0; i<length; i+=2)
 {
   uint16_t data;

   data  = p_data[i+0] << 0;
   data |= p_data[i+1] << 8;

   status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, (uint64_t)data);

   if(status != HAL_OK)
   {
     ret = false;
     break;
   }
 }
#endif //#ifdef _USE_FLASH_PAGE

#ifdef _USE_FLASH_SECTOR
 HAL_FLASH_Unlock();

 for(int i=0; i<length; i++)
 {
   uint8_t data;

   data  = p_data[i];

   status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, (uint64_t)data);

   if(status != HAL_OK)
   {
     ret = false;
     break;
   }
 }
#endif //#ifdef _USE_FLASH_SECTOR

 HAL_FLASH_Lock();

 return ret;
}


bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint8_t *p_byte =  (uint8_t *)addr;

  for(int i=0; i<length; i++)
  {
    p_data[i] = p_byte[i];
  }

  return ret;
}


bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  bool ret = false;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;

  sector_start  = flash_tbl[sector_num].addr;
  sector_end    = flash_tbl[sector_num].addr + flash_tbl[sector_num].length - 1;

  flash_start   = addr;
  flash_end     = addr + length - 1;

  if(sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = true;
  }

  if(sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = true;
  }

  if(flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = true;
  }

  if(flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = true;
  }

  return ret;
}



#ifdef _USE_HW_CLI
void cliFlash(cli_args_t *args)
{
 bool ret = false;

 if(args->isStr(0, "info") == true &&
     args->argc == 1)
 {
   for(int i=0; i<FLASH_SECTOR_MAX; i++)
   {
     cliPrintf("0x%X : %dKB\n", flash_tbl[i].addr, flash_tbl[i].length/1024);
   }

   ret = true;
 }

 if(args->isStr(0, "read") == true &&
     args->getData(1) > 0 &&
     args->getData(2) > 0 &&
     args->argc == 3)
 {
   uint32_t addr;
   uint32_t length;

   addr   = (uint32_t)args->getData(1);
   length = (uint32_t)args->getData(2);

   for(int i=0; i<length; i++)
   {
     cliPrintf("0x%X : 0x%X\n", addr+i, *((uint8_t *)(addr+i)));
   }

   ret = true;
 }

 if(args->isStr(0, "erase") == true &&
     args->getData(1) > 0 &&
     args->getData(2) > 0 &&
     args->argc == 3)
 {
   uint32_t addr;
   uint32_t length;

   addr   = (uint32_t)args->getData(1);
   length = (uint32_t)args->getData(2);

   if(flashErase(addr, length) == true)
   {
     cliPrintf("Erase OK\n");
   }
   else
   {
     cliPrintf("Erase Fail\n");
   }

   ret = true;
 }

 if(args->isStr(0, "write") == true &&
     args->getData(1) > 0 &&
     args->getData(2) > 0 &&
     args->argc == 3)
 {
   uint32_t addr;
   uint32_t data;

   addr = (uint32_t)args->getData(1);
   data = (uint32_t)args->getData(2);

   if(flashWrite(addr, (uint8_t *)&data, 4) == true)
   {
     cliPrintf("Write OK\n");
   }
   else
   {
     cliPrintf("Write Fail\n");
   }

   ret = true;
 }

#ifdef _USE_SW_EEPROM
 if(args->isStr(0, "eeprom") == true &&
     args->isStr(1, "format") == true &&
     args->argc == 2)
 {
   uint32_t addr;
   uint32_t length;

   addr   = (uint32_t)SW_EEPROM_START_ADDR;
   length = (uint32_t)SW_EEPROM_PAGE_SIZE*2;

   if(flashErase(addr, length) == true)
   {
     cliPrintf("EEPROM format Complete!!\n");
   }
   else
   {
     cliPrintf("EEPROM format Fail!!\n");
   }

   ret = true;
 }
#endif //#ifdef _USE_SW_EEPROM


 if(ret != true)
 {
   cliPrintf("\n------------[ Flash Commands ]-------------\n\n");

   cliPrintf(">> flash info\n");
   cliPrintf(">> flash read  [addr] [length]\n");
   cliPrintf(">> flash erase [addr] [length]\n");
   cliPrintf(">> flash write [addr] [data]\n");

#ifdef _USE_SW_EEPROM
   cliPrintf("\n>> flash eeprom format\n");
#endif //#ifdef _USE_SW_EEPROM

   cliPrintf("\n-------------------------------------------\n\n");
 }
}
#endif //#ifdef _USE_HW_CLI


#endif //#ifdef _USE_HW_FLASH
