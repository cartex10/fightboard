#include QMK_KEYBOARD_H

int LAYER = 0;
const int LAST_LAYER = 1;

enum my_keycodes {
  NEXT_LAYER = SAFE_RANGE,
  PREV_LAYER,
  REACTIVE_SIMPLE_RGB,
  REACTIVE_FULL_RGB
};

void keyboard_post_init_user(void) {
	// Customize these values to desired behaviour
	debug_enable=true;
	debug_matrix=true;
	debug_keyboard=true;
	//debug_mouse=true;
	//layer_on(1);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/*
	 *     │LED│   │ - │ = │BCK│ENT│
	 *     ┌───┐   ┌───┬───┬───┬───┐
	 *     │ W │   │ U │ I │ O │ P │
	 * ┌───┼───┼───┼───┼───┼───┼───┤
	 * │ A │ S │ D │ J │ K │ L │ ; │
	 * └───┴───┴───┴───┴───┴───┴───┘
	 *               KC_MINS, KC_EQL, KC_BSPC, KC_ENT,
	 *       KC_W,       KC_U, KC_I, KC_O, KC_P,
	 *  KC_A, KC_S, KC_D, KC_J, KC_K, KC_L, KC_SCLN
	 */
	[0] = LAYOUT(	// P1 Layer (Default)
		KC_J, KC_K, KC_L, KC_SCLN, KC_D, KC_S, KC_A, KC_W, KC_MINS, KC_EQL, KC_BSPC, KC_ENT, KC_U, KC_I, KC_O, KC_P, TG(2)
	),
    [1] = LAYOUT(	// P2 Layer
        KC_T, KC_H, KC_G, KC_F, KC_PENT, KC_P3, KC_P2, KC_P1, KC_PPLS, KC_P6, KC_P5, KC_P4, KC_LEFT_BRACKET, KC_RIGHT_BRACKET, KC_BSPC, KC_ENT, TG(2)
    ),
	[2] = LAYOUT(	// Config Layer (MUST ALWAYS BE LAST)
		RGB_SPD, RGB_M_R, RGB_M_SW, REACTIVE_FULL_RGB, RGB_HUI, RGB_VAD, RGB_HUD, RGB_VAI, PREV_LAYER, NEXT_LAYER, KC_BSPC, KC_ESC, RGB_SPI, RGB_M_P, RGB_M_B, REACTIVE_SIMPLE_RGB, TG(2)
	),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case NEXT_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER++);
				if (LAYER > LAST_LAYER) { LAYER = 0; } 
					layer_on(LAYER);
			}
			return false;
		case PREV_LAYER:
			if (record->event.pressed) { 
				layer_off(LAYER--);
				if (LAYER < 0) { LAYER = LAST_LAYER; } 
					layer_on(LAYER);
			}
			return false;
		case REACTIVE_SIMPLE_RGB:
			if (record->event.pressed) { 
				rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
			}
			return false;
		case REACTIVE_FULL_RGB:
			if (record->event.pressed) { 
				rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE);
			}
			return false;
		default:
			return true; // Process all other keycodes normally
	}
}