#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     *     ┌───┐   ┌───┬───┬───┬───┐
     *     │ W │   │ U │ I │ O │ P │
     * ┌───┼───┼───┼───┤───┼───┴───┘
     * │ A │ S │ D │ J │ K │
     * └───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(
        KC_W, KC_U, KC_I, KC_O, KC_P,
        KC_A, KC_S, KC_D, KC_J, KC_K
    )
};
