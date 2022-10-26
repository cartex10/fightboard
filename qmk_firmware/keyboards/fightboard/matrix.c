#include QMK_KEYBOARD_H
#include "mcp23018.h"
#include "i2c_master.h"
#include "print.h"

#define MCP_ADDR (0x20)
//#define mcp_keys { 'W', 'A', 'S', 'D', 'U', 'I', 'O', 'P', 'J', 'K', 'L', ';', '-', '=', "BKSP", "ESC"}
//#define mcp_pins { A0 , A3 , A2 , A1 , B3 , B2 , B1 , B0 , A7 , A6 , A5 , A4 , B4 , B5 ,   B6  ,   B7 }

extern matrix_row_t matrix[MATRIX_ROWS];
extern rgblight_config_t rgblight_config;
int LEDHoldCount = 0;

void matrix_init_custom(void) {
    setPinOutput(GP27);
    writePinLow(GP27);
    wait_ms(100);
    togglePin(GP27);

    mcp23018_init(MCP_ADDR);
    mcp23018_set_config(MCP_ADDR, mcp23018_PORTA, ALL_INPUT);
    mcp23018_set_config(MCP_ADDR, mcp23018_PORTB, ALL_INPUT);
    setPinInputHigh(GP28);
}

bool matrix_scan_custom(uint16_t current_matrix[]) {
    bool matrix_has_changed = false;

    uint16_t mcpPins = 0x0000;
    mcp23018_readPins_all(MCP_ADDR, &mcpPins);
    mcpPins = ~mcpPins;

    bool LEDpin = readPin(GP28);
    LEDpin = !LEDpin;

    for (int row = 0; row < 2; row++) {
        int temp;
        switch (row) {
            case 0:
                temp = mcpPins;
                break;
            case 1:
                temp = LEDpin;
                break;
        }
        if (current_matrix[row] != temp) {
            current_matrix[row] = temp;
            matrix_has_changed = true;
        }
        //uprintf("Row #%d = %x\n", row, temp);
    }
    
    if (LEDpin) { LEDHoldCount += 1; }
    else { LEDHoldCount = 0; }
    if (LEDHoldCount == 1000) { 
        rgblight_toggle_noeeprom();
        if (rgblight_config.enable == 1) {
            //for (int LEDnum = 0; LEDnum <= RGBLED_NUM; LEDnum++) {
            //                }
        rgblight_setrgb(RGB_ORANGE);
        }
        eeconfig_debug_rgblight();
    }

    //uprintf("current_matrix = %x\n\n\n\n", *current_matrix);
    return matrix_has_changed;
}