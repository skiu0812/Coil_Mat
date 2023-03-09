/*
 * lcd_driver.c
 *
 *  Created on: 2023. 1. 25.
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_LCD


#define LCD_WIDTH         HW_LCD_WIDTH
#define LCD_HEIGHT        HW_LCD_HEIGHT
#define LCD_ROTAT_MODE    HW_LCD_ROTAT_MODE

#define COL_OFFSET        HW_LCD_COL_OFFSET
#define ROW_OFFSET        HW_LCD_ROW_OFFSET

#define LCD_REG           HW_LCD_REG
#define LCD_RAM           HW_LCD_RAM

#define LCD_SPI_CH        SPI_LCD


static void (*frameCallBack)(void) = NULL;
volatile static bool  is_write_frame = false;


static bool     lcdDriverReset(void);
static uint16_t lcdDriverGetWidth(void);
static uint16_t lcdDriverGetHeight(void);
static void     lcdDriverFillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
static bool     lcdDriverSendBuffer(uint16_t *p_data, uint32_t length, uint32_t timeout_ms);
static bool     lcdDriverSetCallBack(void (*p_func)(void));
static void     lcdDriverSetWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
static void     lcdDriverSetRotation(uint8_t m);
static void     lcdDriverInitRegs(void);



void writecommand(__IO uint8_t c)
{
#if HW_LCD_FSMC
  *(__IO uint8_t *)LCD_REG = (uint8_t)c;
#endif
#if HW_LCD_SPI
  gpioPinWrite(GPIO_LCD_DC, GPIO_OFF);
  gpioPinWrite(GPIO_LCD_CS, GPIO_OFF);

  spiTransfer8(LCD_SPI_CH, c);

  gpioPinWrite(GPIO_LCD_CS, GPIO_ON);
#endif
}

void writedata(__IO uint8_t d)
{
#if HW_LCD_FSMC
  *(__IO uint8_t *)LCD_RAM = (uint8_t)d;
#endif
#if HW_LCD_SPI
  gpioPinWrite(GPIO_LCD_DC, GPIO_ON);
  gpioPinWrite(GPIO_LCD_CS, GPIO_OFF);

  spiTransfer8(LCD_SPI_CH, d);

  gpioPinWrite(GPIO_LCD_CS, GPIO_ON);
#endif
}

void writecolor(__IO uint16_t d)
{
#if HW_LCD_FSMC
  *(__IO uint16_t *)LCD_RAM = (uint16_t)d;
#endif
}

uint16_t readdata(void)
{
  uint16_t ret = true;
#if HW_LCD_FSMC
  ret = *(__IO uint16_t *)LCD_RAM;
#endif
  return ret;
}

static void TransferDoneISR(void)
{
  if (is_write_frame == true)
  {
    is_write_frame = false;

#if HW_LCD_SPI
    gpioPinWrite(GPIO_LCD_CS, GPIO_ON);
#endif

    if (frameCallBack != NULL)
    {
      frameCallBack();
    }
  }
}


bool lcdDriverInit(void)
{
  bool ret;

  ret = lcdDriverReset();

  return ret;
}

bool lcdDriverFuncSet(lcd_driver_t *p_driver)
{
  p_driver->init = lcdDriverInit;
  p_driver->reset = lcdDriverReset;
  p_driver->setWindow = lcdDriverSetWindow;
  p_driver->getWidth = lcdDriverGetWidth;
  p_driver->getHeight = lcdDriverGetHeight;
  p_driver->setCallBack = lcdDriverSetCallBack;
  p_driver->sendBuffer = lcdDriverSendBuffer;
  return true;
}

bool lcdDriverReset(void)
{
#ifdef _USE_HW_ST7735
#if HW_LCD_FSMC
  gpioPinWrite(GPIO_LCD_NRESET, GPIO_OFF);
  delay(50);
  gpioPinWrite(GPIO_LCD_NRESET, GPIO_ON);
  delay(50);
#endif

#if HW_LCD_SPI
  spiBegin(SPI_LCD);
  spiSetDataMode(SPI_LCD, SPI_MODE0);

  spiAttachTxInterrupt(SPI_LCD, TransferDoneISR);

  gpioPinWrite(GPIO_LCD_BL, GPIO_OFF);
  gpioPinWrite(GPIO_LCD_DC,  GPIO_ON);
  gpioPinWrite(GPIO_LCD_CS,  GPIO_ON);
  delay(10);
#endif
#endif //#ifdef _USE_HW_ST7735

#ifdef _USE_HW_ILI9341
#if HW_LCD_FSMC
  gpioPinWrite(GPIO_LCD_IM1, GPIO_OFF);
  gpioPinWrite(GPIO_LCD_IM2, GPIO_OFF);
  gpioPinWrite(GPIO_LCD_NEN, GPIO_OFF);
  gpioPinWrite(GPIO_LCD_NRESET, GPIO_OFF);
  delay(100);
  gpioPinWrite(GPIO_LCD_NRESET, GPIO_ON);
#endif
#endif //#ifdef _USE_HW_ILI9341

  lcdDriverInitRegs();

  lcdDriverSetRotation(LCD_ROTAT_MODE);

  lcdDriverFillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
  gpioPinWrite(GPIO_LCD_BL, GPIO_OFF);
  return true;
}

uint16_t lcdDriverGetWidth(void)
{
  return LCD_WIDTH;
}

uint16_t lcdDriverGetHeight(void)
{
  return LCD_HEIGHT;
}

void lcdDriverFillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  // Clipping
  if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return;

  if (x < 0) { w += x; x = 0; }
  if (y < 0) { h += y; y = 0; }

  if ((x + w) > LCD_WIDTH)  w = LCD_WIDTH  - x;
  if ((y + h) > LCD_HEIGHT) h = LCD_HEIGHT - y;

  if ((w < 1) || (h < 1)) return;

  lcdDriverSetWindow(x, y, x + w - 1, y + h - 1);
#if HW_LCD_FSMC
  for (int i=0; i<h*w; i++)
  {
      writecolor(color);
  }
#endif
#if HW_LCD_SPI
  uint16_t line_buf[w];

  spiSetBitWidth(LCD_SPI_CH, 16);

  gpioPinWrite(GPIO_LCD_DC, GPIO_ON);
  gpioPinWrite(GPIO_LCD_CS, GPIO_OFF);

  for (int i=0; i<w; i++)
  {
    line_buf[i] = color;
  }
  for (int i=0; i<h; i++)
  {
    spiDmaTxTransfer(_DEF_SPI1, (void *)line_buf, w, 10);
  }
  gpioPinWrite(GPIO_LCD_CS, GPIO_ON);
#endif
}


bool lcdDriverSendBuffer(uint16_t *p_data, uint32_t length, uint32_t timeout_ms)
{
  is_write_frame = true;

#if HW_LCD_FSMC
  for(int i=0; i<length; i++)
  {
    writecolor((uint16_t)p_data[i]);
  }
  TransferDoneISR();
#endif

#if HW_LCD_SPI
  spiSetBitWidth(LCD_SPI_CH, 16);

  gpioPinWrite(GPIO_LCD_DC, GPIO_ON);
  gpioPinWrite(GPIO_LCD_CS, GPIO_OFF);

  spiDmaTxTransfer(_DEF_SPI1, (void *)p_data, length, 0);
#endif

  return true;
}

bool lcdDriverSetCallBack(void (*p_func)(void))
{
  frameCallBack = p_func;

  return true;
}

void lcdDriverSetRotation(uint8_t mode)
{
  writecommand(LCD_REG_MADCTL);

  switch (mode)
  {
   case 0:
     writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     break;

   case 1:
     writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     break;

  case 2:
    writedata(MADCTL_BGR);
    break;

   case 3:
     writedata(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     break;
  }
}

void lcdDriverSetWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  int32_t sX, sY, eX, eY;

  sX = x0 + COL_OFFSET;
  sY = y0 + ROW_OFFSET;
  eX = x1 + COL_OFFSET;
  eY = y1 + ROW_OFFSET;

#if HW_LCD_SPI
  spiSetBitWidth(LCD_SPI_CH, 8);
#endif
  writecommand(LCD_REG_CASET);     // Column addr set
  writedata((uint8_t)(sX>>8));
  writedata((uint8_t)(sX&0xff));  // XSTART
  writedata((uint8_t)(eX>>8));
  writedata((uint8_t)(eX&0xff));  // XEND

  writecommand(LCD_REG_RASET);     // Row addr set
  writedata((uint8_t)(sY>>8));
  writedata((uint8_t)(sY&0xff));  // YSTART
  writedata((uint8_t)(eY>>8));
  writedata((uint8_t)(eY&0xff));  // YEND

  writecommand(LCD_REG_RAMWR);     // write to RAM
}


void lcdDriverInitRegs(void)
{
#ifdef _USE_HW_ST7735
  writecommand(LCD_REG_SWRESET); //  C: Software reset, 0 args, w/delay
  delay(200);

  writecommand(LCD_REG_SLPOUT);  //  C: Out of sleep mode, 0 args, w/delay
  delay(100);

  writecommand(LCD_REG_FRMCTR1); //  C: Frame rate ctrl - normal mode, 3 args:
  writedata(0x01);              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  writedata(0x2C);
  writedata(0x2D);

  writecommand(LCD_REG_FRMCTR2); //  C: Frame rate control - idle mode, 3 args:
  writedata(0x01);              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  writedata(0x2C);
  writedata(0x2D);

  writecommand(LCD_REG_FRMCTR3); //  C: Frame rate ctrl - partial mode, 6 args:
  writedata(0x01);              //     Dot inversion mode
  writedata(0x2C);
  writedata(0x2D);
  writedata(0x01);              //     Line inversion mode
  writedata(0x2C);
  writedata(0x2D);

  writecommand(LCD_REG_INVCTR);  //  C: Display inversion ctrl, 1 arg, no delay:
  writedata(0x07);              //     No inversion

  writecommand(LCD_REG_PWCTR1);  //  C: Power control, 3 args, no delay:
  writedata(0xA2);
  writedata(0x02);              //     -4.6V
  writedata(0x84);              //     AUTO mode

  writecommand(LCD_REG_PWCTR2);  //  C: Power control, 1 arg, no delay:
  writedata(0xC5);              //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

  writecommand(LCD_REG_PWCTR3);  //  C: Power control, 2 args, no delay:
  writedata(0x0A);              //     Opamp current small
  writedata(0x00);              //     Boost frequency

  writecommand(LCD_REG_PWCTR4);  //  C: Power control, 2 args, no delay:
  writedata(0x8A);              //     BCLK/2, Opamp current small & Medium low
  writedata(0x2A);

  writecommand(LCD_REG_PWCTR5);  //  C: Power control, 2 args, no delay:
  writedata(0x8A);
  writedata(0xEE);

  writecommand(LCD_REG_VMCTR1);  //  C: Power control, 1 arg, no delay:
  writedata(0x0E);

  writecommand(LCD_REG_INVON);   //  C: Don't invert display, no args, no delay

  writecommand(LCD_REG_MADCTL);  //  C: Memory access control (directions), 1 arg:
  writedata(0xC8);              //     row addr/col addr, bottom to top refresh

  writecommand(LCD_REG_COLMOD);  //  C: set color mode, 1 arg, no delay:
  writedata(0x05);              //     16-bit color

  writecommand(LCD_REG_CASET);   //  C: Column addr set, 4 args, no delay:
  writedata(0x00);
  writedata(0x00);              //     XSTART = 0
  writedata(0x00);
  writedata(LCD_WIDTH-1);    //     XEND = 159

  writecommand(LCD_REG_RASET);   //  C: Row addr set, 4 args, no delay:
  writedata(0x00);
  writedata(0x00);              //     XSTART = 0
  writedata(0x00);
  writedata(LCD_HEIGHT-1);   //     XEND = 127

#if HW_LCD_FSMC
  writecommand(LCD_REG_SWRESET); //  C: Software reset, 0 args, w/delay
  delay(200);

  writecommand(LCD_REG_SLPOUT);  //  C: Out of sleep mode, 0 args, w/delay
  delay(100);
#endif //#if HW_LCD_FSMC

  writecommand(LCD_REG_NORON);   //  3: Normal display on, no args, w/delay
  delay(10);

  writecommand(LCD_REG_DISPON);  //  4: Main screen turn on, no args w/delay
  delay(10);
#endif //#ifdef _USE_HW_ST7735

#ifdef _USE_HW_ILI9341
//  char gbuf[4];
//  int repeat =0;
//  gpioPinWrite(GPIO_LCD_NRESET, GPIO_OFF);
//  delay(1);
//  gpioPinWrite(GPIO_LCD_NRESET, GPIO_ON);
//  delay(10);
//  gpioPinWrite(GPIO_LCD_NRESET, GPIO_OFF);
//  delay(120);

//  while(gbuf[2] != 0x93)
//  {
//    writecommand(ILI9341_RDID4);
//    gbuf[0] = readdata();
//    gbuf[1] = readdata();
//    gbuf[2] = readdata();
//    gbuf[3] = readdata();
//    delay(100);
//
//    if( (repeat++) > 0x8ffff)
//      return 0;
//  }

  // Start Initial Sequence
  writecommand(LCD_REG_SLPOUT);   // Exit Sleep
  delay(100);
  writecommand(LCD_REG_PWCTRLA);//AP[2:0]
  writedata(0x01);

  writecommand(LCD_REG_PWCTR1);
  writedata(0x21);

  writecommand(LCD_REG_PWCTR2);
  writedata(0x11);

  writecommand(LCD_REG_VMCTR1);
  writedata(0x35);
  writedata(0x32);

  writecommand(LCD_REG_MADCTL);
  writedata(0x48);

  writecommand(LCD_REG_FRMCTR1);
  writedata(0x00);
  writedata(0x16);

  writecommand(LCD_REG_DISSET);
  writedata(0x0A);
  writedata(0x82);

  writecommand(LCD_REG_VMOFCTR);
  writedata(0xB5);
  writecommand(ILI9341_EN3G);
  writedata(0x00);
  writecommand(LCD_REG_GAMSET);
  writedata(0x01);
  writecommand(LCD_REG_COLMOD);
  writedata(0x55); /* set 16 bit MCU mode */
  writecommand(LCD_REG_MADCTL);
  writedata(0x68); //writedata(0xa8);

  writecommand(LCD_REG_DISSET);
  writedata(0x0a);
  writedata(0xa2);

  writecommand(LCD_REG_CASET);
  writedata(0x00);
  writedata(0x00);
  writedata(0x00);
  writedata(0xef);

  writecommand(LCD_REG_RASET);
  writedata(0x00);
  writedata(0x00);
  writedata(0x01);
  writedata(0x3f);

  writecommand(ILI9341_PGAMCTRL);
  writedata(0x1f);
  writedata(0x1a);
  writedata(0x18);
  writedata(0x0a);
  writedata(0x0f);
  writedata(0x06);
  writedata(0x45);
  writedata(0x87);
  writedata(0x32);
  writedata(0x0a);
  writedata(0x07);
  writedata(0x02);
  writedata(0x07);
  writedata(0x05);
  writedata(0x00);

  writecommand(ILI9341_NGAMCTRL);
  writedata(0x00);
  writedata(0x25);
  writedata(0x27);
  writedata(0x05);
  writedata(0x10);
  writedata(0x09);
  writedata(0x3A);
  writedata(0x78);
  writedata(0x4D);
  writedata(0x05);
  writedata(0x18);
  writedata(0x0d);
  writedata(0x38);
  writedata(0x3a);
  writedata(0x1f);

  writecommand(LCD_REG_DISPON);   // Exit Sleep
  writecommand(LCD_REG_DISPON);   // Exit Sleep

  writecommand(LCD_REG_PWCTRLB);
  writedata(0x00);
  writedata(0x81);
  writedata(0x30);

  writecommand(ILI9341_PWONSEQCTRL);
  writedata(0x64);
  writedata(0x03);
  writedata(0x12);
  writedata(0x81);

  writecommand(ILI9341_DRVTIMCTRLA1);
  writedata(0x85);
  writedata(0x10);
  writedata(0x7a);

  writecommand(LCD_REG_PWCTRLA);
  writedata(0x39);
  writedata(0x2c);
  writedata(0x00);
  writedata(0x34);
  writedata(0x02);

  writecommand(ILI9341_PUMPCTRL);
  writedata(0x20);

  writecommand(ILI9341_DRVTIMCTRLB);
  writedata(0x00);
  writedata(0x00);

  writecommand(LCD_REG_PWCTR1);
  writedata(0x21);

  writecommand(LCD_REG_PWCTR2);
  writedata(0x11);

  writecommand(LCD_REG_VMCTR1);
  writedata(0x3f);
  writedata(0x3c);

  writecommand(LCD_REG_VMOFCTR);
  writedata(0xa7);

  //LCD_RegWRW(0x36, 0x48);
  writecommand(LCD_REG_FRMCTR1);
  writedata(0x00);
  writedata(0x1b);

  writecommand(LCD_REG_DISSET);
  writedata(0x0a);
  writedata(0xa2);

  writecommand(ILI9341_EN3G);
  writedata(0x00);

  writecommand(LCD_REG_GAMSET);
  writedata(0x01);

  writecommand(ILI9341_PGAMCTRL);
  writedata(0x0f);
  writedata(0x23);
  writedata(0x1f);
  writedata(0x0b);
  writedata(0x0e);
  writedata(0x09);
  writedata(0x4b);
  writedata(0xa8);
  writedata(0x3b);
  writedata(0x0a);
  writedata(0x14);
  writedata(0x06);
  writedata(0x10);
  writedata(0x09);
  writedata(0x00);

  writecommand(ILI9341_NGAMCTRL);
  writedata(0x00);
  writedata(0x1c);
  writedata(0x20);
  writedata(0x04);
  writedata(0x10);
  writedata(0x08);
  writedata(0x34);
  writedata(0x47);
  writedata(0x44);
  writedata(0x05);
  writedata(0x0b);
  writedata(0x09);
  writedata(0x2f);
  writedata(0x36);
  writedata(0x0f);

  writecommand(LCD_REG_SLPOUT);   // Exit Sleep
  delay(100);
  writecommand(LCD_REG_DISPON);   // Exit Sleep
  writecommand(LCD_REG_RAMWR);   // Exit Sleep


//  //Start initial Sequence
//  writecommand(LCD_REG_SWRESET); //software reset
//  delay(5);
//  writecommand(LCD_REG_DISPOFF); // display off
//  //------------power control------------------------------
//  writecommand(LCD_REG_PWCTR1); //power control
//  writedata(0x26);
//  writecommand(LCD_REG_PWCTR2); //power control
//  writedata(0x11);
//  writecommand(LCD_REG_PWCTR5); //vcom control
//  writedata(0x5c);//35
//  writedata(0x4c);//3E
//  writecommand(LCD_REG_VMOFCTR); //vcom control
//  writedata(0x94);
//  //------------memory access control------------------------
//  writecommand(LCD_REG_MADCTL); // memory access control
//  writedata(0x48); //0048 my,mx,mv,ml,BGR,mh,0.0
//  writecommand(LCD_REG_COLMOD); // pixel format set
//  writedata(0x55);//16bit /pixel
//  //----------------- frame rate------------------------------
//  writecommand(LCD_REG_FRMCTR1); // frame rate
//  writedata(0x00);
//  writedata(0x1B); //70
//  //----------------Gamma---------------------------------
//  writecommand(ILI9341_EN3G); // 3Gamma Function Disable
//  writedata(0x08);
//  writecommand(LCD_REG_GAMSET);
//  writedata(0x01); // gamma set 4 gamma curve 01/02/04/08
//  writecommand(ILI9341_PGAMCTRL); //positive gamma correction
//  writedata(0x1f);
//  writedata(0x1a);
//  writedata(0x18);
//  writedata(0x0a);
//  writedata(0x0f);
//  writedata(0x06);
//  writedata(0x45);
//  writedata(0x87);
//  writedata(0x32);
//  writedata(0x0a);
//  writedata(0x07);
//  writedata(0x02);
//  writedata(0x07);
//  writedata(0x05);
//  writedata(0x00);
//  writecommand(ILI9341_NGAMCTRL); //negamma correction
//  writedata(0x00);
//  writedata(0x25);
//  writedata(0x27);
//  writedata(0x05);
//  writedata(0x10);
//  writedata(0x09);
//  writedata(0x3a);
//  writedata(0x78);
//  writedata(0x4d);
//  writedata(0x05);
//  writedata(0x18);
//  writedata(0x0d);
//  writedata(0x38);
//  writedata(0x3a);
//  writedata(0x1f);
//  //--------------ddram ---------------------
//  writecommand(LCD_REG_CASET); // column set
//  writedata(0x00);
//  writedata(0x00);
//  writedata(0x00);
//  writedata(0xEF);
//  writecommand(LCD_REG_RASET); // page address set
//  writedata(0x00);
//  writedata(0x00);
//  writedata(0x01);
//  writedata(0x3F);
//  //writecommand(LCD_REG_TEOFF); // tearing effect off
//  //writecommand(LCD_REG_TEON); // tearing effect on
//  //writecommand(LCD_REG_INVCTR); // display inversion
//  //writedata(0x00);
//  writecommand(LCD_REG_ETMOD); //entry mode set
//  writedata(0x07);
//  //-----------------display---------------------
//  writecommand(LCD_REG_DISSET); // display function control
//  writedata(0x0a);
//  writedata(0x82);
//  writedata(0x27);
//  writedata(0x00);
//  writecommand(LCD_REG_SLPOUT); //sleep out
//  delay(100);
//  writecommand(LCD_REG_DISPON); // display on
//  delay(100);
//  writecommand(LCD_REG_RAMWR); //memory write
//  delay(200);
#endif //#ifdef _USE_HW_ILI9341
}

#endif //#ifdef _USE_HW_LCD
