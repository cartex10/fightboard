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
     *     │LED│   │ [ │ ] │BCK│ESC│
     *     ┌───┐   ┌───┬───┬───┬───┐
     *     │ T │   │ 4 │ 5 │ 6 │ + │    Non letters are using
     * ┌───┼───┼───┼───┼───┼───┼───┤    keypad variants to
     * │ F │ G │ H │ 1 │ 2 │ 3 │ENT│    differentiate w/ P1
     * └───┴───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(
        KC_T, KC_H, KC_G, KC_F, KC_PENT, KC_P3, KC_P2, KC_P1, KC_PPLS, KC_P6, KC_P5, KC_P4, KC_LEFT_BRACKET, KC_RIGHT_BRACKET, KC_BSPC, KC_ESC, TG(1)
    ),
    [1] = LAYOUT(
        RGB_VAI, RGB_HUI, RGB_VAD, RGB_HUD, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_G, RGB_M_TW, RGB_M_R, RGB_M_B, RGB_M_P, KC_LEFT_BRACKET, KC_RIGHT_BRACKET, KC_BSPC, KC_ESC, TG(1) 
    ),
};