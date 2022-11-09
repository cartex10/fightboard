#include QMK_KEYBOARD_H

void keyboard_post_init_user(void) {
    // Customize these values to desired behaviour
    //debug_enable=true;
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     *             │ - │ = │BCK│ESC│
     *     ┌───┐   ┌───┬───┬───┬───┐
     *     │ W │   │ U │ I │ O │ P │
     * ┌───┼───┼───┼───┼───┼───┼───┤
     * │ A │ S │ D │ J │ K │ L │ ; │
     * └───┴───┴───┴───┴───┴───┴───┘
     *               KC_MINS, KC_EQL, KC_BSPC, KC_ESC,
     *       KC_W,       KC_U, KC_I, KC_O, KC_P,
     *  KC_A, KC_S, KC_D, KC_J, KC_K, KC_L, KC_SCLN
     *
     */
    [0] = LAYOUT(
        KC_W, KC_D, KC_S, KC_A, KC_SCLN, KC_L, KC_K, KC_J, KC_P, KC_O, KC_I, KC_U, KC_MINS, KC_EQL, KC_BSPC, KC_ESC
    )
};