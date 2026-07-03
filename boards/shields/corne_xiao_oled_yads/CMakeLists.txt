# Copyright (c) 2026
# SPDX-License-Identifier: MIT

if(CONFIG_SHIELD_CORNE_XIAO_OLED_YADS AND CONFIG_ZMK_DISPLAY AND CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)
    zephyr_library()
    zephyr_library_include_directories(${ZEPHYR_LVGL_MODULE_DIR})
    zephyr_library_include_directories(${ZEPHYR_BASE}/lib/gui/lvgl/)
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
    zephyr_library_sources(
        corne_xiao_oled_yads.c
        fonts/micro5_22.c
        fonts/micro5_44.c
    )
endif()
