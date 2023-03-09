/*
 * lcd_mid.h
 *
 *  Created on: 2023. 3. 5.
 *      Author: KSK
 */

#ifndef DISPLAY_LCD_INC_LCD_MID_H_
#define DISPLAY_LCD_INC_LCD_MID_H_


#include "hw_def.h"
#include "fonts.h"


#ifdef _USE_HW_LCD

enum class_color
{
 white              = 0xFFFF,
 transp_white       = 0xFFDF, //skkim
 gray               = 0x8410,
 darkgray           = 0xAD55,
 tuxedo_gray        = 0x52A9, //skkim
 neutral_dark_gray  = 0x4a69,
 black              = 0x0000,
 purple             = 0x8010,
 pink               = 0xFE19,
 red                = 0xF800,
 coral_red          = 0xF9E7, //skkim
 coral_reef         = 0xFBCF, //skkim
 tulip              = 0xFDB6, //skkim
 pastel_red         = 0xFB2C, //skkim
 vermillion         = 0xE206, //skkim
 orange             = 0xFD20,
 brown              = 0xA145,
 beige              = 0xF7BB,
 yellow             = 0xFFE0,
 lightgreen         = 0x9772,
 green              = 0x07E0,
 darkblue           = 0x0011,
 blue               = 0x001F,

 lightblue          = 0xAEDC,
 aztec_blue         = 0x014E, //skkim
 hyundai_blue       = 0x014F, //skkim
 petrol_blue        = 0x0AAF, //skkim
 infantry_blue      = 0x328D, //skkim
};

typedef enum
{
  transp_off,
  transp_on,
}lcd_background_transparancy_e;


typedef struct
{
  bool     (*init)(void);
  bool     (*reset)(void);
  void     (*setWindow)(int32_t x, int32_t y, int32_t w, int32_t h);
  uint16_t (*getWidth)(void);
  uint16_t (*getHeight)(void);
  bool     (*setCallBack)(void (*p_func)(void));
  bool     (*sendBuffer)(uint16_t *p_data, uint32_t length, uint32_t timeout_ms);

} lcd_driver_t;


/*
 * font : font Structure
 * str : string Array
 * posx : x-coordinate(LCD Left-TOP Start)
 * posy : y-coordinate(LCD Left-TOP Start)
 * tcol : text color
 * bcol : text background color
 * btransp : Background Transparent(0:bColor, 1:Transparent)
 * tAlign : String Align(LEFT, CENTER, RIGHT)
 */
typedef struct{
  lcd_font_t  *font;
  const char  *str;
  uint16_t     posx;
  uint16_t     posy;
  uint32_t    tcol;
  uint32_t    bcol;
  bool        btransp;
  uint8_t     tAlign;
}lcd_string_t;


typedef enum
{
  LEFT = 0,
  CENTER,    /* center mode */
  RIGHT,    /* left mode   */
}lcd_str_align_e;


bool      lcdInit(void);
bool      lcdIsInit(void);
void      lcdReset(void);

uint8_t   lcdGetBackLight(void);
void      lcdSetBackLight(uint8_t value);

void      lcdClear(uint32_t rgb_code);
void      lcdClearBuffer(uint32_t rgb_code);

bool      lcdDrawAvailable(void);
bool      lcdRequestDraw(void);
void      lcdUpdateDraw(void);
void      lcdSetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
//void      lcdSendBuffer(uint8_t *p_data, uint32_t length, uint32_t timeout_ms);
void      lcdDisplayOff(void);
void      lcdDisplayOn(void);

uint32_t  lcdGetFps(void);
uint32_t  lcdGetFpsTime(void);
uint32_t  lcdGetDrawTime(void);

int32_t   lcdGetWidth(void);
int32_t   lcdGetHeight(void);

uint16_t  *lcdGetFrameBuffer(void);
uint16_t  *lcdGetCurrentFrameBuffer(void);
//void      lcdSetDoubleBuffer(bool enable);

uint32_t  lcdReadPixel(uint16_t x_pos, uint16_t y_pos);
void      lcdDrawPixel(uint16_t x_pos, uint16_t y_pos, uint32_t rgb_code);
//void      lcdDrawPixelMix(uint16_t x_pos, uint16_t y_pos, uint32_t rgb_code, uint8_t mix);
void      lcdDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void      lcdDrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void      lcdDrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

void      lcdDrawFillScreen(uint16_t color);

void      lcdDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void      lcdDrawFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void      lcdDrawFillCircle(int32_t x0, int32_t y0, int32_t r, uint16_t color);
void      lcdDrawCircleHelper( int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint32_t color);
void      lcdDrawFillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color);

void      lcdDrawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color);
void      lcdDrawFillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color);

void      lcdDrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);
void      lcdDrawFillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);

//void      lcdDrawString(int x, int y, uint16_t color, const char *str);
void      lcdPrintf(int x, int y, uint16_t color,  const char *fmt, ...);

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ [ skkim] ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void      lcdDrawImage_1(lcd_image_t *img);  // skkim-lcd
void      lcdDrawImage_2(lcd_image_t *img, bool transp_bg, uint16_t x_init, uint16_t y_init);  //skkim-lcd
void      lcdDrawBackupImage(lcd_image_t *img, bool transp_bg, uint16_t x_init, uint16_t y_init);  //skkim-lcd
void      lcdBackupImage(lcd_image_t *img, uint16_t x_init, uint16_t y_init);  //skkim-lcd
bool      lcdDrawImage_Move(lcd_image_t *img, bool transp_bg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool rtn, uint32_t time_ms);  //skkim-lcd
bool      lcdDrawImage_GradAtoB(lcd_image_t *img, bool transp_bg, uint16_t target_color, uint16_t a_color, uint16_t b_color, bool rtn, uint32_t time_ms); //skkim-lcd
uint8_t   lcdDrawChar(lcd_font_t *sfonts, uint16_t cX, uint16_t cY, unsigned char c, uint32_t txColor, uint32_t bgColor, bool bgTransparent); //skkim-lcd
uint16_t  lcdDrawString(lcd_string_t *string);  //skkim-lcd
void      lprintf(lcd_font_t *sfonts, uint16_t aX, uint16_t aY, uint32_t txColor, uint32_t bgColor, bool bgTransparent, const char *format, ...); //skkim-lcd


#endif /* _USE_HW_LCD */


#endif /* DISPLAY_LCD_INC_LCD_MID_H_ */
