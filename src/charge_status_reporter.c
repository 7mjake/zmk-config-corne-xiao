/*
 * Report the XIAO nRF52840 charger status through ZMK's split input channel.
 * P0.17 is the BQ25101 active-low CHG output.
 *
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

#define CHARGE_INPUT_NODE DT_NODELABEL(charge_status_input)
#define CHARGE_PIN_NODE DT_NODELABEL(charge_status_pin)

static const struct device *const charge_input = DEVICE_DT_GET(CHARGE_INPUT_NODE);
static const struct gpio_dt_spec charge_pin = GPIO_DT_SPEC_GET(CHARGE_PIN_NODE, gpios);

static void charge_report_work_cb(struct k_work *work);
K_WORK_DELAYABLE_DEFINE(charge_report_work, charge_report_work_cb);

static void charge_report_work_cb(struct k_work *work) {
    ARG_UNUSED(work);

    if (!device_is_ready(charge_input) || !gpio_is_ready_dt(&charge_pin)) {
        return;
    }

    int charging = gpio_pin_get_dt(&charge_pin);
    if (charging < 0) {
        return;
    }

    input_report_key(charge_input, INPUT_KEY_0, charging, true, K_NO_WAIT);

    // While externally powered, periodically refresh the state so a newly
    // reconnected dongle receives it. Do not wake a battery-powered half.
    if (charging) {
        k_work_reschedule(&charge_report_work, K_SECONDS(5));
    }
}

static void charge_input_changed(struct input_event *evt, void *user_data) {
    ARG_UNUSED(user_data);

    if (evt->type != INPUT_EV_KEY || evt->code != INPUT_KEY_0) {
        return;
    }

    if (evt->value) {
        k_work_reschedule(&charge_report_work, K_SECONDS(5));
    } else {
        k_work_cancel_delayable(&charge_report_work);
    }
}

INPUT_CALLBACK_DEFINE(charge_input, charge_input_changed, NULL);

static int charge_status_init(void) {
    // GPIO Keys configures the pin and its edge interrupt before this runs.
    // Delay the first report until the split transport has initialized.
    k_work_reschedule(&charge_report_work, K_SECONDS(2));
    return 0;
}

SYS_INIT(charge_status_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
