#include QMK_KEYBOARD_H
#ifndef LAST_LAYER
#define LAST_LAYER 6
#endif
int LAYER = 0;
const int indicatorHSV[LAST_LAYER+2][3] = {
	{ 0, 255, 100 },	// RED
	{ 170, 255, 100 },	// BLUE
	{ 85, 255, 100 },	// GREEN
	{ 43, 255, 100 },	// YELLOW
	{ 128, 255, 100 },	// CYAN
	{ 234, 255, 100 },	// PINK
	{ 191, 255, 100 },	// PURPLE
	{ 0, 0, 0 }			// OFF
};

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
	rgb_matrix_reload_from_eeprom();
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
		KC_J, KC_K, KC_L, KC_SCLN, KC_D, KC_S, KC_A, KC_W, KC_MINS, KC_EQL, KC_BSPC, KC_ENT, KC_U, KC_I, KC_O, KC_P, TG(LAST_LAYER)
	),
    [1] = LAYOUT(	// P2 Layer
        KC_Z, KC_X, KC_C, KC_V, KC_RGHT, KC_DOWN, KC_LEFT, KC_UP, KC_LBRC, KC_RBRC, KC_BSPC, KC_ENT, KC_B, KC_N, KC_M, KC_QUOT, TG(LAST_LAYER)
    ),
    [2] = LAYOUT(	// P3 Layer
        KC_5, KC_6, KC_7, KC_8, KC_H, KC_G, KC_F, KC_T, KC_COMM, KC_DOT, KC_BSPC, KC_ENT, KC_1, KC_2, KC_3, KC_4, TG(LAST_LAYER)
    ),
    [3] = LAYOUT(	// P4 Layer
        KC_P5, KC_P6, KC_P7, KC_P8, KC_Y, KC_R, KC_E, KC_Q, KC_NUBS, KC_PSLS, KC_BSPC, KC_ENT, KC_P1, KC_P2, KC_P3, KC_P4, TG(LAST_LAYER)
    ),
    [4] = LAYOUT(	// Alt1 Layer
        KC_Z, KC_X, KC_C, KC_V, KC_RGHT, KC_DOWN, KC_LEFT, KC_UP, KC_ESC, KC_M, KC_TAB, KC_ENT, KC_A, KC_S, KC_D, KC_F, TG(LAST_LAYER)
    ),
    [5] = LAYOUT(	// Alt2 Layer
        KC_LEFT, KC_DOWN, KC_RGHT, KC_TAB, KC_D, KC_S, KC_A, KC_W, KC_ESC, KC_R, KC_LCTL, KC_ENT, KC_Q, KC_UP, KC_E, KC_SPC, TG(LAST_LAYER)
    ),
    [6] = LAYOUT(	// Alt3 Layer
        KC_J, KC_K, KC_L, KC_DOWN, KC_F, KC_D, KC_S, KC_A, KC_ESC, KC_Q, KC_E, KC_ENT, KC_LEFT, KC_RGHT, KC_SCLN, KC_UP, TG(LAST_LAYER)
    ),
	[7] = LAYOUT(	// Config Layer (MUST ALWAYS BE LAST)
		RGB_SPD, RGB_M_R, RGB_M_SW, REACTIVE_FULL_RGB, RGB_HUI, RGB_VAD, RGB_HUD, RGB_VAI, PREV_LAYER, NEXT_LAYER, KC_BSPC, KC_ESC, RGB_SPI, RGB_M_P, RGB_M_B, REACTIVE_SIMPLE_RGB, TG(LAST_LAYER)
	),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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