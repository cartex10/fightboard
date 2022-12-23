// Copyright 2022 Carlos Ortiz (@Cartex10)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#include QMK_KEYBOARD_H
#ifndef LAST_LAYER
#define LAST_LAYER 6
#endif
int LAYER = 0;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case PREV_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER--);
				if (LAYER < 0) { LAYER = LAST_LAYER; } 
				layer_on(LAYER);
			}
			return false;
		case NEXT_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER++);
				if (LAYER > LAST_LAYER) { LAYER = 0; } 
				layer_on(LAYER);
			}
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