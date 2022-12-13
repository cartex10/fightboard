#include QMK_KEYBOARD_H
#include "mcp23018.h"
#include "i2c_master.h"
#include <math.h>
#ifndef NO_PRINT
#include "print.h"
#endif

#define MCP_ADDR (0x20)
//#define mcp_keys { 'W', 'A', 'S', 'D', 'U', 'I', 'O', 'P', 'J', 'K', 'L', ';', '-', '=', "BKSP", "ESC"}
//#define mcp_pins { A7 , A6 , A5 , A4 , B4 , B5 , B6 , B7 , A0 , A1 , A2 , A3 , B0 , B1 ,   B2  ,   B3 }

extern matrix_row_t matrix[MATRIX_ROWS];

int LEDHoldCount = 0;
#ifdef SOCD
bool vSOCD = true;
bool hSOCD = true;
#endif

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
				#ifdef SOCD
				if (((temp & 0x00a0) == 0x00a0) && vSOCD) { // SOCD for vertical inputs
					temp = (temp & ~(current_matrix[row] & 0x00a0));
					vSOCD = false;
				}
				else if (((temp & 0x00a0) == 0x00a0)) { temp = temp & (~(~current_matrix[row] & 0x00a0)); }
				else { vSOCD = true; }
				if (((temp & 0x0050) == 0x0050) && hSOCD) { // SOCD for horizontal inputs
					temp = temp & (~(current_matrix[row] & 0x0050));
					hSOCD = false;
				}
				else if (((temp & 0x0050) == 0x0050)) { temp = temp & (~(~current_matrix[row] & 0x0050)); }
				else { hSOCD = true; }
				#endif
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
	if (LEDHoldCount == 1500) { rgb_matrix_toggle(); }
	
	//uprintf("current_matrix = %x\n\n\n\n", *current_matrix);
	return matrix_has_changed;
}


led_config_t g_led_config = { {
	// Key Matrix to LED Index
	{   0,   1,   2,   3,   4,   5,   6,   7, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED }
}, {
	// LED Index to Physical Position
	//x = 224 / (NUMBER_OF_COLS - 1) * COL_POSITION
	//y =  64 / (NUMBER_OF_ROWS - 1) * ROW_POSITION
	{ 0,  32 }, { 32,  32 }, { 64,  32 }, { 96,  32 }, { 128,  32 }, { 160,  32 }, { 192,  32 }, { 224,  32 }
}, {
	// LED Index to Flag
	LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_INDICATOR
} };

void rgb_matrix_indicators_kb(void) {
	switch(get_highest_layer(layer_state|default_layer_state)) {
		case 2:		// Unused layer (for now)
			rgb_matrix_set_color(7, RGB_BLUE);
			break;
		case 1:		//LED layer
			rgb_matrix_set_color(7, RGB_YELLOW);
			break;
		default:	//default layer
			rgb_matrix_set_color(7, RGB_BLACK);
			break;
	}
}