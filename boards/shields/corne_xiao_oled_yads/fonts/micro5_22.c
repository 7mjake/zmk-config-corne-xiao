/*******************************************************************************
 * Size: 22 px
 * Bpp: 1
 * Opts: --font boards\shields\corne_xiao_oled_yads\fonts\Micro5-Regular.ttf --size 22 --bpp 1 --format lvgl --symbols  %.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ --autohint-off --no-compress --no-kerning --lv-include lvgl.h --lv-fallback lv_font_unscii_8 --lv-font-name micro5_22 -o C:\Users\7mjak\AppData\Local\Temp\micro5_22.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef MICRO5_22
#define MICRO5_22 1
#endif

#if MICRO5_22

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0025 "%" */
    0xc0, 0xf0, 0x30, 0x30, 0xc, 0xc, 0x3, 0x3,
    0x0, 0xc0, 0xc0, 0xf0, 0x30,

    /* U+002E "." */
    0xf0,

    /* U+0030 "0" */
    0xff, 0xfc, 0xf3, 0xcf, 0x3c, 0xf3, 0xff, 0xf0,

    /* U+0031 "1" */
    0xff, 0x33, 0x33, 0x33, 0x33,

    /* U+0032 "2" */
    0xff, 0xf0, 0xc3, 0xff, 0xfc, 0x30, 0xff, 0xf0,

    /* U+0033 "3" */
    0xff, 0xf0, 0xc3, 0x3c, 0xf0, 0xc3, 0xff, 0xf0,

    /* U+0034 "4" */
    0xcf, 0x3c, 0xf3, 0xff, 0xf0, 0xc3, 0xc, 0x30,

    /* U+0035 "5" */
    0xff, 0xfc, 0x30, 0xff, 0xf0, 0xc3, 0xff, 0xf0,

    /* U+0036 "6" */
    0xff, 0xfc, 0x30, 0xff, 0xfc, 0xf3, 0xff, 0xf0,

    /* U+0037 "7" */
    0xff, 0xf0, 0xc3, 0xc, 0x33, 0xc, 0x30, 0xc0,

    /* U+0038 "8" */
    0xff, 0xfc, 0xf3, 0xff, 0xfc, 0xf3, 0xff, 0xf0,

    /* U+0039 "9" */
    0xff, 0xfc, 0xf3, 0xff, 0xf0, 0xc3, 0xff, 0xf0,

    /* U+0041 "A" */
    0x30, 0xcc, 0xf3, 0xff, 0xfc, 0xf3, 0xcf, 0x30,

    /* U+0042 "B" */
    0xff, 0xfc, 0xf3, 0xf3, 0xcc, 0xf3, 0xff, 0xf0,

    /* U+0043 "C" */
    0xff, 0xcc, 0xcc, 0xcc, 0xff,

    /* U+0044 "D" */
    0xf3, 0xcc, 0xf3, 0xcf, 0x3c, 0xf3, 0xf3, 0xc0,

    /* U+0045 "E" */
    0xff, 0xfc, 0x30, 0xf3, 0xcc, 0x30, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xfc, 0x30, 0xf3, 0xcc, 0x30, 0xc3, 0x0,

    /* U+0047 "G" */
    0xff, 0xfc, 0x30, 0xcf, 0x3c, 0xf3, 0xf3, 0xc0,

    /* U+0048 "H" */
    0xcf, 0x3c, 0xf3, 0xff, 0xfc, 0xf3, 0xcf, 0x30,

    /* U+0049 "I" */
    0xff, 0xff, 0xf0,

    /* U+004A "J" */
    0x33, 0x33, 0x33, 0x33, 0xcc,

    /* U+004B "K" */
    0xcf, 0x3c, 0xf3, 0xf3, 0xcc, 0xf3, 0xcf, 0x30,

    /* U+004C "L" */
    0xcc, 0xcc, 0xcc, 0xcc, 0xff,

    /* U+004D "M" */
    0xf3, 0x3c, 0xcc, 0xcf, 0x33, 0xcc, 0xf3, 0x3c,
    0xcf, 0x33, 0xcc, 0xf3, 0x30,

    /* U+004E "N" */
    0xf3, 0xcc, 0xf3, 0xcf, 0x3c, 0xf3, 0xcf, 0x30,

    /* U+004F "O" */
    0xff, 0xfc, 0xf3, 0xcf, 0x3c, 0xf3, 0xff, 0xf0,

    /* U+0050 "P" */
    0xff, 0xfc, 0xf3, 0xff, 0xfc, 0x30, 0xc3, 0x0,

    /* U+0051 "Q" */
    0xff, 0xfc, 0xf3, 0xcf, 0x3c, 0xf3, 0xcf, 0x33,
    0xc,

    /* U+0052 "R" */
    0xff, 0xfc, 0xf3, 0xf3, 0xcc, 0xf3, 0xcf, 0x30,

    /* U+0053 "S" */
    0xff, 0xfc, 0x30, 0xff, 0xf0, 0xc3, 0xff, 0xf0,

    /* U+0054 "T" */
    0xff, 0xf3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc0,

    /* U+0055 "U" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3c, 0xf3, 0xff, 0xf0,

    /* U+0056 "V" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3c, 0xf3, 0x30, 0xc0,

    /* U+0057 "W" */
    0xcc, 0xf3, 0x3c, 0xcf, 0x33, 0xcc, 0xf3, 0x3c,
    0xcf, 0x33, 0x33, 0xc, 0xc0,

    /* U+0058 "X" */
    0xcf, 0x3c, 0xf3, 0x30, 0xcc, 0xf3, 0xcf, 0x30,

    /* U+0059 "Y" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x33, 0xc, 0x30, 0xc0,

    /* U+005A "Z" */
    0xff, 0xf0, 0xc3, 0x30, 0xcc, 0x30, 0xff, 0xf0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 96, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 192, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 64, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 96, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 96, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 64, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 96, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 96, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 192, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 220, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 192, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x5, 0xe
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 15, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 48, .range_length = 10, .glyph_id_start = 4,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 14,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_unscii_8;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t micro5_22 = {
#else
lv_font_t micro5_22 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_unscii_8,
#endif
    .user_data = NULL,
};

#endif /*#if MICRO5_22*/
