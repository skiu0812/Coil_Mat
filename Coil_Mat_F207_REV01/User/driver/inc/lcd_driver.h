/*
 * lcd_driver.h
 *
 *  Created on: 2023. 1. 25.
 *      Author: KSK
 */

#ifndef DRIVER_INC_LCD_DRIVER_H_
#define DRIVER_INC_LCD_DRIVER_H_


#include "hw_def.h"
#include "lcd_mid.h"


#ifdef _USE_HW_LCD


#define LCD_REG_NOP             0x00 //ST7735 //ILI9341
#define LCD_REG_SWRESET         0x01 //ST7735 //ILI9341
#define LCD_REG_RDDID           0x04 //ST7735 //ILI9341
#define LCD_REG_RDDST           0x09 //ST7735 //ILI9341
#define LCD_REG_RDDPM           0x0A //ST7735 //ILI9341
#define LCD_REG_RDDMADCTL       0x0B //ST7735 //ILI9341
#define LCD_REG_RDDCOLMOD       0x0C //ST7735 //ILI9341
#define LCD_REG_RDDIM           0x0D //ST7735 //ILI9341
#define LCD_REG_RDDSM           0x0E //ST7735 //ILI9341
#define LCD_REG_RDDSDR          0x0F //  X    //ILI9341

#define LCD_REG_SLPIN           0x10 //ST7735 //ILI9341
#define LCD_REG_SLPOUT          0x11 //ST7735 //ILI9341
#define LCD_REG_PTLON           0x12 //ST7735 //ILI9341
#define LCD_REG_NORON           0x13 //ST7735 //ILI9341

#define LCD_REG_INVOFF          0x20 //ST7735 //ILI9341
#define LCD_REG_INVON           0x21 //ST7735 //ILI9341
#define LCD_REG_GAMSET          0x26 //ST7735 //ILI9341
#define LCD_REG_DISPOFF         0x28 //ST7735 //ILI9341
#define LCD_REG_DISPON          0x29 //ST7735 //ILI9341
#define LCD_REG_CASET           0x2A //ST7735 //ILI9341
#define LCD_REG_RASET           0x2B //ST7735 //ILI9341
#define LCD_REG_RAMWR           0x2C //ST7735 //ILI9341
#define LCD_REG_RGBSET          0x2D //ST7735 //ILI9341
#define LCD_REG_RAMRD           0x2E //ST7735 //ILI9341

#define LCD_REG_PTLAR           0x30 //ST7735 //ILI9341
#define LCD_REG_VSCRDEF         0x33 //  X    //ILI9341
#define LCD_REG_TEOFF           0x34 //ST7735 //ILI9341
#define LCD_REG_TEON            0x35 //ST7735 //ILI9341
#define LCD_REG_MADCTL          0x36 //ST7735 //ILI9341
#define         MADCTL_MY           0x80
#define         MADCTL_MX           0x40
#define         MADCTL_MV           0x20
#define         MADCTL_ML           0x10
#define         MADCTL_RGB          0x00
#define         MADCTL_BGR          0x08
#define         MADCTL_MH           0x04
#define LCD_REG_VSCRSADD        0x37 //  X    //ILI9341
#define LCD_REG_IDMOFF          0x38 //ST7735 //ILI9341
#define LCD_REG_IDMON           0x39 //ST7735 //ILI9341
#define LCD_REG_COLMOD          0x3A //ST7735 //ILI9341
#define LCD_REG_WRMCNTN         0x3C //  X    //ILI9341
#define LCD_REG_RDMCNTN         0x3E //  X    //ILI9341

#define LCD_REG_STSL            0x44 //  X    //ILI9341
#define LCD_REG_GSL             0x45 //  X    //ILI9341

#define LCD_REG_WRDISBV         0x51 //  X    //ILI9341
#define LCD_REG_RDDISBV         0x52 //  X    //ILI9341
#define LCD_REG_WRCTRLD         0x53 //  X    //ILI9341
#define LCD_REG_RDCTRLD         0x54 //  X    //ILI9341
#define LCD_REG_WRCABC          0x55 //  X    //ILI9341
#define LCD_REG_RDCABC          0x56 //  X    //ILI9341
#define LCD_REG_WRBLCTRL1       0x5E //  X    //ILI9341
#define LCD_REG_RDBLCTRL1       0x5F //  X    //ILI9341

#define LCD_REG_RDID1           0xDA //ST7735 //ILI9341
#define LCD_REG_RDID2           0xDB //ST7735 //ILI9341
#define LCD_REG_RDID3           0xDC //ST7735 //ILI9341

#define LCD_REG_IFMODE          0xB0 //  X    //ILI9341
#define LCD_REG_FRMCTR1         0xB1 //ST7735 //ILI9341
#define LCD_REG_FRMCTR2         0xB2 //ST7735 //ILI9341
#define LCD_REG_FRMCTR3         0xB3 //ST7735 //ILI9341
#define LCD_REG_INVCTR          0xB4 //ST7735 //ILI9341
#define LCD_REG_PRCTR           0xB5 //  X    //ILI9341
#define LCD_REG_DISSET          0xB6 //ST7735 //ILI9341
#define LCD_REG_ETMOD           0xB7 //  X    //ILI9341
#define LCD_REG_BLCTRL1         0xB8 //  X    //ILI9341
#define LCD_REG_BLCTRL2         0xB9 //  X    //ILI9341
#define LCD_REG_BLCTRL3         0xBA //  X    //ILI9341
#define LCD_REG_BLCTRL4         0xBB //  X    //ILI9341
#define LCD_REG_BLCTRL5         0xBC //  X    //ILI9341
#define LCD_REG_BLCTRL7         0xBE //  X    //ILI9341
#define LCD_REG_BLCTRL8         0xBF //  X    //ILI9341

#define LCD_REG_PWCTR1          0xC0 //ST7735 //ILI9341
#define LCD_REG_PWCTR2          0xC1 //ST7735 //ILI9341
#define LCD_REG_PWCTR3          0xC2 //ST7735
#define LCD_REG_PWCTR4          0xC3 //ST7735
#define LCD_REG_PWCTR5          0xC4 //ST7735
#define LCD_REG_VMCTR1          0xC5 //ST7735 //ILI9341
#define LCD_REG_VMOFCTR         0xC7 //ST7735 //ILI9341
#define LCD_REG_PWCTRLA         0xCB //  X    //ILI9341
#define LCD_REG_PWCTRLB         0xCF //  X    //ILI9341


#ifdef _USE_HW_ST7735
#define ST7735_WRID2            0xD1 //ST7735
#define ST7735_WRID3            0xD2 //ST7735
#define ST7735_NVCTR1           0xD9 //ST7735
#define ST7735_NVCTR2           0xDE //ST7735
#define ST7735_NVCTR3           0xDF //ST7735

#define ST7735_GMCTRP1          0xE0 //ST7735
#define ST7735_GMCTRN1          0xE1 //ST7735
#endif //#ifdef _USE_HW_ST7735


#ifdef _USE_HW_ILI9341
#define ILI9341_NVMWR           0xD0 //  X    //ILI9341
#define ILI9341_NVMPKEY         0xD1 //  X    //ILI9341
#define ILI9341_RDNVM           0xD2 //  X    //ILI9341
#define ILI9341_RDID4           0xD3 //  X    //ILI9341

#define ILI9341_PGAMCTRL        0xE0 //  X    //ILI9341
#define ILI9341_NGAMCTRL        0xE1 //  X    //ILI9341
#define ILI9341_DGAMCTRL1       0xE2 //  X    //ILI9341
#define ILI9341_DGAMCTRL2       0xE3 //  X    //ILI9341
#define ILI9341_DRVTIMCTRLA1    0xE8 //  X    //ILI9341
#define ILI9341_DRVTIMCTRLA2    0xE9 //  X    //ILI9341
#define ILI9341_DRVTIMCTRLB     0xEA //  X    //ILI9341
#define ILI9341_PWONSEQCTRL     0xED //  X    //ILI9341

#define ILI9341_EN3G            0xF2 //  X    //ILI9341
#define ILI9341_IFCTL           0xF6 //  X    //ILI9341
#define ILI9341_PUMPCTRL        0xF7 //  X    //ILI9341
#endif //#ifdef _USE_HW_ILI9341


void writecommand(__IO uint8_t c);
void writedata(__IO uint8_t d);
void writecolor(__IO uint16_t d);

bool lcdDriverInit(void);
bool lcdDriverFuncSet(lcd_driver_t *p_driver);


#endif //#ifdef _USE_HW_LCD

#endif /* DRIVER_INC_LCD_DRIVER_H_ */
