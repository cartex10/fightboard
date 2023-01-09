// Copyright 2022 Carlos Ortiz (@Cartex10)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "xinput.h"
#ifndef LAST_LAYER
#define LAST_LAYER 6
#endif

int LAYER;
extern const int indicatorHSV[LAST_LAYER+2][3];

typedef union {
	uint32_t raw;
	struct {
		uint8_t current_layer:8;
	};
} user_config_t;

user_config_t user_config;

void eeconfig_init_kb(void) {
	// EEPROM is getting reset!
	user_config.raw = 0;
	user_config.current_layer = 0;

	// Write default value to EEPROM now
	eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_kb(void) {
	// Read the user config from EEPROM
	user_config.raw = eeconfig_read_user();
	LAYER = user_config.current_layer;

	// Reload configs from EEPROM
	layer_move(LAYER);
	rgb_matrix_reload_from_eeprom();

	// Setup controller
	begin();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case PREV_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER--);
				if (LAYER < 0) { LAYER = LAST_LAYER; } 
				layer_on(LAYER);
				user_config.current_layer = LAYER;
				eeconfig_update_user(user_config.raw);
			}
			return false;
		case NEXT_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER++);
				if (LAYER > LAST_LAYER) { LAYER = 0; } 
				layer_on(LAYER);
				user_config.current_layer = LAYER;
				eeconfig_update_user(user_config.raw);
			}
			return false;
		case GP_A:
			if (record->event.pressed){ press(BUTTON_A); }
			else { release(BUTTON_A); }
			return false;
		case GP_B:
			if (record->event.pressed){ press(BUTTON_B); }
			else { release(BUTTON_B); }
			return false;
		case GP_X:
			if (record->event.pressed){ press(BUTTON_X); }
			else { release(BUTTON_X); }
			return false;
		case GP_Y:
			if (record->event.pressed){ press(BUTTON_Y); }
			else { release(BUTTON_Y); }
			return false;
		case GP_LB:
			if (record->event.pressed){ press(BUTTON_LB); }
			else { release(BUTTON_LB); }
			return false;
		case GP_RB:
			if (record->event.pressed){ press(BUTTON_RB); }
			else { release(BUTTON_RB); }
			return false;
		case GP_BACK:
			if (record->event.pressed){ press(BUTTON_BACK); }
			else { release(BUTTON_BACK); }
			return false;
		case GP_START:
			if (record->event.pressed){ press(BUTTON_START); }
			else { release(BUTTON_START); }
			return false;
		case GP_LS:
			if (record->event.pressed){ press(BUTTON_L3); }
			else { release(BUTTON_L3); }
			return false;
		case GP_RS:
			if (record->event.pressed){ press(BUTTON_R3); }
			else { release(BUTTON_R3); }
			return false;
		case GP_UP:
			if (record->event.pressed){ press(DPAD_UP); }
			else { release(DPAD_UP); }
			return false;
		case GP_DOWN:
			if (record->event.pressed){ press(DPAD_DOWN); }
			else { release(DPAD_DOWN); }
			return false;
		case GP_LEFT:
			if (record->event.pressed){ press(DPAD_LEFT); }
			else { release(DPAD_LEFT); }
			return false;
		case GP_RIGHT:
			if (record->event.pressed){ press(DPAD_RIGHT); }
			else { release(DPAD_RIGHT); }
			return false;
		case GP_LT:
			if (record->event.pressed){ setTrigger(TRIGGER_LEFT, 255); }
			else { setTrigger(TRIGGER_LEFT, 0); }
			return false;
		case GP_RT:
			if (record->event.pressed){ setTrigger(TRIGGER_RIGHT, 255); }
			else { setTrigger(TRIGGER_RIGHT, 0); }
			return false;
		case REACTIVE_SIMPLE_RGB:
			if (record->event.pressed) { rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_SIMPLE); }
			return false;
		case REACTIVE_FULL_RGB:
			if (record->event.pressed) { rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE); }
			return false;
		default:
			return true; // Process all other keycodes normally
	}
    return process_record_user(keycode, record);
}

bool rgb_matrix_indicators_kb(void) {
	int layer = get_highest_layer(layer_state|default_layer_state);
	int value = 80;
	if (layer == LAST_LAYER + 1) { value = 0; } 
	HSV layerHSV = { indicatorHSV[layer][0], indicatorHSV[layer][1], value };
	RGB layerRGB = hsv_to_rgb(layerHSV);
	rgb_matrix_set_color(12, layerRGB.r, layerRGB.g, layerRGB.b);
	return true;
}