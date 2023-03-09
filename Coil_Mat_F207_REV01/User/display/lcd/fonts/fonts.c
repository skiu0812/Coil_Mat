/*
 * fonts.c
 *
 *  Created on: 2019. 6. 14.
 *      Author: skiu0
 */


#include "user.h"
#include "fonts.h"


#ifdef _USE_HW_LCD


lcd_font_t font_tbl[] = {
//     fontArr,       fcInfo,                         Height  Bit   Space
    {font_0506,       font_0506_Descriptors,          6,      8,    1},
    {Gulim_8pt,       Gulim_8pt_Descriptors,          11,     8,    1},
    {Gulim_8pt_B,     Gulim_8pt_B_Descriptors,        11,     8,    1},
    {Gulim_12pt,      Gulim_12pt_Descriptors,         16,     8,    3},
    {Gulim_12pt_B,    Gulim_12pt_B_Descriptors,       16,     8,    3},
    {Gulim_14pt,      Gulim_14pt_Descriptors,         17,     8,    3},
    {Gulim_14pt_B,    Gulim_14pt_B_Descriptors,       17,     8,    3},
    {Gulim_18pt,      Gulim_18pt_Descriptors,         24,     8,    4},
    {Gulim_18pt_B,    Gulim_18pt_B_Descriptors,       24,     8,    4},
    {Gulim_22pt_B,    Gulim_22pt_B_Descriptors,       28,     8,    4},
    {Gulim_24pt_B,    Gulim_24pt_B_Descriptors,       31,     8,    5},
    {Gulim_26pt_B,    Gulim_26pt_B_Descriptors,       34,     8,    5},
    {Gulim_28pt_B,    Gulim_28pt_B_Descriptors,       35,     8,    4},

    {yuGothic_8pt,    yuGothic_8pt_Descriptors,       12,     8,    3},
    {yuGothic_8pt_B,  yuGothic_8pt_B_Descriptors,     12,     8,    3},
    {yuGothic_10pt,   yuGothic_10pt_Descriptors,      13,     8,    3},
    {yuGothic_10pt_B, yuGothic_10pt_B_Descriptors,    13,     8,    3},
    {yuGothic_12pt,   yuGothic_12pt_Descriptors,      17,     8,    3},
    {yuGothic_12pt_B, yuGothic_12pt_B_Descriptors,    17,     8,    3},
    {yuGothic_16pt,   yuGothic_16pt_Descriptors,      21,     8,    3},
    {yuGothic_16pt_B, yuGothic_16pt_B_Descriptors,    21,     8,    3},
    {yuGothic_20pt,   yuGothic_20pt_Descriptors,      27,     8,    3},
    {yuGothic_20pt_B, yuGothic_20pt_B_Descriptors,    27,     8,    3},

    {HyTaebaek_30pt_B, HyTaebaek_30pt_B_Descriptors,  40,     8,    5},
    {HyTaebaek_36pt_B, HyTaebaek_36pt_B_Descriptors,  48,     8,    5},
    {HyTaebaek_48pt_B, HyTaebaek_48pt_B_Descriptors,  64,     8,    6},
};


#endif //#ifdef _USE_HW_LCD
