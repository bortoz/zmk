/*
 * Copyright (c) 2022 Alessandro Bortolin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>

#include <zmk/event_manager.h>
#include <zmk/hid_indicators.h>
#include <zmk/events/hid_indicators_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static zmk_hid_indicators indicators = 0;

void zmk_rgb_underglow_effect_user(int num_pixels, struct led_rgb *pixels) {
    for (int i = 0; i < num_pixels; i++) {
        uint8_t brt = (indicators & (1 << i)) ? CONFIG_ZMK_RGB_UNDERGLOW_BRT_MAX : 0;
        pixels[i] = (struct led_rgb){.r = brt, .g = brt, .b = brt};
    }
}

static int leds_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    indicators = ev->indicators;

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(leds_listener, leds_listener);
ZMK_SUBSCRIPTION(leds_listener, zmk_hid_indicators_changed);
