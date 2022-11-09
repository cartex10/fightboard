#include QMK_KEYBOARD_H
#include "mcp23018.h"
#include "i2c_master.h"
#ifndef NO_PRINT
#include "print.h"
#endif

#define MCP_ADDR (0x20)
//#define mcp_keys { 'W', 'A', 'S', 'D', 'U', 'I', 'O', 'P', 'J', 'K', 'L', ';', '-', '=', "BKSP", "ESC"}
//#define mcp_pins { A0 , A3 , A2 , A1 , B3 , B2 , B1 , B0 , A7 , A6 , A5 , A4 , B4 , B5 ,   B6  ,   B7 }

extern matrix_row_t matrix[MATRIX_ROWS];

void matrix_init_custom(void) {
    setPinOutput(GP27);
    writePinLow(GP27);
    wait_ms(100);
    togglePin(GP27);

    mcp23018_init(MCP_ADDR);
    mcp23018_set_config(MCP_ADDR, mcp23018_PORTA, ALL_INPUT);
    mcp23018_set_config(MCP_ADDR, mcp23018_PORTB, ALL_INPUT);
}

bool matrix_scan_custom(uint16_t current_matrix[]) {
    bool matrix_has_changed = false;

    uint16_t mcpPins = 0x0000;
    mcp23018_readPins_all(MCP_ADDR, &mcpPins);
    mcpPins = ~mcpPins;

#ifndef NO_PRINT
    uprintf("Row #%d = %x\n", row, mcpPins);
    uprintf("current_matrix = %x\n\n\n\n", *current_matrix);
#endif

    if (current_matrix[0] != mcpPins) {
        current_matrix[0] = mcpPins;
        matrix_has_changed = true;
    }

    return matrix_has_changed;
}