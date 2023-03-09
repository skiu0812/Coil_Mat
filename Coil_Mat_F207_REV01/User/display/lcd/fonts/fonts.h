/*
 * fonts.h
 *
 *  Created on: 2022. 4. 17.
 *      Author: KSK
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


typedef struct _tFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;

}fonts_t;

typedef struct
{
	uint8_t fontWidth;
	uint8_t fontHeight;
	uint16_t fontOffset;
}font_info_t;


/*
 * fontArr : font array pointer
 * fcInfo : font descriptors
 * fontHeight : font Height
 * fontBit : font data type size
 * fontSpace : space between letters
 */
typedef struct
{
  const uint8_t     *fontArr;
  const font_info_t *fcInfo;
  uint8_t           fontHeight;
  uint8_t           fontBit;
  uint8_t           fontSpace;
}lcd_font_t;


typedef enum
{
  B0506 = 0,

  GULIM8,
  GULIM8B,
  GULIM12,
  GULIM12B,
  GULIM14,
  GULIM14B,
  GULIM18,
  GULIM18B,
  GULIM22B,
  GULIM24B,
  GULIM26B,
  GULIM28B,

  YUGOTHIC8,
  YUGOTHIC8B,
  YUGOTHIC10,
  YUGOTHIC10B,
  YUGOTHIC12,
  YUGOTHIC12B,
  YUGOTHIC16,
  YUGOTHIC16B,
  YUGOTHIC20,
  YUGOTHIC20B,

  HYTAEBAEK30B,
  HYTAEBAEK36B,
  HYTAEBAEK48B,
}font_tbl_e;


extern lcd_font_t font_tbl[];

extern const uint8_t font_0506[];
extern const font_info_t font_0506_Descriptors[];
extern const uint8_t Gulim_8pt[];
extern const font_info_t Gulim_8pt_Descriptors[];
extern const uint8_t Gulim_12pt[];
extern const font_info_t Gulim_12pt_Descriptors[];
extern const uint8_t Gulim_14pt[];
extern const font_info_t Gulim_14pt_Descriptors[];
extern const uint8_t Gulim_18pt[];
extern const font_info_t Gulim_18pt_Descriptors[];
extern const uint8_t Gulim_8pt_B[];
extern const font_info_t Gulim_8pt_B_Descriptors[];
extern const uint8_t Gulim_12pt_B[];
extern const font_info_t Gulim_12pt_B_Descriptors[];
extern const uint8_t Gulim_14pt_B[];
extern const font_info_t Gulim_14pt_B_Descriptors[];
extern const uint8_t Gulim_18pt_B[];
extern const font_info_t Gulim_18pt_B_Descriptors[];
extern const uint8_t Gulim_22pt_B[];
extern const font_info_t Gulim_22pt_B_Descriptors[];
extern const uint8_t Gulim_24pt_B[];
extern const font_info_t Gulim_24pt_B_Descriptors[];
extern const uint8_t Gulim_26pt_B[];
extern const font_info_t Gulim_26pt_B_Descriptors[];
extern const uint8_t Gulim_28pt_B[];
extern const font_info_t Gulim_28pt_B_Descriptors[];

extern const uint8_t yuGothic_8pt[];
extern const font_info_t yuGothic_8pt_Descriptors[];
extern const uint8_t yuGothic_10pt[];
extern const font_info_t yuGothic_10pt_Descriptors[];
extern const uint8_t yuGothic_12pt[];
extern const font_info_t yuGothic_12pt_Descriptors[];
extern const uint8_t yuGothic_14pt_B[];
extern const font_info_t yuGothic_14pt_B_Descriptors[];
extern const uint8_t yuGothic_16pt[];
extern const font_info_t yuGothic_16pt_Descriptors[];
extern const uint8_t yuGothic_20pt[];
extern const font_info_t yuGothic_20pt_Descriptors[];
extern const uint8_t yuGothic_8pt_B[];
extern const font_info_t yuGothic_8pt_B_Descriptors[];
extern const uint8_t yuGothic_10pt_B[];
extern const font_info_t yuGothic_10pt_B_Descriptors[];
extern const uint8_t yuGothic_12pt_B[];
extern const font_info_t yuGothic_12pt_B_Descriptors[];
extern const uint8_t yuGothic_16pt_B[];
extern const font_info_t yuGothic_16pt_B_Descriptors[];
extern const uint8_t yuGothic_20pt_B[];
extern const font_info_t yuGothic_20pt_B_Descriptors[];

extern const uint8_t HyTaebaek_30pt_B[];
extern const font_info_t HyTaebaek_30pt_B_Descriptors[];
extern const uint8_t HyTaebaek_36pt_B[];
extern const font_info_t HyTaebaek_36pt_B_Descriptors[];
extern const uint8_t HyTaebaek_48pt_B[];
extern const font_info_t HyTaebaek_48pt_B_Descriptors[];

/**
  * @}
  */

/** @defgroup FONTS_Exported_Constants
  * @{
  */
#define LINE(x) ((x) * (((fonts_t *)BSP_LCD_GetFont())->Height))

#endif /* INC_FONTS_H_ */
