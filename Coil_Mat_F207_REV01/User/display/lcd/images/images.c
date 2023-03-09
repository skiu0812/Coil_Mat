/*
 * images.c
 *
 *  Created on: 2022. 12. 13.
 *      Author: skiu0
 */


#include "user.h"
#include "images.h"

#ifdef _USE_HW_LCD

#define _DEF_IMG1       0
#define _DEF_IMG2       1
#define _DEF_IMG3       2
#define _DEF_IMG4       3
#define _DEF_IMG5       4
#define _DEF_IMG6       5
#define _DEF_IMG7       6
#define _DEF_IMG8       7
#define _DEF_IMG9       8
#define _DEF_IMG10      9
#define      IMG_TEST       _DEF_IMG1
#define      IMG_MAT        _DEF_IMG2
#define      IMG_HEATING    _DEF_IMG3


lcd_image_t image_tbl[] =
{
//   img              img_bk_buf          is_backup   bg_color    transp_bg     tr_color        x_init  y_init  width   height
    {img_test,        0,                  false,      white,      transp_off,   transp_white,   0,      0,      160,    128},
    {img_mat,         0,                  false,      white,      transp_on,    transp_white,   11,     80,     138,    40},
    {img_mat_heating, img_mat_heating_bk, false,      white,      transp_on,    transp_white,   0,      0,      50,     30},
};


#endif //#ifdef _USE_HW_LCD
