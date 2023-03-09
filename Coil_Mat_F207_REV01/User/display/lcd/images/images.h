/*
 * images.h
 *
 *  Created on: 2022. 12. 13.
 *      Author: skiu0
 */

#ifndef SRC_IMAGES_IMAGES_H_
#define SRC_IMAGES_IMAGES_H_


#include "hw_def.h"


#ifdef _USE_HW_LCD


typedef enum
{
  test = 0,
  mat,
  mat_heating,
} eImages_idx;


typedef struct
{
  const unsigned short  *img;
  unsigned short        *img_bk_buf;
  bool      is_backup;
  uint16_t  bg_color;
  bool      tranp_bg;
  uint16_t  tr_color;
  uint16_t  x_init;
  uint16_t  y_init;
  uint16_t  width;
  uint16_t  height;
} lcd_image_t;


extern lcd_image_t image_tbl[];
extern const unsigned short img_test[];
extern const unsigned short img_mat[];
extern const unsigned short img_mat_heating[];
extern       unsigned short img_mat_heating_bk[];


#endif

#endif /* SRC_IMAGES_IMAGES_H_ */
