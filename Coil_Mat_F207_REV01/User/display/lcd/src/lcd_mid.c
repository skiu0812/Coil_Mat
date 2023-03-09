/*
 * lcd_mid.c
 *
 *  Created on: 2023. 3. 5.
 *      Author: KSK
 */


#include "user.h"


#ifdef _USE_HW_LCD

#include "gpio.h"
#include "han.h"
#include "images.h"


#define LCD_WIDTH           HW_LCD_WIDTH
#define LCD_HEIGHT          HW_LCD_HEIGHT

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define MAKECOL(r, g, b) ( ((r)<<11) | ((g)<<5) | (b))

#define LCD_OPT_DEF   __attribute__((optimize("O2")))

#define GETR(c) (((uint16_t)(c)) >> 11)
#define GETG(c) (((c) & 0x07E0)>>5)
#define GETB(c) ((c) & 0x1F)
#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))


typedef struct
{
  int16_t x;
  int16_t y;
} lcd_pixel_t;


static lcd_driver_t lcd;

static bool is_init = false;
static volatile bool is_tx_done = true;
static uint8_t backlight_value = 100;
static uint8_t frame_index = 0;

static bool lcd_request_draw = false;

static volatile uint32_t fps_pre_time;
static volatile uint32_t fps_time;
static volatile uint32_t fps_count = 0;

static volatile uint32_t draw_fps = 30;
static volatile uint32_t draw_frame_time = 0;

static uint16_t *p_draw_frame_buf = NULL;
static uint16_t __attribute__((aligned(64))) frame_buffer[1][LCD_WIDTH * LCD_HEIGHT];

static volatile bool requested_from_thread = false;


static void disHanFont(int x, int y, han_font_t *FontPtr, uint16_t textcolor);
static void lcdDrawLineBuffer(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, lcd_pixel_t *line);


#ifdef _USE_HW_CLI
static void cliLcd(cli_args_t *args);
#endif


void TransferDoneISR(void)
{
  fps_time = millis() - fps_pre_time;
  fps_pre_time = millis();

  if (fps_time > 0)
  {
    fps_count = 1000 / fps_time;
  }

  lcd_request_draw = false;
}


bool lcdInit(void)
{
  backlight_value = 100;

  lcdDriverInit();
  lcdDriverFuncSet(&lcd);

  lcd.setCallBack(TransferDoneISR);


  for (int i=0; i<LCD_WIDTH*LCD_HEIGHT; i++)
  {
    frame_buffer[0][i] = black;
  }
  memset(frame_buffer, 0x00, sizeof(frame_buffer));

  p_draw_frame_buf = frame_buffer[frame_index];

  lcdDrawFillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, black);
  lcdUpdateDraw();
  delay(100);

  lcdSetBackLight(100);

  is_init = true;

#ifdef _USE_HW_CLI
  cliAdd("lcd", cliLcd);
#endif

  return true;
}


bool lcdIsInit(void)
{
  return is_init;
}


void lcdReset(void)
{
  lcd.reset();
}


uint8_t lcdGetBackLight(void)
{
  return backlight_value;
}


void lcdSetBackLight(uint8_t value)
{
  value = constrain(value, 0, 100);

  if (value != backlight_value)
  {
    backlight_value = value;
  }

  if (backlight_value > 0)
  {
    gpioPinWrite(GPIO_LCD_BL, GPIO_ON);
  }
  else
  {
    gpioPinWrite(GPIO_LCD_BL, GPIO_OFF);
  }
}


LCD_OPT_DEF void lcdClear(uint32_t rgb_code)
{
  lcdClearBuffer(rgb_code);

  lcdUpdateDraw();
}


LCD_OPT_DEF void lcdClearBuffer(uint32_t rgb_code)
{
  uint16_t *p_buf = lcdGetFrameBuffer();

  for (int i=0; i<LCD_WIDTH * LCD_HEIGHT; i++)
  {
    p_buf[i] = rgb_code;
  }
}


bool lcdDrawAvailable(void)
{
  return !lcd_request_draw;
}


bool lcdRequestDraw(void)
{
  if (lcd_request_draw == true)
  {
    return false;
  }

  lcd.setWindow(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);

  lcd_request_draw = true;
  lcd.sendBuffer((uint16_t *)frame_buffer[frame_index], LCD_WIDTH * LCD_HEIGHT, 0);

  return true;
}


void lcdUpdateDraw(void)
{
  lcdRequestDraw();
  while(lcdDrawAvailable() != true)
  {
    delay(1);
  }
}


void lcdSetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  lcd.setWindow(x, y, w, h);
}


void lcdDisplayOff(void)
{
}


void lcdDisplayOn(void)
{
  lcdSetBackLight(lcdGetBackLight());
}


uint32_t lcdGetFps(void)
{
  return fps_count;
}


uint32_t lcdGetFpsTime(void)
{
  return fps_time;
}


uint32_t lcdGetDrawTime(void)
{
  return draw_frame_time;
}


int32_t lcdGetWidth(void)
{
  return LCD_WIDTH;
}


int32_t lcdGetHeight(void)
{
  return LCD_HEIGHT;
}


uint16_t *lcdGetFrameBuffer(void)
{
  return (uint16_t *)p_draw_frame_buf;
}


uint16_t *lcdGetCurrentFrameBuffer(void)
{
  return (uint16_t *)frame_buffer[frame_index];
}


LCD_OPT_DEF uint32_t lcdReadPixel(uint16_t x_pos, uint16_t y_pos)
{
  return p_draw_frame_buf[y_pos * LCD_WIDTH + x_pos];
}


LCD_OPT_DEF void lcdDrawPixel(uint16_t x_pos, uint16_t y_pos, uint32_t rgb_code)
{
  p_draw_frame_buf[y_pos * LCD_WIDTH + x_pos] = rgb_code;
}


void lcdDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);

  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 < 0) x1 = 0;
  if (y1 < 0) y1 = 0;


  if (steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++)
  {
    if (steep)
    {
      lcdDrawPixel(y0, x0, color);
    } else
    {
      lcdDrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}


void lcdDrawLineBuffer(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, lcd_pixel_t *line)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);

  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 < 0) x1 = 0;
  if (y1 < 0) y1 = 0;


  if (steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++)
  {
    if (steep)
    {
      if (line != NULL)
      {
        line->x = y0;
        line->y = x0;
      }
      lcdDrawPixel(y0, x0, color);
    } else
    {
      if (line != NULL)
      {
        line->x = x0;
        line->y = y0;
      }
      lcdDrawPixel(x0, y0, color);
    }
    if (line != NULL)
    {
      line++;
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}


void lcdDrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  lcdDrawLine(x, y, x, y+h-1, color);
}


void lcdDrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  lcdDrawLine(x, y, x+w-1, y, color);
}


void lcdDrawFillScreen(uint16_t color)
{
  lcdDrawFillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}


void lcdDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  lcdDrawHLine(x, y, w, color);
  lcdDrawHLine(x, y+h-1, w, color);
  lcdDrawVLine(x, y, h, color);
  lcdDrawVLine(x+w-1, y, h, color);
}


void lcdDrawFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  for (int16_t i=x; i<x+w; i++)
  {
    lcdDrawVLine(i, y, h, color);
  }
}


LCD_OPT_DEF void lcdDrawFillCircle(int32_t x0, int32_t y0, int32_t r, uint16_t color)
{
  int32_t  x  = 0;
  int32_t  dx = 1;
  int32_t  dy = r+r;
  int32_t  p  = -(r>>1);


  lcdDrawHLine(x0 - r, y0, dy+1, color);

  while(x<r)
  {

    if(p>=0) {
      dy-=2;
      p-=dy;
      r--;
    }

    dx+=2;
    p+=dx;

    x++;

    lcdDrawHLine(x0 - r, y0 + x, 2 * r+1, color);
    lcdDrawHLine(x0 - r, y0 - x, 2 * r+1, color);
    lcdDrawHLine(x0 - x, y0 + r, 2 * x+1, color);
    lcdDrawHLine(x0 - x, y0 - r, 2 * x+1, color);
  }
}


LCD_OPT_DEF void lcdDrawCircleHelper( int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint32_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * r;
  int32_t x     = 0;

  while (x < r)
  {
    if (f >= 0)
    {
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4)
    {
      lcdDrawPixel(x0 + x, y0 + r, color);
      lcdDrawPixel(x0 + r, y0 + x, color);
    }
    if (cornername & 0x2)
    {
      lcdDrawPixel(x0 + x, y0 - r, color);
      lcdDrawPixel(x0 + r, y0 - x, color);
    }
    if (cornername & 0x8)
    {
      lcdDrawPixel(x0 - r, y0 + x, color);
      lcdDrawPixel(x0 - x, y0 + r, color);
    }
    if (cornername & 0x1)
    {
      lcdDrawPixel(x0 - r, y0 - x, color);
      lcdDrawPixel(x0 - x, y0 - r, color);
    }
  }
}


LCD_OPT_DEF void lcdDrawFillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -r - r;
  int32_t y     = 0;

  delta++;

  while (y < r)
  {
    if (f >= 0)
    {
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }

    y++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1)
    {
      lcdDrawHLine(x0 - r, y0 + y, r + r + delta, color);
      lcdDrawHLine(x0 - y, y0 + r, y + y + delta, color);
    }
    if (cornername & 0x2)
    {
      lcdDrawHLine(x0 - r, y0 - y, r + r + delta, color); // 11995, 1090
      lcdDrawHLine(x0 - y, y0 - r, y + y + delta, color);
    }
  }
}


LCD_OPT_DEF void lcdDrawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
  // smarter version
  lcdDrawHLine(x + r    , y        , w - r - r, color); // Top
  lcdDrawHLine(x + r    , y + h - 1, w - r - r, color); // Bottom
  lcdDrawVLine(x        , y + r    , h - r - r, color); // Left
  lcdDrawVLine(x + w - 1, y + r    , h - r - r, color); // Right

  // draw four corners
  lcdDrawCircleHelper(x + r        , y + r        , r, 1, color);
  lcdDrawCircleHelper(x + w - r - 1, y + r        , r, 2, color);
  lcdDrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  lcdDrawCircleHelper(x + r        , y + h - r - 1, r, 8, color);
}


LCD_OPT_DEF void lcdDrawFillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
  // smarter version
  lcdDrawFillRect(x, y + r, w, h - r - r, color);

  // draw four corners
  lcdDrawFillCircleHelper(x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
  lcdDrawFillCircleHelper(x + r, y + r        , r, 2, w - r - r - 1, color);
}


LCD_OPT_DEF void lcdDrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color)
{
  lcdDrawLine(x1, y1, x2, y2, color);
  lcdDrawLine(x1, y1, x3, y3, color);
  lcdDrawLine(x2, y2, x3, y3, color);
}


LCD_OPT_DEF void lcdDrawFillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color)
{
  uint16_t max_line_size_12 = max(abs(x1-x2), abs(y1-y2));
  uint16_t max_line_size_13 = max(abs(x1-x3), abs(y1-y3));
  uint16_t max_line_size_23 = max(abs(x2-x3), abs(y2-y3));
  uint16_t max_line_size = max_line_size_12;
  uint16_t i = 0;

  if (max_line_size_13 > max_line_size)
  {
    max_line_size = max_line_size_13;
  }
  if (max_line_size_23 > max_line_size)
  {
    max_line_size = max_line_size_23;
  }

  lcd_pixel_t line[max_line_size];

  lcdDrawLineBuffer(x1, y1, x2, y2, color, line);
  for (i = 0; i < max_line_size_12; i++)
  {
    lcdDrawLine(x3, y3, line[i].x, line[i].y, color);
  }
  lcdDrawLineBuffer(x1, y1, x3, y3, color, line);
  for (i = 0; i < max_line_size_13; i++)
  {
    lcdDrawLine(x2, y2, line[i].x, line[i].y, color);
  }
  lcdDrawLineBuffer(x2, y2, x3, y3, color, line);
  for (i = 0; i < max_line_size_23; i++)
  {
    lcdDrawLine(x1, y1, line[i].x, line[i].y, color);
  }
}


void lcdPrintf(int x, int y, uint16_t color, const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];
  int Size_Char;
  int i, x_Pre = x;
  han_font_t FontBuf;
  uint8_t font_width;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  for( i=0; i<len; i+=Size_Char )
  {
    hanFontLoad( &print_buffer[i], &FontBuf );

    disHanFont( x, y, &FontBuf, color);

    Size_Char = FontBuf.Size_Char;
    if (Size_Char >= 2)
    {
      font_width = 16;
      x += 2*8;
    }
    else
    {
      font_width = 8;
      x += 1*8;
    }

    if ((x+font_width) > LCD_WIDTH)
    {
      x  = x_Pre;
      y += 16;
    }

    if( FontBuf.Code_Type == PHAN_END_CODE ) break;
  }
}


uint32_t lcdGetStrWidth(const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];
  int Size_Char;
  int i;
  han_font_t FontBuf;
  uint32_t str_len;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  str_len = 0;

  for( i=0; i<len; i+=Size_Char )
  {
    hanFontLoad( &print_buffer[i], &FontBuf );

    Size_Char = FontBuf.Size_Char;

    str_len += (Size_Char * 8);

    if( FontBuf.Code_Type == PHAN_END_CODE ) break;
  }

  return str_len;
}


void disHanFont(int x, int y, han_font_t *FontPtr, uint16_t textcolor)
{
  uint16_t    i, j, Loop;
  uint16_t  FontSize = FontPtr->Size_Char;
  uint16_t index_x;

  if (FontSize > 2)
  {
    FontSize = 2;
  }

  for ( i = 0 ; i < 16 ; i++ )        // 16 Lines per Font/Char
  {
    index_x = 0;
    for ( j = 0 ; j < FontSize ; j++ )      // 16 x 16 (2 Bytes)
    {
      uint8_t font_data;

      font_data = FontPtr->FontBuffer[i*FontSize +j];

      for( Loop=0; Loop<8; Loop++ )
      {
        if( (font_data<<Loop) & (0x80))
        {
          lcdDrawPixel(x + index_x, y + i, textcolor);
        }
        index_x++;
      }
    }
  }
}


//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ [ skkim ] ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//

LCD_OPT_DEF void lcdDrawImage_1(lcd_image_t *img)  // skkim-lcd
{
  uint16_t x_pos, y_pos;

  for(uint16_t y=0; y<(img->height); y++)
  {
    for(uint16_t x=0; x<(img->width); x++)
    {
      x_pos = (x + img->x_init);
      y_pos = (y + img->y_init);

      if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        if(img->tranp_bg == transp_off)
        {
          lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
        else if(img->tranp_bg == transp_on)
        {
          if(img->img[y * img->width + x] != img->tr_color)
            lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }

//        if(img->img[y * img->width + x] != img->tr_color)
          lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
      }
    }
  }
}


LCD_OPT_DEF void lcdDrawImage_2(lcd_image_t *img, bool transp_bg, uint16_t x_init, uint16_t y_init)  // skkim-lcd
{
  uint16_t x_pos, y_pos;

  for(uint16_t y=0; y<(img->height); y++)
  {
    for(uint16_t x=0; x<(img->width); x++)
    {
      x_pos = (x + x_init);
      y_pos = (y + y_init);

      if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        if(transp_bg == transp_off)
        {
          lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
        else if(transp_bg == transp_on)
        {
          if(img->img[y * img->width + x] != img->tr_color)
            lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
      }
    }
  }
}


LCD_OPT_DEF void lcdDrawBackupImage(lcd_image_t *img, bool transp_bg, uint16_t x_init, uint16_t y_init)  // skkim-lcd
{
  uint16_t x_pos, y_pos;

  if(img->is_backup == true)
  {
    for(uint16_t y=0; y<(img->height); y++)
    {
      for(uint16_t x=0; x<(img->width); x++)
      {
        x_pos = (x + x_init);
        y_pos = (y + y_init);

        if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
        {
          if(transp_bg == transp_off)
          {
            lcdDrawPixel(x_pos, y_pos, img->img_bk_buf[y * img->width + x]);
          }
          else if(transp_bg == transp_on)
          {
            if(img->img[y * img->width + x] != img->tr_color)
              lcdDrawPixel(x_pos, y_pos, img->img_bk_buf[y * img->width + x]);
          }
        }

      }//for(uint16_t x=0; x<(img->width); x++)
    }//for(uint16_t y=0; y<(img->height); y++)
  }//if(img->is_backup == true)

  img->is_backup = false;
}


LCD_OPT_DEF void lcdBackupImage(lcd_image_t *img, uint16_t x_init, uint16_t y_init)  // skkim-lcd
{
  uint16_t x_pos = 0;
  uint16_t y_pos = 0;

  for(uint16_t y=0; y<(img->height); y++)
  {
    for(uint16_t x=0; x<(img->width); x++)
    {
      x_pos = (x + x_init);
      y_pos = (y + y_init);

      if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        img->img_bk_buf[y * img->width + x] = lcdReadPixel(x_pos, y_pos);
      }
    }
  }

  img->is_backup =  true;
}


/*
 * @ description  : 이미지를 x_s, y_s 지점에서 시작해서 x_e, y_e 지점 까지
 *                  time_ms의 시간 동안 이동시킴
 * img            : 출력 이미지
 * transp_bg      : 투명으로 설정 할 색
 * x_s, y_s       : start point
 * x_s, y_s       : end point
 * rtn            : return(왕복여부) - 0:oneway, 1:return
 * time_ms        : 편도 동작 시간 - rtn=1이면 전체 동작 시간이 설정 시간의 2배가 됨.
 */
LCD_OPT_DEF bool lcdDrawImage_Move(lcd_image_t *img, bool transp_bg,
                                   uint16_t x_s, uint16_t y_s,
                                   uint16_t x_e, uint16_t y_e,
                                   bool rtn, uint32_t time_ms)  // skkim-lcd
{
  bool ret = false;
  uint16_t x_pos, y_pos;

  static bool       no_repeat;
  static bool       direction; // 0:forward, 1:reverse
  static uint16_t   x_init, y_init, pre_x_init, pre_y_init;
  static uint16_t   cnt, cnt_max;
  static float      dx, dy;

  if(no_repeat == 0)
  {
    if(cnt == 0)
    {
      cnt_max = (float)time_ms/33;
      dx = ((float)(x_e - x_s)/(float)cnt_max);
      dy = ((float)(y_e - y_s)/(float)cnt_max);
    }

    if(cnt < cnt_max-1)
    {
      x_init = (uint16_t)(((float)x_s + (dx*cnt))+0.5);
      y_init = (uint16_t)(((float)y_s + (dy*cnt))+0.5);
    }
    else
    {
      x_init = x_e;
      y_init = y_e;
    }

    pre_x_init = x_init;
    pre_y_init = y_init;

    no_repeat = 1;
  }

  lcdBackupImage(img, x_init, y_init);

  for(uint16_t y=0; y<(img->height); y++)
  {
    for(uint16_t x=0; x<(img->width); x++)
    {
      x_pos = (x + x_init);
      y_pos = (y + y_init);

      if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        if(transp_bg == transp_off)
        {
          lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
        else if(transp_bg == transp_on)
        {
          if(img->img[y * img->width + x] != img->tr_color)
            lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
      }
    }
  }

  if(msWait("lcdDrawImage_Move", 0, 33) == true)
  {
    if(direction == 0)
    {
      if(cnt == cnt_max-1)
      {
        if(rtn == 0)
        {
          dx = 0;
          dy = 0;
          cnt = 0;
          cnt_max = 0;
          direction = 0;
          ret = true;
          lcdDrawBackupImage(img, transp_off, pre_x_init, pre_y_init);
        }
        else if(rtn == 1)
        {
          direction = 1;
        }
      }
      else
      {
        cnt++;
      }
    }
    else if(direction == 1)
    {
      if(cnt == 0)
      {
        if(rtn == 1)
        {
          dx = 0;
          dy = 0;
          cnt = 0;
          cnt_max = 0;
          direction = 0;
          ret = true;
        }
      }
      else
      {
        cnt--;
      }
    }

    if(ret == true)
    {
//      lcdDrawBackupImage(img, transp_off, pre_x_init, pre_y_init);
    }

    no_repeat = 0;
  }

  return ret;
}


/*
 * @ description  : 이미지에서 target_color로 지정된 색을 s_color에서 시작해서
 *                  e_color로 time_ms 시간 동안 변화시킴
 * img            : 출력 이미지
 * transp_bg      : 배경 투명 여부
 * target_color   : 투명으로 설정 할 배경 색
 * s_color        : start color
 * b_color        : end color
 * rtn            : return(왕복여부) - 0:oneway, 1:return
 * time_ms        : 편도 동작 시간 - rtn=1이면 전체 동작 시간이 설정 시간의 2배가 됨.
 */
LCD_OPT_DEF bool lcdDrawImage_GradAtoB(lcd_image_t *img, bool transp_bg,
                                       uint16_t target_color,
                                       uint16_t s_color, uint16_t e_color,
                                       bool rtn, uint32_t time_ms)  // skkim-lcd
{
         bool       ret = false;
  static bool       direction; // 0:forward, 1:reverse
  static bool       no_repeat=0;
  static uint16_t   cnt, cnt_max;
  static uint16_t   s_red, s_green, s_blue;
         uint16_t   e_red, e_green, e_blue;
         int        red_diff, green_diff, blue_diff;
  static uint16_t   grad_red, grad_green, grad_blue, grad_color;
  static float      dr, dg, db;
  static uint16_t   x_pos, y_pos;

  if(no_repeat == 0)
  {
    if(cnt == 0)
    {
      cnt_max = (uint8_t)((float)time_ms / 33);

      s_red   = (s_color & 0xf800) >> 8;
      s_green = (s_color & 0x07e0) >> 3;
      s_blue  = (s_color & 0x001f) << 3;

      e_red   = (e_color & 0xf800) >> 8;
      e_green = (e_color & 0x07e0) >> 3;
      e_blue  = (e_color & 0x001f) << 3;

      red_diff    = s_red   - e_red;
      green_diff  = s_green - e_green;
      blue_diff   = s_blue  - e_blue;

      dr = ((float)red_diff   / (float)cnt_max);
      dg = ((float)green_diff / (float)cnt_max);
      db = ((float)blue_diff  / (float)cnt_max);
    }

    if(cnt < cnt_max-1)
    {
      grad_red   = (uint16_t)(((float)s_red   - (dr * cnt)) + 0.5);
      grad_green = (uint16_t)(((float)s_green - (dg * cnt)) + 0.5);
      grad_blue  = (uint16_t)(((float)s_blue  - (db * cnt)) + 0.5);

      grad_color = ((grad_red << 8) & 0xf800) | ((grad_green << 3) & 0x07e0) | ((grad_blue >> 3) & 0x001f);
    }
    else
    {
      grad_color = e_color;
    }

    no_repeat = 1;
  }

  for(uint16_t y=0; y<(img->height); y++)
  {
    for(uint16_t x=0; x<(img->width); x++)
    {
      x_pos = (x + img->x_init);
      y_pos = (y + img->y_init);

      if(x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        if(transp_bg == transp_off)
        {
          if(img->img[y * img->width + x] == target_color)
            lcdDrawPixel(x_pos, y_pos, grad_color);
          else
            lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
        }
        else if(transp_bg == transp_on)
        {
          if(img->img[y * img->width + x] != img->tr_color)
          {
            if(img->img[y * img->width + x] == target_color)
              lcdDrawPixel(x_pos, y_pos, grad_color);
            else
              lcdDrawPixel(x_pos, y_pos, img->img[y * img->width + x]);
          }
        }
      }
    }
  }

  if(msWait("lcdDrawImage_GradAtoB", 0, 33) == true)
  {
    if(direction == 0)
    {
      if(cnt == cnt_max-1)
      {
        if(rtn == 0)
        {
          cnt = 0;
          cnt_max = 0;
          direction = 0;
          ret = true;
        }
        else if(rtn == 1)
        {
          direction = 1;
        }
      }
      else
      {
        cnt++;
      }
    }
    else if(direction == 1)
    {
      if(cnt == 0)
      {
        if(rtn == 1)
        {
          cnt = 0;
          cnt_max = 0;
          direction = 0;
          ret = true;
        }
      }
      else
      {
        cnt--;
      }
    }

    no_repeat = 0;
  }

  return ret;
}


/*
 * sfonts : Font Structure
 * aX : x-coordinate(LCD Left-TOP Start)
 * aY : y-coordinate(LCD Left-TOP Start)
 * c : Text
 * tColor : Text Color
 * bColro : Background Color
 * bTransparent  : Background Transparent(0:bColor, 1:Transparent)
 * fontSpace : space between letters
 */
uint8_t lcdDrawChar(lcd_font_t *sfonts, uint16_t cX, uint16_t cY, unsigned char c, uint32_t txColor, uint32_t bgColor, bool bgTransparent) // skkim-lcd
{
  uint8_t fontWidth, fontHeight, fNum, fontBit, aLineArr_N, fsFront, fsRear, fontSpace;
  uint16_t x, y, pX, pY, n, x_width, fOffset, cnt;

  pX = cX;
  pY = cY;

  fNum = c-32;

  fontBit = sfonts->fontBit;
  fontWidth = sfonts->fcInfo[fNum].fontWidth;
  fontHeight = sfonts->fcInfo[fNum].fontHeight;
  fOffset = sfonts->fcInfo[fNum].fontOffset;

  fontSpace = sfonts->fontSpace;

  cnt = fOffset;

  aLineArr_N = ceil((float)fontWidth/fontBit);  // font 1 line의 배열 원소 수

  if(fontSpace%2)
  {
    fsRear = fontSpace/2;
    fsFront = fsRear+1;
  }
  else
  {
    fsFront = fontSpace/2;
    fsRear = fsFront;
  }

  //front space
  for(y=0; y<fontHeight; y++)
  {
    if(pY<LCD_HEIGHT)
    {
      pX = cX;

      for(x=0; x<fsFront; x++)
      {
        if(pX<LCD_WIDTH)
        {
          if(!bgTransparent) lcdDrawPixel(pX, pY, bgColor);
          pX++;
        }
      }
      pY++;
    }
  }

  pY = cY;

  //char
  for(y=0; y<fontHeight; y++)
  {
    if(pY<LCD_HEIGHT)
    {
      pX = cX + fsFront;

      for(n=0; n<aLineArr_N; n++)
      {
        if(n < aLineArr_N-1)
        {
          x_width = fontBit;

        }
        else
        {
          x_width = (fontWidth % fontBit);
          if(x_width==0) x_width = fontBit;
        }

        for(x=0; x<x_width; x++)
        {
          if(pX<LCD_WIDTH)
          {
            if((sfonts->fontArr[cnt]>>(fontBit-1-x)) & 1)
            {
              lcdDrawPixel(pX, pY, txColor);
            }
            else
            {
              if(!bgTransparent)
                lcdDrawPixel(pX, pY, bgColor);
            }
            pX++;
          }
        }
        cnt++;
      }
      pY++;
    }
  }

  pY = cY;

  //rear space
  for(y=0; y<fontHeight; y++)
  {
    if(pY<LCD_HEIGHT)
    {
      pX = cX+fsFront+fontWidth;

      for(x=0; x<fsRear; x++)
      {
        if(pX<LCD_WIDTH)
        {
          if(!bgTransparent) lcdDrawPixel(pX, pY, bgColor);
          pX++;
        }
      }
      pY++;
    }
  }

  return (fontWidth + fontSpace);
}


uint16_t lcdDrawString(lcd_string_t *string)  //skkim-lcd
{
  uint16_t i, sX, sY, cX, size, stringWidth=0;
  uint8_t fontWidth, fontHeight, fontWidth_old, fontSpace, charWidth, strWidth=0, fNum, r_flag=0;//, fNum_old;

  size = strlen(string->str);

  if(string->tAlign == CENTER)
  {
    fontSpace = string->font->fontSpace;

    for(i=0; i<size; i++)
    {
      fNum = string->str[i]-32;
      strWidth = strWidth + string->font->fcInfo[fNum].fontWidth + fontSpace;
    }

    cX = (LCD_WIDTH - strWidth)/2;

  }
  else if(string->tAlign == RIGHT)
  {
    fontSpace = string->font->fontSpace;

    for(i=0; i<size; i++)
    {
      fNum = string->str[i]-32;
      strWidth = strWidth + string->font->fcInfo[fNum].fontWidth + fontSpace;
    }

    cX = LCD_WIDTH - strWidth;
  }
  else
  {
    cX = string->posx;
  }

  sX = cX;
  sY = string->posy;

  for(i=0; i<size; i++)
  {
    fNum = string->str[i]-32;

    fontWidth = string->font->fcInfo[fNum].fontWidth;
    fontHeight = string->font->fcInfo[fNum].fontHeight;

    if(string->str[i]=='\r')
    {
      r_flag = 1;
    }
    else if(string->str[i]=='\n')
    {
      sY = sY + fontHeight;
    }
    else
    {
      if(i==0 || r_flag)
      {
        sX = cX;
        r_flag = 0;
      }
      else
      {
        sX = sX + fontWidth_old + string->font->fontSpace;
      }

      charWidth = lcdDrawChar(string->font, sX, sY, string->str[i], string->tcol, string->bcol, string->btransp);
      stringWidth = stringWidth + charWidth;

//      fNum_old = fNum;
      fontWidth_old = fontWidth;
    }
  }

  return stringWidth;
}


/*
 * sfonts         : font
 * aX             : x
 * aY             : y
 * txColor        : text color
 * bgColor        : background color
 * bgTransparent  : Background Transparent(0:bgColor, 1:Transparent)
 */
void lprintf(lcd_font_t *sfonts, uint16_t aX, uint16_t aY, uint32_t txColor, uint32_t bgColor, bool bgTransparent, const char *format, ...) // skkim-lcd
{
  va_list arg;
  char buf[150]={0,};
  lcd_string_t string;

  va_start(arg, format);
  vsprintf(buf, format, arg);
  va_end(arg);

  string.font = sfonts;
  string.str = buf;
  string.posx = aX;
  string.posy = aY;
  string.tcol = txColor;
  string.bcol = bgColor;
  string.btransp = bgTransparent;
  //  string.tAlign = align;

  lcdDrawString(&string);
}


#ifdef _USE_HW_CLI
void cliLcd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "test") == true)
  {
    while(cliKeepLoop())
    {
      if (lcdDrawAvailable() == true)
      {
        lcdClearBuffer(black);

        lcdPrintf(25,16*0, green, "[LCD 테스트]");

        lcdPrintf(0,16*1, white, "%d fps", lcdGetFps());
        lcdPrintf(0,16*2, white, "%d ms" , lcdGetFpsTime());
        lcdPrintf(0,16*3, white, "%d ms" , millis());

        lcdDrawFillRect( 0, 70, 10, 10, red);
        lcdDrawFillRect(10, 70, 10, 10, green);
        lcdDrawFillRect(20, 70, 10, 10, blue);

        lcdRequestDraw();
      }
    }

    lcdClearBuffer(black);
    lcdUpdateDraw();

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "pix") == true)
  {
    uint16_t x, y;

    x = args->getData(1);
    y = args->getData(2);

    lcdClearBuffer(black);
    lcdDrawPixel(x, y, orange);
    lcdUpdateDraw();

    ret = true;
  }

  if (args->argc == 5 && args->isStr(0, "rect") == true)
  {
    uint16_t x, y, w, h;

    x = args->getData(1);
    y = args->getData(2);
    w = args->getData(3);
    h = args->getData(4);

    lcdClearBuffer(black);
    lcdDrawFillRect( x, y, w, h, blue);
    lcdUpdateDraw();

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("\n---------------[ LCD Commands ]---------------------------------------\n\n");

    cliPrintf(">> lcd test\t\t\t\t\t: LCD test program start\n\n");

    cliPrintf(">> lcd pix  [x-pos] [y-pos]\t\t\t: LCD draw pixel\n");
    cliPrintf(">> lcd rect [x-pos] [y-pos] [width] [height]\t: LCD draw rectangle\n");

    cliPrintf("\n----------------------------------------------------------------------\n\n");
  }
}


#endif //#ifdef _USE_HW_CLI

#endif //#ifdef _USE_HW_LCD
