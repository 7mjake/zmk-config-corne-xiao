/*
 * YADS-inspired monochrome status screen for the CORNE XIAO OLED dongle.
 *
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include <lvgl.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/kscan.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/activity.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/split/central.h>
#include <zmk/split/transport/central.h>

#include <zmk/events/battery_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

LV_FONT_DECLARE(micro5_22);
LV_FONT_DECLARE(micro5_44);

#ifndef ZMK_SPLIT_BLE_PERIPHERAL_COUNT
#define ZMK_SPLIT_BLE_PERIPHERAL_COUNT 0
#endif

#define DISPLAY_WIDTH 128
#define BATTERY_COUNT 2
#define CHARGE_ICON_WIDTH 5
#define CHARGE_ICON_HEIGHT 9
#define LEFT_BATTERY_SOURCE 1
#define RIGHT_BATTERY_SOURCE 0

static lv_obj_t *layer_label;
static lv_obj_t *caps_label;
static lv_obj_t *battery_labels[BATTERY_COUNT];
static lv_obj_t *battery_bars[BATTERY_COUNT];
static lv_obj_t *charging_icons[BATTERY_COUNT];
static uint8_t charging_icon_buffers[BATTERY_COUNT][CHARGE_ICON_WIDTH * CHARGE_ICON_HEIGHT];
static bool caps_lock_active;

struct battery_display_state {
    bool connected;
    bool has_level;
    uint8_t level;
};

static struct battery_display_state battery_display_states[BATTERY_COUNT];
static bool charging_states[BATTERY_COUNT];

static uint8_t battery_display_slot(uint8_t source) {
    return source == 0 ? 1 : 0;
}

static void draw_charging_icon(lv_obj_t *canvas) {
    static const uint8_t rows[CHARGE_ICON_HEIGHT] = {
        0x04, // 00100
        0x0c, // 01100
        0x18, // 11000
        0x1e, // 11110
        0x06, // 00110
        0x0c, // 01100
        0x08, // 01000
        0x00, // 00000
        0x00, // 00000
    };

    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    for (uint8_t y = 0; y < CHARGE_ICON_HEIGHT; y++) {
        for (uint8_t x = 0; x < CHARGE_ICON_WIDTH; x++) {
            if ((rows[y] & (1U << (CHARGE_ICON_WIDTH - 1 - x))) != 0) {
                lv_canvas_set_px(canvas, x, y, lv_color_black(), LV_OPA_COVER);
            }
        }
    }
}

static void charging_display_work_cb(struct k_work *work);
K_WORK_DEFINE(charging_display_work, charging_display_work_cb);

static const struct device *oled_display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static const struct device *dongle_buttons =
    DEVICE_DT_GET(DT_NODELABEL(prototype_button_behaviors));

static void button_reblank_work_cb(struct k_work *work) {
    ARG_UNUSED(work);
    if (zmk_activity_get_state() != ZMK_ACTIVITY_ACTIVE) {
        display_blanking_on(oled_display);
    }
}

K_WORK_DELAYABLE_DEFINE(button_reblank_work, button_reblank_work_cb);

static void button_wake_work_cb(struct k_work *work) {
    ARG_UNUSED(work);
    display_blanking_off(oled_display);
    lv_task_handler();
    k_work_reschedule_for_queue(zmk_display_work_q(), &button_reblank_work,
                                K_MSEC(CONFIG_ZMK_IDLE_TIMEOUT));
}

K_WORK_DELAYABLE_DEFINE(button_wake_work, button_wake_work_cb);

static void dongle_button_callback(const struct device *dev, uint32_t row, uint32_t column,
                                   bool pressed) {
    ARG_UNUSED(dev);
    ARG_UNUSED(row);
    ARG_UNUSED(column);

    if (pressed && zmk_display_is_initialized()) {
        k_work_reschedule_for_queue(zmk_display_work_q(), &button_wake_work, K_MSEC(1));
    }
}

static int display_activity_cb(const zmk_event_t *eh) {
    const struct zmk_activity_state_changed *event = as_zmk_activity_state_changed(eh);
    if (event != NULL && event->state == ZMK_ACTIVITY_ACTIVE) {
        k_work_cancel_delayable(&button_reblank_work);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(corne_xiao_yads_display_activity, display_activity_cb);
ZMK_SUBSCRIPTION(corne_xiao_yads_display_activity, zmk_activity_state_changed);

struct layer_state {
    uint8_t index;
    const char *name;
};

static struct layer_state layer_state_get(const zmk_event_t *eh) {
    ARG_UNUSED(eh);
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct layer_state){
        .index = index,
        .name = zmk_keymap_layer_name(index),
    };
}

static void layer_state_update(struct layer_state state) {
    if (layer_label == NULL) {
        return;
    }

    if (state.name != NULL) {
        lv_label_set_text(layer_label, state.name);
    } else {
        lv_label_set_text_fmt(layer_label, "L%u", state.index);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(corne_xiao_yads_layer, struct layer_state, layer_state_update,
                            layer_state_get)
ZMK_SUBSCRIPTION(corne_xiao_yads_layer, zmk_layer_state_changed);

#define HID_INDICATOR_CAPS_LOCK 0x02

struct caps_state {
    bool active;
};

/* ZMK does not currently expose Caps Word state through a public event or getter.
 * Its device data begins with the active flag, so read that flag after relevant
 * events and keep this compatibility shim in one small, easy-to-replace place. */
struct caps_word_data_view {
    bool active;
};

static bool caps_word_is_active(void) {
    const struct device *device = DEVICE_DT_GET(DT_NODELABEL(caps_word));
    if (!device_is_ready(device) || device->data == NULL) {
        return false;
    }

    return ((const struct caps_word_data_view *)device->data)->active;
}

static void update_caps_label(void) {
    if (caps_label == NULL) {
        return;
    }

    lv_label_set_text(caps_label, caps_lock_active ? "CAPS" : caps_word_is_active() ? "CW" : "");
}

static struct caps_state caps_state_get(const zmk_event_t *eh) {
    if (eh == NULL) {
        return (struct caps_state){.active = false};
    }

    const struct zmk_hid_indicators_changed *event = as_zmk_hid_indicators_changed(eh);
    return (struct caps_state){.active = event->indicators & HID_INDICATOR_CAPS_LOCK};
}

static void caps_state_update(struct caps_state state) {
    caps_lock_active = state.active;
    update_caps_label();
}

ZMK_DISPLAY_WIDGET_LISTENER(corne_xiao_yads_caps, struct caps_state, caps_state_update, caps_state_get)
ZMK_SUBSCRIPTION(corne_xiao_yads_caps, zmk_hid_indicators_changed);

static void caps_word_work_cb(struct k_work *work) {
    ARG_UNUSED(work);
    update_caps_label();
}

K_WORK_DELAYABLE_DEFINE(caps_word_work, caps_word_work_cb);

static int caps_word_tracker_cb(const zmk_event_t *eh) {
    ARG_UNUSED(eh);
    if (zmk_display_is_initialized()) {
        k_work_reschedule_for_queue(zmk_display_work_q(), &caps_word_work, K_MSEC(1));
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(corne_xiao_yads_caps_word_tracker, caps_word_tracker_cb);
ZMK_SUBSCRIPTION(corne_xiao_yads_caps_word_tracker, zmk_position_state_changed);
ZMK_SUBSCRIPTION(corne_xiao_yads_caps_word_tracker, zmk_keycode_state_changed);

struct battery_state {
    uint8_t source;
    uint8_t level;
    bool valid;
};

static void render_battery(uint8_t source) {
    if (source >= BATTERY_COUNT) {
        return;
    }

    uint8_t slot = battery_display_slot(source);
    if (battery_labels[slot] == NULL) {
        return;
    }

    struct battery_display_state *state = &battery_display_states[source];

    if (!state->connected) {
        lv_label_set_text(battery_labels[slot], "OFF");
        lv_obj_add_flag(battery_bars[slot], LV_OBJ_FLAG_HIDDEN);
    } else if (!state->has_level) {
        lv_label_set_text(battery_labels[slot], "...");
        lv_obj_add_flag(battery_bars[slot], LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_label_set_text_fmt(battery_labels[slot], "%u", state->level);
        lv_bar_set_value(battery_bars[slot], state->level, LV_ANIM_ON);
        lv_obj_clear_flag(battery_bars[slot], LV_OBJ_FLAG_HIDDEN);
    }

    if (state->connected && charging_states[source]) {
        lv_obj_clear_flag(charging_icons[slot], LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(charging_icons[slot], LV_OBJ_FLAG_HIDDEN);
    }
}

static void charging_display_work_cb(struct k_work *work) {
    ARG_UNUSED(work);

    for (uint8_t source = 0; source < BATTERY_COUNT; source++) {
        render_battery(source);
    }
}

static void charging_input_cb(struct input_event *evt, void *user_data) {
    if (evt->type != INPUT_EV_KEY || evt->code != INPUT_KEY_0) {
        return;
    }

    uint8_t source = (uint8_t)(uintptr_t)user_data;
    if (source >= BATTERY_COUNT) {
        return;
    }

    charging_states[source] = evt->value != 0;
    if (zmk_display_is_initialized()) {
        k_work_submit_to_queue(zmk_display_work_q(), &charging_display_work);
    }
}

INPUT_CALLBACK_DEFINE_NAMED(DEVICE_DT_GET(DT_NODELABEL(charge_left_split)), charging_input_cb,
                            (void *)LEFT_BATTERY_SOURCE, charge_left);
INPUT_CALLBACK_DEFINE_NAMED(DEVICE_DT_GET(DT_NODELABEL(charge_right_split)), charging_input_cb,
                            (void *)RIGHT_BATTERY_SOURCE, charge_right);

static void peripheral_status_work_cb(struct k_work *work) {
    ARG_UNUSED(work);
    bool connected[BATTERY_COUNT] = {};

    STRUCT_SECTION_FOREACH(zmk_split_transport_central, transport) {
        if (transport->api->get_available_source_ids == NULL) {
            continue;
        }

        uint8_t sources[ZMK_SPLIT_CENTRAL_PERIPHERAL_COUNT];
        int count = transport->api->get_available_source_ids(sources);
        for (int i = 0; i < count; i++) {
            if (sources[i] < BATTERY_COUNT) {
                connected[sources[i]] = true;
            }
        }
    }

    for (int i = 0; i < BATTERY_COUNT; i++) {
        struct battery_display_state *state = &battery_display_states[i];
        if (connected[i] != state->connected) {
            state->connected = connected[i];
            state->has_level = false;
        }
        render_battery(i);
    }

    k_work_reschedule_for_queue(zmk_display_work_q(),
                                CONTAINER_OF(work, struct k_work_delayable, work), K_SECONDS(1));
}

K_WORK_DELAYABLE_DEFINE(peripheral_status_work, peripheral_status_work_cb);

static struct battery_state battery_state_get(const zmk_event_t *eh) {
    const struct zmk_peripheral_battery_state_changed *event =
        as_zmk_peripheral_battery_state_changed(eh);

    if (event == NULL) {
        return (struct battery_state){.valid = false};
    }

    return (struct battery_state){
        .source = event->source,
        .level = event->state_of_charge,
        .valid = true,
    };
}

static void battery_state_update(struct battery_state state) {
    if (!state.valid || state.source >= BATTERY_COUNT || battery_labels[state.source] == NULL) {
        return;
    }

    struct battery_display_state *display_state = &battery_display_states[state.source];
    if (state.level > 0) {
        display_state->connected = true;
    }
    display_state->level = state.level;
    display_state->has_level = true;
    render_battery(state.source);
}

ZMK_DISPLAY_WIDGET_LISTENER(corne_xiao_yads_battery, struct battery_state, battery_state_update,
                            battery_state_get)
ZMK_SUBSCRIPTION(corne_xiao_yads_battery, zmk_peripheral_battery_state_changed);

static void style_battery_bar(lv_obj_t *bar) {
    lv_obj_set_size(bar, 54, 4);
    lv_bar_set_range(bar, 0, 100);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);

    lv_obj_set_style_radius(bar, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 0, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(bar, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, lv_color_black(), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    static lv_style_t screen_style;
    lv_style_init(&screen_style);
    lv_style_set_bg_color(&screen_style, lv_color_white());
    lv_style_set_bg_opa(&screen_style, LV_OPA_COVER);
    lv_style_set_text_color(&screen_style, lv_color_black());
    lv_style_set_text_font(&screen_style, &micro5_22);
    lv_obj_add_style(screen, &screen_style, LV_PART_MAIN);

    caps_label = lv_label_create(screen);
    lv_obj_set_size(caps_label, 48, 12);
    lv_obj_set_pos(caps_label, 76, 5);
    lv_obj_set_style_text_align(caps_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_pad_all(caps_label, 0, LV_PART_MAIN);
    lv_label_set_text(caps_label, "");

    layer_label = lv_label_create(screen);
    lv_obj_set_width(layer_label, 76);
    lv_obj_set_style_text_align(layer_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(layer_label, &micro5_44, 0);
    lv_obj_align(layer_label, LV_ALIGN_TOP_LEFT, 4, 1);
    lv_label_set_text(layer_label, "MAC");

    for (int i = 0; i < BATTERY_COUNT; i++) {
        int x = i == 0 ? 4 : 70;

        battery_labels[i] = lv_label_create(screen);
        lv_obj_set_width(battery_labels[i], 54);
        lv_obj_set_style_text_align(battery_labels[i],
                                    i == 0 ? LV_TEXT_ALIGN_LEFT : LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_set_pos(battery_labels[i], x, 45);
        lv_label_set_text(battery_labels[i], "OFF");

        battery_bars[i] = lv_bar_create(screen);
        style_battery_bar(battery_bars[i]);
        if (i == 1) {
            lv_obj_set_style_base_dir(battery_bars[i], LV_BASE_DIR_RTL, LV_PART_MAIN);
        }
        lv_obj_set_pos(battery_bars[i], x, 58);
        lv_obj_add_flag(battery_bars[i], LV_OBJ_FLAG_HIDDEN);

        charging_icons[i] = lv_canvas_create(screen);
        lv_canvas_set_buffer(charging_icons[i], charging_icon_buffers[i], CHARGE_ICON_WIDTH,
                             CHARGE_ICON_HEIGHT, LV_COLOR_FORMAT_L8);
        draw_charging_icon(charging_icons[i]);
        lv_obj_set_pos(charging_icons[i], i == 0 ? 20 : 102, 45);
        lv_obj_add_flag(charging_icons[i], LV_OBJ_FLAG_HIDDEN);
    }

    corne_xiao_yads_layer_init();
    corne_xiao_yads_caps_init();
    update_caps_label();
    corne_xiao_yads_battery_init();
    k_work_reschedule_for_queue(zmk_display_work_q(), &peripheral_status_work, K_NO_WAIT);

    if (device_is_ready(dongle_buttons)) {
        int err = kscan_config(dongle_buttons, dongle_button_callback);
        if (err == 0) {
            err = kscan_enable_callback(dongle_buttons);
        }
        if (err != 0) {
            LOG_WRN("Could not attach display wake callback to dongle buttons: %d", err);
        }
    } else {
        LOG_WRN("Dongle buttons are not ready; display wake is unavailable");
    }

    return screen;
}
