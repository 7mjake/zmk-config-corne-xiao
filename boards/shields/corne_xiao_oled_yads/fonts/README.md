# Micro 5 firmware fonts

`Micro5-Regular.ttf` is the Micro 5 typeface by Sarah Cadigan-Fried and the
Soft Type Project Authors. It is distributed under the SIL Open Font License
1.1 in `OFL.txt`.

Source: <https://github.com/google/fonts/tree/main/ofl/micro5>

The LVGL C files contain the uppercase dashboard glyphs at one bit per pixel.
Auto-hinting is disabled and the sizes are exact integer scales of Micro 5's
3x5 pixel grid: 22 px is 2x and 44 px is 4x. They can be regenerated from the
repository root with:

```powershell
$symbols = ' %.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
npx.cmd --yes lv_font_conv --font boards/shields/corne_xiao_oled_yads/fonts/Micro5-Regular.ttf --size 22 --bpp 1 --format lvgl --symbols $symbols --autohint-off --no-compress --no-kerning --lv-include lvgl.h --lv-fallback lv_font_unscii_8 --lv-font-name micro5_22 -o boards/shields/corne_xiao_oled_yads/fonts/micro5_22.c
npx.cmd --yes lv_font_conv --font boards/shields/corne_xiao_oled_yads/fonts/Micro5-Regular.ttf --size 44 --bpp 1 --format lvgl --symbols $symbols --autohint-off --no-compress --no-kerning --lv-include lvgl.h --lv-fallback lv_font_unscii_8 --lv-font-name micro5_44 -o boards/shields/corne_xiao_oled_yads/fonts/micro5_44.c
```
