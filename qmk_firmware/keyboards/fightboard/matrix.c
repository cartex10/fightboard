#include QMK_KEYBOARD_H
#include "mcp23018.h"
#include "i2c_master.h"
#include "print.h"

#define I2C_ADDR (0x20)
#define keys { 'W', 'A' 'S', 'D', 'U', 'I', 'O', 'P', 'J', 'K', 'L', ';', '-', '=', "BKSP", "ESC"}
#define mcp_pins { A0, A3, A2, A1, B3, B2, B1, B0, A7, A6, A5, A4, B4, B5, B6, B7}

extern matrix_row_t matrix[MATRIX_ROWS];

void matrix_init_custom(void) {
    setPinOutput(GP27);
    writePinLow(GP27);
    wait_ms(250);
    togglePin(GP27);

    mcp23018_init(I2C_ADDR);
    mcp23018_set_config(I2C_ADDR, mcp23018_PORTA, ALL_INPUT);
    mcp23018_set_config(I2C_ADDR, mcp23018_PORTB, ALL_INPUT);
}

bool matrix_scan_custom(uint16_t current_matrix[]) {
    bool matrix_has_changed = false;
    
    uint16_t data = 0x0000;
    mcp23018_readPins_all(I2C_ADDR, &data);
    data = ~data;
    uprintf("data = %x\n", data);
    uprintf("current_matrix = %x\n", *current_matrix);
    if (current_matrix[0] != data) {
        current_matrix[0] = data;
        matrix_has_changed = true;
    }
    print("\n\n\n\n");
    return matrix_has_changed;
}