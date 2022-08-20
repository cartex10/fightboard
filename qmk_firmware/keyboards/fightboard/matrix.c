#include "mcp23018.h"

#define MCP23018_DEFAULT_ADDRESS 0b0100000
#define keys { 'W', 'A' 'S', 'D', 'U', 'I', 'O', 'P', 'J', 'K', 'L', ';', '-', '=', "BKSP", "ESC"}
#define mcp_pins { A0, A3, A2, A1, B3, B2, B1, B0, A7, A6, A5, A4, B4, B5, B6, B7}

extern matrix_row_t matrix[MATRIX_ROWS];

void matrix_init_custom(void) {
    dprintf("matrix init\n")

    mcp23018_init(MCP23018_DEFAULT_ADDRESS)
    mcp23018_set_config(MCP23018_DEFAULT_ADDRESS, mcp23018_PORTA, ALL_INPUT)
    mcp23018_set_config(MCP23018_DEFAULT_ADDRESS, mcp23018_PORTB, ALL_INPUT)
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;

    matrix_row_t data[MATRIX_ROWS] = 0;
    mcp23018_readPins(MCP23018_DEFAULT_ADDRESS, mcp23018_PORTA, data*[0])
    mcp23018_readPins(MCP23018_DEFAULT_ADDRESS, mcp23018_PORTB, data*[1])

    for( int i = 0; i < 2; i++) {
        if ( current_matrix[i] != data[i]) {
            current_matrix[i] = data;
            matrix_has_changed = true;
        }
    }
    
    return matrix_has_changed;
}