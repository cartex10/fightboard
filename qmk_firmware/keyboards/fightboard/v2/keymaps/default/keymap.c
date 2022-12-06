#include QMK_KEYBOARD_H

void keyboard_post_init_user(void) {
	// Customize these values to desired behaviour
	debug_enable=true;
	debug_matrix=true;
	debug_keyboard=true;
	//debug_mouse=true;
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
	[0] = LAYOUT(
		KC_J, KC_K, KC_L, KC_SCLN, KC_D, KC_S, KC_A, KC_W, KC_MINS, KC_EQL, KC_BSPC, KC_ENT, KC_U, KC_I, KC_O, KC_P, TG(1)
	),
	[1] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, RGB_HUI, RGB_VAD, RGB_HUD, RGB_VAI, KC_LEFT_BRACKET, KC_RIGHT_BRACKET, KC_BSPC, KC_ESC, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, TG(1)
	),
};
